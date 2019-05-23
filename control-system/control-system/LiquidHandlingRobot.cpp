#include <Arduino.h>
#include "LiquidHandlingRobot.h"
#include "globals.h"


LiquidHandlingRobot::LiquidHandlingRobot(StepperMotor xMotor, StepperMotor yMotor, StepperMotor zMotor,
    Pipette& p, float xMaxMM, float yMaxMM, float zMaxMM)
  : m_xMotor(xMotor), m_yMotor(yMotor), m_zMotor(zMotor)
{
  m_xMotor = xMotor;
  m_yMotor = yMotor;
  m_zMotor = zMotor;
  m_motors[0] = &m_xMotor;
  m_motors[1] = &m_yMotor;
  m_motors[2] = &m_zMotor;
  m_pip = &p;

  m_homed = false;

  m_xMaxSteps, m_yMaxSteps, m_zMaxSteps = 0;
  m_xMaxMM = xMaxMM;
  m_yMaxMM = yMaxMM;
  m_zMaxMM = zMaxMM;
}

void LiquidHandlingRobot::allPinsDir(bool value) {
  for (int i = 0; i < 3; i++)
    digitalWrite(m_motors[i]->pinDir(), value);
}

void LiquidHandlingRobot::allPinsStep(bool xValue, bool yValue, bool zValue) {
  bool values[] = {xValue, yValue, zValue};
  for (int i = 0; i < 3; i++)
    digitalWrite(m_motors[i]->pinStep(), values[i]);
}

void LiquidHandlingRobot::allPinsStep(bool value){
  for (int i = 0; i < 3; i++)
    digitalWrite(m_motors[i]->pinStep(), value);
}

void LiquidHandlingRobot::goToHome() {

  for (int i = 0; i < 3; i++)
    m_motors[i]->dest = 0;

  allPinsDir(LOW);

  while (!m_motors[2]->isAtHome()) {
    digitalWrite(m_motors[2]->pinStep(), !m_motors[2]->isAtHome());
    delayMicroseconds(PULSE_DURATION_Z);
    digitalWrite(m_motors[2]->pinStep(), LOW);

    if (m_motors[2]->isAtHome()) {
      m_pos[2] = 0;
      m_motors[2]->updatePos();
    }
  }

  while (!m_motors[0]->isAtHome() || !m_motors[1]->isAtHome()) {
    for (int i = 0; i < 2; i++)
      digitalWrite(m_motors[i]->pinStep(), !m_motors[i]->isAtHome());

    delayMicroseconds(PULSE_DURATION_XY);
    allPinsStep(LOW);

    for (int i = 0; i < 2; i++) {
      if (m_motors[i]->isAtHome()) {
        m_pos[i] = 0;
        m_motors[i]->updatePos();
      }
    }
  }

  m_homed = true;
  allPinsDir(HIGH);
}

void LiquidHandlingRobot::calibratePipette() {
  m_pip->dispenseAll();
}

boolean LiquidHandlingRobot::calibrate() {
  if (!m_homed) {
    goToHome();
    m_xMaxSteps, m_yMaxSteps, m_zMaxSteps = 0;
  }

  if (!m_motors[0]->isAtMax()) {
    digitalWrite(m_xMotor.pinStep(), HIGH);
    m_xMaxSteps++;
  }

  if (!m_motors[1]->isAtMax()) {
    digitalWrite(m_motors[1]->pinStep(), HIGH);
    m_yMaxSteps++;
  }

  if (!m_motors[2]->isAtMax() && m_motors[0]->isAtMax() && m_motors[1]->isAtMax()) {
    digitalWrite(m_motors[2]->pinStep(), HIGH);
    m_zMaxSteps++;
  }

  if(m_motors[0]->isAtMax() && m_motors[1]->isAtMax())
    delayMicroseconds(PULSE_DURATION_Z);
  else
    delayMicroseconds(PULSE_DURATION_XY);
  
  allPinsStep(LOW);


  if (m_motors[0]->isAtMax() && m_motors[1]->isAtMax() && m_motors[2]->isAtMax())
  {
    m_xMotor.millisPerStep = m_xMaxMM / m_xMaxSteps;
    m_yMotor.millisPerStep = m_yMaxMM / m_yMaxSteps;
    m_zMotor.millisPerStep = m_zMaxMM / m_zMaxSteps;
    return true;
  }
  return false;
}

void LiquidHandlingRobot::showMaxSteps() {
  Serial.print(m_xMaxSteps); Serial.print(' ');
  Serial.print(m_yMaxSteps); Serial.print(' ');
  Serial.println(m_zMaxSteps);
  Serial.print(m_xMotor.millisPerStep, 6); Serial.print(' ');
  Serial.print(m_yMotor.millisPerStep, 6); Serial.print(' ');
  Serial.println(m_zMotor.millisPerStep, 6);
}

bool LiquidHandlingRobot::goTo(float xDest, float yDest, float zDest) {
  if (xDest > SHAPEKO_XMAXMM || yDest > SHAPEKO_YMAXMM || zDest > SHAPEKO_ZMAXMM)
    return false;

  m_xMotor.dest = xDest;
  m_yMotor.dest = yDest;
  m_zMotor.dest = zDest;

  for (int i = 0; i < 3; i++)
    digitalWrite(m_motors[i]->pinDir(), (m_motors[i]->dest - m_motors[i]->pos()) > 0);

  if (zDest - z() < 0) { // Robot needs to move up
    while (!m_motors[2]->isAtDest()) {
      digitalWrite(m_motors[2]->pinStep(), !m_motors[2]->isAtDest());
      delayMicroseconds(PULSE_DURATION_Z);
      digitalWrite(m_motors[2]->pinStep(), LOW);
      updatePosAfterStep(false, false, true);
    }

    while (!m_motors[0]->isAtDest() || !m_motors[1]->isAtDest()) {
      allPinsStep(!m_motors[0]->isAtDest(), !m_motors[1]->isAtDest(), LOW);
      delayMicroseconds(PULSE_DURATION_XY);
      allPinsStep(LOW);
      updatePosAfterStep(true, true, false);
    }
  }
  else{
    while (!m_motors[0]->isAtDest() || !m_motors[1]->isAtDest()) {
      allPinsStep(!m_motors[0]->isAtDest(), !m_motors[1]->isAtDest(), LOW);
      delayMicroseconds(PULSE_DURATION_XY);
      allPinsStep(LOW);
      updatePosAfterStep(true, true, false);
    }

    while (!m_motors[2]->isAtDest()) {
      digitalWrite(m_motors[2]->pinStep(), !m_motors[2]->isAtDest());
      delayMicroseconds(PULSE_DURATION_Z);
      digitalWrite(m_motors[2]->pinStep(), LOW);
      updatePosAfterStep(false, false, true);
    }
  }

  delay(GOTO_DELAY);
//  showPos();
//  Serial.print(m_motors[0]->isAtDest()); Serial.print(' '); Serial.print(m_motors[1]->isAtDest()); Serial.print(' '); Serial.println(m_motors[2]->isAtDest());
//  Serial.print(m_motors[0]->dest); Serial.print(' '); Serial.print(m_motors[1]->dest); Serial.print(' '); Serial.println(m_motors[2]->dest);
//  Serial.println('-');

  return true;
}

float LiquidHandlingRobot::x() {
  return m_xMotor.pos();
}

float LiquidHandlingRobot::y() {
  return m_yMotor.pos();
}

float LiquidHandlingRobot::z() {
  return m_zMotor.pos();
}

void LiquidHandlingRobot::updatePosAfterStep(bool xMotor, bool yMotor, bool zMotor) {
  bool justStepped[] = {xMotor, yMotor, zMotor};
  for (int i = 0; i < 3; i++) {
    if (!m_motors[i]->isAtDest() && justStepped[i]) m_motors[i]->updatePosAfterStep();
  }
}

void LiquidHandlingRobot::updatePosAfterStep() {
  for (int i = 0; i < 3; i++) {
    if (!m_motors[i]->isAtDest()) m_motors[i]->updatePosAfterStep();
  }
}

void LiquidHandlingRobot::stepAll() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(m_motors[i]->pinStep(), !m_motors[i]->isAtDest());
  }

  delayMicroseconds(PULSE_DURATION_XY);
  allPinsStep(LOW);
  updatePosAfterStep();
}

void LiquidHandlingRobot::setDestAs(float xDest, float yDest, float zDest) {
  m_xMotor.dest = xDest;
  m_yMotor.dest = yDest;
  m_zMotor.dest = zDest;

  for (int i = 0; i < 3; i++)
    digitalWrite(m_motors[i]->pinDir(), (m_motors[i]->dest - m_motors[i]->pos()) > 0);
}

bool LiquidHandlingRobot::isAtDest() {
  return (m_xMotor.isAtDest() && m_yMotor.isAtDest() && m_zMotor.isAtDest());
}

void LiquidHandlingRobot::showPos() {
  Serial.print(m_xMotor.pos()); Serial.print(", ");
  Serial.print(m_yMotor.pos()); Serial.print(", ");
  Serial.println(m_zMotor.pos());
}

// // Pipette operations
//    void drawLiquid(float v);       // Draw up liquid according to volume v
//    void dispenseLiquid(float v);   // Dispense liquid according to volume v
//    void emptyLiquid();             // Dispense all liquid in waste


// Pipette Interface ===================================================

void LiquidHandlingRobot::dispenseLiquid(float v, float zPos) {

  float returnZ = z();

  // Apply zPos if not default value
  if (zPos == -1) zPos = z();
  goTo(x(), y(), zPos);

  // Push down pipette as long as there is enough volume as being requested
  if (v <= m_pip->getVolume() + m_pip->s2v()) m_pip->down(v);
  else {
    Serial.println("ERROR: Not enough fluid in pipette to dispense!");
    Serial.print(m_pip->getVolume());
    Serial.print(' ');
    Serial.println(v);
  }


  // Lift pipette to original position
  goTo(x(), y(), returnZ);

}

void LiquidHandlingRobot::drawLiquid(float v, float zPos) {

  float returnZ = z();

  // Apply zPos if not default value
  if (zPos == -1) zPos = z();
  goTo(x(), y(), zPos);

  // Pull up pipette as long as the requested volume to be drawn doesn't
  // exceed pipette's max volume capacity
  if (v <= m_pip->getMaxVolume()) m_pip->up(v);
  else Serial.println("ERROR: Not enough capacity to take up this much volume!");


  // Lift pipette to original position
  goTo(x(), y(), returnZ);

}

void LiquidHandlingRobot::emptyLiquidInWaste(float xTarget, float yTarget, float zTarget) {
  // go to x,y first, then drop down to z to dispense all
  goTo(xTarget, yTarget, z());
  goTo(x(), y(), zTarget);
  m_pip->dispenseAll();
}


void LiquidHandlingRobot::mix(float dv, float targetZ, int iterations) {
  if (targetZ <= 1) {
    Serial.println("ERROR: Cannot go below z = 1 to mix");
    return;
  }

  // Push pipette down to zero
  // m_pip->dispenseAll();

  // Move to targetZ position
  goTo(x(), y(), targetZ);

  // Perform mix operation
  for (int i = 0; i < iterations; i++) {
    m_pip->up(dv);
    m_pip->down(dv);
    delay(MIX_DELAY);
  }

}

void LiquidHandlingRobot::dispenseWaste() {
  float returnX = x();
  float returnY = y();
  float returnZ = z();

  goTo(WASTE_X_POS, WASTE_Y_POS, WASTE_Z_POS);

  m_pip->dispenseAll();

  goTo(returnX, returnY, returnZ);
}

void LiquidHandlingRobot::pipetteStep(int numSteps, char dir){
  if(dir == 'u') m_pip->dirUp();
  else m_pip->dirDown();

  for(int i = 0; i < numSteps; i++)
    m_pip->stepMotor();
}


