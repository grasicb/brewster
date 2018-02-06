#ifndef A_WINDOW_CONTROLLER_H
#define A_WINDOW_CONTROLLER_H

#include "application.h"
#include "../lib/lcd_nextion/ITEADLIB_Nextion.h"


class AWindowController {

public:
  AWindowController() { };

  void initializeScreen(void *ptr);
  virtual void process();
  NexTouch **getListenerList() {return listenerList;};

protected:
  NexTouch *listenerList[];

private:

};

#endif // A_WINDOW_CONTROLLER_H
