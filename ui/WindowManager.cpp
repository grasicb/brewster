#include "WindowManager.h"

//Logger logger("app.ui.framework");

void WindowManager::openWindow(uint8_t windowId) {
  if (currentWindow != NULL) {
//    delete currentWindow;
    currentWindow = NULL;
  }

  std::map<Windows, AWindow *>::iterator it;
  it = windows.find((Windows)windowId);

  //If window is found in the map, then set it as current window and open it later
  if (it != windows.end()) {
    currentWindow = it->second;

  //If window is not found, then create a new instance
  }else{
    switch (windowId) {
      case Windows::MAIN_WINDOW:
        Log.trace("Initializing Main Window");
        currentWindow = new MainWindow(tft, this);
        break;

      case Windows::WINDOW_SETTINGS:
        Log.trace("Initializing Settings Window");
        currentWindow = new SettingsWindow(tft, this);
        break;

      case Windows::WINDOW_MASHING:
        Log.trace("Initializing Mashing Window");
        currentWindow = new MashingWindow(tft, this);
        break;

      case Windows::WINDOW_FERMENTING:
        Log.trace("Initializing Fermenting Window");
        currentWindow = new FermentationWindow(tft, this);
        break;

      case Windows::WINDOW_TEMP_SENSORS_TEST:
        Log.trace("Initializing Temp Sensors Test Window");
        currentWindow = new TempSensorsTestWindow(tft, this);
        break;

      default:
        Log.error("Could not find window with ID %d", windowId);
        break;
    }

    if (currentWindow != NULL) {
      windows[(Windows)windowId] = currentWindow;
    }
  }

  if (currentWindow != NULL) {
    Log.trace("Opening window id: %d", windowId);
    currentWindow->initScreen();
  }
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
