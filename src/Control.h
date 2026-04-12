#ifndef CONTROL_H
#define CONTROL_H

#include "vendor/AutoTunePID.h"
#include "pwm.h"

enum class TemperatureTarget {
  BT,
  ET,
  MAX
};

class Control {
private:
  AutoTunePID _autotune;
  TemperatureTarget _temperatureTarget;
  PwmOutput _fan;
  PwmOutput _heater;
  const uint8_t noUpdateBeforeMs = 20; // 50 Hz
  unsigned long lastUpdate;
  bool tuningEnabled;
  bool hasResults;

  // Private helper methods
  float getTemperature(const float etbt[3]) const;

public:
  Control(float kp = 0, float ki = 0, float kd = 0);
  ~Control() = default;

  // Setup and initialization
  void setup(float kp, float ki, float kd);

  // PID gain configuration
  void setPidValues(float kp, float ki, float kd);
  float getKp() const;
  float getKi() const;
  float getKd() const;

  // Setpoint control
  void setSetpoint(float setpoint);
  float getSetpoint() const;

  // Heater control
  void setHeater(float value);
  float getHeater() const;

  // Fan control
  void setFan(float value);
  float getFan() const;

  // Temperature target selection
  void setTemperatureTarget(TemperatureTarget target);
  const char* getTemperatureTarget() const;

  // Operational mode
  void setMode(OperationalMode mode);
  const char* getMode() const;

  // Autotuning
  void startAutotune();
  void resetAutotune();
  bool hasAutotuneResults() const;

  // Main control loop
  void temperatureLoop(float etbt[3]);
};

#endif // CONTROL_H
