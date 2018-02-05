#include "LcdController.h"

LcdController* LcdController::instance = NULL;



LcdController* LcdController::get() {
  if (instance == NULL) {
    instance = new LcdController();
  }
  return instance;
}

LcdController::LcdController() {
    logger = new Logger("lcd_controller");
    logger->trace("Initializing serial connection for Nextion LCD");
    readBufOffset = 0;
    buffer = "";

    Serial1.begin(9600); // Nextion

    Serial1.print("page main_page");
    Serial1.write(0xff);
  	Serial1.write(0xff);
  	Serial1.write(0xff);
}

void LcdController::processMessages() {

  // Read data from serial
  	while(Serial1.available()) {

      //logger->info("LCD data available");

  		if (readBufOffset < READ_BUF_SIZE) {
        char c = Serial1.read();
        readBuf[readBufOffset++] = c;

        /*
        char x [50];
        sprintf(x, "%02x", c);
        buffer.concat(x);
        //*/

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
}

void LcdController::updateTemperature() {

}
