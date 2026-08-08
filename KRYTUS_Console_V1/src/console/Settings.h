#pragma once
#include "../hardware/DisplayManager.h"
#include "../hardware/InputManager.h"
#include "../save/SaveManager.h"
#include "AppState.h"

class Settings {
public:
  void reset();
  void render(DisplayManager& display);
  AppState handleInput(InputManager& input, DisplayManager& display, SaveManager& saveManager);

private:
  enum class SubScreen { LIST, DISPLAY, SYSTEM, ABOUT, CONFIRM_RESET };
  SubScreen sub = SubScreen::LIST;
  int selected = 0;
  int brightness = 100;
};
