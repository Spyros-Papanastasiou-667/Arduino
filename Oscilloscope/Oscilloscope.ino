/* for LCD */
#include <UTFT.h>
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t Sinclair_S[];
extern uint8_t TinyFont[];
UTFT myGLCD(ITDB32S,25,26,27,28);
#define TOP		0
#define LEFTEDGE 	0
#define RIGHTEDGE	319
#define BOTTOM	239
#define WIDTH	320
#define HALFW	160
#define HALFH	120
#define HEIGHT	240
#define FONT_HEIGHT	8
#define FONT_WIDTH	8

void setup()
{
	Serial.begin(115200);
	/* initialize lcd */
	myGLCD.InitLCD();
	Serial.println("LCD Initialized!");
	/* set font to 8x8 */
	myGLCD.setFont(Sinclair_S);
	Serial.println("Font Set!");
	myGLCD.setColor(VGA_RED);
	myGLCD.print("Font Set!",CENTER,HALFH-FONT_HEIGHT);
	/* clear screen */
	myGLCD.clrScr();
	analogReadResolution(12);
	Serial.println("Analog Resolution Set!");
	/* */
	/* set grid */
	myGLCD.clrScr();
	Serial.println("Setup finished.");
}
typedef	unsigned	long	ulong;
typedef	unsigned	short	ushort;
//typedef	unsigned	int	uint;
#define	NOPIX	255;
ulong graph0[320]={0},graphLast[320]={0};
ulong counter=0,counter2=0;
ulong start=0,secStart=0;
bool print=false;
void loop()
{
//	myGLCD.clrScr();
	if(millis()-secStart>=70)
	{
		counter2++;
		for(counter=0;counter<320;counter++)
		{
			myGLCD.setColor(VGA_BLACK);
			myGLCD.drawPixel(counter,graphLast[counter]);
			graphLast[counter]=((graph0[counter]+analogRead(A0))/4095.0*239)/counter2;
			myGLCD.setColor(VGA_WHITE);
			myGLCD.drawPixel(counter,graphLast[counter]);
			graph0[counter]=0;
		}
		myGLCD.printNumI(counter2*320.0/((millis()-secStart)/1000.0),CENTER,112);
		myGLCD.print("smpls/s w/ avg",319/2+8*3+8,112);
		secStart=millis();
		counter2=0;
	}else
	{
		counter2++;
		for(counter=0;counter<320;counter++)
		{
			graph0[counter]+=analogRead(A0);
		}
	}
//	Serial.println(millis()-start);
}
