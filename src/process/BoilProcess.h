#pragma once

#include "application.h"
#include "Process.h"

class BoilProcess : public Process {
public:
  BoilProcess(BrewProcess type, String name);
  BoilProcess(BrewProcess type, String name, Recipe* recipe);
  void process();
  boolean isBoilingPointReached();

protected:
  void processStarted();
  void processStopped();
  void processPaused();
  void processResumed();
private:
  void updateOutput();
  void updateStepStatus();

  boolean boilingPointReached;
  long boilStopTime;

  unsigned long lastTick;
};
