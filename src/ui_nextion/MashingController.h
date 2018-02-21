#pragma once

#include "AWindowController.h"
#include "../controller/TemperatureSensor.h"
#include <vector>
#include <map>

class MashingController : public AWindowController {

public:
  MashingController();
  void initializeScreen(void *ptr);
  void deactivateScreen();
  void process();

protected:


private:
  Logger *logger;
  NexNumber nTempMT = NexNumber(6, 3, "n_mt");
  NexNumber nTempHLT = NexNumber(6, 4, "n_hlt");
  NexNumber nTempCOut = NexNumber(6, 5, "n_c_out");

  NexText tCurrStepTime = NexText(6, 7, "t_timelapse");
  NexNumber ncurrStepTemp = NexNumber(6, 13, "n_target");
  NexText tNextStepTime = NexText(6, 15, "t_ns_time");
  NexNumber nNextStepTemp = NexNumber(6, 14, "n_ns_temp");

  NexProgressBar pbProgress = NexProgressBar(6, 6, "pb_progress");

  NexDSButton bPump = NexDSButton(5, 18, "bt_pump");
  NexButton bStartStop = NexButton(6, 16, "b0");

  std::vector<float> temperature;
  std::map<SensorLocation, TemperatureSensor>* sensors;
  uint8_t sensorNo;

  void updateOutputText();
  static void triggerPumpButtonAH(void *ptr);
  static void triggerStartStopAH(void *ptr);
};
