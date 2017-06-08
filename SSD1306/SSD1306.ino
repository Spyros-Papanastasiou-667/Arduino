#include <SPI.h>

/*
 * the following are some notes of when it worked on SPI
 *
 * the 0.96" 128X64 OLED part is UG-2864HSWEG01
 * the controller is SSD1306
 * 	(http://www.datasheet4u.com/pdf/SSD1306-pdf/798762)
 * it was found on ebay
 *
 * resistors are as follows :
 *
 * R{3,4,6,7} = 4.7KOhm
 * R5 = 910KOhm
 *
 * 4-wire SPI
 * SCKL -> D0
 * SDIN -> D1
 *         D2 -> OPEN
 *         D3..7 -> GND (may)
 *
 * in general MOSI -> MOSI
 * https://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus
 *
 * connect the following to ICSP :
 * GND, VCC, D0(SCKL), D1(MOSI), RES(RESET)
 *
 * set DC, CS to LOW
 * DC LOW = write command instead of data
 * CS LOW = module active
 *
 * */

#define CS 7
#define DC 8

void setup() {
  pinMode(CS, OUTPUT);
  pinMode(DC, OUTPUT);
  digitalWrite(CS,LOW);
  digitalWrite(DC,LOW);
  SPI.begin();
  for (byte i = 0; i < 10; i++) {
  	SPI.transfer(0xAF);
  	SPI.transfer(0xA5);
    delay(1300);
    SPI.transfer(0xAE);
    delay(1300);
  }
	SPI.endTransaction();
	digitalWrite(SS,HIGH);
}

void loop() {
}
