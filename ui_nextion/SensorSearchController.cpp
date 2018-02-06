#include "SensorSearchController.h"
#include "../controller/BrewsterController.h"

SensorSearchController::SensorSearchController() {
  logger = new Logger("SensorSearchController");
}


void SensorSearchController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  for (int i=0; i<10; i++) {
    temperature[i] = BrewsterController::get()->temperatureSensors->temperature[i];
    updateOutputText();
  }
}


void SensorSearchController::process() {
  boolean updateNeeded = false;

  for (int i = 0; i<10; i++) {
    if (BrewsterController::get()->temperatureSensors->temperature[i] != temperature[i]) {
      temperature[i] = BrewsterController::get()->temperatureSensors->temperature[i];
      updateNeeded = true;
    }
  }

  if (updateNeeded)
    updateOutputText();
}

void SensorSearchController::updateOutputText() {
  String output = "";

  for (int i = 0; i<10; i++) {
    output.concat(String::format("%2i: %.2f %cC%c%c", i+1, temperature[i], 176, 13, 10));
  }

  outputText.setText(output);
}
