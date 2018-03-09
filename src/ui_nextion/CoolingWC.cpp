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

  listenerList = new NexTouch*[3];
  listenerList[0] = &b1;
  listenerList[1] = &b2;
  listenerList[2] = NULL;
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

  //Set button text
  setStartTime();
  updateButtonText();
  updateTime();

  lastTempBK = 999;
  lastTempCooler = 999;
  lastTempFermentor = 999;

  //Adding listener for change in process & output state
  coolingProcess->addListener(processStateChangeHandler, this);
}

void CoolingWC::deactivateScreen() {
  coolingProcess->removeListener(processStateChangeHandler);
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
        tTimeElapsed.setText(String::format("%i min", runTime));
    }
  }
}

void CoolingWC::setStartTime() {
  startTime = coolingProcess->getStartTime();
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

void CoolingWC::processStateChangeHandler(void* callingObject, ProcessStateChangeEvent event) {
  CoolingWC* wc = (CoolingWC*) callingObject;

  wc->setStartTime();
  wc->updateButtonText();
}
