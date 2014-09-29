#define N1 5
#define N2 4
#define N3 3
#define N4 2

#define STEPS_PER_REV 4076
#define STATE_COUNT 8
#define WAIT 2

int motorState = 0;

int pins[STATE_COUNT][4] = {
  {HIGH, LOW,  LOW,  LOW },
  {HIGH, HIGH, LOW,  LOW },
  {LOW,  HIGH, LOW,  LOW },
  {LOW,  HIGH, HIGH, LOW },
  {LOW,  LOW,  HIGH, LOW },
  {LOW,  LOW,  HIGH, HIGH},
  {LOW,  LOW,  LOW,  HIGH},
  {HIGH, LOW,  LOW,  HIGH}
};

void setup() {
  Serial.begin(9600);
  pinMode(N1, OUTPUT);
  pinMode(N2, OUTPUT);
  pinMode(N3, OUTPUT);
  pinMode(N4, OUTPUT);

  set(pins[motorState]);

  delay(500);
}

void set(int *pins) {
  digitalWrite(N1, pins[0]);
  digitalWrite(N2, pins[1]);
  digitalWrite(N3, pins[2]);
  digitalWrite(N4, pins[3]);
}

void makeSteps(int count, void (*advanceMotorState)()) {
  for (int i=0; i<count; i++) {
    advanceMotorState();
    set(pins[motorState]); delay(WAIT);
  }
}

void forward(int count) {
  makeSteps(count, next);
}

void backward(int count) {
  makeSteps(count, previous);
}

void next() {
  motorState = (motorState + 1) % STATE_COUNT;
}

void previous() {
  motorState = (motorState - 1 + STATE_COUNT) % STATE_COUNT;
}

void loop() {
  forward(STEPS_PER_REV);
  backward(STEPS_PER_REV);
}