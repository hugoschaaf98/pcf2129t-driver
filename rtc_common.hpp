/**
 * rtc_common.hpp
 * 
 * Created: 03/09/2020
 * Author: SCHAAF Hugo
 * Rev : 0
 * 
 * Common things and utils to Real Time Clocks
 */

#ifndef RTC_COMMON_HPP
#define RTC_COMMON_HPP 1

#include <cstdint>

namespace RTC
{
	/**
	 * @brief       Structure which stores Date and Time data retrived from RTC
	 * 				in dedicaml format.
	 */
	typedef struct
    {
        uint8_t sec;	/*< seconds */
        uint8_t min;	/*< minutes */
        uint8_t hour;	/*< hours */
        uint8_t day;	/*< day */
        uint8_t wday;	/*< weekday */
        uint8_t mon;	/*< month */
        uint8_t year;	/*< year */
    } DateTime;

    /**
     * @brief      12h or 24h mode selection to the RTC.
     */
    typedef enum 
    {
        MODE12H,
        MODE24H
    } count_mode_t;

    /**
     * @brief      Convert BCD formatted value to decimal formatted value.
     *
     * @param[in]  bcd   The bcd value to convert into decimal value.
     *
     * @return     The decimal corresponding value
     */
    static inline
    uint8_t bcd_to_dec(uint8_t bcd) { return ((bcd & 0x0F) + (bcd>>4)*10); }

    /**
     * @brief      Convert decimal formatted value to BCD formatted value.
     *
     * @param[in]  dec   The decimal value to convert into BCD value.
     *
     * @return     The decimal corresponding value
     */
    static inline
    uint8_t dec_to_bcd(uint8_t dec) { return ((dec%10) | ((dec/10)<<4)); }

};

#endif