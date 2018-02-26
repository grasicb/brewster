#pragma once

#include "AWindowController.h"
#include "../model/Recipe.h"
#include "../process/Process.h"

class MashStepChange : public AWindowController {

public:
  MashStepChange();
  void initializeScreen(void *ptr);
  void deactivateScreen();
  void process();

protected:


private:
  Logger *logger;
  NexNumber nTemp = NexNumber(10, 6, "nTemp");
  NexNumber nTime = NexNumber(10, 15, "nTime");
  NexText tTimeUOM = NexText(10, 5, "tTimeUOM");
  NexButton bSave = NexButton(10, 19, "bSave");
  NexPage pMashingPage = NexPage(6, 0, "mashing");

  //Mash process information
  Process* mashProcess;
  StepDO* currentStep;

  static void saveButtonAcitonHandler(void *ptr);
};
