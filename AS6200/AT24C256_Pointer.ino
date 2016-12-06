/*
 * hold two bytes on the start of EEPROM for the last record
 * */
/*
 *   constructor(s)
 * * * * * * * * * * */
/*
pointer::pointer():at24c256b(AT24C256_ADDR)
{
	// get last pointer, and continue from that
	current_address=at24c256b.read_byte(0);
	current_address<<=8;
	current_address|=at24c256b.read_byte();
	// ___ ____ _______, ___ ________ ____ ____
	at24c256b.set_current_address(current_address);
}
*/
/*
 *   functions
 * * * * * * * * */
void pointer::append_to_EEPROM(ushort data)
{
	if(current_address+2>0x7FFF)
		return;
	byte tmp_data;
	tmp_data=data>>8;
//	Serial << "DEBUG get_curr_addr :: " << at24c256b.get_current_address() << endl;
	at24c256b.write_byte_manual_start();
	at24c256b.write_byte_manual(tmp_data);
//			Serial << "written :: " << tmp_data << " , " ;
	tmp_data=data&0xFF;
	at24c256b.write_byte_manual(tmp_data);
//			Serial << tmp_data << endl;
	at24c256b.write_byte_manual_end();
//	Serial << "DEBUG get_curr_addr :: " << at24c256b.get_current_address() << endl;
	current_address+=2;
}
void pointer::initialize_EEPROM_pointer_to_zero(void)
{
	current_address=0;
	at24c256b.set_current_address(0);
	at24c256b.write_byte(0,0);
	at24c256b.write_byte(0,1);
//	update_pointer_on_EEPROM();// set to 0 (zero)
}

void pointer::update_pointer_on_EEPROM(void)
{
	ushort old_address=at24c256b.get_current_address();
//	Serial << "DEBUG get_curr_addr :: " << at24c256b.get_current_address() << endl;
	at24c256b.write_byte((current_address&0xFF00)>>8,0);
	at24c256b.write_byte(current_address&0xFF,1);
//	Serial << "DEBUG get_curr_addr :: " << at24c256b.get_current_address() << endl;
	at24c256b.set_current_address(old_address);
//	Serial << "DEBUG get_curr_addr :: " << at24c256b.get_current_address() << endl;
}
void pointer::dump_data_csv(void)
{
//	ushort old_address=at24c256b.read_byte(0);
//	old_address<<=8;
//	old_address|=at24c256b.read_byte();
	uint i=2;
	ushort old_address=at24c256b.get_current_address();
	Serial << "DUMP (printing data from EEPROM) ##########" << endl << endl << endl;
	if(current_address) Serial.println("min,°C");
	else Serial << "Sorry, no data!" << endl;
	at24c256b.set_current_address(2);
//	Serial << "DEBUG get_curr_addr :: " << at24c256b.get_current_address() << endl;
//	at24c256b.read_byte_manual_request(current_address-2);
	while(i<=current_address)
	{
		double data;
		byte tmp_byte;
		Serial << i/2*10 << ",";
//		Serial.print(i/2);
//		Serial.print(",");
		tmp_byte=at24c256b.read_byte();
		data=tmp_byte;
//		data<<=8;
		tmp_byte=at24c256b.read_byte();
		data+=(tmp_byte>>4)*0.0625;
		i+=2;
//		data>>=4;
//				data>>=4;
//				data_double=data*0.0625;
//		Serial.print(data,HEX);
//		Serial.print("\t");
//		Serial.println(data*0.0625);
		Serial << data << endl;
	}
	Serial.println();
	Serial.println();
	Serial.println("DUMP (end of data)               ##########");
	at24c256b.set_current_address(old_address);
}

/*
void pointer::dump_data_hex(void)
{// CHECK copy code from csv
	ushort old_address=at24c256b.read_byte(0);
	old_address<<=8;
	old_address|=at24c256b.read_byte();
	uint i=0;
	Serial.println("printing data from EEPROM #######");
	while(i<old_address)
	{
		byte j=0;
		byte data;
		Serial.print("0x");
		Serial.print(i,HEX);
		Serial.print(" \t:: 0x");
//		Serial << "0x" << _HEX(i) << " \t:: 0x";
		while(j++<32 && i++<old_address)
		{
			data=at24c256b.read_byte();
			if(data&0xF0==0)
				Serial.print("0");
//				Serial << "0";
			else
				Serial.print(data>>4,HEX);
//				Serial << _HEX(data>>4);
			if(data&0x0F==0)
				Serial.print("0");
//				Serial << "0";
			else
				Serial.print(data&0x0F,HEX);
//				Serial << _HEX(data&0x0F);
			if(j!=32 && j%4==0)
				Serial.print(" 0x");
//				Serial << " 0x";
		}
		Serial.println();
//		Serial << endl;
	}
	Serial.println("end of data $$$$$$$");
//	Serial << "end of data $$$$$$$" << endl;
}
*/
