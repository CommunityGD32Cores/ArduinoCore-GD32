/*
SoftwareSerial.cpp (formerly NewSoftSerial.cpp) -
Multi-instance software serial library for Arduino/Wiring
-- Interrupt-driven receive and other improvements by ladyada
   (http://ladyada.net)
-- Tuning, circular buffer, derivation from class Print/Stream,
   multi-instance support, porting to 8MHz processors,
   various optimizations, PROGMEM delay tables, inverse logic and
   direct port writing by Mikal Hart (http://www.arduiniana.org)
-- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
-- 20MHz processor support by Garrett Mace (http://www.macetech.com)
-- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

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

The latest version of this library can always be found at
http://arduiniana.org.
*/
#include "SoftwareSerial.h"

#define OVERSAMPLE 3 // in RX, Timer will generate interruption OVERSAMPLE time during a bit. Thus OVERSAMPLE ticks in a bit. (interrupt not synchonized with edge).

//#define TIMER_SERIAL TIMER1    TIMER1 timer1 is occupied

//Default timer13

#if defined (TIMER13)
#define TIMER_SERIAL TIMER13
#elif defined (TIMER12)
#define TIMER_SERIAL TIMER112
#elif defined (TIMER11)
#define TIMER_SERIAL TIMER111
#elif defined (TIMER10)
#define TIMER_SERIAL TIMER110
#elif defined (TIMER9)
#define TIMER_SERIAL TIMER19
#elif defined (TIMER8)
#define TIMER_SERIAL TIMER18
#elif defined (TIMER7)
#define TIMER_SERIAL TIMER7
#elif defined (TIMER6)
#define TIMER_SERIAL TIMER6
#elif defined (TIMER5)
#define TIMER_SERIAL TIMER5
#elif defined (TIMER4)
#define TIMER_SERIAL TIMER4
#elif defined (TIMER3)
#define TIMER_SERIAL TIMER3
#elif defined (TIMER2)
#define TIMER_SERIAL TIMER2
#elif defined (TIMER1)
#define TIMER_SERIAL TIMER1
#endif

HardwareTimer SoftwareSerial::timer(TIMER_SERIAL);
SoftwareSerial *SoftwareSerial::active_listener = nullptr;
SoftwareSerial *volatile SoftwareSerial::active_out = nullptr;
SoftwareSerial *volatile SoftwareSerial::active_in = nullptr;
int32_t SoftwareSerial::tx_tick_cnt = 0; // OVERSAMPLE ticks needed for a bit
int32_t volatile SoftwareSerial::rx_tick_cnt = 0;  // OVERSAMPLE ticks needed for a bit
uint32_t SoftwareSerial::tx_buffer = 0;
int32_t SoftwareSerial::tx_bit_cnt = 0;
uint32_t SoftwareSerial::rx_buffer = 0;
int32_t SoftwareSerial::rx_bit_cnt = -1; // rx_bit_cnt = -1 :  waiting for start bit
uint32_t SoftwareSerial::cur_speed = 0;

SoftwareSerial::SoftwareSerial(uint16_t receivePin, uint16_t transmitPin,
                               bool inverse_logic /* = false */)
{
    _receivePin = receivePin;
    _transmitPin = transmitPin;
    _transmitPinPort = DIGITAL_PIN_TO_PORT((transmitPin));
    _receivePinPort = DIGITAL_PIN_TO_PORT((receivePin));
    _receivePinNumber = gpio_pin[GD_PIN_GET(DIGITAL_TO_PINNAME(receivePin))];
    _transmitPinNumber = gpio_pin[GD_PIN_GET(DIGITAL_TO_PINNAME(transmitPin))];
    _speed = 0;
    _buffer_overflow = false;
    _inverse_logic = inverse_logic;
    _output_pending = 0;
}

void SoftwareSerial::setSpeed(uint32_t speed)
{
    if (speed != cur_speed) {
        timer.stop();
        if (speed != 0) {
            // Disable the timer
            uint32_t clock_rate, cmp_value;
            // Get timer clock
            clock_rate = timer.getTimerClkFre();
            int pre = 1;
            // Calculate prescale an compare value
            do {
                cmp_value = clock_rate / (speed * OVERSAMPLE);
                if (cmp_value >= UINT16_MAX) {
                    clock_rate = clock_rate / 2;
                    pre *= 2;
                }
            } while (cmp_value >= UINT16_MAX);
            timer.setPrescaler(pre);
            timer.setReloadValue(cmp_value);
            timer.setCounter(0);

            timer.attachInterrupt(&handleInterrupt);
            timer.start();
            timer.refresh();
        } else {
            timer.detachInterrupt();
        }
        cur_speed = speed;
    }
}

// This function sets the current object as the "listening"
// one and returns true if it replaces another
bool SoftwareSerial::listen()
{
    if (active_listener != this) {
        // wait for any transmit to complete as we may change speed
        while (active_out);
        active_listener->stopListening();
        rx_tick_cnt =
            1; // 1 : next interrupt will decrease rx_tick_cnt to 0 which means RX pin level will be considered.
        rx_bit_cnt = -1; // rx_bit_cnt = -1 :  waiting for start bit
        setSpeed(_speed);
        active_listener = this;
        active_in = this;
        return true;
    }
    return false;
}

// Stop listening. Returns true if we were actually listening.
bool SoftwareSerial::stopListening()
{
    if (active_listener == this) {
        // wait for any output to complete
        while (active_out);
        active_listener = nullptr;
        active_in = nullptr;
        // turn off ints
        setSpeed(0);
        return true;
    }
    return false;
}

inline void SoftwareSerial::send()
{
    if (--tx_tick_cnt <=
        0) {  // if tx_tick_cnt > 0 interrupt is discarded. Only when tx_tick_cnt reach 0 we set TX pin.
        if (tx_bit_cnt++ <
            10) {  // tx_bit_cnt < 10 transmission is not finished (10 = 1 start +8 bits + 1 stop)
            // send data (including start and stop bits)
            if (tx_buffer & 1) {
                gpio_bit_set(_transmitPinPort, _transmitPinNumber);
            } else {
                gpio_bit_reset(_transmitPinPort, _transmitPinNumber);
            }
            tx_buffer >>= 1;
            tx_tick_cnt = OVERSAMPLE; // Wait OVERSAMPLE tick to send next bit
        } else { // Transmission finished
            tx_tick_cnt = 1;
            if (_output_pending) {
                active_out = nullptr;
            }
        }
    }
}

inline void SoftwareSerial::handleInterrupt()
{
    if (active_in) {
        active_in->recv();
    }
    if (active_out) {
        active_out->send();
    }
}

SoftwareSerial::~SoftwareSerial()
{
    end();
}

//
// Public methods
//

void SoftwareSerial::begin(long speed)
{
    _speed = speed;
    if ((_receivePin < DIGITAL_PINS_NUM) || (_transmitPin < DIGITAL_PINS_NUM)) {
        gpio_clock_enable(GD_PORT_GET(DIGITAL_TO_PINNAME(_transmitPin)));
        gpio_clock_enable(GD_PORT_GET(DIGITAL_TO_PINNAME(_receivePin)));
    }
    if (_inverse_logic) {
        gpio_bit_reset(_transmitPinPort, _transmitPinNumber);
    } else {
        gpio_bit_set(_transmitPinPort, _transmitPinNumber);
    }
    pinMode(_transmitPin, OUTPUT);
    pinMode(_receivePin, _inverse_logic ? INPUT_PULLDOWN : INPUT_PULLUP);
    listen();
}

void SoftwareSerial::end()
{
    stopListening();
}

size_t SoftwareSerial::write(uint8_t b)
{
    // wait for previous transmit to complete
    _output_pending = 1;
    while (active_out);
    // add start and stop bits.
    tx_buffer = b << 1 | 0x200;
    if (_inverse_logic) {
        tx_buffer = ~tx_buffer;
    }
    tx_bit_cnt = 0;
    tx_tick_cnt = OVERSAMPLE;
    setSpeed(_speed);
    _output_pending = 0;
    // make us active
    active_out = this;
    return 1;
}

// Read data from buffer
int SoftwareSerial::read()
{
    // Empty buffer?
    if (_receive_buffer.head == _receive_buffer.tail) {
        return -1;
    } else {
        // Read from "head"
        uint8_t c = _receive_buffer.buffer[_receive_buffer.head]; // grab next byte
        _receive_buffer.head = (_receive_buffer.head + 1) % _SS_MAX_RX_BUFF;
        return c;
    }
}

int SoftwareSerial::available()
{
    return (_receive_buffer.tail + _SS_MAX_RX_BUFF - _receive_buffer.head) % _SS_MAX_RX_BUFF;
}

void SoftwareSerial::flush()
{
    noInterrupts();
    _receive_buffer.head = _receive_buffer.tail = 0;
    interrupts();
}

int SoftwareSerial::peek(void)
{
    if (_receive_buffer.head == _receive_buffer.tail) {
        return -1;
    } else {
        return _receive_buffer.buffer[_receive_buffer.head];
    }
}

inline void SoftwareSerial::recv()
{
    if (--rx_tick_cnt <=
        0) {  // if rx_tick_cnt > 0 interrupt is discarded. Only when rx_tick_cnt reach 0 RX pin is considered
        bool inbit = gpio_input_bit_get(_receivePinPort, _receivePinNumber) ^ _inverse_logic;
        if (rx_bit_cnt == -1) {  // rx_bit_cnt = -1 :  waiting for start bit
            if (!inbit) {
                // got start bit
                rx_bit_cnt = 0; // rx_bit_cnt == 0 : start bit received
                rx_tick_cnt = OVERSAMPLE +
                              1; // Wait 1 bit (OVERSAMPLE ticks) + 1 tick in order to sample RX pin in the middle of the edge (and not too close to the edge)
                rx_buffer = 0;
            } else {
                rx_tick_cnt =
                    1; // Waiting for start bit, but we don't get right level. Wait for next Interrupt to check RX pin level
            }
        } else if (rx_bit_cnt >= 8) { // rx_bit_cnt >= 8 : waiting for stop bit
            if (inbit) {
                // stop bit read complete add to buffer
                uint8_t next = (_receive_buffer.tail + 1) % _SS_MAX_RX_BUFF;
                if (next != _receive_buffer.head) {
                    // save new data in buffer: tail points to where byte goes
                    _receive_buffer.buffer[_receive_buffer.tail] = rx_buffer; // save new byte
                    _receive_buffer.tail = next;
                } else { // rx_bit_cnt = x  with x = [0..7] correspond to new bit x received
                    _buffer_overflow = true;
                }
            }
            // Full frame received. Restart waiting for start bit at next interrupt
            rx_tick_cnt = 1;
            rx_bit_cnt = -1;
        } else {
            // data bits
            rx_buffer >>= 1;
            if (inbit) {
                rx_buffer |= 0x80;
            }
            rx_bit_cnt++; // Prepare for next bit
            rx_tick_cnt = OVERSAMPLE; // Wait OVERSAMPLE ticks before sampling next bit
        }
    }
}
