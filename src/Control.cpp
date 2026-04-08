#include "Control.h"
#include "AutoTunePID.h"
#include "heater.h"
#include "fan.h"


AutoTunePID _autotune = AutoTunePID(0, 90, TuningMethod::ZieglerNichols);
TemperatureTarget _temperatureTarget = TemperatureTarget::ET;
const uint8_t noUpdateBeforeMs = 20; // 50 Hz
unsigned long lastUpdate = 0;
bool tuningEnabled = false;
bool hasResults = false;


void setupControl(float kp, float ki, float kd) {
  initHeater();
  initFan();
  _autotune.setManualGains(kp, ki, kd);
  _autotune.enableAntiWindup(true, 0.8);
  _autotune.setOscillationMode(OscillationMode::Normal);
  _autotune.setSetpoint(0.);
  _autotune.setOperationalMode(OperationalMode::Manual);
  _autotune.setManualOutput(0.);
}

void setPidValues(float kp, float ki, float kd) {
  _autotune.setManualGains(kp, ki, kd);
}

void setSetpoint(float setpoint) {
  if (setpoint > 0.) {
    _autotune.setSetpoint(min(setpoint,250.f));
    _autotune.setOperationalMode(OperationalMode::Auto);
  } else {
    _autotune.setSetpoint(0.);
    _autotune.setOperationalMode(OperationalMode::Manual);
    _autotune.setManualOutput(0.);
  }
}

void setHeater(float value) {
  _autotune.setOperationalMode(OperationalMode::Manual);
  _autotune.setSetpoint(0.);
  _autotune.setManualOutput(value);
}

void startAutotune() {
  _autotune.setOperationalMode(OperationalMode::Tune);
  tuningEnabled = true;
}

void resetAutotune() {
  hasResults = false;
}

bool hasAutotuneResults() {
  return hasResults;
}
float getKp() {
  return _autotune.getKp();
}

float getKi() {
  return _autotune.getKi();
}

float getKd() {
  return _autotune.getKd();
}

void setFan(long value) {
  setFanSpeed(value);
}

long getFan() {
  return getFanSpeed();
}

void setTemperatureTarget(TemperatureTarget target) {
  _temperatureTarget = target;
}

float getHeater() {
  return _autotune.getOutput();
}

float getSetpoint() {
  return _autotune.getSetpoint();
}

const char* getTemperatureTarget() {
  const char *result;
  if (_temperatureTarget == TemperatureTarget::BT) {
    result = "BT";
  } else if (_temperatureTarget == TemperatureTarget::ET) {
    result = "ET";
  } else {
    result = "MAX";
  }
  return result;
}

const char *getMode() {
  const char *result;
  if (_autotune.getOperationalMode() == OperationalMode::Auto) {
    result = "PID";
  } else if  (_autotune.getOperationalMode() == OperationalMode::Tune) {
    result = "Tuning";
  } else {
    result = "Manual";
  }
  return result;
}

void temperatureLoop(float etbt[3]) {
  if (tuningEnabled && _autotune.getOperationalMode() != OperationalMode::Tune) {
    // tuning completed, set status accordingly
    tuningEnabled = false;
    hasResults = true;
  }
  unsigned long now = millis();
  unsigned long dt = (now - lastUpdate);
  if (dt < noUpdateBeforeMs) {
    return;
  }
  float temp = 0.;
  if (_temperatureTarget == TemperatureTarget::BT) {
    temp = etbt[1];
  }
  if (_temperatureTarget == TemperatureTarget::ET) {
    temp = etbt[0];
  }
  if (_temperatureTarget == TemperatureTarget::MAX) {
    temp = max(etbt[0], etbt[1]);
  }
  _autotune.update(temp);
  float heaterValue = _autotune.getOutput();
  if (heaterValue > 0. && getFan() <= 10) {
    setFan(30);
  }
  setHeaterPower(heaterValue);
}
