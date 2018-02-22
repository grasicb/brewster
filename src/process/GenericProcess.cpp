#include "GenericProcess.h"

GenericProcess::GenericProcess(BrewProcess type, String name): Process(type, name) {
  lastTick = Time.now();
  logger = new Logger("GeneralProcess");
  logger->warn("Initialized general process for proccess: %s. Placeholder only. This handler should be replaced with a real implementation.", (const char*) name);
}

void GenericProcess::process() {
  if (Time.now() - lastTick < 5) {
    lastTick = Time.now();
    logger->info("Hearthbeat for process %s.", (const char*) name);
  }
}

void GenericProcess::processStarted() {
  logger->info("Process %s started.", (const char*) name);
}

void GenericProcess::processStopped() {
  logger->info("Process %s stopped.", (const char*) name);
}

void GenericProcess::processPaused() {
  logger->info("Process %s paused.", (const char*) name);
}

void GenericProcess::processResumed() {
  logger->info("Process %s resumed.", (const char*) name);
}
