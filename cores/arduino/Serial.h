/*
  Serial.h - wrapper over GD32 serial
  Ported from the mbed core - Part of Arduino - http://www.arduino.cc/

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

#include "api/RingBuffer.h"
#include "Arduino.h"
#include "api/HardwareSerial.h"
#include "PinNames.h"
#include "gd32/uart.h"

#ifdef __cplusplus

#ifndef __ARDUINO_UART_IMPLEMENTATION__
#define __ARDUINO_UART_IMPLEMENTATION__

#if !defined(SERIAL_TX_BUFFER_SIZE)
    #define SERIAL_TX_BUFFER_SIZE 64
#endif
#if !defined(SERIAL_RX_BUFFER_SIZE)
    #define SERIAL_RX_BUFFER_SIZE 64
#endif


namespace arduino {

class UART : public HardwareSerial {
        public:
                UART(int tx, int rx, int rts = -1, int cts = -1);
                UART(PinName tx, PinName rx, PinName rts = NC, PinName cts = NC) : _tx(tx), _rx(rx), _rts(rts), _cts(cts) {}
                UART() {
                        is_usb = true;
                }
                void begin(unsigned long);
                void begin(unsigned long baudrate, uint16_t config);
                void end();
                int available(void);
                int peek(void);
                int read(void);
                void flush(void);
                size_t write(uint8_t c);
                size_t write(uint8_t c, size_t s);
                using Print::write; // pull in write(str) and write(buf, size) from Print
                operator bool();

#if defined(SERIAL_CDC)
                uint32_t baud();
                uint8_t stopbits();
                uint8_t paritytype();
                uint8_t numbits();
                bool dtr();
                bool rts();
#endif

        private:
		    // Interrupt handlers
    		static void _rx_complete_irq(serial_t *obj);
    		static void _tx_complete_irq(serial_t *obj);

                void block_tx(int);
                bool _block;
                const size_t WRITE_BUFF_SZ = SERIAL_TX_BUFFER_SIZE;
                serial_t* _serial = NULL;
                //mbed_usb_serial* _usb_serial = NULL;
    		// Has any byte been written to the UART since begin()
    		volatile bool _written;

                PinName _tx, _rx, _rts, _cts;
                RingBufferN<SERIAL_RX_BUFFER_SIZE> rx_buffer;
                uint8_t intermediate_buf[4];
                bool is_usb = false;

};
}

extern arduino::UART _UART1_;
extern arduino::UART _UART2_;
extern arduino::UART _UART3_;
extern arduino::UART _UART4_;
extern arduino::UART _UART_USB_;

#endif
#endif



