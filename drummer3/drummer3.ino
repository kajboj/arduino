#include "AFMotor.h"

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(4);

const int _ = 0;
const int H = 1;
const int F = 2;

int tapDuration        = 10;
int quietTapDuration   = 5;
int swipeDuration      = 150;
int quietSwipeDuration = 75;
int barDuration        = 150;

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
  p(  _  ,  F  ,  _  ); // 0
  p(  _  ,  _  ,  _  );
  p(  _  ,  H  ,  _  );
  p(  _  ,  _  ,  _  );
  p(  _  ,  H  ,  _  ); // 1
  p(  _  ,  F  ,  _  );
  p(  _  ,  _  ,  _  );
  p(  _  ,  H  ,  _  );
  p(  _  ,  F  ,  _  ); // 2
  p(  _  ,  _  ,  _  );
  p(  _  ,  H  ,  _  );
  p(  _  ,  F  ,  _  );
  p(  _  ,  _  ,  _  ); // 3
  p(  _  ,  F  ,  _  );
  p(  _  ,  _  ,  _  );
  p(  _  ,  H  ,  _  );

  p(  F  ,  _  ,  _  ); // 0
  p(  _  ,  _  ,  _  );
  p(  H  ,  _  ,  _  );
  p(  _  ,  _  ,  _  );
  p(  H  ,  _  ,  _  ); // 1
  p(  F  ,  _  ,  _  );
  p(  _  ,  _  ,  _  );
  p(  H  ,  _  ,  _  );
  p(  F  ,  _  ,  _  ); // 2
  p(  _  ,  _  ,  _  );
  p(  H  ,  _  ,  _  );
  p(  F  ,  _  ,  _  );
  p(  _  ,  _  ,  _  ); // 3
  p(  F  ,  _  ,  _  );
  p(  _  ,  _  ,  _  );
  p(  H  ,  _  ,  _  );

  while (true) {
    p(  F  ,  F  ,  H  ); // 0
    p(  _  ,  _  ,  _  );
    p(  _  ,  _  ,  F  );
    p(  H  ,  F  ,  _  );
    p(  F  ,  _  ,  H  ); // 1
    p(  _  ,  _  ,  _  );
    p(  _  ,  F  ,  F  );
    p(  H  ,  _  ,  _  );
    p(  F  ,  _  ,  H  ); // 2
    p(  _  ,  H  ,  _  );
    p(  _  ,  F  ,  F  );
    p(  H  ,  _  ,  _  );
    p(  F  ,  _  ,  H  ); // 3
    p(  _  ,  _  ,  _  );
    p(  _  ,  _  ,  F  );
    p(  H  ,  F  ,  _  );
  }
}