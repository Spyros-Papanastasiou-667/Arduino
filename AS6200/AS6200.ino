#include	<Wire.h>
#include "header.h"
#include "Functions.h"
//#include	"AT24C256_Functions.h"
#include "EEPROM.h"
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

/*
 * hold two bytes for the last record
 * */
class pointer{
	public:
		/*
		 *   constructor(s)
		 * * * * * * * * * * */
		pointer():current_address(0),at24c256b(AT24C256_ADDR){}
		/*
		 *   functions
		 * * * * * * * * */
		void append_to_EEPROM(ushort data)
		{
			if(current_address+2>0x7FFF)
				return;
			byte tmp_data;
			tmp_data=data>>8;
			at24c256b.write_byte(tmp_data);
//			Serial << "written :: " << tmp_data << " , " ;
			tmp_data=data&0xFF;
			at24c256b.write_byte(tmp_data);
//			Serial << tmp_data << endl;
			current_address+=2;
		}
		void initialize_EEPROM(void)
		{
			at24c256b.set_current_address(0);
			update_pointer_on_EEPROM();// set to 0 (zero)
		}
		void update_pointer_on_EEPROM(void)
		{
			ushort old_address=at24c256b.get_current_address();
			at24c256b.write_byte((current_address&0xFF00)>>8,0);
			at24c256b.write_byte(current_address&0xFF,1);
			at24c256b.set_current_address(old_address);
		}
		void dump_data_csv(void)
		{
			ushort old_address=at24c256b.read_byte(0);
			old_address<<=8;
			old_address|=at24c256b.read_byte();
			uint i=2;
			Serial << "printing data from EEPROM ##########" << endl;
			while(i<=old_address)
			{
				ushort data;
				Serial << i << ",";
				i+=2;
				data=at24c256b.read_byte();
				data<<=8;
				data|=at24c256b.read_byte();
//				data>>=4;
//				data_double=data*0.0625;
				Serial.println(data*0.0625);
			}
			Serial << "end of data               ##########" << endl;
		}
		void dump_data_hex(void)
		{// CHECK copy code from csv
			ushort old_address=at24c256b.read_byte(0);
			old_address<<=8;
			old_address|=at24c256b.read_byte();
			uint i=0;
			Serial << "printing data from EEPROM #######" << endl;
			while(i<old_address)
			{
				byte j=0;
				byte data;
				Serial << "0x" << _HEX(i) << " \t:: 0x";
				while(j++<32 && i++<old_address)
				{
					data=at24c256b.read_byte();
					if(data&0xF0==0)
						Serial << "0";
					else
						Serial << _HEX(data>>4);
					if(data&0x0F==0)
						Serial << "0";
					else
						Serial << _HEX(data&0x0F);
					if(j!=32 && j%4==0)
						Serial << " 0x";
				}
				Serial << endl;
			}
			Serial << "end of data $$$$$$$" << endl;
		}
	private:
		EEPROM at24c256b;
		ushort current_address;
};

pointer pointer_on_EEPROM;

void setup()
{
	Wire.begin();
	Serial.begin(115200);
//	while(!Serial);
	thermometer.set_single_shot_mode();
	thermometer.get_temperature();
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

void loop()
{
	if(millis()-temperature_interval> 60000)// FIX does not work with 60*1000
	{
		temperature_interval=millis();
		ushort temperature=thermometer.get_temperature();
		if(Serial) Serial << "temperature :: " << temperature*0.0625 << "°C" << endl;
		pointer_on_EEPROM.append_to_EEPROM(temperature);
//		ushort addr=pointer_on_EEPROM.get_current_address();
//		Serial << "current address :: " << addr << endl;
		pointer_on_EEPROM.update_pointer_on_EEPROM();
		digitalWrite(13,HIGH);
		delay(50);
		digitalWrite(13,LOW);
	}
//	if(millis()-EEPROM_interval>60000)
//	{
//		EEPROM_interval=millis();
//		pointer_on_EEPROM.update_pointer_on_EEPROM();
//	}
}
