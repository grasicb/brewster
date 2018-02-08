#pragma once

#include "application.h"
#include "../lib/lcd_nextion/ITEADLIB_Nextion.h"
#include "AWindowController.h"


class PageEvent {

public:
  PageEvent(void *sourceEntity, NexPage *page, AWindowController *windowController);
  void *getSourceEntity();
  NexPage *getPage();
  AWindowController *getWindowController();

protected:

private:
  void *sourceEntity;
  NexPage *page;
  AWindowController *windowController;
};
