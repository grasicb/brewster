#include "AWindow.h"
#include "../../controller/Speaker.h"

Logger logger("app.ui.framework");

void AWindow::screenTouched(uint16_t x, uint16_t y) {
  buttonPressed = -1;

  for (uint8_t i=0; i<buttons.size(); i++) {
    if (buttons[i].isClicked(x, y)) {
      buttonPressed = i;

      /*
      //Play tone when a button is clicked
      if (lastButtonPressed != buttonPressed) {
        lastButtonPressed = buttonPressed;
        Speaker::playShortTone();
      }
      //*/
    }
  }
}

void AWindow::screenReleased() {
  if (buttonPressed != -1) {
    uint8_t action = buttons[buttonPressed].getAction();
    buttons[buttonPressed].deactivateButton();
    //Speaker::playShortTone();
    buttonPressed = -1;
    lastButtonPressed = -1;

    if (Log.isTraceEnabled()) {
      logger.trace("Trigger action: %03d", action);
    }

    processAction(action);
  }
}
