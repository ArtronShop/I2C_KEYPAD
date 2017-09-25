/*
i2c_keypad by ThaiEasyElec.com 
www.thaieasyelec.com
https://github.com/ThaiEasyElec/
support@thaieasyelec.com
*/


#ifndef i2c_keypad_h
#define i2c_keypad_h

#define PCF_ADD 0x20
enum BUTTON_EVENT { PRESS, RELEASE, DO };
class I2CKEYPAD
{
	public:
		I2CKEYPAD();
		void begin(char addr=PCF_ADD);
		void scand(long interval=200);
		void set_event(unsigned char event = PRESS);
		void (*keypadevent)(char k);
		unsigned char event_; 
		unsigned char release_;
	private:
		char addr_;
};







#endif































