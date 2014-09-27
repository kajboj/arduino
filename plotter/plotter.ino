#include "AFMotor.h"

#define STEPS_PER_REV 2048
#define STEP_COUNT 4 
#define LLEFT BACKWARD
#define RLEFT BACKWARD
#define LRIGHT FORWARD
#define RRIGHT FORWARD

AF_Stepper motorL(STEPS_PER_REV, 2);
AF_Stepper motorR(STEPS_PER_REV, 1);

void setup() {
  Serial.begin(115200);
  delay(1000);
  motorR.setSpeed(8);
  motorL.setSpeed(8);
}

void ln() { step(motorL, LLEFT); }
void nl() { step(motorR, RLEFT); }
void rn() { step(motorL, LRIGHT); }
void nr() { step(motorR, RRIGHT); }
void lr() {
  step(motorL, LLEFT);
  step(motorR, RRIGHT);
}
void rl() {
  step(motorL, LRIGHT);
  step(motorR, RLEFT);
}
void ll() {
  step(motorL, LLEFT);
  step(motorR, RLEFT);
}
void rr() {
  step(motorL, LRIGHT);
  step(motorR, RRIGHT);
}

void step(AF_Stepper motor, int dir) {
  motor.step(STEP_COUNT, dir, MICROSTEP);
}

void dispatch(char c) {
  switch (c) {
    case 'a': break;
    case 'b': break;
    case 'c': ln(); break;
    case 'd': nl(); break;
    case 'e': rn(); break;
    case 'f': nr(); break;
    case 'g': lr(); break;
    case 'h': rl(); break;
    case 'i': ll(); break;
    case 'j': rr(); break;
  }
}

void repeat(int n, char step) {
  for(int i=0; i<n; i++) {
    dispatch(step);
  }
}

char c;

void loop() {
  int r = 100;
  repeat(r, 'c');
  repeat(r, 'd');
  repeat(r, 'e');
  repeat(r, 'f');
  repeat(r, 'g');
  repeat(r, 'h');
  repeat(r, 'i');
  repeat(r, 'j');

  // if (Serial.available() > 0) {
  //   c = Serial.read();
  //   dispatch(c);
  //   Serial.println(c);
  //   Serial.flush();
  // }
}