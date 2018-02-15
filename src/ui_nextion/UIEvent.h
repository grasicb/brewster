#pragma once

#include "application.h"
#include "../lib/lcd_nextion/ITEADLIB_Nextion.h"


class UIEvent {

public:
  UIEvent(void *windowController, NexObject *button);
  void *getWindowController();
  NexObject *getButton();

protected:

private:
  void *windowController;
  NexObject *button;
};
