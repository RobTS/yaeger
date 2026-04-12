#include "pwm.h"
#include "config.h"
#include <Arduino.h>
#include <vendor/ESP32_EnhancedPWM.h>
#include <string>

ESP32_EnhancedPWM enhancedPwm;

PwmOutput::PwmOutput(int pin, float frequencyInput) {
  enhancedPwm.begin(pin, 0, frequencyInput, 10, false, LEDC_AUTO_CLK);
  enhancedPwm.setDuty(0);
}

void PwmOutput::setValue(float power) {
  enhancedPwm.setDuty(power / 100.f);
}

float PwmOutput::getValue() const {
  return enhancedPwm.getDuty() * 100.f;
}
