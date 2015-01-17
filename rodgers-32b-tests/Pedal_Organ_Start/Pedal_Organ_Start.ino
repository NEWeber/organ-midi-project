// last number is the channel number. i.e. if you want to change this to channel 2,
// the NOTE_ON_CMD should be set to 0x91 and the off command should be changed as well.
// Pedal outputs on MIDI channel 5.
#define NOTE_ON_CMD 0x94
#define NOTE_OFF_CMD 0x84
#define NOTE_VELOCITY 127

//MIDI baud rate
#define SERIAL_RATE 31250

// this decides what note the leftmost key will sound
#define LOWEST_NOTE 36

// how many keys
#define NUM_KEYS 32

byte pressStateLower[NUM_KEYS] = {0};

// @todo: generate this with an object constructor
const int c2Pin = 53;  
const int c2SharpPin = 52;
const int d2Pin = 51;
const int d2SharpPin =  50;
const int e2Pin = 49;  
const int f2Pin = 48;
const int f2SharpPin = 47;
const int g2Pin =  46;
const int g2SharpPin = 45;  
const int a2Pin = 44;
const int a2SharpPin = 43;
const int b2Pin = 42;
const int c3Pin = 41;  
const int c3SharpPin = 40;
const int d3Pin = 39;
const int d3SharpPin =  38;
const int e3Pin = 37;  
const int f3Pin = 36;
const int f3SharpPin = 35;
const int g3Pin =  34;
const int g3SharpPin = 33;  
const int a3Pin = 32;
const int a3SharpPin = 31;
const int b3Pin = 30;
const int c4Pin = 29;  
const int c4SharpPin = 28;
const int d4Pin = 27;
const int d4SharpPin =  26;
const int e4Pin = 25;  
const int f4Pin = 24;
const int f4SharpPin = 23;
const int g4Pin =  22;

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

