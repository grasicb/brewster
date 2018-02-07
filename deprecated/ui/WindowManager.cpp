#include "WindowManager.h"

//Logger logger("app.ui.framework");

void WindowManager::openWindow(uint8_t windowId) {
  openWindow(getWindowReference(windowId));
}

void WindowManager::destroyWindow(uint8_t windowId) {}

void WindowManager::process() {

  if (currentWindow != NULL)
    currentWindow->process(NULL);
  else
    Log.warn("Cannot process current window, current window is null.");
}

void WindowManager::screenTouched(uint16_t x, uint16_t y) {
    currentWindow->screenTouched(x, y);
}

void WindowManager::screenReleased() {
    currentWindow->screenReleased();
}

AWindow *WindowManager::getActiveWindow() {
  return currentWindow;
}

void WindowManager::openWindow(void *window) {
  AWindow *windowRef = (AWindow *) window;

  if (windowRef != NULL) {
    if (currentWindow != NULL) {
      previousWindow = currentWindow;
      currentWindow = NULL;
    }

    currentWindow = windowRef;
    windowRef->initScreen();
  }else{
    Log.error("Cannot open window, reference is NULL.");
  }
}

void WindowManager::openPreviousWindow() {
  openWindow(previousWindow);
}

void *WindowManager::getWindow(uint8_t windowId) {
  return getWindowReference(windowId);
}

AWindow *WindowManager::getWindowReference(uint8_t windowId) {
  AWindow *window = NULL;
  std::map<Windows, AWindow *>::iterator it;
  it = windows.find((Windows)windowId);

  //If window is found in the map, then set it as current window and open it later
  if (it != windows.end()) {
    window = it->second;

  //If window is not found, then create a new instance
  }else{
    switch (windowId) {
      case Windows::MAIN_WINDOW:
        Log.trace("Initializing Main Window");
        window = new MainWindow(tft, this);
        break;

      case Windows::WINDOW_SETTINGS:
        Log.trace("Initializing Settings Window");
        window = new SettingsWindow(tft, this);
        break;

      case Windows::WINDOW_MASHING:
        Log.trace("Initializing Mashing Window");
        window = new MashingWindow(tft, this);
        break;

      case Windows::WINDOW_FERMENTING:
        Log.trace("Initializing Fermenting Window");
        window = new FermentationWindow(tft, this);
        break;

      case Windows::WINDOW_TEMP_SENSORS_TEST:
        Log.trace("Initializing Temp Sensors Test Window");
        window = new TempSensorsTestWindow(tft, this);
        break;

      case Windows::OUTPUT_TEST:
        Log.trace("Initializing Output Test Window");
        window = new OutputTestWindow(tft, this);
        break;

      case Windows::ENTER_VALUE:
        Log.trace("Initializing Enter Value Window");
        window = new EnterValueWindow(tft, this);
        break;

      default:
        Log.error("Could not find window with ID %d", windowId);
        break;
    }

    if (window != NULL) {
      windows[(Windows)windowId] = window;
    }
  }

  Log.info("Return Window ID %i: %p", windowId, window);
  return window;
}
