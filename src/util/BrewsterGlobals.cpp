#include "BrewsterGlobals.h"

BrewsterGlobals* BrewsterGlobals::instance = NULL;

BrewsterGlobals* BrewsterGlobals::get() {
  if (instance == NULL) {
    instance = new BrewsterGlobals();
  }

  return instance;
}
