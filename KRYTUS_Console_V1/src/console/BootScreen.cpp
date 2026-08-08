#include "BootScreen.h"
#include "../config.h"

void BootScreen::show(DisplayManager& display) {
  display.clear(TFT_BLACK);
  display.centerText("K.R.Y.T.U.S", 60, TFT_CYAN, 1);
  display.centerText("CONSOLE V1", 80, TFT_WHITE, 1);
  delay(1500);
}
