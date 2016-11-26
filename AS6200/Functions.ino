inline void SerialInitialize()
{
	Serial.begin(115200);
	while (!Serial);
}
