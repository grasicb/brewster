/*
 * Copyright 2014 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "BrewPiTouch.h"
#include "application.h"
#include <limits.h>
// includes for getting a median:
#undef min
#undef max
#undef swap
#include <vector>
#include <algorithm>


BrewPiTouch::BrewPiTouch(SPIArbiter & spia, uint8_t cs, uint8_t irq) : _spi(spia),  pinCS(cs), pinIRQ(irq) {
}

BrewPiTouch::~BrewPiTouch() {
}

void BrewPiTouch::init(uint8_t configuration) {
    // default is:
    // 12bit (mode=0)
    // power down between conversions, penIRQ enabled (PD1,PD0 = 00)
    // Differential reference (SER/DFR = 0)
    config = configuration;
    width = -1801; //initial value: 1600
    height = -1825; //initial value: 1600
    tftWidth = 320;
    tftHeight = 240;
    xOffset = 1855; //initial value: 0
    yOffset = 1916; //initial value: 0


    setStabilityThreshold(); // default threshold
    pinMode(pinCS, OUTPUT);
    pinMode(pinIRQ, INPUT_PULLUP);
    // clock base is 120/2 = 60Mhz on the Photon and 72 Mhz on the core.
    // Minimum clock cycle is 200 + 200 = 400 ns
    // Fmax = 2.5 Mhz. 60 Mhz / 2.5 MHz = 24. Clock div of 32 is within margins

    _spi.setClockDivider(SPI_CLOCK_DIV32);
    _spi.setDataMode(SPI_MODE0);

    _spi.begin(pinCS);
    _spi.transfer(config);
    _spi.end();

    filterX.init(width/2);
    filterX.setCoefficients(SETTLING_TIME_25_SAMPLES);
    filterY.init(height/2);
    filterY.setCoefficients(SETTLING_TIME_25_SAMPLES);
    update();
}

void BrewPiTouch::set8bit() {
    config = config | MODE;
}

void BrewPiTouch::set12bit() {
    config = config & ~MODE;
}

bool BrewPiTouch::is8bit() {
    return (config & MODE) ? 1 : 0;
}

bool BrewPiTouch::is12bit() {
    return (config & MODE) ? 0 : 1;
}

uint16_t BrewPiTouch::readChannel(uint8_t channel) {
    uint16_t data;

    _spi.begin(); // will drive CS pin low, needed for conversion timing
    _spi.transfer((config & CHMASK) | channel); // select channel x/y
    delayMicroseconds(1); // make sure conversion is complete, without checking busy pin
    data = _spi.transfer(0);
    if (is12bit()) {
        data = data << 8;
        data += _spi.transfer(0);
        data = data >> 4;
    }
    _spi.end(); // will drive CS pin high again
    return data;
}

bool BrewPiTouch::isTouched() {
    return (digitalRead(pinIRQ) == HIGH ? 0 : 1);
}

int16_t BrewPiTouch::getXRaw() {
    return filterX.readInput();
}

int16_t BrewPiTouch::getYRaw() {
    return filterY.readInput();
}

int16_t BrewPiTouch::getX() {
    int32_t val = getXRaw(); // create room for multiplication
    val -= xOffset; // remove offset
    val = val * tftWidth / width; //scale
    return val;
}

int16_t BrewPiTouch::getY() {
    int32_t val = getYRaw(); // create room for multiplication
    val -= yOffset; // remove offset
    val = val * tftHeight / height; //scale
    return val;
}

/*
 *  update() updates the x and y coordinates of the touch screen
 *  It reads numSamples values and takes the median
 *  The result is fed to the low pass filters
 */
bool BrewPiTouch::update(uint16_t numSamples) {
    std::vector<int16_t> samplesX;
    std::vector<int16_t> samplesY;

    bool valid = true;

    if (!isTouched()) {
    	return false; // exit immediately when not touched to prevent claiming SPI
    }
    for (uint16_t i = 0; i < numSamples; i++) {
        if (!isTouched()) {
            valid = false;
            break;
        }
        samplesX.push_back(readChannel(CHX));
        samplesY.push_back(readChannel(CHY));
    }
    if (valid) {
        // get median
        size_t middle = samplesX.size() / 2;
        std::nth_element(samplesX.begin(), samplesX.begin() + middle, samplesX.end());
        std::nth_element(samplesY.begin(), samplesY.begin() + middle, samplesY.end());
        // feed to filter to check stability
        filterX.add(samplesX[middle]);
        filterY.add(samplesY[middle]);
    }
    return valid && isStable();
}

void BrewPiTouch::setStabilityThreshold(int16_t threshold){
    stabilityThreshold = threshold;
}

/* isStable() returns true if the difference between the last sample and
 * a low pass filtered value of past samples is under a certain threshold
 */
bool BrewPiTouch::isStable() {
    if (abs(filterX.readInput() - filterX.readOutput()) > stabilityThreshold) {
        return false;
    }
    if (abs(filterY.readInput() - filterY.readOutput()) > stabilityThreshold) {
        return false;
    }
    return true;
}


void BrewPiTouch::calibrate(Adafruit_ILI9341 * tft) {
    Serial.println("Starting Touchscreen calibration");
    int32_t xTouch[3];
    int32_t yTouch[3];

    tftWidth = tft->width();
    tftHeight = tft->height();

    int32_t xDisplay[3] = {CALIBRATE_FROM_EDGE, CALIBRATE_FROM_EDGE, tftWidth - CALIBRATE_FROM_EDGE};
    int32_t yDisplay[3] = {CALIBRATE_FROM_EDGE, tftHeight - CALIBRATE_FROM_EDGE, tftHeight / 2};

    volatile int16_t samples;

    const int16_t requiredSamples = 1024;
    tft->fillScreen(ILI9341_BLACK);
    for (uint8_t i = 0; i < 3; i++) {
        //tft->drawCrossHair(xDisplay[i], yDisplay[i], 10, ILI9341_GREEN);
        tft->drawPixel(xDisplay[i], yDisplay[i], ILI9341_GREEN);
        while (!isTouched()); // wait for touch
        do {
            samples = 0;
            xTouch[i] = 0;
            yTouch[i] = 0;
            tft->drawFastHLine(0, 0, tftWidth, ILI9341_RED);
            while (isTouched()) {
                update();
                if (!isStable()) {
                    // update is not valid, reset
                    break;
                }
                int32_t xSample = getXRaw();
                int32_t ySample = getYRaw();

                xTouch[i] += xSample;
                yTouch[i] += ySample;
                samples++;

                int32_t xAverage = xTouch[i] / samples;
                int32_t yAverage = yTouch[i] / samples;

                int16_t xCalibrated = getX();
                int16_t yCalibrated = getY();
                tft->fillCircle(xCalibrated, yCalibrated, 2, ILI9341_WHITE);

                // print progress line
                uint16_t progress = samples * tftWidth / requiredSamples;
                tft->drawFastHLine(0, 0, progress, ILI9341_BLUE);

                // stop when required number of samples is reached
                if (samples >= requiredSamples) {
                    tft->drawFastHLine(0, 0, tftWidth, ILI9341_GREEN);
                    tft->fillCircle(xDisplay[i], yDisplay[i], 8, ILI9341_BLUE);
                    while (isTouched()); // wait until released
                    break;
                }

                if (abs(xSample - xAverage) > 50 || abs(ySample - yAverage) > 50) {
                    // if new sample deviates too much from average, reset
                    break;
                }
            }
        } while (samples < requiredSamples || isTouched());
        xTouch[i] = xTouch[i] / samples;
        yTouch[i] = yTouch[i] / samples;
    }

    width = tftWidth * (xTouch[2] - xTouch[0]) / (xDisplay[2] - xDisplay[0]);
    height = tftHeight * (yTouch[1] - yTouch[0]) / (yDisplay[1] - yDisplay[0]);
    xOffset = xTouch[0] - xDisplay[0] * width / tftWidth;
    yOffset = yTouch[0] - yDisplay[0] * height / tftHeight;
    Serial.println("Calibration complete");
    Serial.printf("Width: %d, Height: %d, xOffset: %d, yOffset: %d", width, height, xOffset, yOffset);
    Serial.println("\n");
}
