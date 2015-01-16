#define NOTE_ON_CMD 0x90
#define NOTE_OFF_CMD 0x80
#define NOTE_VELOCITY 127

//MIDI baud rate
#define SERIAL_RATE 31250

// this decides what note the leftmost key will sound
#define LOWEST_NOTE 36

// how many keys
#define NUM_KEYS 61

byte pressStateLower[NUM_KEYS] = {0};


const int c3Pin = A0;  
const int c3SharpPin = A1;
const int d3Pin = A2;
const int d3SharpPin =  A3;
const int e3Pin = A4;  
const int f3Pin = A5;
const int f3SharpPin = A6;
const int g3Pin =  A7;
const int g3SharpPin = A8;  
const int a3Pin = A9;
const int a3SharpPin = A10;
const int b3Pin = A11;
const int c4Pin = A12;  
const int c4SharpPin = A13;
const int d4Pin = A14;
const int d4SharpPin =  A15;
const int e4Pin = 53;  
const int f4Pin = 52;
const int f4SharpPin = 51;
const int g4Pin =  50;
const int g4SharpPin = 49;  
const int a4Pin = 48;
const int a4SharpPin = 47;
const int b4Pin = 46;
const int c5Pin = 45;  
const int c5SharpPin = 44;
const int d5Pin = 43;
const int d5SharpPin =  42;
const int e5Pin = 41;  
const int f5Pin = 40;
const int f5SharpPin = 39;
const int g5Pin =  38;
const int g5SharpPin = 37;  
const int a5Pin = 36;
const int a5SharpPin = 35;
const int b5Pin = 34;
const int c6Pin = 33;  
const int c6SharpPin = 32;
const int d6Pin = 31;
const int d6SharpPin =  30;
const int e6Pin = 29;  
const int f6Pin = 28;
const int f6SharpPin = 27;
const int g6Pin =  26;
const int g6SharpPin = 25;  
const int a6Pin = 24;
const int a6SharpPin = 23;
const int b6Pin = 22;
const int c7Pin = 21;  
const int c7SharpPin = 20;
const int d7Pin = 2;
const int d7SharpPin =  3;
const int e7Pin = 4;  
const int f7Pin = 5;
const int f7SharpPin = 6;
const int g7Pin = 7;
const int g7SharpPin = 8;  
const int a7Pin = 9;
const int a7SharpPin = 10;
const int b7Pin = 11;
const int c8Pin = 12;  

const int greatNotes[NUM_KEYS] = {c3Pin, c3SharpPin, d3Pin, d3SharpPin, e3Pin, f3Pin, f3SharpPin, g3Pin, g3SharpPin, a3Pin, a3SharpPin, b3Pin, c4Pin, c4SharpPin, d4Pin, d4SharpPin, e4Pin, f4Pin, f4SharpPin, g4Pin, g4SharpPin, a4Pin, a4SharpPin, b4Pin, c5Pin, c5SharpPin, d5Pin, d5SharpPin, e5Pin, f5Pin, f5SharpPin, g5Pin, g5SharpPin, a5Pin, a5SharpPin, b5Pin, c6Pin, c6SharpPin, d6Pin, d6SharpPin, e6Pin, f6Pin, f6SharpPin, g6Pin, g6SharpPin, a6Pin, a6SharpPin, b6Pin, c7Pin, c7SharpPin, d7Pin, d7SharpPin, e7Pin, f7Pin, f7SharpPin, g7Pin, g7SharpPin, a7Pin, a7SharpPin, b7Pin, c8Pin};

boolean keyPressed[NUM_KEYS];
byte keyToMidiMap[NUM_KEYS];

void setup()
{
  int note = LOWEST_NOTE;

  for(int i = 0; i < NUM_KEYS; i++)
  {
      keyPressed[i] = false;
      keyToMidiMap[i] = note;
      note++;
   
  }
  //go through all the input pins for the great notes and set them to INPUT_PULLUP, so when it connects to ground it will trigger
  for (int i = 0; i < NUM_KEYS; i++) {
    pinMode(greatNotes[i], INPUT_PULLUP);
  }
  
  
  Serial.begin(SERIAL_RATE);
}


void loop()
{
  for (int noteCounter = 0; noteCounter < NUM_KEYS; noteCounter++) {
    //if the key has been pressed and it was not pressed before, send the note on message and set keyPressed to true
    if ((digitalRead(greatNotes[noteCounter]) == LOW) && (keyPressed[noteCounter] == false))
    {
      keyPressed[noteCounter] = true;
      noteOn(noteCounter);
    }
    //if the key is released and it was held before, send note off and set keyPressed to false
    else if ((digitalRead(greatNotes[noteCounter]) == HIGH) && (keyPressed[noteCounter] == true))
    {
      keyPressed[noteCounter] = false;
      noteOff(noteCounter);
    }
  }

delay(10);
}

void noteOn(int noteNum)
{
  Serial.write(NOTE_ON_CMD);
  Serial.write(keyToMidiMap[noteNum]);
  Serial.write(NOTE_VELOCITY);
}

void noteOff(int noteNum)
{
  Serial.write(NOTE_OFF_CMD);
  Serial.write(keyToMidiMap[noteNum]);
  Serial.write(NOTE_VELOCITY);
}

