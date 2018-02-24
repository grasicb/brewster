#pragma once
#include "application.h"
#include "../util/BrewsterGlobals.h"
#include "../model/Recipe.h"

struct ProcessStateChangeEvent {
  ProcessState oldState;
  ProcessState newState;
  void* process;
};

//Type definitions
using f_processStateChange_t = void(*)(void* callingObject, ProcessStateChangeEvent event);
using f_processInfoChange_t = void(*)(void* callingObject, void* process);

class Process {
public:
  Process(BrewProcess type, String name);
  Process(BrewProcess type, String name, Recipe* recipe);
  virtual void process() = 0;

  void start();
  void pause();
  void resume();
  void stop();
  BrewProcess getType();
  String getName();
  ProcessState getState();
  boolean isActive();
  void setRecipe(Recipe *recipe);
  Recipe* getRecipe();
  unsigned long getStartTime();
  unsigned long getRunTime();
  unsigned long getRunTime(TimeUOM timeUOM);

  void addListener(f_processStateChange_t function, void* callingObject);
  void removeListener(f_processStateChange_t function);
  void addListener(f_processInfoChange_t function, void* callingObject);
  void removeListener(f_processInfoChange_t function);
  void removeAllStateChangeListeners();
  void removeAllInfoChangeListeners();
  void removeAllListeners();
  void restoreProcess();

protected:
  void storeToEEPROM();
  void loadFromEEPROM();
  virtual void processStarted() = 0;
  virtual void processStopped() = 0;
  virtual void processPaused() = 0;
  virtual void processResumed() = 0;
  void triggerInfoChangeEvent();

  struct StateChageListener {
    f_processStateChange_t function;
    void* callingObject;
  };
  struct InfoChageListener {
    f_processInfoChange_t function;
    void* callingObject;
  };

  Logger *logger;
  BrewProcess type;
  String name;
  Recipe* recipe = NULL;
  boolean recipeMandatory;

private:
  typedef std::map<f_processStateChange_t, StateChageListener> StateChangeListenerMap;
  typedef std::map<f_processInfoChange_t, InfoChageListener> InfoChangeListenerMap;

  unsigned long startTime;
  ProcessState state;

  StateChangeListenerMap listenersStateChange;
  InfoChangeListenerMap listenersInfoChange;
  void triggerStateChangeEvent(ProcessStateChangeEvent& event);
};
