#define BUFF 10
char str[BUFF+1]={0};
char * strPtr=NULL;
char c='\0';
int i=0;

void setup() {
	Serial.begin(115200);
	Serial.println("setup finished");
}

void loop() {
	if(Serial.available()>0)
	{
		/*      G e t    L i n e      */
		for(i=0;i<BUFF;i++)
		{
			while(!Serial.available());
			str[i]=Serial.read();
			str[i+1]='\0';
			if(str[i] == '\n')
			{
				while(Serial.available())
					Serial.read();
				break;
			}
		}
		if(i>=BUFF)
		{
			while((c=Serial.read())!='\n')
				while(!Serial.available());
		}
		Serial.print(str);
		Serial.print(" StrLen:: ");
		Serial.println(strlen(str));
		/* **************************** */
		/* str holds a BUFF-lengthed input string */
		/* start playing ... */
		Serial.print(str);
		for(int j=0;j<3;j++)
		{
			for (i=0;i< BUFF;i++)
			{
				delay(75);
				Serial.print("\b \b");
			}
			for(i=0;i<BUFF;i++)
			{
				delay(75);
				Serial.print(str[i]);
			}
		}
		Serial.println();
		Serial.println("----------");
		/* empty the string */
		for(i=0;i<BUFF+1;i++)
			str[i]='\0';
	}
}

void clearNL(void){
	while(Serial.available())
		Serial.read();
}
