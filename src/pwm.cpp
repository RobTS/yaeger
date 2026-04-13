#include "pwm.h"
#include <Arduino.h>
#include <string>

PwmOutput::PwmOutput(int pin, int frequencyInput, int dutyResolution, int channel)
: _pin(pin), _currentValue(0.f), _maxDuty(pow(2, dutyResolution)) {
  ledcAttachChannel(pin, frequencyInput, dutyResolution, channel);
}

void PwmOutput::setValue(float power) {
  this->_currentValue = max(min(power,100.f),0.f);
  ledcWrite(this->_pin, this->_currentValue / 100.f * this->_maxDuty  );
}

float PwmOutput::getValue() const {
  return this->_currentValue;
}
