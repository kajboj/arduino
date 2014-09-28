#include "AFMotor.h"

#define STEPS_PER_REV 2048
#define STEP_TYPE MICROSTEP
#define STEP_COUNT 4
#define LLEFT BACKWARD
#define RLEFT BACKWARD
#define LRIGHT FORWARD
#define RRIGHT FORWARD
#define PEN_PIN 9
#define MOTOR_SPEED 12

AF_Stepper motorL(STEPS_PER_REV, 2);
AF_Stepper motorR(STEPS_PER_REV, 1);

void setup() {
  Serial.begin(115200);
  delay(1000);
  motorR.setSpeed(MOTOR_SPEED);
  motorL.setSpeed(MOTOR_SPEED);
  pinMode(PEN_PIN, OUTPUT);
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
  motor.step(STEP_COUNT, dir, STEP_TYPE);
}

void penUp() {
  digitalWrite(PEN_PIN, LOW);
  delay(100);
}

void penDown() {
  digitalWrite(PEN_PIN, HIGH);
  delay(100);
}

void dispatch(char c) {
  switch (c) {
    case 'a': penUp(); break;
    case 'b': penDown(); break;
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
  // int r = 20;
  // penDown();
  // repeat(r, 'c');
  // repeat(r, 'd');
  // repeat(r, 'e');
  // repeat(r, 'f');
  // repeat(r, 'g');
  // repeat(r, 'h');
  // repeat(r, 'i');
  // repeat(r, 'j');

  // penUp();
  // delay(1000);
  // penDown();
  // delay(1000);

  if (Serial.available() > 0) {
    c = Serial.read();
    dispatch(c);
    Serial.println(c);
    Serial.flush();
  }
}