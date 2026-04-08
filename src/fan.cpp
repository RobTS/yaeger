
#include <ESP32_EnhancedPWM.h>
#include "config.h"

ESP32_EnhancedPWM fanPwm;

int currentFanSpeed = 0;

void initFan() {
  fanPwm.begin(FAN_PIN, 1, 20000, 8, false, LEDC_AUTO_CLK);
  fanPwm.setDutyNormalized(0);
}

void setFanSpeed(int power) {
  currentFanSpeed = max(min(power,100),0);
  fanPwm.setDutyNormalized(currentFanSpeed * 0.01f);
}

int getFanSpeed() {
  return currentFanSpeed;
}
