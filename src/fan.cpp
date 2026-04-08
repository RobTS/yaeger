
#include <ESP32_EnhancedPWM.h>
#include "config.h"

ESP32_EnhancedPWM fanPwm;

float currentFanSpeed = 0;

void initFan() {
  fanPwm.begin(FAN_PIN, 1, 20000, 8, false, LEDC_AUTO_CLK);
  fanPwm.setDutyNormalized(0);
}

void setFanSpeed(float power) {
  currentFanSpeed = max(min(power,100.f),0.f);
  fanPwm.setDutyNormalized(currentFanSpeed * 0.01f);
}

float getFanSpeed() {
  return currentFanSpeed;
}
