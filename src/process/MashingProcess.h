#pragma once

#include "application.h"
#include "Process.h"
#include "../model/Recipe.h"

class MashingProcess : public Process {
public:
  MashingProcess(BrewProcess type, String name);
  MashingProcess(BrewProcess type, String name, Recipe* recipe);
  void process();

  void setRecipe(Recipe *recipe);
  Recipe* getRecipe();
protected:
  void processStarted();
  void processStopped();
  void processPaused();
  void processResumed();
private:
  void updateOutput();

  unsigned long lastTick;
  Recipe* recipe;
  StepDO* currentStep;
  StepDO* nextStep;
};
