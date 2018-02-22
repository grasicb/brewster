#include "LcdController.h"

#include "../ui_nextion/PageEvent.h"
#include "../ui_nextion/CommonWindowController.h"
#include "../ui_nextion/SensorSearchController.h"
#include "../ui_nextion/SensorTestController.h"
#include "../ui_nextion/OutputTestController.h"
#include "../ui_nextion/MashingController.h"

LcdController* LcdController::instance = NULL;

CommonWindowController commonWC;
SensorSearchController sensorSearchWC;
SensorTestController sensorTestWC;
OutputTestController outputTestWC;
MashingController mashingWC;

NexPage loadingPage = NexPage(0, 0, "loading_page");
NexPage mainPage = NexPage(1, 0, "main_page");
NexPage settingsPage = NexPage(2, 0, "settings_page");
NexPage settingsSensorSearch = NexPage(3, 0, "set_sensor_search");
NexPage settingsSensorsTest = NexPage(4, 0, "set_sensors_test");
NexPage settingsOutputTest = NexPage(5, 0, "set_output_test");
NexPage mashingPage = NexPage(6, 0, "mashing");


LcdController* LcdController::get() {
  if (instance == NULL) {
    instance = new LcdController();
  }
  return instance;
}

void LcdController::windowOpenCallback(void *ptr)
{
  PageEvent *event = (PageEvent *)ptr;
  LcdController *lcdController = (LcdController *)event->getSourceEntity();

  ///////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////

  Log.info("Openning Window: %s", event->getPage()->getObjName());

  //Deactivate previous screen
  AWindowController *previousController = lcdController->getCurrentWindowController();
  if (previousController != NULL) {
    previousController->deactivateScreen();
  }

  //Activate new screen
  lcdController->setCurrentWindowController(event->getWindowController());
  lcdController->setCurrentWindow(event->getPage());
  lcdController->updateListenerList();
  event->getWindowController()->initializeScreen(event->getPage());


  ///////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////

  //event->getWindowController()->initializeScreen(event->getPage());
  //lcdController->updateListenerList();

/*
  if (event->getPage() == &settingsSensorSearch)
    sensorSearchWC.initializeScreen(event->getPage());
  else if (event->getPage() == &settingsSensorsTest)
    sensorTestWC.initializeScreen(event->getPage());
  else if (event->getPage() == &settingsOutputTest)
    outputTestWC.initializeScreen(event->getPage());
  else if (event->getPage() == &mashingPage)
    mashingWC.initializeScreen(event->getPage());
  else
    commonWC.initializeScreen(event->getPage());
*/
}

LcdController::LcdController() {
    logger = new Logger("lcd_controller");
    logger->trace("Initializing serial connection for Nextion LCD");

    currentWindowController = NULL;
    currentWindow = NULL;

    //Register event handlers for pages
    mainPage.attachPop(windowOpenCallback, new PageEvent(this, &mainPage, &commonWC));
    settingsPage.attachPop(windowOpenCallback, new PageEvent(this, &settingsPage, &commonWC));
    settingsSensorSearch.attachPop(windowOpenCallback, new PageEvent(this, &settingsSensorSearch, &sensorSearchWC));
    settingsSensorsTest.attachPop(windowOpenCallback, new PageEvent(this, &settingsSensorsTest, &sensorTestWC));
    settingsOutputTest.attachPop(windowOpenCallback, new PageEvent(this, &settingsOutputTest, &outputTestWC));
    mashingPage.attachPop(windowOpenCallback, new PageEvent(this, &mashingPage, &mashingWC));

    //Save pages, which should be listened for events
    page_list = new NexTouch*[6];//malloc(sizeof(&mainPage)*4);
    page_list[0] = &mainPage;
    page_list[1] = &settingsPage;
    page_list[2] = &settingsSensorSearch;
    page_list[3] = &settingsSensorsTest;
    page_list[4] = &settingsOutputTest;
    page_list[5] = &mashingPage;
    page_list[6] = NULL;

    nex_listen_list = NULL;
    updateListenerList();

    //Init LCD
    nexInit();
    loadingPage.show();
    //Set high speed of connection to LCD
    setBaudrate(115200);
    Serial1.flush();
    Serial1.end();
    delay(50);
    Serial1.begin(115200);
    delay(150);

    loadingPage.show();
}

void LcdController::showMainPage() {
    mainPage.show();
}

void LcdController::setCurrentWindowController(AWindowController *wc) {
    currentWindowController = wc;
}

AWindowController *LcdController::getCurrentWindowController() {
    return currentWindowController;
}

void LcdController::setCurrentWindow(NexPage *w) {
  currentWindow = w;
}

void LcdController::updateListenerList() {
  if (nex_listen_list != NULL)
    delete []nex_listen_list;

  uint8_t listenersNo = 0;
  uint8_t i = 0;
  uint8_t j = 0;
  NexTouch *e = NULL;
  NexTouch **windowListeners = NULL;

  //Count the listeners
  for(i = 0; (e = page_list[i]) != NULL; i++)
    listenersNo++;

  if(currentWindowController != NULL && currentWindowController->getListenerList() != NULL) {
    windowListeners = currentWindowController->getListenerList();

    for(i = 0; (e = windowListeners[i]) != NULL; i++)
      listenersNo++;
  }

  //Copy listeners to the new array
  nex_listen_list = new NexTouch *[listenersNo+1];
  for(i = 0; (e = page_list[i]) != NULL; i++) {
    nex_listen_list[j] = e;
    j++;
//    Log.trace("Page listener: 0x%X", e);
  }
  if (windowListeners != NULL) {
    for(i = 0; (e = windowListeners[i]) != NULL; i++) {
      nex_listen_list[j] = e;
      j++;
//      Log.trace("Window listener: 0x%X", e);
    }
  }
  nex_listen_list[j] = NULL;
}

void LcdController::processMessages() {
  nexLoop(nex_listen_list);

  if (currentWindowController != NULL) {
    currentWindowController->process();
  }
}
