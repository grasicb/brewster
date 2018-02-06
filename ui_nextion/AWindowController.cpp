#include "AWindowController.h"
#include "../controller/LcdController.h"

void AWindowController::initializeScreen(void *ptr) {
  NexPage *window = (NexPage *) ptr;
  Log.info("Openning Window: %s", window->getObjName());

  LcdController::get()->setCurrentWindowController(this);
  LcdController::get()->setCurrentWindow(window);
}
