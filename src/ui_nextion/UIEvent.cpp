#include "UIEvent.h"

UIEvent::UIEvent(void *windowController, NexObject *button) {
  this->windowController=windowController;
  this->button=button;
}

void *UIEvent::getWindowController() {
  return windowController;
}

NexObject *UIEvent::getButton() {
  return button;
}
