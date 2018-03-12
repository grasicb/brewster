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
  bool operator<(const AdditionDO &add) const { return time < add.time; }
  bool operator>(const AdditionDO &add) const { return time > add.time; }
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
  uint8_t targetCoolingTemperature;
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

  void setTargetCoollingTemperature(uint8_t temperature);
  uint8_t getTargetCoollingTemperature();

  void addBoilAddition(AdditionDO addition);
  void addBoilAddition(String name, float qty, QtyUOM qtyUOM, uint8_t time, TimeUOM timeUOM);
  int getBoilAdditionsCount();
  AdditionDO* getBoilAddition(int i);
  unsigned long getNextBoilAdditionTime(unsigned long boilStartTime);
  std::vector<AdditionDO*> getPreviousBoilAdditions(unsigned long boilStartTime);
  std::vector<AdditionDO*> getNextBoilAdditions(unsigned long boilStartTime);

  void addMashingStep(StepDO& step);
  void addMashingStep(String name, uint8_t time, TimeUOM timeUOM, uint8_t temperature);
  int getMashingStepsCount();
  StepDO* getMashingStep(int i);
  StepDO* getCurrentMashingStep(unsigned long startTime);
  StepDO* getNextMashingStep(unsigned long startTime);
  unsigned long getMashingStepStartTime(StepDO* selectedStep);
  unsigned long getMashingTime();

  void addFermentationStep(StepDO& step);
  void addFermentationStep(String name, uint8_t time, TimeUOM timeUOM, uint8_t temperature);
  int getFermentationStepsCount();
  StepDO* getFermentationStep(int i);
  StepDO* getCurrentFermentationStep(unsigned long startTime);
  StepDO* getNextFermentationStep(unsigned long startTime);
  unsigned long getFermentationStepStartTime(StepDO* selectedStep);

  void addFermentationPhase(StepDO& step);
  void addFermentationPhase(String name, uint8_t time, TimeUOM timeUOM);
  int getFermentationPhaseCount();
  StepDO* getFermentationPhase(int i);
  StepDO* getCurrentFermentationPhase(unsigned long startTime);
  StepDO* getNextFermentationPhase(unsigned long startTime);
  unsigned long getFermentationPhaseStartTime(StepDO* selectedStep);
  unsigned long getFermentationTime();

private:
  RecipeDO recipe;
  Logger *logger;

  void addStep(std::vector<StepDO>& steps, StepDO& step);
  void addStep(std::vector<StepDO>& steps, String name, uint8_t time, TimeUOM timeUOM, uint8_t temperature);
  StepDO* getStep(std::vector<StepDO>& steps, int i);
  StepDO* getCurrentStep(std::vector<StepDO>& steps, unsigned long startTime);
  StepDO* getNextStep(std::vector<StepDO>& steps, unsigned long startTime);
  unsigned long getStepStartTime(std::vector<StepDO>& steps, StepDO* selectedStep);
  unsigned long getTotalStepsTime(std::vector<StepDO>& steps);
};
