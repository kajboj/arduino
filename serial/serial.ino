void setup() {
  Serial.begin(9600);
}

char c;

void loop() {
  if (Serial.available() > 0) {
    c = Serial.read();
    Serial.println(c);
    Serial.flush();
  }
}