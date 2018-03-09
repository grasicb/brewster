#include "ProcessManager.h"
#include "GenericProcess.h"
#include "MashingProcess.h"
#include "BoilingProcess.h"
#include "CoolingProcess.h"
#include "../controller/BrewsterController.h"

ProcessManager::ProcessManager() {
  logger = new Logger("ProcessManager");
  initProcesses();
}

void ProcessManager::initProcesses() {
  logger->info("Initializing brewing processes");

  processes[BrewProcess::MASHING] = new MashingProcess(BrewProcess::MASHING, "Mashing");
  processes[BrewProcess::BOILING] = new BoilingProcess(BrewProcess::BOILING, "Boiling");
  processes[BrewProcess::COOLING] = new CoolingProcess(BrewProcess::COOLING, "Chilling");
  processes[BrewProcess::FERMENTING] = new GenericProcess(BrewProcess::FERMENTING, "Fermenting");

  for (ProcessMap::iterator it=processes.begin(); it!=processes.end(); ++it)
    it->second->addListener(processChangeEventHandler, this);
}

void ProcessManager::processActiveProcesses() {
  for (ProcessMap::iterator it=activeProcesses.begin(); it!=activeProcesses.end(); ++it)
    it->second->process();
}

void ProcessManager::processChangeEventHandler(void* callingObject, ProcessStateChangeEvent event) {
  ProcessManager* controller = (ProcessManager*) callingObject;
  Process* process = (Process*) event.process;

  controller->logger->info("Process %s has changed state [old state=%s, new state=%s].",
                                (const char*)process->getName(),
                                (const char*) ProcessStateNames[event.oldState],
                                (const char*) ProcessStateNames[event.newState]);

  switch (event.newState) {
    case ProcessState::STOPPED:
      controller->activeProcesses.erase(process->getType());
      break;
    case ProcessState::PAUSED:
      controller->activeProcesses.erase(process->getType());
      break;
    case ProcessState::STARTED:
      controller->activeProcesses[process->getType()]=process;
      break;
    default:
      controller->logger->error("processChangeEventHandler: State change handler for state %s is not implemented.", (const char*) ProcessStateNames[event.newState]);
  }
}

Process* ProcessManager::getProcess(BrewProcess process) {
  return processes[process];
}

ProcessMap& ProcessManager::getActiveProcesses() {
  return activeProcesses;
}

void ProcessManager::restoreAllProcesses() {
  for (ProcessMap::iterator it=processes.begin(); it!=processes.end(); ++it)
    it->second->restoreProcess();
}

void ProcessManager::setRecipe(Recipe *recipe) {
  for (ProcessMap::iterator it=processes.begin(); it!=processes.end(); ++it) {
    if(!it->second->isActive() || it->second->getRecipe() == NULL)
      it->second->setRecipe(recipe);
  }
}
