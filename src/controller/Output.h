#pragma once

#include "application.h"
#include "../util/BrewsterGlobals.h"
#include "../lib/pid/PID.h"
#include <map>

struct OutputChangeEvent {
  boolean isActive;
  boolean isPID;
  double targetValue;
};

//Type definitions
using f_outputCB_t = void(*)(void* callingObject, void* outputIdentifier, OutputChangeEvent event);

class Output {
public:
    Output(String name, uint8_t pin, boolean enablePWM);
    ~Output() {};
    void setOutput(uint8_t percentage);
    uint8_t getOutput();
    void setTargetValue(double target, float* input);
    void changeTargetValue(double target);
    double getTargetValue();
    void process();
    boolean isActive();
    boolean isPID();

    void addListener(f_outputCB_t function, void* callingObject, void* outputIdentifier);
    void removeListener(f_outputCB_t function);
    void removeAllListeners();

protected:
  struct OutputListener {
    f_outputCB_t function;
    void* callingObject;
    void* outputIdentifier;
  };

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

  std::map<f_outputCB_t, OutputListener> listeners;
  void triggerChangeEvent();
};
