#include "SensorSearchController.h"
#include "../controller/BrewsterController.h"
#include "../controller/TemperatureSensor.h"
#include "UIEvent.h"
#include <map>

#include "../util/BrewsterUtils.h"
#include "../util/TempUtils.h"

#include "../model/Recipe.h"

SensorSearchController::SensorSearchController() {
  logger = new Logger("SensorSearchController");

  bSearch.attachPop(bSearchCallback, new UIEvent(this, &bSearch));

  listenerList = new NexTouch*[2];
  listenerList[0] = &bSearch;
  listenerList[1] = NULL;
}


void SensorSearchController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  updateOutputText();
}

void SensorSearchController::deactivateScreen() {
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
}

void SensorSearchController::bSearchCallback(void *ptr)
{
    //logger->info("Search sensors button pressed");
  UIEvent *obj = (UIEvent *) ptr;
  SensorSearchController *window = (SensorSearchController *) obj->getWindowController();
  window->logger->info("Search sensors button pressed");

  Particle.process();
  //BrewsterUtils::i2c_scanner();
  TempUtils::setPrecision(DS18::PRECISION::BIT_11);

  delay(500);
  Particle.process();

  TempUtils::listSensors();

  Recipe r = BrewsterController::get()->getRecipe();
  r.clearRecipe();
  r.printRecipe();
  r.loadFromEEPROM();
  r.printRecipe();

/*
  AdditionDO *a1;
  AdditionDO *a2;

  window->logger->info("Printing boil additions.");

  a1 = r.getCurrentBoilAddition(Time.now()+(1*60));
  a2 = r.getNextBoilAddition(Time.now()+(1*60));
  window->logger->info("Current step t0 - -1m: %s %.1f %s @ %i %s.", (const char *) a1->name, a1->qty, (const char *) QtyUOMNames[a1->qtyUOM], a1->time, (const char *) TimeUOMNames[a1->timeUOM]);
  window->logger->info("Next step t0 - -1m: %s %.1f %s @ %i %s.", (const char *) a2->name, a2->qty, (const char *) QtyUOMNames[a2->qtyUOM], a2->time, (const char *) TimeUOMNames[a2->timeUOM]);

  a1 = r.getCurrentBoilAddition(Time.now()-(2*60));
  a2 = r.getNextBoilAddition(Time.now()-(2*60));
  window->logger->info("Current step t1 - 2m: %s %.1f %s @ %i %s.", (const char *) a1->name, a1->qty, (const char *) QtyUOMNames[a1->qtyUOM], a1->time, (const char *) TimeUOMNames[a1->timeUOM]);
  window->logger->info("Next step t1 - 2m: %s %.1f %s @ %i %s.", (const char *) a2->name, a2->qty, (const char *) QtyUOMNames[a2->qtyUOM], a2->time, (const char *) TimeUOMNames[a2->timeUOM]);

  a1 = r.getCurrentBoilAddition(Time.now()-(31*60));
  a2 = r.getNextBoilAddition(Time.now()-(31*60));
  window->logger->info("Current step t2 - 31m: %s %.1f %s @ %i %s.", (const char *) a1->name, a1->qty, (const char *) QtyUOMNames[a1->qtyUOM], a1->time, (const char *) TimeUOMNames[a1->timeUOM]);
  window->logger->info("Next step t2 - 31m: %s %.1f %s @ %i %s.", (const char *) a2->name, a2->qty, (const char *) QtyUOMNames[a2->qtyUOM], a2->time, (const char *) TimeUOMNames[a2->timeUOM]);

  a1 = r.getCurrentBoilAddition(Time.now()-(46*60));
  a2 = r.getNextBoilAddition(Time.now()-(46*60));
  window->logger->info("Current step t3 - 46m: %s %.1f %s @ %i %s.", (const char *) a1->name, a1->qty, (const char *) QtyUOMNames[a1->qtyUOM], a1->time, (const char *) TimeUOMNames[a1->timeUOM]);
  window->logger->info("Next step t3 - 46m: %s %.1f %s @ %i %s.", (const char *) a2->name, a2->qty, (const char *) QtyUOMNames[a2->qtyUOM], a2->time, (const char *) TimeUOMNames[a2->timeUOM]);

  a1 = r.getCurrentBoilAddition(Time.now()-(61*60));
  a2 = r.getNextBoilAddition(Time.now()-(61*60));
  window->logger->info("Current step t4 - 61m: %s %.1f %s @ %i %s.", (const char *) a1->name, a1->qty, (const char *) QtyUOMNames[a1->qtyUOM], a1->time, (const char *) TimeUOMNames[a1->timeUOM]);
  window->logger->info("Next step t4 - 61m: %s %.1f %s @ %i %s.", (const char *) a2->name, a2->qty, (const char *) QtyUOMNames[a2->qtyUOM], a2->time, (const char *) TimeUOMNames[a2->timeUOM]);
*/

  window->logger->info("Printing mashing steps.");

  StepDO *s1;
  StepDO *s2;

  s1 = r.getCurrentMashingStep(Time.now()+(1*60));
  s2 = r.getNextMashingStep(Time.now()+(1*60));
  if (s1 != NULL)
    window->logger->info("Current step t0 - -1m: %s %i %s @ %i °C", (const char *) s1->name, s1->time, (const char *) TimeUOMNames[s1->timeUOM], s1->temperature);
  if (s2 != NULL)
    window->logger->info("Next step t0 - -1m: %s %i %s @ %i °C", (const char *) s2->name, s2->time, (const char *) TimeUOMNames[s2->timeUOM], s2->temperature);

  s1 = r.getCurrentMashingStep(Time.now()-(5*60));
  s2 = r.getNextMashingStep(Time.now()-(5*60));
  if (s1 != NULL)
    window->logger->info("Current step t1 - 5m: %s %i %s @ %i °C", (const char *) s1->name, s1->time, (const char *) TimeUOMNames[s1->timeUOM], s1->temperature);
  if (s2 != NULL)
    window->logger->info("Next step t1 - 5m: %s %i %s @ %i °C", (const char *) s2->name, s2->time, (const char *) TimeUOMNames[s2->timeUOM], s2->temperature);

  s1 = r.getCurrentMashingStep(Time.now()-(25*60));
  s2 = r.getNextMashingStep(Time.now()-(25*60));
  if (s1 != NULL)
    window->logger->info("Current step t2 - 25m: %s %i %s @ %i °C", (const char *) s1->name, s1->time, (const char *) TimeUOMNames[s1->timeUOM], s1->temperature);
  if (s2 != NULL)
    window->logger->info("Next step t2 - 25m: %s %i %s @ %i °C", (const char *) s2->name, s2->time, (const char *) TimeUOMNames[s2->timeUOM], s2->temperature);

  s1 = r.getCurrentMashingStep(Time.now()-(55*60));
  s2 = r.getNextMashingStep(Time.now()-(55*60));
  if (s1 != NULL)
    window->logger->info("Current step t3 - 55m: %s %i %s @ %i °C", (const char *) s1->name, s1->time, (const char *) TimeUOMNames[s1->timeUOM], s1->temperature);
  if (s2 != NULL)
    window->logger->info("Next step t3 - 55m: %s %i %s @ %i °C", (const char *) s2->name, s2->time, (const char *) TimeUOMNames[s2->timeUOM], s2->temperature);

  s1 = r.getCurrentMashingStep(Time.now()-(85*60));
  s2 = r.getNextMashingStep(Time.now()-(85*60));
  if (s1 != NULL)
    window->logger->info("Current step t4 - 85m: %s %i %s @ %i °C", (const char *) s1->name, s1->time, (const char *) TimeUOMNames[s1->timeUOM], s1->temperature);
  if (s2 != NULL)
    window->logger->info("Next step t4 - 85m: %s %i %s @ %i °C", (const char *) s2->name, s2->time, (const char *) TimeUOMNames[s2->timeUOM], s2->temperature);

  s1 = r.getCurrentMashingStep(Time.now()-(120*60));
  s2 = r.getNextMashingStep(Time.now()-(120*60));
  if (s1 != NULL)
    window->logger->info("Current step t5 - 120m: %s %i %s @ %i °C", (const char *) s1->name, s1->time, (const char *) TimeUOMNames[s1->timeUOM], s1->temperature);
  if (s2 != NULL)
    window->logger->info("Next step t5 - 120m: %s %i %s @ %i °C", (const char *) s2->name, s2->time, (const char *) TimeUOMNames[s2->timeUOM], s2->temperature);
}
