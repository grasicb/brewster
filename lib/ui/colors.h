#ifndef UI_COLORS
#define UI_COLORS

static uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

//const static uint16_t colorBackground = color565(65,70,76);
const static uint16_t colorBackground = color565(0,0,0);
const static uint16_t colorFont = color565(0,0,0);
const static uint16_t colorButtonFont = color565(0,0,0);
const static uint16_t colorButtonNormal = color565(198,206,45);
const static uint16_t colorButtonSelected = color565(250,255,0);
//const static uint16_t colorComponentBackground = color565(131,181,61);
const static uint16_t colorComponentBackground = color565(49,224,198);

/*
const static uint16_t colorBackground = 0;
const static uint16_t colorFont = 0;
const static uint16_t colorButtonNormal = 0;
const static uint16_t colorButtonSelected = 0;
const static uint16_t colorComponentBackground = 0;
*/

#endif /* end of include guard: UI_COLORS */
