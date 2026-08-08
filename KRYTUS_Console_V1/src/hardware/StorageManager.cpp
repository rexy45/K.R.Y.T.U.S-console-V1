#include "StorageManager.h"

bool StorageManager::begin(const char* namespaceName) {
  ready = prefs.begin(namespaceName, false);
  return ready;
}

void StorageManager::end() {
  if (ready) prefs.end();
  ready = false;
}

bool StorageManager::writeBytes(const char* key, const void* data, size_t len) {
  if (!ready) return false;
  return prefs.putBytes(key, data, len) == len;
}

size_t StorageManager::readBytes(const char* key, void* buffer, size_t maxLen) {
  if (!ready) return 0;
  return prefs.getBytes(key, buffer, maxLen);
}

bool StorageManager::hasKey(const char* key) {
  if (!ready) return false;
  return prefs.isKey(key);
}

bool StorageManager::remove(const char* key) {
  if (!ready) return false;
  return prefs.remove(key);
}
