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
  wc->logger->trace("PID calibration button pressed");

  //Business logic

  //Init variables
  kp=2;
  ki=0.5;
  kd=2;
  pid = new PID(&input, &output, &setpoint,kp,ki,kd, DIRECT);
  aTune = new PID_ATune(&input, &output);

  pid.SetMode(AUTOMATIC);
  

}
