#include <Wire.h>

const int highCLedPin =  22;    // LED pin

void setup()
{
  
  Wire.begin();
  
  // Set up the pushbutton pins to be an input:

  // Set up the LED pin to be an output:
  pinMode(highCLedPin, OUTPUT);      
}

void loop()
{
  
  int highCLedState;
  
  Wire.requestFrom(1, 1);
  
  while(Wire.available()) {
  
    highCLedState = Wire.read();
    if (highCLedState == 1)
    {
      digitalWrite(highCLedPin, HIGH); 
    }
    else {
      digitalWrite(highCLedPin, LOW); 
    }    
  }

}
