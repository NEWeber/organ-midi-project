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
  //send request for two bytes to slave at address 2 (right now the great keyboard) to get data.
  Wire.requestFrom(1, 2);
  if(Wire.available() == 2) {
    int keyOnAndOff = Wire.read();
    if(keyOnAndOff == 1) {
    }
    else {
      Serial.print("Just got ");
      Serial.print(keyOnAndOff);
      Serial.println(" from the slave board!");
        
      if (findOnOrOff(keyOnAndOff)) {
        Serial.print("Sending key number ");
        Serial.print(findNoteNumber(keyOnAndOff));
        Serial.println(" ON to MIDI!");
        Serial1.write(NOTE_ON_CMD);
        Serial1.write(findNoteNumber(keyOnAndOff));
        Serial1.write(NOTE_VELOCITY);
        //note on stuff
      }
      else {
        //note off stuff
        Serial.print("Sending key number ");
        Serial.print(findNoteNumber(keyOnAndOff));
        Serial.println(" OFF to MIDI.");
        Serial1.write(NOTE_OFF_CMD);
        Serial1.write(findNoteNumber(keyOnAndOff));
        Serial1.write(NOTE_VELOCITY);
      }
    }
  }
  //play with delay value to get desired communication experience.
  delay(19);
}
  
//Find if a note is on or off: we're getting 1xx and 2xx from the slave board.
//This will find what the digit in the 100s place is and return true if it's
//1xx (on) and false if it's 2xx (off)
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
// of the three digit number that we're getting from the slave board.
int findNoteNumber(int incomingValue)
{
  int noteNumber = incomingValue % 100;
  return noteNumber;
}
