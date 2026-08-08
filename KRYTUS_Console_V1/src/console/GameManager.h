#pragma once
// ============================================================
// GameManager
// Registers games, launches them, drives their update/render
// each frame, and hands off high scores to SaveManager. New
// games are added with a single registerGame() call -- nothing
// else in the console needs to change.
// ============================================================
#include "../games/Game.h"
#include "../hardware/DisplayManager.h"
#include "../hardware/InputManager.h"
#include "../save/SaveManager.h"

class GameManager {
public:
  void registerGame(Game* game);
  int gameCount();
  const char* gameNameAt(int index);

  void launch(int index);
  void launchDefault();

  void update(InputManager& input, SaveManager& saveManager);
  void render(DisplayManager& display);

  bool wantsExit();
  void resetExitFlag();

private:
  static const int MAX_GAMES = 8;
  Game* games[MAX_GAMES] = {};
  int count = 0;
  int activeIndex = -1;
  bool exitRequested = false;
  bool scoreSaved = false;
};
