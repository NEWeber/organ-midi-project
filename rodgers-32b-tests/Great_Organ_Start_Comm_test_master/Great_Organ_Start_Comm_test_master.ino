#include <Wire.h>

#define NOTE_ON_CMD 0x90
#define NOTE_OFF_CMD 0x80
#define NOTE_VELOCITY 127
//MIDI baud rate
#define SERIAL_RATE 31250

void setup()
{
  
  Wire.begin();
  Serial.begin(SERIAL_RATE);
  
}

void loop()
{

  Wire.requestFrom(1, 4);
  int keyOnAndOff = Wire.read();
  int nDigits = floor(log10(abs(keyOnAndOff))) + 1;
  if (nDigits == 3) {
    //do this if it's on/off + 2 digit key
    if (getDigit(keyOnAndOff, 3) == 1) {
      Serial.write(NOTE_ON_CMD);
      Serial.write((keyOnAndOff % 100));
      Serial.write(NOTE_VELOCITY);
      //note on stuff
    }
    else {
      //note off stuff
      Serial.write(NOTE_OFF_CMD);
      Serial.write((keyOnAndOff % 100));
      Serial.write(NOTE_VELOCITY);
    }
    
  }
  else {
   //do this if it's on/off + 3 digit key 
       if (getDigit(keyOnAndOff, 4) == 1) {
      Serial.write(NOTE_ON_CMD);
      Serial.write((keyOnAndOff % 1000));
      Serial.write(NOTE_VELOCITY);
      //note on stuff
    }
    else {
      //note off stuff
      Serial.write(NOTE_OFF_CMD);
      Serial.write((keyOnAndOff % 1000));
      Serial.write(NOTE_VELOCITY);
    }
    

  }
  
  
}

int getDigit(int value, int positionFromLeft)
{
    int posFromRight = 1;
    {
        int v = value;
        while (v /= 10)
            ++posFromRight;
    }
    posFromRight -= positionFromLeft - 1;
    while (--posFromRight)
        value /= 10;
    value %= 10;
    return value > 0 ? value : -value;
}
