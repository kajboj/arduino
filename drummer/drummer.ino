// Define which pin to be used to communicate with Base pin of TIP120 transistor
int pin = 3; //for this project, I pick Arduino's PMW pin 11

void setup() {
  pinMode(pin, OUTPUT); // Set pin for output to control TIP120 Base pin
}

int tapDuration = 30;
int quietTapDuration = 10;
int barDuration = 200;

void tap() {
  digitalWrite(pin, HIGH);
  delay(tapDuration);
  digitalWrite(pin, LOW);
  delay(barDuration-tapDuration);
}

void quiet() {
  digitalWrite(pin, HIGH);
  delay(quietTapDuration);
  digitalWrite(pin, LOW);
  delay(barDuration-quietTapDuration);
}

void bar() {
  delay(barDuration);
}

void loop() {
  tap();
    bar();
    bar();
    bar();
  quiet();
    bar();
    bar();
    bar();
  tap();
    bar();
  tap();
    bar();
  quiet();
  tap();
    bar();
  quiet();
  tap();
    bar();
    bar();
    bar();
  quiet();
    bar();
    bar();
  tap();
    bar();
    bar();
  tap();
    bar();
  quiet();
    bar();
  tap();
  tap();
}