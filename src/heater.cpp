#include "heater.h"
#include "config.h"
#include <Arduino.h>
#include <vendor/ESP32_EnhancedPWM.h>

ESP32_EnhancedPWM heaterPwm;

void initHeater() {
  heaterPwm.begin(HEATER_PIN, 0, 1, 8, false, LEDC_AUTO_CLK);
  heaterPwm.setDutyNormalized(0);
}

void setHeaterPower(float power) {
  heaterPwm.setDutyNormalized(power * 0.01f);
}
