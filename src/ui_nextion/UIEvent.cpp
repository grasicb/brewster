#include "UIEvent.h"

UIEvent::UIEvent(void *windowController, NexButton *button) {
  this->windowController=windowController;
  this->button=button;
}

void *UIEvent::getWindowController() {
  return windowController;
}

NexButton *UIEvent::getButton() {
  return button;
}
