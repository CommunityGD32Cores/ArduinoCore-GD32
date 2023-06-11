/*
    Copyright (c) 2021, GigaDevice Semiconductor Inc.

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

    Based on mbed-os\targets\TARGET_GigaDevice\TARGET_GD32F30X\i2c_api.c
*/

#include "utility/twi.h"
#include "pinmap.h"
#include "twi.h"

typedef enum {
#if defined(I2C0)
    I2C0_INDEX,
#endif
#if defined(I2C1)
    I2C1_INDEX,
#endif
#if defined(I2C2)
    I2C2_INDEX,
#endif
    I2C_NUM
} internal_i2c_index_t;

static struct i2c_s *obj_s_buf[I2C_NUM] = {NULL};

#ifndef WIRE_I2C_FLAG_TIMEOUT
#define WIRE_I2C_FLAG_TIMEOUT  (0xF0000U)
#endif

#ifndef WIRE_I2C_FLAG_TIMEOUT_BUSY
#define WIRE_I2C_FLAG_TIMEOUT_BUSY WIRE_I2C_FLAG_TIMEOUT
#endif

#ifndef WIRE_I2C_FLAG_TIMEOUT_START
#define WIRE_I2C_FLAG_TIMEOUT_START WIRE_I2C_FLAG_TIMEOUT
#endif

#ifndef WIRE_I2C_FLAG_TIMEOUT_STOP_BIT_RESET
#define WIRE_I2C_FLAG_TIMEOUT_STOP_BIT_RESET WIRE_I2C_FLAG_TIMEOUT
#endif

#ifndef WIRE_I2C_FLAG_TIMEOUT_ADDR_ACK
#define WIRE_I2C_FLAG_TIMEOUT_ADDR_ACK WIRE_I2C_FLAG_TIMEOUT
#endif

#ifndef WIRE_I2C_FLAG_TIMEOUT_DATA_ACK
#define WIRE_I2C_FLAG_TIMEOUT_DATA_ACK WIRE_I2C_FLAG_TIMEOUT
#endif

#ifndef WIRE_I2C_FLAG_TIMEOUT_BYTE_TRANSMITTED
#define WIRE_I2C_FLAG_TIMEOUT_BYTE_TRANSMITTED WIRE_I2C_FLAG_TIMEOUT
#endif

#ifndef WIRE_I2C_FLAG_TIMEOUT_BYTE_RECEIVED
#define WIRE_I2C_FLAG_TIMEOUT_BYTE_RECEIVED WIRE_I2C_FLAG_TIMEOUT
#endif

#define I2C_S(obj)    (struct i2c_s *) (obj)

#if defined(GD32F1x0) || defined(GD32F3x0) || defined(GD32F4xx) || defined(GD32E23x)|| defined(GD32E50X)
#define GD32_I2C_FLAG_IS_TRANSMTR_OR_RECVR I2C_FLAG_TR
#else
#define GD32_I2C_FLAG_IS_TRANSMTR_OR_RECVR I2C_FLAG_TRS
#endif

/** Initialize the I2C peripheral
 *
 * @param obj       The I2C object
 * @param sda       The sda pin
 * @param scl       The scl pin
 * @param address   The I2C own address

 */
void i2c_init(i2c_t *obj, PinName sda, PinName scl, uint8_t address)
{
    struct i2c_s *obj_s = I2C_S(obj);
    uint32_t default_speed = 100000;
    /* find the I2C by pins */
    uint32_t i2c_sda = pinmap_peripheral(sda, PinMap_I2C_SDA);
    uint32_t i2c_scl = pinmap_peripheral(scl, PinMap_I2C_SCL);

    obj_s->sda = sda;
    obj_s->scl = scl;
    obj_s->i2c = pinmap_merge(i2c_sda, i2c_scl);

    switch (obj_s->i2c) {
        case I2C0:
            /* enable I2C0 clock and configure the pins of I2C0 */
            obj_s->index = 0;
            rcu_periph_clock_enable(RCU_I2C0);
            break;
        case I2C1:
            /* enable I2C1 clock and configure the pins of I2C1 */
            obj_s->index = 1;
            rcu_periph_clock_enable(RCU_I2C1);
            break;
#ifdef I2C2
        case I2C2:
            /* enable I2C2 clock and configure the pins of I2C2 */
            obj_s->index = 2;
            rcu_periph_clock_enable(RCU_I2C2);
            break;
#endif
        default:
            break;
    }

    /* configure the pins of I2C */
    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);

    /* I2C clock configure */
    i2c_clock_config(obj->i2c, default_speed, I2C_DTCY_2);

    /* I2C address configure */
    i2c_mode_addr_config(obj->i2c, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, address);


    /* enable I2C */
    i2c_enable(obj->i2c);
    /* enable acknowledge */
    i2c_ack_config(obj->i2c, I2C_ACK_ENABLE);
    /* get obj_s_buf */
    obj_s_buf[obj_s->index] = obj_s;
}

/** Enable the I2C interrupt
 *
 * @param obj       The I2C object
 */
void i2c_slaves_interrupt_enable(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    switch (obj_s->i2c) {
        case I2C0:
            /* enable I2C0 interrupt */
#if !defined(GD32E23x)
            nvic_irq_enable(I2C0_EV_IRQn, 1, 3);
            nvic_irq_enable(I2C0_ER_IRQn, 1, 2);
#else 
            nvic_irq_enable(I2C0_EV_IRQn, 1);
            nvic_irq_enable(I2C0_ER_IRQn, 1);
#endif           
            break;
        case I2C1:
            /* enable I2C1 interrupt */
#if !defined(GD32E23x)
            nvic_irq_enable(I2C1_EV_IRQn, 1, 3);
            nvic_irq_enable(I2C1_ER_IRQn, 1, 2);
#else 
            nvic_irq_enable(I2C1_EV_IRQn, 1);
            nvic_irq_enable(I2C1_ER_IRQn, 1);
#endif           
            break;
#ifdef I2C2
        case I2C2:
            /* enable I2C2 interrupt */
            nvic_irq_enable(I2C2_EV_IRQn, 1, 3);
            nvic_irq_enable(I2C2_ER_IRQn, 1, 2);
            break;
#endif
        default:
            break;
    }

    i2c_interrupt_enable(obj_s->i2c, I2C_INT_ERR);
    i2c_interrupt_enable(obj_s->i2c, I2C_INT_BUF);
    i2c_interrupt_enable(obj_s->i2c, I2C_INT_EV);
}

/** Write one byte (master)
 *
 * @param obj  The I2C object
 * @param data Byte to be written
 * @return I2C_NACK_DATA if NACK was received, I2C_OK if ACK was received, I2C_TIMEOUT for timeout.
 */
i2c_status_enum i2c_byte_write(i2c_t *obj, int data)
{
    uint32_t timeout;
    struct i2c_s *obj_s = I2C_S(obj);

    I2C_DATA(obj_s->i2c) = (uint8_t)data;

    /* wait until the byte is transmitted */
    timeout = WIRE_I2C_FLAG_TIMEOUT_BYTE_TRANSMITTED;
    while (((i2c_flag_get(obj_s->i2c, I2C_FLAG_TBE)) == RESET) &&
           ((i2c_flag_get(obj_s->i2c, I2C_FLAG_BTC)) == RESET)) {
        if ((timeout--) == 0) {
            return I2C_TIMEOUT;
        }
    }
    return I2C_OK;
}

/** Send STOP command
 *
 * @param obj The I2C object
 */
i2c_status_enum  i2c_stop(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    /* generate a STOP condition */
    i2c_stop_on_bus(obj_s->i2c);

    /* wait for STOP bit reset with timeout */
    int timeout = WIRE_I2C_FLAG_TIMEOUT_STOP_BIT_RESET;
    while ((I2C_CTL0(obj_s->i2c) & I2C_CTL0_STOP)) {
        if ((timeout--) == 0) {
            return I2C_TIMEOUT;
        }
    }

    return I2C_OK;
}

/** Write bytes at a given address
 *
 * @param obj     The I2C object
 * @param address 7-bit address (last bit is 0)
 * @param data    The buffer for sending
 * @param length  Number of bytes to write
 * @param stop    Stop to be generated after the transfer is done
 * @return Status
 */
i2c_status_enum i2c_master_transmit(i2c_t *obj, uint8_t address, uint8_t *data, uint16_t length,
                                    uint8_t stop)
{


    /* When size is 0, this is usually an I2C scan / ping to check if device is there and ready */
    if (length == 0) {
        return i2c_wait_standby_state(obj, address);
    }

    i2c_status_enum ret = I2C_OK;
    uint32_t timeout = 0;
    uint32_t count = 0;



    if (I2C_BUSY == _i2c_busy_wait(obj)) {
        return I2C_BUSY;
    }

    /* generate a START condition */
    i2c_start_on_bus(obj->i2c);

    /* ensure the i2c has been started successfully */
    timeout = WIRE_I2C_FLAG_TIMEOUT_START;
    while ((!i2c_flag_get(obj->i2c, I2C_FLAG_SBSEND)) && (--timeout != 0));
    if (0 == timeout) {
        return I2C_TIMEOUT;
    }

    /* send slave address */
    i2c_master_addressing(obj->i2c, address, I2C_TRANSMITTER);

    /* wait until I2C_FLAG_ADDSEND flag is set */
    timeout = WIRE_I2C_FLAG_TIMEOUT_ADDR_ACK;
    while ((!i2c_flag_get(obj->i2c, I2C_FLAG_ADDSEND)) && (--timeout != 0));
    if (0 == timeout) {
        ret = I2C_NACK_ADDR;
    }

    /* clear ADDSEND */
    i2c_flag_clear(obj->i2c, I2C_FLAG_ADDSEND);



    for (count = 0; count < length; count++) {
        if (I2C_OK != ret) {
            break;
        }
        if (I2C_OK != i2c_byte_write(obj, data[count])) {
            // If we didn't write the byte successfully,
            // we really don't want to keep trying to write subsequent
            // bytes

            ret = I2C_NACK_DATA;
        }
    }
    /* if not sequential write, then send stop */
    if (stop) {
        i2c_stop(obj);
    }
    return ret;
}

/** Read one byte
 *
 * @param obj  The I2C object
 * @param last Acknowledge
 * @return The read byte
 */
static int i2c_byte_read(i2c_t *obj, int last)
{
    struct i2c_s *obj_s = I2C_S(obj);

    if (last) {
        /* disable acknowledge */
        i2c_ack_config(obj_s->i2c, I2C_ACK_DISABLE);
    } else {
        /* enable acknowledge */
        i2c_ack_config(obj_s->i2c, I2C_ACK_ENABLE);
    }

    /* wait until the byte is received */
    uint32_t timeout = WIRE_I2C_FLAG_TIMEOUT_BYTE_RECEIVED;
    while ((i2c_flag_get(obj_s->i2c, I2C_FLAG_RBNE)) == RESET) {
        if ((timeout--) == 0) {
            return -1;
        }
    }
    return (int)I2C_DATA(obj_s->i2c);
}

/** read bytes in master mode at a given address
 *
 * @param obj     The I2C object
 * @param address 7-bit address (last bit is 1)
 * @param data    The buffer for receiving
 * @param length  Number of bytes to read
 * @param stop    Stop to be generated after the transfer is done
 * @return status
 */
i2c_status_enum i2c_master_receive(i2c_t *obj, uint8_t address, uint8_t *data, uint16_t length,
                                   int stop)
{
    i2c_status_enum ret = I2C_OK;
    uint32_t timeout = 0;
    uint32_t count = 0;

    if (I2C_BUSY == _i2c_busy_wait(obj)) {
        return I2C_BUSY;
    }



    if (1 == length) {
        /* disable acknowledge */
        i2c_ack_config(obj->i2c, I2C_ACK_DISABLE);
        /* send a stop condition to I2C bus*/
    } else if (2 == length) {
        /* send a NACK for the next data byte which will be received into the shift register */
        i2c_ackpos_config(obj->i2c, I2C_ACKPOS_NEXT);
        /* disable acknowledge */
        i2c_ack_config(obj->i2c, I2C_ACK_DISABLE);
    } else {
        /* enable acknowledge */
        i2c_ack_config(obj->i2c, I2C_ACK_ENABLE);
    }
    /* ensure the i2c has been started successfully */
    timeout = WIRE_I2C_FLAG_TIMEOUT_START;
    /* generate a START condition */
    i2c_start_on_bus(obj->i2c);
    while ((!i2c_flag_get(obj->i2c, I2C_FLAG_SBSEND)) && (--timeout != 0));
    if (0 == timeout) {
        return I2C_TIMEOUT;
    }
    /* send slave address */
    i2c_master_addressing(obj->i2c, address, I2C_RECEIVER);
    timeout = WIRE_I2C_FLAG_TIMEOUT_ADDR_ACK;
    while ((!i2c_flag_get(obj->i2c, I2C_FLAG_ADDSEND)) && (--timeout != 0));

    if (0 == timeout) {
        ret = I2C_NACK_ADDR;
    }

    /* clear ADDSEND */
    i2c_flag_clear(obj->i2c, I2C_FLAG_ADDSEND);

    for (count = 0; count < length; count++) {
        if (ret != I2C_OK) {
            break;
        }
        if (length > 2 && count == (uint32_t)length - 3) {
            timeout = WIRE_I2C_FLAG_TIMEOUT_DATA_ACK;

            while ((!i2c_flag_get(obj->i2c, I2C_FLAG_BTC)) && (--timeout != 0));

            if (0 == timeout) {
                ret = I2C_NACK_DATA;
            }

            i2c_ack_config(obj->i2c, I2C_ACK_DISABLE);
        } else if (2 == length && count == 0) {
            timeout = WIRE_I2C_FLAG_TIMEOUT_DATA_ACK;

            while ((!i2c_flag_get(obj->i2c, I2C_FLAG_BTC)) && (--timeout != 0));

            if (0 == timeout) {
                ret = I2C_NACK_DATA;
            }
        }


        timeout = WIRE_I2C_FLAG_TIMEOUT_BYTE_RECEIVED;
        while ((!i2c_flag_get(obj->i2c, I2C_FLAG_RBNE)) && (--timeout != 0));
        if (0 == timeout) {
            ret = I2C_NACK_DATA;
        } else {
            data[count] = i2c_data_receive(obj->i2c);
        };
    }
    /* if not sequential read, then send stop */
    if (stop) {
        i2c_stop(obj);
    }
    return ret;
}

/** Checks if target device is ready for communication
 *
 * @param obj     The I2C object
 * @param address 7-bit address (last bit is 1)
 * @return status
 */
i2c_status_enum i2c_wait_standby_state(i2c_t *obj, uint8_t address)
{
    __IO uint32_t val = 0;
    i2c_status_enum status = I2C_OK;
    uint32_t timeout;


    if (I2C_BUSY == _i2c_busy_wait(obj)) {
        return I2C_BUSY;
    }

    /* send a start condition to I2C bus */
    i2c_start_on_bus(obj->i2c);
    timeout = WIRE_I2C_FLAG_TIMEOUT_START;
    /* wait until SBSEND bit is set */
    while ((!i2c_flag_get(obj->i2c, I2C_FLAG_SBSEND)) && (--timeout != 0));
    if (0 == timeout) {
        status = I2C_TIMEOUT;
    }

    /* send slave address to I2C bus */
    i2c_master_addressing(obj->i2c, address, I2C_TRANSMITTER);
    timeout = WIRE_I2C_FLAG_TIMEOUT_ADDR_ACK;
    /* keep looping till the address is acknowledged or the AERR flag is set (address not acknowledged at time) */
    do {
        /* get the current value of the I2C_STAT0 register */
        val = I2C_STAT0(obj->i2c);

    } while ((0 == (val & (I2C_STAT0_ADDSEND | I2C_STAT0_AERR))) && (--timeout != 0));

    /* check if the ADDSEND flag has been set */
    if (0 == timeout) {
        status = I2C_TIMEOUT;
    } else if (val & I2C_STAT0_ADDSEND) {

        /* clear ADDSEND flag */
        i2c_flag_clear(obj->i2c, I2C_FLAG_ADDSEND);

        /* send a stop condition to I2C bus */
        i2c_stop_on_bus(obj->i2c);

        /* exit the function */
        status = I2C_OK;

    } else {
        /* clear the bit of AERR */
        i2c_flag_clear(obj->i2c, I2C_FLAG_AERR);
        status = I2C_NACK_ADDR;
    }


    // On failure to send a stop, return the timeout
    if (i2c_stop(obj) != I2C_OK) {
        return I2C_TIMEOUT;
    }

    return status;
}

/** sets function called before a slave read operation
 *
 * @param obj      The I2C object
 * @param function Callback function to use
 */
void i2c_attach_slave_rx_callback(i2c_t *obj, void (*function)(void*, uint8_t*, int), void* pWireObj)
{
    if (obj == NULL) {
        return;
    }
    /* Exit if a reception is already on-going */
    if (function == NULL) {
        return;
    }
    obj->slave_receive_callback = function;
    obj->pWireObj;
}

/** sets function called before a slave write operation
 *
 * @param obj      The I2C object
 * @param function Callback function to use
 */
void i2c_attach_slave_tx_callback(i2c_t *obj, void (*function)(void*), void* pWireObj)
{
    if (obj == NULL) {
        return;
    }
    /* Exit if a reception is already on-going */
    if (function == NULL) {
        return;
    }
    obj->slave_transmit_callback = function;
    obj->pWireObj = pWireObj;
}

/** Write bytes to master
 *
 * @param obj    The I2C object
 * @param data   The buffer for transfer
 * @param length Number of bytes to read
 * @return status
 */
i2c_status_enum i2c_slave_write_buffer(i2c_t *obj, uint8_t *data, uint16_t length){
    struct i2c_s *obj_s = I2C_S(obj);
    if (    (obj_s->tx_count + length) > obj->tx_rx_buffer_size) 
        return I2C_DATA_TOO_LONG;

    uint8_t i = 0;
    for (i; i < length; i++) {
        *obj_s->tx_buffer_ptr++ = *(data + i);
        obj_s->tx_count += length;
    }
    return I2C_OK;
}

/** Check the I2C bus to see if it's busy
 *
 * @param obj    The I2C object
 * @returns I2C_BUSY on timeout and I2C_OK otherwise
 **/

i2c_status_enum _i2c_busy_wait(i2c_t *obj)
{

    /* wait until I2C_FLAG_I2CBSY flag is reset */
    uint32_t timeout = WIRE_I2C_FLAG_TIMEOUT_BUSY;
    while ((i2c_flag_get(obj->i2c, I2C_FLAG_I2CBSY)) && (--timeout != 0));
    if (0 == timeout) {
        return I2C_BUSY;
    }

    return I2C_OK;
}

void i2c_set_clock(i2c_t *obj, uint32_t clock_hz)
{
    i2c_clock_config(obj->i2c, clock_hz, I2C_DTCY_2);
}

void i2c_err_handler(uint32_t i2c) 
{
    /* no acknowledge received */
    if (i2c_interrupt_flag_get(i2c, I2C_INT_FLAG_AERR)) {
        i2c_interrupt_flag_clear(i2c, I2C_INT_FLAG_AERR);
    }

    /* SMBus alert */
    if (i2c_interrupt_flag_get(i2c, I2C_INT_FLAG_SMBALT)) {
        i2c_interrupt_flag_clear(i2c, I2C_INT_FLAG_SMBALT);
    }

    /* bus timeout in SMBus mode */
    if (i2c_interrupt_flag_get(i2c, I2C_INT_FLAG_SMBTO)) {
        i2c_interrupt_flag_clear(i2c, I2C_INT_FLAG_SMBTO);
    }

    /* over-run or under-run when SCL stretch is disabled */
    if (i2c_interrupt_flag_get(i2c, I2C_INT_FLAG_OUERR)) {
        i2c_interrupt_flag_clear(i2c, I2C_INT_FLAG_OUERR);
    }

    /* arbitration lost */
    if (i2c_interrupt_flag_get(i2c, I2C_INT_FLAG_LOSTARB)) {
        i2c_interrupt_flag_clear(i2c, I2C_INT_FLAG_LOSTARB);
    }

    /* bus error */
    if (i2c_interrupt_flag_get(i2c, I2C_INT_FLAG_BERR)) {
        i2c_interrupt_flag_clear(i2c, I2C_INT_FLAG_BERR);
    }

    /* CRC value doesn't match */
    if (i2c_interrupt_flag_get(i2c, I2C_INT_FLAG_PECERR)) {
        i2c_interrupt_flag_clear(i2c, I2C_INT_FLAG_PECERR);
    }
}


/** This function handles I2C interrupt handler
 *
 * @param i2c_periph The I2C peripheral
 */
static void i2c_irq(struct i2c_s *obj_s)
{
    // unregistered object?
    if(obj_s == NULL) {
        return;
    }
    uint32_t i2c = obj_s->i2c;
    if (i2c_interrupt_flag_get(i2c, I2C_INT_FLAG_ADDSEND)) {
        /* clear the ADDSEND bit */
        i2c_interrupt_flag_clear(i2c, I2C_INT_FLAG_ADDSEND);
        //memset(_rx_Buffer, _rx_count, 0);
        obj_s->rx_count = 0;
        if (i2c_flag_get(i2c, GD32_I2C_FLAG_IS_TRANSMTR_OR_RECVR)) {
            obj_s->slave_transmit_callback(obj_s->pWireObj);
        }
    } else if ((i2c_interrupt_flag_get(i2c, I2C_INT_FLAG_TBE)) &&
               (!i2c_interrupt_flag_get(i2c, I2C_INT_FLAG_AERR))) {
        /* Send a data byte */
        if (obj_s->tx_count > 0) {
            i2c_data_transmit(i2c, *obj_s->tx_buffer_ptr++);
            obj_s->tx_count--;
        }
    } else if (i2c_interrupt_flag_get(i2c, I2C_INT_FLAG_RBNE)) {
        /* if reception data register is not empty, I2C1 will read a data from I2C_DATA */
        /* also check that our RX buffer has enough space for it */
        if(obj_s->rx_count < obj_s->tx_rx_buffer_size) {
            *obj_s->rx_buffer_ptr++ = i2c_data_receive(i2c);
            obj_s->rx_count++;
        } else {
            /* indicate RX buffer too small */
        }
    } else if (i2c_interrupt_flag_get(i2c, I2C_INT_FLAG_STPDET)) {
        /* clear the STPDET bit */
        i2c_enable(i2c);
        if (!i2c_flag_get(i2c, GD32_I2C_FLAG_IS_TRANSMTR_OR_RECVR)) {
            obj_s->rx_buffer_ptr = obj_s->rx_buffer_ptr - obj_s->rx_count ;
            obj_s->slave_receive_callback(obj_s->pWireObj, obj_s->rx_buffer_ptr, obj_s->rx_count);
        }
    }
}

#ifdef I2C0
/** Handle I2C0 event interrupt request
 *
 */
extern "C" void I2C0_EV_IRQHandler(void)
{
    i2c_irq(obj_s_buf[I2C0_INDEX]);
}

/** handle I2C0 error interrupt request
 *
 */
extern "C" void I2C0_ER_IRQHandler(void)
{
    i2c_err_handler(I2C0);
}
#endif

#ifdef I2C1

/** Handle I2C1 event interrupt request
 *
 */
extern "C" void I2C1_EV_IRQHandler(void)
{
    i2c_irq(obj_s_buf[I2C1_INDEX]);
}

/** handle I2C1 error interrupt request
 *
 */
extern "C" void I2C1_ER_IRQHandler(void)
{
    i2c_err_handler(I2C1);
}
#endif

#ifdef I2C2
/** Handle I2C2 event interrupt request
 *
 */
extern "C" void I2C2_EV_IRQHandler(void)
{
    i2c_irq(obj_s_buf[I2C2_INDEX]);
}

/** handle I2C1 error interrupt request
 *
 */
extern "C" void I2C2_ER_IRQHandler(void)
{
    i2c_err_handler(I2C2);
}

#endif
