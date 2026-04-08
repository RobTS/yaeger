#include <Arduino.h>

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

void setFan(long value);

long getFan();

float getKp();
float getKi();
float getKd();

void resetAutoTune();
bool hasAutotuneResults();
