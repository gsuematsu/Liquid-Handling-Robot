#include <Arduino.h>
#include "StepperMotor.h"


StepperMotor::StepperMotor(int pinStep, int pinDir, int pinMinSwitch, int pinMaxSwitch){
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

float StepperMotor::pos(){
  return m_pos;
}

int StepperMotor::pinStep(){
  return m_pinStep;
}

int StepperMotor::pinDir(){
  return m_pinDir;
}

int StepperMotor::pinMinSwitch(){
  return m_pinMinSwitch;
}

int StepperMotor::pinMaxSwitch(){
  return m_pinMaxSwitch;
}

bool StepperMotor::isAtDest() {
  float error = abs(dest - m_pos);
  return error < (0.5 * millisPerStep);
}

bool StepperMotor::isAtHome(){
  return digitalRead(m_pinMinSwitch);
}

bool StepperMotor::isAtMax(){
  return digitalRead(m_pinMaxSwitch);
}

void StepperMotor::updatePos(){
  m_pos = dest;
}

void StepperMotor::updatePosAfterStep() {
  float sign = 1;

  if (!digitalRead(m_pinDir)) {
    sign = -1;
  }
  m_pos += (sign * millisPerStep);
}

void StepperMotor::setPinDir(int i) {
  if (i == 0 || i == 1) {
    digitalWrite(m_pinDir, i);
  }
}
