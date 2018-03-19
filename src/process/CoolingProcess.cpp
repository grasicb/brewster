#include "CoolingProcess.h"
#include "../controller/BrewsterController.h"

CoolingProcess::CoolingProcess(BrewProcess type, String name): Process(type, name) {
  lastTick = Time.now();
  logger = new Logger("CoolingProcess");
  recipeMandatory = true;
  tempFermentor = 0;
  lastTempLogged=0;
  tempFermentorSum=0;
  tempFermentorCount=0;
  Particle.variable("tempFerm", tempFermentor);

  logger->warn("Initialized chilling process for proccess named: %s.", (const char*) name);
}

CoolingProcess::CoolingProcess(BrewProcess type, String name, Recipe* recipe): Process(type, name, recipe) {
  CoolingProcess(type, name);
}

void CoolingProcess::process() {
  if (Time.now() - lastTick >= 30) {
    lastTick = Time.now();
    logger->trace("Chilling process is active.");
  }

  if (Time.now() - lastTempLogged >= 1) {
    tempFermentorSum += BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::FERMENTOR).getValue();
    tempFermentorCount++;
    lastTempLogged = Time.now();

    if(tempFermentorCount>=300) {
      Particle.publish("tempFerment", String::format("%lu;%lu;%.2f",
              Time.now()-getStartTime(),
              Time.now(),
              tempFermentorSum/tempFermentorCount),
          PRIVATE);

      tempFermentorSum = 0;
      tempFermentorCount = 0;
    }
  }


  if (tempFermentor !=  BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::FERMENTOR).getValue()) {
    tempFermentor = BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::FERMENTOR).getValue();
  }
}


void CoolingProcess::processStarted() {
  logger->info("Starting process %s.", (const char*) name);

  float *temperatureRef = BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::COOLER_OUT).getValueReference();
  BrewsterController::get()->getOutput(coolingPump)->setTargetValue(recipe->getTargetCoollingTemperature(), temperatureRef, REVERSE, &(BrewsterGlobals::get()->getPIDSettings()[BrewProcess::COOLING]));

  lastTempLogged=0;
  tempFermentorSum=0;
  tempFermentorCount=0;

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
