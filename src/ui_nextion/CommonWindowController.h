#ifndef WINDOW_CONTROLLER_H
#define WINDOW_CONTROLLER_H


#include "AWindowController.h"

class CommonWindowController : public AWindowController {

public:
  CommonWindowController();
  void initializeScreen(void *ptr);
  void process();

protected:


private:
  Logger *logger;

};

#endif //WINDOW_CONTROLLER_H
