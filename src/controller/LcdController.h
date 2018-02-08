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
    void updateListenerList();

private:
    LcdController();
    Logger *logger;
    static LcdController *instance;

    NexTouch **nex_listen_list;
    NexTouch **page_list;
    AWindowController *currentWindowController;
    NexPage *currentWindow;

    static void windowOpenCallback(void *ptr);
};


#endif //LCD_CONTROLLER_H
