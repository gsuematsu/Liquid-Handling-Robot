#include "Pipette.h"
#include "globals.h"
#include <Arduino.h>

Pipette::Pipette(float s2v, float maxV, int stepPin, int dirPin) {
  m_s2v = s2v;
  m_v2s = 1/s2v;
  m_stepPin = stepPin;
  m_dirPin = dirPin;
  m_maxVol = maxV; 
}

void Pipette::down(float volume, bool delayOn) {
  int s = volume * m_v2s;
  dirDown();
  for (int i = 0; i < s; i++) 
    stepMotor();
  
  if (delayOn) delay(WAIT_AFTER_PIPETTE);
}

void Pipette::up(float volume, bool delayOn) {
  int s = volume * m_v2s;
  dirUp();
  for (int i = 0; i < s; i++) 
    stepMotor();
  
  if (delayOn) delay(WAIT_AFTER_PIPETTE);
}

void Pipette::dispenseAll() {
  // TODO: Move motor all the way down to zero
  int s = m_maxVol * m_v2s + 100;   // steps to release all volume, plus extra 100 steps
  dirDown();
  for (int i = 0; i < s; i++)
    stepMotor();
  Serial.println("dispensed and reset position to zero");
  m_volume = 0;
  dirUp();
}

float Pipette::s2v() {
  return m_s2v;
}

float Pipette::v2s() {
  return m_v2s;
}

float Pipette::getVolume() {
  return m_volume;
}

float Pipette::getMaxVolume() {
  return m_maxVol;
}


// PRIVATE ==============================================

void Pipette::dirDown() {
  // TODO: verify direction pin 
  digitalWrite(m_dirPin, HIGH);
  m_dir = 1;
  
}

void Pipette::dirUp() {
  // TODO: verify direction pin
  digitalWrite(m_dirPin, LOW);
  m_dir = 0;
}

void Pipette::stepMotor() {
  // Serial.println("trying to step");
  digitalWrite(m_stepPin, HIGH);
  delayMicroseconds(PIPETTE_STEP_DELAY);
  digitalWrite(m_stepPin, LOW);

  if (m_dir == 0) {
    // if direction is up
    m_volume += m_s2v;
  } else {
    m_volume -= m_s2v;
  }
  
}




