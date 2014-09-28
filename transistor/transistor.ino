// Define which pin to be used to communicate with Base pin of TIP120 transistor
int pin = 9; //for this project, I pick Arduino's PMW pin 11

void setup() {
  pinMode(pin, OUTPUT); // Set pin for output to control TIP120 Base pin
}

void loop() {
  digitalWrite(pin, HIGH);
  delay(1000);

  digitalWrite(pin, LOW);
  delay(1000);
}