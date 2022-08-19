/*
  TwoWire.cpp - TWI/I2C library for Wiring & Arduino
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
*/

extern "C" {
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
}

#include "Wire.h"
#include <functional>

#if defined(HAVE_I2C)
TwoWire Wire(SDA, SCL, 0);
#endif

TwoWire::TwoWire(uint8_t sda, uint8_t scl, int i2c_index)
{
    user_onRequest = NULL;
    transmitting = 0;
    _i2c.sda = DIGITAL_TO_PINNAME(sda);
    _i2c.scl = DIGITAL_TO_PINNAME(scl);

    _i2c.rx_buffer_ptr = _rx_buffer.buffer;
    _i2c.tx_buffer_ptr = _tx_buffer.buffer;
    _i2c.tx_count = 0;
    _i2c.rx_count = 0;
    _i2c.index = i2c_index;

    _rx_buffer.head = 0;
    _rx_buffer.tail = 0;
    _tx_buffer.head = 0;
    _tx_buffer.tail = 0;
}

/*!
    \brief      configure I2C
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TwoWire::begin()
{
    ownAddress = MASTER_ADDRESS << 1;
    i2c_init(&_i2c, _i2c.sda, _i2c.scl, ownAddress);
}

/*!
    \brief      configure slave I2C
    \param[in]  slave address
    \param[out] none
    \retval     none
*/
void TwoWire::begin(uint8_t address)
{
    ownAddress = address << 1;
    i2c_init(&_i2c, _i2c.sda, _i2c.scl, ownAddress);

    i2c_slaves_interrupt_enable(&_i2c);

    i2c_attach_slave_tx_callback(&_i2c, std::bind(&TwoWire::onRequestService, this));
    i2c_attach_slave_rx_callback(
        &_i2c, 
        std::bind(&TwoWire::onReceiveService, this, std::placeholders::_1, std::placeholders::_2)
    );
}

void TwoWire::begin(int address)
{
    begin((uint8_t)address);
}

void TwoWire::end(void)
{
    //clear any received data
    _rx_buffer.head = _rx_buffer.tail;
    //wait for any outstanding data to be sent
    flush();
    i2c_deinit(_i2c.i2c);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize,
                             uint8_t sendStop)
{

    if (isize > 0) {
        // send internal address; this mode allows sending a repeated start to access
        // some devices' internal registers. This function is executed by the hardware
        // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

        beginTransmission(address);

        // the maximum size of internal address is 3 bytes
        if (isize > 3) {
            isize = 3;
        }

        // write internal register address - most significant byte first
        while (isize-- > 0) {
            write((uint8_t)(iaddress >> (isize * 8)));
        }
        endTransmission(false);
    }

    // clamp to buffer length
    if (quantity > WIRE_BUFFER_LENGTH) {
        quantity = WIRE_BUFFER_LENGTH;
    }

    _rx_buffer.head = 0;
    if (I2C_OK == i2c_master_receive(&_i2c, address << 1, &_rx_buffer.buffer[_rx_buffer.head], quantity,
                                     sendStop)) {
        _rx_buffer.head = quantity;
    }
    // set rx buffer iterator vars
    _rx_buffer.tail = 0;
    return quantity;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

/*!
    \brief      Begin a transmission to the I2C slave device with the given address.
    \param[in]  the 7-bit address of the device to transmit to
    \param[out] none
    \retval     none
*/
void TwoWire::beginTransmission(uint8_t address)
{
    // indicate that we are transmitting
    transmitting = 1;
    // set address of targeted slave
    txAddress = address << 1;
    // reset tx buffer iterator vars
    _tx_buffer.head = 0;
    _tx_buffer.tail = 0;
}

void TwoWire::beginTransmission(int address)
{
    beginTransmission((uint8_t)address);
}

uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    int8_t ret = 4;
    ret = i2c_master_transmit(&_i2c, txAddress, &_tx_buffer.buffer[_tx_buffer.tail], _i2c.tx_count,
                              sendStop);

    _tx_buffer.head = 0;
    _tx_buffer.tail = 0;
    _i2c.tx_count = 0;
    /* indicate that we are done transmitting */
    transmitting = 0;
    return ret;
}

//  This provides backwards compatibility with the original
//  definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{
    return endTransmission((uint8_t)true);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(uint8_t data)
{
    size_t ret = 1;
    if (transmitting) {
        _tx_buffer.buffer[_tx_buffer.head] = data;
        _tx_buffer.head = (_tx_buffer.head + 1) % WIRE_BUFFER_LENGTH;
        _i2c.tx_count++;
    } else {
        // in slave send mode
        // reply to master
        if (i2c_slave_write_buffer(&_i2c, &data, 1) != I2C_OK) {
            ret = 0;
        }
    }
    return ret;
}

/**
  * @brief  This function must be called in slave Tx event callback or after
  *         beginTransmission() and before endTransmission().
  * @param  pdata: pointer to the buffer data
  * @param  quantity: number of bytes to write
  * @retval the number of bytes written
  */
size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
    size_t i;
    size_t ret = quantity;

    if (transmitting) {
        for (i = 0; i < quantity; ++i) {
            write(data[i]);
        }
    } else {
        // in slave send mode
        // reply to master
        if (i2c_slave_write_buffer(&_i2c, (uint8_t *)data, quantity) != I2C_OK) {
            ret = 0;
        }
    }
    return ret;
}

int TwoWire::available(void)
{
    return ((unsigned int)(WIRE_BUFFER_LENGTH + _rx_buffer.head - _rx_buffer.tail)) %
           WIRE_BUFFER_LENGTH;
}

int TwoWire::read(void)
{
    unsigned char c;

    if (_rx_buffer.head > _rx_buffer.tail) {

        c = _rx_buffer.buffer[_rx_buffer.tail];

        _rx_buffer.tail = (_rx_buffer.tail + 1) % WIRE_BUFFER_LENGTH;
        return c;
    } else {
        /* TODO: there are no elements in the ringbuffer... think about better error handling here! */
        return -1;
    }
}

int TwoWire::peek(void)
{
    if (_rx_buffer.head == _rx_buffer.tail) {
        return -1;
    } else {
        return _rx_buffer.buffer[_rx_buffer.tail];
    }
}

void TwoWire::flush()
{
    //wait for transmit data to be sent
    while ((_tx_buffer.head != _tx_buffer.tail)) {
        // wait for transmit data to be sent
    }
}


void TwoWire::onReceiveService(uint8_t *inBytes, int numBytes)
{

    if (user_onReceive) {
        _rx_buffer.head = numBytes;
        _rx_buffer.tail = 0;
        // alert user program
        user_onReceive(numBytes);
    }
}

// behind the scenes function that is called when data is requested
void TwoWire::onRequestService(void)
{
    // don't bother if user hasn't registered a callback
    if (user_onRequest) {
        // reset tx buffer iterator vars
        // !!! this will kill any pending pre-master sendTo() activity
        _tx_buffer.head = 0;
        _tx_buffer.tail = 0;
        // alert user program
        user_onRequest();
    }

}
// sets function called on slave write
void TwoWire::onReceive(void (*function)(int))
{
    user_onReceive = function;
}

// sets function called on slave read
void TwoWire::onRequest(void (*function)(void))
{
    user_onRequest = function;
}

void TwoWire::setClock(uint32_t clock_hz)
{
    //tests show tha clock can only be changed while the I2C peripheral is **of**.
    i2c_disable(_i2c.i2c);
    i2c_set_clock(&_i2c, clock_hz);
    i2c_enable(_i2c.i2c);
}

