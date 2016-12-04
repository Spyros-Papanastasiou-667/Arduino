/*
 * hold two bytes on the start of EEPROM for the last record
 * */
/*
 *   constructor(s)
 * * * * * * * * * * */
/*
 *   functions
 * * * * * * * * */
void pointer::append_to_EEPROM(ushort data)
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
void pointer::initialize_EEPROM(void)
{
	at24c256b.set_current_address(0);
	update_pointer_on_EEPROM();// set to 0 (zero)
}
void pointer::update_pointer_on_EEPROM(void)
{
	ushort old_address=at24c256b.get_current_address();
	at24c256b.write_byte((current_address&0xFF00)>>8,0);
	at24c256b.write_byte(current_address&0xFF,1);
	at24c256b.set_current_address(old_address);
}
void pointer::dump_data_csv(void)
{
	ushort old_address=at24c256b.read_byte(0);
	old_address<<=8;
	old_address|=at24c256b.read_byte();
	uint i=2;
	Serial << "DUMP (printing data from EEPROM) ##########" << endl << endl << endl;
	Serial << "min,°C" << endl;
	while(i<=old_address)
	{
		ushort data;
		Serial << i/2 << ",";
		i+=2;
		data=at24c256b.read_byte();
		data<<=8;
		data|=at24c256b.read_byte();
//				data>>=4;
//				data_double=data*0.0625;
		Serial.println(data*0.0625);
	}
	Serial << endl << endl << "DUMP (end of data)               ##########" << endl;
}
void pointer::dump_data_hex(void)
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
