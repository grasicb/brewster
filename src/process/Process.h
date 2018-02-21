#pragma once
#include "application.h"
#include "../util/BrewsterGlobals.h"

struct ProcessStateChangeEvent {
  ProcessState oldState;
  ProcessState newState;
  void* process;
};

//Type definitions
using f_processStateChange_t = void(*)(void* callingObject, ProcessStateChangeEvent event);

class Process {
public:
  Process(BrewProcess type, String name);
  virtual void process() = 0;

  void start();
  void pause();
  void resume();
  void stop();
  BrewProcess getType();
  String getName();
  ProcessState getState();
  unsigned long getStartTime();
  unsigned long getRunTime();
  unsigned long getRunTime(TimeUOM timeUOM);

  void addListener(f_processStateChange_t function, void* callingObject);
  void removeListener(f_processStateChange_t function);
  void removeAllListeners();

protected:
  void storeToEEPROM();
  void loadFromEEPROM();
  struct StateChageListener {
    f_processStateChange_t function;
    void* callingObject;
  };

  virtual void processStarted() = 0;
  virtual void processStopped() = 0;
  virtual void processPaused() = 0;
  virtual void processResumed() = 0;

  Logger *logger;
  BrewProcess type;
  String name;

private:
  unsigned long startTime;
  ProcessState state;

  std::map<f_processStateChange_t, StateChageListener> listeners;
  void triggerStateChangeEvent(ProcessStateChangeEvent& event);
};
