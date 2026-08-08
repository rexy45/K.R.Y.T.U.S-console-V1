#pragma once
#include "../hardware/DisplayManager.h"
#include "../hardware/InputManager.h"
#include "AppState.h"

class MainMenu {
public:
  void reset();
  void render(DisplayManager& display);
  AppState handleInput(InputManager& input);

private:
  static const int ITEM_COUNT = 5;
  const char* items[ITEM_COUNT] = {"PLAY", "GAMES", "SAVE DATA", "SETTINGS", "SYSTEM"};
  int selected = 0;
};
