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
    i = 0;
    temperatureSensors = new TemperatureSensors();

    //Initialize Outputs
    outputs[Outputs::AC1] = new Output(OutputNames[Outputs::AC1], BrewsterGlobals::get()->pinAC1, false);
    outputs[Outputs::AC2] = new Output(OutputNames[Outputs::AC2], BrewsterGlobals::get()->pinAC2, false);
    outputs[Outputs::DC1] = new Output(OutputNames[Outputs::DC1], BrewsterGlobals::get()->pinDC1, true);
    outputs[Outputs::DC2] = new Output(OutputNames[Outputs::DC2], BrewsterGlobals::get()->pinDC2, true);

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

void BrewsterController::controllerLoopTemperature() {
  temperatureSensors->readSensors();
}

void BrewsterController::controllerLoopOutput() {
  if (lastStateChange + 5 < millis()) {
    for (int i=0; i<4; i++) {
      outputs[i]->process();
    }
    lastStateChange = millis();
  }
}

Output* BrewsterController::getOutput(Outputs outputID) {
  return outputs[outputID];
}

boolean BrewsterController::isOutputActive(Outputs outputID) {
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
