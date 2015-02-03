#include <Wire.h>

#define NOTE_ON_CMD 0x90
#define NOTE_OFF_CMD 0x80
#define NOTE_VELOCITY 127
//MIDI baud rate
#define SERIAL_RATE 31250

void setup()
{
  
  Wire.begin();
  //Send out MIDI signals out of Serial1, can switch this to serial after done debugging
  Serial1.begin(SERIAL_RATE);
  //Set up Serial out to send debugging messages to the computer.
  Serial.begin(9600);
  
}

void loop()
{
  queryKeyboard(1, 1);
  //play with delay value to get desired communication experience.
  delay(5);
}

//queryKeyboard takes what slave address you want to query and 
// what MIDI channel you want to send the query to. 
void queryKeyboard(int midiChannel, int slaveAddress) {

 //send request for two bytes to the address set in slaveAddress
  Wire.requestFrom(slaveAddress, 2);
  if(Wire.available() == 2) {
    //read incoming value and assign it to incomingKeyData
    int incomingKeyData = Wire.read();
    //debugging info, can be deleted when done
    Serial.print("Just got ");
    Serial.print(incomingKeyData);
    Serial.println(" from the slave board!");
    //take incomingKeyData and send the right info to the MIDI port
    rawDataToMidi(midiChannel, incomingKeyData);
  }
}

//takes the incomingKeyData and sends the correct information to the MIDI port
void rawDataToMidi(int midiChannel, int rawKeyValue) 
{
  int noteOnCmd = ((0x90) + (midiChannel - 1));
  int noteOffCmd = ((0x80) + (midiChannel - 1));
  if (isItOn(rawKeyValue)) {
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
    else {
      //note off debugging info (can be deleted when done)
      Serial.print("Sending key number ");
      Serial.print(findNoteNumber(rawKeyValue));
      Serial.println(" OFF to MIDI.");
      //Sends the required note off information to the MIDI port
      //NOTE: It's going to Serial1, not Serial
      Serial1.write(noteOnCmd);
      Serial1.write(findNoteNumber(rawKeyValue));
      Serial1.write(NOTE_VELOCITY);
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
  
  //This will just return the note number which is the tens and ones position
  // of the two or three digit number that we're getting from the slave board.
  int findNoteNumber(int incomingValue)
  {
    int noteNumber = incomingValue % 100;
    return noteNumber;
  }
}
