/**
 * pca2129.cpp
 *
 * Created: 02/09/2020
 * Author: SCHAAF Hugo
 * Rev : 0.1
 * 
 * Tiny driver for PCA2129 Real Time Clock from NXP
 * 
 */ 

#include "pcf2129.hpp"
#include "pcf2129_registers.h"

namespace RTC {
	/**
	 * @brief      Write the configuration to the RTC. 
	 * 
	 * @note 	   This functions returns immediately
	 * 			   on I2C transfer fail without continuing the configuration.
	 *
	 * @return     0 on success or the I2C bus error.
	 */
	int PCF2129::configure()
	{
		int err = -1;
		err = twiWrapperWriteRegister(TWI_ADDR, 	CONTROL_1, 	CONTROL_1_FORMAT(_control1) );
		if(err) return err;
		err = twiWrapperWriteRegister(TWI_ADDR, 	CONTROL_2, 	CONTROL_2_FORMAT(_control2) );
		if(err) return err;
		err = twiWrapperWriteRegister(TWI_ADDR, 	CONTROL_3, 	CONTROL_3_FORMAT(_control3) );
		if(err) return err;
		err = twiWrapperWriteRegister(TWI_ADDR, 	CLKOUT_CTL, CLKOUT_CTL_FORMAT(_clkout_ctl) );
		if(err) return err;
		err = twiWrapperWriteRegister(TWI_ADDR, 	WATCHDG_TIM_CTL,	WATCHDG_TIM_CTL_FORMAT(_watchdg_tim_ctl) );
		if(err) return err;
		err = twiWrapperWriteRegister(TWI_ADDR, 	TIMESTP_CTL,		TIMESTP_CTL_FORMAT(_timestp_ctl) );
		return err;
	}

	/**
	 * @brief      Start the RTC.
	 *
	 * @return     0 on success or the I2C bus error.
	 */
	int PCF2129::start()
	{
		int err = -1;
		_control1 &= ~BIT_U8(CONTROL_1_STOP); // clear the stop bit to start the RTC
		err = twiWrapperWriteRegister(TWI_ADDR, CONTROL_1, CONTROL_1_FORMAT(_control1) );
		return err;
	}

	/**
	 * @brief      Stop the RTC.
	 *
	 * @return     0 on success or the I2C bus error.
	 */
	int PCF2129::stop()
	{
		int err = -1;
		_control1 |= BIT_U8(CONTROL_1_STOP); // set the stop bit to stop the RTC
		err = twiWrapperWriteRegister(TWI_ADDR, CONTROL_1, CONTROL_1_FORMAT(_control1) );
		return err;
	}

    /**
     * @brief      Select whether the clock operates in 12H or 24H mode.
     *
     * @param[in]  mode  the count mode
     */
    void PCF2129::selectCountMode(count_mode_t mode)
	{
		if(mode == MODE12H)
		{
			_control1 |= BIT_U8(CONTROL_1_12_24); // set 12_24 bit
		}
		else if( mode == MODE24H)
		{
			_control1 &= ~BIT_U8(CONTROL_1_12_24); // clear 12_24 bit
		}
	}

	/**
	 * @brief      Sets the CLKOUT frequency.
	 *
	 * @param[in]  clkfreq 	The Clkout clock frequency
	 */
	void PCF2129::selectClkoutFreq(clkout_freq_t clkfreq)
	{
		_clkout_ctl &= 0xF8; // reset COF[2:0] bits

		switch(clkfreq)
		{
			case FREQ32768HZ:
				break;		
			case FREQ16384HZ:
				_clkout_ctl |= BIT_U8(CLKOUT_CTL_COF_0);
				break;
			case FREQ8192HZ:
				_clkout_ctl |= BIT_U8(CLKOUT_CTL_COF_1);
				break;
			case FREQ4096HZ:
				_clkout_ctl |= BIT_U8(CLKOUT_CTL_COF_0) | BIT_U8(CLKOUT_CTL_COF_1);
				break;
			case FREQ2048HZ:
				_clkout_ctl |= BIT_U8(CLKOUT_CTL_COF_2);
				break;
			case FREQ1024HZ:
				_clkout_ctl |= BIT_U8(CLKOUT_CTL_COF_2) | BIT_U8(CLKOUT_CTL_COF_0);
				break;
			case FREQ1HZ:
				_clkout_ctl |= BIT_U8(CLKOUT_CTL_COF_1) | BIT_U8(CLKOUT_CTL_COF_2);
				break;
			default: // no output by default - FREQ0HZ and CLKOUT pin is High Impedance
				_clkout_ctl |= BIT_U8(CLKOUT_CTL_COF_0) | BIT_U8(CLKOUT_CTL_COF_1) | BIT_U8(CLKOUT_CTL_COF_2);
				break;
		}
	}

	/**
	 * @brief      Sets the date and the time in the RTC
	 *
	 * @param      datetime  The datetime data to write to the RTC.
	 *
	 * @return     0 on success or the I2C bus error.
	 */
	int PCF2129::setDateTime(DateTime &datetime)
	{
		int err = -1;
		uint8_t* tmp = (uint8_t*)(&datetime);
		// convert each Datetime struct member into BCD format
		for(uint8_t i=0; i < sizeof(datetime);)
		{
			tmp[i] = dec_to_bcd(tmp[i]);
			i++;
		}
		err = twiWrapperWriteMultpileRegisters(TWI_ADDR, SECONDS, tmp, sizeof(datetime) );
		return err;
	}

	/**
	 * @brief      Read the date ant time from RTC. The 7 different data registers
	 *  			are read in a single I2C data transfer from the RTC.
	 *  			
	 * @param      datetime  The datetime structure to be filled
	 *
	 * @return     0 if success, -1 if an error occured during the I2C transfer.
	 */
	int PCF2129::dateTime(DateTime &datetime)
	{
		int err = -1; // error
		uint8_t br = 0; // Number of bytes read
		uint8_t tmp[7] = {0}; // temporary buffer to store the 7 bytes representing the date & time

		// retrieve date time from RTC
		br = twiWrapperReadMultipleRegisters(TWI_ADDR, SECONDS, tmp, sizeof(tmp)*sizeof(tmp[0]));

		// check if the expected amount of data has been received
		if(br < sizeof(tmp)*sizeof(tmp[0]))
		{
			// something went wrong during the i2c transfer
			return err;
		}

		// convert each received byte of data into decimal format
		for(uint8_t i=0; i < sizeof(tmp)*sizeof(tmp[0]); i++)
		{
			tmp[i] = bcd_to_dec(tmp[i]);
		}

		err = 0x00;

		// store converted data in datetime structure
		datetime.sec 	 = tmp[0];
		datetime.min 	 = tmp[1];
		datetime.hour	 = tmp[2];
		datetime.day 	 = tmp[3];
		datetime.wday	 = tmp[4];
		datetime.mon 	 = tmp[5];
		datetime.year 	 = tmp[6];

		return err;
	}

} // namespace RTC