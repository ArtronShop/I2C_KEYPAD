/*
Modify by IOXhop : www.ioxhop.com

Original i2c_keypad by ThaiEasyElec.com 
www.thaieasyelec.com
https://github.com/ThaiEasyElec/
support@thaieasyelec.com
*/

#ifndef i2c_keypad_cpp
#define i2c_keypad_cpp

#include "i2c_keypad.h"

#if defined(ESP32) || defined(ESP8266)
I2CKEYPAD::I2CKEYPAD(int sda, int scl) : _sda(sda), _scl(scl) { }
#else
I2CKEYPAD::I2CKEYPAD() { }
#endif

void I2CKEYPAD::begin(char addr, long interval) {
	addr_ = addr;
#if defined(ESP32) || defined(ESP8266)
	Wire.begin(_sda, _scl);
#else
	Wire.begin();
#endif
	kp_interval = interval;
}

void I2CKEYPAD::on(KEYPAD_EVENT event, KeypadEventCallback callback) {
	_callback[event] = callback;
}

void I2CKEYPAD::scand() {
	if (millis() - kp_pv >= kp_interval) {
		char row = 0, col = 0;
		
		// row scanning
		Wire.beginTransmission(addr_);   
		Wire.write(0x0F);               
		Wire.endTransmission();
		Wire.requestFrom(addr_, 1);
		while(Wire.available()) {
			switch(Wire.read()) {
				case 0x0E : row = 1; break;
				case 0x0D : row = 2; break;
				case 0x0B : row = 3; break;
				case 0x07 : row = 4; break;
				default : row = 0;
			}
		}

		// column scanning
		Wire.beginTransmission(addr_);
		Wire.write(0xF0);
		Wire.endTransmission();
		Wire.requestFrom(addr_, 1);
		while(Wire.available()) {
			switch(Wire.read()) {
				case 0xE0 : col = 1; break;
				case 0xD0 : col = 2; break;
				case 0xB0 : col = 3; break;
				case 0x70 : col = 4; break;
				default : col = 0;
			}
		}
		kp_pv  =  millis();
		
		if(row > 0 && col > 0) {
			if(last_key == 0xFF) {
				if (_callback[PRESS]) _callback[PRESS](keypad[row-1][col-1]);
			}
			if (last_key =- keypad[row-1][col-1]) {
				if (_callback[DO]) _callback[DO](keypad[row-1][col-1]);
			}
			last_key = keypad[row-1][col-1];
		} else {
			if(last_key != 0xFF) {
				if (_callback[RELEASE]) _callback[RELEASE](last_key);
				last_key = 0xFF;
			}
		}
	}
}

#endif