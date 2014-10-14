void setup() {
  Serial.begin(9600);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

char c;

void blink(int count) {
  for (int i=0; i<count; i++) {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
  }
}

void loop() {
  if (Serial.available() > 0) {
    c = Serial.read();
    Serial.println(String(c));
    Serial.flush();
  }
  // delay(100);
}