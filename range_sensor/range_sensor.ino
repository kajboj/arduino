#include "AFMotor.h"

AF_DCMotor motorL(1);
AF_DCMotor motorR(2);

int motorSpeed = 80;

int scanDelayMillis = 300;

int distanceFromObstacle = 20;

int stepLMillis = 400;
int stepRMillis = 440;

#define trigPin 10
#define echoPin 9

void ping() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

int timeToDistance(unsigned long time) {
  return (time/2) / 29.1;
}

int getDistance() {
  unsigned long duration;
  int distance;
  ping();
  duration = pulseIn(echoPin, HIGH);
  distance = timeToDistance(duration);
  Serial.print(distance); Serial.println(" cm");
  return distance;
}

void rotateRandom() {
  long coinFlip = random(0, 2);
  if (coinFlip == 0) {
    motorL.run(BACKWARD);
    delay(2*stepLMillis);
    motorL.run(RELEASE);
  } else {
    motorR.run(BACKWARD);
    delay(2*stepRMillis);
    motorR.run(RELEASE);
  }
}

boolean blocked(int distance) {
  return distance < distanceFromObstacle;
}

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  motorR.setSpeed(motorSpeed);
  motorL.setSpeed(motorSpeed);
}

void stepForward(AF_DCMotor motor, int time) {
  motor.run(FORWARD);
  delay(time);
  motor.run(RELEASE);
}

void stepBackward(AF_DCMotor motor, int time) {
  motor.run(BACKWARD);
  delay(time);
  motor.run(RELEASE);
}

boolean tryForward(AF_DCMotor motor, int stepMillis) {
  stepForward(motor, stepMillis);

  int distance = getDistance();
  if (blocked(distance)) {
    stepBackward(motor, stepMillis);
    return false;
  }
  return true;
}

void loop() {
  stepForward(motorR, stepLMillis);
  delay(500);
  
  stepForward(motorL, stepLMillis);
  delay(500);
  // boolean left = tryForward(motorL, stepLMillis);
  // boolean right = tryForward(motorR, stepRMillis);

  // if (!right && !left) {
  //   rotateRandom();
  // }
}