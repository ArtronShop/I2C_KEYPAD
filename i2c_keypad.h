/*
Modify by IOXhop : www.ioxhop.com

Original i2c_keypad by ThaiEasyElec.com 
www.thaieasyelec.com
https://github.com/ThaiEasyElec/
support@thaieasyelec.com
*/

#ifndef i2c_keypad_h
#define i2c_keypad_h

#include <Arduino.h>
#include <Wire.h>

#define PCF_ADD 0x20

typedef enum { 
	PRESS, 
	RELEASE, 
	DO 
} KEYPAD_EVENT;

typedef void(*KeypadEventCallback)(char);

class I2CKEYPAD {
	public:
#if defined(ESP32) || defined(ESP8266)
		I2CKEYPAD(int sda = -1, int scl = -1) ; 
#else
		I2CKEYPAD() ;
#endif

		void begin(char addr = PCF_ADD, long interval = 200) ;
		void scand() ;
		void on(KEYPAD_EVENT event, KeypadEventCallback callback) ;
		
	private:
#if defined(ESP32) || defined(ESP8266)
		int _sda = -1, _scl = -1;
#endif
		char addr_ = PCF_ADD;
		int kp_interval = 200;
		unsigned long kp_pv = 0; 
		unsigned char last_key=0xFF;
		
		KeypadEventCallback _callback[3] = { NULL, NULL, NULL };
		
		char keypad[4][4] = {
								{'1', '2', '3', 'A'},
								{'4', '5', '6', 'B'},
								{'7', '8', '9', 'C'},
								{'*', '0', '#', 'D'}
							};
						 
};

#endif
