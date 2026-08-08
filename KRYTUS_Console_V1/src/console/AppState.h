#pragma once
// Top-level application states for the KRYTUS console state machine.
enum class AppState {
  BOOT,
  MENU,
  GAMES_LIST,
  IN_GAME,
  SAVE_DATA,
  SETTINGS,
  SYSTEM_INFO
};
