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
float start=0,secStart=0;
ushort diff=0;
bool print=false;
void loop()
{
//	myGLCD.clrScr();
	if((diff=millis()-secStart)>=analogRead(A0)/4095.0*150)
	{
		secStart+=diff;
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
/*		myGLCD.printNumF(counter2*320.0/((diff)/1000.0),1,CENTER,112);
		myGLCD.print(" @",319/2+8*3+8,112);
		myGLCD.printNumF(1000.0/(diff),1,319/2+8*3+2*8+2*8,112);
		myGLCD.print(" FPS",319/2+8*3+3*8+4*8,112);
		myGLCD.printNumF(diff,1,319/2+8*3+3*8+4*8+5*8,112);
*/		Serial.print(counter2*320.0/((diff)/1000.0)+1/**** <--- +1 *******/,1);
		Serial.print(" @ ");
		Serial.print(1000.0/(diff),1);
		Serial.print(" FPS ");
		Serial.println(diff,1);
//		Serial.println((counter2*320.0/((diff)/1000.0)+1)*1000.0/(diff));
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
