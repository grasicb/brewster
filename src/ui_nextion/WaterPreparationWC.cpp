#include "WaterPreparationWC.h"
#include "../controller/BrewsterController.h"
#include "../controller/TemperatureSensor.h"
#include "../controller/Speaker.h"
#include "UIEvent.h"
#include <map>

#include "../util/BrewsterUtils.h"
#include "../util/TempUtils.h"

#include "../model/Recipe.h"

WaterPreparationWC::WaterPreparationWC() {
  logger = new Logger("WaterPreparationWC");

  tempSensorHLT = &(BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::HLT));
  tempSensorBK = &(BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::BK));

  outputHLT = BrewsterController::get()->getOutput(mashHeater);
  outputBK = BrewsterController::get()->getOutput(boilHeater);

  bHLTPower.attachPop(bTriggerPowerCB, new UIEvent(this, &bHLTPower));
  bBKPower.attachPop(bTriggerPowerCB, new UIEvent(this, &bBKPower));
  bHLTSettings.attachPop(bTriggerSettingsCB, new UIEvent(this, &bHLTSettings));
  bBKSettings.attachPop(bTriggerSettingsCB, new UIEvent(this, &bBKSettings));

  listenerList = new NexTouch*[3];
  listenerList[0] = &bHLTPower;
  listenerList[1] = &bBKPower;
  listenerList[2] = &bHLTSettings;
  listenerList[3] = &bBKSettings;
  listenerList[4] = NULL;
}


void WaterPreparationWC::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  logger->info("Initializing water preparation screen");

  lastTempHLT = tempSensorHLT->getValue();
  lastTempBK = tempSensorBK->getValue();
  tTempHLT.setText(String::format("%.1f", lastTempHLT));
  tTempBK.setText(String::format("%.1f", lastTempBK));

  refreshOutputStatus();

  notificationHLT = true;
  notificationBK = true;
  targetTempHLT = 999;
  targetTempBK = 999;

  outputHLT->addListener(outputChangedEvent, this, mashHeater);
  outputBK->addListener(outputChangedEvent, this, boilHeater);
}

void WaterPreparationWC::deactivateScreen() {
  outputHLT->removeListener(outputChangedEvent);
  outputBK->removeListener(outputChangedEvent);
}

void WaterPreparationWC::refreshOutputStatus() {
  //Setting status of HLT output
  if (outputHLT->isPID() || !outputHLT->isActive()) {
    bHLTPower.setValue((uint32_t)outputHLT->isActive());
    nTargetHLT.setValue((uint32_t)outputHLT->getTargetValue());

    NexSendCommand("vis bt1,1");
    NexSendCommand("vis bt3,1");
  }else {
    logger->warn("HLT outout is not in PID setting. Disabling control of this output from the current window.");
    nTargetHLT.setValue((uint32_t)outputHLT->getOutput());
    NexSendCommand("vis bt1,0");
    NexSendCommand("vis bt3,0");
  }

  //Setting status of BK output
  if (outputHLT->isPID() || !outputHLT->isActive()) {
    bBKPower.setValue((uint32_t)outputBK->isActive());
    nTargetBK.setValue((uint32_t)outputHLT->getTargetValue());

    NexSendCommand("vis bt2,1");
    NexSendCommand("vis bt4,1");
  }else {
    logger->warn("HLT outout is not in PID setting. Disabling control of this output from the current window.");
    nTargetBK.setValue((uint32_t)outputBK->getOutput());
    NexSendCommand("vis bt2,0");
    NexSendCommand("vis bt4,0");
  }
}


void WaterPreparationWC::process() {

  //Update temperature display
  if(lastTempHLT != tempSensorHLT->getValue()) {
    lastTempHLT = tempSensorHLT->getValue();
    tTempHLT.setText(String::format("%.1f", lastTempHLT));
  }

  if(lastTempBK != tempSensorBK->getValue()) {
    lastTempBK = tempSensorBK->getValue();
    tTempBK.setText(String::format("%.1f", lastTempBK));
  }

  //Notification when target temperature is reached
  if(!notificationHLT && targetTempHLT < lastTempHLT) {
    Speaker::playBeep();
    notificationHLT = true;
  }

  if(!notificationBK && targetTempBK < lastTempBK) {
    Speaker::playBeep();
    notificationBK = true;
  }
}

void WaterPreparationWC::bTriggerPowerCB(void *ptr)
{
    //logger->info("Search sensors button pressed");
  UIEvent *obj = (UIEvent *) ptr;
  WaterPreparationWC *wc = (WaterPreparationWC *) obj->getWindowController();
  NexDSButton *button = (NexDSButton *) obj->getButton();

  Output *output;
  uint32_t targetValue;
  //uint32_t switchState = button->getValue(&switchState);
  uint32_t switchState=0;
  float *temperatureReference;

  //Power button triggered for HLT
  if(button == &wc->bHLTPower) {
    temperatureReference = wc->tempSensorHLT->getValueReference();
    wc->nTargetHLT.getValue(&targetValue);
    wc->bHLTPower.getValue(&switchState);
    output = wc->outputHLT;

    if(switchState>0) {
      wc->targetTempHLT = (float)targetValue;
      wc->notificationHLT = false;
    }

    wc->logger->trace("Trigger power button for HLT [swtich=%i]", switchState);
  }

  //Power button triggered for BK
  if(button == &wc->bBKPower) {
    temperatureReference = wc->tempSensorBK->getValueReference();
    wc->nTargetBK.getValue(&targetValue);
    wc->bBKPower.getValue(&switchState);
    output = wc->outputBK;

    if(switchState>0) {
      wc->targetTempBK = (float)targetValue;
      wc->notificationBK = false;
    }

    wc->logger->trace("Trigger power button for BK [swtich=%i]", switchState);
  }

  //Set the output
  if(output!=NULL) {
    if(switchState == 0) {
      output->setOutput(0);
    }else {
      output->setTargetValue(targetValue, temperatureReference, &(BrewsterGlobals::get()->getPIDSettings()[BrewProcess::WATER_PREP]));
    }
  }
}

void WaterPreparationWC::bTriggerSettingsCB(void *ptr)
{
    //logger->info("Search sensors button pressed");
  UIEvent *obj = (UIEvent *) ptr;
  WaterPreparationWC *wc = (WaterPreparationWC *) obj->getWindowController();
  NexDSButton *button = (NexDSButton *) obj->getButton();

  wc->logger->trace("Trigger settings button.");

  Output *output;
  uint32_t targetValue;
  uint32_t switchState;


  //Power button triggered for HLT
  if(button == &wc->bHLTSettings) {
    wc->nTargetHLT.getValue(&targetValue);
    switchState = wc->bHLTPower.getValue(&switchState);
    output = wc->outputHLT;

    if(switchState>0) {
      wc->targetTempHLT = (float)targetValue;
      wc->notificationHLT = false;
    }
  }

  //Power button triggered for BK
  if(button == &wc->bBKSettings) {
    wc->nTargetBK.getValue(&targetValue);
    switchState = wc->bBKPower.getValue(&switchState);
    output = wc->outputBK;

    if(switchState>0) {
      wc->targetTempBK = (float)targetValue;
      wc->notificationBK = false;
    }
  }

  //Set the output
  if(output!=NULL && switchState==1) {
    output->changeTargetValue(targetValue);
  }
}

/////////////////////////////////
// Output Callback functions
/////////////////////////////////
void WaterPreparationWC::outputChangedEvent(void* callingObject, int outputIdentifier, Output::OutputChangeEvent event) {
  WaterPreparationWC *wc = (WaterPreparationWC *) callingObject;

  wc->logger->info("Output event change received for output %i [ON=%i, AUTO=%i, VALUE=%.1f]", outputIdentifier, (int)event.isActive, (int)event.isPID, event.targetValue);

  wc->refreshOutputStatus();
}
