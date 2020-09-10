/**
 * pca2129.hpp
 *
 * Created: 02/09/2020
 * Author: SCHAAF Hugo
 * Rev : 0
 * 
 * Tiny driver for PCA2129 Real Time Clock from NXP
 * 
 */ 

#ifndef PCF2129_HPP
#define PCF2129_HPP   1

#include <cstdint>
#include <cstdbool>

#include "pcf2129_registers.h"
#include "rtc_common.hpp"
#include "twi_wrapper.hpp"


namespace RTC
{

	/**
     * CLKOUT frequencies.
     */
    typedef enum 
    {
        FREQ32768HZ,
        FREQ16384HZ,
        FREQ8192HZ,
        FREQ4096HZ,
        FREQ2048HZ,
        FREQ1024HZ,
        FREQ1HZ,
        FREQ0HZ         /*< No clkout output and CLKOUT pin remains High Impedance */
    }clkout_freq_t;


    /**
     * @brief      This class describes a pcf 2129.
     */
    class PCF2129
    {

    public:

        const uint8_t TWI_ADDR {0x51}; // The I2C address of PCF2129

    	/**
    	 * @brief      Constructs a new instance.
    	 *
    	 * @param[in]  pin_int  The pin integer
    	 */ 
        PCF2129(bool twiInit=false): _control1{0x00}, _control2{0x00}, _control3{0x00}, _clkout_ctl{0x00}, _watchdg_tim_ctl{0x00}, _timestp_ctl{0x00}
        {
        	// Here are default settings
        	// _control1 |= ;
        	// _control2 |= ;
        	_control3 |= BIT_U8(CONTROL_3_PWRMNG_0); // Battery switch-over function enable in standard mode, battery low detection disabled. 
        	_clkout_ctl |= BIT_U8(CLKOUT_CTL_TCR_1); // Perform temperature measurement every minute.
        	_watchdg_tim_ctl |= BIT_U8(WATCHDG_TIM_CTL_TI_TP) | BIT_U8(WATCHDG_TIM_CTL_TF_1); // disable watchdog function. Select pulsed mode and 1Hz clock source however
        	_timestp_ctl |= BIT_U8(TIMESTP_CTL_TSOFF); // disable timestamp function

        	// Initialize the I2C peripheral if needed.
        	if(twiInit) twiWrapperPeripheralInit();
        }


        /*** Configuration ***
         * 
         * These methods DO NOT write the RTC's internal registers.
         * They must be invoked before calling configure() 
         * method to customize the default RTC configuration
         * if desired only. 
         */

        /**
         * @brief      Select whether the clock operates in 12H or 24H mode.
         *
         * @param[in]  mode  the count mode
         */
        void selectCountMode(count_mode_t mode);

        /**
         * @brief      Sets the CLKOUT frequency.
         *
         * @param[in]  clkfreq 	The Clkout clock frequency
         */
        void selectClkoutFreq(clkout_freq_t clkfreq);



        /*** Setters ***/
        /*
         * These methods DO write the RTC's internal registers.
         */


        /**
         * @brief      Configure the RTC. This method is responsible
         * 			   for writing the configuration prepared with 
         * 			   selectXxxxx() methods to the internal RTC's registers.
         */
        int configure();

        /**
         * @brief      Start the RTC.
         *
         * @return     0 on success or the I2C bus error.
         */
        int start();

        /**
         * @brief      Stop the RTC.
         *
         * @return     0 on success or the I2C bus error.
         */
        int stop();

        /*** TODO ***/

        // void setTemperatureMeasurementPeriod(uint8_t mode);
        // void setWatchdogTimer();
        // void setPowerManagementMode(uint8_t flags);

        /**
         * @brief      Sets the date and the time in the RTC
         *
         * @param      datetime  The datetime data to write to the RTC.
         *
         * @return     0 on success or the I2C bus error.
         */
        int setDateTime(DateTime &datetime);

        /*** Getters ***/

        /**
         *  @brief 	Read a single value. Each call to one of the following functions
         *  		perfoms a single read to the I2C bus ie. write the register'address
         *  		to read and then read the value.
         */
        uint8_t seconds()	{ return ( bcd_to_dec(twiWrapperReadRegister(TWI_ADDR, SECONDS)) ); }
        uint8_t minutes()	{ return ( bcd_to_dec(twiWrapperReadRegister(TWI_ADDR, MINUTES)) ); }
        uint8_t hours()		{ return ( bcd_to_dec(twiWrapperReadRegister(TWI_ADDR, HOURS)) ); }
        uint8_t day()		{ return ( bcd_to_dec(twiWrapperReadRegister(TWI_ADDR, DAYS)) ); }
        uint8_t weekday()	{ return ( bcd_to_dec(twiWrapperReadRegister(TWI_ADDR, WEEKDAYS)) ); }
        uint8_t month()		{ return ( bcd_to_dec(twiWrapperReadRegister(TWI_ADDR, MONTHS)) ); }
        uint8_t year()		{ return ( bcd_to_dec(twiWrapperReadRegister(TWI_ADDR, YEARS)) ); }

        /**
         * @brief      Read the date ant time from RTC. The 7 different data registers
         *  			are read in a single I2C data transfer from the RTC.
         *  			
         * @param      datetime  The datetime structure to be filled
         *
         * @return     0 on success or the I2C bus error.
         */
        int dateTime(DateTime &datetime);


    private:


        uint8_t _control1;
        uint8_t _control2;
        uint8_t _control3;
        // uint8_t _seconds;
        // uint8_t _minutes;
        // uint8_t _hours;
        // uint8_t _days;
        // uint8_t _weekdays;
        // uint8_t _months;
        // uint8_t _years;
        // uint8_t _second_alarm;
        // uint8_t _minute_alarm;
        // uint8_t _hour_alarm;
        // uint8_t _day_alarm;
        // uint8_t _weekday_alarm;
        uint8_t _clkout_ctl;
        uint8_t _watchdg_tim_ctl;
        // uint8_t _watchdg_tim_val;
        uint8_t _timestp_ctl;
        // uint8_t _sec_timestp;
        // uint8_t _min_timestp;
        // uint8_t _hour_timestp;
        // uint8_t _day_timestp;
        // uint8_t _mon_timestp;
        // uint8_t _year_timestp;
        // uint8_t _aging_offset;
        // uint8_t _internal_reg;
    };

} // namespace RTC

#endif // PCF2129_HPP