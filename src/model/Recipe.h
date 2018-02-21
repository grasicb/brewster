#pragma once

#include "application.h"
#include "../util/BrewsterGlobals.h"
#include "../util/BrewsterUtils.h"
#include <vector>

#define MAX_STR_LEN 20

struct AdditionDO { //24
  String name;
  float qty;
  QtyUOM qtyUOM;
  uint8_t time;
  TimeUOM timeUOM;
};

struct StepDO { //21 byte
  String name;
  uint8_t time;
  TimeUOM timeUOM;
  uint8_t temperature;
};

struct RecipeDO {
  String name;
  std::vector<StepDO> mashingSteps;
  uint8_t boilingTime;
  std::vector<AdditionDO> boilAdditions;
  std::vector<StepDO> fermentationSteps;
  std::vector<StepDO> fermentationPhases;
};

class Recipe {
public:
  Recipe();
  Recipe(String name);

  void loadFromEEPROM();
  void storeToEEPROM();

  void clearRecipe();
  void printRecipe();

  void setName(String name);
  String getName();

  void setBoilingTime(uint8_t time);
  uint8_t getBoilingTime();

  void addBoilAddition(AdditionDO addition);
  void addBoilAddition(String name, float qty, QtyUOM qtyUOM, uint8_t time, TimeUOM timeUOM);
  int getBoilAdditionsCount();
  AdditionDO* getBoilAddition(int i);
  std::vector<AdditionDO>& getPreviousBoilAdditions(unsigned long boilStartTime);
  std::vector<AdditionDO>& getNextBoilAdditions(unsigned long boilStartTime);

  void addMashingStep(StepDO& step);
  void addMashingStep(String name, uint8_t time, TimeUOM timeUOM, uint8_t temperature);
  int getMashingStepsCount();
  StepDO* getMashingStep(int i);
  StepDO* getCurrentMashingStep(unsigned long startTime);
  StepDO* getNextMashingStep(unsigned long startTime);

  void addFermentationStep(StepDO& step);
  void addFermentationStep(String name, uint8_t time, TimeUOM timeUOM, uint8_t temperature);
  int getFermentationStepsCount();
  StepDO* getFermentationStep(int i);
  StepDO* getCurrentFermentationStep(unsigned long startTime);
  StepDO* getNextFermentationStep(unsigned long startTime);

  void addFermentationPhase(StepDO& step);
  void addFermentationPhase(String name, uint8_t time, TimeUOM timeUOM);
  int getFermentationPhaseCount();
  StepDO* getFermentationPhase(int i);
  StepDO* getCurrentFermentationPhase(unsigned long startTime);
  StepDO* getNextFermentationPhase(unsigned long startTime);

private:
  RecipeDO recipe;
  Logger *logger;

  void addStep(std::vector<StepDO>& steps, StepDO& step);
  void addStep(std::vector<StepDO>& steps, String name, uint8_t time, TimeUOM timeUOM, uint8_t temperature);
  StepDO* getStep(std::vector<StepDO>& steps, int i);
  StepDO* getCurrentStep(std::vector<StepDO>& steps, unsigned long startTime);
  StepDO* getNextStep(std::vector<StepDO>& steps, unsigned long startTime);
};
