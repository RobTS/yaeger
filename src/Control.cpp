#include "Control.h"
#include "config.h"
#include <Arduino.h>

Control::Control(float kp, float ki, float kd)
  : lastUpdate(0),
    tuningEnabled(false),
    hasResults(false),
    _fan(FAN_PIN, FAN_FREQUENCY, 10, 0),
    _heater(HEATER_PIN, HEATER_FREQUENCY, 10, 1),
    _etSensor(MAX1CLK, MAX1CS, MAX1DO, "Exhaust"),
    _btSensor(MAX2CLK, MAX2CS, MAX2DO, "Bean")
{
}


void Control::setHeater(float value)
{
  if (value > 0. && getFan() <= 10)
  {
    setFan(30.f);
  }
  this->_heater.setValue(min(max(0.f, value),MAX_HEATER_POWER + 0.f));
}

void Control::setFan(float value)
{
  _fan.setValue(value);
}

float Control::getFan() const
{
  return _fan.getValue();
}


float Control::getHeater() const
{
  return this->_heater.getValue();
}

float Control::getExhaustTemp() const
{
  return this->_etSensor.getValue();
}

float Control::getBeanTemp() const
{
  return this->_btSensor.getValue();
}

float Control::getAmbientTemp() const
{
  return this->_btSensor.getAmbient();
}


void Control::loop()
{
  unsigned long now = millis();
  unsigned long dt = (now - lastUpdate);
  if (dt < noUpdateBeforeMs)
  {
    return;
  }
  this->_btSensor.takeReading();
  this->_etSensor.takeReading();
}
