#ifndef PIPETTE_H
#define PIPETTE_H

class Pipette {
  public:
    Pipette(float s2v, float maxV, int stepPin, int dirPin);
    
    void down(float volume, bool delayOn = true);  // moves linear motor down to dispense volume.
                                              // delayOn indicates delay after command is on by default
    void up(float volume, bool delayOn = true);     // moves the linear motor up to draw volume
    float getVolume();        // get current volume in pipette
    float getMaxVolume();     // return max volume
    float s2v();              // returns the step 2 volume conversion rate
    float v2s();              // returns the volume 2 step conversion rate
    void dispenseAll();       // dispenses all fluid and returns to position 0 (the bottom);
    void stepMotor();   // steps and updates position accordingly

void dirDown();     // changes pindir for stepper to down
    void dirUp();       // changes pindir for stepper to up
  private:
    v2s(float v);       // converts volume to steps
    

    int m_stepPin;      // 
    int m_dirPin;       // 
    int m_dir;          // dirPin current value
    float m_s2v;        // step -> volume conversion factor
    float m_v2s;        // volume -> step conversion factor
    float m_volume;     // current volume in pipette
    float m_maxVol;     // maximum volume able to be stored
};

#endif // PIPETTE_H

