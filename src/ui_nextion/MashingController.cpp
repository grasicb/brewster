#include "MashingController.h"
#include "../controller/BrewsterController.h"
#include "../controller/SensorManager.h"
#include "../controller/TemperatureSensor.h"

MashingController::MashingController() {
  logger = new Logger("MashingController");
  //temperature.resize(sensorNo, 0);
  sensors = &(BrewsterController::get()->getSensorManager()->getAllTemperatureSensors());

  temperature[SensorLocation::HLT] = 0;
  temperature[SensorLocation::MT] = 0;
  temperature[SensorLocation::COOLER_OUT] = 0;
}


void MashingController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);
  logger->info("Initializing mashing screen");

  //Setting new recipe if the process is not running, otherwise retrieve recipe from running process
  mashProcess = BrewsterController::get()->getProcessManager()->getProcess(BrewProcess::MASHING);
  if (mashProcess->isActive())
    recipe = mashProcess->getRecipe();
  else {
    recipe = BrewsterController::get()->getRecipe();
    mashProcess->setRecipe(recipe);
  }

  //Adding listener for change in process
  mashProcess->addListener(processInfoChangeHandler, this);

  //Update recipe information (current & next steps)
  getRecipeInformation();

  //Init relevant temp sensors
  temperature[SensorLocation::HLT] = (*sensors)[SensorLocation::HLT].getValue();
  temperature[SensorLocation::MT] = (*sensors)[SensorLocation::MT].getValue();
  temperature[SensorLocation::COOLER_OUT] = (*sensors)[SensorLocation::COOLER_OUT].getValue();

  //Update screen
  updateLcdTemp();
  updateLcdProcessInfo();
}

void MashingController::deactivateScreen() {
  mashProcess->removeListener(processInfoChangeHandler);
}


void MashingController::process() {
  boolean tempUpdate = false;

  //Check if temperature update is needed
  for ( auto &p : (temperature) ) {
    if(p.second != (*sensors)[p.first].getValue()) {
      //p.second = (*sensors)[p.first].getValue();
      temperature[p.first] = (*sensors)[p.first].getValue();
      tempUpdate = true;
    }
  }



  if (tempUpdate)
    updateLcdTemp();
}

void MashingController::updateLcdTemp() {
  nTempMT.setValue(temperature[SensorLocation::MT]);
  nTempHLT.setValue(temperature[SensorLocation::HLT]);
  nTempCOut.setValue(temperature[SensorLocation::COOLER_OUT]);
}

void MashingController::updateLcdProcessInfo() {

  /*
  unsigned long start = millis();

  String output = "";
  for (int i = 0; i<sensorNo; i++) {
    output.concat(String::format("%2i - %s: %.2f%c%c", i+1, (const char*)sensorNames[i], temperature[i], 13, 10));
  }
  outputText1.setText(output);

  unsigned long duration = millis() - start;
  if(logger->isTraceEnabled())
    logger->trace("Refreshed output of sensors in %lu ms", duration);
    */
}

void MashingController::getRecipeInformation() {
  processStartTime = mashProcess->getStartTime();
  currentStep = recipe->getCurrentMashingStep(processStartTime);
  nextStep = recipe->getNextMashingStep(processStartTime);
  currentStepStartTime = processStartTime + recipe->getMashingStepStartTime(currentStep);
  nextStepStartTime = currentStepStartTime + BrewsterUtils::getSeconds(currentStep->time, currentStep->timeUOM);
}

void MashingController::triggerPumpButtonAH(void *ptr) {

}

void MashingController::triggerStartStopAH(void *ptr) {

}

void MashingController::processInfoChangeHandler(void* callingObject, void* process) {
  MashingController* mc = (MashingController*) callingObject;
  mc->updateLcdProcessInfo();
}
