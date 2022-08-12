/*
  HardwareSerial.h - Hardware serial library for Wiring
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

  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>

#include "api/Stream.h"
#include "uart.h"


// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which head is the index of the location
// to which to write the next incoming character and tail is the index of the
// location from which to read.
// NOTE: a "power of 2" buffer size is reccomended to dramatically
//       optimize all the modulo operations for ring buffers.
// WARNING: When buffer sizes are increased to > 256, the buffer index
// variables are automatically increased in size, but the extra
// atomicity guards needed for that are not implemented. This will
// often work, but occasionally a race condition can occur that makes
// Serial behave erratically. See https://github.com/arduino/Arduino/issues/2405

#if !defined(SERIAL_TX_BUFFER_SIZE)
#define SERIAL_TX_BUFFER_SIZE 64
#endif
#if !defined(SERIAL_RX_BUFFER_SIZE)
#define SERIAL_RX_BUFFER_SIZE 64
#endif
#if (SERIAL_TX_BUFFER_SIZE>256)
typedef uint16_t tx_buffer_index_t;
#else
typedef uint8_t tx_buffer_index_t;
#endif
#if  (SERIAL_RX_BUFFER_SIZE>256)
typedef uint16_t rx_buffer_index_t;
#else
typedef uint8_t rx_buffer_index_t;
#endif


typedef struct {
    unsigned char buffer[SERIAL_RX_BUFFER_SIZE];
    volatile int head;
    volatile int tail;
} ring_buffer_r;

typedef struct {
    unsigned char buffer[SERIAL_TX_BUFFER_SIZE];
    volatile int head;
    volatile int tail;
} ring_buffer_t;

#define SERIAL_8N1 0x06
#define SERIAL_8N2 0x0E
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

class HardwareSerial : public Stream
{
    protected:
        // Has any byte been written to the UART since begin()
        volatile bool _written;
        // Don't put any members after these buffers, since only the first
        // 32 bytes of this struct can be accessed quickly using the ldd
        // instruction.
        unsigned char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
        unsigned char _tx_buffer[SERIAL_TX_BUFFER_SIZE];
        serial_t _serial;

    public:
        HardwareSerial(uint8_t rx, uint8_t tx, int uart_index);
        void begin(unsigned long baud)
        {
            begin(baud, SERIAL_8N1);
        }
        void begin(unsigned long, uint8_t);
        void end();
        virtual int available(void);
        virtual int peek(void);
        virtual int read(void);
        int availableForWrite(void);
        virtual void flush(void);
        virtual size_t write(uint8_t);
        inline size_t write(unsigned long n)
        {
            return write((uint8_t)n);
        }
        inline size_t write(long n)
        {
            return write((uint8_t)n);
        }
        inline size_t write(unsigned int n)
        {
            return write((uint8_t)n);
        }
        inline size_t write(int n)
        {
            return write((uint8_t)n);
        }
        using Print::write; // pull in write(str) and write(buf, size) from Print
        operator bool()
        {
            return true;
        }

        // Interrupt handlers
        static void _rx_complete_irq(serial_t *obj);
        static void _tx_complete_irq(serial_t *obj);

        // helper func for linker
        static int availableSerialN(unsigned n);

    private:

};

/*
 * ‘Serial’ is for the CDC-ACM if enabled. Hardware serial peripherals begin at
 * ‘Serial1’.
 */

#ifndef DEFAULT_HWSERIAL_INSTANCE 
#define DEFAULT_HWSERIAL_INSTANCE 1
#endif

/* Macro-define Serial to actual serial instance. We don't yet have a selection mechanism, use the first one. */
#if !defined(USBD_USE_CDC)
#if !defined(Serial)
#if DEFAULT_HWSERIAL_INSTANCE == 1
#define Serial Serial1
#elif DEFAULT_HWSERIAL_INSTANCE == 2
#define Serial Serial2
#elif DEFAULT_HWSERIAL_INSTANCE == 3
#define Serial Serial3
#elif DEFAULT_HWSERIAL_INSTANCE == 4
#define Serial Serial4
#elif DEFAULT_HWSERIAL_INSTANCE == 5
#define Serial Serial5
#endif /* DEFAULT_HWSERIAL_INSTANCE */
#endif /* Serial */
#endif /* USBD_USE_CDC */

#if defined(HAVE_HWSERIAL1)
extern HardwareSerial Serial1;
#endif

#if defined(HAVE_HWSERIAL2)
extern HardwareSerial Serial2;
#endif

#if defined(HAVE_HWSERIAL3)
extern HardwareSerial Serial3;
#endif

#if defined(HAVE_HWSERIAL4)
extern HardwareSerial Serial4;
#endif

#if defined(HAVE_HWSERIAL5)
extern HardwareSerial Serial5;
#endif

extern void serialEventRun(void) __attribute__((weak));
#endif
