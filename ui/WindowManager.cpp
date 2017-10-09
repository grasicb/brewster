#include "WindowManager.h"

//Logger logger("app.ui.framework");

void WindowManager::openWindow(uint8_t windowId) {
  if (currentWindow != NULL) {
//    delete currentWindow;
    currentWindow = NULL;
  }

  switch (windowId) {
    case Windows::MAIN_WINDOW:
      Log.trace("Opening Main Window");
      currentWindow = new MainWindow(tft, this);
      break;

    case Windows::WINDOW_SETTINGS:
      Log.trace("Opening Settings Window");
      currentWindow = new SettingsWindow(tft, this);
      break;

    default:
      Log.error("Could not find window with ID %d", windowId);
      break;
  }

  if (currentWindow != NULL) {
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
