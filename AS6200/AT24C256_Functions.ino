inline void EEPROMInitialize()
{
	Wire.begin();
}

int EEPROMWriteByte(ushort addr,byte b)
{
	if(addr>0x7FFF)
		return -1;
	Wire.beginTransmission(AT24C256_ADDR);
	Wire.write((byte) (addr>>8)&0xFF );
	Wire.write((byte) addr&0xFF);
	Wire.write(b);
	Wire.endTransmission();
	delay(10);
	return 0;
}

byte EEPROMReadByte(ushort addr/*, + optional bool stop connection */)
{
	Wire.beginTransmission(AT24C256_ADDR);
	Wire.write((byte) (addr>>8)&0xFF );
	Wire.write((byte) addr&0xFF);
	Wire.endTransmission();
	Wire.requestFrom(AT24C256_ADDR,1,false);
	return Wire.read();
}

int EEPROMWritePage(ushort addr,byte data[], ushort len)
{
	if(addr>0x7FFF || len>0x40 /* 64 */)
		return -1;
	Wire.beginTransmission(AT24C256_ADDR);
	Wire.write((byte) (addr>>8)&0xFF );
	Wire.write((byte) addr&0xFF);
	for(int i=0;i<len;i++)
		Wire.write(data[i]);
	Wire.endTransmission();
	delay(10);
	return 0;
}

int EEPROMReadPage(ushort addr, ushort many/*, + optional bool stop connection */)
{
	if(addr>0x7FFF || many>0x7FFF)
		return -1;
	Wire.beginTransmission(AT24C256_ADDR);
	Wire.write((byte) (addr>>8)&0xFF );
	Wire.write((byte) addr&0xFF);
	Wire.endTransmission();
	Wire.requestFrom(AT24C256_ADDR,many,false);
	return 0;
}
