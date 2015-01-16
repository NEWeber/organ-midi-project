// last number is the channel number. i.e. if you want to change this to channel 2,
// the NOTE_ON_CMD should be set to 0x91 and the off command should be changed as well.
#define NOTE_ON_CMD 0x94
#define NOTE_OFF_CMD 0x84
#define NOTE_VELOCITY 127
// @todo: find a way to set output to a different MIDI channel

//MIDI baud rate
#define SERIAL_RATE 31250

// this decides what note the leftmost key will sound
#define LOWEST_NOTE 36

// how many keys
#define NUM_KEYS 32

byte pressStateLower[NUM_KEYS] = {0};

// @todo: generate this with an object constructor
const int c2Pin = A0;  
const int c2SharpPin = A1;
const int d2Pin = A2;
const int d2SharpPin =  A3;
const int e2Pin = A4;  
const int f2Pin = A5;
const int f2SharpPin = A6;
const int g2Pin =  A7;
const int g2SharpPin = A8;  
const int a2Pin = A9;
const int a2SharpPin = A10;
const int b2Pin = A11;
const int c3Pin = A12;  
const int c3SharpPin = A13;
const int d3Pin = A14;
const int d3SharpPin =  A15;
const int e3Pin = 53;  
const int f3Pin = 52;
const int f3SharpPin = 51;
const int g3Pin =  50;
const int g3SharpPin = 49;  
const int a3Pin = 48;
const int a3SharpPin = 47;
const int b3Pin = 46;
const int c4Pin = 45;  
const int c4SharpPin = 44;
const int d4Pin = 43;
const int d4SharpPin =  42;
const int e4Pin = 41;  
const int f4Pin = 40;
const int f4SharpPin = 39;
const int g4Pin =  38;

// @todo: generate this with a constructor
const int greatNotes[NUM_KEYS] = {c2Pin, c2SharpPin, d2Pin, d2SharpPin, e2Pin, f2Pin, f2SharpPin, g2Pin, g2SharpPin, a2Pin, a2SharpPin, b2Pin, c3Pin, c3SharpPin, d3Pin, d3SharpPin, e3Pin, f3Pin, f3SharpPin, g3Pin, g3SharpPin, a3Pin, a3SharpPin, b3Pin, c4Pin, c4SharpPin, d4Pin, d4SharpPin, e4Pin, f4Pin, f4SharpPin, g4Pin};

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

