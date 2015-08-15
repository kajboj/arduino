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
  { 2, KEY_LEFT_SHIFT },
  { 3, 'a' },
  { 4, 'b' }
};

static const int keyCount = sizeof(keys)/sizeof(key);
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
  for(int i=0; i<keyCount; i++) {
    keys[i].lastDebounceTime = 0;
    keys[i].previousState = LOW;
    keys[i].state = LOW;
    pinMode(keys[i].pin, INPUT);
  }
  Keyboard.begin();
}

void loop() {
  for(int i=0; i<keyCount; i++) {
    update(&keys[i]);
  }
}

