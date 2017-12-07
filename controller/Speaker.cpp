#include "Speaker.h"

Speaker::Speaker() {
}

void Speaker::playTheme() {
  int speakerPin = BrewsterGlobals::get()->pinSpeaker;
  int melody[] = {1908,2551,2551,2273,2551,0,2024,1908};
  int noteDurations[] = {4,8,8,4,4,4,4,4 };
  for (int thisNote = 0; thisNote < 8; thisNote++) {

   // to calculate the note duration, take one second
   // divided by the note type.
   //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
   int noteDuration = 1000/noteDurations[thisNote];
   tone(speakerPin, melody[thisNote],noteDuration);

   // to distinguish the notes, set a minimum time between them.
   // the note's duration + 30% seems to work well:
   int pauseBetweenNotes = noteDuration * 1.30;
   delay(pauseBetweenNotes);
   // stop the tone playing:
   noTone(speakerPin);
 }
}

void Speaker::playBeep() {
    int speakerPin = BrewsterGlobals::get()->pinSpeaker;
    tone(speakerPin, g,750);
}

void Speaker::playComplete() {
    int speakerPin = BrewsterGlobals::get()->pinSpeaker;
    tone(speakerPin, b,500);
}

void Speaker::playShortTone() {
    int speakerPin = BrewsterGlobals::get()->pinSpeaker;
    tone(speakerPin, b,150);
}

void Speaker::playAlarm() {
    int speakerPin = BrewsterGlobals::get()->pinSpeaker;
    tone(speakerPin, c,900);
}
