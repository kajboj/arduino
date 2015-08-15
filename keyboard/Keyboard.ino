static const unsigned long DEBOUNCE_DELAY = 10;
static const unsigned long CHORDING_DELAY = 30;

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
  int mask;
  unsigned long lastDebounceTime;
  int previousState;
  int state;
  KeyEvent event;
} Key;

Key keys[] = {
  { 2, (char) KEY_LEFT_SHIFT, true, 0 },

  { 3, ' ', false, 0b0000000000000001 },
  { 4, ' ', false, 0b0000000000000010 }
};

static const int keyCount = sizeof(keys)/sizeof(Key);
static const int NO_KEY_PRESSED = 0;

char chordMap[4];
unsigned long lastChordChangeTime;
int previousChord;
int chord;
boolean waitingForChord;

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
    pinMode(keys[i].pin, INPUT);
  }

  chordMap[0b0000000000000001] = 'a';
  chordMap[0b0000000000000010] = 'b';
  chordMap[0b0000000000000011] = 'c';

  lastChordChangeTime = 0;
  previousChord = 0;
  chord = 0;
  waitingForChord = false;

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
      chord = chord | key->mask;
      break;
    case JUST_RELEASED:
      chord = chord ^ key->mask;
      break;
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

  if (chord != previousChord) {
    if (previousChord != NO_KEY_PRESSED) {
      Keyboard.release(chordMap[previousChord]);
    }

    lastChordChangeTime = millis();

    if (chord != NO_KEY_PRESSED) {
      waitingForChord = true;
    }
    
    previousChord = chord;
  } else {
    if (waitingForChord) {
      if (millis() - lastChordChangeTime > CHORDING_DELAY) {
        Keyboard.press(chordMap[chord]);
        waitingForChord = false;
      }
    }
  }
}
