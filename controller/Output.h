#pragma once

#include "application.h"
#include "../util/BrewsterGlobals.h"
#include "../lib/pid/PID.h"


class Output {
public:
    Output(String name, uint8_t pin, boolean enablePWM);
    ~Output() {};
    void setOutput(uint8_t percentage);
    void setTargetValue(double target, double* input);
    void process();
    boolean isActive();


private:
  Logger *_logger;
  double _lastOutput;
  double _output;
  boolean _isPWM;
  boolean _pidOn;
  int _frequency;
  const static unsigned long _timeWindow = 500;
  unsigned long _windowStartTime = 0;
  double* _input;
  double _target;
  PID* _pid = NULL;
  String _name;

  uint8_t _pin;
};
