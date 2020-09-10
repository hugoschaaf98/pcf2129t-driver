#ifndef TWI_WRAPPER_HPP
#define TWI_WRAPPER_HPP 1

/*** Headers section ***/
#include <cstdint>
#include <cstdbool>

/**
 *  Place here your specific headers needed to implement 
 *  the twi wrapper's functions 
 */
#include "Arduino.h"
#include "Wire.h"

/**
 * Utility stuffs
 */

/**
 * @brief      Format properly twi read and write requests
 *
 * @param      addr  The slave's address
 *
 */
#define TWI_READ(addr)	(uint8_t)(addr<<1 | 0x01)	/*< Format read request at address addr */
#define TWI_WRITE(addr)	(addr<<1)					/*< Format write request at address addr */


/**
 * Below are the functions you need to implement
 */


/**
 * @brief      Initialize the I2C peripheral.
 *
 * @return     0 on success or the I2C bus error.
 */
static inline
int twiWrapperPeripheralInit()
{
    int err = 0;
    Wire.begin();
    return err;
}

/**
 * @brief      Read one byte from slave
 *
 * @param[in]  addr  The address of the slave
 * @param[in]  reg   The register's address to read
 *
 * @return     The received byte of data
 */
static inline 
uint8_t twiWrapperReadRegister(uint8_t addr, uint8_t reg)
{
    // Send the address @ which to read
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission(true); // Terminate with a stop condition

    Wire.requestFrom(addr, 1, true); // Request one byte and terminate with a stop condition

    if (Wire.available()) return Wire.read();
    else                  return 0x00;
}


/**
 * @brief      Read a sequence of multiple bytes starting at start_reg and store
 *             in buffer. This function assumes the slave has an auto incrementing 
 *             address register.
 *
 * @param[in]  addr       The slave's address
 * @param[in]  start_reg  The start register address
 * @param[in]  buffer     The buffer in which to store received data
 * @param[in]  length     The length in bytes of the buffer
 *
 * @return     The number of bytes received
 */
static inline
uint8_t twiWrapperReadMultipleRegisters(uint8_t addr, uint8_t start_reg, uint8_t* buffer, uint8_t length)
{
    uint8_t br = 0x00; // Number of bytes received

    // Send the address @ which to start reading bytes
    Wire.beginTransmission(addr);
    Wire.write(start_reg);
    Wire.endTransmission(true); // Terminate with a stop condition

    Wire.requestFrom(addr, length, true); // Request length bytes and terminate with a stop condition

    // Place received bytes in the buffer
    br = Wire.available();
    while(Wire.available())
    {
        *(buffer++) = Wire.read();
    }
    return br;
}


/**
 * @brief      Write to a register
 *
 * @param[in]  addr  The slave's address
 * @param[in]  reg   The register's address
 * @param[in]  val   The value to be written
 *
 * @return     0 on success or the I2C bus error.
 */
static inline 
int twiWrapperWriteRegister(uint8_t addr, uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(addr); // Send the address @ which to write val
    // Queue data to write
    Wire.write(reg);
    Wire.write(val);
    int err = Wire.endTransmission(true); // Terminate with a stop condition
    if (err != 0)
    {
        Serial.print("I2C error : ");
        Serial.println(err);
    }
    return err;
}

/**
 * @brief      Write a sequence of multiple bytes starting at start_reg. 
 *             This function assumes the slave has an auto incrementing 
 *             address register.
 *             
 * @param[in]  addr         The slave's address
 * @param[in]  start_reg    The register's address
 * @param[in]  data         The data to be written
 * @param[in]  length       The data length in bytes
 *
 * @return     0 on success or the I2C bus error.
 */
static inline
int twiWrapperWriteMultpileRegisters(uint8_t addr, uint8_t start_reg, const uint8_t* data, uint8_t length)
{
    Wire.beginTransmission(addr); // Send the address @ which to start writing bytes
    // Queue data to write
    Wire.write(start_reg);
    Wire.write(data, length);
    int err = Wire.endTransmission(true); // Terminate with a stop condition
    if (err != 0)
    {
        Serial.print("I2C error : ");
        Serial.println(err);
    }
    return err;
}

#endif // TWI_WRAPPER_HPP