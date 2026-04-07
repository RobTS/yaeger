#include "RoasterPrefs.h"
#include <Preferences.h>

Preferences preferences;

void setupPreferences() {
  preferences.begin("preferences");
}

void setDoubleValue(const char* name, double value) {
  preferences.putDouble(name, value);
}

double getDoubleValue(const char* name, double defaultValue) {
  return preferences.getDouble(name, defaultValue);
}

void setLongValue(const char* name, long value) {
  preferences.putDouble(name, value);
}

long getLongValue(const char* name, long defaultValue) {
  return preferences.getLong(name, defaultValue);
}
