#include "SystemInfo.h"
#include "../config.h"
#include <Arduino.h>

void SystemInfo::render(DisplayManager& display) {
  display.clear(TFT_BLACK);
  display.centerText("SYSTEM INFO", 4, TFT_CYAN, 1);

  char buf[32];
  int y = 24;
  const int lineH = 12;

  snprintf(buf, sizeof(buf), "CHIP: %s", ESP.getChipModel());
  display.text(buf, 4, y, TFT_WHITE, 1); y += lineH;

  snprintf(buf, sizeof(buf), "CPU: %d MHz", ESP.getCpuFreqMHz());
  display.text(buf, 4, y, TFT_WHITE, 1); y += lineH;

  snprintf(buf, sizeof(buf), "FLASH: %u KB", (unsigned)(ESP.getFlashChipSize() / 1024));
  display.text(buf, 4, y, TFT_WHITE, 1); y += lineH;

  snprintf(buf, sizeof(buf), "PSRAM: %u KB", (unsigned)(ESP.getPsramSize() / 1024));
  display.text(buf, 4, y, TFT_WHITE, 1); y += lineH;

  snprintf(buf, sizeof(buf), "FREE HEAP: %u KB", (unsigned)(ESP.getFreeHeap() / 1024));
  display.text(buf, 4, y, TFT_WHITE, 1); y += lineH;

  snprintf(buf, sizeof(buf), "FW: %s", FIRMWARE_VERSION);
  display.text(buf, 4, y, TFT_WHITE, 1); y += lineH;

  display.text("B: BACK", 4, 145, TFT_YELLOW, 1);
}

AppState SystemInfo::handleInput(InputManager& input) {
  if (input.b()) return AppState::MENU;
  return AppState::SYSTEM_INFO;
}
