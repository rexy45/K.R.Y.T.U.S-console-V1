#pragma once
#include "../hardware/DisplayManager.h"
#include "../hardware/InputManager.h"
#include "AppState.h"

class SystemInfo {
public:
  void render(DisplayManager& display);
  AppState handleInput(InputManager& input);
};
