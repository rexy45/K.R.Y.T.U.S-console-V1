#pragma once
// ============================================================
// DisplayManager
// Wraps TFT_eSPI so the rest of the console never touches
// TFT_eSPI directly. All colors are RGB565.
// ============================================================
#include <TFT_eSPI.h>
#include "../config.h"

class DisplayManager {
public:
  void begin();

  void clear(uint16_t color = TFT_BLACK);
  void text(const char* str, int x, int y, uint16_t color = TFT_WHITE, uint8_t size = 1);
  void centerText(const char* str, int y, uint16_t color = TFT_WHITE, uint8_t size = 1);
  void rect(int x, int y, int w, int h, uint16_t color, bool filled = false);
  void pixel(int x, int y, uint16_t color);
  void bitmap(int x, int y, const uint16_t* data, int w, int h);

  // percent: 0-100
  void setBrightness(uint8_t percent);

  bool isOk() const { return ok; }

private:
  TFT_eSPI tft = TFT_eSPI();
  bool ok = false;
  uint8_t brightnessPercent = 100;
  static const int BL_PWM_CHANNEL = 0;
};
