static const unsigned long DEBOUNCE_DELAY = 10;
static const unsigned long CHORDING_DELAY = 40;

typedef int KeyEvent;
static const KeyEvent NOTHING_HAPPENED = 0;
static const KeyEvent JUST_PRESSED     = 1;
static const KeyEvent JUST_RELEASED    = 2;

static const int PRESSED  = HIGH;
static const int RELEASED = LOW;

typedef int ModifierState;
static const ModifierState OFF                  = 0;
static const ModifierState AWAITING_STICKY      = 1;
static const ModifierState STUCK_AWAITING_LOCK  = 2;
static const ModifierState HELD_AWAITING_LOCK   = 3;
static const ModifierState STUCK                = 4;
static const ModifierState HELD                 = 5;

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

Key allKeys[] = {
  { 2, (char) KEY_LEFT_SHIFT, true, 0 },

  { 3, ' ', false, 0b0000000000000001 },
  { 4, ' ', false, 0b0000000000000010 }
};
static const int allKeyCount = sizeof(allKeys)/sizeof(Key);

Key *modifiers[] = {
  &allKeys[0]
};
static const int modifierCount = sizeof(modifiers)/sizeof(Key*);

Key *keys[] = {
  &allKeys[1],
  &allKeys[2]
};
static const int keyCount = sizeof(keys)/sizeof(Key*);

static const int NO_KEY_PRESSED = 0;

char chordMap[4];
unsigned long lastChordChangeTime;
int previousChord;
int chord;
boolean waitingForChord;

void updateEvents() {
  for(int i=0; i<allKeyCount; i++) {
    Key *key = &allKeys[i];
    key->event = NOTHING_HAPPENED;

    int reading = digitalRead(key->pin);

    if (reading != key->previousState) {
      key->lastDebounceTime = millis();
    }

    if ((millis() - key->lastDebounceTime) > DEBOUNCE_DELAY) {
      if (reading != key->state) {
        if ((reading == HIGH) && (key->state == LOW)) {
          key->event = JUST_PRESSED;
        }

        if ((reading == LOW) && (key->state == HIGH)) {
          key->event = JUST_RELEASED;
        }

        key->state = reading;
      }
    }

    key->previousState = reading;
  }
}

void setup() {
  for(int i=0; i<allKeyCount; i++) {
    allKeys[i].lastDebounceTime = 0;
    allKeys[i].previousState = LOW;
    allKeys[i].state = LOW;
    allKeys[i].event = NOTHING_HAPPENED;
    pinMode(allKeys[i].pin, INPUT);
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

void handleModifiers() {
  for(int i=0; i<modifierCount; i++) {
    Key *key = modifiers[i];
    switch(key->event) {
      case JUST_PRESSED:
        Keyboard.press(key->code);
        break;
      case JUST_RELEASED:
        Keyboard.release(key->code);
        break;
    }
  }
}

void handleKeys() {
  for(int i=0; i<keyCount; i++) {
    Key *key = keys[i];
    switch(key->event) {
      case JUST_PRESSED:
        chord = chord | key->mask;
        break;
      case JUST_RELEASED:
        chord = chord ^ key->mask;
        break;
    }
  }
}

void processChord() {
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

void loop() {
  updateEvents();

  handleModifiers();
  handleKeys();

  processChord();
}
