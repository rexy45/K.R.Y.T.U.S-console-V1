#include "GameManager.h"

void GameManager::registerGame(Game* game) {
  if (count < MAX_GAMES) games[count++] = game;
}

int GameManager::gameCount() { return count; }

const char* GameManager::gameNameAt(int index) {
  if (index < 0 || index >= count) return "";
  return games[index]->name();
}

void GameManager::launch(int index) {
  if (index < 0 || index >= count) return;
  activeIndex = index;
  exitRequested = false;
  scoreSaved = false;
  games[activeIndex]->init();
}

void GameManager::launchDefault() {
  if (count > 0) launch(0);
}

void GameManager::update(InputManager& input, SaveManager& saveManager) {
  if (activeIndex < 0) return;
  Game* g = games[activeIndex];

  if (input.b()) {
    g->shutdown();
    exitRequested = true;
    return;
  }

  g->update(input);

  if (g->isGameOver() && !scoreSaved) {
    SaveData current;
    uint32_t best = 0;
    if (saveManager.loadGame(current)) best = current.highScore;

    uint32_t score = g->getScore();
    if (score > best) {
      saveManager.saveGame(score); // only writes to flash when it matters
    }
    scoreSaved = true;
  }
}

void GameManager::render(DisplayManager& display) {
  if (activeIndex < 0) return;
  games[activeIndex]->render(display);
}

bool GameManager::wantsExit() { return exitRequested; }
void GameManager::resetExitFlag() { exitRequested = false; }
