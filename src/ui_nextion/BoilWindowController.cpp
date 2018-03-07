#include "BoilWindowController.h"
#include "../controller/BrewsterController.h"
#include "../controller/TemperatureSensor.h"
#include "UIEvent.h"
#include <map>

#include "../util/BrewsterUtils.h"
#include "../util/TempUtils.h"

#include "../model/Recipe.h"

BoilWindowController::BoilWindowController() {
  logger = new Logger("BoilWindowController");

  bSearch.attachPop(bSearchCallback, new UIEvent(this, &bSearch));

  listenerList = new NexTouch*[2];
  listenerList[0] = &bSearch;
  listenerList[1] = NULL;
}


void BoilWindowController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  updateOutputText();
}

void BoilWindowController::deactivateScreen() {
}


void BoilWindowController::process() {

}

void BoilWindowController::updateOutputText() {
  String output = "";
  std::map<SensorLocation, TemperatureSensor>& sensors = BrewsterController::get()->getSensorManager()->getAllTemperatureSensors();

  for ( auto &p : sensors ) {
    output.concat(String::format("%2i: %s %c%c", p.first+1, (const char*) p.second.getAddressString(), 13, 10));
  }

  output = output.substring(0, 249);
  outputText.setText(output);
  logger->info("Update output: \n\r%s", (const char*)output);
}

void BoilWindowController::bSearchCallback(void *ptr)
{
    //logger->info("Search sensors button pressed");
  UIEvent *obj = (UIEvent *) ptr;
  BoilWindowController *window = (BoilWindowController *) obj->getWindowController();
  window->logger->info("Search sensors button pressed");

  Particle.process();
  //BrewsterUtils::i2c_scanner();
  TempUtils::setPrecision(DS18::PRECISION::BIT_11);

  delay(500);
  Particle.process();

  TempUtils::listSensors();

  Recipe r = *BrewsterController::get()->getRecipe();
  r.clearRecipe();
  r.printRecipe();
  r.loadFromEEPROM();
  r.printRecipe();
}
