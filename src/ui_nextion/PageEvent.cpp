#include "PageEvent.h"

PageEvent::PageEvent(void *sourceEntity, NexPage *page, AWindowController *windowController) {
  this->sourceEntity=sourceEntity;
  this->page=page;
  this->windowController = windowController;
}

void *PageEvent::getSourceEntity() {
  return sourceEntity;
}

NexPage *PageEvent::getPage() {
  return page;
}

AWindowController *PageEvent::getWindowController() {
  return windowController;
}
