This is a crude code to interface an AS6200 temperature sensor

* The goal is to get everyday ( say every 10' ) measurements.

** an AT24C256B EEPROM will be used ( 32KB )
** an RTC will not be used, ( the code will run supposedly continuously )
** on every serial connection, the accumulated data will be printed in CVS format
** if EEPROM is full, the stop taking measurements and blink L13 led
