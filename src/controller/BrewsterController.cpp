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

    ds18Interface = new DS18(BrewsterGlobals::get()->pinOneWire);

    i = 0;
    sensorManger = new SensorManager();

    //Initialize Outputs
    outputs[ControllerOutput::AC1] = new Output(OutputNames[ControllerOutput::AC1], BrewsterGlobals::get()->pinAC1, false);
    outputs[ControllerOutput::AC2] = new Output(OutputNames[ControllerOutput::AC2], BrewsterGlobals::get()->pinAC2, false);
    outputs[ControllerOutput::DC1] = new Output(OutputNames[ControllerOutput::DC1], BrewsterGlobals::get()->pinDC1, true);
    outputs[ControllerOutput::DC2] = new Output(OutputNames[ControllerOutput::DC2], BrewsterGlobals::get()->pinDC2, true);

    //reading configuration from EEPROM
    int value_int;
    EEPROM.get(EEPROM_ACTIVE_PROCESS, value_int);
    if (value_int == 0xFFFF) {
      processActive = BrewProcesses::NONE;
    } else {
      processActive = (BrewProcesses)value_int;
    }

    EEPROM.get(EEPROM_PROCESS_START, value_int);
    if (value_int == 0xFFFF) {
      processStarted = 0;
    } else {
      processStarted = value_int;
    }
}

void BrewsterController::controllerLoop() {
    controllerLoopTemperature();
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

void BrewsterController::controllerLoopTemperature() {
  sensorManger->readTemperatureSensors();
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

void BrewsterController::startProcess(BrewProcesses process) {
  processActive = process;
  processStarted = Time.now();
  EEPROM.put(EEPROM_ACTIVE_PROCESS, process);
  EEPROM.put(EEPROM_PROCESS_START, processStarted);
}

void BrewsterController::stopProcess() {
  processActive = BrewProcesses::NONE;
  EEPROM.put(EEPROM_ACTIVE_PROCESS, BrewProcesses::NONE);
}

BrewProcesses BrewsterController::getActiveProcess() {
  return processActive;
}

int BrewsterController::getProcessStartTime() {
  return processStarted;
}

SensorManager* BrewsterController::getSensorManager() {
  return sensorManger;
}
