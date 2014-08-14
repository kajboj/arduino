int lightPin = 5;
int ledPin = 13;
int motorPin = 3;

int calibrationLoops = 32000;
int lightMin;
int lightMax;

float targetRevsPerSecond = 15.0;

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

  calibrateLightSensor();
}

void startMotor() {
  analogWrite(motorPin, 255);
  delay(10);
}

void revMotor() {
  analogWrite(motorPin, 127);
}

void stopMotor() {
  analogWrite(motorPin, 0);
}

void calibrateLightSensor() {
  Serial.println("Calibration started");
  startMotor();
  revMotor();
  delay(1000);

  digitalWrite(ledPin, HIGH);

  int wait = 20;
  lightMin = 1024;
  lightMax = 0;

  int lightReading;
  for(int i = 0; i < calibrationLoops; i++) {
    lightReading = analogRead(lightPin); 
    lightMax = max(lightReading, lightMax);
    lightMin = min(lightReading, lightMin);

    // if (i == calibrationLoops/2) {
    //   stopMotor();
    // }
  }
  stopMotor();

  digitalWrite(ledPin, LOW);
  Serial.println("Calibration complete");
  Serial.print("min = "); Serial.print(lightMin);
  Serial.print("max = "); Serial.println(lightMax);
}

int readLigth() {
  mapAndTrim(analogRead(lightPin), lightMin, lightMax, 0, 255);
}

int mapAndTrim(int value, int fromLow, int fromHigh, int toLow, int toHigh) {
  int scaled = map(value, fromLow, fromHigh, toLow, toHigh);
  return min(toHigh, max(scaled, toLow));
}

void highToLow(unsigned long lastHighToLow) {
}

float revsPerSecond(unsigned long now, unsigned long lastHighToLow) {
    int halfRevTime = now - lastHighToLow;
    int revTime = 2*halfRevTime;
    float revsPerSec = 1000.0 / revTime;
    return revsPerSec;
}

boolean low = false;
int i = 0;
unsigned long lastHighToLow = 0;
int motorPower = 127;

void loop()
{
  int light = readLigth();

  if (light < 32) {
    if (low == false) {
      low = true;
      if (lastHighToLow == 0) {
        lastHighToLow = millis();
      } else {
        int now = millis();
        float revsPerSec = revsPerSecond(now, lastHighToLow);
        lastHighToLow = now;

        if (revsPerSec < targetRevsPerSecond) {
          motorPower += 20;
        } else {
          motorPower -= 20;
        }
        motorPower = constrain(motorPower, 0, 200);

        Serial.print(motorPower);
        Serial.print(" ");
        Serial.println(revsPerSec);
      }
      // Serial.println("going LOW");
    }
  } else if (light > 128) {
    if (low == true) {
      low = false;
      // Serial.println("going HIGH");
    }
  }

  if (i==0) {
    startMotor();    
    i++;
  } else {
    analogWrite(motorPin, motorPower);
  }
}