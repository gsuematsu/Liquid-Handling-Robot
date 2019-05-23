#include <Servo.h>

int LED_PIN = 13;
char serialData;
bool ROTATE_TEST = true;
bool MOTOR_TEST = false;

// MOTOR TEST
// Speed: forwards slow => fast D:\Ryan\Dropbox\Projects\Malignancy Analyzer\app.py
//                  98  =>  250
// Speed: backward slow => fast 
//                  90  =>   0
int MOTOR_SPEED = 98;
int MOTOR_PIN = 11;

// SERVO TEST
int SERVO_PIN = 11;
int INIT_SERVO_POS = 70;
int SERVO_MAX_POS = 120;
int SERVO_MIN_POS = 70;
int DELAY_LENGTH = 5;

Servo my_servo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  if (ROTATE_TEST) {
    my_servo.attach(SERVO_PIN);
  }

  if (MOTOR_TEST) {
    my_servo.attach(MOTOR_PIN);
  }
}


int servo_pos = INIT_SERVO_POS;
bool ascending = true;

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0) {
    serialData = Serial.read();
    Serial.print(serialData);
    if (serialData == '0') blink();
  }

//  if (MOTOR_TEST) {
//    my_servo.write(MOTOR_SPEED);
//  }
//  
//  if (ROTATE_TEST) {
//    my_servo.write(servo_pos);
//    if (ascending) {
//      if (servo_pos == SERVO_MAX_POS) {
//        ascending = false;
//      }
//      servo_pos++;
//    } else 
//    {
//      if (servo_pos == SERVO_MIN_POS) {
//        ascending = true;
//      }
//      servo_pos--;
//    }
//    
//    delay(DELAY_LENGTH);
//
//  }

  delay(100);

}


void blink() {
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
}

void turn_left() {
  
}

void turn_right() {
  
}



