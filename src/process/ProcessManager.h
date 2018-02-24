#pragma once

#include "application.h"
#include "../util/BrewsterGlobals.h"
#include "Process.h"
#include "../model/Recipe.h"
#include <map>

const static String BrewProcessNames[] = {"None", "Mashing", "Boiling", "Chilling", "Fermenting"};

typedef std::map<BrewProcess, Process*> ProcessMap;

class ProcessManager {
public:
  ProcessManager();
  void processActiveProcesses();
  Process* getProcess(BrewProcess process);
  ProcessMap& getActiveProcesses();
  void restoreAllProcesses();
  void setRecipe(Recipe *recipe);

private:
  void initProcesses();
  static void processChangeEventHandler(void* callingObject, ProcessStateChangeEvent event);

  Logger* logger;
  ProcessMap processes;
  ProcessMap activeProcesses;
};
