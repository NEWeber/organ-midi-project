// Code for the master board. It requests raw key pressed data from the slave boards
// interprets it and then sends it to the MIDI port.

#include <Wire.h>
//MIDI baud rate
const int midiSerialRate = 31250;

void setup()
{
  Wire.begin();
  //Send out MIDI signals out of Serial1, can switch this to serial after done debugging
  Serial1.begin(midiSerialRate);
  //Set up Serial out to send debugging messages to the computer.
  Serial.begin(9600);
}

void loop()
{
  //Query slave at address 1 and output results to MIDI channel 1. (Great Keyboard outputs to MIDI channel 1.)
  queryKeyboard(1, 1);
  // @todo: add the other keyboards here when they're ready
  //Rückpositiv outputs to MIDI channel 2
  //Swell outputs to MIDI channel 3
  //play with delay value to get desired communication experience.
  delay(750);
}

char responseBuffer [30];

//queryKeyboard takes what slave address you want to query and 
// what MIDI channel you want to send results of the query to. 
void queryKeyboard(int slaveAddress, int midiChannel) 
{
  Wire.beginTransmission (slaveAddress);
  Wire.write (66);  // do something
  Wire.endTransmission ();
  
  memset (responseBuffer, 0, sizeof responseBuffer);
  
  if (!Wire.requestFrom (SLAVE_ADDRESS, sizeof responseBuffer))
  {
    Serial.println (F("No length from slave"));
    return;  
  }

  byte count = Wire.read ();
  Serial.print (F("Got size of "));
  Serial.print (int (count));
  Serial.println (F(" bytes."));
  // don't bother if nothing to read
  if (count == 0)
    return;
    
  if (!Wire.requestFrom (SLAVE_ADDRESS, size_t (count)))
    {
    Serial.println (F("No data from slave"));
    return;  
    }
  
  for (int i = 0; i < count; i++)
    responseBuffer [i] = Wire.read ();
  //send request for one byte to the address set in slaveAddress
  int thisManyNotes = sizeof(responseBuffer) / sizeof(responseBuffer[0]);
  Serial.println(thisManyNotes);
  for (int i = 0; i < thisManyNotes; i++) 
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
  const int noteVelocity = 127;
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
    Serial1.write(noteVelocity);
    }
  else 
  {
    //note off debugging info (can be deleted when done)
    Serial.print("Sending key number ");
    Serial.print(findNoteNumber(rawKeyValue));
    Serial.println(" OFF to MIDI.");
    //Sends the required note off information to the MIDI port
    //NOTE: It's going to Serial1, not Serial
    Serial1.write(noteOnCmd);
    Serial1.write(findNoteNumber(rawKeyValue));
    Serial1.write(noteVelocity);
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

