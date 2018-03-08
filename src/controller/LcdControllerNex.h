#pragma once

#include "application.h"
#include "../lib/lcd_nextion/ITEADLIB_Nextion.h"

#include "../ui_nextion/AWindowController.h"
#include "../ui_nextion/CommonWindowController.h"
#include "../ui_nextion/SensorSearchController.h"
#include "../ui_nextion/SensorTestController.h"
#include "../ui_nextion/OutputTestController.h"
#include "../ui_nextion/MashingWC.h"
#include "../ui_nextion/MashStepChange.h"
#include "../ui_nextion/BoilingWC.h"

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
    MashingWC *mashingWC;
    MashStepChange *mashStepChangeWC;
    BoilingWC *boilingWC;

    NexPage loadingPage = NexPage(0, 0, "loading_page");
    NexPage mainPage = NexPage(1, 0, "main_page");
    NexPage settingsPage = NexPage(2, 0, "settings_page");
    NexPage settingsSensorSearch = NexPage(3, 0, "set_sensor_search");
    NexPage settingsSensorsTest = NexPage(4, 0, "set_sensors_test");
    NexPage settingsOutputTest = NexPage(5, 0, "set_output_test");
    NexPage mashingPage = NexPage(6, 0, "mashing");
    NexPage boilPage = NexPage(7, 0, "boiling");
    NexPage mashStepChangePage = NexPage(10, 0, "mashing_change");

    static void windowOpenCallback(void *ptr);
};
