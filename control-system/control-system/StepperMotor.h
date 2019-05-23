#ifndef STEPPER_DEF
#define STEPPER_DEF

class StepperMotor{
  public:
    StepperMotor(int pinStep, int pinDir, int pinMinSwitch, int pinMaxSwitch);
    float pos();
    bool isAtDest();
    bool isAtHome();
    bool isAtMax();
    float dest;
    int pinStep();
    int pinDir();
    int pinMinSwitch();
    int pinMaxSwitch();
    void updatePos();
    double millisPerStep;
    void updatePosAfterStep();
    void setPinDir(int i);  // sets direction pin i: {0, 1}

  private:
    int m_pinStep;
    int m_pinDir;
    int m_pinMinSwitch;
    int m_pinMaxSwitch;
    float m_pos;
};

#endif // STEPPER_DEF
