/*
 * pca2129_registers.h
 *
 * Created: 02/09/2020
 * Author: SCHAAF Hugo
 * Rev : 0
 * 
 * Register map and flags map for PCA2129 Real Time Clock from NXP
 * 
 * ****************************************************************************
 * Note : 
 * To continue the implementation, please follow the following pattern :
 *
 *	#define {REGISTER_NAME}					{REGISTER ADDRESS}
 *	// flags
 *	#define {REGISTER_NAME}_{FLAG_NAME}		{BIT NUMBER}
 * 	// bit x unused							if any bit is unused	
 *  #define {REGISTER_NAME}_FORMAT(val)		(val & 0xBF)	// ensure that bit x is always 0/1
 *  
 * ******************************* TODO ***************************************
 *
 * - Finish the implementations for alarm macros.
 *   Only the address mapping is implemented. 
 *   The format check and the flags definition has to be done.
 * 
 * - Finish the implementations of watchdog and alarms.
 *   Only the address mapping is implemented. 
 *   The format check and the flags definition has to be done.
 */

#ifndef PCA2129_REGISTERS_H
#define PCA2129_REGISTERS_H


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


#define BIT_U8(b) 				(uint8_t)(1<<b)

/*---------------------------------------------------------------------------*/
/* Control registers                                                         */

/*---------------------------------------------------------------------------*/
/* Register CONTROL_1                                                        */
/*---------------------------------------------------------------------------*/
#define CONTROL_1 				0x00
// flags
#define CONTROL_1_SI			0
#define CONTROL_1_MI			1
#define CONTROL_1_12_24			2
#define CONTROL_1_POR_OVRD		3
#define CONTROL_1_TSF1			4
#define CONTROL_1_STOP			5
// bit 6 unused
#define CONTROL_1_EXT_TEST		7
#define CONTROL_1_FORMAT(val)	(val & 0xBF)	// ensure that bit 6 is always 0.

/*---------------------------------------------------------------------------*/
/* Register CONTROL_2                                                        */
/*---------------------------------------------------------------------------*/
#define CONTROL_2 				0x01
// flags
// bit 0 unused
#define CONTROL_2_AIE			1
#define CONTROL_2_TSIE			2
// bit 3 unused
#define CONTROL_2_AF			4
#define CONTROL_2_TSF2			5
#define CONTROL_2_WDTF			6
#define CONTROL_2_MSF			7
#define CONTROL_2_FORMAT(val)	(val & 0xF6)	// ensure that bit 0 and bit 3 are always 0.
/*---------------------------------------------------------------------------*/
/* Register CONTROL_3                                                        */
/*---------------------------------------------------------------------------*/
#define CONTROL_3 				0x03
// flags
#define CONTROL_3_BLIE			0	// Battery Low Interrupt Enable
#define CONTROL_3_BIE			1	// Battery Interrupt
#define CONTROL_3_BLF			2	// Battery Low Flag
#define CONTROL_3_BF 			3	// Battery Siwtch-over flag
#define CONTROL_3_BTSE			4	// Battery Time Stamp Enable 
#define CONTROL_3_PWRMNG_0		5	// Power management mode selection
#define CONTROL_3_PWRMNG_1		6	//
#define CONTROL_3_PWRMNG_2		7	//
#define CONTROL_3_FORMAT(val)	(val)

/*---------------------------------------------------------------------------*/
/* Register CLKOUT_CTL                                                       */
/*---------------------------------------------------------------------------*/
#define CLKOUT_CTL 				0x0F
// flags
#define CLKOUT_CTL_COF_0		0	// CLKOUT frequency selection
#define CLKOUT_CTL_COF_1		1	//
#define CLKOUT_CTL_COF_2		2	//
// bit 3 unused
// bit 4 unused
#define CLKOUT_CTL_OTPR			5	// OTP refresh
#define CLKOUT_CTL_TCR_0		6	// Temperature measurement period selection
#define CLKOUT_CTL_TCR_1		7	//
#define CLKOUT_CTL_FORMAT(val)	(val)


/*---------------------------------------------------------------------------*/
/* Register WATCHDG_TIM_CTL                                                  */
/*---------------------------------------------------------------------------*/
#define WATCHDG_TIM_CTL				0x10
// flags
#define WATCHDG_TIM_CTL_TF_0		0	// Timer source clock for watchdog timer selection
#define WATCHDG_TIM_CTL_TF_1		1	//
// bit 2 unused
// bit 3 unused
// bit 4 unused
#define WATCHDG_TIM_CTL_TI_TP		5	// Timer interrupt on pin /INT
// bit 6 unused
#define WATCHDG_TIM_CTL_WD_CD		7	// Watchdog timer enable
#define WATCHDG_TIM_CTL_FORMAT(val)	(val & 0xA3) // ensure that bis 6 is 0

/*---------------------------------------------------------------------------*/
/* Register TIMESTP_CTL                                                      */
/*---------------------------------------------------------------------------*/
#define TIMESTP_CTL				0x12
// flags
#define _1_O_16_TIMESTP_0 		0	// 1/16 second timestamp information
#define _1_O_16_TIMESTP_1 		1	// coded in BCD format
#define _1_O_16_TIMESTP_2 		2	//
#define _1_O_16_TIMESTP_3 		3	//
#define _1_O_16_TIMESTP_4		4	//
// bit 5 unused
#define TIMESTP_CTL_TSOFF		6	// Timestamp function enable
#define TIMESTP_CTL_TSM			7	// First/Last event store
#define TIMESTP_CTL_FORMAT(val)	(val & 0xDF)





/*---------------------------------------------------------------------------*/
/* Time registers                                                            */

/*---------------------------------------------------------------------------*/
/* Register SECONDS                                                          */
/*---------------------------------------------------------------------------*/
#define SECONDS 				0x03
// flags
#define SECONDS_OSF				7				
#define SECONDS_FORMAT(val)		(val & 0x7F)	// must not exceed 59

/*---------------------------------------------------------------------------*/
/* Register MINUTES                                                          */
/*---------------------------------------------------------------------------*/
#define MINUTES 				0x04
#define MINUTES_FORMAT(val)		(val & 0x7F)	// must not exceed 59

/*---------------------------------------------------------------------------*/
/* Register HOURS                                                            */
/*---------------------------------------------------------------------------*/
#define HOURS 					0x05
// flags
#define HOURS_AMPM				5
#define HOURS_FORMAT(val)		(val & 0x3F)	// must not exceed 23 in 24h mode

/*---------------------------------------------------------------------------*/
/* Register DAYS                                                             */
/*---------------------------------------------------------------------------*/
#define DAYS 					0x06
#define DAYS_FORMAT(val)		(val & 0x3F)	// must not exceed 31

/*---------------------------------------------------------------------------*/
/* Register DAYS                                                             */
/*---------------------------------------------------------------------------*/
#define WEEKDAYS 				0x07
#define WEEKDAYS_FORMAT(val)	(val & 0x07)	// must not exceed 6

/*---------------------------------------------------------------------------*/
/* Register DAYS                                                             */
/*---------------------------------------------------------------------------*/
#define MONTHS 					0x08
#define MONTHS_FORMAT(val)		(val & 0x1F)	// must not exceed 12

/*---------------------------------------------------------------------------*/
/* Register DAYS                                                             */
/*---------------------------------------------------------------------------*/
#define YEARS 					0x09
#define MONTHS_FORMAT(val)		(val & 0x1F)	// must not exceed 99





/*---------------------------------------------------------------------------*/
/* Alarm registers                                                           */

/*** TODO ***
 *
 * Finish the implementations of alarm macros.
 * Only the address mapping is implemented. 
 * The format check and the flags definition has to be done.
 */

/*---------------------------------------------------------------------------*/
/* Register SECOND_ALARM                                                     */
/*---------------------------------------------------------------------------*/
#define SECOND_ALARM 	0x0A

/*---------------------------------------------------------------------------*/
/* Register MINUTE_ALARM                                                     */
/*---------------------------------------------------------------------------*/
#define MINUTE_ALARM 	0x0B

/*---------------------------------------------------------------------------*/
/* Register HOUR_ALARM                                                       */
/*---------------------------------------------------------------------------*/
#define HOUR_ALARM 		0x0C

/*---------------------------------------------------------------------------*/
/* Register DAY_ALARM                                                        */
/*---------------------------------------------------------------------------*/
#define DAY_ALARM 		0x0D

/*---------------------------------------------------------------------------*/
/* Register WEEKDAY_ALARM                                                    */
/*---------------------------------------------------------------------------*/
#define WEEKDAY_ALARM 	0x0E





/*---------------------------------------------------------------------------*/
/* Watchdog and Timestamps registers                                         */


/*** TODO ***
 *
 * Finish the implementations of watchdog and alarms.
 * Only the address mapping is implemented. 
 * The format check and the flags definition has to be done.
 */

/*---------------------------------------------------------------------------*/
/* Register WATCHDG_TIM_VAL                                                  */
/*---------------------------------------------------------------------------*/
#define WATCHDG_TIM_VAL	0x11

/*---------------------------------------------------------------------------*/
/* Register SEC_TIMESTP                                                      */
/*---------------------------------------------------------------------------*/
#define SEC_TIMESTP		0x13

/*---------------------------------------------------------------------------*/
/* Register MIN_TIMESTP                                                      */
/*---------------------------------------------------------------------------*/
#define MIN_TIMESTP		0x14

/*---------------------------------------------------------------------------*/
/* Register HOUR_TIMESTP                                                     */
/*---------------------------------------------------------------------------*/
#define HOUR_TIMESTP	0x15

/*---------------------------------------------------------------------------*/
/* Register DAY_TIMESTP                                                      */
/*---------------------------------------------------------------------------*/
#define DAY_TIMESTP		0x16

/*---------------------------------------------------------------------------*/
/* Register MON_TIMESTP                                                      */
/*---------------------------------------------------------------------------*/
#define MON_TIMESTP		0x17

/*---------------------------------------------------------------------------*/
/* Register YEAR_TIMESTP                                                     */
/*---------------------------------------------------------------------------*/
#define YEAR_TIMESTP	0x18

/*---------------------------------------------------------------------------*/
/* Register AGING_OFFSET                                                     */
/*---------------------------------------------------------------------------*/
#define AGING_OFFSET	0x19

/*---------------------------------------------------------------------------*/
/* Register INTERNAL_REG                                                     */
/*---------------------------------------------------------------------------*/
#define INTERNAL_REG	0x1A

#ifdef __cplusplus
};
#endif

#endif // PCA2129_REGISTERS_H