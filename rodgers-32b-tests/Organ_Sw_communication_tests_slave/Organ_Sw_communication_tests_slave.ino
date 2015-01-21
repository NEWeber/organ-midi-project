#include <Wire.h>

const int highCPin = 2;  // pushbutton 1 pin

void setup()
{

  Wire.begin(1);
  Wire.onRequest(findCState);
  // Set up the pushbutton pins to be an input:
  pinMode(highCPin, INPUT_PULLUP);

}


void loop()
{
    // variable to hold the pushbutton states 
}

void findCState() 
{
  int highCState = digitalRead(highCPin);
  if (highCState == LOW)   // if high c is connecting the circuit to ground (making it LOW)   
  {
    Wire.write(1);  // turn the LED on
  }
  else
  {
    Wire.write(0);  // turn the LED off
  }
} 
