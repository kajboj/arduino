#define STEPS_PER_REV 4076
#define STATE_COUNT 8
#define WAIT 2

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

int motorStates[2] =  {
  0, // left
  0  // right
};

int pins[2][4] = {
// N1 N2 N3 N4
  {9, 8, 7, 6}, // left
  {5, 4, 3, 2}  // right
};

int states[STATE_COUNT][4] = {
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
  pMode(LEFT_MOTOR);
  pMode(RIGHT_MOTOR);

  outputMotorStateToPins();

  delay(500);
}

void pMode(int motor) {
  pinMode(pins[motor][0], OUTPUT);
  pinMode(pins[motor][1], OUTPUT);
  pinMode(pins[motor][2], OUTPUT);
  pinMode(pins[motor][3], OUTPUT);
}

void outputMotorStateToPins() {
  outputState(LEFT_MOTOR, states[motorStates[LEFT_MOTOR]]);
  outputState(RIGHT_MOTOR, states[motorStates[RIGHT_MOTOR]]);
}

void outputState(int motor, int *state) {
  digitalWrite(pins[motor][0], state[0]);
  digitalWrite(pins[motor][1], state[1]);
  digitalWrite(pins[motor][2], state[2]);
  digitalWrite(pins[motor][3], state[3]);
}

void makeSteps(int count, void (*advanceLeftMotorState)(int), void (*advanceRightMotorState)(int)) {
  for (int i=0; i<count; i++) {
    advanceLeftMotorState(LEFT_MOTOR);
    advanceRightMotorState(RIGHT_MOTOR);
    outputMotorStateToPins();
    delay(WAIT);
  }
}

void motorLeft(int motor) { next(motor); };
void motorRight(int motor) { previous(motor); };

void next(int motor) {
  motorStates[motor] = (motorStates[motor] + 1) % STATE_COUNT;
}

void previous(int motor) {
  motorStates[motor] = (motorStates[motor] - 1 + STATE_COUNT) % STATE_COUNT;
}

void stay(int motor) {}

void loop() {
  makeSteps(STEPS_PER_REV, motorLeft, motorRight);
  makeSteps(STEPS_PER_REV, stay, motorLeft);
}