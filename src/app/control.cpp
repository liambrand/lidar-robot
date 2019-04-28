#include "control.h"

/* MOTOR DRIVER SHIELD */
DigitalOut  M1_DIR(D4);
PwmOut      M1_SPD(D3);

DigitalOut  M2_DIR(D12);
PwmOut      M2_SPD(D11);

DigitalOut  M3_DIR(D8);
PwmOut      M3_SPD(D5);

Control::Control() {
	// Start at a dead stop
	M1_SPD = 0;
	M2_SPD = 0;
	M3_SPD = 0;
}

void drive(int angle, int speed) {
  // stub
}

void stop(void) {
  M1_SPD = 0;
	M2_SPD = 0;
	M3_SPD = 0;
}
      

int getMotor1Force(int angle, int speed) {
  // method stub
}

int getMotor2Force(int angle, int speed) {
  // method stub
}

int getMotor3Force(int angle, int speed){
  // method stub
}


