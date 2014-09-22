#include "AFMotor.h"

#define STEPS_PER_REV 2048

AF_Stepper motorL(STEPS_PER_REV, 1);
AF_Stepper motorR(STEPS_PER_REV, 2);

void rotate(int degrees, int direction) {
}

void setup() {
  motorR.setSpeed(10);
  motorL.setSpeed(10);
}

int s = 1024;
 
void loop() {
  motorR.step(s, FORWARD, MICROSTEP); 
  motorL.step(s, FORWARD, MICROSTEP); 
  motorR.step(s, BACKWARD, MICROSTEP); 
  motorL.step(s, BACKWARD, MICROSTEP); 

  s = s/2;

  if (s==0) {
    s = 1024;
  }
}