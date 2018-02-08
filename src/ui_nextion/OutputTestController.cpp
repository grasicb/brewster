#include "OutputTestController.h"
#include "../controller/BrewsterController.h"
#include "../controller/SensorManager.h"
#include "../controller/TemperatureSensor.h"

OutputTestController::OutputTestController() {
  logger = new Logger("OutputTestController");

  sensors = &(BrewsterController::get()->getSensorManager()->getAllTemperatureSensors());
  it=sensors->begin();
  selectedSensor = it->first;
}


void OutputTestController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);


}


void OutputTestController::process() {

}
