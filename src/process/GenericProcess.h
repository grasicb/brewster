#pragma once

#include "application.h"
#include "Process.h"

class GenericProcess : public Process {
public:
  GenericProcess(BrewProcess type, String name);
  void process();

  void processStarted();
  void processStopped();
  void processPaused();
  void processResumed();
  void processRestored();
private:
  //Logger *logger;
  unsigned long lastTick;
};
