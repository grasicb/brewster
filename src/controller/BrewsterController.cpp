#include "BrewsterController.h"

BrewsterController* BrewsterController::instance = NULL;



BrewsterController* BrewsterController::get() {
  if (instance == NULL) {
    instance = new BrewsterController();
  }
  return instance;
}

BrewsterController::BrewsterController() {
    Logger logger("controller");

    //Initialize Outputs
    outputs[ControllerOutput::AC1] = new Output(OutputNames[ControllerOutput::AC1], BrewsterGlobals::get()->pinAC1, false);
    outputs[ControllerOutput::AC2] = new Output(OutputNames[ControllerOutput::AC2], BrewsterGlobals::get()->pinAC2, false);
    outputs[ControllerOutput::DC1] = new Output(OutputNames[ControllerOutput::DC1], BrewsterGlobals::get()->pinDC1, true);
    outputs[ControllerOutput::DC2] = new Output(OutputNames[ControllerOutput::DC2], BrewsterGlobals::get()->pinDC2, true);

    //Initialize other Brewster components
    ds18Interface = new DS18(BrewsterGlobals::get()->pinOneWire);
    sensorManger = new SensorManager();
    processManager = new ProcessManager();
}

void BrewsterController::controllerLoop() {
    controllerLoopOther();
    controllerLoopOutput();
}

void BrewsterController::initRecipe() {
  logger.info("Creating a recipe");

  recipe.setName("Spring ale");

  recipe.addMashingStep("Step 1", 20, TimeUOM::minute, 40);
  recipe.addMashingStep("Step 2", 30, TimeUOM::minute, 64);
  recipe.addMashingStep("Step 3", 25, TimeUOM::minute, 66);
  recipe.addMashingStep("Step 4", 5, TimeUOM::minute, 74);

  recipe.setBoilingTime(60);
  recipe.addBoilAddition("Styrian Golding", 20, QtyUOM::g, 0, TimeUOM::minute);
  recipe.addBoilAddition("Amarillo", 15, QtyUOM::g, 30, TimeUOM::minute);
  recipe.addBoilAddition("Cascade", 20, QtyUOM::g, 45, TimeUOM::minute);
  recipe.addBoilAddition("Amarillo", 15, QtyUOM::g, 45, TimeUOM::minute);
  recipe.addBoilAddition("Irish moss", 11, QtyUOM::g, 45, TimeUOM::minute);

  recipe.printRecipe();
  recipe.storeToEEPROM();
}

Recipe BrewsterController::getRecipe() {
    return recipe;
}

void BrewsterController::controllerLoopOther() {
  sensorManger->readTemperatureSensors();
  processManager->processActiveProcesses();
}

void BrewsterController::controllerLoopOutput() {
  if (lastStateChange + 5 < millis()) {
    for (int i=0; i<OUTPUT_NUMBER; i++) {
      outputs[i]->process();
    }
    lastStateChange = millis();
  }
}

Output* BrewsterController::getOutput(ControllerOutput outputID) {
  return outputs[outputID];
}

DS18* BrewsterController::getDS18Interface() {
  return ds18Interface;
}

boolean BrewsterController::isOutputActive(ControllerOutput outputID) {
  return outputs[outputID]->isActive();
}

SensorManager* BrewsterController::getSensorManager() {
  return sensorManger;
}

ProcessManager* BrewsterController::getProcessManager() {
  return processManager;
}
