#pragma once
// ============================================================
// Game
// Abstract base every KRYTUS game implements. GameManager only
// ever talks to games through this interface, so new games can
// be added later without touching the console.
// ============================================================
#include "../hardware/DisplayManager.h"
#include "../hardware/InputManager.h"

class Game {
public:
  virtual ~Game() {}

  virtual const char* name() = 0;
  virtual void init() = 0;
  virtual void update(InputManager& input) = 0;
  virtual void render(DisplayManager& display) = 0;
  virtual void shutdown() = 0;

  // Optional hooks GameManager uses for save integration.
  virtual bool isGameOver() { return false; }
  virtual uint32_t getScore() { return 0; }
};
