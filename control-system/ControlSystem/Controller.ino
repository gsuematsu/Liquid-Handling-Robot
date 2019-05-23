#include "LiquidHandlingRobot.h"

#define xScale 0.183
#define yScale 0.209
#define zScale 0.030

StepperMotor xMotor(8, 9, A0, A3);
StepperMotor yMotor(10, 11, A1, A4);
StepperMotor zMotor(12, 13, A2, A5);

MotorFrame lhr(xMotor, yMotor, zMotor);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lhr.goToHome();
  lhr.callibrate(xScale, yScale, zScale);
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(5000);
  lhr.goTo(100, 100, 100);


}
