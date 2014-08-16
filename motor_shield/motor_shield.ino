#include "AFMotor.h"

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(4);

int tapDuration = 10;
int quietTapDuration = 5;
int swipeDuration = 200;
int barDuration = 200;

void tap(AF_DCMotor motor) {
  motor.run(FORWARD);
  delay(tapDuration);
  motor.run(RELEASE);
  delay(barDuration-tapDuration);
}

void quiet(AF_DCMotor motor) {
  motor.run(FORWARD);
  delay(quietTapDuration);
  motor.run(RELEASE);
  delay(barDuration-quietTapDuration);
}

void swipe() {
  motor3.run(FORWARD);
  delay(swipeDuration);
  motor3.run(RELEASE);
  delay(barDuration-swipeDuration);
}

void tap1()   { tap(motor1); }
void quiet1() { quiet(motor1); }
void tap2()   { tap(motor2); }
void quiet2() { quiet(motor2); }

void bar() {
  delay(barDuration);
}

void setup() {
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
}

void loop() {
  tap1();
    bar();
    bar();
    bar();
  tap2();
    bar();
    bar();
    bar();
  tap1();
  tap2();
    bar();
  quiet1();
  tap2();
  quiet1();
    bar();
  quiet2();
}