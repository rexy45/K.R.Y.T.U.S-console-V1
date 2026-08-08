#pragma once
// ============================================================
// TestGame ("KRYTUS TEST")
// Minimal built-in game proving DISPLAY + INPUT + GAME LOOP +
// GAME MANAGER + SAVE SYSTEM all work together end to end.
//
// UP/DOWN move the player square vertically to dodge obstacles
// that scroll in from the right. Score increases as obstacles
// are dodged. Colliding ends the game; A retries, B (handled by
// GameManager) exits to the menu.
// ============================================================
#include "Game.h"

class TestGame : public Game {
public:
  const char* name() override { return "KRYTUS TEST"; }

  void init() override;
  void update(InputManager& input) override;
  void render(DisplayManager& display) override;
  void shutdown() override;

  bool isGameOver() override { return gameOver; }
  uint32_t getScore() override { return score; }

private:
  static const int PLAYER_SIZE = 8;
  static const int MAX_OBSTACLES = 4;

  struct Obstacle {
    int x = 0, y = 0;
    bool active = false;
  };

  int playerX = 20;
  int playerY = 0;
  Obstacle obstacles[MAX_OBSTACLES];

  uint32_t score = 0;
  unsigned long lastSpawn = 0;
  unsigned long lastMove = 0;
  bool gameOver = false;
};
