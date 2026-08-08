#pragma once
// ============================================================
// KRYTUS Console V1 - Central hardware/config header
// All GPIO assignments and global constants live HERE ONLY.
// Nothing else in the codebase should hardcode a pin number.
// ============================================================
#include <Arduino.h>

// ---------------- Display (ST7735, 128x160, SPI) ----------------
// VCC/VDD -> 3V3   GND -> GND   MISO -> NOT CONNECTED
#define PIN_TFT_SCLK   7   // D8
#define PIN_TFT_MOSI   9   // D10
#define PIN_TFT_CS     2   // D1
#define PIN_TFT_DC     3   // D2
#define PIN_TFT_RST    4   // D3
#define PIN_TFT_BL     5   // D4

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  160

// ---------------- Buttons (active LOW, internal pull-ups) -------
#define PIN_BTN_UP     1
#define PIN_BTN_DOWN   6
#define PIN_BTN_A      44
#define PIN_BTN_B      43

#define BTN_DEBOUNCE_MS 30

// ---------------- Firmware / save versioning ---------------------
#define FIRMWARE_VERSION "1.0.0"
#define SAVE_VERSION      1
