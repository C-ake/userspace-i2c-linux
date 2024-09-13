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
 * \file us-i2c.c
 * \brief This file contains the definitions for a Linux userspace I2C library,
 * useful to quickly manage and control I2C peripherals.
 * \author Giuseppe Giglio <g.giglio001@gmail.com>
 * \version 1.0.0 - First release
*/

/* Standard library includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* Linux os includes */
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

/* Local includes */
#include "us-i2c.h"


/* Function definitions */

int16_t i2cInit(uint16_t bus, i2cConfiguration_t *i2cConf)
{
    char *i2cDevPath = NULL;

    /* Verifying input */
    if (i2cConf == NULL)
    {
        return US_I2C_ERROR;
    }

    /* Allocating memory for the I2C device path */
    i2cDevPath = (char *)malloc(I2C_DEV_PATH_PREFIX_SIZE + 1);

    /* Verifying memory allocation */
    if (i2cDevPath == NULL)
    {
        return US_I2C_ERROR;
    }

    /* Building the I2C device path */
    (void)snprintf(i2cDevPath, I2C_DEV_PATH_PREFIX_SIZE + 2, "%s%d", I2C_DEV_PATH_PREFIX, bus);

    /* Opening the I2C device */
    i2cConf->busFD = open(i2cDevPath, O_RDWR);

    /* Verifying the device has been opened */
    if (i2cConf->busFD < 0)
    {
        /* Freeing the allocated memory */
        (void)free(i2cDevPath);
        (void)perror("Error opening I2C device, maybe permissions?\n");
        return US_I2C_ERROR;
    }

    (void)free(i2cDevPath);
    return US_I2C_SUCCESS;
}


int16_t i2cRead(const i2cConfiguration_t *i2cConf, uint8_t slaveAddr,
                uint8_t dataSize, uint8_t addr, uint8_t dataBufferSize, 
                uint8_t *dataBuffer)
{
    
    /* Validating input */
    if (i2cConf == NULL || dataBuffer == NULL)
    {
        return US_I2C_ERROR;
    }

    if (i2cConf->busFD <= 0)
    {
        return US_I2C_ERROR;
    }

    if (dataSize > dataBufferSize)
    {
        return US_I2C_ERROR;
    }

    /* Setting the slave address */
    if (ioctl(i2cConf->busFD, I2C_SLAVE, slaveAddr) < 0)
    {
        (void)perror("Error setting I2C slave address\n");
        return US_I2C_ERROR;
    }

    /* Reading data from the I2C bus */

    /* Writing register address to I2C bus */
    if (write(i2cConf->busFD, &addr, I2C_REGISTER_ADDR_SIZE) != I2C_REGISTER_ADDR_SIZE)
    {
        (void)perror("Error writing register address to I2C bus\n");
        return US_I2C_ERROR;
    }


    /* Reading data from the I2C bus */
    if (read(i2cConf->busFD, dataBuffer, dataSize) != dataSize)
    {
        (void)perror("Error reading data from I2C bus\n");
        return US_I2C_ERROR;
    }

    return US_I2C_SUCCESS;

}

int16_t i2cWrite(const i2cConfiguration_t *i2cConf, uint8_t slaveAddr,
                uint8_t dataSize, uint8_t addr, uint8_t dataBufferSize, 
                const uint8_t *dataBuffer)
{
    uint16_t txBufferSize = dataSize + I2C_REGISTER_ADDR_SIZE;
    uint8_t *txBuffer = (uint8_t *) calloc(txBufferSize, sizeof(uint8_t));

    /* Validating input */
    if (i2cConf == NULL || dataBuffer == NULL)
    {
        return US_I2C_ERROR;
    }

    if (i2cConf->busFD <= 0)
    {
        return US_I2C_ERROR;
    }

    if (dataSize > dataBufferSize)
    {
        return US_I2C_ERROR;
    }

    /* Preparing txBuffer */
    txBuffer[0] = addr;
    (void)memcpy(&txBuffer[I2C_REGISTER_ADDR_SIZE], dataBuffer, dataSize);

    /* Setting the slave address */
    if (ioctl(i2cConf->busFD, I2C_SLAVE, slaveAddr) < 0)
    {
        (void)perror("Error setting I2C slave address\n");
        return US_I2C_ERROR;
    }

    /* Writing data to the I2C bus */
    if (write(i2cConf->busFD, txBuffer, txBufferSize) != txBufferSize)
    {
        (void)perror("Error writing data to I2C bus\n");
        return US_I2C_ERROR;
    }

    return US_I2C_SUCCESS;
}

int16_t i2cClose(i2cConfiguration_t *i2cConf)
{
    /* Validating input */
    if (i2cConf == NULL)
    {
        return US_I2C_ERROR;
    }

    /* Closing the I2C device */
    if (close(i2cConf->busFD) < 0)
    {
        (void)perror("Error closing I2C device\n");
        return US_I2C_ERROR;
    }

    i2cConf->busFD = -1;

    return US_I2C_SUCCESS;
}