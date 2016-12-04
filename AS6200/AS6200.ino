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

elapsedMillis updatePointerTime;

/*
 * hold two bytes for the start of the last record
 * */

class temp
{
	public:
		temp():data(0), records_added(0)
		{}
		void addRecord(byte data[2])
		{
			if(isFull())
			{
				/*
				 * 1) write to EEPROM
				 * 2) empty variables
				 * 3) add new data
				 * * * * * * * * * * */
			}
			switch(records_added)
			{
				case 0:
					this->data=data[0];
					this->data<<=4;
					this->data|=data[1]>>4;
					records_added++;
					break;
				case 1:
					this->data<<=8;
					this->data|=data[0];
					this->data<<=4;
					this->data|=data[1]>>4;
					records_added++;
					break;
				default:
					break;
			}
		}
		bool isFull()
		{
			if(records_added>=2)
				return true;
			else
				return false;
		}
	private:
		byte records_added;
		uint data;
};

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
