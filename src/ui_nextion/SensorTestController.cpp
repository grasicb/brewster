#include "SensorTestController.h"
#include "../controller/BrewsterController.h"
#include "../controller/SensorManager.h"
#include "../controller/TemperatureSensor.h"

SensorTestController::SensorTestController() {
  logger = new Logger("SensorSearchController");
  sensorNo = sensorLocationSize;
  temperature.resize(sensorNo, 0);
  sensors = &(BrewsterController::get()->getSensorManager()->getAllTemperatureSensors());
}


void SensorTestController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  for ( auto &p : (*sensors) ) {
    temperature[p.second.getLocation()] = p.second.getValue();
    if (logger->isTraceEnabled()) {
      logger->trace("Temperature Sensor - Location ID:%i  Value 1:%.2f  Value 2:%.2f", p.second.getLocation(), p.second.getValue(), temperature[p.second.getLocation()]);
    }
  }

  updateOutputText();
}

void SensorTestController::deactivateScreen() {
}


void SensorTestController::process() {
  boolean updateNeeded = false;

  for ( auto &p : (*sensors) ) {
    if(temperature[p.second.getLocation()] != p.second.getValue()) {
      temperature[p.second.getLocation()] = p.second.getValue();
      updateNeeded = true;
    }
  }

/*
  for (int i = 0; i<10; i++) {
    if (BrewsterController::get()->temperatureSensors->temperature[i] != temperature[i]) {
      temperature[i] = BrewsterController::get()->temperatureSensors->temperature[i];
      updateNeeded = true;
    }
  }
*/
  if (updateNeeded)
    updateOutputText();
}

void SensorTestController::updateOutputText() {
  unsigned long start = millis();

  String output = "";
  for (int i = 0; i<sensorNo; i++) {
    output.concat(String::format("%2i - %s: %.2f%c%c", i+1, (const char*)sensorNames[i], temperature[i], 13, 10));
  }
  outputText1.setText(output);


  /* Legacy code - from the time when test screen was split in 2
  output = "";
  for (int i = sensorNo/2; i<sensorNo; i++) {
    output.concat(String::format("%2i: %.2f (%s)%c%c", i+1, temperature[i], (const char*)sensorShortNames[i], 13, 10));
  }
  outputText2.setText(output);
  */

  unsigned long duration = millis() - start;
  if(logger->isTraceEnabled())
    logger->trace("Refreshed output of sensors in %lu ms", duration);
}
