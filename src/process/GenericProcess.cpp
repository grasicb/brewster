#include "GenericProcess.h"

GenericProcess::GenericProcess(BrewProcess type, String name): Process(type, name) {
  lastTick = Time.now();
  logger = new Logger("GeneralProcess");
  logger->warn("Initialized general process for proccess: %s. Placeholder only. This handler should be replaced with a real implementation.", (const char*) name);
}

void GenericProcess::process() {
  if (Time.now() - lastTick < 1000) {
    lastTick = Time.now();
    logger->info("Hearthbeat for process %s.", (const char*) name);
  }
}
