#include "SensorSearchController.h"
#include "../controller/BrewsterController.h"
#include "../controller/TemperatureSensor.h"
#include <map>

SensorSearchController::SensorSearchController() {
  logger = new Logger("SensorSearchController");
}


void SensorSearchController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  updateOutputText();
}


void SensorSearchController::process() {

}

void SensorSearchController::updateOutputText() {
  String output = "";
  std::map<SensorLocation, TemperatureSensor>& sensors = BrewsterController::get()->getSensorManager()->getAllTemperatureSensors();

  for ( auto &p : sensors ) {
    output.concat(String::format("%2i: %s %c%c", p.first+1, (const char*) p.second.getAddressString(), 13, 10));
  }

  output = output.substring(0, 249);
  outputText.setText(output);
  logger->info("Update output: \n\r%s", (const char*)output);

  /*
  for (int i = 0; i<3; i++) {
    //output.concat(String::format("%2i: %.2f %cC%c%c", i+1, temperature[i], 176, 13, 10));

    uint8_t *ts = BrewsterGlobals::get()->tempSensors[i];

    output.concat(String::format("%2i: %2X %2X %2X %2X %2X %2X %2X %2X %c%c", i+1, ts[0], ts[1], ts[2], ts[3], ts[4], ts[5], ts[6], ts[7], 13, 10));
  }
  */

}
