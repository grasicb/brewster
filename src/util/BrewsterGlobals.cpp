#include "BrewsterGlobals.h"

BrewsterGlobals* BrewsterGlobals::instance = NULL;

BrewsterGlobals::BrewsterGlobals() {
  logger = new Logger("globals");
}

BrewsterGlobals* BrewsterGlobals::get() {
  if (instance == NULL) {
    instance = new BrewsterGlobals();
  }

  return instance;
}
