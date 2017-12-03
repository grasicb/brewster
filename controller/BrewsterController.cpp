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
//    i++;
//    logger.trace("Loop iteration: %ld", i);

    temperatureSensors->readSensors();

    if (lastStateChange + 1000 < millis()) {
      lastStateChange = millis();
      outputAC = !outputAC;

      digitalWrite(BrewsterGlobals::get()->pinAC1, outputAC);
      digitalWrite(BrewsterGlobals::get()->pinAC2, !outputAC);
    }
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
