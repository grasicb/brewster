#include "MashingProcess.h"
#include "../controller/BrewsterController.h"

MashingProcess::MashingProcess(BrewProcess type, String name): Process(type, name) {
  lastTick = Time.now();
  logger = new Logger("MashingProcess");
  recipeMandatory = true;
  logger->warn("Initialized mashing process for proccess named: %s.", (const char*) name);
}

MashingProcess::MashingProcess(BrewProcess type, String name, Recipe* recipe): Process(type, name, recipe) {
  MashingProcess(type, name);
}

void MashingProcess::process() {
  if (Time.now() - lastTick < 60) {
    lastTick = Time.now();
    logger->info("Mashing process is active.");
  }

  if (currentStep != recipe->getCurrentMashingStep(getStartTime()))
    updateOutput();
}

void MashingProcess::processStarted() {
  logger->info("Process %s started.", (const char*) name);

  if (recipe == NULL) {
    logger->error("Recipe is not set. Cannot start the mashing process.");
    stop();
  }else{
    updateOutput();
    BrewsterController::get()->getOutput(mashPump)->setOutput(mashPumpFlowRate);
  }
}

void MashingProcess::processStopped() {
  logger->info("Process %s stopped.", (const char*) name);

  BrewsterController::get()->getOutput(mashHeater)->setOutput(0);
  BrewsterController::get()->getOutput(mashPump)->setOutput(0);
}

void MashingProcess::processPaused() {
  logger->info("Process %s paused.", (const char*) name);

  BrewsterController::get()->getOutput(mashHeater)->setOutput(0);
  BrewsterController::get()->getOutput(mashPump)->setOutput(0);
}

void MashingProcess::processResumed() {
  logger->info("Process %s resumed.", (const char*) name);

  updateOutput();
  BrewsterController::get()->getOutput(mashPump)->setOutput(mashPumpFlowRate);
}

void MashingProcess::updateOutput() {
  currentStep = recipe->getCurrentMashingStep(getStartTime());
  nextStep = recipe->getNextMashingStep(getStartTime());

  float *temp = BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::HLT).getValueReference();

  BrewsterController::get()->getOutput(mashHeater)->setTargetValue(currentStep->temperature, temp);
}
