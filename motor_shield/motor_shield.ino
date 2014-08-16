#include "AFMotor.h"

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(4);

const int _ = 0;
const int H = 1;
const int F = 2;

int tapDuration = 10;
int quietTapDuration = 5;
int swipeDuration = 200;
int quietSwipeDuration = 100;
int barDuration = 200;

void startMotor(AF_DCMotor motor) { motor.run(FORWARD); }
void stopMotor(AF_DCMotor motor)  { motor.run(RELEASE); }

void startBass() { startMotor(motor1); }
void stopBass()  { stopMotor(motor1); }
void startSnare() { startMotor(motor2); }
void stopSnare()  { stopMotor(motor2); }
void startSwipe() { startMotor(motor3); }
void stopSwipe()  { stopMotor(motor3); }

void bar() {
  delay(barDuration);
}

void p(int bass, int snare, int hihat) {
  if ((bass == F) || (bass == H)) startBass();
  if ((snare == F) || (snare == H)) startSnare();
  if ((hihat == F) || (hihat == H)) startSwipe();
  delay(quietTapDuration);
  if (bass == H) stopBass();
  if (snare == H) stopSnare();
  delay(tapDuration-quietTapDuration);
  if (bass == F) stopBass();
  if (snare == F) stopSnare();
  delay(quietSwipeDuration-tapDuration);
  if (hihat == H) stopSwipe();
  delay(swipeDuration-quietSwipeDuration);
  if (hihat == F) stopSwipe();
  delay(barDuration-swipeDuration);
}

void setup() {
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
}

void loop() {
  p(  F  ,  _  ,  F  ); // 0
  p(  _  ,  _  ,  _  );
  p(  _  ,  _  ,  H  );
  p(  _  ,  _  ,  _  );
  p(  _  ,  F  ,  F  ); // 1
  p(  _  ,  _  ,  _  );
  p(  F  ,  _  ,  H  );
  p(  _  ,  H  ,  _  );
  p(  _  ,  _  ,  F  ); // 2
  p(  _  ,  H  ,  _  );
  p(  F  ,  _  ,  H  );
  p(  _  ,  _  ,  _  );
  p(  _  ,  F  ,  F  ); // 3
  p(  _  ,  _  ,  _  );
  p(  _  ,  _  ,  H  );
  p(  _  ,  _  ,  _  );
}