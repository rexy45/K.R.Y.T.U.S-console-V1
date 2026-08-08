#include "InputManager.h"

void InputManager::begin() {
  btnUp.pin   = PIN_BTN_UP;
  btnDown.pin = PIN_BTN_DOWN;
  btnA.pin    = PIN_BTN_A;
  btnB.pin    = PIN_BTN_B;

  pinMode(btnUp.pin,   INPUT_PULLUP);
  pinMode(btnDown.pin, INPUT_PULLUP);
  pinMode(btnA.pin,    INPUT_PULLUP);
  pinMode(btnB.pin,    INPUT_PULLUP);
}

void InputManager::updateButton(ButtonState& btn) {
  bool reading = digitalRead(btn.pin);
  unsigned long now = millis();

  if (reading != btn.lastReading) {
    btn.lastChangeMs = now;
    btn.lastReading = reading;
  }

  if ((now - btn.lastChangeMs) > BTN_DEBOUNCE_MS) {
    if (btn.stable != reading) {
      btn.stable = reading;
      if (btn.stable == LOW) {
        btn.edgeTriggered = true; // new confirmed press
      }
    }
  }
}

void InputManager::update() {
  updateButton(btnUp);
  updateButton(btnDown);
  updateButton(btnA);
  updateButton(btnB);
}

bool InputManager::consumeEdge(ButtonState& btn) {
  if (btn.edgeTriggered) {
    btn.edgeTriggered = false;
    return true;
  }
  return false;
}

bool InputManager::isHeld(ButtonState& btn) {
  return btn.stable == LOW;
}

bool InputManager::up()   { return consumeEdge(btnUp); }
bool InputManager::down() { return consumeEdge(btnDown); }
bool InputManager::a()    { return consumeEdge(btnA); }
bool InputManager::b()    { return consumeEdge(btnB); }

bool InputManager::upHeld()   { return isHeld(btnUp); }
bool InputManager::downHeld() { return isHeld(btnDown); }
bool InputManager::aHeld()    { return isHeld(btnA); }
bool InputManager::bHeld()    { return isHeld(btnB); }
