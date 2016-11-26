inline void EEPROMInitialize();
inline void SerialInitialize();
int EEPROMWriteByte(ushort addr,byte b);
byte EEPROMReadByte(ushort addr/*, + optional bool stop connection */);
int EEPROMWritePage(ushort addr,byte data[],ushort len);
int EEPROMReadPage(ushort addr, ushort many/*, + optional bool stop connection */);
