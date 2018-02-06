#ifndef LCD_CONTROLLER_H
#define LCD_CONTROLLER_H

#define DEBUG_SERIAL_ENABLE

#include "application.h"
#include "../lib/lcd_nextion/ITEADLIB_Nextion.h"

#include "../ui_nextion/AWindowController.h"

class LcdController {
public:
    static LcdController *get();
    ~LcdController() {};
    void processMessages();
    void setCurrentWindowController(AWindowController *wc);
    void setCurrentWindow(NexPage *w);

private:
    LcdController();
    Logger *logger;
    static LcdController *instance;

    NexTouch **nex_listen_list;
    AWindowController *currentWindowController;
    NexPage *currentWindow;
};


#endif //LCD_CONTROLLER_H
