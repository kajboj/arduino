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

#define trigPin A2
#define echoPin A3

#define INFINITE_DISTANCE 32767

#define MOTOR_SPEED 120 

#define LONG_STEP_DURATION 20
#define SHORT_STEP_DURATION 10

AF_DCMotor motorL(1);
AF_DCMotor motorR(2);

void setup() 
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Init_HMC5803L();

  motorR.setSpeed(MOTOR_SPEED);
  motorL.setSpeed(MOTOR_SPEED);
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

int addAngle(int a, int b) {
  int c = a + b;

  if (c > 180) {
    return c - 360;
  } else if (c < -179) {
    return c + 360;
  } else {
    return c;
  }
}

int stepDuration(int angleDiff) {
  if (abs(angleDiff) < 5) {
    return SHORT_STEP_DURATION;
  } else {
    return LONG_STEP_DURATION;
  }
}

void rotateBy(int degrees) {
  int targetAngle = addAngle(getAngle(), degrees);
  int angleDiff;

  do {
    angleDiff = targetAngle - getAngle();

    if (angleDiff == 0) {
      Serial.println("DONE!");
    } else if (angleDiff < 0) {
      rotateLeft(stepDuration(angleDiff));
    } else {
      rotateRight(stepDuration(angleDiff));
    }
    Serial.print(angleDiff);
    Serial.println(" degrees");
    Serial.print(getDistanceInMm());
    Serial.println(" mm");

    // delay(500);
  } while (angleDiff != 0); 
}

void loop() 
{
  rotateBy(90);
  delay(1000);
  rotateBy(-90);
  delay(1000);
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

void ping() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

int getDistanceInMm() {
  ping();
  unsigned long duration = pulseIn(echoPin, HIGH);

  if (duration == 0) {
    return INFINITE_DISTANCE;
  } else {
    return (duration/2) / 2.91;
  };
}