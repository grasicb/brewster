#pragma once

#include "application.h"
#include "../lib/lcd_nextion/ITEADLIB_Nextion.h"

#include "../ui_nextion/AWindowController.h"
#include "../ui_nextion/CommonWindowController.h"
#include "../ui_nextion/SensorSearchController.h"
#include "../ui_nextion/SensorTestController.h"
#include "../ui_nextion/OutputTestController.h"
#include "../ui_nextion/MashingController.h"

class LcdControllerNex {
public:
//    static LcdControllerNex *get();
    LcdControllerNex();
    ~LcdControllerNex() {};
    void processMessages();
    void setCurrentWindowController(AWindowController *wc);
    AWindowController *getCurrentWindowController();
    void setCurrentWindow(NexPage *w);
    void updateListenerList();
    void showMainPage();

private:

    Logger *logger;
    static LcdControllerNex *instance;

    NexTouch **nex_listen_list;
    NexTouch **page_list;
    AWindowController *currentWindowController;
    NexPage *currentWindow;

    CommonWindowController *commonWC;
    SensorSearchController *sensorSearchWC;
    SensorTestController *sensorTestWC;
    OutputTestController *outputTestWC;
    MashingController *mashingWC;

    NexPage loadingPage = NexPage(0, 0, "loading_page");
    NexPage mainPage = NexPage(1, 0, "main_page");
    NexPage settingsPage = NexPage(2, 0, "settings_page");
    NexPage settingsSensorSearch = NexPage(3, 0, "set_sensor_search");
    NexPage settingsSensorsTest = NexPage(4, 0, "set_sensors_test");
    NexPage settingsOutputTest = NexPage(5, 0, "set_output_test");
    NexPage mashingPage = NexPage(6, 0, "mashing");

    static void windowOpenCallback(void *ptr);
};
