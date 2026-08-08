#include "MainMenu.h"
#include "../config.h"

void MainMenu::reset() { selected = 0; }

void MainMenu::render(DisplayManager& display) {
  display.clear(TFT_BLACK);
  display.centerText("K R Y T U S", 10, TFT_CYAN, 1);

  int startY = 40;
  for (int i = 0; i < ITEM_COUNT; i++) {
    int y = startY + i * 16;
    uint16_t color = (i == selected) ? TFT_YELLOW : TFT_WHITE;
    char label[16];
    snprintf(label, sizeof(label), "%s%s", (i == selected) ? "> " : "  ", items[i]);
    display.text(label, 8, y, color, 1);
  }
}

AppState MainMenu::handleInput(InputManager& input) {
  if (input.up())   selected = (selected - 1 + ITEM_COUNT) % ITEM_COUNT;
  if (input.down()) selected = (selected + 1) % ITEM_COUNT;

  if (input.a()) {
    switch (selected) {
      case 0: return AppState::IN_GAME;    // PLAY -> launch default game
      case 1: return AppState::GAMES_LIST; // GAMES
      case 2: return AppState::SAVE_DATA;  // SAVE DATA
      case 3: return AppState::SETTINGS;   // SETTINGS
      case 4: return AppState::SYSTEM_INFO;// SYSTEM
    }
  }
  return AppState::MENU;
}
