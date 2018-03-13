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
  if (Time.now() - lastTick > 60) {
    lastTick = Time.now();
    logger->trace("Mashing process is active.");
  }

  if(Time.now() > nextStepStartTime) {
    if(nextStep == NULL) {
        logger->info("No further mashing steps. Stopping mashing process");
        stop();
    }else{
      logger->info("Moving to next step[%s - %i %s @ %i °C]", (const char*)nextStep->name, nextStep->time, (const char*)TimeUOMNames[nextStep->timeUOM], nextStep->temperature);
      updateOutput();
      triggerInfoChangeEvent();
    }
  }
}

void MashingProcess::processStarted() {
  logger->info("Starting process %s.", (const char*) name);

  if (recipe == NULL) {
    logger->error("Recipe is not set. Cannot start the mashing process.");
    stop();
  }else{
    updateOutput();

    Output* pump = BrewsterController::get()->getOutput(mashPump);
    if(pump == NULL)
      logger->error("Error while starting mashing process. Cannot start the pumpt, invalid reference to output.");
    else {
      logger->trace("Setting mash pum to rate %i", mashPumpFlowRate);
      pump->setOutput(mashPumpFlowRate);
    }

    //BrewsterController::get()->getOutput(mashPump)->setOutput(mashPumpFlowRate);
    logger->info("Process %s started.", (const char*) name);
  }
}

void MashingProcess::processStopped() {
  logger->info("Stopping process %s.", (const char*) name);

  BrewsterController::get()->getOutput(mashHeater)->setOutput(0);
  BrewsterController::get()->getOutput(mashPump)->setOutput(0);

  logger->info("Process %s stopped.", (const char*) name);
}

void MashingProcess::processPaused() {
  logger->info("Pausing process %s.", (const char*) name);

  BrewsterController::get()->getOutput(mashHeater)->setOutput(0);
  BrewsterController::get()->getOutput(mashPump)->setOutput(0);

  logger->info("Process %s paused.", (const char*) name);
}

void MashingProcess::processResumed() {
  logger->info("Resuming process %s.", (const char*) name);

  if (recipe == NULL) {
    logger->error("Recipe is not set. Cannot resume the mashing process.");
    stop();
  }else{
    updateOutput();

    Output* pump = BrewsterController::get()->getOutput(mashPump);
    if(pump == NULL)
      logger->error("Error while starting mashing process. Cannot start the pumpt, invalid reference to output.");
    else {
      logger->trace("Setting mash pum to rate %i", mashPumpFlowRate);
      pump->setOutput(mashPumpFlowRate);
    }

    logger->info("Process %s resumed.", (const char*) name);
  }
}

void MashingProcess::processRestored() {
  logger->info("Restoring process %s after system startup.", (const char*) name);

  if(getState()==ProcessState::STARTED) {
    processResumed();
  }else if(getState()==ProcessState::PAUSED) {
    if (recipe == NULL) {
      logger->error("Recipe is not set. The mashing process might not work as expected.");
      stop();
    }
    processPaused();
  }

  logger->info("Process %s restored.", (const char*) name);

}

void MashingProcess::updateStepStatus() {
  currentStep = recipe->getCurrentMashingStep(getStartTime());
  nextStep = recipe->getNextMashingStep(getStartTime());

  if(currentStep != NULL && recipe != NULL)
    nextStepStartTime = getStartTime()+recipe->getMashingStepStartTime(currentStep)+BrewsterUtils::getSeconds(currentStep->time, currentStep->timeUOM);
  else {
    logger->error("Cannot set next step start time. Either recipe is not set or could not allocate current step.");
    nextStepStartTime = 0;
  }
}

void MashingProcess::updateOutput() {
  updateStepStatus();

  float *temp = BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::HLT).getValueReference();

  if(currentStep == NULL)
    logger->error("Error while updating output. Current step is not set.");
  else if(temp == NULL)
    logger->error("Error while updating output. Reference temperature for PID is NULL.");
  else {
    BrewsterController::get()->getOutput(mashHeater)->setTargetValue(currentStep->temperature, temp, &(BrewsterGlobals::get()->getPIDSettings()[BrewProcess::MASHING]));
  }
}
