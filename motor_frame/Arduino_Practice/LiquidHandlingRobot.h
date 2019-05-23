#include <Arduino.h>

class StepperMotor{
  public:
    StepperMotor(int pinStep, int pinDir, int pinMinSwitch, int pinMaxSwitch);
    float pos();
    bool isAtDest(); 
    bool isAtHome();
    float dest;
    int pinStep();
    int pinDir();
    int pinMinSwitch();
    int pinMaxSwitch();
    void updatePos();
    float millisPerStep;
    void justStepped();
    
  private:
    int m_pinStep;
    int m_pinDir;
    int m_pinMinSwitch;
    int m_pinMaxSwitch;
    float m_pos;       
};

class MotorFrame{
  public:
    MotorFrame(StepperMotor xMotor, StepperMotor yMotor, StepperMotor zMotor);
    void allPinsDir(bool value);
    void allPinsStep(bool value);
    void goToHome();
    void calibrate(float xScale, float yScale, float zScale);
    boolean calibrate();
    void showMaxSteps();
    void goTo(float xDest, float yDest, float zDest);
    float xPos();
    float yPos();
    float zPos();
    void stepAll();
    void setDestAs(float xDest, float yDest, float zDest);
    bool isAtDest();
    void showPos();
      
  private:
    float m_pos[3];
    StepperMotor m_xMotor;
    StepperMotor m_yMotor;
    StepperMotor m_zMotor;
    bool m_homed;

    int m_xMaxSteps;
    int m_yMaxSteps;
    int m_zMaxSteps;

    float m_xMaxMM;
    float m_yMaxMM;
    float m_zMaxMM;

    float m_xmmps, m_ymmps, m_zmmps;  // measures mm moved per step
  
};

class WellPlate96{
  public:
    WellPlate96(float xPos, float yPos);
    float getVolume(int col);

  private:
    float m_xPos;
    float m_yPos;
    float m_volume[12];
    
};

void serialDilution(int dilutionFactor, float volume, WellPlate wellPlate);

