#include	<Wire.h>
#include "header.h"
#include "Functions.h"
//#include	"AT24C256_Functions.h"
#include "EEPROM.h"
#include	"AT24C256_Pointer.h"
#include "AS6200_class.h"
#include "Streaming.h"
#include <elapsedMillis.h>

#define UPDATE_POINTER_INTERVAL 5000// ms

AS6200 thermometer;

pointer pointer_on_EEPROM;

void setup()
{
	Wire.begin();
	Serial.begin(115200);
//	while(!Serial);
	thermometer.set_single_shot_mode();
//	thermometer.get_temperature();
	for(byte i=0;i<30;i++)
	{
		digitalWrite(13,HIGH);
		delay(50);
		digitalWrite(13,LOW);
		delay(950);
	}
	if(Serial) pointer_on_EEPROM.dump_data_csv();
	pointer_on_EEPROM.initialize_EEPROM();
//	pointer.getAddressFromEEPROM();
	delay(40);
}

//elapsedMillis temperature_interval;
//elapsedMillis EEPROM_interval;

ulong temperature_interval;
ulong EEPROM_interval;
ulong regular_print_interval;

void loop()
{
	double temperature;
	if(millis()-temperature_interval> 60000)// FIX does not work with 60*1000
	{
		temperature_interval=millis();
		temperature=thermometer.get_temperature_human_readable();
		thermometer.set_single_shot_mode();
		if(Serial) Serial << temperature << "°C (appended to EEPROM)" << endl;
		pointer_on_EEPROM.append_to_EEPROM(temperature);
//		ushort addr=pointer_on_EEPROM.get_current_address();
//		Serial << "current address :: " << addr << endl;
		pointer_on_EEPROM.update_pointer_on_EEPROM();
		digitalWrite(13,HIGH);
		delay(50);
		digitalWrite(13,LOW);
	}
	if(millis()-regular_print_interval>1300 && Serial)
	{
		regular_print_interval=millis();
		temperature=thermometer.get_temperature_human_readable();
		thermometer.set_single_shot_mode();
		Serial << temperature << "°C" << endl;
	}
//	if(millis()-EEPROM_interval>60000)
//	{
//		EEPROM_interval=millis();
//		pointer_on_EEPROM.update_pointer_on_EEPROM();
//	}
}
