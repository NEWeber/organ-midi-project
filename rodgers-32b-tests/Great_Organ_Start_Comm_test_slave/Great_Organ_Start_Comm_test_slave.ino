#include <QueueArray.h>
#include <Wire.h>
// this decides what note the leftmost key will sound
const int lowestNote = 36;
// how many keys
const int numKeys = 61;

// @todo: generate this with a constructor
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
const int g4SharpPin = 37;  
const int a4Pin = 36;
const int a4SharpPin = 35;
const int b4Pin = 34;
const int c5Pin = 33;  
const int c5SharpPin = 32;
const int d5Pin = 31;
const int d5SharpPin =  30;
const int e5Pin = 29;  
const int f5Pin = 28;
const int f5SharpPin = 27;
const int g5Pin =  26;
const int g5SharpPin = 25;  
const int a5Pin = 24;
const int a5SharpPin = 23;
const int b5Pin = 22;
//note the change here
const int c6Pin = 15;  
const int c6SharpPin = 14;
const int d6Pin = 2;
const int d6SharpPin =  3;
const int e6Pin = 4;  
const int f6Pin = 5;
const int f6SharpPin = 6;
const int g6Pin = 7;
const int g6SharpPin = 8;  
const int a6Pin = 9;
const int a6SharpPin = 10;
const int b6Pin = 11;
const int c7Pin = 12;  


// @todo: generate this with a constructor
const int greatNotes[numKeys] = {c2Pin, c2SharpPin, d2Pin, d2SharpPin, e2Pin, f2Pin, f2SharpPin, g2Pin, g2SharpPin, a2Pin, a2SharpPin, b2Pin, c3Pin, c3SharpPin, d3Pin, d3SharpPin, e3Pin, f3Pin, f3SharpPin, g3Pin, g3SharpPin, a3Pin, a3SharpPin, b3Pin, c4Pin, c4SharpPin, d4Pin, d4SharpPin, e4Pin, f4Pin, f4SharpPin, g4Pin, g4SharpPin, a4Pin, a4SharpPin, b4Pin, c5Pin, c5SharpPin, d5Pin, d5SharpPin, e5Pin, f5Pin, f5SharpPin, g5Pin, g5SharpPin, a5Pin, a5SharpPin, b5Pin, c6Pin, c6SharpPin, d6Pin, d6SharpPin, e6Pin, f6Pin, f6SharpPin, g6Pin, g6SharpPin, a6Pin, a6SharpPin, b6Pin, c7Pin};

boolean keyPressed[numKeys];
byte keyToMidiMap[numKeys];

//start the FIFO array with the QueueArray Library to keep track of the data to transmit to the master board.
QueueArray <int> queue;


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
  for (int i = 0; i < numKeys; i++) {
    pinMode(greatNotes[i], INPUT_PULLUP);
  }
  //Start wire transmission ability and assign this slave board to the address 1.
  Wire.begin(1);
  //When the master board asks for a transmission, run the getPressedNotes function
  Wire.onRequest(requestEvent);  
  Serial.begin(9600);
}


void loop()
{
  for (int noteCounter = 0; noteCounter < numKeys; noteCounter++) {
    //if the key has been pressed and it was not pressed before, send the note on message and set keyPressed to true
    if ((digitalRead(greatNotes[noteCounter]) == LOW) && (keyPressed[noteCounter] == false))
    {
      Serial.println("A note was pressed!");
      keyPressed[noteCounter] = true;
      noteOn(noteCounter);
    }
    //if the key is released and it was held before, send note off and set keyPressed to false
    else if ((digitalRead(greatNotes[noteCounter]) == HIGH) && (keyPressed[noteCounter] == true))
    {
      Serial.println("A note was released.");
      keyPressed[noteCounter] = false;
      noteOff(noteCounter);
    }
  }

delay(10);
}

void noteOn(int noteNum)
{
  int sendThis = 100 + (keyToMidiMap[noteNum]);
  queue.enqueue(sendThis);
  Serial.println(sendThis);
  //Serial.println(queue.front());
}

void noteOff(int noteNum)
{
  int sendThis = (keyToMidiMap[noteNum]);
  queue.enqueue(sendThis);
  Serial.println(sendThis);
  //Serial.println(queue.front());
}

//This transmits the pressed notes to the master board  
void requestEvent() 
{
  //if there's something in the queue, send it to the master board
  if (!queue.isEmpty()) {
    Serial.println("sending notes");
    Wire.write(queue.dequeue());
  }
}  


