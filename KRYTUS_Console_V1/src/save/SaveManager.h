#pragma once
// ============================================================
// SaveManager
// Public save/load API used by the console and games. Backed
// by StorageManager (NVS) but callers never need to know that.
//
// Save data is versioned and checksummed: a corrupted save is
// detected and rejected rather than bricking the console.
// ============================================================
#include <Arduino.h>
#include "../hardware/StorageManager.h"
#include "../config.h"

struct SaveData {
  uint8_t  version;
  uint32_t highScore;
  uint32_t checksum;
};

class SaveManager {
public:
  bool begin();

  bool hasSave();
  bool saveGame(uint32_t highScore);
  bool loadGame(SaveData& out);
  bool deleteSave();

private:
  StorageManager storage;
  static const char* KEY;
  uint32_t computeChecksum(const SaveData& data);
};
