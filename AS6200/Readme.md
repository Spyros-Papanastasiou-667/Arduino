This is a crude code to interface an **AS6200** temperature sensor

* The goal is to get everyday ( say every 10' ) measurements.

1. an AT24C256B EEPROM will be used ( 32KB )
2. an RTC will not be used, ( the code will run supposedly continuously )
3. on every serial connection, the accumulated data will be printed in CVS format
4. if EEPROM is full, the stop taking measurements and blink L13 led

Ref: 
* [AS6200](http://ams.com/eng/Support/Demoboards/Environmental-Sensors/Temperature-Sensors/AS6200-Adapterboard) adapter board
* [AS6200](http://ams.com/eng/Products/Environmental-Sensors/Temperature-Sensors/AS6200)
* [AT24C256B](http://www.atmel.com/images/doc5279.pdf) datasheet
