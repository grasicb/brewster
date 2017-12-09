#pragma once

#include "../lib/ui/AWindow.h"
#include "WindowManager.h"
#include "../controller/BrewsterController.h"

class EnterValueWindow : public AWindow {

public:
  enum IncrementMode {PRECISE, COARSE};
  EnterValueWindow(Adafruit_ILI9341 *tft, AWindowManager *windowManager) : AWindow(tft, windowManager) { controller = BrewsterController::get(); };
  ~EnterValueWindow() {};
  void initScreen();
  void refreshScreen();
  void process(void* param);
  void processAction(uint8_t action);
  enum Action {A_OK, A_CANCEL};
  void setLimits(double lowerLimit, double upperLimit);
  void setOutputValue(int *value);
  void setOutputValue(float *value);
  void setOutputValue(double *value);
  void setIncrementMode(IncrementMode mode);

protected:


private:

  uint16_t refreshRate = 50;
  uint16_t minorIncrease = 200;
  uint16_t majorIncrease = 3000;
  float minorIncrement = 0.5;
  float majorIncrement = 1;
  unsigned long lastRefresh = 0;
  unsigned long increaseStarted = 0;
  unsigned long lastIncrease = 0;
  double value = 50;

  double lowerLimit = 0;
  double upperLimit = 100;

  int *refIntValue = NULL;
  float *refFloatValue = NULL;
  double *refDoubleValue = NULL;

  BrewsterController *controller;
};
