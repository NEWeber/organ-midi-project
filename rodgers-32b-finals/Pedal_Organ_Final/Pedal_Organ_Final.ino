#include <Wire.h> 

// last number is the channel number. i.e. if you want to change this to channel 2,
// the NOTE_ON_CMD should be set to 0x91 and the off command should be changed as well.
// Pedal outputs on MIDI channel 5.
#define NOTE_ON_CMD 0x94
#define NOTE_OFF_CMD 0x84
#define NOTE_VELOCITY 127

//MIDI baud rate
const int midiSerialRate = 31250;

// this decides what note the leftmost key will sound
#define LOWEST_NOTE 36

// how many keys
#define NUM_KEYS 32
const int numButtons = 7;

// @todo: generate this with an object constructor
const int c2Pin = 22;  
const int c2SharpPin = 23;
const int d2Pin = 24;
const int d2SharpPin =  25;
const int e2Pin = 26;  
const int f2Pin = 27;
const int f2SharpPin = 28;
const int g2Pin =  29;
const int g2SharpPin = 30;  
const int a2Pin = 31;
const int a2SharpPin = 32;
const int b2Pin = 33;
const int c3Pin = 34;  
const int c3SharpPin = 35;
const int d3Pin = 36;
const int d3SharpPin =  37;
const int e3Pin = 38;  
const int f3Pin = 39;
const int f3SharpPin = 40;
const int g3Pin =  41;
const int g3SharpPin = 42;  
const int a3Pin = 43;
const int a3SharpPin = 44;
const int b3Pin = 45;
const int c4Pin = 46;  
const int c4SharpPin = 47;
const int d4Pin = 48;
const int d4SharpPin =  49;
const int e4Pin = 50;  
const int f4Pin = 51;
const int f4SharpPin = 52;
const int g4Pin =  53;

const int pedalForward = 2;
const int button1Pin = 3;  
const int button2Pin = 4;
const int button3Pin = 5;
const int button4Pin = 6;
const int button5Pin = 7;  
const int button6Pin = 8;

const int leftExpressionPin = A0;
const int rightExpressionPin = A1;

// @todo: generate this with a constructor
const int pedalNotes[NUM_KEYS] = {c2Pin, c2SharpPin, d2Pin, d2SharpPin, e2Pin, f2Pin, f2SharpPin, g2Pin, g2SharpPin, a2Pin, a2SharpPin, b2Pin, c3Pin, c3SharpPin, d3Pin, d3SharpPin, e3Pin, f3Pin, f3SharpPin, g3Pin, g3SharpPin, a3Pin, a3SharpPin, b3Pin, c4Pin, c4SharpPin, d4Pin, d4SharpPin, e4Pin, f4Pin, f4SharpPin, g4Pin};

boolean keyPressed[NUM_KEYS];
byte keyToMidiMap[NUM_KEYS];

const int buttons[numButtons] = {pedalForward, button1Pin, button2Pin, button3Pin, button4Pin, button5Pin, button6Pin};

boolean buttonPressed[numButtons];
byte buttonToMidiMap[numButtons];

int leftExpressionPedal;
int rightExpressionPedal;

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
    pinMode(pedalNotes[i], INPUT_PULLUP);
  }
  
  //start pedal forward
  
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }
  
    for(int i = 0; i < numButtons; i++)
  {
      buttonPressed[i] = false;
      buttonToMidiMap[i] = i + 3; 
  }
  
  Wire.begin();
  //Send out MIDI signals out of Serial1, can switch this to serial after done debugging
  Serial1.begin(midiSerialRate);
  //Set up Serial out to send debugging messages to the computer.
  Serial.begin(9600);
  
  pinMode(leftExpressionPin, INPUT);
  
}


void loop()
{
  for (int noteCounter = 0; noteCounter < NUM_KEYS; noteCounter++) {
    //if the key has been pressed and it was not pressed before, send the note on message and set keyPressed to true
    if ((digitalRead(pedalNotes[noteCounter]) == LOW) && (keyPressed[noteCounter] == false))
    {
      keyPressed[noteCounter] = true;
      localNoteOn(noteCounter);
    }
    //if the key is released and it was held before, send note off and set keyPressed to false
    else if ((digitalRead(pedalNotes[noteCounter]) == HIGH) && (keyPressed[noteCounter] == true))
    {
      keyPressed[noteCounter] = false;
      localNoteOff(noteCounter);
    }
  }
  
  for (int buttonCounter = 0; buttonCounter < numButtons; buttonCounter++) {
    //if the key has been pressed and it was not pressed before, send the note on message and set keyPressed to true
    if ((digitalRead(buttons[buttonCounter]) == LOW) && (buttonPressed[buttonCounter] == false))
    {
      buttonPressed[buttonCounter] = true;
      Serial1.write(0xB0);
      Serial1.write(buttonToMidiMap[buttonCounter]);
      Serial1.write(1);
    }
    //if the key is released and it was held before, send note off and set keyPressed to false
    else if ((digitalRead(buttons[buttonCounter]) == HIGH) && (buttonPressed[buttonCounter] == true))
    {
      buttonPressed[buttonCounter] = false;
    }
  }
  
  int newLeftExpressionMeasurement;
  int leftExpressionMeasurement = analogRead(leftExpressionPin);
  if (leftExpressionMeasurement < 284) {
    newLeftExpressionMeasurement = 0;
  }
  else if (leftExpressionMeasurement < 353) {
    newLeftExpressionMeasurement = 1;
  }
  else if (leftExpressionMeasurement < 422) {
    newLeftExpressionMeasurement = 2;
  }
  else if (leftExpressionMeasurement < 491) {
    newLeftExpressionMeasurement = 3;
  }
  else if (leftExpressionMeasurement < 560) {
    newLeftExpressionMeasurement = 4;
  }
  else if (leftExpressionMeasurement < 629) {
    newLeftExpressionMeasurement = 5;
  }
  else if (leftExpressionMeasurement < 698) {
    newLeftExpressionMeasurement = 6;
  }
  else {
    newLeftExpressionMeasurement = 7;
  }
  
  if (newLeftExpressionMeasurement != leftExpressionPedal) {
    if (newLeftExpressionMeasurement == 0) {
      Serial1.write(0xB0);
      Serial1.write(1);
      Serial1.write(0);
    } 
    else if (newLeftExpressionMeasurement == 1) {
      Serial1.write(0xB0);
      Serial1.write(1);
      Serial1.write(21);
    } 
    else if (newLeftExpressionMeasurement == 2) {
      Serial1.write(0xB0);
      Serial1.write(1);
      Serial1.write(42);
    }
   else if (newLeftExpressionMeasurement == 3) {
      Serial1.write(0xB0);
      Serial1.write(1);
      Serial1.write(63);
    }  
    else if (newLeftExpressionMeasurement == 4) {
      Serial1.write(0xB0);
      Serial1.write(1);
      Serial1.write(80);
    } 
    else if (newLeftExpressionMeasurement == 5) {
      Serial1.write(0xB0);
      Serial1.write(1);
      Serial1.write(97);
    } 
    else if (newLeftExpressionMeasurement == 6) {
      Serial1.write(0xB0);
      Serial1.write(1);
      Serial1.write(114);
    } 
    else if (newLeftExpressionMeasurement == 7) {
      Serial1.write(0xB0);
      Serial1.write(1);
      Serial1.write(127);
    } 
    leftExpressionPedal = newLeftExpressionMeasurement;
  }
  
  int newRightExpressionMeasurement;
  int rightExpressionMeasurement = analogRead(rightExpressionPin);
  if (rightExpressionMeasurement < 340) {
    newRightExpressionMeasurement = 0;
  }
  else if (rightExpressionMeasurement < 390) {
    newRightExpressionMeasurement = 1;
  }
  else if (rightExpressionMeasurement < 440) {
    newRightExpressionMeasurement = 2;
  }
  else if (rightExpressionMeasurement < 490) {
    newRightExpressionMeasurement = 3;
  }
  else if (rightExpressionMeasurement < 540) {
    newRightExpressionMeasurement = 4;
  }
  else if (rightExpressionMeasurement < 590) {
    newRightExpressionMeasurement = 5;
  }
  else if (rightExpressionMeasurement < 639) {
    newRightExpressionMeasurement = 6;
  }
  else {
    newRightExpressionMeasurement = 7;
  }
  
  if (newRightExpressionMeasurement != rightExpressionPedal) {
    if (newRightExpressionMeasurement == 0) {
      Serial1.write(0xB0);
      Serial1.write(2);
      Serial1.write(0);
    } 
    else if (newRightExpressionMeasurement == 1) {
      Serial1.write(0xB0);
      Serial1.write(2);
      Serial1.write(21);
    } 
    else if (newRightExpressionMeasurement == 2) {
      Serial1.write(0xB0);
      Serial1.write(2);
      Serial1.write(42);
    }
   else if (newRightExpressionMeasurement == 3) {
      Serial1.write(0xB0);
      Serial1.write(2);
      Serial1.write(63);
    }  
    else if (newRightExpressionMeasurement == 4) {
      Serial1.write(0xB0);
      Serial1.write(2);
      Serial1.write(80);
    } 
    else if (newRightExpressionMeasurement == 5) {
      Serial1.write(0xB0);
      Serial1.write(2);
      Serial1.write(97);
    } 
    else if (newRightExpressionMeasurement == 6) {
      Serial1.write(0xB0);
      Serial1.write(2);
      Serial1.write(114);
    } 
    else if (newRightExpressionMeasurement == 7) {
      Serial1.write(0xB0);
      Serial1.write(2);
      Serial1.write(127);
    } 
    rightExpressionPedal = newRightExpressionMeasurement;
  }
  
  
  queryKeyboard(1, 1);
  queryKeyboard(2, 2);
  queryKeyboard(3, 3);
}

void localNoteOn(int noteNum)
{
  Serial.println("Local note on!");
  Serial1.write(NOTE_ON_CMD);
  Serial1.write(keyToMidiMap[noteNum]);
  Serial1.write(NOTE_VELOCITY);
}

void localNoteOff(int noteNum)
{
  Serial.println("Local note off!");
  Serial1.write(NOTE_OFF_CMD);
  Serial1.write(keyToMidiMap[noteNum]);
  Serial1.write(NOTE_VELOCITY);
}

byte responseBuffer [30];

//queryKeyboard takes what slave address you want to query and 
// what MIDI channel you want to send results of the query to. 
void queryKeyboard(int slaveAddress, int midiChannel) 
{
  Wire.beginTransmission (slaveAddress);
  Wire.write (66);  // do something
  Wire.endTransmission ();
  
  memset (responseBuffer, 0, sizeof responseBuffer);
  
  if (!Wire.requestFrom (slaveAddress, sizeof responseBuffer))
  {
    Serial.println (F("No length from slave"));
    return;  
  }

  byte count = Wire.read ();
  //Serial.print (F("Got size of "));
  //Serial.print (int (count));
  //Serial.println (F(" bytes."));
  // don't bother if nothing to read
  if (count == 0)
    return;
    
  if (!Wire.requestFrom (slaveAddress, size_t (count)))
    {
    Serial.println (F("No data from slave"));
    return;  
    }
  
  for (int i = 0; i < count; i++)
  {
    responseBuffer [i] = Wire.read ();
  }
  for (int i = 0; i < count; i++) 
  {
    rawDataToMidi(responseBuffer[i], midiChannel);
  }
}

//takes the incomingKeyData and sends the correct information to the MIDI port
void rawDataToMidi(int rawKeyValue, int midiChannel) 
{
  // Set up MIDI serial commands.
  // The last number is the channel number. If you wanted to send a channel 2 note on command,
  // it would be 0x91 (and the off command would be changed as well). Remember to count from 0.
  int noteOnCmd = ((0x90) + (midiChannel - 1));
  int noteOffCmd = ((0x80) + (midiChannel - 1));
  if (isItOn(rawKeyValue)) 
  {
    //note on debugging info (can be deleted when done)
    Serial.print("Sending key number ");
    Serial.print(findNoteNumber(rawKeyValue));
    Serial.println(" ON to MIDI!");
    //Sends the required note on information to the MIDI port 
    //NOTE: It's going to Serial1, not Serial
    Serial1.write(noteOnCmd);
    Serial1.write(findNoteNumber(rawKeyValue));
    Serial1.write(NOTE_VELOCITY);
    }
  else 
  {
    //note off debugging info (can be deleted when done)
    Serial.print("Sending key number ");
    Serial.print(findNoteNumber(rawKeyValue));
    Serial.println(" OFF to MIDI.");
    //Sends the required note off information to the MIDI port
    //NOTE: It's going to Serial1, not Serial
    Serial1.write(noteOffCmd);
    Serial1.write(findNoteNumber(rawKeyValue));
    Serial1.write(NOTE_VELOCITY);
  }
}
  
//Find if a note is on or off: we're getting xx (note xx off) and 1xx (note xx on) from the slave board.
//This will find what the digit in the 100s place is and return true if the raw value was
//1xx (on) and false if it's xx (off)
boolean isItOn(int incomingValue)
{
  int noteNumber = findNoteNumber(incomingValue);
  int OnOrOff = incomingValue - noteNumber;
  if (OnOrOff == 100) 
  {
    return true;
  }
  else  
  {
    return false;
  }
}

// This will just return the note number which is the tens and ones position
// of the two or three digit number that we're getting from the slave board.
int findNoteNumber(int incomingValue)
{
  int noteNumber = incomingValue % 100;
  return noteNumber;
}
