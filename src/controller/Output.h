#pragma once

#include "application.h"
#include "../util/BrewsterGlobals.h"
#include "../lib/pid/PID.h"


class Output {
public:
    Output(String name, uint8_t pin, boolean enablePWM);
    ~Output() {};
    void setOutput(uint8_t percentage);
    uint8_t getOutput();
    void setTargetValue(double target, float* input);
    void changeTargetValue(double target);
    void process();
    boolean isActive();
    boolean isPID();



private:
  Logger *_logger;
  double _lastOutput;
  double _output;
  boolean _isPWM;
  boolean _pidOn;
  int _frequency;
  const static unsigned long _timeWindow = 500;
  unsigned long _windowStartTime = 0;
  float* _input;
  double _target;
  PID* _pid = NULL;
  String _name;

  uint8_t _pin;
};
