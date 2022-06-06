/*
  HardwareSerial.cpp - Hardware serial library for Wiring
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

  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
*/

#include <stdio.h>
#include "Arduino.h"
#include "HardwareSerial.h"
//#if defined(HAVE_HWSERIAL) || defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3)

// SerialEvent functions are weak, so when the user doesn't define them,
// the linker just sets their address to 0 (which is checked below).
// The Serialx_available is just a wrapper around Serialx.available(),
// but we can refer to it weakly so we don't pull in the entire
// HardwareSerial instance if the user doesn't also refer to it.

#if defined(HAVE_HWSERIAL1)
HardwareSerial Serial1(RX0, TX0, 0);
void serialEvent1() __attribute__((weak));
bool Serial1_available()
{
    return Serial1.available() > 0;
}
#endif

#if defined(HAVE_HWSERIAL2)
HardwareSerial Serial2(RX1, TX1, 1);
void serialEvent2() __attribute__((weak));
bool Serial2_available()
{
    return Serial2.available() > 0;
}
#endif

#if defined(HAVE_HWSERIAL3)
HardwareSerial Serial3(RX2, TX2, 2);
void serialEvent3() __attribute__((weak));
bool Serial3_available()
{
    return Serial3.available() > 0;
}
#endif

#if defined(HAVE_HWSERIAL4)
HardwareSerial Serial4(RX3, TX3, 3);
void serialEvent4() __attribute__((weak));
bool Serial4_available()
{
    return Serial4.available() > 0;
}
#endif

#if defined(HAVE_HWSERIAL5)
HardwareSerial Serial5(RX4, TX4, 4);
void serialEvent5() __attribute__((weak));
bool Serial5_available()
{
    return Serial5.available() > 0;
}
#endif


void serialEventRun(void)
{
#if defined(HAVE_HWSERIAL1)
    if (serialEvent1 && Serial1_available()) {
        serialEvent1();
    }
#endif
#if defined(HAVE_HWSERIAL2)
    if (serialEvent2 && Serial2_available()) {
        serialEvent2();
    }
#endif
#if defined(HAVE_HWSERIAL3)
    if (serialEvent3 && Serial3_available()) {
        serialEvent3();
    }
#endif
#if defined(HAVE_HWSERIAL4)
    if (serialEvent4 && Serial4_available()) {
        serialEvent4();
    }
#endif
#if defined(HAVE_HWSERIAL5)
    if(serialEvent5 && Serial5_available()) {
        serialEvent5();
    }
#endif
}
ring_buffer_r HardwareSerial::_rx_buffer = {{0}, 0, 0};
ring_buffer_t HardwareSerial::_tx_buffer = {{0}, 0, 0};


HardwareSerial::HardwareSerial(uint8_t rx, uint8_t tx, int uart_index)
{
    _serial.pin_rx = DIGITAL_TO_PINNAME(rx);
    _serial.pin_tx =  DIGITAL_TO_PINNAME(tx);
    _serial.rx_buffer_ptr = _rx_buffer.buffer;
    _serial.tx_buffer_ptr = _tx_buffer.buffer;
    _rx_buffer.head = 0;
    _rx_buffer.tail = 0;
    _tx_buffer.head = 0;
    _tx_buffer.tail = 0;
    _serial.tx_count = 0;
    _serial.index = uart_index;

}

void HardwareSerial::begin(unsigned long baud, uint8_t config)
{
    uint32_t databits = 0;
    uint32_t stopbits = 0;
    SerialParity parity;
    // Manage databits
    switch (config & 0x07) {
        case 0x04:
            databits = 7;
            break;
        case 0x06:
            databits = 8;
            break;
        default:
            databits = 0;
            break;
    }

    if ((config & 0x30) == 0x30) {
        parity = ParityOdd;
        databits++;
    } else if ((config & 0x20) == 0x20) {
        parity = ParityEven;
        databits++;
    } else {
        parity = ParityNone;
    }

    if ((config & 0x08) == 0x08) {
        stopbits = 2;
    } else {
        stopbits = 1;
    }

    serial_init(&_serial, _serial.pin_tx, _serial.pin_rx);
    serial_baud(&_serial, baud);
    serial_format(&_serial, databits, parity, stopbits);

    uart_attach_rx_callback(&_serial, _rx_complete_irq);
    serial_receive(&_serial, &_rx_buffer.buffer[_rx_buffer.head], 1);

}

void HardwareSerial::end()
{
    //clear any received data
    _rx_buffer.head = _rx_buffer.tail;
    //wait for any outstanding data to be sent
    flush();
    //disable the USART
    serial_free(&_serial);
}

int HardwareSerial::available(void)
{
    return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer.head - _rx_buffer.tail)) %
           SERIAL_RX_BUFFER_SIZE;
}
int HardwareSerial::peek(void)
{
    if (_rx_buffer.head == _rx_buffer.tail) {
        return -1;
    } else {
        return _rx_buffer.buffer[_rx_buffer.tail];
    }
}

int HardwareSerial::read(void)
{
    unsigned char c;

    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer.head == _rx_buffer.tail) {
        return -1;
    } else {
        c = _rx_buffer.buffer[_rx_buffer.tail];
        _rx_buffer.tail = (rx_buffer_index_t)(_rx_buffer.tail + 1) % SERIAL_RX_BUFFER_SIZE;
        return c;
    }
}

int HardwareSerial::availableForWrite(void)
{
    tx_buffer_index_t head = _tx_buffer.head;
    tx_buffer_index_t tail = _tx_buffer.tail;

    if (head >= tail) {
        return SERIAL_TX_BUFFER_SIZE - 1 - head + tail;
    }
    return tail - head - 1;
}

void HardwareSerial::flush()
{
    // If we have never written a byte, no need to flush. This special
    // case is needed since there is no way to force the TXC (transmit
    // complete) bit to 1 during initialization
    if (!_written) {
        return;
    }
    //wait for transmit data to be sent
    while ((_tx_buffer.head != _tx_buffer.tail)) {
        // wait for transmit data to be sent
    }
    // Wait for transmission to complete
    while ((_serial.tx_state & OP_STATE_BUSY) != 0);
}

size_t HardwareSerial::write(uint8_t c)
{
    _written = true;
    tx_buffer_index_t nextWrite = (_tx_buffer.head + 1) % SERIAL_TX_BUFFER_SIZE;
    while (_tx_buffer.tail == nextWrite) {
    }   // Spin locks if we're about to overwrite the buffer. This continues once the data is sent
    _tx_buffer.buffer[_tx_buffer.head] = c;
    _tx_buffer.head = nextWrite;

    _serial.tx_count++;

    if (!serial_tx_active(&_serial)) {
        uart_attach_tx_callback(&_serial, _tx_complete_irq);
        serial_transmit(&_serial, &_tx_buffer.buffer[_tx_buffer.tail], 1);

    }
    return 1;
}

void HardwareSerial::_rx_complete_irq(serial_t *obj)
{
    // No Parity error, read byte and store it in the buffer if there is room
    unsigned char c;
    if (obj == NULL) {
        return;
    }
    if (serial_rx_active(obj)) {
        return;
    }
    c = serial_getc(obj);
    rx_buffer_index_t i = (unsigned int)(_rx_buffer.head + 1) % SERIAL_RX_BUFFER_SIZE;
    if (i != _rx_buffer.tail) {
        _rx_buffer.buffer[_rx_buffer.head] = c;
        _rx_buffer.head = i;
    }
    serial_receive(obj, &_rx_buffer.buffer[_rx_buffer.head], 1);
}

void HardwareSerial::_tx_complete_irq(serial_t *obj)
{
    if (obj == NULL) {
        return;
    }
    _tx_buffer.tail = (_tx_buffer.tail + 1) % SERIAL_TX_BUFFER_SIZE;
    if (_tx_buffer.head == _tx_buffer.tail) {
    } else {
        serial_transmit(obj, &_tx_buffer.buffer[_tx_buffer.tail], 1);
    }
}
































