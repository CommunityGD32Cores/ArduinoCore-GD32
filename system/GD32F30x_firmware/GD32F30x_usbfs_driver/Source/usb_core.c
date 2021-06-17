/*!
    \file  usb_core.c
    \brief USB core driver which can operate in host-mode and device-mode

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    All rights reserved.

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
*/

#include "usb_core.h"

static void usb_commonint_enable(usb_core_handle_struct *pudev);
static usb_status_enum usb_core_reset(usb_core_handle_struct *pudev);

/*!
    \brief      enable the commmon interrupts which are used in both device and host modes
    \param[in]  pudev: pointer to selected usb device
    \param[out] none
    \retval     none
*/
static void usb_commonint_enable(usb_core_handle_struct *pudev)
{
#ifndef USE_OTG_MODE

    /* clear any pending USB interrupts */
    USB_GOTGINTF = 0xFFFFFFFFU;

#endif /* USE_OTG_MODE */

    /* enable the usb wakeup and suspend interrupts */
    USB_GINTEN = GINTEN_WKUPIE | GINTEN_SPIE;

#ifdef USE_OTG_MODE

    /* enable the OTG interrupts, session interrrupts and connector ID pin interrupt */
    USB_GINTEN |= GINTEN_OTGIE | GINTEN_SESIE | GINTEN_CIDPSCIE;

#endif /* USE_OTG_MODE */
}

/*!
    \brief      soft reset of the OTG_FS core
    \param[in]  pudev: pointer to usb device
    \param[out] none
    \retval     operation status
*/
static usb_status_enum usb_core_reset(usb_core_handle_struct *pudev)
{
    uint32_t count = 0U;

    /* enable core soft reset */
    USB_GRSTCTL |= GRSTCTL_CSRST;

    /* wait for the core to be soft reset */
    do {
        if(++count > 200000U) {
            break;
        }
    } while(1U == (USB_GRSTCTL & GRSTCTL_CSRST));

    /* wait for addtional 3 PHY clocks */
    if(NULL != pudev->udelay) {
        pudev->udelay(3U);
    }

    return USB_OK;
}

/*!
    \brief      write a packet into the Tx FIFO associated with the endpoint
    \param[in]  src: pointer to source buffer
    \param[in]  chep_num: channel or endpoint identifier which is in (0..3)
    \param[in]  len: packet length
    \param[out] none
    \retval     operation status
*/
usb_status_enum usb_fifo_write(uint8_t *src, uint8_t chep_num, uint16_t len)
{
    uint32_t count32b = 0U, i = 0U;
    __IO uint32_t *fifo = USB_FIFO(chep_num);

    count32b = (len + 3U) / 4U;

    for(i = 0U; i < count32b; i++) {
        *fifo = *((__packed uint32_t *)src);

        src += 4U;
    }

    return USB_OK;
}

/*!
    \brief      read a packet from the Rx FIFO associated with the endpoint
    \param[in]  dest: pointer to destination buffer
    \param[in]  len: packet length
    \param[out] none
    \retval     void type pointer
*/
void *usb_fifo_read(uint8_t *dest, uint16_t len)
{
    uint32_t i = 0U;
    uint32_t count32b = (len + 3U) / 4U;

    __IO uint32_t *fifo = USB_FIFO(0U);

    for(i = 0U; i < count32b; i++) {
        *(__packed uint32_t *)dest = *fifo;

        dest += 4U;
    }

    return ((void *)dest);
}

/*!
    \brief      initialize core parameters
    \param[in]  pudev: pointer to usb device
    \param[in]  core_id: USB core id
    \param[out] none
    \retval     operation status
*/
usb_status_enum usb_core_select(usb_core_handle_struct *pudev, usb_core_id_enum core_id)
{
    /* at startup the core is in FS mode */
    pudev->cfg.core_speed = USB_CORE_SPEED_FULL;
    pudev->cfg.max_packet_size = USBFS_MAX_PACKET_SIZE;

    /* initialize the core parameters */
    if(USB_FS_CORE_ID == core_id) {

        pudev->cfg.core_id = USB_FS_CORE_ID;

        /* set the host channel numbers */
        pudev->cfg.host_channel_num = USBFS_MAX_HOST_CHANNELCOUNT;

        /* set the device endpoint numbers */
        pudev->cfg.dev_endp_num = USBFS_MAX_DEV_EPCOUNT;

        /* fifo size is in terms of DWORD */
        pudev->cfg.max_fifo_size = USBFS_MAX_FIFO_WORDLEN;

        /* OTG_FS core use embedded physical layer */
        pudev->cfg.phy_interface = USB_CORE_EMBEDDED_PHY;

#ifdef USBFS_SOF_OUTPUT_ENABLED
        pudev->cfg.sof_output = 1U;
#endif /* USBFS_SOF_OUTPUT_ENABLED */

#ifdef USBFS_LOW_PWR_MGMT_SUPPORT
        pudev->cfg.low_power = 1U;
#endif /* USBFS_LOW_PWR_MGMT_SUPPORT */
    }

    return USB_OK;
}

/*!
    \brief      initializes the USB controller registers and
                prepares the core device mode or host mode operation
    \param[in]  pudev: pointer to usb device
    \param[out] none
    \retval     operation status
*/
usb_status_enum usb_core_init(usb_core_handle_struct *pudev)
{
    /* soft reset the core */
    usb_core_reset(pudev);

    /* active the transceiver and enable vbus sensing */
    USB_GCCFG |= GCCFG_PWRON | GCCFG_VBUSACEN | GCCFG_VBUSBCEN;

    /* set Tx FIFO empty level to half empty mode */
    USB_GAHBCS &= ~GAHBCS_TXFTH | TXFIFO_EMPTY_HALF;

#ifndef VBUS_SENSING_ENABLED
    USB_GCCFG |= GCCFG_VBUSIG;
#endif /* VBUS_SENSING_ENABLED */

    if(pudev->cfg.sof_output) {
        USB_GCCFG |= GCCFG_SOFOEN;
    }

    if(NULL != pudev->mdelay) {
        pudev->mdelay(20U);
    }


#ifdef USE_OTG_MODE
    /* enable OTG features */
    USB_GUSBCS |= GUSBCS_HNPCAP | GUSBCS_SRPCAP;
    USB_OTG_EnableCommonInt(pudev);

#endif /* USE_OTG_MODE */

    return USB_OK;
}

/*!
    \brief      flush a Tx FIFO or all Tx FIFOs
    \param[in]  pudev: pointer to usb device
    \param[in]  fifo_num: FIFO number which is in (0..3)
    \param[out] none
    \retval     operation status
*/
usb_status_enum usb_txfifo_flush(usb_core_handle_struct *pudev, uint8_t fifo_num)
{
    uint32_t count = 0U;

    USB_GRSTCTL &= ~GRSTCTL_TXFNUM;
    USB_GRSTCTL = ((uint32_t)fifo_num << 6U) | GRSTCTL_TXFF;

    /* wait for Tx FIFO flush bit is set */
    do {
        if(++count > 200000U) {
            break;
        }
    } while(USB_GRSTCTL & GRSTCTL_TXFF);

    /* wait for 3 PHY clocks */
    if(NULL != pudev->udelay) {
        pudev->udelay(3U);
    }

    return USB_OK;
}

/*!
    \brief      flush the entire Rx FIFO
    \param[in]  pudev: pointer to usb device
    \param[out] none
    \retval     operation status
*/
usb_status_enum usb_rxfifo_flush(usb_core_handle_struct *pudev)
{
    uint32_t count = 0U;

    USB_GRSTCTL = GRSTCTL_RXFF;

    /* wait for Rx FIFO flush bit is set */
    do {
        if(++count > 200000U) {
            break;
        }
    } while(USB_GRSTCTL & GRSTCTL_RXFF);

    /* wait for 3 PHY clocks */
    if(NULL != pudev->udelay) {
        pudev->udelay(3U);
    }

    return USB_OK;
}

/*!
    \brief      set operation mode (host or device)
    \param[in]  pudev: pointer to usb device
    \param[in]  mode: operation mode which need to set
      \arg        HOST_MODE
      \arg        DEVICE_MODE
    \param[out] none
    \retval     operation status
*/
usb_status_enum usb_mode_set(usb_core_handle_struct *pudev, uint8_t mode)
{
    if(HOST_MODE == mode) {
        USB_GUSBCS &= ~GUSBCS_FDM;
        USB_GUSBCS |= GUSBCS_FHM;
    } else if(DEVICE_MODE == mode) {
        USB_GUSBCS &= ~GUSBCS_FHM;
        USB_GUSBCS |= GUSBCS_FDM;
    } else {
        /* no operation */
    }

    if(NULL != pudev->mdelay) {
        pudev->mdelay(50U);
    }

    return USB_OK;
}

#ifdef USE_HOST_MODE

/*!
    \brief      initializes USB core for host mode
    \param[in]  pudev: pointer to selected usb host
    \param[out] none
    \retval     operation status
*/
usb_status_enum usb_hostcore_init(usb_core_handle_struct *pudev)
{
    uint32_t i = 0U;
    __IO uint32_t nptxfifolen = 0U;
    __IO uint32_t ptxfifolen = 0U;

#ifdef USE_OTG_MODE
    __IO uint32_t otgctl = 0;
#endif /* USE_OTG_MODE */

    /* restart the PHY clock */
    USB_PWRCLKCTL = 0U;

    /* initialize host configuration register */
    if(USB_CORE_ULPI_PHY == pudev->cfg.phy_interface) {
        USB_FSLSCLOCK_INIT(HCTLR_30_60_MHZ);
    } else {
        USB_FSLSCLOCK_INIT(HCTLR_48_MHZ);
    }

    /* configure data FIFO sizes */
    if(USB_FS_CORE_ID == pudev->cfg.core_id) {
        /* set Rx FIFO size */
        USB_GRFLEN = USBFS_RX_FIFO_SIZE;

        /* set non-periodic Tx FIFO size and address */
        nptxfifolen &= ~HNPTFLEN_HNPTXRSAR;
        nptxfifolen |= USBFS_RX_FIFO_SIZE;
        nptxfifolen &= ~HNPTFLEN_HNPTXFD;
        nptxfifolen |= USBFS_HTX_NPFIFO_SIZE << 16;
        USB_HNPTFLEN = nptxfifolen;

        /* set periodic Tx FIFO size and address */
        ptxfifolen &= ~HPTFLEN_HPTXFSAR;
        ptxfifolen |= USBFS_RX_FIFO_SIZE + USBFS_HTX_PFIFO_SIZE;
        ptxfifolen &= ~HPTFLEN_HPTXFD;
        ptxfifolen |= USBFS_HTX_PFIFO_SIZE << 16;
        USB_HPTFLEN = ptxfifolen;
    }

#ifdef USE_OTG_MODE

    /* clear Host Set HNP Enable bit in the USB OTG Control Register */
    otgctl |= GOTGCS_HHNPEN;
    USB_GOTGCS &= ~otgctl;
    USB_GOTGCS |= 0;

#endif /* USE_OTG_MODE */

    /* make sure the FIFOs are flushed */

    /* flush all Tx FIFOs in device or host mode */
    usb_txfifo_flush(pudev, 0x10U);

    /* flush the entire Rx FIFO */
    usb_rxfifo_flush(pudev);

    /* clear all pending host channel interrupts */
    USB_HACHINTEN &= ~HACHINTEN_CINTEN;

    for(i = 0U; i < pudev->cfg.host_channel_num; i++) {
        USB_HCHxINTEN(i) = 0U;
        USB_HCHxINTF(i) = 0xFFFFFFFFU;
    }

#ifndef USE_OTG_MODE
    usb_vbus_drive(pudev, 1U);
#endif /* USE_OTG_MODE */

    usb_hostint_enable(pudev);

    return USB_OK;
}

/*!
    \brief      control the VBUS to power
    \param[in]  pudev: pointer to selected usb host
    \param[in]  state: VBUS state
    \param[out] none
    \retval     none
*/
void usb_vbus_drive(usb_core_handle_struct *pudev, uint8_t state)
{
    __IO uint32_t host_port = 0U;

    /* enable or disable the external charge pump */
    if((void *)0 != pudev->host.vbus_drive) {
        pudev->host.vbus_drive(pudev, state);
    }

    /* turn on the host port power. */
    host_port = USB_PORT_READ();

    if((0U == (host_port & HPCS_PP)) && (1U == state)) {
        host_port |= HPCS_PP;
    } else if((1U == (host_port & HPCS_PP)) && (0U == state)) {
        host_port &= ~HPCS_PP;
    } else {
        /* no operation */
    }

    USB_HPCS = host_port;

    if(NULL != pudev->mdelay) {
        pudev->mdelay(200U);
    }
}

/*!
    \brief      enables the host mode interrupts
    \param[in]  pudev: pointer to selected usb host
    \param[out] none
    \retval     operation status
*/
usb_status_enum usb_hostint_enable(usb_core_handle_struct *pudev)
{
    uint32_t gintf = 0U;

    /* disable all interrupts */
    USB_GINTEN = 0U;

    /* clear any pending interrupts */
    USB_GINTF = 0xFFFFFFFFU;

    /* enable the common interrupts */
    usb_commonint_enable(pudev);

    gintf |= GINTF_RXFNEIF;

    /* enable host_mode-related interrupts */
    gintf |= GINTF_HPIF | GINTF_HCIF | GINTF_DISCIF | GINTF_SOF | GINTF_ISOONCIF;

    USB_GINTEN &= ~gintf;
    USB_GINTEN |= gintf;

    return USB_OK;
}

/*!
    \brief      reset host port
    \param[in]  pudev: pointer to usb device
    \param[out] none
    \retval     operation status
*/
uint32_t usb_port_reset(usb_core_handle_struct *pudev)
{
    USB_HPCS = USB_PORT_READ() | HPCS_PRST;

    if(NULL != pudev->mdelay) {
        pudev->mdelay(100U);
    }

    USB_HPCS &= ~HPCS_PRST;

    if(NULL != pudev->mdelay) {
        pudev->mdelay(20U);
    }

    return USB_OK;
}

/*!
    \brief      initialize host channel
    \param[in]  pudev: pointer to usb device
    \param[in]  hc_num: host channel number which is in (0..7)
    \param[out] none
    \retval     operation status
*/
usb_status_enum usb_hostchannel_init(usb_core_handle_struct *pudev, uint8_t hc_num)
{
    uint8_t is_low_speed = 0U;
    __IO uint32_t chinten = 0U;
    __IO uint32_t chctl = 0U;

    usb_hostchannel_struct *puhc = &pudev->host.host_channel[hc_num];

    /* clear old interrupt conditions for this host channel */
    USB_HCHxINTF((uint16_t)hc_num) = 0xFFFFFFFFU;

    /* enable channel interrupts required for this transfer */
    switch(puhc->endp_type) {
        case USB_EPTYPE_CTRL:
        case USB_EPTYPE_BULK:
            chinten |= HCHINTEN_TFIE | HCHINTEN_STALLIE | HCHINTEN_USBERIE \
                       | HCHINTEN_DTERIE | HCHINTEN_NAKIE;

            if(puhc->endp_in) {
                chinten |= HCHINTEN_BBERIE;
            } else {
                chinten |= HCHINTEN_NYETIE;
            }
            break;

        case USB_EPTYPE_INTR:
            chinten |= HCHINTEN_TFIE | HCHINTEN_STALLIE | HCHINTEN_USBERIE | HCHINTEN_DTERIE \
                       | HCHINTEN_NAKIE | HCHINTEN_REQOVRIE;

            if(puhc->endp_in) {
                chinten |= HCHINTEN_BBERIE;
            }
            break;

        case USB_EPTYPE_ISOC:
            chinten |= HCHINTEN_TFIE | HCHINTEN_REQOVRIE | HCHINTEN_ACKIE;

            if(puhc->endp_in) {
                chinten |= HCHINTEN_USBERIE | HCHINTEN_BBERIE;
            }
            break;

        default:
            break;
    }

    USB_HCHxINTEN((uint16_t)hc_num) = chinten;

    /* enable the top level host channel interrupt */
    USB_HACHINTEN |= 1U << hc_num;

    /* make sure host channel interrupts are enabled */
    USB_GINTEN |= GINTEN_HCIE;

    /* program the hcctlr register */
    chctl = 0U;

    if(HPRT_PRTSPD_LOW_SPEED == puhc->dev_speed) {
        is_low_speed = 1U;
    }

    chctl |= (uint32_t)puhc->dev_addr << 22U;
    chctl |= (uint32_t)puhc->endp_type << 18U;
    chctl |= (uint32_t)puhc->endp_id << 11U;
    chctl |= (uint32_t)puhc->endp_in << 15U;
    chctl |= (uint32_t)is_low_speed << 17U;
    chctl |= puhc->endp_mps;

    if(HCCHAR_INTR == puhc->endp_type) {
        chctl |= HCHCTL_ODDFRM;
    }

    USB_HCHxCTL((uint16_t)hc_num) = chctl;

    return USB_OK;
}

/*!
    \brief      prepare host channel for transferring packets
    \param[in]  pudev: pointer to usb device
    \param[in]  hc_num: host channel number which is in (0..7)
    \param[out] none
    \retval     operation status
*/
usb_status_enum usb_hostchannel_startxfer(usb_core_handle_struct *pudev, uint8_t hc_num)
{
    uint16_t dword_len = 0U;
    uint16_t packet_num = 0U;

    __IO uint32_t chxlen = 0U;
    __IO uint32_t chctl = 0U;

    usb_hostchannel_struct *puhc = &pudev->host.host_channel[hc_num];

    /* compute the expected number of packets associated to the transfer */
    if(puhc->xfer_len > 0U) {
        packet_num = ((uint16_t)puhc->xfer_len + puhc->endp_mps - 1U) / puhc->endp_mps;

        if(packet_num > HC_MAX_PACKET_COUNT) {
            packet_num = HC_MAX_PACKET_COUNT;
            puhc->xfer_len = (uint32_t)(packet_num) * (uint32_t)(puhc->endp_mps);
        }
    } else {
        packet_num = 1U;
    }

    if(puhc->endp_in) {
        puhc->xfer_len = (uint32_t)(packet_num) * (uint32_t)(puhc->endp_mps);
    }

    /* initialize the host channel length register */
    chxlen &= ~HCHLEN_TLEN;
    chxlen |= puhc->xfer_len;
    chxlen &= ~HCHLEN_PCNT;
    chxlen |= (uint32_t)packet_num << 19;
    chxlen &= ~HCHLEN_DPID;
    chxlen |= (uint32_t)(puhc->DPID) << 29;
    USB_HCHxLEN((uint16_t)hc_num) = (uint32_t)chxlen;

    /* set host channel enable */
    chctl = USB_HCHxCTL((uint16_t)hc_num);

    if(1 == USB_EVEN_FRAME()) {
        chctl |= HCHCTL_ODDFRM;
    } else {
        chctl &= ~HCHCTL_ODDFRM;
    }

    chctl |= HCHCTL_CEN;
    chctl &= ~HCHCTL_CDIS;
    USB_HCHxCTL((uint16_t)hc_num) = chctl;

    if((0U == puhc->endp_in) && (puhc->xfer_len > 0U)) {
        dword_len = (uint16_t)(puhc->xfer_len + 3U) / 4U;

        switch(puhc->endp_type) {
            /* non-periodic transfer */
            case USB_EPTYPE_CTRL:
            case USB_EPTYPE_BULK:
                /* check if there is enough space in fifo space */
                if(dword_len > (USB_HNPTFQSTAT & HNPTFQSTAT_NPTXFS)) {
                    /* need to process data in non-periodic transfer fifo empty interrupt */
                    USB_GINTEN |= GINTEN_NPTXFEIE;
                }
                break;

            /* periodic transfer */
            case USB_EPTYPE_INTR:
            case USB_EPTYPE_ISOC:
                /* check if there is enough space in FIFO space */
                if(dword_len > (USB_HPTFQSTAT & HPTFQSTAT_PTXFS)) {
                    /* need to process data in periodic transfer fifo empty interrupt */
                    USB_GINTEN |= GINTEN_PTXFEIE;
                }
                break;

            default:
                break;
        }

        /* write packet into the Tx FIFO. */
        usb_fifo_write(puhc->xfer_buff, hc_num, (uint16_t)puhc->xfer_len);
    }

    return USB_OK;
}

/*!
    \brief      halt channel
    \param[in]  pudev: pointer to usb device
    \param[in]  hc_num: host channel number which is in (0..7)
    \param[out] none
    \retval     operation status
*/
usb_status_enum usb_hostchannel_halt(usb_core_handle_struct *pudev, uint8_t hc_num)
{
    uint8_t endp_type = 0U;
    __IO uint32_t chctl = USB_HCHxCTL((uint16_t)hc_num);

    chctl |= HCHCTL_CEN | HCHCTL_CDIS;

    endp_type = (uint8_t)((chctl & HCHCTL_EPTYPE) >> 18U);

    /* check for space in the request queue to issue the halt. */
    if((HCCHAR_CTRL == endp_type) || (HCCHAR_BULK == endp_type)) {
        if(0U == (USB_HNPTFQSTAT & HNPTFQSTAT_NPTXFS)) {
            chctl &= ~HCHCTL_CEN;
        }
    } else {
        if(0U == (USB_HPTFQSTAT & HPTFQSTAT_PTXFS)) {
            chctl &= ~HCHCTL_CEN;
        }
    }

    USB_HCHxCTL((uint16_t)hc_num) = chctl;

    return USB_OK;
}

/*!
    \brief      stop the USB host and clean up fifos
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usb_host_stop(usb_core_handle_struct *pudev)
{
    uint32_t i;

    /* disable all host channel interrupt */
    USB_HACHINTEN = 0U;
    USB_HACHINT = 0xFFFFFFFFU;

    /* flush out any leftover queued requests */
    for(i = 0U; i < pudev->cfg.host_channel_num; i++) {
        USB_HCHxCTL(i) |= HCHCTL_CEN | HCHCTL_CDIS | HCHCTL_EPDIR;
    }

    /* flush the FIFO */
    usb_rxfifo_flush(pudev);
    usb_txfifo_flush(pudev, 0x10U);
}

#endif /* USE_HOST_MODE */


#ifdef USE_DEVICE_MODE

/* USB endpoint Tx FIFO size */
static uint16_t USBFS_TX_FIFO_SIZE[USBFS_MAX_DEV_EPCOUNT] = {
    (uint16_t)TX0_FIFO_FS_SIZE,
    (uint16_t)TX1_FIFO_FS_SIZE,
    (uint16_t)TX2_FIFO_FS_SIZE,
    (uint16_t)TX3_FIFO_FS_SIZE
};

static usb_status_enum usb_devint_enable(usb_core_handle_struct *pudev);

/*!
    \brief      initialize USB core registers for device mode
    \param[in]  pudev: pointer to usb device
    \param[out] none
    \retval     operation status
*/
usb_status_enum usb_devcore_init(usb_core_handle_struct *pudev)
{
    uint32_t i, ram_address = 0U;
    __IO uint32_t devinep0intf = USB_DIEP0TFLEN;
    __IO uint32_t devinepintf = 0U;

    /* restart the Phy Clock (Maybe don't need to...) */
    USB_PWRCLKCTL = 0U;

    /* config periodic frmae interval to default */
    USB_DCFG &= ~DCFG_EOPFT;
    USB_DCFG |= FRAME_INTERVAL_80;

    if(USB_FS_CORE_ID == pudev->cfg.core_id) {
        /* set full speed PHY */
        USB_DCFG &= ~DCFG_DS;
        USB_DCFG |= USB_SPEED_INP_FULL;

        /* set Rx FIFO size */
        USB_GRFLEN &= ~GRFLEN_RXFD;
        USB_GRFLEN |= (uint32_t)RX_FIFO_FS_SIZE;

        /* set endpoint 0 Tx FIFO length and RAM address */
        devinep0intf &= ~DIEP0TFLEN_IEP0TXFD;
        devinep0intf |= (uint32_t)TX0_FIFO_FS_SIZE << 16;
        devinep0intf &= ~DIEP0TFLEN_IEP0TXRSAR;
        devinep0intf |= (uint32_t)RX_FIFO_FS_SIZE;

        USB_DIEP0TFLEN = devinep0intf;

        ram_address = (uint32_t)RX_FIFO_FS_SIZE;

        /* set endpoint 1 to 3's Tx FIFO length and RAM address */
        for(i = 1U; i < USBFS_MAX_DEV_EPCOUNT; i++) {
            ram_address += USBFS_TX_FIFO_SIZE[i - 1U];

            devinepintf &= ~DIEPTFLEN_IEPTXFD;
            devinepintf |= (uint32_t)USBFS_TX_FIFO_SIZE[i] << 16U;
            devinepintf &= ~DIEPTFLEN_IEPTXRSAR;
            devinepintf |= ram_address;

            USB_DIEPxTFLEN(i) = devinepintf;
        }
    }

    /* make sure all FIFOs are flushed */

    /* flush all Tx FIFOs */
    usb_txfifo_flush(pudev, 0x10U);

    /* flush entire Rx FIFO */
    usb_rxfifo_flush(pudev);

    /* clear all pending device interrupts */
    USB_DIEPINTEN = 0U;
    USB_DOEPINTEN = 0U;
    USB_DAEPINT = 0xFFFFFFFFU;
    USB_DAEPINTEN = 0U;

    /* configure all IN/OUT endpoints */
    for(i = 0U; i < pudev->cfg.dev_endp_num; i++) {
        if(USB_DIEPxCTL(i) & DEPCTL_EPEN) {
            USB_DIEPxCTL(i) |= DEPCTL_EPD | DEPCTL_SNAK;
        } else {
            USB_DIEPxCTL(i) = 0U;
        }

        if(USB_DOEPxCTL(i) & DEPCTL_EPEN) {
            USB_DOEPxCTL(i) |= DEPCTL_EPD | DEPCTL_SNAK;
        } else {
            USB_DOEPxCTL(i) = 0U;
        }

        /* set IN/OUT endpoint transfer length to 0 */
        USB_DIEPxLEN(i) = 0U;
        USB_DOEPxLEN(i) = 0U;

        /* clear all pending IN/OUT endpoints interrupts */
        USB_DIEPxINTF(i) = 0xFFU;
        USB_DOEPxINTF(i) = 0xFFU;
    }

    usb_devint_enable(pudev);

    return USB_OK;
}

/*!
    \brief      enable the device mode interrupts
    \param[in]  pudev: pointer to usb device
    \param[out] none
    \retval     status
*/
static usb_status_enum usb_devint_enable(usb_core_handle_struct *pudev)
{
    uint32_t int_mask = 0U;

    /* disable all interrupts */
    USB_GINTEN = 0U;

    /* clear any pending interrupts */
    USB_GINTF = 0xBFFFFFFFU;

    /* enable the common interrupts */
    usb_commonint_enable(pudev);

    int_mask = GINTEN_RXFNEIE;

    /* enable device_mode-related interrupts */
    int_mask |= GINTEN_SPIE | GINTEN_RSTIE | GINTEN_ENUMFIE \
                | GINTEN_IEPIE | GINTEN_OEPIE | GINTEN_SOFIE | GINTEN_ISOONCIE \
                | GINTEN_ISOINCIE;

#ifdef VBUS_SENSING_ENABLED
    int_mask |= GINTEN_SESIE | GINTEN_OTGIE;
#endif /* VBUS_SENSING_ENABLED */

    USB_GINTEN &= ~int_mask;
    USB_GINTEN |= int_mask;

    return USB_OK;
}

/*!
    \brief      configures endpoint 0 to receive SETUP packets
    \param[in]  pudev: pointer to usb device
    \param[out] none
    \retval     none
*/
void usb_ep0_startout(usb_core_handle_struct *pudev)
{
    __IO uint32_t ep0len = 0U;

    /* set OUT endpoint 0 receive length to 24 bytes */
    ep0len &= ~DOEP0LEN_TLEN;
    ep0len |= 8U * 3U;

    /* set OUT endpoint 0 receive length to 1 packet */
    ep0len &= ~DOEP0LEN_PCNT;
    ep0len |= 1U << 19;

    /* set SETUP packet count to 3 */
    ep0len &= ~DOEP0LEN_STPCNT;
    ep0len |= 3U << 29;

    USB_DOEPxLEN(0U) = ep0len;
}

/*!
    \brief      active remote wakeup signalling
    \param[in]  pudev: pointer to usb device
    \param[out] none
    \retval     none
*/
void usb_remotewakeup_active(usb_core_handle_struct *pudev)
{
    __IO uint32_t power_clock;

    if(pudev->dev.remote_wakeup) {
        if(1U == (USB_DSTAT & DSTAT_SPST)) {
            if(pudev->cfg.low_power) {
                /* ungate USB core clock */
                power_clock = USB_PWRCLKCTL;
                power_clock &= ~PWRCLKCTL_SHCLK;
                power_clock &= ~PWRCLKCTL_SUCLK;

                USB_PWRCLKCTL = power_clock;
            }

            /* active remote wakeup signaling */
            USB_DCTL |= DCTL_RWKUP;

            if(pudev->mdelay != (void *)0) {
                pudev->mdelay(5U);
            }

            USB_DCTL &= ~DCTL_RWKUP;
        }
    }
}

/*!
    \brief      active USB core clock
    \param[in]  pudev: pointer to usb device
    \param[out] none
    \retval     none
*/
void usb_clock_ungate(usb_core_handle_struct *pudev)
{
    if(pudev->cfg.low_power) {
        __IO uint32_t power_clock;

        if(1U == (USB_DSTAT & DSTAT_SPST)) {
            /* un-gate USB core clock */
            power_clock = USB_PWRCLKCTL;
            power_clock &= ~PWRCLKCTL_SHCLK;
            power_clock &= ~PWRCLKCTL_SUCLK;

            USB_PWRCLKCTL = power_clock;
        }
    }
}

/*!
    \brief      stop the device and clean up fifos
    \param[in]  pudev: pointer to usb device
    \param[out] none
    \retval     none
*/
void usb_device_stop(usb_core_handle_struct *pudev)
{
    uint32_t i;

    pudev->dev.status = 1U;

    for(i = 0U; i < pudev->cfg.dev_endp_num; i++) {
        USB_DIEPxINTF(i) = 0xFFU;
        USB_DOEPxINTF(i) = 0xFFU;
    }

    USB_DIEPINTEN = 0U;
    USB_DOEPINTEN = 0U;
    USB_DAEPINTEN = 0U;
    USB_DAEPINT = 0xFFFFFFFFU;

    /* flush the FIFO */
    usb_rxfifo_flush(pudev);
    usb_txfifo_flush(pudev, 0x10U);
}
#endif /* USE_DEVICE_MODE */
