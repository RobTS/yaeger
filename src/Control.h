#include <Arduino.h>
#include "AutoTunePID.h"

enum class TemperatureTarget {
  ET,
  BT,
  MAX
};


void setupControl(float kp, float ki, float kd);

void temperatureLoop(float etbt[3]);

void setSetpoint(float setpoint);

void setHeater(float heaterVal);

void startAutotune();

void setTemperatureTarget(TemperatureTarget target);

void setPidValues(float kp, float ki, float kd);

float getHeater();

float getSetpoint();

const char *getTemperatureTarget();

const char *getMode();

void setMode(OperationalMode mode);

void setFan(float value);

float getFan();

float getKp();
float getKi();
float getKd();

void resetAutotune();
bool hasAutotuneResults();
