#pragma once

#include "application.h"
#include "Process.h"

class CoolingProcess : public Process {
public:
  CoolingProcess(BrewProcess type, String name);
  CoolingProcess(BrewProcess type, String name, Recipe* recipe);
  void process();
  boolean isBoilingPointReached();

protected:
  void processStarted();
  void processStopped();
  void processPaused();
  void processResumed();
  void processRestored();
private:
  void updateOutput();
  void updateStepStatus();

  unsigned long lastTick;
};
