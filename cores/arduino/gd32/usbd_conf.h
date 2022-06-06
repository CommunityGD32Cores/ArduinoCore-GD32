#ifndef __USBD_CONF_H
#define __USBD_CONF_H

#if USBCON

#include "gd32xxyy.h"
#include "variant.h"

/*
 * define if low power mode is enabled; it allows entering the device
 * into DEEP_SLEEP mode following USB suspend event and wakes up after
 * the USB wakeup event is received.
 */
#undef USBD_LOWPWR_MODE_ENABLE

/* USB feature -- Self Powered */
#undef USBD_IS_SELF_POWERED

/* link power mode support */
#undef LPM_ENABLED

/*
 * TODO: I’m currently using the maximum values allowed by the spec
 * for available interfaces and endpoints, because I can’t know this
 * ahead of time when using PluggableUSB. However, this wastes a fair
 * amount of memory: almost no device is going to have 256 interfaces.
 */
#define USBD_CFG_MAX_NUM 1
#define USBD_ITF_MAX_NUM 256

#define EP_COUNT 8

#define USB_STRING_COUNT 4

/*
 * Offset from USBD RAM base used to store endpoint buffer
 * descriptors.
 *
 * cf. GD32F30x User Manual §24.6.1.
 */
#define BTABLE_OFFSET 0

/*
 * Offsets from BTABLE in the peripheral for transmission and
 * reception buffers.
 *
 * These offsets are stored directly in the ‘USBD_EPxTBADDR’ and
 * ‘USBD_EPxRBADDR’ registers, and thus are half the real offset used
 * when accessing the data buffer.
 *
 * Other endpoint buffers are come after ‘EP0_RX_ADDR’, and assume the
 * maximum packet size is the same for all endpoints, at
 * ‘USBD_EP0_MAX_SIZE’ octets.
 */
#define EP0_TX_ADDR 0x40
#define EP0_RX_ADDR (EP0_TX_ADDR+USBD_EP0_MAX_SIZE)

#endif
#endif /* __USBD_CONF_H */
