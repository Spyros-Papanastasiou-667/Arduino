#include <Streaming.h>
/*
 * hold two bytes on the start of EEPROM for the last record
 * */
class pointer{
	public:
		/*
		 *   constructor(s)
		 * * * * * * * * * * */
		pointer():\
			at24c256b(AT24C256_ADDR),\
			current_address(0)
		{
			//please call initialize()
/*
			// get last pointer, and continue from that
			current_address=at24c256b.read_byte(0);
			current_address<<=8;
			current_address|=at24c256b.read_byte();
			// ___ ____ _______, ___ ________ ____ ____
			at24c256b.set_current_address(current_address);
*/
		}
		/*
		 *   functions
		 * * * * * * * * */
		void append_to_EEPROM(ushort data);
///*
		void initialize()
		{
			// get last pointer, and continue from that
			current_address=at24c256b.read_byte(0);
			current_address<<=8;
			current_address|=at24c256b.read_byte(1);
//			current_address+=current_address%2;// fix possible error
			// ___ ____ _______, ___ ________ ____ ____
//			Serial << "DEBUG: setting EEPROM address at :: " << current_address << endl;
			at24c256b.set_current_address(current_address+2);
		}
//*/
		void initialize_EEPROM_pointer_to_zero(void);
		void dump_data_csv(void);
//		void dump_data_hex(void);
		void update_pointer_on_EEPROM(void);
	private:
		EEPROM at24c256b;
		ushort current_address;
};
