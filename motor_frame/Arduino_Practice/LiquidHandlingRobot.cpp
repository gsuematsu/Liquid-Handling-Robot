#include <Arduino.h>
#include "LiquidHandlingRobot.h"

#define timeDelay 1700

StepperMotor::StepperMotor(int pinStep, int pinDir, int pinMinSwitch, int pinMaxSwitch) {
  pinMode(pinStep, OUTPUT);
  pinMode(pinDir, OUTPUT);
  pinMode(pinMinSwitch, INPUT_PULLUP);
  pinMode(pinMaxSwitch, INPUT_PULLUP);

  m_pinStep = pinStep;
  m_pinDir = pinDir;
  m_pinMinSwitch = pinMinSwitch;
  m_pinMaxSwitch = pinMaxSwitch;
  m_pos = 1;
  dest = 0;
  millisPerStep = 1;

}

float StepperMotor::pos() {
  return m_pos;
}

int StepperMotor::pinStep() {
  return m_pinStep;
}

int StepperMotor::pinDir() {
  return m_pinDir;
}

int StepperMotor::pinMinSwitch() {
  return m_pinMinSwitch;
}

int StepperMotor::pinMaxSwitch() {
  return m_pinMaxSwitch;
}

bool StepperMotor::isAtDest() {
  float error = abs(dest - m_pos);
  return error < millisPerStep;
}

bool StepperMotor::isAtHome(){
  return digitalRead(m_pinMinSwitch);
}

void StepperMotor::updatePos() {
  m_pos = dest;
}

void StepperMotor::justStepped() {
  int sign = 1;

  if (!digitalRead(m_pinDir)) {
    sign = -1;
  }
  m_pos += (sign * millisPerStep);
}

MotorFrame::MotorFrame(StepperMotor xMotor, StepperMotor yMotor, StepperMotor zMotor): m_xMotor(xMotor), m_yMotor(yMotor), m_zMotor(zMotor) {
  m_xMotor = xMotor;
  m_yMotor = yMotor;
  m_zMotor = zMotor;
  m_homed = false;

  m_xMaxSteps, m_yMaxSteps, m_zMaxSteps = 0;
  m_xMaxMM = 150;
  m_yMaxMM = 150;
  m_zMaxMM = 50;
}

void MotorFrame::allPinsDir(bool value) {
  digitalWrite(m_xMotor.pinDir(), value);
  digitalWrite(m_yMotor.pinDir(), value);
  digitalWrite(m_zMotor.pinDir(), value);
}

void MotorFrame::allPinsStep(bool value) {
  digitalWrite(m_xMotor.pinStep(), value);
  digitalWrite(m_yMotor.pinStep(), value);
  digitalWrite(m_zMotor.pinStep(), value);
}

void MotorFrame::goToHome() {
  m_xMotor.dest = 0;
  m_yMotor.dest = 0;
  m_zMotor.dest = 0;

  allPinsDir(LOW);

  while (!m_xMotor.isAtHome() || !m_yMotor.isAtHome() || !m_zMotor.isAtHome()) {
    digitalWrite(m_xMotor.pinStep(), !m_xMotor.isAtHome());
    digitalWrite(m_yMotor.pinStep(), !m_yMotor.isAtHome());
    digitalWrite(m_zMotor.pinStep(), !m_zMotor.isAtHome());

    delayMicroseconds(timeDelay);

    allPinsStep(LOW);

    if (digitalRead(m_xMotor.pinMinSwitch())) {
      m_pos[0] = 0;
      m_xMotor.updatePos();
    }
    if (digitalRead(m_yMotor.pinMinSwitch())) {
      m_pos[1] = 0;
      m_yMotor.updatePos();
    }
    if (digitalRead(m_zMotor.pinMinSwitch())) {
      m_pos[2] = 0;
      m_zMotor.updatePos();
    }
  }

  m_homed = true;
  allPinsDir(HIGH);
}

void MotorFrame::calibrate(float xScale, float yScale, float zScale) {
  m_xMotor.millisPerStep = xScale;
  m_yMotor.millisPerStep = yScale;
  m_zMotor.millisPerStep = zScale;
}

boolean MotorFrame::calibrate() {
  if (!m_homed) {
    goToHome();
    m_xMaxSteps = 0;
    m_yMaxSteps = 0;
    m_zMaxSteps = 0;
  }

  setDestAs(m_xMaxMM, m_yMaxMM, m_zMaxMM);  // HACK

  if (digitalRead(m_xMotor.pinMaxSwitch()) != 1) {
    digitalWrite(m_xMotor.pinStep(), HIGH);
    delayMicroseconds(timeDelay);
    digitalWrite(m_xMotor.pinStep(), LOW);
    m_xMaxSteps++;
  }

  if (digitalRead(m_yMotor.pinMaxSwitch()) != 0) {
    digitalWrite(m_yMotor.pinStep(), HIGH);
    delayMicroseconds(timeDelay);
    digitalWrite(m_yMotor.pinStep(), LOW);
    m_yMaxSteps++;
  }

  //  if (m_zMotor.pinMaxSwitch() != 1) {
  //    digitalWrite(m_zMotor.pinStep(), HIGH);
  //    delayMicroseconds(timeDelay);
  //    digitalWrite(m_zMotor.pinStep(), LOW);
  //    zMaxSteps++;
  //  }

  // TODO ADD Z CALIBRATION
  if (digitalRead(m_xMotor.pinMaxSwitch()) == 1 && digitalRead(m_yMotor.pinMaxSwitch()) == 0)
  {
    //      m_xmmps = m_xMaxMM / m_xMaxSteps;
    //      m_ymmps = m_yMaxMM / m_yMaxSteps;

    m_xMotor.millisPerStep = m_xMaxMM / m_xMaxSteps;
    m_yMotor.millisPerStep = m_yMaxMM / m_yMaxSteps;
    //m_zMotor.millisPerStep = zScale;

    return true;
  }

  return false;
}

void MotorFrame::showMaxSteps(){
  Serial.print(m_xMaxSteps); Serial.print(' ');
  Serial.print(m_yMaxSteps); Serial.print(' ');
  Serial.println(m_zMaxSteps);
  Serial.print(m_xMotor.millisPerStep); Serial.print(' ');
  Serial.print(m_yMotor.millisPerStep); Serial.print(' ');
  Serial.println(m_zMotor.millisPerStep);
}

void MotorFrame::goTo(float xDest, float yDest, float zDest) {
  m_xMotor.dest = xDest;
  m_yMotor.dest = yDest;
  m_zMotor.dest = zDest;

  digitalWrite(m_xMotor.pinDir(), (m_xMotor.dest - m_xMotor.pos()) > 0);
  digitalWrite(m_yMotor.pinDir(), (m_yMotor.dest - m_yMotor.pos()) > 0);
  digitalWrite(m_zMotor.pinDir(), (m_zMotor.dest - m_zMotor.pos()) > 0);

  while (!m_xMotor.isAtDest() || !m_yMotor.isAtDest() || !m_zMotor.isAtDest()) {
    digitalWrite(m_xMotor.pinStep(), !m_xMotor.isAtDest());
    digitalWrite(m_yMotor.pinStep(), !m_yMotor.isAtDest());
    digitalWrite(m_zMotor.pinStep(), !m_zMotor.isAtDest());

    delayMicroseconds(timeDelay);

    allPinsStep(LOW);

    if (!m_xMotor.isAtDest()) {
      m_xMotor.justStepped();
    }
    if (!m_yMotor.isAtDest()) {
      m_yMotor.justStepped();
    }
    if (!m_zMotor.isAtDest()) {
      m_zMotor.justStepped();
    }
  }
}

float MotorFrame::xPos() {
  return m_xMotor.pos();
}

float MotorFrame::yPos() {
  return m_yMotor.pos();
}

float MotorFrame::zPos() {
  return m_zMotor.pos();
}

void MotorFrame::stepAll() {

  digitalWrite(m_xMotor.pinStep(), !m_xMotor.isAtDest());
  digitalWrite(m_yMotor.pinStep(), !m_yMotor.isAtDest());
  digitalWrite(m_zMotor.pinStep(), !m_zMotor.isAtDest());

  delayMicroseconds(timeDelay);

  allPinsStep(LOW);

  if (!m_xMotor.isAtDest()) {
    m_xMotor.justStepped();
  }
  if (!m_yMotor.isAtDest()) {
    m_yMotor.justStepped();
  }
  if (!m_zMotor.isAtDest()) {
    m_zMotor.justStepped();
  }
}

void MotorFrame::setDestAs(float xDest, float yDest, float zDest) {
  m_xMotor.dest = xDest;
  m_yMotor.dest = yDest;
  m_zMotor.dest = zDest;

  digitalWrite(m_xMotor.pinDir(), (m_xMotor.dest - m_xMotor.pos()) > 0);
  digitalWrite(m_yMotor.pinDir(), (m_yMotor.dest - m_yMotor.pos()) > 0);
  digitalWrite(m_zMotor.pinDir(), (m_zMotor.dest - m_zMotor.pos()) > 0);
}

bool MotorFrame::isAtDest() {
  return (m_xMotor.isAtDest() && m_yMotor.isAtDest() && m_zMotor.isAtDest());
}

void MotorFrame::showPos(){
  Serial.print(m_xMotor.pos()); Serial.print(' ');
  Serial.print(m_yMotor.pos()); Serial.print(' ');
  Serial.println(m_zMotor.pos());
}


WellPlate96::WellPlate96(float xPos, float yPos){
  m_xPos = xPos;
  m_yPos = yPos;
  for(int i = 0; i < sizeof(m_volume)/sizeof(float); i++){
    m_volume[i] = 0;
  }
}

float WellPlate96::getVolume(int col){
  return m_volume[col-1];
}

void serialDilution(int dilutionFactor, float volume, WellPlate wellPlate){
  l
}

