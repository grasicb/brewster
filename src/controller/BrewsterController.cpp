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

    // register the cloud function
    Particle.function("setPIDParams", &BrewsterUtils::setPIDParameters);
    Particle.function("enterDfuMode", &BrewsterUtils::enterDfuMode);
}

void BrewsterController::controllerLoop() {
    controllerLoopOther();
    controllerLoopOutput();
}

void BrewsterController::initRecipe() {
  logger.info("Creating a recipe");

  recipe = new Recipe();
  recipe->setName("Summer Wolf");

  recipe->addMashingStep("Step 1", 60, TimeUOM::minute, 66);
  recipe->addMashingStep("Step 2", 10, TimeUOM::minute, 74);
  //recipe->addMashingStep("Step 3", 3, TimeUOM::minute, 66);
  //recipe->addMashingStep("Step 4", 5, TimeUOM::minute, 74);

  recipe->setBoilingTime(60);
  recipe->setTargetCoollingTemperature(20);
  recipe->addBoilAddition("Columbus", 10, QtyUOM::g, 0, TimeUOM::minute);
  recipe->addBoilAddition("Styrian Fox", 15, QtyUOM::g, 40, TimeUOM::minute);
  recipe->addBoilAddition("Styrian Fox", 20, QtyUOM::g, 50, TimeUOM::minute);
  recipe->addBoilAddition("Irish moss", 7, QtyUOM::g, 50, TimeUOM::minute);


  /*
  recipe->addBoilAddition("Amarillo", 15, QtyUOM::g, 2, TimeUOM::minute);
  recipe->addBoilAddition("Cascade", 20, QtyUOM::g, 1, TimeUOM::minute);
  recipe->addBoilAddition("Amarillo", 15, QtyUOM::g, 4, TimeUOM::minute);
  recipe->addBoilAddition("Irish moss", 11, QtyUOM::g, 8, TimeUOM::minute);
*/
  recipe->printRecipe();
  recipe->storeToEEPROM();
}

Recipe* BrewsterController::getRecipe() {
    return recipe;
}

void BrewsterController::initProcesses() {
  processManager->setRecipe(getRecipe());
  processManager->restoreAllProcesses();
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
