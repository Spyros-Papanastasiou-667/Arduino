#include	<Wire.h>	// http://www.arduino.cc/en/Reference/Wire
#include "header.h"
#include "EEPROM.h"
#include "AT24C256_Pointer.h"
#include "AS6200_class.h"
#include <Streaming.h>	// http://arduiniana.org/libraries/streaming/

#define UPDATE_POINTER_INTERVAL 5000// ms

AS6200 thermometer;

pointer pointer_on_EEPROM;// please call pointer_on_EEPROM.initialize()\
in setup, it seems i cannot call constructor (read/write to I2C EEPROM) without hanging the Leonardo

const byte interrupt_pin=7;// Leonardo pins:: 0,1,2,3,7 (https://www.arduino.cc/en/Reference/AttachInterrupt)\
pins 2,3 are I2C (https://www.arduino.cc/en/Hacking/PinMapping32u4)\
pins 0,1 are Serial1
volatile bool flag_reset_pointer_on_EEPROM=false;
void setup()
{
	for(byte i=0;i<10;i++)
	{
		digitalWrite(13,HIGH);
		delay(50);
		digitalWrite(13,LOW);
		delay(950);
	}
	// initialize I2C, Serial communication (USB)
	Wire.begin();
	Serial.begin(115200);
	// __________ ___, ______ _____________ (___)
	// interrupt
	pinMode(interrupt_pin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(interrupt_pin), set_flag_reset_pointer_on_EEPROM, LOW);
	// _________
	// constructors
	pointer_on_EEPROM.initialize(); // called instead of constructor
	// ____________
	// blink for 10 sec to wait to dump last data
/*
	for(byte i=0;i<10;i++)
	{
		digitalWrite(13,HIGH);
		delay(50);
		digitalWrite(13,LOW);
		delay(950);
	}
*/
	if(Serial) pointer_on_EEPROM.dump_data_csv();
	delay(40);
	// _____ ___ __ ___ __ ___ _____ __ ____ ____ ____
	// initialize thermometer (AS6200)
	thermometer.set_single_shot_mode();
	delay(30);
	// __________ ___________ (______)
}

void set_flag_reset_pointer_on_EEPROM(void)
{
	flag_reset_pointer_on_EEPROM=true;
}

//elapsedMillis temperature_interval;
//elapsedMillis EEPROM_interval;

ulong temperature_interval;
ulong EEPROM_interval;
ulong regular_print_interval;

void loop()
{
///*
	ushort temperature;
	if(millis()-temperature_interval> 600000)// FIX does not work with 60*1000
	{
		temperature_interval=millis();
		temperature=thermometer.get_temperature();
		double temperature_human_readable=(temperature>>8)+((temperature>>4)&0x0F)*0.0625;
		if(Serial) Serial << temperature_human_readable << "°C (appended to EEPROM)" << endl;
		pointer_on_EEPROM.append_to_EEPROM(temperature);
//		ushort addr=pointer_on_EEPROM.get_current_address();
//		Serial << "current address :: " << addr << endl;
		pointer_on_EEPROM.update_pointer_on_EEPROM();
		digitalWrite(13,HIGH);
		delay(50);
		digitalWrite(13,LOW);
		thermometer.set_single_shot_mode();
	}
	if(millis()-regular_print_interval>10000 && Serial)
	{
		regular_print_interval=millis();
		double temperature_human_readable=thermometer.get_temperature_human_readable();
		Serial << temperature_human_readable << "°C" << endl;
		thermometer.set_single_shot_mode();
	}
	if(flag_reset_pointer_on_EEPROM)
	{
		flag_reset_pointer_on_EEPROM=false;
		digitalWrite(13,HIGH);
		delay(50);
		digitalWrite(13,LOW);
		if(Serial) pointer_on_EEPROM.dump_data_csv();
		pointer_on_EEPROM.initialize_EEPROM_pointer_to_zero();
	}
//	if(millis()-EEPROM_interval>60000)
//	{
//		EEPROM_interval=millis();
//		pointer_on_EEPROM.update_pointer_on_EEPROM();
//	}
//*/
}
