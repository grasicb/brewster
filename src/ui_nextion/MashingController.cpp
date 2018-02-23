#include "MashingController.h"
#include "UIEvent.h"
#include "../controller/BrewsterController.h"
#include "../controller/SensorManager.h"
#include "../controller/TemperatureSensor.h"

MashingController::MashingController() {
  logger = new Logger("MashingController");
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
  BrewsterController::get()->getProcessManager()->getProcess(BrewProcess::MASHING)->stop();
}


void MashingController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);
  logger->info("Initializing mashing screen");

  //Setting new recipe if the process is not running, otherwise retrieve recipe from running process
  mashProcess = BrewsterController::get()->getProcessManager()->getProcess(BrewProcess::MASHING);
  if (mashProcess->isActive())
    recipe = mashProcess->getRecipe();
  else {
    recipe = BrewsterController::get()->getRecipe();
    mashProcess->setRecipe(recipe);
  }

  //Adding listener for change in process & output state
  mashProcess->addListener(processInfoChangeHandler, this);
  BrewsterController::get()->getOutput(mashPump)->addListener(pumpStateChanged, this, mashPump);

  //Update recipe information (current & next steps)
  getRecipeInformation();

  if(mashProcess->isActive()) {
    runTime = BrewsterUtils::convertSeconds(Time.now()-processStartTime, TimeUOM::minute);
    remainingTime = BrewsterUtils::convertSeconds(nextStepStartTime - Time.now(), TimeUOM::minute);
    progress = (uint8_t)((Time.now()-processStartTime)/recipe->getMashingTime()*100);
    logger->info("Progress: %i", progress);
  } else {
    runTime = 0;
    remainingTime = BrewsterUtils::convertSeconds(nextStepStartTime - currentStepStartTime, TimeUOM::minute);
    progress = 85;
  }

  //Set pump status
  if(BrewsterController::get()->isOutputActive(mashPump))
    bPump.setValue(1);
  else
    bPump.setValue(0);

  if(mashProcess->isActive())
    bStartStop.setText(bStopText);
  else
    bStartStop.setText(bStartText);


  //Init relevant temp sensors
  temperature[SensorLocation::HLT] = (*sensors)[SensorLocation::HLT].getValue();
  temperature[SensorLocation::MT] = (*sensors)[SensorLocation::MT].getValue();
  temperature[SensorLocation::COOLER_OUT] = (*sensors)[SensorLocation::COOLER_OUT].getValue();

  //Update screen
  updateLcdTemp();
  updateLcdProcessInfo();
}

void MashingController::deactivateScreen() {
  mashProcess->removeListener(processInfoChangeHandler);
  BrewsterController::get()->getOutput(mashPump)->removeListener(pumpStateChanged);
}


void MashingController::process() {
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
}

void MashingController::updateLcdTemp() {
  nTempMT.setValue(temperature[SensorLocation::MT]);
  nTempHLT.setValue(temperature[SensorLocation::HLT]);
  nTempCOut.setValue(temperature[SensorLocation::COOLER_OUT]);
}

void MashingController::updateLcdProcessInfo() {
  tCurrStepTime.setText(String::format("%i min", runTime));
  tNextStepTime.setText(String::format("%i min", remainingTime));
  ncurrStepTemp.setValue(currentStep->temperature);
  nNextStepTemp.setValue(nextStep->temperature);
  pbProgress.setValue(progress);
}

void MashingController::getRecipeInformation() {
  if(mashProcess->isActive())
    processStartTime = mashProcess->getStartTime();
  else
    processStartTime = 0;
  currentStep = recipe->getCurrentMashingStep(processStartTime);
  nextStep = recipe->getNextMashingStep(processStartTime);
  currentStepStartTime = processStartTime + recipe->getMashingStepStartTime(currentStep);
  nextStepStartTime = currentStepStartTime + BrewsterUtils::getSeconds(currentStep->time, currentStep->timeUOM);
}

void MashingController::triggerPumpButtonAH(void *ptr) {
  UIEvent *obj = (UIEvent *) ptr;
  NexDSButton *button = (NexDSButton *)obj->getButton();
  MashingController *w = (MashingController *) obj->getWindowController();
  w->logger->trace("Pump button pressed");

  uint32_t value;
  button->getValue(&value);

  if(value==0)
    BrewsterController::get()->getOutput(mashPump)->setOutput(0);
  else
    BrewsterController::get()->getOutput(mashPump)->setOutput(mashPumpFlowRate);
}

void MashingController::triggerStartStopAH(void *ptr) {
  UIEvent *obj = (UIEvent *) ptr;
  NexButton *button = (NexButton *)obj->getButton();
  MashingController *w = (MashingController *) obj->getWindowController();

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
}

void MashingController::processInfoChangeHandler(void* callingObject, void* process) {
  MashingController* mc = (MashingController*) callingObject;
  mc->getRecipeInformation();
  mc->updateLcdProcessInfo();
}

void MashingController::pumpStateChanged(void* callingObject, int outputIdentifier, OutputChangeEvent event) {
  MashingController *w = (MashingController *) callingObject;

  w->logger->info("Output event change received for output %i [ON=%i, AUTO=%i, VALUE=%.1f]", outputIdentifier, (int)event.isActive, (int)event.isPID, event.targetValue);
  uint32_t value;
  w->bPump.getValue(&value);

  if(event.isActive && value == 0)
    w->bPump.setValue(1);
  else if (!event.isActive && value == 1) 
    w->bPump.setValue(0);
}
