/* configuration register
* address : 0x01
*
* D15 D14 D13 D12 D11 D10 D09 D08 D07 D06 D05 D04 D03 D02 D01 D00
*  RW  RO++RO  RW++RW++RW++RW++RW++RW++RW  RO  RW  RO++RO++RO++RO
*  SS  --++--  CF++CF POL  IM  SM  CR++CR  AL  --++--++--++--++--
*
*	ss - single-shot
* cf - consecutive faults
* pol -polarity (alert)
* im - interrupt mode (comparator/interrupt mode)
* sm - sleep mode
* cr - conversion rate
*      0 0 - 0.25Hz
*      0 1 - 1Hz
*      1 0 - 4Hz (default)
*      1 1 - 8Hz
* al - alert (TLOW THIGH)
* */
/*
 *  constructors
 * * * * * * * * */
/*
 *   functions
 * * * * * * * * */
ushort AS6200::get_register(byte reg_id)
{
	if(reg_id>3)
		reg_id=0;
	Wire.beginTransmission(device_address);
	Wire.write(reg_id);
	Wire.endTransmission();
	Wire.requestFrom(device_address,2);
	ushort data;
	if(Wire.available())
		data=Wire.read();
	data<<=8;
	if(Wire.available())
		data|=Wire.read();
	Wire.beginTransmission(device_address);
	Wire.write((byte) 0);// prepare to read TVAL
	Wire.endTransmission();
	return data;
}
ushort AS6200::get_temperature(void)
{
	ushort data;
	Wire.requestFrom(device_address,2);
	if(Wire.available())
		data=Wire.read();
	data<<=8;
	if(Wire.available())
		data|=Wire.read();
//	data>>=4;
	last_temperature=data;
	return data;
}
double AS6200::get_temperature_human_readable(void)
{
	get_temperature();
	double temperature;
	temperature=last_temperature>>8;
	temperature+=(last_temperature&0xFF)*0.0625;
//	temperature=last_temperature*0.0625;
	return temperature;
}
void AS6200::set_single_shot_mode(void)
{
	Wire.beginTransmission(device_address);
	Wire.write((byte) 1);//CONFIG
	Wire.write((byte) 0x81);// SS, SM
	Wire.write((byte) 0x00);
	Wire.endTransmission();
	Wire.beginTransmission(device_address);
	Wire.write((byte) 0);// prepare to read TVAL
	Wire.endTransmission();
}
void AS6200::set_continuous_mode(void)
{
	Wire.beginTransmission(device_address);
	Wire.write((byte) 1);//CONFIG
	Wire.write((byte) 0x00);
	Wire.write((byte) 0x00);// T=4sec
	Wire.endTransmission();
	Wire.beginTransmission(device_address);
	Wire.write((byte) 0);// prepare to read TVAL
	Wire.endTransmission();
}
