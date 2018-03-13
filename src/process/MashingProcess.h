#pragma once

#include "application.h"
#include "Process.h"

class MashingProcess : public Process {
public:
  MashingProcess(BrewProcess type, String name);
  MashingProcess(BrewProcess type, String name, Recipe* recipe);
  void process();

protected:
  void processStarted();
  void processStopped();
  void processPaused();
  void processResumed();
  void processRestored();
private:
  void updateOutput();
  void updateStepStatus();

  unsigned long nextStepStartTime;

  unsigned long lastTick;
  unsigned long lastTempLogged;
  StepDO* currentStep;
  StepDO* nextStep;
};
