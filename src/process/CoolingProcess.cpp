#include "CoolingProcess.h"
#include "../controller/BrewsterController.h"

CoolingProcess::CoolingProcess(BrewProcess type, String name): Process(type, name) {
  lastTick = Time.now();
  logger = new Logger("CoolingProcess");
  recipeMandatory = true;

  logger->warn("Initialized chilling process for proccess named: %s.", (const char*) name);
}

CoolingProcess::CoolingProcess(BrewProcess type, String name, Recipe* recipe): Process(type, name, recipe) {
  CoolingProcess(type, name);
}

void CoolingProcess::process() {
  if (Time.now() - lastTick > 30) {
    lastTick = Time.now();
    logger->trace("Chilling process is active.");
  }
}


void CoolingProcess::processStarted() {
  logger->info("Starting process %s.", (const char*) name);

  float *temperatureRef = BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::COOLER_OUT).getValueReference();
  BrewsterController::get()->getOutput(coolingPump)->setTargetValue(recipe->getTargetCoollingTemperature(), temperatureRef, REVERSE, &(BrewsterGlobals::get()->getPIDSettings()[BrewProcess::COOLING]));

  logger->info("Process %s started.", (const char*) name);
}

void CoolingProcess::processStopped() {
  logger->info("Stopping process %s.", (const char*) name);

  BrewsterController::get()->getOutput(coolingPump)->setOutput(0);

  logger->info("Process %s stopped.", (const char*) name);
}

void CoolingProcess::processPaused() {
  logger->info("Pausing process %s.", (const char*) name);

  BrewsterController::get()->getOutput(coolingPump)->setOutput(0);

  logger->info("Process %s paused.", (const char*) name);
}

void CoolingProcess::processResumed() {
  logger->info("Resuming process %s.", (const char*) name);

  float *temperatureRef = BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::COOLER_OUT).getValueReference();
  BrewsterController::get()->getOutput(coolingPump)->setTargetValue(recipe->getTargetCoollingTemperature(), temperatureRef, REVERSE, &(BrewsterGlobals::get()->getPIDSettings()[BrewProcess::COOLING]));

  logger->info("Process %s resumed.", (const char*) name);
}

void CoolingProcess::processRestored() {
  logger->info("Restoring process %s after system startup.", (const char*) name);

  if(getState() == ProcessState::STARTED) {
    float *temperatureRef = BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::COOLER_OUT).getValueReference();
    BrewsterController::get()->getOutput(coolingPump)->setTargetValue(recipe->getTargetCoollingTemperature(), temperatureRef, REVERSE, &(BrewsterGlobals::get()->getPIDSettings()[BrewProcess::COOLING]));
  }else {
    BrewsterController::get()->getOutput(coolingPump)->setOutput(0);
  }

  logger->info("Process %s restored.", (const char*) name);
}
