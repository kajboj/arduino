#include "AFMotor.h"

AF_DCMotor motorL(1);
AF_DCMotor motorR(2);

int motorSpeed = 255;

void setup() {
  Serial.begin(9600);
  motorR.setSpeed(motorSpeed);
  motorL.setSpeed(motorSpeed);
}

void loop() {
  motorR.setSpeed(motorSpeed);
  motorL.setSpeed(motorSpeed);
  for(int i=0; i<40; i++) {
    motorR.run(FORWARD);
    motorL.run(FORWARD);
    delay(10);
    motorR.run(RELEASE);
    motorL.run(RELEASE);
    delay(100);
  }

  motorR.setSpeed(200);
  motorL.setSpeed(200);
  for(int i=0; i<10; i++) {
    motorR.run(BACKWARD);
    motorL.run(BACKWARD);
    delay(10);
    motorR.run(RELEASE);
    motorL.run(RELEASE);
    delay(200);
  }
}