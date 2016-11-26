#include	<Wire.h>
#include "header.h"
#include "Functions.h"
#include	"AT24C256_Functions.h"
#include "Streaming.h"
#include <elapsedMillis.h>

#define UPDATE_POINTER_INTERVAL 5000// ms

/*
 * hold two bytes for the start of the last record
 * */
class EEPROMaddress
{
	public:
		void getAddressFromEEPROM(void)
		{
			EEPROMReadPage(0,2);
			byte i=0;
			while(Wire.available() && i<2)
			{
				b[i++]=Wire.read();
			}
		}
		inline void updateAddress(void)
		{
			if(updatePointerTime>UPDATE_POINTER_INTERVAL)
			{
				EEPROMWritePage(0,b,2);
				updatePointerTime=0;
			}
		}
		EEPROMaddress& operator = (ushort address)
		{
			this->address=address;
			return *this;
		}
		EEPROMaddress& operator ++(void)
		{
			address+=3;
		}
	private:
		ushort giveAddress(void)
		{
			return address;
		};
		union
		{
			byte b[2];
			ushort address;
		};
}pointer;

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

void setup()
{
	Wire.begin();
	pointer.getAddressFromEEPROM();
	Serial.begin(115200);
	prepareThermometerForReadValue();
	delay(40);
	takeTemperature();
}

void prepareThermometerForReadValue(void)
{
	Wire.beginTransmission(AS6200_ADDR);
	Wire.write((byte) 1);//CONFIG
	Wire.write((byte) 0x90);// SS, SM
	Wire.endTransmission();
	Wire.beginTransmission(AS6200_ADDR);
	Wire.write((byte) 0);// prepare to read TVAL
	Wire.endTransmission();
}

elapsedMillis getTempTime;
elapsedMillis updatePointerTime;

byte t[2]={0};
void takeTemperature(void)
{
	Wire.requestFrom(AS6200_ADDR,2);
	byte i=0;
	while(Wire.available() && i<2)
		t[i++]=Wire.read();// get temperature
}

double min=255,max=0;
double scale;
double temperature;
void loop()
{
	if(getTempTime> 1300)
	{
		takeTemperature();
		getTempTime=0;
		prepareThermometerForReadValue();
		temperature=(((t[0]<<8)|t[1])>>4)*0.0625;
		if(temperature+40>max)
			max=temperature+40;
		if(temperature+40<min)
			min=temperature+40;
		if(max!=min)
			scale=255*(temperature+40-min)/(max-min);
		analogWrite(13,scale);
		Serial << _FLOAT(temperature,4) << " ~ " << max-40 << " - " << min-40 << " : " << scale << endl;
	}
}
