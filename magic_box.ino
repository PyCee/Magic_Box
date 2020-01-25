#include <ArduinoLowPower.h>
const char numDeadlocks = 2;
const char deadlockOutputPins[] = {0, 1};
const char capacitiveSensorPin = 2;

const unsigned long holdMs = 5000;

unsigned long startTime = 0;

enum State {
  Sleeping,
  FirstTouch,
  Open,
  SecondTouch
};
State lidState;

void setup() {

  // Set deadlock output pins to OUTPUT
  for (int i = 0; i < numDeadlocks; i++) {
    pinMode(deadlockOutputPins[i], OUTPUT);
  }

  pinMode(capacitiveSensorPin, INPUT);
  LowPower.attachInterruptWakeup(capacitiveSensorPin, onTouch, RISING);
  attachInterrupt(digitalPinToInterrupt(capacitiveSensorPin), onTouchRemove, FALLING);

  lidState = Sleeping;
  LowPower.deepSleep();
}

void loop() {
  switch (lidState) {
    case Sleeping:
      LowPower.deepSleep();
      break;
    case FirstTouch:
      // TODO update lights
      if (hasTimeElapsed()) {
        // TODO Undo locks
        lidState = Open;
      }
      break;
    case Open:
      // Does anything belong here?
      break;
    case SecondTouch:
      // TODO update lights
      if (hasTimeElapsed()) {
        // TODO Trigger locks
        lidState = Sleeping;
      }
      break;
  }
}

void resetTimer() {
  startTime = millis();
}
unsigned long hasTimeElapsed() {
  return (millis() - startTime) > holdMs;
}

void onTouch() {
  switch (lidState) {
    case Sleeping:
      lidState = FirstTouch;
      // Start first touch timer
      resetTimer();
      break;
    case Open:
      lidState = SecondTouch;
      // Start touch timer
      resetTimer();
      break;
  }
}
void onTouchRemove() {
  switch (lidState) {
    case FirstTouch:
      lidState = Sleeping;
      break;
    case SecondTouch:
      lidState = Open;
      break;
  }
}
