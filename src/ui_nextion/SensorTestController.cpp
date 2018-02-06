#include "SensorTestController.h"
#include "../controller/BrewsterController.h"

SensorTestController::SensorTestController() {
  logger = new Logger("SensorSearchController");
}


void SensorTestController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  for (int i=0; i<10; i++) {
    temperature[i] = BrewsterController::get()->temperatureSensors->temperature[i];
  }
  updateOutputText();
}


void SensorTestController::process() {
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

void SensorTestController::updateOutputText() {
  unsigned long start = millis();

  String output = "";
  for (int i = 0; i<5; i++) {
    //output.concat(String::format("%2i: %.2f %cC%c%c", i+1, temperature[i], 176, 13, 10));
    output.concat(String::format("%2i: %.2f %c%c", i+1, temperature[i], 13, 10));
  }
  outputText1.setText(output);


  output = "";
  for (int i = 5; i<10; i++) {
    //output.concat(String::format("%2i: %.2f %cC%c%c", i+1, temperature[i], 176, 13, 10));
    output.concat(String::format("%2i: %.2f %c%c", i+1, temperature[i], 13, 10));
  }
  outputText2.setText(output);

  unsigned long duration = millis() - start;
  logger->info("Refreshed output of sensors in %lu ms", duration);
}
