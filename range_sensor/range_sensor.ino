#include "AFMotor.h"

AF_DCMotor motorL(1);
AF_DCMotor motorR(2);

int motorLForwardPower = 255;
int motorLBackwardPower = 255;

int motorRForwardPower = 160;
int motorRBackwardPower = 160;

int fullRotationMillis = 320; 
int rotationConstant = 50;

int forwardStepMillis = 200;

int scanDelayMillis = 300;

int distanceFromObstacle = 20;

int leftDistance;
int middleDistance;
int rightDistance;

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

void startRForward() {
  motorR.setSpeed(motorRForwardPower);
  motorR.run(FORWARD);
}

void startRBackward() {
  motorR.setSpeed(motorRBackwardPower);
  motorR.run(BACKWARD);
}

void stopR() { motorR.run(RELEASE); }

void startLForward() {
  motorL.setSpeed(motorLForwardPower);
  motorL.run(FORWARD);
}

void startLBackward() {
  motorL.setSpeed(motorLBackwardPower);
  motorL.run(BACKWARD);
}

void stopL() { motorL.run(RELEASE); }

int degreesToTime(int degrees) {
  int fraction = (int) ((degrees / 360.0) * fullRotationMillis);
  return fraction + rotationConstant;
}

void rotateLeft(int degrees) {
  startLBackward();
  startRForward();
  delay(degreesToTime(degrees));
  stopL();
  stopR();
}

void rotateRight(int degrees) {
  startRBackward();
  startLForward();
  delay(degreesToTime(degrees));
  stopR();
  stopL();
}

void rotateRandom(int degrees) {
  long coinFlip = random(0, 2);
  if (coinFlip == 0) {
    rotateRight(degrees);
  } else {
    rotateLeft(degrees);
  }
}

void goForward() {
  startRForward();
  startLForward();
  delay(forwardStepMillis);
  stopR();
  stopL();
}

boolean blocked(int distance) {
  return distance < distanceFromObstacle;
}

void makeStep() {
  boolean l = blocked(leftDistance);
  boolean m = blocked(middleDistance);
  boolean r = blocked(rightDistance);

  if (l && m && r) { rotateRandom(90); return; }
  if (l && m)      { rotateRight(45);  return; }
  if (m && r)      { rotateLeft(45);   return; }
  if (l && r)      { goForward();      return; }
  if (l)           { rotateRight(22);  return; }
  if (m)           { rotateRandom(45); return; }
  if (r)           { rotateLeft(22);   return; }

  goForward();
}

void scanSurroundings() {
  middleDistance = getDistance();

  rotateLeft(45);
  delay(scanDelayMillis);
  leftDistance = getDistance();

  rotateRight(90);
  delay(scanDelayMillis);
  rightDistance = getDistance();

  rotateLeft(45);
}

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  scanSurroundings();
  // rotateLeft(90);
  delay(2000);
  // delay(500);
  // makeStep();
  // delay(500);
}