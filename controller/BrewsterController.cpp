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
}

void BrewsterController::controllerLoop() {
//    i++;
//    logger.trace("Loop iteration: %ld", i);

    temperatureSensors->readSensors();
}
