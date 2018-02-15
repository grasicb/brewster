#pragma once

#include "application.h"
#include "../lib/lcd_nextion/ITEADLIB_Nextion.h"


class AWindowController {

public:
  AWindowController() { };

  void initializeScreen(void *ptr);
  virtual void deactivateScreen();
  virtual void process();
  NexTouch **getListenerList() {return listenerList;};

protected:
  NexTouch **listenerList;

private:

};
