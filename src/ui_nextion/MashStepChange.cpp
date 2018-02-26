#include "MashStepChange.h"
#include "../controller/BrewsterController.h"
#include "UIEvent.h"

MashStepChange::MashStepChange() {
  logger = new Logger("MashStepChange");

  bSave.attachPop(saveButtonAcitonHandler, new UIEvent(this, &bSave));

  listenerList = new NexTouch*[2];
  listenerList[0] = &bSave;
  listenerList[1] = NULL;
}


void MashStepChange::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  mashProcess = BrewsterController::get()->getProcessManager()->getProcess(BrewProcess::MASHING);
  Recipe *r = mashProcess->getRecipe();
  if(r != NULL)  {
    currentStep = r->getCurrentMashingStep(mashProcess->getStartTime());
  }else {
    currentStep = NULL;
    logger->error("Current step cannot be identified");
  }

  if(currentStep != NULL) {
    nTemp.setValue(currentStep->temperature);
    nTime.setValue(currentStep->time);
    tTimeUOM.setText(TimeUOMNames[currentStep->timeUOM]);
  }
}

void MashStepChange::deactivateScreen() {
}


void MashStepChange::process() {

}

void MashStepChange::saveButtonAcitonHandler(void *ptr)
{
    //logger->info("Search sensors button pressed");
  UIEvent *obj = (UIEvent *) ptr;
  MashStepChange *wc = (MashStepChange *) obj->getWindowController();
  wc->logger->info("Save button pressed");

  if(wc->currentStep != NULL) {
    uint32_t vTemp;
    uint32_t vTime;
    wc->nTemp.getValue(&vTemp);
    wc->nTime.getValue(&vTime);
    if(vTemp<0) {
        wc->logger->error("Invalid temperature set (temp: %i). Temp should be higher or equal to 0.", vTemp);
        vTemp = 0;
    }
    if(vTime<=0) {
        wc->logger->error("Invalid time set (time: %i). Time should be higher than 0.", vTime);
        vTime = 1;
    }
    wc->currentStep->time = vTime;
    wc->currentStep->temperature = vTemp;

  }else{
    wc->logger->error("Cannot save changes because current step could not be identified");
  }

  wc->pMashingPage.show();
}
