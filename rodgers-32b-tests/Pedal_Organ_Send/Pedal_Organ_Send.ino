#include <QueueArray.h>
#include <Wire.h>
volatile byte numNotesToSend;
// this decides what note the leftmost key will sound
const int lowestNote = 36;
// how many keys
const int numKeys = 61;

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

// @todo: generate this with a constructor
const int pedalNotes[numKeys] = {c2Pin, c2SharpPin, d2Pin, d2SharpPin, e2Pin, f2Pin, f2SharpPin, g2Pin, g2SharpPin, a2Pin, a2SharpPin, b2Pin, c3Pin, c3SharpPin, d3Pin, d3SharpPin, e3Pin, f3Pin, f3SharpPin, g3Pin, g3SharpPin, a3Pin, a3SharpPin, b3Pin, c4Pin, c4SharpPin, d4Pin, d4SharpPin, e4Pin, f4Pin, f4SharpPin, g4Pin};

boolean keyPressed[numKeys];
byte keyToMidiMap[numKeys];

//start the FIFO array with the QueueArray Library to keep track of the data to transmit to the master board.
QueueArray <byte> queue;


void setup()
{
  int note = lowestNote;

  for(int i = 0; i < numKeys; i++)
  {
    keyPressed[i] = false;
    keyToMidiMap[i] = note;
    note++;
  }
  
  //go through all the input pins for the great notes and set them to INPUT_PULLUP, so when it connects to ground it will trigger
  for (int i = 0; i < numKeys; i++) 
  {
    pinMode(pedalNotes[i], INPUT_PULLUP);
  }
  //Start wire transmission ability and assign this slave board to the address 1.
  Wire.begin(4);
  //When the master board asks for a transmission, run the requestEvent function
  Wire.onRequest(requestEvent);  
  Wire.onReceive (receiveEvent);
}


void loop()
{
  for (int noteCounter = 0; noteCounter < numKeys; noteCounter++) {
    //if the key has been pressed and it was not pressed before, send the note on message and set keyPressed to true
    if ((digitalRead(pedalNotes[noteCounter]) == LOW) && (keyPressed[noteCounter] == false))
    {
      keyPressed[noteCounter] = true;
      noteOn(noteCounter);
      delay(1);
    }
    //if the key is released and it was held before, send note off and set keyPressed to false
    else if ((digitalRead(pedalNotes[noteCounter]) == HIGH) && (keyPressed[noteCounter] == true))
    {
      keyPressed[noteCounter] = false;
      noteOff(noteCounter);
      delay(1);
    }
  }
}

void noteOn(int noteNum)
{
  byte sendThis = 100 + (keyToMidiMap[noteNum]);
  queue.enqueue(sendThis);
}

void noteOff(int noteNum)
{
  byte sendThis = (keyToMidiMap[noteNum]);
  queue.enqueue(sendThis);
}

volatile byte command;
volatile bool wantLength;

// called by interrupt service routine when incoming data arrives
void receiveEvent (int howMany)
 {
   command = Wire.read (); 
   wantLength = true;
 }  // end of receiveEvent

//This transmits the pressed notes to the master board  
void requestEvent() 
{
  if (wantLength)
  {
    numNotesToSend = queue.count();
    Wire.write ((byte) (numNotesToSend & 7));
    wantLength = false;
    return;
  }
  byte myResponse [numNotesToSend];
  for(int i = 0; i < numNotesToSend; i++)
  {
    myResponse[i] = queue.dequeue();
  } 
  Wire.write ((const byte *) myResponse, numNotesToSend & 7); 
}
