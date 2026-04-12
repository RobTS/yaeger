#ifndef PWM_H
#define PWM_H

#include "vendor/ESP32_EnhancedPWM.h"

class PwmOutput {
public:
  PwmOutput(int pin, float frequency);
  ~PwmOutput() = default;

  void setValue(float power);
  float getValue() const;
};

#endif
