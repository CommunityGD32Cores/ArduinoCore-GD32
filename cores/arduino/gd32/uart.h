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

    Based on mbed-os/hal/include/hal/serial_api.h
*/

#ifndef UART_H
#define UART_H

/* Includes ------------------------------------------------------------------*/
#include "gd32xxyy.h"
#include "PinNames.h"
#include "PeripheralNames.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#ifdef __cplusplus
extern "C" {
#endif

#define SERIAL_EVENT_TX_SHIFT (2)
#define SERIAL_EVENT_RX_SHIFT (8)

#define SERIAL_EVENT_TX_MASK (0x00FC)
#define SERIAL_EVENT_RX_MASK (0x3F00)

#define SERIAL_EVENT_ERROR (1 << 1)

/**
 * @defgroup SerialTXEvents Serial TX Events Macros
 *
 * @{
 */
#define SERIAL_EVENT_TX_COMPLETE (1 << (SERIAL_EVENT_TX_SHIFT + 0))
#define SERIAL_EVENT_TX_ALL      (SERIAL_EVENT_TX_COMPLETE)
/**@}*/

/**
 * @defgroup SerialRXEvents Serial RX Events Macros
 *
 * @{
 */
#define SERIAL_EVENT_RX_COMPLETE        (1 << (SERIAL_EVENT_RX_SHIFT + 0))
#define SERIAL_EVENT_RX_OVERRUN_ERROR   (1 << (SERIAL_EVENT_RX_SHIFT + 1))
#define SERIAL_EVENT_RX_FRAMING_ERROR   (1 << (SERIAL_EVENT_RX_SHIFT + 2))
#define SERIAL_EVENT_RX_PARITY_ERROR    (1 << (SERIAL_EVENT_RX_SHIFT + 3))
#define SERIAL_EVENT_RX_OVERFLOW        (1 << (SERIAL_EVENT_RX_SHIFT + 4))
#define SERIAL_EVENT_RX_CHARACTER_MATCH (1 << (SERIAL_EVENT_RX_SHIFT + 5))
#define SERIAL_EVENT_RX_ALL             (SERIAL_EVENT_RX_OVERFLOW | SERIAL_EVENT_RX_PARITY_ERROR | \
                                         SERIAL_EVENT_RX_FRAMING_ERROR | SERIAL_EVENT_RX_OVERRUN_ERROR | \
                                         SERIAL_EVENT_RX_COMPLETE | SERIAL_EVENT_RX_CHARACTER_MATCH)
/**@}*/

#define SERIAL_RESERVED_CHAR_MATCH (255)

// gd_{status,operation_state}_enum cribbed from previous library
// and put here because they don’t appear in the GD firmware library
// upstream, and appear to have been added after-the-fact in
// gd32f30x.h by some porter. -bjc (2021-Aug-20)

typedef enum {
    GD_OK       = 0x00U,
    GD_ERROR    = 0x01U,
    GD_BUSY     = 0x02U,
    GD_TIMEOUT  = 0x03U
} gd_status_enum;

typedef enum {
    OP_STATE_RESET             = 0x00U,
    OP_STATE_READY             = 0x01U,
    OP_STATE_BUSY              = 0x02U,
    OP_STATE_TIMEOUT           = 0x03U,
    OP_STATE_ERROR             = 0x04U,
    OP_STATE_ABORT             = 0x05U,
    OP_STATE_LISTEN            = 0x06U,

    OP_STATE_BUSY_TX           = 0x21U, /* (OP_STATE_BUSY << 4) + 1 */
    OP_STATE_BUSY_RX           = 0x22U, /* (OP_STATE_BUSY << 4) + 2 */

    OP_STATE_BUSY_TX_LISTEN    = 0x61U, /* (OP_STATE_LISTEN << 4) + 1 */
    OP_STATE_BUSY_RX_LISTEN    = 0x62U, /* (OP_STATE_LISTEN << 4) + 2 */

    OP_STATE_BUTT
} operation_state_enum;

typedef enum {
    ParityNone = 0,
    ParityOdd = 1,
    ParityEven = 2,
    ParityForced1 = 3,
    ParityForced0 = 4
} SerialParity;

typedef struct serial_s serial_t;

struct serial_s {
    /* basic information */
    UARTName uart;
    int     index;
    PinName pin_tx;
    PinName pin_rx;

    /* configure information */
    uint32_t baudrate;
    uint32_t databits;
    uint32_t stopbits;
    uint32_t parity;

    /* operating parameters */
    uint16_t        rx_size;
    uint8_t         *tx_buffer_ptr;
    uint8_t         *rx_buffer_ptr;
    uint16_t   tx_count;
    uint16_t   rx_count;

    volatile uint16_t rx_tail;
    volatile uint16_t tx_head;
    volatile uint16_t rx_head;
    volatile uint16_t tx_tail;

    uint32_t   error_code;
    operation_state_enum  tx_state;
    operation_state_enum  rx_state;

    void (*tx_callback)(serial_t *obj);
    void (*rx_callback)(serial_t *obj);
};

/* Initialize the serial peripheral. It sets the default parameters for serial peripheral, and configures its specifieds pins. */
void serial_init(serial_t *obj, PinName tx, PinName rx);
/* Release the serial peripheral, not currently invoked. It requires further resource management. */
void serial_free(serial_t *obj);
/* Configure the baud rate */
void serial_baud(serial_t *obj, int baudrate);
/* Configure the format. Set the number of bits, parity and the number of stop bits. */
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits);
/* Get character. This is a blocking call, waiting for a character. */
int  serial_getc(serial_t *obj);
/* Send a character. This is a blocking call, waiting for a peripheral to be available for writing. */
void serial_putc(serial_t *obj, int c);
/* Check if the serial peripheral is readable. */
int  serial_readable(serial_t *obj);
/* Check if the serial peripheral is writable. */
int  serial_writable(serial_t *obj);
/* Clear the serial peripheral. */
void serial_clear(serial_t *obj);
/* Attempts to determine if the serial peripheral is already in use for TX. */
uint8_t serial_tx_active(serial_t *obj);
/* Attempts to determine if the serial peripheral is already in use for RX. */
uint8_t serial_rx_active(serial_t *obj);
/* Attach UART transmit callback */
void uart_attach_tx_callback(serial_t *obj, void (*callback)(serial_t *));
/* Attach UART receive callback */
void uart_attach_rx_callback(serial_t *obj, void (*callback)(serial_t *));
/* Begin asynchronous TX transfer. */
int serial_transmit(serial_t *obj, const void *tx, size_t tx_length);
/* Begin asynchronous RX transfer (enable interrupt for data collecting). */
void serial_receive(serial_t *obj, void *rx, size_t rx_length);

#ifdef __cplusplus
}
#endif

#endif /* UART_H */
