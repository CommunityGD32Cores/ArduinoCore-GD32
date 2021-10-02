/*!
    \file    ccid_core.h
    \brief   definitions for CCID core

    \version 2020-07-23, V3.0.0, firmware for GD32F1x0
*/

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
*/

#ifndef __CCID_CORE_H
#define __CCID_CORE_H

#include "usbd_enum.h"
#include "ccid_cmd.h"
#include "ccid_itf.h"
#include "smartcard.h"
#include "smartcard_itf.h"

/* CCID functional descriptor type */
#define USB_DESCTYPE_CCID              0x21U

/* CCID device class code */
#define USB_CCID_CLASS                 0x0BU

/* CCID subclass code */
#define USB_CCID_SUBCLASS              0x00U
#define USB_CCID_PROTOCOL              0x00U

/* class-specific request codes */
#define REQUEST_ABORT                  0x01U
#define REQUEST_GET_CLOCK_FREQUENCIES  0x02U
#define REQUEST_GET_DATA_RATES         0x03U

#define TPDU_EXCHANGE                  0x01U
#define SHORT_APDU_EXCHANGE            0x02U
#define EXTENDED_APDU_EXCHANGE         0x04U
#define CHARACTER_EXCHANGE             0x00U
#define EXCHANGE_LEVEL_FEATURE         TPDU_EXCHANGE

#pragma pack(1)

/* smart card device class descriptors structure */
typedef struct {
    usb_desc_header header;               /*!< descriptor header, including type and size. */
    uint16_t  bcdCCID;                    /*!< bcdCCID: CCID Class specification release number in Binary-Coded decimal */
    uint8_t   bMaxSlotIndex;              /*!< bMaxSlotIndex: Highest available slot on this device. All slots are consecutive starting at 00 */
    uint8_t   bVoltageSupport;            /*!< bVoltageSupport: It is bitwise OR for 01h-5.0V 02h-3.0V 04h-1.8V, others for future use */
    uint32_t  dwProtocols;                /*!< dwProtocols: 0001h = Protocol T = 0, 0002h = Protocol T = 1 */
    uint32_t  dwDefaultClock;             /*!< dwDefaultClock: Default ICC clock frequency in KHz. */
    uint32_t  dwMaximumClock;             /*!< dwMaximumClock: Maximum supported ICC clock frequency in KHz. */
    uint8_t   bNumClockSupported;         /*!< bNumClockSupported: No setting from PC If the value is 00h, the supported clock frequencies 
                                               are assumed to be the default clock frequency defined by dwDefaultClock and the maximum clock 
                                               frequency defined by dwMaximumClock  */
    uint32_t  dwDataRate;                 /*!< dwDataRate: Default ICC I/O data rate in bps. */
    uint32_t  dwMaxDataRate;              /*!< dwMaxDataRate: Maximum supported ICC I/O data rate in bps */
    uint8_t   bNumDataRatesSupported;     /*!< NumDataRatesSupported: The number of data rates that are supported by the CCID. If the value is 00h, 
                                               all data rates between the default data rate dwDataRate and the maximum data rate dwMaxDataRate are 
                                               supported. Don't support GET_CLOCK_FREQUENCIES */
    uint32_t  dwMaxIFSD;                  /*!< dwMaxIFSD: Indicates the maximum IFSD supported by CCID for protocol T=1. */
    uint32_t  dwSynchProtocols;           /*!< dwSynchProtocols: Upper-Word is RFU = 0000h, Lower-Word encodes the supported protocol types. A '1' 
                                               in a given bit position indicates support for the associated protocol.
                                               - 0001h indicates support for the 2-wire protocol 1
                                               - 0002h indicates support for the 3-wire protocol 1
                                               - 0004h indicates support for the I2C protocol 1
                                               All other values are outside of this specification, and must be handled by vendor-supplied drivers.*/
    uint32_t  dwMechanical;               /*!< dwMechanical: The value is a bitwise OR operation performed on the following values: 
                                               - 00000000h No special characteristics
                                               - 00000001h Card accept mechanism 2
                                               - 00000002h Card ejection mechanism 2
                                               - 00000004h Card capture mechanism 2
                                               - 00000008h Card lock/unlock mechanism */
    uint32_t  dwFeatures;                 /*!< dwFeatures: clk, baud rate, voltage: automatic
                                               - 00000008h Automatic ICC voltage selection 
                                               - 00000010h Automatic ICC clock frequency change
                                               - 00000020h Automatic baud rate change according to active parameters provided by the Host or self determined
                                               - 00000040h Automatic parameters negotiation made by the CCID (use of warm or cold resets or PPS according
                                                           to a manufacturer proprietary algorithm to select the communication parameters with the ICC)
                                               Only one of the following values may be present to select a level of exchange:
                                               - 00010000h TPDU level exchanges with CCID
                                               - 00020000h Short APDU level exchange with CCID
                                               - 00040000h Short and Extended APDU level exchange 
                                               If none of those values : character level of exchange */
    uint32_t  dwMaxCCIDMessageLength;     /*!< dwMaxCCIDMessageLength: For extended APDU level the value shall be between 261 + 10 (header) and 65544 +10, 
                                               otherwise the minimum value is the wMaxPacketSize of the Bulk-OUT endpoint. */
    uint8_t   bClassGetResponse;          /*!< bClassGetResponse: Significant only for CCID that offers an APDU level for exchanges. Indicates the 
                                               default class value used by the CCID when it sends a Get Response command to perform the transportation of
                                               an APDU by T=0 protocol. Value FFh indicates that the CCID echoes the class of the APDU. */
    uint8_t   bClassEnvelope;             /*!< bClassEnvelope: Significant only for CCID that offers an extended APDU level for exchanges. Indicates the 
                                               default class value used by the CCID when it sends an Envelope command to perform the transportation of
                                               an extended APDU by T=0 protocol. Value FFh indicates that the CCID echoes the class of the APDU. */
    uint16_t  wLcdLayout;                 /*!< wLcdLayout: Number of lines and characters for the LCD display used to send messages for PIN entry. */
    uint8_t   bPINSupport;                /*!< bPINSupport: This value indicates what PIN support features the CCID has. The value is a bitwise 
                                               OR operation performed on the following values:
                                               - 01h PIN Verification supported
                                               - 02h PIN Modification supported */
    uint8_t   bMaxCCIDBusySlots;          /*!< bMaxCCIDBusySlots: Maximum number of slots which can be simultaneously busy. */
} usb_desc_ccid_class_func;

#pragma pack()

typedef struct {
    usb_desc_config                  config;
    usb_desc_itf                     ccid_itf;
    usb_desc_ccid_class_func         ccid_func;
    usb_desc_ep                      ccid_command_ep;
    usb_desc_ep                      ccid_response_ep;
    usb_desc_ep                      ccid_event_notify_ep;
} usb_ccid_desc_config_set;

extern usb_desc ccid_desc;
extern usb_class ccid_class;

#endif /* __CCID_CORE_H */
