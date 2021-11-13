/*
 * SPI Master library
 * Copyright (c) 2015 Arduino LLC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Modified by Ajay Bhargav <contact@rickeyworld.info>
 * Modified by Gigadevice
 *
 */

#include <Arduino.h>
#include <assert.h>
#include "SPI.h"
#include "pins_arduino.h"

SPIClass SPI;

SPIClass::SPIClass(void)
{
    _spi.pin_miso = DIGITAL_TO_PINNAME(MISO);
    _spi.pin_mosi = DIGITAL_TO_PINNAME(MOSI);
    _spi.pin_sclk = DIGITAL_TO_PINNAME(SCK);
    _spi.pin_ssel = NC;

    initialized = false;
}

SPIClass::SPIClass(PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    _spi.pin_miso = miso;
    _spi.pin_mosi = mosi;
    _spi.pin_sclk = sclk;
    _spi.pin_ssel = ssel;

    initialized = false;
}

SPIClass::SPIClass(PinName mosi, PinName miso, PinName sclk)
{
    _spi.pin_miso = miso;
    _spi.pin_mosi = mosi;
    _spi.pin_sclk = sclk;
    _spi.pin_ssel = NC;

    initialized = false;
}

void SPIClass::begin()
{
    uint32_t test = 0;
    if (initialized) {
        return;
    }

    spi_begin(&_spi, spisettings.speed, spisettings.datamode, spisettings.bitorder);

    initialized = true;
}

void SPIClass::end()
{
    if (initialized) {
        spi_free(&_spi);
        initialized = false;
    }
}

void SPIClass::beginTransaction(SPISettings settings)
{
    config(settings);

    spi_begin(&_spi, spisettings.speed, spisettings.datamode, spisettings.bitorder);

    initialized = true;
}

void SPIClass::endTransaction(void)
{
    if (initialized) {
        spi_free(&_spi);
        initialized = false;
    }
}

uint8_t SPIClass::transfer(uint8_t val8)
{
    uint32_t out_byte;
    out_byte = spi_master_write(&_spi, val8);

    return out_byte;
}

uint16_t SPIClass::transfer16(uint16_t val16)
{
    uint16_t out_halfword;
    uint8_t trans_data0, trans_data1, rec_data0, rec_data1;

    trans_data0 = uint8_t(val16 & 0x00FF);
    trans_data1 = uint8_t((val16 & 0xFF00) >> 8);

    if (spisettings.bitorder == LSBFIRST) {
        rec_data0 = transfer(trans_data0);
        rec_data1 = transfer(trans_data1);
        out_halfword = uint16_t(rec_data0 || rec_data1 << 8);
    } else {
        rec_data0 = transfer(trans_data1);
        rec_data1 = transfer(trans_data0);
        out_halfword = uint16_t(rec_data1 || rec_data0 << 8);
    }

    return out_halfword;
}

void SPIClass::transfer(void *buf, size_t count)
{
    spi_master_block_write(&_spi, ((uint8_t *)buf), ((uint8_t *)buf), count);
}

void SPIClass::transfer(void *bufout, void *bufin, size_t count)
{
    spi_master_block_write(&_spi, ((uint8_t *)bufout), ((uint8_t *)bufin), count);
}

void SPIClass::setBitOrder(BitOrder order)
{
    spisettings.bitorder = order;
    spi_begin(&_spi, spisettings.speed, spisettings.datamode, spisettings.bitorder);
}

void SPIClass::setDataMode(uint8_t mode)
{
    spisettings.datamode = mode;
    spi_begin(&_spi, spisettings.speed, spisettings.datamode, spisettings.bitorder);
}

void SPIClass::setClockDivider(uint32_t divider)
{
    if (divider == 0) {
        spisettings.speed = SPI_SPEED_DEFAULT;
    } else {
        /* Get clk freq of the SPI instance and compute it */
        spisettings.speed = dev_spi_clock_source_frequency_get(&_spi) / divider;
    }

    spi_begin(&_spi, spisettings.speed, spisettings.datamode, spisettings.bitorder);
}

void SPIClass::config(SPISettings settings)
{
    spisettings.speed = settings.speed;
    spisettings.datamode = settings.datamode;
    spisettings.bitorder = settings.bitorder;
}
