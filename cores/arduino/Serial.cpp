/*
  Serial.cpp - wrapper over mbed RawSerial
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2018-2019 Arduino SA

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#include "Arduino.h"
#include "Serial.h"
//#include "drivers/UnbufferedSerial.h"
#if defined(SERIAL_CDC)
//#include "USB/PluggableUSBSerial.h"
#endif

#ifdef Serial
#undef Serial
#endif

using namespace arduino;

UART::UART(int tx, int rx, int rts, int cts) {
    _tx = digitalPinToPinName(tx);
    _rx = digitalPinToPinName(rx);
    if (rts >= 0) {
        _rts = digitalPinToPinName(rts);
    } else {
        _rts = NC;
    }
    if (cts >= 0) {
        _cts = digitalPinToPinName(cts);
    } else {
        _cts = NC;
    }
}

void UART::begin(unsigned long baudrate, uint16_t config) {

#if defined(SERIAL_CDC)
    if (is_usb) {
        return;
    }
#endif
    begin(baudrate);
    int bits = 8;
    SerialParity parity = ParityNone;
    int stop_bits = 1;

    switch (config & SERIAL_DATA_MASK) {
    case SERIAL_DATA_7:
        bits = 7;
        break;
    case SERIAL_DATA_8:
        bits = 8;
        break;
        /*
        		case SERIAL_DATA_9:
        			bits = 9;
        			break;
        */
    }

    switch (config & SERIAL_STOP_BIT_MASK) {
    case SERIAL_STOP_BIT_1:
        stop_bits = 1;
        break;
    case SERIAL_STOP_BIT_2:
        stop_bits = 2;
        break;
    }

    switch (config & SERIAL_PARITY_MASK) {
    case SERIAL_PARITY_EVEN:
        parity = ParityEven;
        break;
    case SERIAL_PARITY_ODD:
        parity = ParityOdd;
        break;
    case SERIAL_PARITY_NONE:
        parity = ParityNone;
        break;
    }

    serial_format(_serial, bits, parity, stop_bits);
}

void UART::begin(unsigned long baudrate) {
#if defined(SERIAL_CDC)
    if (is_usb) {
        return;
    }
#endif
    if (_serial == NULL) {
        serial_init(_serial, _serial->pin_tx, _serial->pin_rx);
        serial_baud(_serial, baudrate);
    } else {
        serial_baud(_serial, baudrate);
    }
    // TODO
    if (_rts != NC) {
        //	_serial->obj->set_flow_control(mbed::SerialBase::Flow::RTSCTS, _rts, _cts);
    }

    uart_attach_rx_callback(_serial, _rx_complete_irq);
}

void UART::_rx_complete_irq(serial_t *obj) {
    unsigned char c;
#if defined(SERIAL_CDC)
    if (is_usb) {
        return;
    }
#endif
    if(obj == NULL) {
        return;
    }
    if(serial_rx_active(obj)) {
        return;
    }
    if (!serial_readable(obj)) {
        return;
    }
    // No Parity error, read byte and store it in the buffer if there is room
    c = serial_getc(obj);

    uint16_t i = (unsigned int)(obj->rx_head + 1) % SERIAL_RX_BUFFER_SIZE;
    if(i != obj->rx_tail) {
        obj->rx_buffer_ptr[obj->rx_head] = c;
        obj->rx_head = i;
    }
    serial_receive(obj, &obj->rx_buffer_ptr[obj->rx_head], 1);


    // TODO
    //core_util_critical_section_exit();
}

void UART::_tx_complete_irq(serial_t *obj) {
    if(obj == NULL) {
        return;
    }
    obj->tx_tail = (obj->tx_tail + 1) % SERIAL_TX_BUFFER_SIZE;
    if(obj->tx_head == obj->tx_tail) {
    } else {
        serial_transmit(obj, &obj->tx_buffer_ptr[obj->tx_tail], 1);
    }

}


void UART::end() {
#if defined(SERIAL_CDC)
    if (is_usb) {
        return _SerialUSB.end();
    }
#endif
    //clear any received data
    _serial->rx_head = _serial->rx_tail;
    //wait for any outstanding data to be sent
    flush();
    //disable the USART
    serial_free(_serial);
}

int UART::available() {
#if defined(SERIAL_CDC)
    if (is_usb) {
        return _SerialUSB.available();
    }
#endif
    //core_util_critical_section_enter();
    int c = (SERIAL_RX_BUFFER_SIZE + _serial->rx_head - _serial->rx_tail) %
            SERIAL_RX_BUFFER_SIZE;


    //core_util_critical_section_exit();
    return c;
}

int UART::peek() {
#if defined(SERIAL_CDC)
    if (is_usb) {
        return _SerialUSB.peek();
    }
#endif
    int c;
    //core_util_critical_section_enter();

    if(_serial->rx_head == _serial->rx_tail) {
        c= -1;
    } else {
        c=  _serial->rx_buffer_ptr[_serial->rx_tail];
    }


    //core_util_critical_section_exit();
    return c;
}

int UART::read() {
#if defined(SERIAL_CDC)
    if (is_usb) {
        return _SerialUSB.read();
    }
#endif
    unsigned char c;
    //core_util_critical_section_enter();

    // if the head isn't ahead of the tail, we don't have any characters
    if(_serial->rx_head == _serial->rx_tail) {
        c= -1;
    } else {
        c = _serial->rx_buffer_ptr[_serial->rx_tail];
        _serial->rx_tail = (uint16_t)(_serial->rx_tail + 1) % SERIAL_RX_BUFFER_SIZE;
    }



    //core_util_critical_section_exit();
    return c;
}

void UART::flush() {

    // If we have never written a byte, no need to flush. This special
    // case is needed since there is no way to force the TXC (transmit
    // complete) bit to 1 during initialization
    //
    //
    //XXXX TODO
#if (0)
    if(!_written) {
        return;
    }
    //wait for transmit data to be sent
    while((_serial->tx_head != _serial->tx_tail)) {
        // wait for transmit data to be sent
    }
    // Wait for transmission to complete
    while((_serial->tx_state & OP_STATE_BUSY) != 0);

#endif

}



size_t UART::write(uint8_t c) {
#if defined(SERIAL_CDC)
    if (is_usb) {
        return _SerialUSB.write(c);
    }
#endif
    _written = true;
    uint16_t nextWrite = (_serial->tx_head + 1) % SERIAL_TX_BUFFER_SIZE;
    while(_serial->tx_tail == nextWrite) {
    }   // Spin locks if we're about to overwrite the buffer. This continues once the data is sent
    _serial->tx_buffer_ptr[_serial->tx_head] = c;
    _serial->tx_head = nextWrite;

    _serial->tx_count++;

    if(!serial_tx_active(_serial)) {
        uart_attach_tx_callback(_serial, _tx_complete_irq);
        serial_transmit(_serial, &_serial->tx_buffer_ptr[_serial->tx_tail], 1);

    }
    // XXX TODO arduino wants us to return the length of what we wrote
    return 1;

}

void UART::block_tx(int _a) {
    _block = false;
}

UART::operator bool() {
#if defined(SERIAL_CDC)
    if (is_usb) {
        return _SerialUSB;
    }
#endif
    return _serial != NULL;
}

#if 0
UART::operator mbed::FileHandle*() {
#if defined(SERIAL_CDC)
    if (is_usb) {
        return &_SerialUSB;
    }
#endif
    return _serial;
}
#endif

#if defined(SERIAL_CDC)
uint32_t UART::baud() {
    if (is_usb) {
        return _SerialUSB.baud();
    }
    return 0;
}
uint8_t UART::stopbits() {
    if (is_usb) {
        return _SerialUSB.stopbits();
    }
    return 0;
}
uint8_t UART::paritytype() {
    if (is_usb) {
        return _SerialUSB.paritytype();
    }
    return 0;
}
uint8_t UART::numbits() {
    if (is_usb) {
        return _SerialUSB.numbits();
    }
    return 0;
}
bool UART::dtr() {
    if (is_usb) {
        return _SerialUSB.dtr();
    }
    return false;
}
bool UART::rts() {
    if (is_usb) {
        return _SerialUSB.rts();
    }
    return false;
}
#endif

#if defined(SERIAL_CDC)
UART _UART_USB_;
#endif

#if SERIAL_HOWMANY > 0

#ifdef SERIAL1_RTS
UART _UART1_(SERIAL1_TX, SERIAL1_RX, SERIAL1_RTS, SERIAL1_CTS);
#else
UART _UART1_(SERIAL1_TX, SERIAL1_RX, NC, NC);
#endif

#if SERIAL_HOWMANY > 1

#ifdef SERIAL2_RTS
UART _UART2_(SERIAL2_TX, SERIAL2_RX, SERIAL2_RTS, SERIAL2_CTS);
#else
UART _UART2_(SERIAL2_TX, SERIAL2_RX, NC, NC);
#endif

#if SERIAL_HOWMANY > 2

#ifdef SERIAL3_RTS
UART _UART3_(SERIAL3_TX, SERIAL3_RX, SERIAL3_RTS, SERIAL3_CTS);
#else
UART _UART3_(SERIAL3_TX, SERIAL3_RX, NC, NC);
#endif

#if SERIAL_HOWMANY > 3

#ifdef SERIAL4_RTS
UART _UART4_(SERIAL4_TX, SERIAL4_RX, SERIAL4_RTS, SERIAL4_CTS);
#else
UART _UART4_(SERIAL4_TX, SERIAL4_RX, NC, NC);
#endif

#endif
#endif
#endif
#endif
