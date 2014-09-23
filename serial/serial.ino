void setup(){
  Serial.begin(9600);
}

char c;

void loop() {
  while (Serial.available() == 0);
  c = Serial.read();
  Serial.println(c);

  delay(500);
}