#ifndef LCD_CONTROLLER_H
#define LCD_CONTROLLER_H

#include "application.h"


class LcdController {
public:
    static LcdController *get();
    ~LcdController() {};
    void processMessages();
    void updateTemperature();

private:
    LcdController();
    Logger *logger;
    static LcdController *instance;

    static const size_t READ_BUF_SIZE = 64;
    char readBuf[READ_BUF_SIZE];
    String buffer;
    size_t readBufOffset = 0;
};


#endif //LCD_CONTROLLER_H
