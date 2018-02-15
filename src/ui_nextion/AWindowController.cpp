#include "AWindowController.h"
#include "../controller/LcdController.h"

void AWindowController::initializeScreen(void *ptr) {
  NexPage *window = (NexPage *) ptr;
  Log.info("Openning Window: %s", window->getObjName());

  //Deactivate previous screen
  AWindowController *previousController = LcdController::get()->getCurrentWindowController();
  if (previousController != NULL) {
    previousController->deactivateScreen();
  }

  //Activate new screen
  LcdController::get()->setCurrentWindowController(this);
  LcdController::get()->setCurrentWindow(window);
  LcdController::get()->updateListenerList();
}
