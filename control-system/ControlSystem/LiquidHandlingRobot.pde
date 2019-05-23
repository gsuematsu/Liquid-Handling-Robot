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

    LiquidHandlingRobot(Serial p, Motor mX, Motor mY, Motor mZ,
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

    boolean calibrate() {
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
    boolean returnToOrigin() {

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
    boolean setPosition(float x, float y, float z) {

        // New Position in steps
        PVector newPos = new PVector (int(x * 1/xmmps), int(y * 1/ymmps), int(z * 1/zmmps));
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

    void draw() {
        display();
    }

    void display() {
        background(50);
        fill(ROBOT_COLOR);
        rect( int(drawLocation.x + pos.x * MM_TO_PIXEL_CONVERSION),
              int(drawLocation.y + pos.y * MM_TO_PIXEL_CONVERSION),
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

    Serial p;
    String id;
    int stepPin;
    int dirPin;

    private boolean y = false;  // indicates if this motor is y and needs coordinate
                                // adjustment.

    Motor(Serial p, String id, int sPin, int dPin) {
        this.a = a;
        this.id = id;
        this.stepPin = sPin;
        this.dirPin = dPin;
        this.a.pinMode(sPin, Arduino.OUTPUT);
        this.a.pinMode(dPin, Arduino.OUTPUT);
        this.a.digitalWrite(dPin, Arduino.LOW);
        println("Motor " + id + " initialized.");
    }

    void step(int i) {
        println("Motor " + id + " stepping: " + i);
        step_motor(i);
    }

    private void step_motor(int i) {
        // TODO: link with hardware.
        a.digitalWrite(stepPin, Arduino.HIGH);
        delay(MOT_DELAY);
        a.digitalWrite(stepPin, Arduino.LOW);
    }

    void dirPositive() {
        a.digitalWrite(dirPin, Arduino.HIGH);
    }


}

class TrackSwitchArray {

    int hxPin, hyPin, hzPin;    // Home pins indicating 0
    int exPin, eyPin, ezPin;    // End pins indicating MAX


    TrackSwitchArray(Serial p, int hx, int hy, int hz, int ex, int ey, int ez) {
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

    int getState(String s) {
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
