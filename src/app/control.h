#ifndef __control_h__
#define __control_h__

#include <mbed.h>
#include <rplidar.h>
#include <BufferedSerial.h>

extern DigitalOut  M1_DIR;
extern PwmOut      M1_SPD;

extern DigitalOut  M2_DIR;
extern PwmOut      M2_SPD;

extern DigitalOut  M3_DIR;
extern PwmOut      M3_SPD;

class Control{   
    public:
      Control();
      void drive(int angle, int speed);
      void stop(void);
      
    private:
      int getMotor1Force(int angle, int speed);
      int getMotor2Force(int angle, int speed);
      int getMotor3Force(int angle, int speed);
};

#endif __control_h__
