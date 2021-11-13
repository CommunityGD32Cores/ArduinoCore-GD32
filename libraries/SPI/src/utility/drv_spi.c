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

    Based on mbed-os/target/TARGET_GigaDevice/TARGET_GD32F30X/spi_api.c
*/

#include "drv_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_S(obj)    (( struct spi_s *)(obj))
#define SPI_PINS_FREE_MODE   0x00000001

/** Initialize the SPI structure
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * @param[out] obj  The SPI object to initialize
 */
static void dev_spi_struct_init(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);

    spi_disable(spiobj->spi);
    spi_init(spiobj->spi, &obj->spi_struct);
    spi_enable(spiobj->spi);
}

/** Get the frequency of SPI clock source
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * @param[out] spi_freq  The SPI clock source frequency
 * @param[in] obj  The SPI object
 */
uint32_t dev_spi_clock_source_frequency_get(spi_t *obj)
{
    uint32_t spi_freq = 0;
    struct spi_s *spiobj = SPI_S(obj);

    switch ((int)spiobj->spi) {
        case SPI0:
            /* clock source is APB2 */
            spi_freq = rcu_clock_freq_get(CK_APB2);
            break;
        case SPI1:
            /* clock source is APB1 */
            spi_freq = rcu_clock_freq_get(CK_APB1);
            break;
        case SPI2:
            /* clock source is APB1 */
            spi_freq = rcu_clock_freq_get(CK_APB1);
            break;
        default:
            //error("SPI clock source frequency get error");
            break;
    }
    return spi_freq;
}

/**
  * @brief  SPI initialization function
  * @param  obj : pointer to spi_t structure
  * @param  speed : spi output speed
  * @param  mode : one of the spi modes
  * @param  msb : set to 1 in msb first
  * @retval None
  */
void spi_begin(spi_t *obj, uint32_t speed, uint8_t mode, uint8_t endian)
{
    struct spi_s *spiobj = SPI_S(obj);

    uint32_t spi_freq = 0;

    /* Determine the SPI to use */
    SPIName spi_mosi = (SPIName)pinmap_peripheral(spiobj->pin_mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(spiobj->pin_miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(spiobj->pin_sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(spiobj->pin_ssel, PinMap_SPI_SSEL);

    /* return SPIName according to PinName */
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);

    spiobj->spi = (SPIName)pinmap_merge(spi_data, spi_cntl);

    /* enable SPI clock */
    if (spiobj->spi == SPI0) {
        rcu_periph_clock_enable(RCU_SPI0);
    }
    if (spiobj->spi == SPI1) {
        rcu_periph_clock_enable(RCU_SPI1);
    }
    if (spiobj->spi == SPI2) {
        rcu_periph_clock_enable(RCU_SPI2);
    }

    /* configure GPIO mode of SPI pins */
    pinmap_pinout(spiobj->pin_mosi, PinMap_SPI_MOSI);
    pinmap_pinout(spiobj->pin_miso, PinMap_SPI_MISO);
    pinmap_pinout(spiobj->pin_sclk, PinMap_SPI_SCLK);

    /* Configure the SPI pins */
    if (spiobj->pin_ssel != NC) {
        pinmap_pinout(spiobj->pin_ssel, PinMap_SPI_SSEL);
        spiobj->spi_struct.nss = SPI_NSS_HARD;
        spi_nss_output_enable(spiobj->spi);
    } else {
        spiobj->spi_struct.nss = SPI_NSS_SOFT;
    }

    spi_freq = dev_spi_clock_source_frequency_get(obj);
    if (speed >= (spi_freq / SPI_CLOCK_DIV2)) {
        spiobj->spi_struct.prescale             = SPI_PSC_2;
    } else if (speed >= (spi_freq / SPI_CLOCK_DIV4)) {
        spiobj->spi_struct.prescale             = SPI_PSC_4;
    } else if (speed >= (spi_freq / SPI_CLOCK_DIV8)) {
        spiobj->spi_struct.prescale             = SPI_PSC_8;
    } else if (speed >= (spi_freq / SPI_CLOCK_DIV16)) {
        spiobj->spi_struct.prescale             = SPI_PSC_16;
    } else if (speed >= (spi_freq / SPI_CLOCK_DIV32)) {
        spiobj->spi_struct.prescale             = SPI_PSC_32;
    } else if (speed >= (spi_freq / SPI_CLOCK_DIV64)) {
        spiobj->spi_struct.prescale             = SPI_PSC_64;
    } else if (speed >= (spi_freq / SPI_CLOCK_DIV128)) {
        spiobj->spi_struct.prescale             = SPI_PSC_128;
    } else {
        /*
         * As it is not possible to go below (spi_freq / SPI_SPEED_CLOCK_DIV256_MHZ).
         * Set prescaler at max value so get the lowest frequency possible.
         */
        spiobj->spi_struct.prescale             = SPI_PSC_256;
    }

    if (mode == SPI_MODE0) {
        spiobj->spi_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    } else if (mode == SPI_MODE1) {
        spiobj->spi_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    } else if (mode == SPI_MODE2) {
        spiobj->spi_struct.clock_polarity_phase =  SPI_CK_PL_HIGH_PH_1EDGE;
    } else if (mode == SPI_MODE3) {
        spiobj->spi_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    } else {
        return;
    }

    if (endian == 0) {
        spiobj->spi_struct.endian               = SPI_ENDIAN_LSB;
    } else {
        spiobj->spi_struct.endian               = SPI_ENDIAN_MSB;
    }

    /* Default values */
    spiobj->spi_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spiobj->spi_struct.device_mode          = SPI_MASTER;
    spiobj->spi_struct.frame_size           = SPI_FRAMESIZE_8BIT;

    dev_spi_struct_init(obj);
}

/**
  * @brief This function is implemented to deinitialize the SPI interface
  *        (IOs + SPI block)
  * @param  obj : pointer to spi_t structure
  * @retval None
  */
void spi_free(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    spi_disable(spiobj->spi);

    /* Disable and deinit SPI */
    if (spiobj->spi == SPI0) {
        spi_i2s_deinit(SPI0);
        rcu_periph_clock_disable(RCU_SPI0);
    }
    if (spiobj->spi == SPI1) {
        spi_i2s_deinit(SPI1);
        rcu_periph_clock_disable(RCU_SPI1);
    }
    if (spiobj->spi == SPI2) {
        spi_i2s_deinit(SPI2);
        rcu_periph_clock_disable(RCU_SPI2);
    }
    /* Deinit GPIO mode of SPI pins */
    pin_function(spiobj->pin_miso, SPI_PINS_FREE_MODE);
    pin_function(spiobj->pin_mosi, SPI_PINS_FREE_MODE);
    pin_function(spiobj->pin_sclk, SPI_PINS_FREE_MODE);
    if (spiobj->spi_struct.nss != SPI_NSS_SOFT) {
        pin_function(spiobj->pin_ssel, SPI_PINS_FREE_MODE);
        spi_nss_output_disable(spiobj->spi);
    }
}

/**
  * @brief This function is implemented by user to send data over SPI interface
  * @param  obj : pointer to spi_t structure
  * @param  value : data to be sent
  * @retval status of the send operation (0) in case of error
  */
uint32_t spi_master_write(spi_t *obj, uint8_t value)
{
    int count = 0;
    struct spi_s *spiobj = SPI_S(obj);

    /* wait the SPI transmit buffer is empty */
    while ((RESET == spi_i2s_flag_get(spiobj->spi, SPI_FLAG_TBE)) && (count++ < 1000));
    if (count >= 1000) {
        return -1;
    } else {
        spi_i2s_data_transmit(spiobj->spi, value);
    }

    count = 0;
    /* wait the SPI receive buffer is not empty */
    while ((RESET == spi_i2s_flag_get(spiobj->spi, SPI_FLAG_RBNE)) && (count++ < 1000));
    if (count >= 1000) {
        return -1;
    } else {
        return spi_i2s_data_receive(spiobj->spi);
    }
}

/**
  * @brief This function is implemented by user to send/receive data over
  *         SPI interface
  * @param  obj : pointer to spi_t structure
  * @param  tx_buffer : tx data to send before reception
  * @param  rx_buffer : data to receive
  * @param  len : length in byte of the data to send and receive
  * @retval status of the send operation (0) in case of error
  */
void spi_master_block_write(spi_t *obj, uint8_t *tx_buffer, uint8_t *rx_buffer, uint16_t len)
{
    for (int i = 0; i < len; i++) {
        char in = spi_master_write(obj, tx_buffer[i]);
        rx_buffer[i] = in;
    }
}

#ifdef __cplusplus
}
#endif
