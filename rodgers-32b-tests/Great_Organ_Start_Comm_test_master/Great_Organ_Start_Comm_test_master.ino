#include <Wire.h>

#define NOTE_ON_CMD 0x90
#define NOTE_OFF_CMD 0x80
#define NOTE_VELOCITY 127
//MIDI baud rate
#define SERIAL_RATE 31250

void setup()
{
  
  Wire.begin();
  Serial1.begin(SERIAL_RATE);
  Serial.begin(9600);
  
}

void loop()
{
  queryKeyboard(1, 1);
  //play with delay value to get desired communication experience.
  delay(5);
}
  
//Find if a note is on or off: we're getting xx (note xx off) and 1xx (note xx on) from the slave board.
//This will find what the digit in the 100s place is and return true if it's
//1xx (on) and false if it's xx (off)
boolean findOnOrOff(int incomingValue)
{
  int noteNumber = incomingValue % 100;
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

//queryKeyboard takes what slave address you want to query and 
// what MIDI channel you want to send the query to. 
void queryKeyboard(int midiChannel, int slaveAddress) {
  int noteOnCmd = ((0x90) + (midiChannel - 1));
  int noteOffCmd = ((0x80) + (midiChannel - 1));
 //send request for two bytes to the address set in slaveAddress
  Wire.requestFrom(slaveAddress, 2);
  if(Wire.available() == 2) {
    int keyOnAndOff = Wire.read();
    Serial.print("Just got ");
    Serial.print(keyOnAndOff);
    Serial.println(" from the slave board!");
      
    if (findOnOrOff(keyOnAndOff)) {
      Serial.print("Sending key number ");
      Serial.print(findNoteNumber(keyOnAndOff));
      Serial.println(" ON to MIDI!");
      Serial1.write(noteOnCmd);
      Serial1.write(findNoteNumber(keyOnAndOff));
      Serial1.write(NOTE_VELOCITY);
      //note on stuff
    }
    else {
      //note off stuff
      Serial.print("Sending key number ");
      Serial.print(findNoteNumber(keyOnAndOff));
      Serial.println(" OFF to MIDI.");
      Serial1.write(noteOnCmd);
      Serial1.write(findNoteNumber(keyOnAndOff));
      Serial1.write(NOTE_VELOCITY);
    }
    
  }
  
}
