#include "SaveDataScreen.h"
#include "../config.h"

void SaveDataScreen::render(DisplayManager& display, SaveManager& saveManager) {
  display.clear(TFT_BLACK);
  display.centerText("SAVE DATA", 6, TFT_CYAN, 1);

  if (saveManager.hasSave()) {
    SaveData data;
    if (saveManager.loadGame(data)) {
      char buf[24];
      snprintf(buf, sizeof(buf), "HIGH SCORE: %lu", (unsigned long)data.highScore);
      display.text(buf, 8, 40, TFT_WHITE, 1);
      display.text("A: DELETE SAVE", 8, 70, TFT_WHITE, 1);
    } else {
      // Corrupted save -- never brick the console, just tell the user.
      display.text("SAVE DATA ERROR", 8, 40, TFT_RED, 1);
      display.text("A: CLEAR + NEW SAVE", 8, 60, TFT_WHITE, 1);
    }
  } else {
    display.text("NO SAVE DATA", 8, 40, TFT_WHITE, 1);
  }

  display.text("B: BACK", 8, 145, TFT_YELLOW, 1);
}

AppState SaveDataScreen::handleInput(InputManager& input, SaveManager& saveManager) {
  if (input.a() && saveManager.hasSave()) {
    saveManager.deleteSave(); // handles both "delete existing" and
                               // "clear corrupted entry" cases
  }
  if (input.b()) return AppState::MENU;
  return AppState::SAVE_DATA;
}
