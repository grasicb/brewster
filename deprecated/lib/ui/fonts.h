#ifndef UI_FONTS
#define UI_FONTS

#include "../lcd/Fonts/FreeMonoBold12pt7b.h"
#include "../lcd/Fonts/FreeMono9pt7b.h"
#include "../lcd/Fonts/FreeSans9pt7b.h"
#include "../lcd/Fonts/FreeSans12pt7b.h"
#include "../lcd/Fonts/FreeSans18pt7b.h"
#include "../lcd/Fonts/Picopixel.h"
#include "../lcd/Fonts/TomThumb.h"

const static GFXfont *font_standard = &FreeSans12pt7b;
const static GFXfont *font_small = &FreeSans9pt7b;
const static GFXfont *font_large = &FreeSans18pt7b;
//const static GFXfont *font_tiny = &Picopixel;
//const static GFXfont *font_tiny = NULL;
const static GFXfont *font_tiny = &TomThumb;
const static GFXfont *font_monospace = &FreeMonoBold12pt7b;
const static GFXfont *font_button = NULL;

#endif /* end of include guard:  */
