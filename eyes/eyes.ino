/*

Eyes on Me

A simple Arduino demo that plays the the first part of "Eyes on Me"
(from Final Fantasy VIII) by Faye Wang on an 8-ohm piezo speaker.

The code uses the convenient square wave values in pitches.h by Tom Igoe.

Version:   1.0
Author:    Derrick Sobodash <derrick@sobodash.com>
Copyright: (c) 2012 Derrick Sobodash
Web site:  https://github.com/sobodash/arduino-speaker/
License:   BSD License <http://opensource.org/licenses/bsd-license.php>

-----------------------------------------------------------------------------

Parts Needed:

* 4x Red LEDs
* 4x Green LEDs
* 2x Blue LEDs
* 5x 220-ohm resistors
* 1x 100-ohm resistor
* 1x 8-ohm piezoelectric speaker
  
Arduino Pins:

*  8 - Speaker
* 13 - Innermost LED pair
* 12 - Second LED pair
* 11 - Third LED pair
* 10 - Fourth LED pair
*  9 - Fifth LED pair

*/

#include "pitches.h"

#define  REST 0

// Notes of the melody (0 marks a rest)
// Played in the key of C Major (no sharps or flats)
int notes[] = {
  // Whenever sang my songs
  NOTE_G3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_E4, REST,
  // On the stage, on my own
  NOTE_D4, NOTE_E4, NOTE_C4, NOTE_A3, NOTE_C4, NOTE_D4,
  // Whenever said my words
  NOTE_G3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_B4, REST,
  // Wishing they would be heard
  NOTE_B4, NOTE_C5, NOTE_A4, REST,    NOTE_G4, NOTE_A4, NOTE_G4, REST,
  // I saw you smiling at me
  NOTE_G4, NOTE_C5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4, NOTE_G4,
  // Was it real, or just my fantasy
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_D4, NOTE_E4, REST,
  // You'd always be there in the corner
  NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4,
  // Of this tiny little bar
  NOTE_A3, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_D4, REST,
  // My last night here for you
  NOTE_G3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_E4, REST,
  // Same old songs, just once more
  NOTE_D4, NOTE_E4, NOTE_C4, NOTE_A3, NOTE_C4, NOTE_D4,
  // My last night here with you?
  NOTE_G3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_B4, REST,
  // Maybe yes, maybe no
  NOTE_B4, NOTE_C5, NOTE_A4, 0,       NOTE_G4, NOTE_A4, NOTE_G4, REST,
  // I king of liked it your way
  NOTE_G4, NOTE_C5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4, NOTE_G4,
  // How you shyly placed your eyes on me
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_C4, NOTE_E4, NOTE_D4, REST,
  // Oh, did you ever know?
  NOTE_G3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, REST,
  // That I had mine on you
  NOTE_G4, NOTE_F4, NOTE_C4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_C4, REST, REST,
  // Darling, so there you are
  NOTE_C4, NOTE_E4, NOTE_G4, NOTE_B4, NOTE_A4, NOTE_A4, REST,
  // With that look on your face
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_G4, REST,
  // As if you're never hurt
  NOTE_E4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_F4, REST,
  // As if you're never down
  NOTE_F4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, REST,
  // Shall I be the one for you
  NOTE_C4, NOTE_C4, NOTE_E4, NOTE_G4, NOTE_B4, NOTE_A4, NOTE_A4, REST,
  // Who pinches you softly, but sure
  NOTE_F4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_B4, NOTE_A4, REST,
  // If frown is shown then
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_C5, NOTE_F4, NOTE_F4,
  // I will know that you are no dreamer
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_B3, NOTE_D4, NOTE_C4, NOTE_C4
};

// Int sucks for dotted notes since they can't be represented by a nice
// whole number. Rather than rewrite everything to use float or additive time
// (melody is based on division) I decided to just double everything.
// This means:
// *  2 = whole note
// *  4 = half note
// *  8 = quarter note
// * 16 = eighth note
// * 32 = sixteenth note
// Lazy, but it allows us to represent a dotted quarter note as 6.
int beats[] = {
  16,  8,  8,  8, 16,  3,  4,
  16, 16,  4, 16,  8,  3,
  16,  8,  8,  8, 16,  3,  4,
  16, 16,  8, 16, 16, 16,  3, 16,
  16,  6, 16,  8, 16,  8,  3,
  16, 16,  6, 16,  8,  8, 16, 16,  4, 16,
  16, 16, 16, 16, 16,  8, 16, 16, 16,  3,
  16, 16,  3, 16, 16, 16,  3, 16,
  16,  8,  8,  8, 16,  3,  4,
  16, 16,  4, 16,  8,  3,
  16,  8,  8,  8, 16,  3,  4,
  16, 16,  8, 16, 16, 16,  3, 16,
  16,  6, 16, 12, 12,  8,  3,
  16, 16,  6, 16, 16, 16, 16,  8,  3, 16,
  16,  8,  6, 16, 16,  3, 16,
  16, 16, 16,  3, 16, 16,  3,  8,  4,
  12, 12, 16,  6, 16,  4,  4,
  12, 12, 16,  8, 12,  3,  6,
  16, 16, 16,  6, 16,  4,  4,
  12, 12, 16,  8, 16,  3,  6,
  16, 12, 12, 16,  6, 16,  4,  6,
  16, 12, 12, 16,  8,  4,  8,  3, 16,
  16, 16, 16, 16, 16,  6,
  32, 32,  6,  8, 16, 16, 16,  3, 16,  2
};

// Turn on LED pairs based on pitch so we have something like a 10-light
// colorful visualizer.
void setLights(int light) {
  if(light > 150) digitalWrite(13, HIGH);
  if(light > 225) digitalWrite(12, HIGH);
  if(light > 300) digitalWrite(11, HIGH);
  if(light > 375) digitalWrite(10, HIGH);
  if(light > 450) digitalWrite( 9, HIGH);
}

// Return all LED pairs to off. This is called after each note.
void clearLights() { 
  digitalWrite( 9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}

void setup() {
  // Set Arduino pins 9-13 for on/off output
  pinMode( 9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
  // Step through the notes of the melody:
  for (int i=0; i<185; i++) {
    
    // 1800 seemed quite close to the tempo, so doubled this gets us a
    // base duration of 3600
    int duration = 3600/beats[i];

    // Play the note and turn on the lights
    tone(8, notes[i], duration);
    setLights(notes[i]);
    
    // Pause long enough that repeat hits of the same note will be distinguishable.
    int pause = duration * 1.30;
    delay(pause);

    // Clear the speaker state and all LEDs.
    noTone(8);
    clearLights();

  }
}

// Unused, but required
void loop() {
  
}
