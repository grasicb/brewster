#include "AWindow.h"

Logger logger("app.ui.framework");

void AWindow::screenTouched(uint16_t x, uint16_t y) {
  buttonPressed = -1;

  for (uint8_t i=0; i<buttons.size(); i++) {
    if (buttons[i].isClicked(x, y)) {
      buttonPressed = i;
    }
  }
}

void AWindow::screenReleased() {
  if (buttonPressed != -1) {
    uint8_t action = buttons[buttonPressed].getAction();
    buttons[buttonPressed].deactivateButton();
    buttonPressed = -1;

    if (Log.isTraceEnabled()) {
      logger.trace("Trigger action: %03d", action);
    }

    processAction(action);
  }
}
