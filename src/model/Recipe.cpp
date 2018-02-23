#include "Recipe.h"

Recipe::Recipe() {
  recipe.name = "";
  recipe.boilingTime = 0;
  logger = new Logger("Recipe");
}

Recipe::Recipe(String name) {
  /*
  if (name.length()>=MAX_STR_LEN) {
    logger->warn("Addition name longer than allowed (max length: %i, addition name: %s).", MAX_STR_LEN, (const char*) name);
  }
  name.toCharArray(recipe.name, MAX_STR_LEN);
  */
  recipe.name = name;
  recipe.boilingTime = 0;
  logger = new Logger("Recipe");
}

/************************
** Loading & Storing
*************************/

void Recipe::loadFromEEPROM() {
  logger->info("Loading recipe from EEPROM (location=%i).", EEPROM_RECIPE);
  EEPROM.get(EEPROM_RECIPE, recipe);
}

void Recipe::storeToEEPROM() {
  logger->info("Storing recipe to EEPROM (size=%u).", sizeof(recipe));
  EEPROM.put(EEPROM_RECIPE, recipe);
}

void Recipe::clearRecipe() {
  //recipe.name[0] = 0;
  recipe.name = "";
  recipe.boilingTime = 0;
  recipe.mashingSteps.clear();
  recipe.boilAdditions.clear();
  recipe.fermentationSteps.clear();
  recipe.fermentationPhases.clear();
}

void Recipe::printRecipe() {
  logger->info("Recipe %s:", (const char*) recipe.name);

  if (recipe.mashingSteps.size()>0) {
    logger->info("  Mashing steps:");
    for(StepDO step : recipe.mashingSteps) {
       logger->info("    %s %i %s @ %i °C", (const char *) step.name, step.time, (const char *) TimeUOMNames[step.timeUOM], step.temperature);
     }
  }

  logger->info("  Boling time: %i min", recipe.boilingTime);

  if (recipe.boilAdditions.size()>0) {
    logger->info("  Boil additions:");
    for(AdditionDO add : recipe.boilAdditions) {
       logger->info("    %s %.1f %s @ %i %s.", (const char *) add.name, add.qty, (const char *) QtyUOMNames[add.qtyUOM], add.time, (const char *) TimeUOMNames[add.timeUOM]);
     }
  }

  if (recipe.fermentationSteps.size()>0) {
    logger->info("  Fermentation steps:");
    for(StepDO step : recipe.fermentationSteps) {
       logger->info("    %s %i %s @ %i °C", (const char *) step.name, step.time, (const char *) TimeUOMNames[step.timeUOM], step.temperature);
     }
  }

  if (recipe.fermentationPhases.size()>0) {
    logger->info("  Fermentation phases:");
    for(StepDO step : recipe.fermentationPhases) {
       logger->info("    %s %i %s", (const char *) step.name, step.time, (const char *) TimeUOMNames[step.timeUOM]);
     }
  }
}

/************************
** General functions
*************************/

void Recipe::setName(String name) {
  recipe.name = name;
}

String Recipe::getName() {
  return recipe.name;
}

void Recipe::setBoilingTime(uint8_t time) {
  recipe.boilingTime = time;
}

uint8_t Recipe::getBoilingTime() {
  return recipe.boilingTime;
}

/************************
** Boil addition functions
*************************/

void Recipe::addBoilAddition(AdditionDO addition) {
  recipe.boilAdditions.push_back(addition);
}

void Recipe::addBoilAddition(String name, float qty, QtyUOM qtyUOM, uint8_t time, TimeUOM timeUOM) {
  AdditionDO add;
  /*
  if (name.length()>=MAX_STR_LEN) {
    logger->warn("Addition name longer than allowed (max length: %i, addition name: %s).", MAX_STR_LEN, (const char*) name);
  }
  name.toCharArray(add.name, MAX_STR_LEN);
  */
  add.name = name;
  add.qty = qty;
  add.qtyUOM = qtyUOM;
  add.time = time;
  add.timeUOM = timeUOM;

  addBoilAddition(add);
}

int Recipe::getBoilAdditionsCount() {
    return recipe.boilAdditions.size();
}

AdditionDO *Recipe::getBoilAddition(int i) {
  if(i >= (int)recipe.boilAdditions.size()) {
    logger->error("getBoildAddition: requested element beyond the size of array (size: %i, requested: %i)", recipe.boilAdditions.size(), i);
    i = recipe.boilAdditions.size()-1;
  }
  return &recipe.boilAdditions[i];
}

std::vector<AdditionDO> Recipe::getPreviousBoilAdditions(unsigned long boilStartTime) {
  unsigned long curTime = Time.now();
  std::vector<AdditionDO> additions;

  for (std::vector<AdditionDO>::iterator it = recipe.boilAdditions.begin() ; it != recipe.boilAdditions.end(); ++it) {
    unsigned int dt = curTime - boilStartTime - BrewsterUtils::getSeconds(it->time, it->timeUOM);

    //If the addition is from the past 5 minutes
    if(dt > 0 && dt < 300) {
      additions.push_back(*it);
    }
  }

  return additions;
}

std::vector<AdditionDO> Recipe::getNextBoilAdditions(unsigned long boilStartTime) {
  unsigned long curTime = Time.now();
  std::vector<AdditionDO> additions;

  for (std::vector<AdditionDO>::iterator it = recipe.boilAdditions.begin() ; it != recipe.boilAdditions.end(); ++it) {
    unsigned int t = boilStartTime + BrewsterUtils::getSeconds(it->time, it->timeUOM);

    //If the step is in the future, then add it to the list
    if(t > curTime) {
      additions.push_back(*it);
    }
  }

  return additions;
}

/************************
** Mashing step functions
*************************/
void Recipe::addMashingStep(StepDO& step) {
    addStep(recipe.mashingSteps, step);
}
void Recipe::addMashingStep(String name, uint8_t time, TimeUOM timeUOM, uint8_t temperature) {
  addStep(recipe.mashingSteps, name, time, timeUOM, temperature);
}
int Recipe::getMashingStepsCount() {
  return recipe.mashingSteps.size();
}
StepDO* Recipe::getMashingStep(int i) {
    return getStep(recipe.mashingSteps, i);
}
StepDO* Recipe::getCurrentMashingStep(unsigned long startTime) {
  return getCurrentStep(recipe.mashingSteps, startTime);
}
StepDO* Recipe::getNextMashingStep(unsigned long startTime) {
  return getNextStep(recipe.mashingSteps, startTime);
}
unsigned long Recipe::getMashingStepStartTime(StepDO* selectedStep) {
  return getStepStartTime(recipe.mashingSteps, selectedStep);
}
unsigned long Recipe::getMashingTime() {
  return getTotalStepsTime(recipe.mashingSteps);
}

/************************
** Fermentation step functions
*************************/
void Recipe::addFermentationStep(StepDO& step) {
    addStep(recipe.fermentationSteps, step);
}
void Recipe::addFermentationStep(String name, uint8_t time, TimeUOM timeUOM, uint8_t temperature) {
  addStep(recipe.fermentationSteps, name, time, timeUOM, temperature);
}
int Recipe::getFermentationStepsCount() {
  return recipe.fermentationSteps.size();
}
StepDO* Recipe::getFermentationStep(int i) {
    return getStep(recipe.fermentationSteps, i);
}
StepDO* Recipe::getCurrentFermentationStep(unsigned long startTime) {
  return getCurrentStep(recipe.fermentationSteps, startTime);
}
StepDO* Recipe::getNextFermentationStep(unsigned long startTime) {
  return getNextStep(recipe.fermentationSteps, startTime);
}
unsigned long Recipe::getFermentationStepStartTime(StepDO* selectedStep) {
  return getStepStartTime(recipe.fermentationSteps, selectedStep);
}

/************************
** Fermentation phase functions
*************************/
void Recipe::addFermentationPhase(StepDO& step) {
    addStep(recipe.fermentationPhases, step);
}
void Recipe::addFermentationPhase(String name, uint8_t time, TimeUOM timeUOM) {
  addStep(recipe.fermentationPhases, name, time, timeUOM, 0);
}
int Recipe::getFermentationPhaseCount() {
  return recipe.fermentationPhases.size();
}
StepDO* Recipe::getFermentationPhase(int i) {
    return getStep(recipe.fermentationPhases, i);
}
StepDO* Recipe::getCurrentFermentationPhase(unsigned long startTime) {
  return getCurrentStep(recipe.fermentationPhases, startTime);
}
StepDO* Recipe::getNextFermentationPhase(unsigned long startTime) {
  return getNextStep(recipe.fermentationPhases, startTime);
}
unsigned long Recipe::getFermentationPhaseStartTime(StepDO* selectedStep) {
  return getStepStartTime(recipe.fermentationPhases, selectedStep);
}
unsigned long Recipe::getFermentationTime() {
  return getTotalStepsTime(recipe.fermentationPhases);
}

/************************
** Help functions for step manipulation
*************************/

void Recipe::addStep(std::vector<StepDO>& steps, StepDO& step) {
    steps.push_back(step);
}

void Recipe::addStep(std::vector<StepDO>& steps, String name, uint8_t time, TimeUOM timeUOM, uint8_t temperature) {
    StepDO step;
    step.name = name;
    step.time = time;
    step.timeUOM = timeUOM;
    step.temperature = temperature;

    addStep(steps, step);
}

StepDO* Recipe::getStep(std::vector<StepDO>& steps, int i) {
  if(i >= (int)steps.size()) {
    logger->error("getStep: requested element beyond the size of array (size: %i, requested: %i)", steps.size(), i);
    i = steps.size()-1;
  }

  return &steps[i];
}

StepDO* Recipe::getCurrentStep(std::vector<StepDO>& steps, unsigned long startTime) {
  logger->info("Get current step [start time=%u]", startTime);

  unsigned long accTime = startTime;
  unsigned long curTime = Time.now();
  uint i=0;

  if(accTime == 0)
    accTime = Time.now();

  for(StepDO& step : steps) {
    accTime += BrewsterUtils::getSeconds(step.time, step.timeUOM);
    if(accTime > curTime)
      break;
    i++;
  }

  if(i >= steps.size())
    i = steps.size()-1;

  if(steps.size()==0)
    return NULL;
  else
    return &steps[i];
}

StepDO* Recipe::getNextStep(std::vector<StepDO>& steps, unsigned long startTime) {
  logger->info("Get next step [start time=%u]", startTime);

  unsigned long accTime = startTime;
  unsigned long curTime = Time.now();
  uint i=0;

  if(accTime == 0)
    accTime = Time.now();

  for(StepDO& step : steps) {
    accTime += BrewsterUtils::getSeconds(step.time, step.timeUOM);
    if(accTime > curTime)
      break;
    i++;
  }

  if(steps.size()==0)
    return NULL;
  else if (i>=steps.size()-1)
    return NULL;
  else
    return &steps[i+1];
}

unsigned long Recipe::getStepStartTime(std::vector<StepDO>& steps, StepDO* selectedStep) {
  unsigned long totalTime=0;

  for(StepDO& step : steps) {
    if(&step == selectedStep)
      return totalTime;
    totalTime+=BrewsterUtils::getSeconds(step.time, step.timeUOM);
  }
  logger->warn("Step not found with the first approach");

  for(std::vector<StepDO>::iterator it=steps.begin(); it!=steps.end(); ++it) {
    if(&*it == selectedStep)
      return totalTime;
    totalTime+=BrewsterUtils::getSeconds(it->time, it->timeUOM);
  }
  logger->warn("Step not found with the second approach");

  return 0;
}

unsigned long Recipe::getTotalStepsTime(std::vector<StepDO>& steps) {
  unsigned long totalTime=0;

  for(StepDO& step : steps) {
    totalTime+=BrewsterUtils::getSeconds(step.time, step.timeUOM);
  }

  return totalTime;
}
