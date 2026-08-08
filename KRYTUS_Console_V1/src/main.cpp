// ============================================================
// KRYTUS Console V1 - main.cpp
// Top-level state machine only. All real logic lives in
// hardware/, console/, games/, and save/.
// ============================================================
#include <Arduino.h>
#include "config.h"
#include "console/AppState.h"
#include "hardware/DisplayManager.h"
#include "hardware/InputManager.h"
#include "save/SaveManager.h"
#include "console/BootScreen.h"
#include "console/MainMenu.h"
#include "console/GameManager.h"
#include "console/GamesListScreen.h"
#include "console/SaveDataScreen.h"
#include "console/Settings.h"
#include "console/SystemInfo.h"
#include "games/TestGame.h"

static DisplayManager display;
static InputManager input;
static SaveManager saveManager;

static BootScreen bootScreen;
static MainMenu mainMenu;
static GameManager gameManager;
static GamesListScreen gamesListScreen;
static SaveDataScreen saveDataScreen;
static Settings settingsScreen;
static SystemInfo systemInfoScreen;

static TestGame testGame;

static AppState state = AppState::BOOT;

static void enterState(AppState newState) {
  state = newState;
  switch (state) {
    case AppState::MENU:       mainMenu.reset();       break;
    case AppState::GAMES_LIST: gamesListScreen.reset();break;
    case AppState::SETTINGS:   settingsScreen.reset(); break;
    default: break;
  }
}

void setup() {
  Serial.begin(115200);

  display.begin();
  input.begin();

  bool storageOk = saveManager.begin();
  if (!storageOk) {
    // Fail gracefully: keep booting, saving is simply disabled.
    display.clear(TFT_BLACK);
    display.centerText("STORAGE INIT", 60, TFT_RED, 1);
    display.centerText("FAILED", 76, TFT_RED, 1);
    display.centerText("SAVING DISABLED", 96, TFT_WHITE, 1);
    delay(2000);
  }

  gameManager.registerGame(&testGame);
  if (gameManager.gameCount() == 0) {
    // Should never happen, but fail gracefully instead of hanging.
    display.clear(TFT_BLACK);
    display.centerText("NO GAMES", 70, TFT_RED, 1);
    display.centerText("REGISTERED", 86, TFT_RED, 1);
    delay(2000);
  }

  bootScreen.show(display);
  enterState(AppState::MENU);
}

void loop() {
  input.update();

  switch (state) {
    case AppState::MENU: {
      AppState next = mainMenu.handleInput(input);
      if (next == AppState::IN_GAME) {
        if (gameManager.gameCount() > 0) {
          gameManager.launchDefault();
          enterState(AppState::IN_GAME);
        }
      } else if (next != state) {
        enterState(next);
      }
      mainMenu.render(display);
      break;
    }

    case AppState::GAMES_LIST: {
      AppState next = gamesListScreen.handleInput(input, gameManager);
      if (next != state) enterState(next);
      gamesListScreen.render(display, gameManager);
      break;
    }

    case AppState::IN_GAME: {
      gameManager.update(input, saveManager);
      if (gameManager.wantsExit()) {
        gameManager.resetExitFlag();
        enterState(AppState::MENU);
      } else {
        gameManager.render(display);
      }
      break;
    }

    case AppState::SAVE_DATA: {
      AppState next = saveDataScreen.handleInput(input, saveManager);
      if (next != state) enterState(next);
      saveDataScreen.render(display, saveManager);
      break;
    }

    case AppState::SETTINGS: {
      AppState next = settingsScreen.handleInput(input, display, saveManager);
      if (next != state) enterState(next);
      settingsScreen.render(display);
      break;
    }

    case AppState::SYSTEM_INFO: {
      AppState next = systemInfoScreen.handleInput(input);
      if (next != state) enterState(next);
      systemInfoScreen.render(display);
      break;
    }

    default:
      break;
  }

  delay(16); // ~60 FPS cap
}
