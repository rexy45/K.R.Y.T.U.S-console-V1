#pragma once
// ============================================================
// InputManager
// Debounced abstraction over the 4 physical buttons.
// Buttons are active LOW (pressed = LOW, released = HIGH).
//
// up()/down()/a()/b()      -> edge-triggered: true exactly once
//                              per physical press (menu navigation)
// upHeld()/downHeld()/etc. -> raw held state (in-game movement)
//
// Nothing outside this class should ever call digitalRead()
// directly on a button pin.
// ============================================================
#include <Arduino.h>
#include "../config.h"

class InputManager {
public:
  void begin();
  void update(); // call once per loop() iteration, before reading state

  bool up();
  bool down();
  bool a();
  bool b();

  bool upHeld();
  bool downHeld();
  bool aHeld();
  bool bHeld();

private:
  struct ButtonState {
    uint8_t pin = 0;
    bool stable = true;       // debounced state, HIGH = released
    bool lastReading = true;
    unsigned long lastChangeMs = 0;
    bool edgeTriggered = false;
  };

  ButtonState btnUp, btnDown, btnA, btnB;

  void updateButton(ButtonState& btn);
  bool consumeEdge(ButtonState& btn);
  bool isHeld(ButtonState& btn);
};
