#include "SaveManager.h"

const char* SaveManager::KEY = "save";

bool SaveManager::begin() {
  return storage.begin("krytus");
}

uint32_t SaveManager::computeChecksum(const SaveData& data) {
  // Simple, fast integrity check -- not cryptographic, just enough
  // to catch flash corruption / partial writes.
  uint32_t sum = 0;
  sum += data.version;
  sum += data.highScore * 2654435761u; // Knuth multiplicative hash constant
  return sum;
}

bool SaveManager::hasSave() {
  return storage.hasKey(KEY);
}

bool SaveManager::saveGame(uint32_t highScore) {
  SaveData data;
  data.version = SAVE_VERSION;
  data.highScore = highScore;
  data.checksum = computeChecksum(data);
  return storage.writeBytes(KEY, &data, sizeof(SaveData));
}

bool SaveManager::loadGame(SaveData& out) {
  SaveData temp;
  size_t bytesRead = storage.readBytes(KEY, &temp, sizeof(SaveData));

  if (bytesRead != sizeof(SaveData)) return false;      // missing/short read
  if (temp.version != SAVE_VERSION) return false;       // future/old format
  if (temp.checksum != computeChecksum(temp)) return false; // corrupted

  out = temp;
  return true;
}

bool SaveManager::deleteSave() {
  return storage.remove(KEY);
}
