#include "TimerOne.h"

int position;
int width = 1023;
int step_max = 20;
int step_min = -step_max;

void setup() {
  Timer1.initialize();
  position = random(width);
} 

void loop() {
  Timer1.pwm(9, position, 8);

  position = position + random(step_min, step_max);
  if (position > width) {
    position = 2*width - position;
  } else {
    if (position < 0) {
      position = -position;
    }
  };

}