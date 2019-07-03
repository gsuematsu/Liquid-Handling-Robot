#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>

#define SHAPEKO_XMAXMM 173
#define SHAPEKO_YMAXMM 158
#define SHAPEKO_ZMAXMM 65

const float PIPETTE_S2V = 0.06386364;
const float PIPETTE_S2D = 1 / PIPETTE_S2V;
const float PIPETTE_MAX_VOL = 300;  // ul
#define PIPETTE_STEP_PIN 7
#define PIPETTE_DIR_PIN 6

#define X_MOTOR_STEP_PIN 13
#define X_MOTOR_DIR_PIN 12
#define X_MIN_SWITCH_PIN A0
#define X_MAX_SWITCH_PIN A3

#define Y_MOTOR_STEP_PIN 11
#define Y_MOTOR_DIR_PIN 10
#define Y_MIN_SWITCH_PIN A1
#define Y_MAX_SWITCH_PIN A4

#define Z_MOTOR_STEP_PIN 9
#define Z_MOTOR_DIR_PIN 8
#define Z_MIN_SWITCH_PIN A2
#define Z_MAX_SWITCH_PIN A5

const float WELL_ORIGIN_X = 0;       // where pipette is lined up with well
const float WELL_ORIGIN_Y = 0;
const float WELL_DISPENSE_POS_Z = 50; // where pipette is needs to move in order
                                      // to dispense liquid
const float LOAD_ORIGIN_X = 150;      // where pipette aligned to reload fluid
const float LOAD_ORIGIN_Y = 10;
const float LOAD_POS_Z = 60;          // where pipette must drop to to reload fluid
const float LOAD_ORIGIN_OFFSET = 60;  // offset between load types. I.e. dye and water.
const float LOAD_Z_CLEARANCE = 20;

const int WELL_ROWS = 8;              // well layout
const int WELL_COLS = 12;
const float DISTANCE_BETWEEN_WELLS = 8.30; // 9.10;  // distance between center of each circular well
const float WELL_PLATE_Z_CLEARANCE = 45;
const float WELL_PLATE_Z_POS = 50;
const float WELL_PLATE_Z_DRAW_OFFSET = 11;
const float MAX_WELL_VOLUME = 360;

const float WASTE_X_POS = 50;
const float WASTE_Y_POS = 150;
const float WASTE_Z_POS = 0;

const int GOTO_DELAY = 200;           // delay after every call to goTo, and potentially other functions down the line
const int PIPETTE_STEP_DELAY = 700;
const int WAIT_AFTER_PIPETTE = 10;   // delay after a pipette command
const int MIX_DELAY = 10;            // delay between mixing iterations
const int PULSE_DURATION_XY = 800;
const int PULSE_DURATION_Z = 330;
const int STEPSORVOLTOFILL = 50;

const byte DO_NOTHING = 0x0;
const byte COMMAND_RECEIVED = 0x1;
const byte TRANSMISSION_TERMINATED = 0xf;

static bool CALIBRATING = true;

#endif // GLOBAL_H
