class AS6200
{
	/* configuration register
	 * address : 0x01
	 *
	 * D15 D14 D13 D12 D11 D10 D09 D08 D07 D06 D05 D04 D03 D02 D01 D00
	 *  RW  RO++RO  RW++RW++RW++RW++RW++RW++RW  RO  RW  RO++RO++RO++RO
	 *  SS  --++--  CF++CF POL  IM  SM  CR++CR  AL  --++--++--++--++--
	 *
	 *	ss - single-shot
	 * cf - consecutive faults
	 * pol -polarity (alert)
	 * im - interrupt mode (comparator/interrupt mode)
	 * sm - sleep mode
	 * cr - conversion rate
	 *      0 0 - 0.25Hz
	 *      0 1 - 1Hz
	 *      1 0 - 4Hz (default)
	 *      1 1 - 8Hz
	 * al - alert (TLOW THIGH)
	 * */
	public:
		/*
		 *  constructors
		 * * * * * * * * */
		AS6200(): device_address(AS6200_ADDR){}
		/*
		 *   functions
		 * * * * * * * * */
		ushort get_register(byte reg_id);
		ushort get_temperature(void);
		double get_temperature_human_readable(void);
		void set_single_shot_mode(void);
		void set_continuous_mode(void);
	private:
		ushort last_temperature;
		const byte device_address;
};
