static const unsigned long DEBOUNCE_DELAY = 10;
static const unsigned long CHORDING_DELAY = 50;

typedef int KeyEvent;

static const KeyEvent NOTHING_HAPPENED = 0;
static const KeyEvent JUST_PRESSED     = 1;
static const KeyEvent JUST_RELEASED    = 2;

static const int PRESSED  = HIGH;
static const int RELEASED = LOW;

typedef struct {
  int pin;
  char code;
  boolean isModifier;
  unsigned long lastDebounceTime;
  int previousState;
  int state;
  KeyEvent event;
  int pressTime;
} Key;

Key keys[] = {
  { 2, (char) KEY_LEFT_SHIFT, true },
  { 3, 'a', false },
  { 4, 'b', false}
};

static const int keyCount = sizeof(keys)/sizeof(Key);

void updateEvents(Key *button) {
  button->event = NOTHING_HAPPENED;

  int reading = digitalRead(button->pin);

  if (reading != button->previousState) {
    button->lastDebounceTime = millis();
  }

  if ((millis() - button->lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != button->state) {
      if ((reading == HIGH) && (button->state == LOW)) {
        button->event = JUST_PRESSED;
      }

      if ((reading == LOW) && (button->state == HIGH)) {
        button->event = JUST_RELEASED;
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
    keys[i].event = NOTHING_HAPPENED;
    keys[i].pressTime = 0;
    pinMode(keys[i].pin, INPUT);
  }
  Keyboard.begin();
}

void handleModifier(Key *key) {
  switch(key->event) {
    case JUST_PRESSED:
      Keyboard.press(key->code);
      break;
    case JUST_RELEASED:
      Keyboard.release(key->code);
      break;
  }
}

void handleNonModifier(Key *key) {
  switch(key->event) {
    case JUST_PRESSED:
      key->pressTime = millis();
      break;
    case JUST_RELEASED:
      Keyboard.release(key->code);
      key->pressTime = 0;
      break;
    default:
      if (key->state == PRESSED) {
        if (millis() - key->pressTime > CHORDING_DELAY) {
          Keyboard.press(key->code);
        }
      }
  }
}

void loop() {
  for(int i=0; i<keyCount; i++) {
    updateEvents(&keys[i]);
  }

  for(int i=0; i<keyCount; i++) {
    Key *key = &keys[i];
    if (key->isModifier) {
      handleModifier(key);
    } else {
      handleNonModifier(key);
    }
  }
}
