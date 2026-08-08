#pragma once
#include "../hardware/DisplayManager.h"
#include "../hardware/InputManager.h"
#include "../save/SaveManager.h"
#include "AppState.h"

class SaveDataScreen {
public:
  void render(DisplayManager& display, SaveManager& saveManager);
  AppState handleInput(InputManager& input, SaveManager& saveManager);
};
