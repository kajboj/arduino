/*
 The circuit:
 * pushbutton attached from pin 4 to +5V
 * 10-kilohm resistor attached from pin 4 to ground
 */

typedef int KeyEvent;

static const KeyEvent NOTHING  = 0;
static const KeyEvent PRESSED  = 1;
static const KeyEvent RELEASED = 2;

typedef struct {
  int pin;
  char code;
  unsigned long lastDebounceTime;
  int previousState;
  int state;
  KeyEvent event;
} Key;

Key keys[] = {
  { 2, (char) KEY_LEFT_SHIFT },
  { 3, 'a' },
  { 4, 'b' }
};

static const int keyCount = sizeof(keys)/sizeof(Key);
static const unsigned long DEBOUNCE_DELAY = 10;

void updateEvents(Key *button) {
  button->event = NOTHING;

  int reading = digitalRead(button->pin);

  if (reading != button->previousState) {
    button->lastDebounceTime = millis();
  }

  if ((millis() - button->lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != button->state) {
      if ((reading == HIGH) && (button->state == LOW)) {
        button->event = PRESSED;
      }

      if ((reading == LOW) && (button->state == HIGH)) {
        button->event = RELEASED;
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
    keys[i].event = NOTHING;
    pinMode(keys[i].pin, INPUT);
  }
  Keyboard.begin();
}

void loop() {
  for(int i=0; i<keyCount; i++) {
    updateEvents(&keys[i]);
  }

  for(int i=0; i<keyCount; i++) {
    Key *key = &keys[i];
    switch(key->event) {
      case PRESSED:
        Keyboard.press(key->code);
        break;
      case RELEASED:
        Keyboard.release(key->code);
        break;
    }
  }
}
