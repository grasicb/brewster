#include "BrewsterGlobals.h"

BrewsterGlobals::BrewsterGlobals() {
  EEPROM_PROCESS_DATA[MASHING] = 8;
  EEPROM_PROCESS_DATA[BOILING] = 18;
  EEPROM_PROCESS_DATA[CHILLING] = 28;
  EEPROM_PROCESS_DATA[FERMENTING] = 38;
}

BrewsterGlobals* BrewsterGlobals::instance = NULL;

BrewsterGlobals* BrewsterGlobals::get() {
  if (instance == NULL) {
    instance = new BrewsterGlobals();
  }

  return instance;
}
