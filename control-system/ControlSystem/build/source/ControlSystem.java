import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 
import cc.arduino.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class ControlSystem extends PApplet {

// TODO: figure out how to draw moving block so that you can use to test movement L 80


/*
arduino_input

Demonstrates the reading of digital and analog pins of an Arduino board
running the StandardFirmata firmware.

To use:
* Using the Arduino software, upload the StandardFirmata example (located
  in Examples > Firmata > StandardFirmata) to your Arduino board.
* Run this sketch and look at the list of serial ports printed in the
  message area below. Note the index of the port corresponding to your
  Arduino board (the numbering starts at 0).  (Unless your Arduino board
  happens to be at index 0 in the list, the sketch probably won't work.
  Stop it and proceed with the instructions.)
* Modify the "arduino = new Arduino(...)" line below, changing the number
  in Arduino.list()[0] to the number corresponding to the serial port of
  your Arduino board.  Alternatively, you can replace Arduino.list()[0]
  with the name of the serial port, in double quotes, e.g. "COM5" on Windows
  or "/dev/tty.usbmodem621" on Mac.
* Run this sketch. The squares show the values of the digital inputs (HIGH
  pins are filled, LOW pins are not). The circles show the values of the
  analog inputs (the bigger the circle, the higher the reading on the
  corresponding analog input pin). The pins are laid out as if the Arduino
  were held with the logo upright (i.e. pin 13 is at the upper left). Note
  that the readings from unconnected pins will fluctuate randomly.

For more information, see: http://playground.arduino.cc/Interfacing/Processing
*/




Arduino arduino;
LiquidHandlingRobot robot;
Motor mX, mY, mZ;

int off = color(4, 79, 111);
int on = color(84, 145, 158);

public void setup() {
    

    // Prints out the available serial ports.
    println(Arduino.list());

    // Modify this line, by changing the "0" to the index of the serial
    // port corresponding to your Arduino board (as it appears in the list
    // printed by the line above).
    arduino = new Arduino(this, Arduino.list()[PORT], 57600);

    // Alternatively, use the name of the serial port corresponding to your
    // Arduino (in double-quotes), as in the following line.
    //arduino = new Arduino(this, "/dev/tty.usbmodem621", 57600);

    // Set the Arduino digital pins as inputs.
    for (int i = 0; i <= 13; i++)
        arduino.pinMode(i, Arduino.INPUT);


    TrackSwitchArray tsa = new TrackSwitchArray(arduino,
                                                HX_PIN, HY_PIN, HZ_PIN,
                                                EX_PIN, EY_PIN, EZ_PIN);
    mX = new Motor(arduino, "X", 12, 13);
    mY = new Motor(arduino, "Y", 10, 11);
    mZ = new Motor(arduino, "Z", 6, 7);
    robot = new LiquidHandlingRobot(arduino, mX, mY, mZ, tsa, 300, 300, 10);
}

public void draw() {
    background(50);
    stroke(on);

    if (CALIBRATING) {
        if (robot.calibrate()) CALIBRATING = false;
    }

    robot.display();


    // TODO: figure out how to draw moving block so that you can use to test movement

    // // Draw a filled box for each digital pin that's HIGH (5 volts).
    // for (int i = 0; i <= 13; i++) {
    //     if (arduino.digitalRead(i) == Arduino.HIGH)
    //       fill(on);
    //     else
    //       fill(off);
    //
    //       rect(420 - i * 30, 30, 20, 20);
    // }
    //
    // // Draw a circle whose size corresponds to the value of an analog input.
    // noFill();
    // for (int i = 0; i <= 5; i++) {
    //     ellipse(280 + i * 30, 240, arduino.analogRead(i) / 16, arduino.analogRead(i) / 16);
    // }
}
public final int PORT = 3;      // Ryan's Macbook pro LHS USB Port

public final int HX_PIN = 0;    // Switch pin numbers
public final int HY_PIN = 1;
public final int HZ_PIN = 2;
public final int EX_PIN = 3;
public final int EY_PIN = 4;
public final int EZ_PIN = 5;

public final boolean NO_HARDWARE_MODE = false;
public boolean IS_MOVING = false;
public boolean CALIBRATING = true;

public final int ROBOT_COLOR = color(0, 130, 126);
public final PVector ROBOT_DRAW_SIZE = new PVector(100, 100);
public final PVector ROBOT_DRAW_LOCATION = new PVector (40, 40);
public final float MM_TO_PIXEL_CONVERSION = 0.0000001f;


public final int MOT_DELAY = 2;


public int a2d (int a) {
    if (a < 511) return 0;
    else return 1;
}
// HACK: build a tester for the eventual state machine

/**
 * LIQUID HANDLING ROBOT
 * Object representing machine. Has ability to control its own moving parts, namely
 * x, y, z and its Pipette mechanism.
 */
class LiquidHandlingRobot {

    private float xMax, yMax, zMax;             // millimeter ranges for machine
    private int[] sMax;                         // Step motion ranges for machine
    private float xmmps, ymmps, zmmps;          // millis per step
    private PVector pos;                        // Position of robot. 3D Vector. Step position

    private Motor motors[];         // Motors corresponding to x, y, z
    private TrackSwitchArray tsa;   // Array containing values for the switches on the track
    private Pipette pipette;        // Pipette attached to center piece

    private PVector cSize = ROBOT_DRAW_SIZE;          // Size of liquid handler center piece on draw display
    private PVector drawLocation = ROBOT_DRAW_LOCATION;   // Location to draw the robot

    LiquidHandlingRobot(Arduino arduino, Motor mX, Motor mY, Motor mZ,
                        TrackSwitchArray tsa, float xMax, float yMax, float zMax) {
        this.xMax = xMax;
        this.yMax = yMax;
        this.zMax = zMax;
        this.sMax = new int[] {0, 0, 0};
        this.motors = new Motor[] {mX, mY, mZ};
        this.tsa = tsa;
        pos = new PVector(0, 0, 0);

        // HACK: remove when done testing movement
        if (NO_HARDWARE_MODE) pos = new PVector(10, 10, 15);

        // calibrate();
        println("TODO: Hardware not integrated yet. Connect motor pins etc");
        println("Robot initialized.");

        // TODO: Pipette
    }

    public boolean calibrate() {
        // Move motors to home positions
        if (!returnToOrigin()) return false;
        String switches[] = new String[] {"ex", "ey", "ez"};
        boolean done = true;

        for (int i = 0; i < 3; i++) {
            if (tsa.getState(switches[i]) == 0) {
                motors[i].step(1);
                sMax[i]++;
                done = false;
            }
        }


        // for (;;)
        // {
        //     boolean moving = false;
        //     for (int i = 0; i < 3; i++)
        //     {
        //         if (tsa.getState(switches[i]) == 0) {
        //             motors[i].step(1);
        //             c[i]++;
        //             moving = true;
        //         }
        //     }
        //     if (!moving) break;
        //
        //     // HACK: remove this break as it just prevents infinite loop.
        //     // Safe to remove once motors and switches are connected
        //     break;
        //

        if (NO_HARDWARE_MODE) {
            done = true;
            sMax = new int[] {100, 100, 100};
        }

        if (done) {
            xmmps = xMax / sMax[0];
            ymmps = yMax / sMax[1];
            zmmps = zMax / sMax[2];

            // Output calibration results
            println(
                "\n=================== CALIBRATION RESULTS =================== \n" +
                "\n" +
                "MM -> Step Conversion: \n" +
                "\t" + "xmmps: " + xmmps + "\n" +
                "\t" + "ymmps: " + ymmps + "\n" +
                "\t" + "zmmps: " + zmmps + "\n" +
                "\n" +
                "Maximum Steps: \n" +
                "\t" + "xsMax: " + sMax[0] + "\n" +
                "\t" + "ysMax: " + sMax[1] + "\n" +
                "\t" + "zsMax: " + sMax[2] + "\n" +
                "\n"
            );
        }

        return done;
    }

    /**
     * Returns pipette to pos (0, 0, 0).
     * Continues to move pipette motors backwards (towards zero) until the home switches are
     * actuated. Sets position to (0,0,0) and allows GUI to render device on screen. NOTE:
     * cannot render virtual device until physical device is confirmed at location.
     * @return true if at origin. If not, returns false and loop runs again
     */
    public boolean returnToOrigin() {

        // Use boolean to check if at origin.
        // TODO: insert digital writes to motors here.
        boolean atOrigin = false;
        if (tsa.getState("hx") == 0) motors[0].step(-1);
        if (tsa.getState("hy") == 0) motors[1].step(-1);
        if (tsa.getState("hz") == 0) motors[2].step(-1);

        if ( (tsa.getState("hx") == 1 &&
             tsa.getState("hy") == 1 &&
             tsa.getState("hz") == 1) ||
             NO_HARDWARE_MODE
           )
        {
            atOrigin = true;
            pos = new PVector(0,0,0);
        }

        println("Returning to origin - pos:" + pos);

        if (atOrigin) {
            motors[0].dirPositive();
            motors[1].dirPositive();
            motors[2].dirPositive();
        }

        return atOrigin;
    }

    /**
     * Set the position of the robot centerpiece.
     * @param  x
     * @param  y
     * @param  z
     * @return   if at desired position. i.e. movevector is 0
     */
    public boolean setPosition(float x, float y, float z) {

        // New Position in steps
        PVector newPos = new PVector (PApplet.parseInt(x * 1/xmmps), PApplet.parseInt(y * 1/ymmps), PApplet.parseInt(z * 1/zmmps));
        if (!posInRange(newPos)) return false;
        println("Requested move is in range");
        PVector move = PVector.sub(newPos, pos);
        println(move);

        // Calculate step vector: Mod = x/ abs(x)
        PVector step = new PVector(move.x/abs(move.x), move.y/abs(move.y), move.z/abs(move.z));
        println("Step: " + step);

        // Move to new position

        return true;
    }

    /**
     * Checks to see if new steps vector p is within range
     * @param  p : new vector
     * @return   true if in range
     */
    private boolean posInRange(PVector p) {
        return (p.x <= sMax[0] &&
                p.y <= sMax[1] &&
                p.z <= sMax[2]);
    }

    public void draw() {
        display();
    }

    public void display() {
        background(50);
        fill(ROBOT_COLOR);
        rect( PApplet.parseInt(drawLocation.x + pos.x * MM_TO_PIXEL_CONVERSION),
              PApplet.parseInt(drawLocation.y + pos.y * MM_TO_PIXEL_CONVERSION),
              cSize.x, cSize.y);
    }




}

/**
 * PIPETTE
 * The liquid handling pipette
 */
class Pipette {

    Pipette() {
        println("Liquid Handling Robot: Pipette initialized.");
    }
}


class Motor {

    Arduino a;
    String id;
    int stepPin;
    int dirPin;

    private boolean y = false;  // indicates if this motor is y and needs coordinate
                                // adjustment.

    Motor(Arduino a, String id, int sPin, int dPin) {
        this.a = a;
        this.id = id;
        this.stepPin = sPin;
        this.dirPin = dPin;
        this.a.pinMode(sPin, Arduino.OUTPUT);
        this.a.pinMode(dPin, Arduino.OUTPUT);
        this.a.digitalWrite(dPin, Arduino.LOW);
        println("Motor " + id + " initialized.");
    }

    public void step(int i) {
        println("Motor " + id + " stepping: " + i);
        step_motor(i);
    }

    private void step_motor(int i) {
        // TODO: link with hardware.
        a.digitalWrite(stepPin, Arduino.HIGH);
        delay(MOT_DELAY);
        a.digitalWrite(stepPin, Arduino.LOW);
    }

    public void dirPositive() {
        a.digitalWrite(dirPin, Arduino.HIGH);
    }


}

class TrackSwitchArray {

    int hxPin, hyPin, hzPin;    // Home pins indicating 0
    int exPin, eyPin, ezPin;    // End pins indicating MAX


    TrackSwitchArray(Arduino a, int hx, int hy, int hz, int ex, int ey, int ez) {
        hxPin = hx;
        hyPin = hy;
        hzPin = hz;
        exPin = ex;
        eyPin = ey;
        ezPin = ez;

        // a.pinMode(hxPin, Arduino.INPUT_PULLUP);
        // a.pinMode(hyPin, Arduino.INPUT_PULLUP);
        // a.pinMode(hzPin, Arduino.INPUT_PULLUP);
        // a.pinMode(exPin, Arduino.INPUT_PULLUP);
        // a.pinMode(eyPin, Arduino.INPUT_PULLUP);
        // a.pinMode(ezPin, Arduino.INPUT_PULLUP);

    }

    public int getState(String s) {
        int pin;
        switch (s) {
            case "hx":
                pin = hxPin;
                break;
            case "ex":
                pin = exPin;
                break;
            case "hy":
                pin = hyPin;
                break;
            case "ey":
                pin = eyPin;    // buttons
                break;
            case "hz":
                pin = hzPin;
                break;
            case "ez":
                pin = ezPin;    // buttons
                break;
            default:
                // if an incorrect code is given, send a message to console and
                // send -1, signaling invalid code
                pin = -1;
                return -1;
        }



        // TODO: Verify that triggering the switch writes the value 1
        if (pin == eyPin || pin == ezPin) return (a2d(arduino.analogRead(pin)) == 1) ? 1:0;
        return (a2d(arduino.analogRead(pin)) == Arduino.HIGH) ? 1:0;
    }
}

  public void settings() {  size(940, 560); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "ControlSystem" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
