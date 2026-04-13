#ifndef PWM_H
#define PWM_H


class PwmOutput {
private:
  float _currentValue;
  int _pin;
  int _maxDuty;

public:
  PwmOutput(int pin,
            int frequency,
            int dutyResolution,
            int channel);

  ~PwmOutput() = default;

  void setValue(float power);

  float getValue() const;
};

#endif
