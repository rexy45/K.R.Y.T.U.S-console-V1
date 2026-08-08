#pragma once
// ============================================================
// StorageManager
// Thin abstraction over ESP32 NVS (via Preferences). Nothing
// above this layer knows or cares that flash/NVS is used --
// games and the console only ever talk to SaveManager, which
// talks to this.
// ============================================================
#include <Preferences.h>

class StorageManager {
public:
  bool begin(const char* namespaceName);
  void end();

  bool writeBytes(const char* key, const void* data, size_t len);
  size_t readBytes(const char* key, void* buffer, size_t maxLen);
  bool hasKey(const char* key);
  bool remove(const char* key);

  bool isReady() const { return ready; }

private:
  Preferences prefs;
  bool ready = false;
};
