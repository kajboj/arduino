/*

CONNECTIONS:

MODULE    ARDUINO
VCC       3.3V
GND       GND
SCL       A5
SDA       A4
DRDY      N/A

*/

#include "AFMotor.h"
#include <Wire.h>

/* The I2C address of the module */
#define HMC5803L_Address 0x1E

/* Register address for the X Y and Z data */
#define X 3
#define Y 7
#define Z 5

#define RAD_TO_DEG 57.2957795 

#define TRIG_PIN_RIGHT A0
#define ECHO_PIN_RIGHT A1
#define TRIG_PIN_LEFT A2
#define ECHO_PIN_LEFT A3

#define INFINITE_DISTANCE 32767

#define NORMAL_MOTOR_SPEED 120 

#define ROTATION_LONG_STEP_DURATION 20
#define ROTATION_SHORT_STEP_DURATION 10

#define MOVE_LONG_STEP_DURATION 50 
#define MOVE_SHORT_STEP_DURATION 10 

#define DISTANCE_SCAN_SAMPLE_COUNT 15
#define DISTANCE_FROM_OBSTACLE_MM 150 
#define DISTANCE_FROM_OBSTACLE_TOLERANCE_MM 10 

#define RANGE_FINDER_REPEAT 5 

AF_DCMotor motorL(1);
AF_DCMotor motorR(2);

void setup() 
{
  Serial.begin(9600);
  pinMode(TRIG_PIN_LEFT, OUTPUT);
  pinMode(ECHO_PIN_LEFT, INPUT);
  pinMode(TRIG_PIN_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_RIGHT, INPUT);

  Init_HMC5803L();

  motorR.setSpeed(NORMAL_MOTOR_SPEED);
  motorL.setSpeed(NORMAL_MOTOR_SPEED);
}

void rotateLeft(int time) {
  motorR.run(FORWARD);
  motorL.run(BACKWARD);
  delay(time);
  motorR.run(RELEASE);
  motorL.run(RELEASE);
}

void rotateRight(int time) {
  motorR.run(BACKWARD);
  motorL.run(FORWARD);
  delay(time);
  motorR.run(RELEASE);
  motorL.run(RELEASE);
}

void rotateLeftForward(int time) {
  motorR.run(FORWARD);
  delay(time);
  motorR.run(RELEASE);
}

void rotateRightForward(int time) {
  motorL.run(FORWARD);
  delay(time);
  motorL.run(RELEASE);
}

void stepForward(int time) {
  motorR.run(FORWARD);
  motorL.run(FORWARD);
  delay(time);
  motorR.run(RELEASE);
  motorL.run(RELEASE);
}

void stepBackward(int time) {
  motorR.run(BACKWARD);
  motorL.run(BACKWARD);
  delay(time);
  motorR.run(RELEASE);
  motorL.run(RELEASE);
}

// we could use division modulo instead of all those ifs
int normalizeAngle(int a) {
  if (a > 180) {
    return a - 360;
  } else if (a < -179) {
    return a + 360;
  } else {
    return a;
  }
}

int addAngle(int a, int b) {
  return normalizeAngle(a + b);
}

int stepDurationByAngle(int angleDiff) {
  if (abs(angleDiff) < 5) {
    return ROTATION_SHORT_STEP_DURATION;
  } else {
    return ROTATION_LONG_STEP_DURATION;
  }
}

int stepDurationByDistance(int distanceDiff) {
  if (abs(distanceDiff) < 10) {
    return MOVE_SHORT_STEP_DURATION;
  } else {
    return MOVE_LONG_STEP_DURATION;
  }
}

void rotateBy(int degrees, void (*rotateLeft)(int), void (*rotateRight)(int)) {
  int targetAngle = addAngle(getAngle(), degrees);
  int angleDiff;

  do {
    angleDiff = addAngle(targetAngle, -getAngle());

    if (angleDiff == 0) {
    } else if (angleDiff < 0) {
      rotateLeft(stepDurationByAngle(angleDiff));
    } else {
      rotateRight(stepDurationByAngle(angleDiff));
    }
  } while (angleDiff != 0); 
}

void rotateTo(int degrees, void (*rotateLeft)(int), void (*rotateRight)(int)) {
  int angleDiff = addAngle(degrees, -getAngle());
  rotateBy(angleDiff, *rotateLeft, *rotateRight);
}

void goSlowlyToObstacle(int targetAngle) {
  int targetDistance = DISTANCE_FROM_OBSTACLE_MM;
  int distanceDiff;

  do {
    int distance = getDistanceInMm();

    if (isInfinite(distance)) { return; };

    distanceDiff = distance - targetDistance;

    if (abs(distanceDiff) <= DISTANCE_FROM_OBSTACLE_TOLERANCE_MM) {
    } else if (distanceDiff < 0) {
      rotateTo(targetAngle, rotateLeft, rotateRight);
      stepBackward(stepDurationByDistance(distanceDiff));
    } else {
      rotateTo(targetAngle, rotateLeftForward, rotateRightForward);
      stepForward(stepDurationByDistance(distanceDiff));
    }
  } while (abs(distanceDiff) > DISTANCE_FROM_OBSTACLE_TOLERANCE_MM); 
}

void goToObstacle(int angle) {
  goSlowlyToObstacle(angle);
}

int randomAngle() {
  long coinFlip = random(0, 2);
  if (coinFlip == 0) {
    return 90;
  } else {
    return -90;
  }
}

void printVal(String msg, int val) {
  Serial.print(msg);
  Serial.println(val);
}

int scanForBestDirection(int angleRange) {
  int maxDistance = 0; 
  int maxDistanceAngle = 0; 
  int initialAngle = getAngle();
  int rotation = round((float)angleRange / DISTANCE_SCAN_SAMPLE_COUNT);

  for(int angle=initialAngle; angle<=initialAngle+angleRange; angle+=rotation) {
    int distance = getDistanceInMm();
    if (distance > maxDistance && !isInfinite(distance)) {
      maxDistance = distance;
      maxDistanceAngle = getAngle();
    }

    // printVal("angle = ", angle);
    // printVal("distance = ", distance);
    // printVal("maxDistance = ", maxDistance);
    // printVal("maxDistanceAngle = ", maxDistanceAngle);
    // delay(500);

    rotateTo(angle, rotateLeft, rotateRight);
  }

  return maxDistanceAngle;
}

/* This function will initialise the module and only needs to be run once
   after the module is first powered up or reset */
void Init_HMC5803L(void)
{
  /* Set the module to 8x averaging and 15Hz measurement rate */
  Wire.beginTransmission(HMC5803L_Address);
  Wire.write((uint8_t) 0x00);
  Wire.write(0x70);
          
  /* Set a gain of 5 */
  Wire.write(0x01);
  Wire.write(0xA0);
  Wire.endTransmission();
}


/* This function will read once from one of the 3 axis data registers
and return the 16 bit signed result. */
int HMC5803L_Read(byte Axis)
{
  int Result;
  
  /* Initiate a single measurement */
  Wire.beginTransmission(HMC5803L_Address);
  Wire.write(0x02);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(6);
  
  /* Move modules the resiger pointer to one of the axis data registers */
  Wire.beginTransmission(HMC5803L_Address);
  Wire.write(Axis);
  Wire.endTransmission();
   
  /* Read the data from registers (there are two 8 bit registers for each axis) */  
  Wire.requestFrom(HMC5803L_Address, 2);
  Result = Wire.read() << 8;
  Result |= Wire.read();

  return Result;
}

int getAngle() {
  int x = HMC5803L_Read(X);
  int y = HMC5803L_Read(Y);
  return round(RAD_TO_DEG * atan2(x, y));
}

void ping(int trigPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

boolean isInfinite(int distance) {
  return distance == INFINITE_DISTANCE;
}

int pingDurationToMm(int duration) {
  if (duration == 0) {
    return INFINITE_DISTANCE;
  } else {
    return (duration/2) / 2.91;
  };
}

unsigned long getPingDuration(int trigPin, int echoPin) {
  ping(trigPin);
  return pulseIn(echoPin, HIGH);
}

int getDistanceInMm() {
  int sumDistance = 0;
  int goodSampleCount = 0;

  for(int i=0; i<RANGE_FINDER_REPEAT; i++) {
    int distance = 
      pingDurationToMm(getPingDuration(TRIG_PIN_LEFT, ECHO_PIN_LEFT));

    if (!isInfinite(distance)) {
      sumDistance += distance;
      goodSampleCount++;
    }
    delay(5); // we have to delay otherwise pings interfere with each other
  }

  if (goodSampleCount == 0) { return INFINITE_DISTANCE; };
  return round((float) sumDistance / (float) goodSampleCount);
}

void loop() 
{
 printVal("getDistanceInMm() = ", getDistanceInMm());

  // int angle = scanForBestDirection(360);
  // rotateTo(angle, rotateLeft, rotateRight);
  // goSlowlyToObstacle(angle);

  // scanForBestDirection(90);
  // rotateBy(-90, rotateLeftForward, rotateRightForward);

  // goToObstacle(getAngle());
}
