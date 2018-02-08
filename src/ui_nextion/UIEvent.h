#pragma once

#include "application.h"
#include "../lib/lcd_nextion/ITEADLIB_Nextion.h"


class UIEvent {

public:
  UIEvent(void *windowController, NexButton *button);
  void *getWindowController();
  NexButton *getButton();

protected:

private:
  void *windowController;
  NexButton *button;

};
