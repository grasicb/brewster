#pragma once

#include "application.h"
#include "../util/BrewsterGlobals.h"

// TONES  ==========================================
// Start by defining the relationship between
//       note, period, &  frequency.
#define  c     3830    // 261 Hz
#define  d     3400    // 294 Hz
#define  e     3038    // 329 Hz
#define  f     2864    // 349 Hz
#define  g     2550    // 392 Hz
#define  a     2272    // 440 Hz
#define  b     2028    // 493 Hz
#define  C     1912    // 523 Hz
// Define a special note, 'R', to represent a rest
#define  R     0


class Speaker {
public:
    Speaker();
    ~Speaker() {};
    static void playTheme();
    static void playBeep();
    static void playComplete();
    static void playAlarm();
    static void playShortTone();

private:
};
