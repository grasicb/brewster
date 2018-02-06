#include "CommonWindowController.h"
//#include "../controller/LcdController.h"

CommonWindowController::CommonWindowController() {
  logger = new Logger("CommonWindowController");
}


void CommonWindowController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);
}


void CommonWindowController::process() {

}
