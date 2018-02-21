#include "MashingController.h"
#include "../controller/BrewsterController.h"
#include "../controller/SensorManager.h"
#include "../controller/TemperatureSensor.h"

MashingController::MashingController() {
  logger = new Logger("MashingController");
  sensorNo = sensorLocationSize;
  temperature.resize(sensorNo, 0);
  sensors = &(BrewsterController::get()->getSensorManager()->getAllTemperatureSensors());
}


void MashingController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  for ( auto &p : (*sensors) ) {
    temperature[p.second.getLocation()] = p.second.getValue();
    if (logger->isTraceEnabled()) {
      logger->trace("Temperature Sensor - Location ID:%i  Value 1:%.2f  Value 2:%.2f Ref: 0x%X", p.second.getLocation(), p.second.getValue(), temperature[p.second.getLocation()], &p.second);
    }
  }

  updateOutputText();
}

void MashingController::deactivateScreen() {
}


void MashingController::process() {
  boolean updateNeeded = false;

  for ( auto &p : (*sensors) ) {
    if(temperature[p.second.getLocation()] != p.second.getValue()) {
      temperature[p.second.getLocation()] = p.second.getValue();
      updateNeeded = true;
    }
  }

  if (updateNeeded)
    updateOutputText();
}

void MashingController::updateOutputText() {
  /*
  unsigned long start = millis();

  String output = "";
  for (int i = 0; i<sensorNo; i++) {
    output.concat(String::format("%2i - %s: %.2f%c%c", i+1, (const char*)sensorNames[i], temperature[i], 13, 10));
  }
  outputText1.setText(output);

  unsigned long duration = millis() - start;
  if(logger->isTraceEnabled())
    logger->trace("Refreshed output of sensors in %lu ms", duration);
    */
}
