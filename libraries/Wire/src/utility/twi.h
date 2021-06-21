/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.

    Based on mbed-os/hal/include/hal/i2c_api.h
*/

#ifndef __TWI_H__
#define __TWI_H__

#include "gd32_def.h"
#include "PeripheralPins.h"
#include "gd32f30x.h"

#ifdef __cplusplus
extern "C" {
#endif

/* I2C Tx/Rx buffer size */
#if !defined(I2C_BUFFER_SIZE)
#define I2C_BUFFER_SIZE    32
#elif (I2C_BUFFER_SIZE >= 256)
#error I2C buffer size cannot exceed 255
#endif

typedef struct i2c_s i2c_t;

struct i2c_s {
    /* basic information */
    uint32_t i2c;
    uint8_t index;
    PinName sda;
    PinName scl;
    /* operating parameters */
    uint8_t    *tx_buffer_ptr;
    uint8_t    *rx_buffer_ptr;
    uint16_t   tx_count;
    uint16_t   rx_count;

    void (*slave_transmit_callback)(void);
    void (*slave_receive_callback)(uint8_t *, int);
};

typedef enum {
    /* transfer status */
    I2C_OK            = 0,
    I2C_DATA_TOO_LONG = 1,
    I2C_NACK_ADDR     = 2,
    I2C_NACK_DATA     = 3,
    I2C_ERROR         = 4,
    I2C_TIMEOUT       = 5,
    I2C_BUSY          = 6
} i2c_status_enum;

/* Initialize the I2C peripheral */
void i2c_init(i2c_t *obj, PinName sda, PinName scl, uint8_t address);
/* Enable the I2C interrupt */
void i2c_slaves_interrupt_enable(i2c_t *obj);
/* Write one byte */
i2c_status_enum i2c_master_transmit(i2c_t *obj, uint8_t dev_address, uint8_t *data, uint16_t size,
                                    uint8_t stop);
/* Write bytes at a given address */
i2c_status_enum i2c_master_receive(i2c_t *obj, uint8_t address, uint8_t *data, uint16_t length,
                                   int stop);
/* read bytes in master mode at a given address */
i2c_status_enum i2c_wait_standby_state(i2c_t *obj, uint8_t address);
/* sets function called before a slave read operation */
void i2c_attach_slave_rx_callback(i2c_t *obj, void (*function)(uint8_t *, int));
/* sets function called before a slave write operation */
void i2c_attach_slave_tx_callback(i2c_t *obj, void (*function)(void));
/* Write bytes to master */
i2c_status_enum i2c_slave_write_buffer(i2c_t *obj, uint8_t *data, uint16_t size);
/* set I2C clock speed */
void i2c_set_clock(i2c_t *obj, uint32_t clock_hz);



#ifdef __cplusplus
}
#endif

#endif /* __TWI_H__ */
