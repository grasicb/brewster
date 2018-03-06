#pragma once

#include "AWindowController.h"

class BoilWindowController : public AWindowController {

public:
  BoilWindowController();
  void initializeScreen(void *ptr);
  void deactivateScreen();
  void process();

protected:


private:
  Logger *logger;
  NexText outputText = NexText(3, 1, "t0");
  NexButton bSearch = NexButton(3, 4, "b1");

  void updateOutputText();
  static void bSearchCallback(void *ptr);
};
