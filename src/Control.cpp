#include "Control.h"
#include "config.h"

Control::Control(float kp, float ki, float kd)
  : _autotune(0, 90, TuningMethod::ZieglerNichols),
    _temperatureTarget(TemperatureTarget::ET),
    lastUpdate(0),
    tuningEnabled(false),
    hasResults(false),
    _fan(FAN_PIN, 20000),
    _heater(HEATER_PIN, 50) {
  setup(kp, ki, kd);
}

void Control::setup(float kp, float ki, float kd) {
  _autotune.setManualGains(kp, ki, kd);
  _autotune.enableAntiWindup(true, 0.8);
  _autotune.setOscillationMode(OscillationMode::Normal);
  _autotune.setSetpoint(0.);
  _autotune.setOperationalMode(OperationalMode::Manual);
  _autotune.setManualOutput(0.);
}

void Control::setPidValues(float kp, float ki, float kd) {
  _autotune.setManualGains(kp, ki, kd);
}

void Control::setSetpoint(float setpoint) {
  if (_autotune.getSetpoint() == 0 && setpoint > 0.) {
    _autotune.resetError();
  }
  _autotune.setSetpoint(max(min(setpoint, 250.f), 0.f));
}

void Control::setHeater(float value) {
  _autotune.setManualOutput(value);
}

void Control::startAutotune() {
  setFan(55);
  _autotune.setSetpoint(140);
  _autotune.setOperationalMode(OperationalMode::Tune);
  tuningEnabled = true;
}

void Control::resetAutotune() {
  hasResults = false;
}

bool Control::hasAutotuneResults() const {
  return hasResults;
}

float Control::getKp() const {
  return _autotune.getKp();
}

float Control::getKi() const {
  return _autotune.getKi();
}

float Control::getKd() const {
  return _autotune.getKd();
}

void Control::setFan(float value) {
  _fan.setValue(value);
}

float Control::getFan() const {
  return _fan.getValue();
}

void Control::setTemperatureTarget(TemperatureTarget target) {
  _temperatureTarget = target;
}

float Control::getHeater() const {
  return _autotune.getOutput();
}

float Control::getSetpoint() const {
  return _autotune.getSetpoint();
}

const char *Control::getTemperatureTarget() const {
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

const char *Control::getMode() const {
  const char *result;
  if (_autotune.getOperationalMode() == OperationalMode::Auto) {
    result = "PID";
  } else if (_autotune.getOperationalMode() == OperationalMode::Tune) {
    result = "Tuning";
  } else {
    result = "Manual";
  }
  return result;
}

void Control::setMode(OperationalMode mode) {
  _autotune.setOperationalMode(mode);
}

float Control::getTemperature(const float etbt[3]) const {
  float temp = 0.;
  if (_temperatureTarget == TemperatureTarget::BT) {
    temp = etbt[1];
  } else if (_temperatureTarget == TemperatureTarget::ET) {
    temp = etbt[0];
  } else if (_temperatureTarget == TemperatureTarget::MAX) {
    temp = max(etbt[0], etbt[1]);
  }
  return temp;
}

void Control::temperatureLoop(float etbt[3]) {
  if (tuningEnabled && _autotune.getOperationalMode() != OperationalMode::Tune) {
    // tuning completed, set status accordingly
    tuningEnabled = false;
    hasResults = true;
    setFan(30.f);
    setSetpoint(0);
  }

  unsigned long now = millis();
  unsigned long dt = (now - lastUpdate);
  if (dt < noUpdateBeforeMs) {
    return;
  }

  float temp = getTemperature(etbt);
  _autotune.update(temp);

  float heaterValue = _autotune.getOutput();
  if (heaterValue > 0. && getFan() <= 10) {
    setFan(30.f);
  }

  _heater.setValue(heaterValue);
}
