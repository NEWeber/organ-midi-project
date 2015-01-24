#include <Wire.h>

#define NOTE_ON_CMD 0x90
#define NOTE_OFF_CMD 0x80
#define NOTE_VELOCITY 127
//MIDI baud rate
#define SERIAL_RATE 31250

void setup()
{
  
  Wire.begin();
  Serial.begin(9600);
  
}

void loop()
{
  //send request for two bytes to slave at address 2 (right now the great keyboard) to get data.
  Wire.requestFrom(1, 2);
  if(Wire.available() == 2) {
    int keyOnAndOff = Wire.read();
    Serial.println("Just got ");
    Serial.print(keyOnAndOff);
    Serial.print(" from the slave board!");
      
    if (findOnOrOff(keyOnAndOff)) {
      Serial.println("Sending key number ");
      Serial.print(findNoteNumber(keyOnAndOff));
      Serial.print(" ON to MIDI!");
      Serial.write(NOTE_ON_CMD);
      Serial.write(findNoteNumber(keyOnAndOff));
      Serial.write(NOTE_VELOCITY);
      //note on stuff
    }
    else {
      //note off stuff
      Serial.println("Sending key number ");
      Serial.print(findNoteNumber(keyOnAndOff));
      Serial.print(" OFF to MIDI.");
      Serial.write(NOTE_OFF_CMD);
      Serial.write(findNoteNumber(keyOnAndOff));
      Serial.write(NOTE_VELOCITY);
    }
  }
  //play with delay value to get desired communication experience.
  delay(1000);
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
  else if (OnOrOff == 200) 
  {
    return false;
  }
  else 
  {
    Serial.println("Somthing is wrong with findOnOrOff!");
  }
}

//This will just return the note number which is the tens and ones position
// of the three digit number that we're getting from the slave board.
int findNoteNumber(int incomingValue)
{
  int noteNumber = incomingValue % 100;
  return noteNumber;
}
