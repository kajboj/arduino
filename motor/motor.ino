int flexSensorPin = A0;
int ledPin = 13;
int motorPin = 3;

int calibrationTimeInSeconds = 3;
int flexSensorMin;
int flexSensorMax;
int flexMin = 0;
int flexMax = 255;

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

void setup(){
  setPwmFrequency(motorPin, 1);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  calibrateFlexSensor();
}

void calibrateFlexSensor() {
  Serial.println("Calibration complete");
  digitalWrite(ledPin, HIGH);

  int wait = 20;
  flexSensorMin = 1024;
  flexSensorMax = 0;

  int flexSensorReading;
  int i = calibrationTimeInSeconds * 1000;
  while (i > 0) {
    flexSensorReading = analogRead(flexSensorPin); 
    flexSensorMax = max(flexSensorReading, flexSensorMax);
    flexSensorMin = min(flexSensorReading, flexSensorMin);

    delay(wait);
    i -= wait;
  }

  digitalWrite(ledPin, LOW);
  Serial.println("Calibration complete");
  Serial.print("min = "); Serial.print(flexSensorMin);
  Serial.print("max = "); Serial.println(flexSensorMax);
}

int mapAndTrim(int value, int fromLow, int fromHigh, int toLow, int toHigh) {
  int scaled = map(value, fromLow, fromHigh, toLow, toHigh);
  return min(toHigh, max(scaled, toLow));
}

void loop() {
  int flexSensorReading = analogRead(flexSensorPin); 

  int flex = mapAndTrim(flexSensorReading, flexSensorMin,
    flexSensorMax, flexMin, flexMax);
  Serial.println(flex);

  analogWrite(motorPin, flex);

  delay(250);
}