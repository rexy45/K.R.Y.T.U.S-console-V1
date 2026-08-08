#include "GamesListScreen.h"
#include "../config.h"

void GamesListScreen::reset() { selected = 0; }

void GamesListScreen::render(DisplayManager& display, GameManager& gameManager) {
  display.clear(TFT_BLACK);
  display.centerText("GAMES", 6, TFT_CYAN, 1);

  int count = gameManager.gameCount();
  if (count == 0) {
    display.text("NO GAMES", 8, 40, TFT_WHITE, 1);
  } else {
    for (int i = 0; i < count; i++) {
      uint16_t color = (i == selected) ? TFT_YELLOW : TFT_WHITE;
      char label[24];
      snprintf(label, sizeof(label), "%s%s", (i == selected) ? "> " : "  ", gameManager.gameNameAt(i));
      display.text(label, 8, 30 + i * 16, color, 1);
    }
  }
  display.text("B: BACK", 8, 145, TFT_WHITE, 1);
}

AppState GamesListScreen::handleInput(InputManager& input, GameManager& gameManager) {
  int count = gameManager.gameCount();
  if (count > 0) {
    if (input.up())   selected = (selected - 1 + count) % count;
    if (input.down()) selected = (selected + 1) % count;
    if (input.a()) {
      gameManager.launch(selected);
      return AppState::IN_GAME;
    }
  }
  if (input.b()) return AppState::MENU;
  return AppState::GAMES_LIST;
}
