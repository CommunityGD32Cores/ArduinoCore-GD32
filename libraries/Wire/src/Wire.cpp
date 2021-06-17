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

// Initialize Class Variables
uint8_t *TwoWire::rxBuffer = nullptr;
uint8_t TwoWire::rxBufferAllocated = 0;
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t *TwoWire::txBuffer = nullptr;
uint8_t TwoWire::txBufferAllocated = 0;
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(int);

TwoWire::TwoWire()
{
    _i2c.sda = DIGITAL_TO_PINNAME(SDA);
    _i2c.scl = DIGITAL_TO_PINNAME(SCL);
}

TwoWire::TwoWire(uint8_t sda, uint8_t scl)
{
    _i2c.sda = DIGITAL_TO_PINNAME(sda);
    _i2c.scl = DIGITAL_TO_PINNAME(scl);
}

// Public Methods //////////////////////////////////////////////////////////////

// void TwoWire::begin(uint8_t sda, uint8_t scl)
// {
//   _i2c.sda = DIGITAL_TO_PINNAME(sda);
//   _i2c.scl = DIGITAL_TO_PINNAME(scl);
//   begin();
// }

void TwoWire::begin(bool general_call)
{
    begin(MASTER_ADDRESS, general_call);
}

void TwoWire::begin(uint8_t address, bool general_call)
{
    rxBufferIndex = 0;
    rxBufferLength = 0;
    rxBuffer = nullptr;
    rxBufferAllocated = 0;
    resetRxBuffer();

    txBufferIndex = 0;
    txBufferLength = 0;
    txAddress = 0;
    txBuffer = nullptr;
    txBufferAllocated = 0;
    resetTxBuffer();

    user_onRequest = NULL;
    transmitting = 0;

    ownAddress = address << 1;

    if(MASTER_ADDRESS == address) {
        _i2c.i2c_master = 1;
    } else {
        _i2c.i2c_master = 0;
    }

    if(true == general_call) {
        _i2c.general_call = 1;
    } else {
        _i2c.general_call = 0;
    }

    i2c_init(&_i2c, _i2c.sda, _i2c.scl, ownAddress, _i2c.i2c_master);

    if(0 == _i2c.i2c_master) {
        i2c_attach_slave_tx_callback(&_i2c, onRequestService);
        i2c_attach_slave_rx_callback(&_i2c, onReceiveService);
    }
}

void TwoWire::begin(int address, bool general_call)
{
    begin((uint8_t)address, general_call);
}

void TwoWire::end(void)
{
    i2c_deinit(_i2c.i2c);
    free(txBuffer);
    txBuffer = nullptr;
    txBufferAllocated = 0;
    free(rxBuffer);
    rxBuffer = nullptr;
    rxBufferAllocated = 0;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize,
                             uint8_t sendStop)
{
    uint8_t read = 0;

    if(1 == _i2c.i2c_master) {
        allocateRxBuffer(quantity);
        /* error if no memory block available to allocate the buffer */
        if(rxBuffer == nullptr) {
            setWriteError();
        } else {
            if(isize > 0) {
                // send internal address; this mode allows sending a repeated start to access
                // some devices' internal registers. This function is executed by the hardware
                // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)
                beginTransmission(address);
                // the maximum size of internal address is 3 bytes
                if(isize > 3) {
                    isize = 3;
                }

                // write internal register address - most significant byte first
                while(isize-- > 0) {
                    write((uint8_t)(iaddress >> (isize * 8)));
                }
                endTransmission(false);
            }
            if(I2C_OK == i2c_master_receive(&_i2c, address << 1, rxBuffer, quantity, sendStop)) {
                read = quantity;
            }
            // set rx buffer iterator vars
            rxBufferIndex = 0;
            rxBufferLength = read;
        }
    }
    return read;
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

void TwoWire::beginTransmission(uint8_t address)
{
    // indicate that we are transmitting
    transmitting = 1;
    // set address of targeted slave
    txAddress = address << 1;
    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;
}

void TwoWire::beginTransmission(int address)
{
    beginTransmission((uint8_t)address);
}

//
//  Originally, 'endTransmission' was an f(void) function.
//  It has been modified to take one parameter indicating
//  whether or not a STOP should be performed on the bus.
//  Calling endTransmission(false) allows a sketch to
//  perform a repeated start.
//
//  WARNING: Nothing in the library keeps track of whether
//  the bus tenure has been properly ended with a STOP. It
//  is very possible to leave the bus in a hung state if
//  no call to endTransmission(true) is made. Some I2C
//  devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    int8_t ret = 4;

    if(_i2c.i2c_master == 1) {
        switch(i2c_master_transmit(&_i2c, txAddress, txBuffer, txBufferLength, sendStop)) {
            case I2C_OK :
                /* Success */
                ret = 0;
                break;
            case I2C_DATA_TOO_LONG :
                ret = 1;
                break;
            case I2C_NACK_ADDR:
                ret = 2;
                break;
            case I2C_NACK_DATA:
                ret = 3;
                break;
            case I2C_TIMEOUT:
            case I2C_BUSY:
            default:
                ret = 4;
                break;
        }
        /* reset Tx buffer */
        resetTxBuffer();

        /* reset tx buffer iterator vars */
        txBufferIndex = 0;
        txBufferLength = 0;

        /* indicate that we are done transmitting */
        transmitting = 0;
    }
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
    if(transmitting) {
        // in master transmitter mode
        allocateTxBuffer(txBufferLength + 1);
        // error if no memory block available to allocate the buffer
        if(txBuffer == nullptr) {
            setWriteError();
            ret = 0;
        } else {
            // put byte in tx buffer
            txBuffer[txBufferIndex] = data;
            ++txBufferIndex;
            // update amount in buffer
            txBufferLength = txBufferIndex;
        }
    } else {
        // in slave send mode
        // reply to master
        if(i2c_slave_write_buffer(&_i2c, &data, 1) != I2C_OK) {
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
  * @retval number of bytes ready to write.
  */
size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
    size_t ret = quantity;

    if(transmitting) {
        // in master transmitter mode
        allocateTxBuffer(txBufferLength + quantity);
        // error if no memory block available to allocate the buffer
        if(txBuffer == nullptr) {
            setWriteError();
            ret = 0;
        } else {
            // put bytes in tx buffer
            memcpy(&(txBuffer[txBufferIndex]), data, quantity);
            txBufferIndex = txBufferIndex + quantity;
            // update amount in buffer
            txBufferLength = txBufferIndex;
        }
    } else {
        // in slave send mode
        // reply to master
        if(i2c_slave_write_buffer(&_i2c, (uint8_t *)data, quantity) != I2C_OK) {
            ret = 0;
        }
    }
    return ret;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void)
{
    return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void)
{
    int value = -1;

    // get each successive byte on each call
    if(rxBufferIndex < rxBufferLength) {
        value = rxBuffer[rxBufferIndex];
        ++rxBufferIndex;

        /* Commented as not I think it is not useful
        * but kept to show that it is possible to
        * reset rx buffer when no more data available */
        /*if(rxBufferIndex == rxBufferLength) {
        resetRxBuffer();
        }*/
    }
    return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::peek(void)
{
    int value = -1;

    if(rxBufferIndex < rxBufferLength) {
        value = rxBuffer[rxBufferIndex];
    }
    return value;
}

void TwoWire::flush(void)
{
    rxBufferIndex = 0;
    rxBufferLength = 0;
    resetRxBuffer();
    txBufferIndex = 0;
    txBufferLength = 0;
    resetTxBuffer();
}

// behind the scenes function that is called when data is received
void TwoWire::onReceiveService(uint8_t *inBytes, int numBytes)
{
    // don't bother if user hasn't registered a callback
    if(user_onReceive) {
        // don't bother if rx buffer is in use by a master requestFrom() op
        // i know this drops data, but it allows for slight stupidity
        // meaning, they may not have read all the master requestFrom() data yet
        if(rxBufferIndex >= rxBufferLength) {

            allocateRxBuffer(numBytes);
            // error if no memory block available to allocate the buffer
            if(rxBuffer == nullptr) {
                // Error_Handler();
            }

            // copy twi rx buffer into local read buffer
            // this enables new reads to happen in parallel
            memcpy(rxBuffer, inBytes, numBytes);
            // set rx iterator vars
            rxBufferIndex = 0;
            rxBufferLength = numBytes;
            // alert user program
            user_onReceive(numBytes);
        }
    }
}

// behind the scenes function that is called when data is requested
void TwoWire::onRequestService(void)
{
    // don't bother if user hasn't registered a callback
    if(user_onRequest) {
        // reset tx buffer iterator vars
        // !!! this will kill any pending pre-master sendTo() activity
        txBufferIndex = 0;
        txBufferLength = 0;
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

/**
  * @brief  Allocate the Rx/Tx buffer to the requested length if needed
  * @note   Minimum allocated size is BUFFER_LENGTH)
  * @param  length: number of bytes to allocate
  */
void TwoWire::allocateRxBuffer(size_t length)
{
    if(rxBufferAllocated < length) {
        // By default we allocate BUFFER_LENGTH bytes. It is the min size of the buffer.
        if(length < BUFFER_LENGTH) {
            length = BUFFER_LENGTH;
        }
        uint8_t *tmp = (uint8_t *)realloc(rxBuffer, length * sizeof(uint8_t));
        if(tmp != nullptr) {
            rxBuffer = tmp;
            rxBufferAllocated = length;
        }
    }
}

inline void TwoWire::allocateTxBuffer(size_t length)
{
    if(txBufferAllocated < length) {
        // By default we allocate BUFFER_LENGTH bytes. It is the min size of the buffer.
        if(length < BUFFER_LENGTH) {
            length = BUFFER_LENGTH;
        }
        uint8_t *tmp = (uint8_t *)realloc(txBuffer, length * sizeof(uint8_t));
        if(tmp != nullptr) {
            txBuffer = tmp;
            txBufferAllocated = length;
        }
    }
}

/**
  * @brief  Reset Rx/Tx buffer content to 0
  */
inline void TwoWire::resetRxBuffer(void)
{
    if(rxBuffer != nullptr) {
        memset(rxBuffer, 0, rxBufferAllocated);
    }
}

inline void TwoWire::resetTxBuffer(void)
{
    if(txBuffer != nullptr) {
        memset(txBuffer, 0, txBufferAllocated);
    }
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire = TwoWire();
