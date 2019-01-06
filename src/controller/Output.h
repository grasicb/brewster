#pragma once

#include "application.h"
#include "../util/BrewsterGlobals.h"
#include "../lib/pid/PID.h"
#include "../lib/pid_autotune/PID_ATune.h"
#include <map>

//Type definitions


class Output {
public:
    struct OutputChangeEvent {
      boolean isActive;
      boolean isPID;
      boolean isAutoTune;
      double targetValue;
      Output* output;
    };
    using f_outputCB_t = void(*)(void* callingObject, int outputIdentifier, OutputChangeEvent event);

    Output(String name, uint8_t pin, boolean enablePWM);
    ~Output() {};
    void setOutput(uint8_t percentage);
    uint8_t getOutput();
    void setTargetValue(double target, float* input, PidSettings* settings = NULL);
    void setTargetValue(double target, float* input, int direction, PidSettings* settings = NULL);
    void changeTargetValue(double target);
    double getTargetValue();
    boolean isAutoTune();
    void toggleAutoTune();
    //void SetPidDirection(int direction);
    void process();
    boolean isActive();
    boolean isPID();
    String getName();
    PidSettings* getPIDSettings();

    void addListener(f_outputCB_t function, void* callingObject, int outputIdentifier);
    void removeListener(f_outputCB_t function);
    void removeAllListeners();

protected:
  struct OutputListener {
    f_outputCB_t function;
    void* callingObject;
    int outputIdentifier;
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
  PID_ATune* pidATune = NULL;
  String _name;
  uint8_t _pin;

  PidSettings *pidSettings = NULL;
  boolean autoTune = false;
  double aTuneStep=80;
  double aTuneNoise=1;
  double aTuneStartValue=127;
  unsigned int aTuneLookBack=20;
  byte aTuneModeRemember=2;
  unsigned long autoTuneUpdate=0;
  unsigned long autoTuneStart=0;

  std::map<f_outputCB_t, OutputListener> listeners;
  void triggerChangeEvent();
};
