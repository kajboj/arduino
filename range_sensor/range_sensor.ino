#include "AFMotor.h"

AF_DCMotor motorL(1);
AF_DCMotor motorR(2);
AF_DCMotor motorM(3);

int leftMotorPower = 100;
int rightMotorPower = 128;
int stepDuration = 200;
int distanceFromObstacle = 15;

#define trigPin 10
#define echoPin 9

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  motorL.setSpeed(leftMotorPower);
  motorR.setSpeed(rightMotorPower);
  motorM.setSpeed(0);
}

void ping() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

float timeToDistance(unsigned long time) {
  float t = (float) time;
  return (t/2) / 29.1;
}

float getDistance() {
  long duration;
  float distance;
  ping();
  duration = pulseIn(echoPin, HIGH);
  distance = timeToDistance(duration);
  Serial.print(distance); Serial.println(" cm");
  return distance;
}

void stepForward(AF_DCMotor motor) {
  motor.run(FORWARD);
  delay(stepDuration);
  motor.run(RELEASE);
}

void stepBackward(AF_DCMotor motor) {
  motor.run(BACKWARD);
  delay(stepDuration);
  motor.run(RELEASE);
}

void rotate() {
  stepForward(motorL);
  stepBackward(motorR);
  stepForward(motorL);
  stepBackward(motorR);
  stepForward(motorL);
  stepBackward(motorR);
}

float makeStep(AF_DCMotor motor) {
  float distance;
  distance = getDistance();
  if (distance > distanceFromObstacle) {
    stepForward(motor);
  } else {
    stepBackward(motor);
    rotate();
  }
}

void loop() {
  makeStep(motorL);
  makeStep(motorR);
}