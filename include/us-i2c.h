/*
MIT License

Copyright (c) 2024 Giuseppe Giglio <g.giglio001@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**
 * \file us-i2c.h
 * \brief This file contains the declarations for a Linux userspace I2C library,
 * useful to quickly manage and control I2C peripherals.
 * \author Giuseppe Giglio <g.giglio001@gmail.com>
 * \version 1.0.0 - First release
*/

#ifndef US_I2C_H
#define US_I2C_H


#ifdef __cplusplus
extern "C" {
#endif

/* Standard library includes */
#include <stdint.h>
#include <string.h>

/* Macros */

/** I2C Linux Dev path prefix */
#define I2C_DEV_PATH_PREFIX ((const char *)"/dev/i2c-")

/** String size of I2C_DEV_PATH_PREFIX */
#define I2C_DEV_PATH_PREFIX_SIZE ((size_t) strlen(I2C_DEV_PATH_PREFIX))

/** Supported register address size */
#define I2C_REGISTER_ADDR_SIZE ((uint8_t)1)

/** Success Return code */
#define US_I2C_SUCCESS ((int16_t)0)

/** Error Return code */
#define US_I2C_ERROR ((int16_t)-1)

/* Typedefs */

/**
 * \struct i2cConfiguration_t "us-i2c.h" us-i2c.h
 * \brief This structure holds the current configuration items for the I2C bus.
*/
typedef struct i2cConfiguration_s
{
    uint16_t bus; /** I2C bus number */
    int32_t busFD; /** Opened bus file descriptor */
} i2cConfiguration_t;


/* Functions Declarations */

/**
 * \brief This function initializes the I2C bus by opening the device.
 * \param [in] bus -- The I2C bus number.
 * \param [out] i2cConf -- Pointer to an allocated i2cConfiguration_t structure.
 * \returns US_I2C_SUCCESS if i2c device is successfully opened, US_I2C_ERROR otherwise.
*/
int16_t i2cInit(uint16_t bus, i2cConfiguration_t *i2cConf);


/**
 * \brief This function reads an arbitrary number of bytes from the I2C bus.
 * \param [in] i2cConf -- The i2cConfiguration_t structure.
 * \param [in] slaveAddr -- I2C slave device address.
 * \param [in] dataSize -- The number of bytes to read.
 * \param [in] regAddr -- I2C slave device register address.
 * \param [in] dataBufferSize -- Size of the output data buffer.
 * \param [out] dataBuffer -- Pointer to the data read from the I2C bus.
 * \returns US_I2C_SUCCESS if data is successfully read, US_I2C_ERROR otherwise.
*/
int16_t i2cRead(const i2cConfiguration_t *i2cConf, uint8_t slaveAddr,
                uint8_t dataSize, uint8_t addr, uint8_t dataBufferSize, 
                uint8_t *dataBuffer);

/**
 * \brief This function writes an arbitrary number of bytes on to the I2C bus.
 * \param [in] i2cConf -- The i2cConfiguration_t structure.
 * \param [in] slaveAddr -- I2C slave device address.
 * \param [in] dataSize -- The number of bytes to write.
 * \param [in] regAddr -- I2C slave device register address.
 * \param [in] dataBufferSize -- Size of the input data buffer.
 * \param [in] dataBuffer -- Pointer to the data to write on the I2C bus.
 * \returns US_I2C_SUCCESS if data is successfully written, US_I2C_ERROR otherwise.
*/
int16_t i2cWrite(const i2cConfiguration_t *i2cConf, uint8_t slaveAddr,
                uint8_t dataSize, uint8_t addr, uint8_t dataBufferSize, 
                const uint8_t *dataBuffer);

/**
 * \brief This function closes the I2C bus by closing the device.
 * \param [in] i2cConf -- The i2cConfiguration_t structure.
 * \returns US_I2C_SUCCESS if i2c device is successfully closed, US_I2C_ERROR otherwise.
*/
int16_t i2cClose(i2cConfiguration_t *i2cConf);


#ifdef __cplusplus
}
#endif

#endif // US_I2C_H