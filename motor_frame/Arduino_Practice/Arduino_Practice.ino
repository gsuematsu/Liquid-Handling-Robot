#include "LiquidHandlingRobot.h"
#include "globals.h"

#define xScale 0.183
#define yScale 0.209
#define zScale 0.030

bool CALIBRATING = true;


StepperMotor xMotor(9, 8, A0, A3);
StepperMotor yMotor(11, 10, A1, A4);
StepperMotor zMotor(13, 12, A2, A5);

MotorFrame lhr(xMotor, yMotor, zMotor);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lhr.goToHome();
  lhr.calibrate(xScale, yScale, zScale);
  delay(100);

}

void loop() {
  // put your main code here, to run repeatedly:
  //
  if (CALIBRATING) {
    if (lhr.calibrate() == true) {
      CALIBRATING = false;
      lhr.showMaxSteps();
      delay(500);
    }
  }
  else {
    lhr.goToHome();
    //lhr.showPos();
    delay(300);
    //  for(int i = 0; i < 160; i += 10){
    //    for(int j = 0; j < 160; j += 10){
    //      lhr.goTo(i, j, 100);
    //      Serial.print(lhr.xPos()); Serial.print(' ');
    //      Serial.print(lhr.yPos()); Serial.print(' ');
    //      Serial.println(lhr.zPos());
    //      delay(800);
    //    }
    //  }

    drawX();

  }

}

void drawX() {
  lhr.setDestAs(50, 50, 0);
  while (!lhr.isAtDest()) {
    lhr.stepAll();
  }
  delay(300);
  lhr.setDestAs(150, 150, 0);
  while (!lhr.isAtDest()) {
    lhr.stepAll();
  }
  delay(300);
  lhr.setDestAs(150, 50, 0);
  while (!lhr.isAtDest()) {
    lhr.stepAll();
  }
  delay(300);
  lhr.setDestAs(50, 150, 0);
  while (!lhr.isAtDest()) {
    lhr.stepAll();
  }

  delay(1000);
  lhr.goToHome();
  delay(1000);
}

