#include "BoilProcess.h"
#include "../controller/BrewsterController.h"

BoilProcess::BoilProcess(BrewProcess type, String name): Process(type, name) {
  lastTick = Time.now();
  logger = new Logger("BoilProcess");
  recipeMandatory = true;
  boilingPointReached = false;
  boilStopTime = 0;
  logger->warn("Initialized boil process for proccess named: %s.", (const char*) name);
}

BoilProcess::BoilProcess(BrewProcess type, String name, Recipe* recipe): Process(type, name, recipe) {
  BoilProcess(type, name);
}

void BoilProcess::process() {
  if (Time.now() - lastTick > 60) {
    lastTick = Time.now();
    logger->trace("Boil process is active.");
  }

  if(!boilingPointReached) {
    if(BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::HLT).getValue() > boilingPoint) {
      logger->info("Boiling point reached.");
      boilingPointReached = true;
      if(getStartTime() == 0) {}
        setStartTime(Time.now());
        boilStopTime = getStartTime() + (recipe->getBoilingTime()*60);
      }

      updateOutput();
      triggerInfoChangeEvent();

  }else if(boilingPoint - BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::HLT).getValue() > 4) {
    logger->info("Temperature below lower boiling tresshold. Setting boiling heater output to high.");
    boilingPointReached = false;
  }

  if(boilStopTime > 0) {
    logger->info("Boiling time over, boiling process stopped automatically.");
    stop();
  }
}

boolean BoilProcess::isBoilingPointReached() {
  return boilingPointReached;
}

void BoilProcess::processStarted() {
  logger->info("Starting process %s.", (const char*) name);

  if (recipe == NULL) {
    logger->error("Recipe is not set. Cannot start the mashing process.");
    stop();
  }else{
    boilingPointReached = false;
    boilStopTime = 0;
    setStartTime(0);
    updateOutput();
    logger->info("Process %s started.", (const char*) name);
  }
}

void BoilProcess::processStopped() {
  logger->info("Stopping process %s.", (const char*) name);

  BrewsterController::get()->getOutput(boilHeater)->setOutput(0);

  logger->info("Process %s stopped.", (const char*) name);
}

void BoilProcess::processPaused() {
  logger->info("Pausing process %s.", (const char*) name);

  BrewsterController::get()->getOutput(boilHeater)->setOutput(0);
  boilingPointReached = false;

  logger->info("Process %s paused.", (const char*) name);
}

void BoilProcess::processResumed() {
  logger->info("Resuming process %s.", (const char*) name);

  if (recipe == NULL) {
    logger->error("Recipe is not set. Cannot resume the boil process.");
    stop();
  }else{
    updateOutput();

    logger->info("Process %s resumed.", (const char*) name);
  }
}

void BoilProcess::updateOutput() {
  if(getState() == ProcessState::STARTED) {
    if(boilingPointReached) {
      BrewsterController::get()->getOutput(boilHeater)->setOutput(boilHeatingRate);
    }else{
      BrewsterController::get()->getOutput(boilHeater)->setOutput(100);
    }
  }else{
    BrewsterController::get()->getOutput(boilHeater)->setOutput(0);
  }
}