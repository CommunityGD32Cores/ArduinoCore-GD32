/*
  TwoWire.h - TWI/I2C library for Arduino & Wiring
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

#ifndef TwoWire_h
#define TwoWire_h

#include "api/Stream.h"
#include "Arduino.h"
#include "utility/twi.h"

#if !defined(WIRE_BUFFER_LENGTH)
#define WIRE_BUFFER_LENGTH 32
#endif

#define MASTER_ADDRESS 0x33

typedef struct {
    unsigned char buffer[WIRE_BUFFER_LENGTH];
    int head;
    int tail;
} ring_buffer;

class TwoWire : public Stream
{
    private:
        uint8_t txAddress = 0;


        uint8_t transmitting;

        uint8_t ownAddress;
        i2c_t _i2c;

        static void onRequestService(void* pWireObj);
        static void onReceiveService(void* pWireObj, uint8_t *, int);

    protected:
        ring_buffer _rx_buffer = {{0}, 0, 0};;
        ring_buffer _tx_buffer = {{0}, 0, 0};;
        void (*user_onRequest)(void);
        void (*user_onReceive)(int);

    public:

        TwoWire(uint8_t sda, uint8_t scl, int i2c_index);

        void begin();
        void begin(uint8_t address);
        void begin(int);
        void end();
        void setClock(uint32_t);
        void beginTransmission(uint8_t);
        void beginTransmission(int);
        uint8_t endTransmission(void);
        uint8_t endTransmission(uint8_t);
        uint8_t requestFrom(uint8_t, uint8_t);
        uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
        uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
        uint8_t requestFrom(int, int);
        uint8_t requestFrom(int, int, int);
        virtual size_t write(uint8_t);
        virtual size_t write(const uint8_t *, size_t);
        virtual int available(void);
        virtual int read(void);
        virtual int peek(void);
        virtual void flush(void);
        void onReceive(void (*)(int));
        void onRequest(void (*)(void));

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
        using Print::write;
};

#if defined(HAVE_I2C)
extern TwoWire Wire;
#endif

#if defined(HAVE_I2C1)
extern TwoWire Wire1;
#endif

#if defined(HAVE_I2C2)
extern TwoWire Wire2;
#endif

#endif
