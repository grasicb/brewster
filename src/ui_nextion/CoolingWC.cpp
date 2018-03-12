#include "CoolingWC.h"
#include "../controller/BrewsterController.h"
#include "../controller/TemperatureSensor.h"
#include "UIEvent.h"
#include <map>

#include "../util/BrewsterUtils.h"
#include "../util/TempUtils.h"

#include "../model/Recipe.h"

CoolingWC::CoolingWC() {
  logger = new Logger("CoolingWC");

  tempSensorBK = &(BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::BK));
  tempSensorCooler = &(BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::COOLER_OUT));
  tempSensorFermentor = &(BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::FERMENTOR));

  b1.attachPop(bTriggerProcessCB, new UIEvent(this, &b1));
  b2.attachPop(bTriggerProcessCB, new UIEvent(this, &b2));
  bPump.attachPop(triggerPumpButtonAH, new UIEvent(this, &bPump));
  bSettings.attachPop(triggerSettingsButtonAH, new UIEvent(this, &bSettings));


  listenerList = new NexTouch*[4];
  listenerList[0] = &b1;
  listenerList[1] = &b2;
  listenerList[2] = &bPump;
  listenerList[3] = &bSettings;
  listenerList[4] = NULL;
}


void CoolingWC::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  logger->info("Initializing cooling screen");

  coolingProcess = BrewsterController::get()->getProcessManager()->getProcess(BrewProcess::COOLING);
  if (coolingProcess->isActive() && coolingProcess->getRecipe() != NULL)
    recipe = BrewsterController::get()->getRecipe();
  else {
    recipe = BrewsterController::get()->getRecipe();
    coolingProcess->setRecipe(recipe);
  }

  //Init temperature values and time value to trigger update on next process loop
  lastTempBK = 999;
  lastTempCooler = 999;
  lastTempFermentor = 999;
  startTime = 0;

  //Set button text
  setStartTime();
  updateButtonText();
  nTargetTemp.setValue(recipe->getTargetCoollingTemperature());

  //Set pump status
  if(BrewsterController::get()->isOutputActive(coolingPump))
    bPump.setValue(1);
  else
    bPump.setValue(0);

  //Adding listener for change in process & output state
  coolingProcess->addListener(processStateChangeHandler, this);
  BrewsterController::get()->getOutput(coolingPump)->addListener(pumpStateChanged, this, coolingPump);
}

void CoolingWC::deactivateScreen() {
  coolingProcess->removeListener(processStateChangeHandler);
  BrewsterController::get()->getOutput(coolingPump)->removeListener(pumpStateChanged);

}


void CoolingWC::process() {
  if(lastTempBK != tempSensorBK->getValue()) {
    lastTempBK = tempSensorBK->getValue();
    tTempBK.setText(String::format("%.1f", lastTempBK));
  }

  if(lastTempCooler != tempSensorCooler->getValue()) {
    lastTempCooler = tempSensorCooler->getValue();
    tTempCooler.setText(String::format("%.1f", lastTempCooler));
  }

  if(lastTempFermentor != tempSensorFermentor->getValue()) {
    lastTempFermentor = tempSensorFermentor->getValue();
    if(lastTempFermentor != 0)
      tTempFermentor.setText(String::format("%.1f", lastTempFermentor));
    else
      tTempFermentor.setText("");
  }


  updateTime();
}

void CoolingWC::updateTime() {
  ProcessState state = coolingProcess->getState();

  if(state != ProcessState::STOPPED) {
    boolean timeUpdate = false;
    long newRunTime = BrewsterUtils::convertSeconds(Time.now()-startTime, TimeUOM::minute);
    if (runTime != newRunTime) {
        runTime = newRunTime;
        timeUpdate = true;
    }

    //If time update is needed
    if(timeUpdate) {
        logger->trace("Runtime changed to: %u", runTime);
        tTimeElapsed.setText(String::format("%i min", runTime));
    }
  }
}

void CoolingWC::setStartTime() {
  startTime = coolingProcess->getStartTime();
  logger->trace("Start time set to %lu", startTime);
}

void CoolingWC::updateButtonText() {
  ProcessState state = coolingProcess->getState();

  if(state == ProcessState::STOPPED) {
    NexSendCommand("vis b2,0");
    b1.setText(bStartText);
  }else if (state == ProcessState::STARTED) {
    NexSendCommand("vis b2,1");
    b1.setText(bStopText);
    b2.setText(bPauseText);
  }else if (state == ProcessState::PAUSED) {
    NexSendCommand("vis b2,0");
    b1.setText(bResumeText);
  }else{
    logger->error("Unknown process state %s. Not processing initialization of buttons.", (const char*) ProcessStateNames[state] );

  }
}

////////////////////////////////////
// Event Handlers

void CoolingWC::bTriggerProcessCB(void *ptr)
{
    //logger->info("Search sensors button pressed");
  UIEvent *obj = (UIEvent *) ptr;
  CoolingWC *wc = (CoolingWC *) obj->getWindowController();
  NexButton *button = (NexButton *) obj->getButton();
  char* buf = (char*)malloc(20);
  button->getText(buf, 20);
  String buttonText = String(buf);

  wc->logger->info("Trigger process button pressed[action: %s].", (const char*) buttonText);

  if(wc->bStartText.equals(buttonText)) {
    wc->coolingProcess->start();
  }else if (wc->bStopText.equals(buttonText)) {
    wc->coolingProcess->stop();
  }else if(wc->bPauseText.equals(buttonText)) {
    wc->coolingProcess->pause();
  }else if (wc->bResumeText.equals(buttonText)) {
    wc->coolingProcess->resume();
  }else{
    wc->logger->error("Start/Stop button state unknow. Not processing button press.");
  }
}

void CoolingWC::triggerPumpButtonAH(void *ptr) {
  UIEvent *obj = (UIEvent *) ptr;
  NexDSButton *button = (NexDSButton *)obj->getButton();
  CoolingWC *wc = (CoolingWC *) obj->getWindowController();
  wc->logger->trace("Pump button pressed");

  uint32_t value;
  button->getValue(&value);

  //If the process is started then pause and resume the process to turn-on/-ff the pump
  if(wc->coolingProcess->getState() != ProcessState::STOPPED) {
    if(value==0)
      wc->coolingProcess->pause();
    else
      wc->coolingProcess->resume();

  //If the process is not started, then cycle power state of the pump
  }else{
    if(value==0)
      BrewsterController::get()->getOutput(coolingPump)->setOutput(0);
    else
      BrewsterController::get()->getOutput(coolingPump)->setOutput(50);
  }
}

void CoolingWC::triggerSettingsButtonAH(void *ptr) {
  UIEvent *obj = (UIEvent *) ptr;
  NexDSButton *button = (NexDSButton *)obj->getButton();
  CoolingWC *wc = (CoolingWC *) obj->getWindowController();
  wc->logger->trace("Settings button pressed");

  uint32_t value;
  button->getValue(&value);

  //Set the new target value in the recipe if the value has been changed
  if(value == 0) {
    wc->nTargetTemp.getValue(&value);

    wc->recipe->setTargetCoollingTemperature(value);
    //If the process is running, pause and resume the process in order to activate the new target value
    if(wc->coolingProcess->getState() == ProcessState::STARTED) {
      wc->coolingProcess->pause();
      wc->coolingProcess->resume();
    }
  }
}

void CoolingWC::processStateChangeHandler(void* callingObject, ProcessStateChangeEvent event) {
  CoolingWC* wc = (CoolingWC*) callingObject;

  wc->setStartTime();
  wc->updateButtonText();
}

void CoolingWC::pumpStateChanged(void* callingObject, int outputIdentifier, OutputChangeEvent event) {
  CoolingWC *wc = (CoolingWC *) callingObject;

  wc->logger->info("Output event change received for cooling pump [ON=%i, AUTO=%i, VALUE=%.1f]", outputIdentifier, (int)event.isActive, (int)event.isPID, event.targetValue);

  uint32_t value;
  wc->bPump.getValue(&value);

  if(event.isActive && value == 0)
    wc->bPump.setValue(1);
  else if (!event.isActive && value == 1)
    wc->bPump.setValue(0);
}
