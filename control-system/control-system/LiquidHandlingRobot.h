#ifndef LHR_DEF
#define LHR_DEF
#include "StepperMotor.h"
#include "Pipette.h"

class LiquidHandlingRobot{
  public:
    LiquidHandlingRobot(StepperMotor xMotor, StepperMotor yMotor, StepperMotor zMotor,
                        Pipette& p, float xMaxMM, float yMaxMM, float zMaxMM);
    void goToHome();
    boolean calibrate();
    void calibratePipette();
    void allPinsDir(bool value);
    void allPinsStep(bool xValue, bool yValue, bool zValue);
    void allPinsStep(bool value);
    void showMaxSteps();
    bool goTo(float xDest, float yDest, float zDest);
    float x();
    float y();
    float z();
    void stepAll();
    void setDestAs(float xDest, float yDest, float zDest);
    bool isAtDest();
    bool notAtDest();
    void showPos();

    // Pipette operations
    void drawLiquid(float v, float zPos = -1);       // Draw up liquid according to volume v at target position z
    void dispenseLiquid(float v, float zPos = -1);   // Dispense liq volume v with a dispense point zPos
    void emptyLiquidInWaste(float x, float y, float z); // Dispense all liquid at waste specified by location 
    void mix(float dv, float targetZ, int iterations);  // Mix with volume dv at depth targetZ by times specified by iterations
    void dispenseWaste();
    void pipetteStep(int numSteps, char dir);
    
  

  private:
    void updatePosAfterStep(bool xMotor, bool yMotor, bool zMotor);
    void updatePosAfterStep();
    int m_pos[3];

    StepperMotor* m_motors[3];
    Pipette* m_pip;

    StepperMotor m_xMotor;
    StepperMotor m_yMotor;
    StepperMotor m_zMotor;
    int m_xMaxSteps;
    int m_yMaxSteps;
    int m_zMaxSteps;
    float m_xMaxMM;
    float m_yMaxMM;
    float m_zMaxMM;
    bool m_homed;

};

#endif // LHR_DEF
