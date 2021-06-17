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

    Based on mbed-os/hal/spi_api.h
*/

#ifndef __DRV_SPI_H
#define __DRV_SPI_H

#include "PinNames.h"
#include "PeripheralPins.h"
#include "PeripheralNames.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

/* SPI default speed */
#define SPI_SPEED_DEFAULT     4000000

#define SPI_CLOCK_DIV2    ((uint32_t)2U)
#define SPI_CLOCK_DIV4    ((uint32_t)4)
#define SPI_CLOCK_DIV8    ((uint32_t)8)
#define SPI_CLOCK_DIV16   ((uint32_t)16)
#define SPI_CLOCK_DIV32   ((uint32_t)32)
#define SPI_CLOCK_DIV64   ((uint32_t)64)
#define SPI_CLOCK_DIV128  ((uint32_t)128)
#define SPI_CLOCK_DIV256  ((uint32_t)256)

struct spi_s {
    spi_parameter_struct spi_struct;
    SPIName spi;
    PinName pin_miso;
    PinName pin_mosi;
    PinName pin_sclk;
    PinName pin_ssel;
};

typedef struct spi_s spi_t;

void spi_begin(spi_t *obj, uint32_t speed, uint8_t mode, uint8_t endian);
uint32_t spi_master_write(spi_t *obj, uint8_t value);
void spi_master_block_write(spi_t *obj, uint8_t *tx_buffer, uint8_t *rx_buffer, uint16_t len);
uint32_t dev_spi_clock_source_frequency_get(spi_t *obj);
void spi_free(spi_t *obj);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_SPI_H */
