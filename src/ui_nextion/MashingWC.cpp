#include "MashingWC.h"
#include "UIEvent.h"
#include "../controller/BrewsterController.h"
#include "../controller/SensorManager.h"
#include "../controller/TemperatureSensor.h"

MashingWC::MashingWC() {
  logger = new Logger("MashingWC");
  sensors = &(BrewsterController::get()->getSensorManager()->getAllTemperatureSensors());

  temperature[SensorLocation::HLT] = 0;
  temperature[SensorLocation::MT] = 0;
  temperature[SensorLocation::COOLER_OUT] = 0;

  bPump.attachPop(triggerPumpButtonAH, new UIEvent(this, &bPump));
  bStartStop.attachPop(triggerStartStopAH, new UIEvent(this, &bStartStop));

  int j=0;
  listenerList = new NexTouch*[3];
  listenerList[j++] = &bPump;
  listenerList[j++] = &bStartStop;
  listenerList[j++] = NULL;

  //TO BE REMOVED
  //BrewsterController::get()->getProcessManager()->getProcess(BrewProcess::MASHING)->stop();
}


void MashingWC::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);
  logger->info("Initializing mashing screen");

  //Setting new recipe if the process is not running, otherwise retrieve recipe from running process
  mashProcess = BrewsterController::get()->getProcessManager()->getProcess(BrewProcess::MASHING);
  if (mashProcess->isActive() && mashProcess->getRecipe() != NULL)
    recipe = BrewsterController::get()->getRecipe();
  else {
    recipe = BrewsterController::get()->getRecipe();
    mashProcess->setRecipe(recipe);
  }

  //Adding listener for change in process & output state
  mashProcess->addListener(processInfoChangeHandler, this);
  mashProcess->addListener(processStateChangeHandler, this);
  BrewsterController::get()->getOutput(mashPump)->addListener(pumpStateChanged, this, mashPump);

  //Update recipe information (current & next steps)
  getRecipeInformation();
  updateRecipeValues();

  if(mashProcess->isActive())
    bStartStop.setText(bStopText);
  else
    bStartStop.setText(bStartText);

  //Set pump status
  if(BrewsterController::get()->isOutputActive(mashPump))
    bPump.setValue(1);
  else
    bPump.setValue(0);

  //Init relevant temp sensors
  temperature[SensorLocation::HLT] = (*sensors)[SensorLocation::HLT].getValue();
  temperature[SensorLocation::MT] = (*sensors)[SensorLocation::MT].getValue();
  temperature[SensorLocation::COOLER_OUT] = (*sensors)[SensorLocation::COOLER_OUT].getValue();


  //Update screen
  updateLcdTemp();
  updateLcdProcessInfo();
}

void MashingWC::deactivateScreen() {
  mashProcess->removeListener(processInfoChangeHandler);
  mashProcess->removeListener(processStateChangeHandler);
  BrewsterController::get()->getOutput(mashPump)->removeListener(pumpStateChanged);
}


void MashingWC::process() {
  boolean tempUpdate = false;

  //Check if temperature update is needed
  for ( auto &p : (temperature) ) {
    if(p.second != (*sensors)[p.first].getValue()) {
      //p.second = (*sensors)[p.first].getValue();
      temperature[p.first] = (*sensors)[p.first].getValue();
      tempUpdate = true;
    }
  }

  if (tempUpdate)
    updateLcdTemp();

  //Check if process progress update is needed
  unsigned long _runTime = runTime;
  int _remainingTime = remainingTime;
  uint8_t _progress = progress;
  updateRecipeValues();

  if(_runTime != runTime
    || _remainingTime != remainingTime
    || _progress != progress)
    updateLcdProcessInfo();
}

void MashingWC::updateLcdTemp() {
  nTempMT.setValue(temperature[SensorLocation::MT]);
  nTempHLT.setValue(temperature[SensorLocation::HLT]);
  nTempCOut.setValue(temperature[SensorLocation::COOLER_OUT]);
}

void MashingWC::updateLcdProcessInfo() {
  //Update runtime
  tCurrStepTime.setText(String::format("%i min", runTime));

  //Update time to next step
  if(remainingTime > 0)
    tNextStepTime.setText(String::format("%i min", remainingTime));
  else if (remainingTime == 0)
    tNextStepTime.setText(String::format("<1 min"));
  else
    tNextStepTime.setText(String::format(""));


  //Update current step temp
  if(currentStep != NULL) {
    ncurrStepTemp.setValue(currentStep->temperature);
  }else{
    ncurrStepTemp.setValue(0);
  }

  //Update next step temp
  if(nextStep != NULL) {
    nNextStepTemp.setValue(nextStep->temperature);
  }else{
    nNextStepTemp.setValue(0);
  }

  //Update progress bar
  pbProgress.setValue(progress);
}

void MashingWC::getRecipeInformation() {
  if(mashProcess->isActive())
    processStartTime = mashProcess->getStartTime();
  else
    processStartTime = 0;
  currentStep = recipe->getCurrentMashingStep(processStartTime);
  nextStep = recipe->getNextMashingStep(processStartTime);

  if(currentStep != NULL) {
    currentStepStartTime = processStartTime + recipe->getMashingStepStartTime(currentStep);
    nextStepStartTime = currentStepStartTime + BrewsterUtils::getSeconds(currentStep->time, currentStep->timeUOM);
  }else{
    currentStepStartTime = 0;
    nextStepStartTime = 0;
  }
}

void MashingWC::updateRecipeValues() {

  //If the process is running then calculates the values
  if(mashProcess->isActive()) {
    runTime = BrewsterUtils::convertSeconds(Time.now()-processStartTime, TimeUOM::minute);

    if(nextStepStartTime >0 && nextStepStartTime>Time.now())
      remainingTime = BrewsterUtils::convertSeconds(nextStepStartTime - Time.now(), TimeUOM::minute)+1;
    else
      remainingTime = -1;

    if(runTime > recipe->getMashingTime()/60)
      progress = 100;
    else
      progress = (uint8_t)((Time.now()-processStartTime)*100/recipe->getMashingTime());
    //logger->info("Progress: %i [now:%u, processStartTime:%u, mashingTime:%u, runTime:%u]", progress, Time.now(), processStartTime, recipe->getMashingTime(), Time.now()-processStartTime);

  //If the process is not running, then is sets dummy values
  } else {
    runTime = 0;
    remainingTime = BrewsterUtils::convertSeconds(nextStepStartTime - currentStepStartTime, TimeUOM::minute);
    progress = 0;
  }
}

////////////////////////////////////
// Event Handlers

void MashingWC::triggerPumpButtonAH(void *ptr) {
  UIEvent *obj = (UIEvent *) ptr;
  NexDSButton *button = (NexDSButton *)obj->getButton();
  MashingWC *w = (MashingWC *) obj->getWindowController();
  w->logger->trace("Pump button pressed");

  uint32_t value;
  button->getValue(&value);

  if(value==0)
    BrewsterController::get()->getOutput(mashPump)->setOutput(0);
  else
    BrewsterController::get()->getOutput(mashPump)->setOutput(mashPumpFlowRate);
}

void MashingWC::triggerStartStopAH(void *ptr) {
  UIEvent *obj = (UIEvent *) ptr;
  NexButton *button = (NexButton *)obj->getButton();
  MashingWC *w = (MashingWC *) obj->getWindowController();

  w->logger->trace("Start/Stop pressed");

  char* buf = (char*)malloc(10);
  button->getText(buf, 10);
  String bt = String(buf);

  if(w->bStartText.equals(bt)) {
    button->setText(w->bStopText);
    w->mashProcess->start();
  }else if (w->bStopText.equals(bt)) {
    button->setText(w->bStartText);
    w->mashProcess->stop();
  }else{
    w->logger->error("Start/Stop button state unknow. Not processing button press.");
  }

  w->getRecipeInformation();
  w->updateLcdProcessInfo();
}

void MashingWC::processInfoChangeHandler(void* callingObject, void* process) {
  MashingWC* mc = (MashingWC*) callingObject;
  mc->getRecipeInformation();
  mc->updateLcdProcessInfo();
}

void MashingWC::processStateChangeHandler(void* callingObject, ProcessStateChangeEvent event) {
  MashingWC* mc = (MashingWC*) callingObject;

  if(event.newState == ProcessState::STOPPED) {
    mc->bStartStop.setText(mc->bStartText);
  }else if (event.newState == ProcessState::STARTED) {
    mc->bStartStop.setText(mc->bStopText);
  }else{
    mc->logger->error("Start/Stop button state unknow. Not processing button press.");
  }
}

void MashingWC::pumpStateChanged(void* callingObject, int outputIdentifier, Output::OutputChangeEvent event) {
  MashingWC *w = (MashingWC *) callingObject;

  w->logger->info("Output event change received for output %i [ON=%i, AUTO=%i, VALUE=%.1f]", outputIdentifier, (int)event.isActive, (int)event.isPID, event.targetValue);
//  Particle.process();
  uint32_t value;
  w->bPump.getValue(&value);

  if(event.isActive && value == 0)
    w->bPump.setValue(1);
  else if (!event.isActive && value == 1)
    w->bPump.setValue(0);
}
