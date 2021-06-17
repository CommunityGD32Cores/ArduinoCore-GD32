/*!
    \file  usbh_std.h
    \brief header file for usbh_std.c

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

#ifndef USBH_STD_H
#define USBH_STD_H

#include "usbh_core.h"
#include "usbh_usr.h"

/* standard feature selector for clear feature command */
#define FEATURE_SELECTOR_ENDPOINT      0x00U
#define FEATURE_SELECTOR_DEVICE        0x01U

#define USBH_SETUP_PACKET_SIZE         8U      /* setup packet size */
#define ENUM_HANDLE_TABLE_SIZE         10U     /* enumerate handle table size */

extern uint8_t                         usbh_cfg_desc[512];
extern uint8_t                         enum_polling_handle_flag;
extern state_table_struct              enum_handle_table[ENUM_HANDLE_TABLE_SIZE];

typedef enum {
    ENUN_EVENT_IDLE = 0,                   /* the enum idle event */
    ENUM_EVENT_SET_ADDR,                   /* the enum set address event */
    ENUN_EVENT_GET_FULL_DEV_DESC,          /* the enum get full device descripter event */
    ENUN_EVENT_GET_CFG_DESC,               /* the enum get congiguration descripter event */
    ENUN_EVENT_GET_FULL_CFG_DESC,          /* the enum get full configuration descripter event */
    ENUN_EVENT_GET_MFC_STRING_DESC,        /* the enum get MFC string descripter event */
    ENUN_EVENT_GET_PRODUCT_STRING_DESC,    /* the enum get product string event */
    ENUN_EVENT_GET_SERIALNUM_STRING_DESC,  /* the enum get serialnum string event */
    ENUN_EVENT_SET_CONFIGURATION,          /* the enum set configuration event */
    ENUN_EVENT_DEV_CONFIGURED              /* the enum device configured event */
} enum_event_enum;

/* function declarations */
/* the polling function of enumeration state */
usbh_status_enum enum_state_polling_fun(usb_core_handle_struct *pudev, usbh_host_struct *puhost,
                                        void *pustate);
/* get descriptor in usb host enumeration stage */
void usbh_enum_desc_get(usb_core_handle_struct *pudev,
                        usbh_host_struct *puhost,
                        uint8_t *buf,
                        uint8_t  req_type,
                        uint16_t value_idx,
                        uint16_t len);
/* set address in usb host enumeration stage */
void usbh_enum_addr_set(usb_core_handle_struct *pudev, usbh_host_struct *puhost,
                        uint8_t device_address);
/* set configuration in usb host enumeration stage */
void usbh_enum_cfg_set(usb_core_handle_struct *pudev, usbh_host_struct *puhost, uint16_t cfg_idx);
/* parse the device descriptor */
void usbh_device_desc_parse(usb_descriptor_device_struct *dev_desc, uint8_t *buf, uint16_t len);
/* parse the configuration descriptor */
void usbh_cfg_desc_parse(usb_descriptor_configuration_struct *cfg_desc,
                         usb_descriptor_interface_struct *itf_desc,
                         usb_descriptor_endpoint_struct ep_desc[][USBH_MAX_EP_NUM],
                         uint8_t *buf,
                         uint16_t len);
/* parse the interface descriptor */
void usbh_interface_desc_parse(usb_descriptor_interface_struct *itf_desc, uint8_t *buf);
/* parse the endpoint descriptor */
void usbh_endpoint_desc_parse(usb_descriptor_endpoint_struct *ep_desc, uint8_t *buf);
/* parse the string descriptor */
void usbh_string_desc_parse(uint8_t *psrc, uint8_t *pdest, uint16_t len);
/* get the next descriptor header */
usb_descriptor_header_struct *usbh_next_desc_get(uint8_t *pbuf, uint16_t *ptr);

#endif /* USBH_STD_H */
