#include "LiquidHandlingRobot.h"
#include "globals.h"




// bool CALIBRATING = true;

bool isFirstLoop = true;

String receivingString = "";
String commandString = "";
float commandParameters[4];


StepperMotor xMotor(X_MOTOR_STEP_PIN, X_MOTOR_DIR_PIN, X_MIN_SWITCH_PIN, X_MAX_SWITCH_PIN);
StepperMotor yMotor(Y_MOTOR_STEP_PIN, Y_MOTOR_DIR_PIN, Y_MIN_SWITCH_PIN, Y_MAX_SWITCH_PIN);
StepperMotor zMotor(Z_MOTOR_STEP_PIN, Z_MOTOR_DIR_PIN, Z_MIN_SWITCH_PIN, Z_MAX_SWITCH_PIN);
Pipette pip (PIPETTE_S2V, PIPETTE_MAX_VOL, PIPETTE_STEP_PIN, PIPETTE_DIR_PIN);

LiquidHandlingRobot lhr(xMotor, yMotor, zMotor, pip,
                        SHAPEKO_XMAXMM, SHAPEKO_YMAXMM, SHAPEKO_ZMAXMM);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  lhr.goToHome();
  delay(1000);

  while (CALIBRATING) {
    if (lhr.calibrate() == true) {
      CALIBRATING = false;
      lhr.showMaxSteps();
      delay(500);
    }
  }

  lhr.goToHome();
  delay(300);
  lhr.dispenseWaste();
  lhr.goToHome();
}

void loop() {
  // put your main code here, to run repeatedly:
  receiveSerialData();
  executeCommand();
  clearCommands();
}


void pipetteTester() {
  while (true) {
    lhr.goToHome();
    delay(300);
    //lhr.emptyLiquidInWaste(0, 0, 0);
    lhr.goTo(100, 100, 0);
    lhr.drawLiquid(STEPSORVOLTOFILL, 20);
    delay (100);
    lhr.goTo(50, 100, 0);
    lhr.dispenseLiquid(STEPSORVOLTOFILL, 20);
    delay (100);
  }
}

void serialDilution(float totalVol, float dilutionFactor) {
  lhr.goToHome();
  delay(300);
  lhr.dispenseWaste();
  int lastWellOffset = WELL_COLS * DISTANCE_BETWEEN_WELLS;

  if (dilutionFactor < 1) {
    Serial.println("Clearly, you don't know how to math... exiting (Dilution factor must be a number greater than 1");
    return;
  }

  float dilutantVol[12];
  dilutantVol[11] = totalVol - totalVol / dilutionFactor;

  for (int i = sizeof(dilutantVol) / sizeof(dilutantVol[0]) - 2; i >= 0; i--)
    dilutantVol[i] = (totalVol + dilutantVol[i + 1]) * (dilutionFactor - 1) / dilutionFactor;

  for (int i = 0; i < sizeof(dilutantVol) / sizeof(dilutantVol[0]); i++)
    Serial.println(dilutantVol[i]);


  if (dilutantVol[0]*dilutionFactor > MAX_WELL_VOLUME) {
    Serial.println("Well cannot hold that much liquid... try again");
    return;
  }

  lhr.goTo(LOAD_ORIGIN_X, LOAD_ORIGIN_Y, 0);

  for (int i = 0; i < 12; i ++) {
    // for each well, draw and dispense diluatant volume
    if (i != 0) lhr.goTo(lhr.x(), lhr.y(), LOAD_Z_CLEARANCE);
    lhr.goTo(LOAD_ORIGIN_X, LOAD_ORIGIN_Y, LOAD_Z_CLEARANCE);
    lhr.drawLiquid(dilutantVol[i], LOAD_POS_Z);
    
    lhr.goTo(WELL_ORIGIN_X, WELL_ORIGIN_Y + i * DISTANCE_BETWEEN_WELLS, lhr.z());        // move above well position to dispense in
    Serial.print(i + 1); Serial.print(' '); Serial.println(dilutantVol[i]);
    Serial.print("Should be at "); Serial.print(WELL_ORIGIN_X); Serial.print(", "); Serial.println(WELL_ORIGIN_Y + i * DISTANCE_BETWEEN_WELLS);
    Serial.print("Located at "); lhr.showPos();
    lhr.dispenseLiquid(dilutantVol[i], WELL_PLATE_Z_POS);
    
  }

  // Load solute into well, performing serial dilution
  float stockVol = dilutantVol[0];

  lhr.goTo(LOAD_ORIGIN_X, LOAD_ORIGIN_Y + LOAD_ORIGIN_OFFSET, LOAD_Z_CLEARANCE);  // move to above the stock load position
  lhr.drawLiquid(stockVol, LOAD_POS_Z);     // draw up stock solution
  lhr.goTo(WELL_ORIGIN_X, WELL_ORIGIN_Y, WELL_PLATE_Z_CLEARANCE);        // move above well position to dispense in
  lhr.dispenseLiquid(stockVol, WELL_PLATE_Z_POS);
  lhr.mix(0.5 * totalVol, WELL_PLATE_Z_POS + WELL_PLATE_Z_DRAW_OFFSET, 3);       // mix solution

  for (int i = 0; i < 11; i ++) {
    float transVol = dilutantVol[i + 1];
    lhr.goTo(WELL_ORIGIN_X, WELL_ORIGIN_Y + i * DISTANCE_BETWEEN_WELLS, WELL_PLATE_Z_CLEARANCE);        // move above well position to dispense in
    lhr.drawLiquid(transVol, WELL_PLATE_Z_POS + WELL_PLATE_Z_DRAW_OFFSET);

    lhr.goTo(WELL_ORIGIN_X, WELL_ORIGIN_Y + (i + 1) * DISTANCE_BETWEEN_WELLS, WELL_PLATE_Z_CLEARANCE);      // move above well position to dispense in
    lhr.dispenseLiquid(transVol, WELL_PLATE_Z_POS);           // dispense
    lhr.mix(0.5 * totalVol, WELL_PLATE_Z_POS + WELL_PLATE_Z_DRAW_OFFSET, 3);       // mix solution
  }

  lhr.goToHome();
}

void loadFluid() {
  lhr.goTo(LOAD_ORIGIN_X, LOAD_ORIGIN_Y, 0);              // move to above the load solution
  lhr.goTo(LOAD_ORIGIN_X, LOAD_ORIGIN_Y, LOAD_POS_Z);     // drop pipette into solution
  Serial.println("Loading fluid");
  // TODO LOAD FLUID DELAY
  lhr.goTo(LOAD_ORIGIN_X, LOAD_ORIGIN_Y, 0);              // lift pipette back up
}

void dispenseFluid() {

  float ox = lhr.x();
  float oy = lhr.y();
  float oz = lhr.z();

  lhr.goTo(ox, oy, WELL_DISPENSE_POS_Z);  // drop pipette into well
  Serial.println("Dispensing fluid");
  // TODO: FLUID DELAY
  lhr.goTo(ox, oy, 0);                    // lift pipette from well POTENTIALLY A PART OF THE DISPENSE CLASS
  // i.e. return from where you came from

}

void traceGrid() {
  lhr.goTo(lhr.x(), lhr.y(), 60);

  for (int i = 0; i <= SHAPEKO_XMAXMM; i += 50) {
    for (int j = 0; j <= SHAPEKO_YMAXMM; j += 50) {
      lhr.goTo(i, j, lhr.z());
      lhr.showPos();
      delay(10000);
    }
  }
}

void calibratePipette() {
  int steps = 2000;
  lhr.goToHome();
  for (int i = 0; i < 20; i++) {
    lhr.goTo(LOAD_ORIGIN_X, LOAD_ORIGIN_Y, LOAD_POS_Z);
    lhr.pipetteStep(steps, 'u');
    lhr.dispenseWaste();
    Serial.println(steps * (i + 1));
  }
  Serial.println("Calibration completed.");
}

void receiveSerialData() {
  receivingString = Serial.readStringUntil('\n');
  char receivingChar[30];
  String transferString = "";
  int indexChar = 0;
  int indexFloat = 0;
  bool foundCommand = false;
  char *ptrString;

  // receivingString.toCharArray(receivingChar, receivingString.length());
  ptrString = (char*) receivingString.c_str();

  // Transfer char to float
  while (*ptrString != '\0') {
    if (foundCommand) {
      if (*ptrString == '\n' && transferString.length() > 0) {
        commandParameters[indexFloat] = transferString.toFloat();
        transferString = "";
      }
      else {
        if (*ptrString == ',' && transferString.length() > 0) {
          commandParameters[indexFloat] = transferString.toFloat();
          transferString = "";
          indexChar++;
          ptrString++;
          indexFloat++;
        }
        else {
          transferString.concat(*ptrString);
          ptrString++;
          indexChar++;
        }
      }
    }
    else {
      if (*ptrString == ',' && transferString.length() > 0) {
        commandString = transferString;
        foundCommand = true;
        transferString = "";
        ptrString++;
        indexChar++;
      }
      else {
        transferString.concat(*ptrString);
        ptrString++;
        indexChar++;
      }
    }
  }
}

void executeCommand() {
  if (commandString == "GT"){
    lhr.goTo(commandParameters[0], commandParameters[1], commandParameters[2]);
  }
  else if (commandString == "SD"){
    serialDilution(commandParameters[0], commandParameters[1]);
  }
  else if (commandString == "PU"){
    lhr.drawLiquid(commandParameters[0]);
  }
  else if (commandString == "PD"){
    lhr.dispenseLiquid(commandParameters[0]);
  }
}

void clearCommands(){
  commandString = "";
  for(int i = 0; i < 3; i++)
    commandParameters[i] = 0;
}
