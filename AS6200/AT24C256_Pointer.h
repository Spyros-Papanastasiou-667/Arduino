/*
 * hold two bytes on the start of EEPROM for the last record
 * */
class pointer{
	public:
		/*
		 *   constructor(s)
		 * * * * * * * * * * */
		pointer():\
			current_address(0),\
			at24c256b(AT24C256_ADDR)
		{}
		/*
		 *   functions
		 * * * * * * * * */
		void append_to_EEPROM(ushort data);
		void initialize_EEPROM(void);
		void dump_data_csv(void);
		void dump_data_hex(void);
		void update_pointer_on_EEPROM(void);
	private:
		EEPROM at24c256b;
		ushort current_address;
};
