/*

Rain

A slightly more complicated Arduino audio demo that uses an 8-ohm piezo
speaker to play the rain summoning music from Xian Jian Qi Xia Zhuan.

I opted to subset the notes used and dump my dependence on pitches.h.

Version:   1.0
Author:    Derrick Sobodash <derrick@sobodash.com>
Copyright: (c) 2012 Derrick Sobodash
Web site:  https://github.com/sobodash/arduino-speaker/
License:   BSD License <http://opensource.org/licenses/bsd-license.php>

-----------------------------------------------------------------------------

Parts Needed

  *  3x Red LEDs
  *  3x Green LEDs
  *  1x Digital LED
  *  1x 500-ohm potentiometer
  * 12x 220-ohm resistors
  *  1x 100-ohm resistor
  *  1x 8-ohm piezoelectric speaker

-----------------------------------------------------------------------------

Hooking components to the Arduino

DigitalPin 2 -> Green LED 1
DigitalPin 3 -> Green LED 2
DigitalPin 4 -> Green LED 3
DigitalPin 5 -> Red LED 1
DigitalPin 6 -> Red LED 2
DigitalPin 7 -> Red LED 3
DigitalPin 9 -> Speaker

Make sure all LEDs have 220-ohm resistors before the anode. The piezo
speaker should have a 100-ohm resistor on its ground wire.

You can pass DigitalPin 7 to the potentiometer's input and its output
to the speaker, allowing you to use it as a volume dial. A 500-ohm
potentiometer has enough resistance to significantly reduce the volume,
but not enough to mute it.

-----------------------------------------------------------------------------

Connecting the Arduino to the 74HC595

DigitalPin 8  -> ST_CP (PIN12)
DigitalPin 11 -> DS    (PIN14)
DigitalPin 12 -> SH_CP (PIN11)
   Common GND -> GND   (PIN8)
   Common GND -> OE    (PIN13)
   Common +5V -> VCC   (PIN16)
   Common +5V -> MR    (PIN10)

-----------------------------------------------------------------------------

Connecting the digital LED to the 74HC595
 
   ____c____         dp - parallel data output 0 (Q0)
  |         |         a - parallel data output 1 (Q1)
f |         | b       b - parallel data output 2 (Q2)
  |____g____|         c - parallel data output 3 (Q3)
  |         |         d - parallel data output 4 (Q4)
e |         | a       e - parallel data output 5 (Q5)
  |____d____| o dp    f - parallel data output 6 (Q6)
                      g - parallel data output 7 (Q7)

I have no idea whether this is "standard," but it's how you will have to
connect your digital LED if you want it to work with my glyphs :)

Connect the common anode on both sides to Common +5V on the Arduino. Put
220-ohm resistors between the cathodes and parallel data output pins.

*/

// Frequencies for notes
#define _AS4 466
#define _A5  880
#define _AS5 932
#define _A6  1760
#define _C5  523
#define _C6  1047
#define _CS6 1109
#define _D5  587
#define _D6  1175
#define _E5  659
#define _E6  1319
#define _F5  698
#define _F6  1397
#define _G5  784
#define _G6  1568

// Notes for the song
int notes[218] = {
  _A5, _G5, _G5, _F5, _D5, _G5, _F5, _G5, _F5, _D5, _G5, _F5, _C6, _G5, _A5,
  _G5, _F5, _F5, _E5, _F5, _E5, _D5, _C5, _A5, _G5, _G5, _F5, _D5, _G5, _F5,
  _G5, _F5, _D5, _G5, _F5, _C6, _G5, _A5, _G5, _F5, _F5, _E5, _F5, _E5, _D5,
  _C5, _AS4,_D5, _AS4,_D5, _AS4,_D5, _AS4,_D5, _AS4,_D5, _A5, _G5, _F5, _E5,
  _D5, _C5, _C5, _E5, _C5, _E5, _C5, _E5, _C5, _E5, _C5, _E5, _C6, _AS5,_A5,
  _G5, _F5, _G5, _A5, _A5, _A5, _A5, _A5, _A5, _A5, _A5, _A5, _A5, _A5, _A5,
  _A5, _A5, _AS5,_C6, _D6, _D6, _D6, _D6, _D6, _D6, _D6, _F6, _E6, _E6, _E6,
  _E6, _E6, _E6, _E6, _G6, _F6, _F6, _F6, _F6, _F6, _F6, _E6, _F6, _D6, _D6,
  _D6, _D6, _D6, _D6, _D6, _D6, _D6, _D6, _D6, _D6, _D6, _D6, _F6, _E6, _E6,
  _E6, _E6, _E6, _E6, _D6, _C6, _A5, _CS6,_A5, _D6, _A5, _E6, _A5, _A6, _A5,
  _CS6,_A5, _D6, _A5, _E6, _A6, _G6, _D6, _D6, _D6, _D6, _D6, _D6, _D6, _F6,
  _E6, _E6, _E6, _E6, _E6, _E6, _E6, _G6, _F6, _F6, _F6, _F6, _F6, _F6, _E6,
  _D6, _D6, _D6, _D6, _D6, _C6, _D6, _D6, _D6, _D6, _D6, _D6, _D6, _F6, _E6,
  _E6, _E6, _E6, _E6, _E6, _D6, _C6, _A5, _CS6,_A5, _D6, _A5, _E6, _A5, _A6,
  _A5, _CS6,_A5, _D6, _A5, _E6, _G6, _A6
};

// Durations of each note
const int beats[218] = {
  16,  8, 16, 16,  6, 16,  8, 16, 16,  6, 16,  8, 16, 16,  6, 16, 16, 16, 16,
  16, 16, 16, 16, 16,  8, 16, 16,  6, 16,  8, 16, 16,  6, 16,  8, 16, 16,  6,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 16, 16,  8, 16, 16, 16, 16,  8,  8, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 16, 16, 16
};

// Glyphs to draw on the digital LCD
const byte chars[36] = {
  B01111110, B00000110, B10111100, B10011110,	// 0123
  B11000110, B11011010, B11111010, B00001110,	// 4567
  B11111110, B11011110, B11101110, B11110010,	// 89AB
  B01111000, B10110110, B11111000, B11101000,	// CDEF
  B11011110, B11100010, B01100000, B00110110,	// GHIJ
  B11100110, B01110000, B10011110, B01101110,	// KLMN (M is sideways)
  B01111110, B11101100, B11001110, B10100000,	// OPQR
  B11011010, B11110000, B01110110, B01110110,	// STUV (U and V are the same)
  B11111000, B11100110, B11010110, B10111100	// WXYZ
};

// Pins connected to the LED pitch visualizer
const int meter1 = 2;
const int meter2 = 3;
const int meter3 = 4;
const int meter4 = 5;
const int meter5 = 6;
const int meter6 = 7;

// Pins connected to the serial shifter
const int latch  = 8;
const int sound  = 9;
const int data   = 11;
const int clock  = 12;

// Set all the necessary pins to output
void setup() {
  pinMode(latch,  OUTPUT);
  pinMode(data,   OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(meter1, OUTPUT);
  pinMode(meter2, OUTPUT);
  pinMode(meter3, OUTPUT);
  pinMode(meter4, OUTPUT);
  pinMode(meter5, OUTPUT);
  pinMode(meter6, OUTPUT);
    delay(3200);
  play();
}

// Play the song
void play() {
  for(int i=0; i<218; i++) {
    // 1700 seems to be the right temp; double it for the dotted notes hack
    int duration = 3400/beats[i];

    // Write the current note to the digital LED
    strobe(notes[i]);

    // Play the note
    tone(sound, notes[i], duration);
    delay(duration * 1.3);

    // Reset and loop
    noTone(sound);
    strobe(0);
  }
}

// Unused, but required
void loop() { }

// Draw a glyph to the digital LCD and strobe the visualizer
void strobe(int note) {
  byte bits;
  
  // Decimal point will indicate sharps.
  if     (note == _AS4 || note == _AS5) bits = chars[10] + 1;
  else if(note == _A5  || note == _A6 ) bits = chars[10];
  else if(note == _C5  || note == _C6 ) bits = chars[12];
  else if(note == _CS6)                 bits = chars[12] + 1;
  else if(note == _D5  || note == _D6 ) bits = chars[13];
  else if(note == _E5  || note == _E6 ) bits = chars[14];
  else if(note == _F5  || note == _F6 ) bits = chars[15];
  else if(note == _G5  || note == _G6 ) bits = chars[16];
  
  // Use _A4 - 1 to calibrate the visualizer
  int baseline = 465;
  
  // Switch on all applicable LEDs in the visualizer
  if(note > baseline +    0) digitalWrite(meter1, HIGH);
                        else digitalWrite(meter1, LOW);
  if(note > baseline +  200) digitalWrite(meter2, HIGH);
                        else digitalWrite(meter2, LOW);
  if(note > baseline +  400) digitalWrite(meter3, HIGH);
                        else digitalWrite(meter3, LOW);
  if(note > baseline +  600) digitalWrite(meter4, HIGH);
                        else digitalWrite(meter4, LOW);
  if(note > baseline +  800) digitalWrite(meter5, HIGH);
                        else digitalWrite(meter5, LOW); 
  if(note > baseline + 1000) digitalWrite(meter6, HIGH);
                        else digitalWrite(meter6, LOW);
  
  // Invert glyph bits since we use a common anode LED
  bits = bits ^ 0xff;
  
  // Push the bits to the serial
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, bits);
  digitalWrite(latch, HIGH);
}

// Reset the visualizer
void clear_meter() {
  digitalWrite(meter1, LOW);
  digitalWrite(meter2, LOW);
  digitalWrite(meter3, LOW);
  digitalWrite(meter4, LOW);
  digitalWrite(meter5, LOW);
  digitalWrite(meter6, LOW);
}

