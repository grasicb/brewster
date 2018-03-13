#include "PIDCalibrationWC.h"

#include "../controller/BrewsterController.h"
#include "../util/BrewsterUtils.h"
#include "UIEvent.h"

#include "../lib/pid/PID.h"
#include "../lib/pid_autotune/PID_ATune.h"


PIDCalibrationWC::PIDCalibrationWC() {
  logger = new Logger("PIDCalibrationWC");

  bHLT.attachPop(bTriggerCalibrationButtonCB, new UIEvent(this, &bHLT));
  bBK.attachPop(bTriggerCalibrationButtonCB, new UIEvent(this, &bBK));
  bMT.attachPop(bTriggerCalibrationButtonCB, new UIEvent(this, &bMT));
  bCooling.attachPop(bTriggerCalibrationButtonCB, new UIEvent(this, &bCooling));
  bFermentation.attachPop(bTriggerCalibrationButtonCB, new UIEvent(this, &bFermentation));


  listenerList = new NexTouch*[6];
  listenerList[0] = &bHLT;
  listenerList[1] = &bBK;
  listenerList[2] = &bMT;
  listenerList[3] = &bCooling;
  listenerList[4] = &bFermentation;
  listenerList[5] = NULL;
}


void PIDCalibrationWC::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  logger->info("Initializing PID calibration");
  output.clear();
  addOutput("Ready for calibration");
}

void PIDCalibrationWC::deactivateScreen() {
}


void PIDCalibrationWC::process() {

}

void PIDCalibrationWC::addOutput(String text) {
  while(output.size() >=3)
    output.pop_back();

  output.insert(output.begin(), text);
  refreshOutput();
}

void PIDCalibrationWC::refreshOutput() {
  String txt = "";
  for(String s:output) {
    txt.concat(String::format("%s%c%c", (const char*) s, 13, 10));
  }
  tOutput.setText(txt);
}

////////////////////////////////////
// Event Handlers

void PIDCalibrationWC::bTriggerCalibrationButtonCB(void *ptr) {
  UIEvent *obj = (UIEvent *) ptr;
  NexButton *button = (NexButton *)obj->getButton();
  PIDCalibrationWC *wc = (PIDCalibrationWC *) obj->getWindowController();
  wc->logger->info("PID calibration button pressed");

  //Business logic
  Output *output = NULL;
  float *input = NULL;
  double target;
  int direction = DIRECT;
  PidSettings *pidSettings = NULL;
  BrewProcess bp;

  //Setting params based on button press
  if(button == &wc->bHLT) {
    bp = BrewProcess::WATER_PREP;
    output = BrewsterController::get()->getOutput(mashHeater);
    input = BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::HLT).getValueReference();
    pidSettings = &(BrewsterGlobals::get()->getPIDSettings()[bp]);
    target = (*input) + 10;
  }else if(button == &wc->bBK) {
    bp = BrewProcess::BOILING;
    output = BrewsterController::get()->getOutput(boilHeater);
    input = BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::BK).getValueReference();
    pidSettings = &(BrewsterGlobals::get()->getPIDSettings()[bp]);
    target = (*input) + 10;
  }else if(button == &wc->bMT) {
    bp = BrewProcess::MASHING;
    output = BrewsterController::get()->getOutput(mashHeater);
    //input = &(BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::MT))->getValueReference();
    input = BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::COOLER_OUT).getValueReference();
    pidSettings = &(BrewsterGlobals::get()->getPIDSettings()[bp]);
    target = (*input) + 10;
  }else if(button == &wc->bCooling) {
    bp = BrewProcess::COOLING;
    output = BrewsterController::get()->getOutput(coolingPump);
    input = BrewsterController::get()->getSensorManager()->getTemperatureSensor(SensorLocation::COOLER_OUT).getValueReference();
    pidSettings = &(BrewsterGlobals::get()->getPIDSettings()[bp]);
    target = (*input) - 5;
    direction = REVERSE;
  }else if(button == &wc->bFermentation) {
    wc->logger->warn("Autotune for fermentation is not available. Fermentation process is not implemented yet.");
  }

  //Starting autotune process
  output->addListener(outputChangedCB, wc, bp);
  if(output->isActive() && !output->isAutoTune()) {
    wc->logger->info("Output %s is active. Stopping the output before starting autotune.", (const char*)output->getName());
    output->setOutput(0);
  }
  output->setTargetValue(target, input, direction, pidSettings);
  PidSettings* ps = output->getPIDSettings();
  wc->addOutput(String::format("Autotune started [output: %s, p=%.1f, i=%.1f, d=%.1f].", (const char*)output->getName(), ps->kp, ps->ki, ps->kd));
}

void PIDCalibrationWC::outputChangedCB(void* callingObject, int outputIdentifier, Output::OutputChangeEvent event) {
  PIDCalibrationWC *wc = (PIDCalibrationWC *) callingObject;
  wc->logger->info("Output status changed [output: %s]", (const char*)event.output->getName());
  BrewProcess bp = (BrewProcess) outputIdentifier;

  PidSettings* ps = event.output->getPIDSettings();
  if(!event.output->isAutoTune()) {
    wc->addOutput(String::format("Autotune complete [output: %s, p=%.1f, i=%.1f, d=%.1f].", (const char*)event.output->getName(), ps->kp, ps->ki, ps->kd));
    event.output->removeListener(outputChangedCB);
    //Storing PID values
    PidSettings ps1 = BrewsterGlobals::get()->getPIDSettings()[bp];
    ps1.kp = ps->kp;
    ps1.ki = ps->ki;
    ps1.kd = ps->kd;
    BrewsterGlobals::get()->storePIDSettings();
    
  }else {
    wc->addOutput(String::format("  output status changed [output: %s, p=%.1f, i=%.1f, d=%.1f].", (const char*)event.output->getName(), ps->kp, ps->ki, ps->kd));
  }
}
