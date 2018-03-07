#include "BoilWindowController.h"
#include "../controller/BrewsterController.h"
#include "../controller/TemperatureSensor.h"
#include "UIEvent.h"
#include <map>

#include "../util/BrewsterUtils.h"
#include "../util/TempUtils.h"

#include "../model/Recipe.h"

BoilWindowController::BoilWindowController() {
  logger = new Logger("BoilWindowController");

  tempSensor = &(BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::BK));

  b1.attachPop(bTriggerProcessCB, new UIEvent(this, &b1));
  b2.attachPop(bTriggerProcessCB, new UIEvent(this, &b2));

  listenerList = new NexTouch*[3];
  listenerList[0] = &b1;
  listenerList[1] = &b2;
  listenerList[2] = NULL;
}


void BoilWindowController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  logger->info("Initializing boil screen");
  pbProgress.setValue(0);

  boilProcess = (BoilProcess *)BrewsterController::get()->getProcessManager()->getProcess(BrewProcess::BOILING);
  if (boilProcess->isActive() && boilProcess->getRecipe() != NULL)
    recipe = BrewsterController::get()->getRecipe();
  else {
    recipe = BrewsterController::get()->getRecipe();
    boilProcess->setRecipe(recipe);
  }

  //Set button text
  setStartTime();
  updateButtonText();
  updateTime();
  updateAdditions();

  lastTemp = tempSensor->getValue();
  tTemp.setText(String::format("%.1f", lastTemp));

  //Adding listener for change in process & output state
  boilProcess->addListener(processInfoChangeHandler, this);
  boilProcess->addListener(processStateChangeHandler, this);
}

void BoilWindowController::deactivateScreen() {
  boilProcess->removeListener(processInfoChangeHandler);
  boilProcess->removeListener(processStateChangeHandler);
}


void BoilWindowController::process() {
  if(lastTemp != tempSensor->getValue()) {
    lastTemp = tempSensor->getValue();
    tTemp.setText(String::format("%.1f", lastTemp));
  }

  updateTime();
}

void BoilWindowController::updateAdditions() {
  std::vector<AdditionDO> additions = recipe->getNextBoilAdditions(startTime);
  int size = additions.size();
  if(size>6)
    size = 6;
  for(int i=0; i<size; i++) {
    tAdd[i].setText(String::format("%i %s: %.1f %s %s", additions[i].time, TimeUOMNames[additions[i].timeUOM], additions[i].qty, QtyUOM[additions[i].qtyUOM], name));
  }

  for(int i=size; i<6; i++) {
    tAdd[i].setText("");
  }
}

void BoilWindowController::updateTime() {
  ProcessState state = boilProcess->getState();
  if(state != ProcessState::STOPPED) {
    boolean timeUpdate = false;
    long newRunTime = BrewsterUtils::convertSeconds(Time.now()-startTime, TimeUOM::minute);
    if (runTime != newRunTime) {
        runTime = newRunTime;
        timeUpdate = true;
    }

    //If time update is needed
    if(timeUpdate) {
      if(boilProcess->isBoilingPointReached())
        tTimeElapsed.setText(String::format("%u min", runTime));
      else
        tTimeElapsed.setText("Warming up");

      if(runTime == 0) {
        pbProgress.setValue(0);
        tTimeRemaining.setText(String::format("%i min", totalTime));
      }else{
        pbProgress.setValue(totalTime/runTime);
        tTimeRemaining.setText(String::format("%i min", (int)totalTime-runTime));
      }
    }
  }
}

void BoilWindowController::setStartTime() {
  ProcessState state = boilProcess->getState();

  if(state == ProcessState::STOPPED) {
    runTime = 9999;
    totalTime = 0;
    startTime = 0;
  }else{
    startTime = boilProcess->getStartTime();
    totalTime = recipe->getBoilingTime();
  }

}

void BoilWindowController::updateButtonText() {
  ProcessState state = boilProcess->getState();

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

void BoilWindowController::bTriggerProcessCB(void *ptr)
{
    //logger->info("Search sensors button pressed");
  UIEvent *obj = (UIEvent *) ptr;
  BoilWindowController *bwc = (BoilWindowController *) obj->getWindowController();
  NexButton *button = (NexButton *) obj->getButton();
  char* buf = (char*)malloc(20);
  button->getText(buf, 20);
  String buttonText = String(buf);

  bwc->logger->info("Trigger process button pressed[action: %s].", (const char*) buttonText);

  if(bwc->bStartText.equals(buttonText)) {
    bwc->boilProcess->start();
  }else if (bwc->bStopText.equals(buttonText)) {
    bwc->boilProcess->stop();
  }else if(bwc->bPauseText.equals(buttonText)) {
    bwc->boilProcess->pause();
  }else if (bwc->bResumeText.equals(buttonText)) {
    bwc->boilProcess->resume();
  }else{
    bwc->logger->error("Start/Stop button state unknow. Not processing button press.");
  }
}

void BoilWindowController::processInfoChangeHandler(void* callingObject, void* process) {
  BoilWindowController* bwc = (BoilWindowController*) callingObject;
  bwc->setStartTime();
  bwc->updateAdditions();
}

void BoilWindowController::processStateChangeHandler(void* callingObject, ProcessStateChangeEvent event) {
  BoilWindowController* bwc = (BoilWindowController*) callingObject;

  bwc->updateButtonText();

  if(event.newState == ProcessState::STOPPED) {
    //bwc->tTemp.setText("");
    //tTimeElapsed.setText("");
    bwc->tTimeRemaining.setText("Process stopped");
    for(int i=0; i<6; i++)
      bwc->tAdd[0].setText("");
  }
}
