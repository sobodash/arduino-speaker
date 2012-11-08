/*

Gong Jin Ou

A demo that plays the national anthem of Qing Dynasty China. Code is based on
the public domain Melody demo by Tom Igoe.

Version:   1.0
Author:    Derrick Sobodash <derrick@sobodash.com>
Copyright: (c) 2012 Derrick Sobodash
Web site:  https://github.com/sobodash/arduino-speaker/
License:   BSD License <http://opensource.org/licenses/bsd-license.php>

*/

#include "pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_G4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_C4, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_C5, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_D4, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  1, 4, 8, 8, 4, 8, 16, 16, 4, 16, 16, 16, 16, 8, 8, 16, 16, 16, 16, 4, 8, 16, 16, 16, 16, 16, 16, 8, 16, 16, 4, 16, 16, 16, 16, 8, 8, 16, 16, 16, 16, 4, 8, 8, 8, 8, 16, 16, 16, 16, 4, 16, 16, 16, 16, 8, 8, 16, 16, 16, 16, 4, 8, 16, 16, 16, 16, 16, 16, 8, 16, 16, 4, 16, 16, 16, 16, 8, 8, 16, 16, 16, 16, 4, 16, 16, 16, 16, 8, 8, 16, 16, 16, 16, 2
};

void setLights(int light) {
  switch(light) {
    case NOTE_A4:
      digitalWrite(13, HIGH); break;
    case NOTE_C4:
      digitalWrite(12, HIGH); break;
    case NOTE_C5:
      digitalWrite(12, HIGH); break;
    case NOTE_D4:
      digitalWrite(11, HIGH); break;
    case NOTE_D5:
      digitalWrite(11, HIGH); break;
    case NOTE_E4:
      digitalWrite(10, HIGH); break;
    case NOTE_E5:
      digitalWrite(10, HIGH); break;
    case NOTE_G4:
      digitalWrite(9, HIGH); break;
    }  
}

void clearLights() { 
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 95; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 2000/noteDurations[thisNote];
    tone(8, melody[thisNote],noteDuration);
    setLights(melody[thisNote]);
    
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
    clearLights();

  }
}

void loop() {
  // no need to repeat the melody.
}
