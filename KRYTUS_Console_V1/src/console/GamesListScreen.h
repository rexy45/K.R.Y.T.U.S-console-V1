#pragma once
#include "../hardware/DisplayManager.h"
#include "../hardware/InputManager.h"
#include "GameManager.h"
#include "AppState.h"

class GamesListScreen {
public:
  void reset();
  void render(DisplayManager& display, GameManager& gameManager);
  AppState handleInput(InputManager& input, GameManager& gameManager);

private:
  int selected = 0;
};
