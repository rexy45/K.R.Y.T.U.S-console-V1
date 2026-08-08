#include "Settings.h"
#include "../config.h"

void Settings::reset() {
  sub = SubScreen::LIST;
  selected = 0;
}

void Settings::render(DisplayManager& display) {
  display.clear(TFT_BLACK);

  switch (sub) {
    case SubScreen::LIST: {
      display.centerText("SETTINGS", 6, TFT_CYAN, 1);
      const char* items[3] = {"DISPLAY", "SYSTEM", "ABOUT"};
      for (int i = 0; i < 3; i++) {
        uint16_t color = (i == selected) ? TFT_YELLOW : TFT_WHITE;
        char label[16];
        snprintf(label, sizeof(label), "%s%s", (i == selected) ? "> " : "  ", items[i]);
        display.text(label, 8, 30 + i * 16, color, 1);
      }
      display.text("B: BACK", 8, 145, TFT_WHITE, 1);
      break;
    }
    case SubScreen::DISPLAY: {
      display.centerText("DISPLAY", 6, TFT_CYAN, 1);
      char buf[24];
      snprintf(buf, sizeof(buf), "BRIGHTNESS: %d%%", brightness);
      display.text(buf, 8, 40, TFT_WHITE, 1);
      display.text("UP/DOWN: ADJUST", 8, 60, TFT_WHITE, 1);
      display.text("B: BACK", 8, 145, TFT_WHITE, 1);
      break;
    }
    case SubScreen::SYSTEM: {
      display.centerText("SYSTEM", 6, TFT_CYAN, 1);
      char buf[24];
      snprintf(buf, sizeof(buf), "FIRMWARE: %s", FIRMWARE_VERSION);
      display.text(buf, 8, 40, TFT_WHITE, 1);
      display.text("A: RESET SAVE DATA", 8, 60, TFT_WHITE, 1);
      display.text("B: BACK", 8, 145, TFT_WHITE, 1);
      break;
    }
    case SubScreen::ABOUT: {
      display.centerText("ABOUT", 6, TFT_CYAN, 1);
      display.text("KRYTUS CONSOLE V1", 8, 40, TFT_WHITE, 1);
      display.text("ESP32-S3", 8, 56, TFT_WHITE, 1);
      display.text("128x160 ST7735", 8, 72, TFT_WHITE, 1);
      display.text("B: BACK", 8, 145, TFT_WHITE, 1);
      break;
    }
    case SubScreen::CONFIRM_RESET: {
      display.centerText("RESET SAVE DATA?", 50, TFT_RED, 1);
      display.centerText("A: CONFIRM", 70, TFT_WHITE, 1);
      display.centerText("B: CANCEL", 86, TFT_WHITE, 1);
      break;
    }
  }
}

AppState Settings::handleInput(InputManager& input, DisplayManager& display, SaveManager& saveManager) {
  switch (sub) {
    case SubScreen::LIST:
      if (input.up())   selected = (selected - 1 + 3) % 3;
      if (input.down()) selected = (selected + 1) % 3;
      if (input.a()) sub = (selected == 0) ? SubScreen::DISPLAY
                          : (selected == 1) ? SubScreen::SYSTEM
                                             : SubScreen::ABOUT;
      if (input.b()) return AppState::MENU;
      break;

    case SubScreen::DISPLAY:
      if (input.up())   { brightness = min(100, brightness + 10); display.setBrightness(brightness); }
      if (input.down()) { brightness = max(0, brightness - 10);   display.setBrightness(brightness); }
      if (input.b()) sub = SubScreen::LIST;
      break;

    case SubScreen::SYSTEM:
      if (input.a()) sub = SubScreen::CONFIRM_RESET;
      if (input.b()) sub = SubScreen::LIST;
      break;

    case SubScreen::ABOUT:
      if (input.b()) sub = SubScreen::LIST;
      break;

    case SubScreen::CONFIRM_RESET:
      if (input.a()) { saveManager.deleteSave(); sub = SubScreen::SYSTEM; }
      if (input.b()) sub = SubScreen::SYSTEM;
      break;
  }
  return AppState::SETTINGS;
}
