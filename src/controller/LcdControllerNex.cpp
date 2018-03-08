#include "LcdControllerNex.h"

#include "../ui_nextion/PageEvent.h"
/*
LcdControllerNex* LcdControllerNex::instance = NULL;

LcdControllerNex* LcdControllerNex::get() {
  if (instance == NULL) {
    instance = new LcdControllerNex();
  }
  return instance;
}
*/
void LcdControllerNex::windowOpenCallback(void *ptr)
{
  PageEvent *event = (PageEvent *)ptr;
  LcdControllerNex *lcdController = (LcdControllerNex *)event->getSourceEntity();

  lcdController->logger->info("Openning Window: %s", event->getPage()->getObjName());

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
}

LcdControllerNex::LcdControllerNex() {
    this->logger = new Logger("lcd");
    this->logger->trace("Creating LCD Controller");
    Log.info("LCD: Creating LCD Controller");

    //Init LCD
    nexInit();
    //setDefaultBaudrate(9600);
    sendCommand("bkcmd=1");
    setBaudrate(57600);
    sendCommand("bkcmd=1");
    //sendCommand("bkcmd=1");
    //setDefaultBaudrate(57600);
    /*
    setBaudrate(115200);
    */
    loadingPage.show();

    currentWindowController = NULL;
    currentWindow = NULL;
    nex_listen_list = new NexTouch *[1];
    nex_listen_list[0] = NULL;

    //Set high speed of connection to LCD
    /*
    setBaudrate(115200);
    Serial1.flush();
    Serial1.end();
    delay(50);
    Serial1.begin(115200);
    delay(150);
    */
}

void LcdControllerNex::showMainPage() {
  this->logger->trace("Initializing serial connection for Nextion LCD");
  Log.info("LCD: Initializing serial connection for Nextion LCD");

  commonWC = new CommonWindowController();
  sensorSearchWC = new SensorSearchController();
  sensorTestWC = new SensorTestController();
  outputTestWC = new OutputTestController();
  mashingWC = new MashingWC();
  mashStepChangeWC = new MashStepChange();
  boilingWC = new BoilingWC();

  //Register event handlers for pages
  mainPage.attachPop(windowOpenCallback, new PageEvent(this, &mainPage, commonWC));
  settingsPage.attachPop(windowOpenCallback, new PageEvent(this, &settingsPage, commonWC));
  settingsSensorSearch.attachPop(windowOpenCallback, new PageEvent(this, &settingsSensorSearch, sensorSearchWC));
  settingsSensorsTest.attachPop(windowOpenCallback, new PageEvent(this, &settingsSensorsTest, sensorTestWC));
  settingsOutputTest.attachPop(windowOpenCallback, new PageEvent(this, &settingsOutputTest, outputTestWC));
  mashingPage.attachPop(windowOpenCallback, new PageEvent(this, &mashingPage, mashingWC));
  mashStepChangePage.attachPop(windowOpenCallback, new PageEvent(this, &mashStepChangePage, mashStepChangeWC));
  boilPage.attachPop(windowOpenCallback, new PageEvent(this, &boilPage, boilingWC));

  //Save pages, which should be listened for events
  page_list = new NexTouch*[9];//malloc(sizeof(&mainPage)*4);
  int i=0;
  page_list[i++] = &mainPage;
  page_list[i++] = &settingsPage;
  page_list[i++] = &settingsSensorSearch;
  page_list[i++] = &settingsSensorsTest;
  page_list[i++] = &settingsOutputTest;
  page_list[i++] = &mashingPage;
  page_list[i++] = &mashStepChangePage;
  page_list[i++] = &boilPage;
  page_list[i++] = NULL;

  nex_listen_list = NULL;
  updateListenerList();

  mainPage.show();
}

void LcdControllerNex::setCurrentWindowController(AWindowController *wc) {
    currentWindowController = wc;
}

AWindowController *LcdControllerNex::getCurrentWindowController() {
    return currentWindowController;
}

void LcdControllerNex::setCurrentWindow(NexPage *w) {
  currentWindow = w;
}

void LcdControllerNex::updateListenerList() {
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

    for(i = 0; (e = windowListeners[i]) != NULL; i++) {
      listenersNo++;
    }
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

void LcdControllerNex::processMessages() {
  nexLoop(nex_listen_list);

  if (currentWindowController != NULL) {
    currentWindowController->process();
  }
}
