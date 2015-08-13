/*
 The circuit:
 * pushbutton attached from pin 4 to +5V
 * 10-kilohm resistor attached from pin 4 to ground
 */

const int buttonPin = 2;
int previousButtonState = LOW;
int buttonState = LOW;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 10;

unsigned long triggerTime = 0;
boolean autoRepeat = false;
const unsigned long autoRepeatDelay = 600;
const unsigned long autoRepeatTime = 100;

void setup() {
  pinMode(buttonPin, INPUT);
  Keyboard.begin();
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != previousButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      if ((reading == HIGH) && (buttonState == LOW)) {
        Keyboard.print("a");
        triggerTime = millis();
        autoRepeat = false;
      }

      buttonState = reading;
    }

    if ((reading == HIGH) && (buttonState == HIGH)) {
      if (autoRepeat) {
        if ((millis() - triggerTime) > autoRepeatTime) {
          Keyboard.print("a");
          triggerTime = millis();
        }
      } else {
        if ((millis() - triggerTime) > autoRepeatDelay) {
          Keyboard.print("a");
          triggerTime = millis();
          autoRepeat = true;
        }
      }
    }
  }

  previousButtonState = reading;
}

