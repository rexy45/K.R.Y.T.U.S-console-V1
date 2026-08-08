#include "DisplayManager.h"
#include <string.h>

void DisplayManager::begin() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  ok = true; // TFT_eSPI has no return-code init failure path; a wiring
             // fault will simply show a blank screen. If display() calls
             // ever hang, callers should treat it as a hardware fault.

  pinMode(PIN_TFT_BL, OUTPUT);
#if defined(ARDUINO_ARCH_ESP32)
  ledcSetup(BL_PWM_CHANNEL, 5000, 8);
  ledcAttachPin(PIN_TFT_BL, BL_PWM_CHANNEL);
  ledcWrite(BL_PWM_CHANNEL, 255);
#else
  digitalWrite(PIN_TFT_BL, HIGH);
#endif
}

void DisplayManager::clear(uint16_t color) {
  tft.fillScreen(color);
}

void DisplayManager::text(const char* str, int x, int y, uint16_t color, uint8_t size) {
  tft.setTextColor(color, TFT_BLACK);
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.print(str);
}

void DisplayManager::centerText(const char* str, int y, uint16_t color, uint8_t size) {
  // Default GLCD font is 6px wide per character at size 1.
  int16_t w = 6 * size * (int16_t)strlen(str);
  int x = (SCREEN_WIDTH - w) / 2;
  if (x < 0) x = 0;
  text(str, x, y, color, size);
}

void DisplayManager::rect(int x, int y, int w, int h, uint16_t color, bool filled) {
  if (filled) tft.fillRect(x, y, w, h, color);
  else tft.drawRect(x, y, w, h, color);
}

void DisplayManager::pixel(int x, int y, uint16_t color) {
  tft.drawPixel(x, y, color);
}

void DisplayManager::bitmap(int x, int y, const uint16_t* data, int w, int h) {
  tft.pushImage(x, y, w, h, data);
}

void DisplayManager::setBrightness(uint8_t percent) {
  if (percent > 100) percent = 100;
  brightnessPercent = percent;
#if defined(ARDUINO_ARCH_ESP32)
  uint8_t duty = (uint16_t)percent * 255 / 100;
  ledcWrite(BL_PWM_CHANNEL, duty);
#else
  digitalWrite(PIN_TFT_BL, percent > 0 ? HIGH : LOW);
#endif
}
