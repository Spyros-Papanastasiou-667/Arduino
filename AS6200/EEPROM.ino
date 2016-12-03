/* con(de)structors
 * * * * * * * * * * */
/* * * * * * * * * *
 *                 *
 *    functions    *
 *                 *
 * * * * * * * * * */
ushort EEPROM::get_current_address(void)
{
	return current_address;
}

/*
ushort EEPROM::get_last_address(void)
{
	return last_address;
}
*/

void EEPROM::set_current_address(ushort addr)
{
/*			if (addr>0x7FFF)
		return;
*/
	current_address=addr;
	limit_current_address();
}

byte EEPROM::read_byte(void)
{
	Wire.requestFrom(device_address,1);//,false);
	current_address++;
//	last_address++;
	limit_current_address();
	// check for wire.available CHECK
	byte data;
	if(Wire.available())
		data=Wire.read();
	delay(50);
	return data;
}

byte EEPROM::read_byte(ushort addr/*, + optional bool stop connection */)
{
	Wire.beginTransmission(device_address);
	Wire.write((byte) (addr>>8)&0xFF );
	Wire.write((byte) addr&0xFF);
	Wire.endTransmission();
	Wire.requestFrom(device_address,1);//,false);
	current_address=addr+1;
	limit_current_address();
	// check for wire.available CHECK
	byte data;
	if(Wire.available())
		data=Wire.read();
	delay(50);
	return data;
}
void EEPROM::read_page(byte page[],ushort many)
{
	read_page(page,many,current_address);
}
void EEPROM::read_page(byte page[],ushort many,ushort addr/*, + optional bool stop connection */)
{// FIX
	if(addr>0x7FFF || many>0x7FFF)
		return;
	Wire.beginTransmission(device_address);
	Wire.write((byte) (addr>>8)&0xFF );
	Wire.write((byte) addr&0xFF);
	Wire.endTransmission();
	Wire.requestFrom(device_address,many);//,false);
	ushort i=0;
	while(Wire.available())
	{
		page[i++]=Wire.read();
	}
	// DEBUG "got # bytes"
	current_address=addr+i;// check for overflow CHECK
	limit_current_address();
}
void EEPROM::update_current_address(ushort addr)
{
	current_address=addr;
	limit_current_address();
}

void EEPROM::write_byte(byte b)
{
	write_byte(b,current_address);
}
void EEPROM::write_byte(byte b,ushort addr)
{
	if(addr>0x7FFF)
		return;
	Wire.beginTransmission(device_address);
	Wire.write((byte) (addr>>8)&0xFF );
	Wire.write((byte) addr&0xFF);
	Wire.write(b);
	Wire.endTransmission();
	delay(50);
	current_address=addr+1;
	limit_current_address();
}
void EEPROM::write_page(ushort addr,byte data[], ushort len)
{
	if(addr>0x7FFF || len>0x40 /* 64 */)
		return;
	Wire.beginTransmission(device_address);
	Wire.write((byte) (addr>>8)&0xFF );
	Wire.write((byte) addr&0xFF);
	for(int i=0;i<len;i++)
		Wire.write(data[i]);
	Wire.endTransmission();
	current_address=addr+len;
	limit_current_address();
	delay(50);
}
/* * * * * * * * * *
 *                 *
 *  P R I V A T E  *
 *                 *
 * * * * * * * * * */
void EEPROM::limit_current_address(void)
{
	if(current_address>0x7FFF)
	{
		current_address-=0x8000;
	}
}
/*
void EEPROM::limit_last_address(void)
{
	if(last_address>0x7FFF)
	{
		last_address-=0x8000;
	}
}
*/
