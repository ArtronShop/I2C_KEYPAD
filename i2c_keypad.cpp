/*
i2c_keypad by ThaiEasyElec.com 
www.thaieasyelec.com
https://github.com/ThaiEasyElec/
support@thaieasyelec.com
*/

#include "i2c_keypad.h"
#include <Arduino.h>
#include <Wire.h>

unsigned long 	kp_pv = 0; 
unsigned char last_key=0xFF;
void key_null(char k){};

I2CKEYPAD::I2CKEYPAD()
{
	keypadevent = key_null;
}
void I2CKEYPAD::begin(char addr)
{
	addr_ = addr;
	Wire.begin();   
	set_event();
}
void I2CKEYPAD::set_event(unsigned char event)
{
	event_ = event;
}
void I2CKEYPAD::scand(long kp_interval)
{
	//kp_interval = interval;
	unsigned char recv;
	char row, col = 0;
	char keypad[4][4] = {{'1','2','3','A'},
						 {'4','5','6','B'},
						 {'7','8','9','C'},
						 {'*','0','#','D'}};
	
	unsigned long kp_current = millis();
	if (kp_current - kp_pv >= kp_interval) 
	{
		Wire.beginTransmission(addr_);   
		Wire.write(0x0F);               
		Wire.endTransmission();
		Wire.requestFrom(addr_, 1);
		while(Wire.available())
		{
			recv = Wire.read();
			switch(recv)
			{
				case 0x0E : row = 1; break;
				case 0x0D : row = 2; break;
				case 0x0B : row = 3; break;
				case 0x07 : row = 4; break;
				default : row = 0;
			}
		}

		//column scanning
		Wire.beginTransmission(addr_);
		Wire.write(0xF0);
		Wire.endTransmission();
		Wire.requestFrom(addr_, 1);
		while(Wire.available())
		{
			recv = 0x00FF&Wire.read();
			switch(recv)
			{
				case 0xE0 : col = 1; break;
				case 0xD0 : col = 2; break;
				case 0xB0 : col = 3; break;
				case 0x70 : col = 4; break;
				default : col = 0;
			}
		}
		kp_pv  =  millis();
		if(row!=0 && col!=0)
		{
			if(event_ ==  PRESS)
			{
				if(last_key != keypad[row-1][col-1])
				(*keypadevent)(keypad[row-1][col-1]);
				last_key = keypad[row-1][col-1];
			}
			if(event_ ==  RELEASE)
			{
				if(last_key != keypad[row-1][col-1])
				{
					release_ = keypad[row-1][col-1];
					last_key = 0xFF;
				}
					
			}
			if(event_ ==  DO)
			{
				(*keypadevent)(keypad[row-1][col-1]);
			}
			
		}
		else
		{
			if(event_ ==  RELEASE)
			{
				if(last_key != release_)
				{
						(*keypadevent)(release_);
						release_ = 0xFF;
						last_key = 0xFF;
				}
				
			}
			else
			last_key = 0xFF;
		}
			 
			
	}
}





