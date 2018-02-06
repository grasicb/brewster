#include "LcdController.h"

#include "../ui_nextion/CommonWindowController.h"
#include "../ui_nextion/SensorSearchController.h"

LcdController* LcdController::instance = NULL;

CommonWindowController commonWC;
SensorSearchController sensorSearchWC;

NexPage mainPage = NexPage(1, 0, "main_page");
NexPage settingsPage = NexPage(2, 0, "settings_page");
NexPage settingsSensorSearch = NexPage(3, 0, "set_sensor_search");
NexPage settingsSensorsTest = NexPage(4, 0, "set_sensors_test");
NexPage settingsOutputTest = NexPage(4, 0, "set_output_test");


LcdController* LcdController::get() {
  if (instance == NULL) {
    instance = new LcdController();
  }
  return instance;
}

void windowOpenCallback(void *ptr)
{
  if(ptr == &settingsSensorSearch) {
    sensorSearchWC.initializeScreen(ptr);
  }else {
    commonWC.initializeScreen(ptr);
  }
}

LcdController::LcdController() {
    logger = new Logger("lcd_controller");
    logger->trace("Initializing serial connection for Nextion LCD");

    currentWindowController = NULL;
    currentWindow = NULL;

    nex_listen_list = new NexTouch*[6];//malloc(sizeof(&mainPage)*4);

    nex_listen_list[0] = &mainPage;
    nex_listen_list[1] = &settingsPage;
    nex_listen_list[2] = &settingsSensorSearch;
    nex_listen_list[3] = &settingsSensorsTest;
    nex_listen_list[4] = &settingsOutputTest;
    nex_listen_list[5] = NULL;

/*
    nex_listen_list =
    {
        &mainPage,
        &settingsPage,
        &settingsSensorSearchPage,
        NULL
    };
*/

    //*nex_listen_list = (*NexTouch[]) { &mainPage, NULL };

    nexInit();

    mainPage.attachPop(windowOpenCallback, &mainPage);
    settingsPage.attachPop(windowOpenCallback, &settingsPage);
    settingsSensorSearch.attachPop(windowOpenCallback, &settingsSensorSearch);
    settingsSensorsTest.attachPop(windowOpenCallback, &settingsSensorsTest);
    settingsOutputTest.attachPop(windowOpenCallback, &settingsOutputTest);

    mainPage.show();
}

void LcdController::setCurrentWindowController(AWindowController *wc) {
    currentWindowController = wc;
}

void LcdController::setCurrentWindow(NexPage *w) {
  currentWindow = w;
}

void LcdController::processMessages() {
  nexLoop(nex_listen_list);

  if (currentWindowController != NULL) {
    currentWindowController->process();
  }

  /*
  // Read data from serial
  	while(Serial1.available()) {

      //logger->info("LCD data available");

  		if (readBufOffset < READ_BUF_SIZE) {
        char c = Serial1.read();
        readBuf[readBufOffset++] = c;

        buffer.concat(String(c, HEX) + " ");
  		}
  		else {
  			Serial.println("readBuf overflow, emptying buffer");
  			readBufOffset = 0;
  		}
  	}

    if (readBufOffset>0) {
      readBuf[readBufOffset] = 0;
      //Serial.printlnf("Received from Nextion LCD: %s", buffer);
      logger->info(buffer);
      readBufOffset = 0;
      buffer = "";
    }
    */
}
