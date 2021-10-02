/*!
    \file    ccid_core.c
    \brief   CCID class driver

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

#include "usbd_transc.h"
#include "ccid_core.h"

#define USBD_VID                     0x28E9U
#define USBD_PID                     0x3258U

/* Note:it should use the C99 standard when compiling the below codes */
/* USB standard device descriptor */
usb_desc_dev ccid_dev_desc =
{
    .header = 
     {
         .bLength          = USB_DEV_DESC_LEN, 
         .bDescriptorType  = USB_DESCTYPE_DEV
     },
    .bcdUSB                = 0x0200U,
    .bDeviceClass          = 0x00U,
    .bDeviceSubClass       = 0x00U,
    .bDeviceProtocol       = 0x00U,
    .bMaxPacketSize0       = USBD_EP0_MAX_SIZE,
    .idVendor              = USBD_VID,
    .idProduct             = USBD_PID,
    .bcdDevice             = 0x0100U,
    .iManufacturer         = STR_IDX_MFC,
    .iProduct              = STR_IDX_PRODUCT,
    .iSerialNumber         = STR_IDX_SERIAL,
    .bNumberConfigurations = USBD_CFG_MAX_NUM
};

/* USB device configure descriptor */
usb_ccid_desc_config_set ccid_config_desc = 
{
    .config = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_config), 
             .bDescriptorType = USB_DESCTYPE_CONFIG 
         },
        .wTotalLength         = sizeof(usb_ccid_desc_config_set),
        .bNumInterfaces       = 0x01U,
        .bConfigurationValue  = 0x01U,
        .iConfiguration       = 0x04U,
        .bmAttributes         = 0x80U,
        .bMaxPower            = 0x32U
    },

    .ccid_itf = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_itf), 
             .bDescriptorType = USB_DESCTYPE_ITF
         },
        .bInterfaceNumber     = 0x00U,
        .bAlternateSetting    = 0x00U,
        .bNumEndpoints        = 0x03U,
        .bInterfaceClass      = USB_CCID_CLASS,
        .bInterfaceSubClass   = USB_CCID_SUBCLASS,
        .bInterfaceProtocol   = USB_CCID_PROTOCOL,
        .iInterface           = 0x05U
    },

    .ccid_func = 
    {
        .header = 
         {
             .bLength           = sizeof(usb_desc_ccid_class_func), 
             .bDescriptorType   = USB_DESCTYPE_CCID 
         },
        .bcdCCID                = 0x0110,     /* BCD 1.10 */
        .bMaxSlotIndex          = 0x00,       /* highest available slot on this device is 00, only one slot can be used */
        .bVoltageSupport        = 0x03,       /* support for 5.0V and 3.0V */
        .dwProtocols            = 0x00000001, /* Protocol T = 0 */
        .dwDefaultClock         = 0x00000E10, /* default supported ICC clock frequency is 3600KHz */
        .dwMaximumClock         = 0x00000E10, /* maximum supported ICC clock frequency is 3600KHz */
        .bNumClockSupported     = 0x00,       /* all clock frequency between dwDefaultClock and dwMaximumClock are supported */
        .dwDataRate             = 0x000025CD, /* default supported ICC I/O data rate is 9677bps */
        .dwMaxDataRate          = 0x000025CD, /* maximum supported ICC I/O data rate is 9677bps */
        .bNumDataRatesSupported = 0x00,       /* all data rates between dwDataRate and dwMaxDataRate are supported */
        .dwMaxIFSD              = 0x00000000, /* only for T = 1 */
        .dwSynchProtocols       = 0x00000000,
        .dwMechanical           = 0x00000000, /* no special characteristics */
        .dwFeatures             = 0x000100F8,
        .dwMaxCCIDMessageLength = 0x0000010F, /* (maximum block size) + header = (261 + 10) */
        .bClassGetResponse      = 0x00,
        .bClassEnvelope         = 0x00,
        .wLcdLayout             = 0x0000,     /* no LCD */
        .bPINSupport            = 0x00,       /* no PIN verify and modify */
        .bMaxCCIDBusySlots      = 0x01,
    },

    .ccid_command_ep = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_ep), 
             .bDescriptorType = USB_DESCTYPE_EP
         },
        .bEndpointAddress     = CCID_BULK_OUT_EP,
        .bmAttributes         = USB_EP_ATTR_BULK,
        .wMaxPacketSize       = CCID_DATA_PACKET_SIZE,
        .bInterval            = 0x00U
    },

    .ccid_response_ep = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_ep), 
             .bDescriptorType = USB_DESCTYPE_EP
         },
        .bEndpointAddress     = CCID_BULK_IN_EP,
        .bmAttributes         = USB_EP_ATTR_BULK,
        .wMaxPacketSize       = CCID_DATA_PACKET_SIZE,
        .bInterval            = 0x00U
    },

    .ccid_event_notify_ep = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_ep), 
             .bDescriptorType = USB_DESCTYPE_EP
         },
        .bEndpointAddress     = CCID_INT_IN_EP,
        .bmAttributes         = USB_EP_ATTR_INT,
        .wMaxPacketSize       = CCID_NOTIFY_PACKET_SIZE,
        .bInterval            = 0x10U
    }
};

/* USB language ID Descriptor */
usb_desc_LANGID usbd_language_id_desc = 
{
    .header = 
     {
         .bLength         = sizeof(usb_desc_LANGID), 
         .bDescriptorType = USB_DESCTYPE_STR
     },
    .wLANGID              = ENG_LANGID
};

/* USB manufacture string */
static usb_desc_str manufacturer_string = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(10U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'i', 'g', 'a', 'D', 'e', 'v', 'i', 'c', 'e'}
};

/* USB product string */
static usb_desc_str product_string = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(24U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'D', '3', '2', '-', 'U', 'S', 'B', '_', 'S', 'm', 'a', 'r', 't', 'C', 'a', 'r', 'd', 'R', 'e', 'a', 'd', 'e', 'r'}
};

/* USB serial string */
static usb_desc_str serial_string = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(12U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     }
};

/* USB product string */
static usb_desc_str config_string = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(19U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'D', '3', '2', '-', 'C', 'C', 'I', 'D', '_', 'C', 'o', 'n', 'f', 'i', 'g', 'u', 'r', 'e'}
};

/* USB product string */
static usb_desc_str interface_string = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(13U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'S', 'C', 'R', '_', 'I', 'n', 't', 'e', 'r', 'f', 'a', 'c', 'e'}
};

static uint8_t* usbd_ccid_strings[] = 
{
    [STR_IDX_LANGID]  = (uint8_t *)&usbd_language_id_desc,
    [STR_IDX_MFC]     = (uint8_t *)&manufacturer_string,
    [STR_IDX_PRODUCT] = (uint8_t *)&product_string,
    [STR_IDX_SERIAL]  = (uint8_t *)&serial_string,
    [STR_IDX_CONFIG]  = (uint8_t *)&config_string,
    [STR_IDX_ITF]     = (uint8_t *)&interface_string
};

usb_desc ccid_desc = {
    .dev_desc    = (uint8_t *)&ccid_dev_desc,
    .config_desc = (uint8_t *)&ccid_config_desc,
    .strings     = usbd_ccid_strings
};

/* local function prototypes ('static') */
static uint8_t ccid_init          (usb_dev *udev, uint8_t config_index);
static uint8_t ccid_deinit        (usb_dev *udev, uint8_t config_index);
static uint8_t ccid_req_handler   (usb_dev *udev, usb_req *req);
static void ccid_data_in_handler  (usb_dev *udev, uint8_t ep_num);
static void ccid_data_out_handler (usb_dev *udev, uint8_t ep_num);

usb_class ccid_class = {
    .init            = ccid_init,
    .deinit          = ccid_deinit,
    .req_process     = ccid_req_handler,
    .data_in         = ccid_data_in_handler,
    .data_out        = ccid_data_out_handler
};

/*!
    \brief      initialize the CCID device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t ccid_init (usb_dev *udev, uint8_t config_index)
{
    /* initialize Tx and Rx endpoint */
    usbd_ep_init(udev, EP_BUF_SNG, BULK_RX_ADDR, &(ccid_config_desc.ccid_command_ep));
    usbd_ep_init(udev, EP_BUF_SNG, BULK_TX_ADDR, &(ccid_config_desc.ccid_response_ep));
    usbd_ep_init(udev, EP_BUF_SNG, INT_TX_ADDR, &(ccid_config_desc.ccid_event_notify_ep));

    udev->ep_transc[EP_ID(CCID_BULK_OUT_EP)][TRANSC_OUT] = ccid_class.data_out;
    udev->ep_transc[EP_ID(CCID_BULK_IN_EP)][TRANSC_IN] = ccid_class.data_in;
    udev->ep_transc[EP_ID(CCID_INT_IN_EP)][TRANSC_IN] = ccid_class.data_in;

    /* initialize the CCID layer */
    CCID_init(udev);

    return USBD_OK;
}

/*!
    \brief      deinitialize the CCID device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t ccid_deinit (usb_dev *udev, uint8_t config_index)
{
    /* deinitialize CCID endpoints */
    usbd_ep_deinit (udev, CCID_BULK_OUT_EP);
    usbd_ep_deinit (udev, CCID_BULK_IN_EP);
    usbd_ep_deinit (udev, CCID_INT_IN_EP);

    /* deinitialize the CCID layer */
    CCID_deinit (udev);

    return USBD_OK;
}

/*!
    \brief      handle the CCID class-specific requests
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: device class-specific request
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t ccid_req_handler (usb_dev *udev, usb_req *req)
{
    uint8_t status = REQ_NOTSUPP;
    uint8_t slot_num;
    uint8_t seq_num;
    uint16_t len;

    uint8_t ccid_ep0_buffer[USBD_EP0_MAX_SIZE];

    switch (req->bRequest) {
    case REQUEST_ABORT:
        if ((0U == req->wLength) && (0x80U != (req->bmRequestType & 0x80U))) {
            /* the wValue field contains the slot number (bSlot) in the low byte 
            and the sequence number (bSeq) in the high byte. */
            slot_num = (uint8_t) ((req->wValue) & 0x00ffU);
            seq_num = (uint8_t) (((req->wValue) & 0xff00U) >> 8U);

            CCID_cmd_abort(udev, slot_num, seq_num);
            status = REQ_SUPP; 
        }
        break;

    case REQUEST_GET_CLOCK_FREQUENCIES:
        if ((0U == req->wValue) && (0U != req->wLength) && (0x80U == (req->bmRequestType & 0x80U))) {
            sc_request_get_clock_freq(ccid_ep0_buffer, &len);

            usb_transc_config(&udev->transc_in[0U], 
                              (uint8_t *)ccid_ep0_buffer, 
                              USB_MIN(USBD_EP0_MAX_SIZE, len), 
                              0U);

            status = REQ_SUPP; 
        }
        break;
        
    case REQUEST_GET_DATA_RATES:
        if ((0U == req->wValue) && (0U != req->wLength) && (0x80U == (req->bmRequestType & 0x80U))) {
            sc_request_get_data_rates(ccid_ep0_buffer, &len);

            usb_transc_config(&udev->transc_in[0U], 
                              (uint8_t *)ccid_ep0_buffer, 
                              USB_MIN(USBD_EP0_MAX_SIZE, len), 
                              0U);

            status = REQ_SUPP;
        }
        break;

    default:
        break;
    }

    return status;
}

/*!
    \brief      handle data stage in DATA IN transaction
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint identifier
    \param[out] none
    \retval     none
*/
static void ccid_data_in_handler (usb_dev *udev, uint8_t ep_num)
{
    CCID_bulk_message_in(udev, ep_num);
}

/*!
    \brief      handle data stage in DATA OUT transaction
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint identifier
    \param[out] none
    \retval     none
*/
static void ccid_data_out_handler (usb_dev *udev, uint8_t ep_num)
{
    if ((CCID_BULK_OUT_EP & 0x7FU) == ep_num) {
        CCID_bulk_message_out(udev , ep_num);
    }
}
