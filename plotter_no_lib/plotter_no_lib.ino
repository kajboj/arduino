#define STEPS_PER_REV 4076
#define STATE_COUNT 8
#define WAIT 1
#define STEP_COUNT 1
#define PEN_STEP_COUNT 10

#define MOTOR_COUNT 3

#define PEN_MOTOR 0
#define LEFT_MOTOR 1
#define RIGHT_MOTOR 2

int motorStates[MOTOR_COUNT] =  {
  0, // pen
  0, // left
  0  // right
};

int pins[MOTOR_COUNT][4] = {
// N1 N2 N3 N4
  {13, 12, 11, 10}, // pen
  { 9,  8,  7,  6}, // left
  { 5,  4,  3,  2}, // right
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
  Serial.begin(115200);
  setPinMode(PEN_MOTOR);
  setPinMode(LEFT_MOTOR);
  setPinMode(RIGHT_MOTOR);

  outputMotorStateToPins();

  delay(1000);
}

void setPinMode(int motor) {
  pinMode(pins[motor][0], OUTPUT);
  pinMode(pins[motor][1], OUTPUT);
  pinMode(pins[motor][2], OUTPUT);
  pinMode(pins[motor][3], OUTPUT);
}

void outputMotorStateToPins() {
  outputState(PEN_MOTOR,   states[motorStates[PEN_MOTOR]]);
  outputState(LEFT_MOTOR,  states[motorStates[LEFT_MOTOR]]);
  outputState(RIGHT_MOTOR, states[motorStates[RIGHT_MOTOR]]);
}

void outputState(int motor, int *state) {
  for (int i = 0; i<4; i++) {
    digitalWrite(pins[motor][i], state[i]);
  }
}

void makeSteps(int count, void (*advancePenMotor)(int), void (*advanceLeftMotor)(int), void (*advanceRightMotor)(int)) {
  for (int i=0; i<count; i++) {
    advancePenMotor(PEN_MOTOR);
    advanceLeftMotor(LEFT_MOTOR);
    advanceRightMotor(RIGHT_MOTOR);
    outputMotorStateToPins();
    delay(WAIT);
  }
}

void motorLeft(int motor) { previous(motor); };
void motorRight(int motor) { next(motor); };

void next(int motor) {
  motorStates[motor] = (motorStates[motor] + 1) % STATE_COUNT;
}

void previous(int motor) {
  motorStates[motor] = (motorStates[motor] - 1 + STATE_COUNT) % STATE_COUNT;
}

void stay(int motor) {}

void ln()      { makeSteps(STEP_COUNT,     stay,       motorLeft,  stay      ); }
void nl()      { makeSteps(STEP_COUNT,     stay,       stay,       motorLeft ); }
void rn()      { makeSteps(STEP_COUNT,     stay,       motorRight, stay      ); }
void nr()      { makeSteps(STEP_COUNT,     stay,       stay,       motorRight); }
void lr()      { makeSteps(STEP_COUNT,     stay,       motorLeft,  motorRight); }
void rl()      { makeSteps(STEP_COUNT,     stay,       motorRight, motorLeft ); }
void ll()      { makeSteps(STEP_COUNT,     stay,       motorLeft,  motorLeft ); }
void rr()      { makeSteps(STEP_COUNT,     stay,       motorRight, motorRight); }
void penUp()   { makeSteps(PEN_STEP_COUNT, motorLeft,  stay,       stay      ); }
void penDown() { makeSteps(PEN_STEP_COUNT, motorRight, stay,       stay      ); }

void dispatch(char c) {
  switch (c) {
    case 'a': penUp(); break;
    case 'b': penDown(); break;
    case 'c': ln(); break;
    case 'd': nl(); break;
    case 'e': rn(); break;
    case 'f': nr(); break;
    case 'g': lr(); break;
    case 'h': rl(); break;
    case 'i': ll(); break;
    case 'j': rr(); break;
  }
}

void repeat(int n, char step) {
  for(int i=0; i<n; i++) {
    dispatch(step);
  }
}

char c;

void loop() {
  // int r = 20;
  // penUp();
  // repeat(r, 'c');
  // repeat(r, 'd');
  // repeat(r, 'e');
  // repeat(r, 'f');
  // repeat(r, 'g');
  // repeat(r, 'h');
  // repeat(r, 'i');
  // repeat(r, 'j');

  // penUp();
  // delay(1000);
  // penDown();
  // delay(1000);

  if (Serial.available() > 0) {
    c = Serial.read();
    dispatch(c);
    Serial.println(c);
    Serial.flush();
  }
}