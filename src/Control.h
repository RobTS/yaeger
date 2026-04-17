#ifndef CONTROL_H
#define CONTROL_H


#include "pwm.h"
#include "sensor.h"


class Control
{
private:
  PwmOutput _fan;
  PwmOutput _heater;
  Sensor _etSensor;
  Sensor _btSensor;
  const uint8_t noUpdateBeforeMs = 20; // 50 Hz
  unsigned long lastUpdate;
  bool tuningEnabled;
  bool hasResults;

public:
  Control(float kp, float ki, float kd);
  ~Control() = default;

  // Heater control
  void setHeater(float value);
  float getHeater() const;

  // Fan control
  void setFan(float value);
  float getFan() const;

  float getExhaustTemp() const;
  float getBeanTemp() const;
  float getAmbientTemp() const;


  // Main control loop
  void loop();
};

#endif // CONTROL_H
