#include "i2c_keypad.h"
I2CKEYPAD keypad;
void setup() 
{
  Serial.begin(9600);
  keypad.begin();
  keypad.set_event(PRESS); // PRESS, RELEASE, DO
  keypad.keypadevent = keyevent;
  
}
void loop() 
{
  keypad.scand(100); //scan time(ms)
}
void keyevent(char key)
{
  Serial.println(key);
}
