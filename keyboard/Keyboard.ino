/*
 The circuit:
 * pushbutton attached from pin 4 to +5V
 * 10-kilohm resistor attached from pin 4 to ground
 */

typedef struct {
  int pin;
  char code;
  unsigned long lastDebounceTime;
  int previousState;
  int state;
} key;

key keys[] = {
  { 4, 'a', 0, LOW, LOW }
};

// static const char key = KEY_LEFT_SHIFT;
// static const unsigned int keyCount = 3;
static const unsigned long debounceDelay = 10;

void update(key *button) {
  int reading = digitalRead(button->pin);

  if (reading != button->previousState) {
    button->lastDebounceTime = millis();
  }

  if ((millis() - button->lastDebounceTime) > debounceDelay) {
    if (reading != button->state) {
      if ((reading == HIGH) && (button->state == LOW)) {
        Keyboard.press(button->code);
      }

      if ((reading == LOW) && (button->state == HIGH)) {
        Keyboard.release(button->code);
      }

      button->state = reading;
    }
  }

  button->previousState = reading;
}

void setup() {
  pinMode(keys[0].pin, INPUT);
  Keyboard.begin();
}

void loop() {
  //for(int i=0; i<keyCount; i++) {
    update(&keys[0]);
  //}
}

