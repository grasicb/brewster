#include "ProcessManager.h"
#include "GenericProcess.h"
#include "MashingProcess.h"
#include "../controller/BrewsterController.h"

ProcessManager::ProcessManager() {
  logger = new Logger("ProcessManager");
  initProcesses();
}

void ProcessManager::initProcesses() {
  logger->info("Initializing brewing processes");

  processes[BrewProcess::MASHING] = new MashingProcess(BrewProcess::MASHING, "Mashing");
  processes[BrewProcess::BOILING] = new GenericProcess(BrewProcess::BOILING, "Boilin");
  processes[BrewProcess::CHILLING] = new GenericProcess(BrewProcess::CHILLING, "Chilling");
  processes[BrewProcess::FERMENTING] = new GenericProcess(BrewProcess::FERMENTING, "Fermenting");
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
    case ProcessState::PAUSED:
      controller->activeProcesses.erase(process->getType());
    case ProcessState::STARTED:
      controller->activeProcesses[process->getType()]=process;
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
