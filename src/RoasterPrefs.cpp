#include "RoasterPrefs.h"
#include <Preferences.h>

Preferences preferences;

void setupPreferences() {
  preferences.begin("preferences-v1");
}

void setFloatValue(const char* name, float value) {
  preferences.putFloat(name, value);
}

float getFloatValue(const char* name, float defaultValue) {
  return preferences.getFloat(name, defaultValue);
}

void setLongValue(const char* name, long value) {
  preferences.putLong(name, value);
}

long getLongValue(const char* name, long defaultValue) {
  return preferences.getLong(name, defaultValue);
}
