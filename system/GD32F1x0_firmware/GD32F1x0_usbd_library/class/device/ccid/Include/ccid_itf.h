/*!
    \file    ccid_itf.h
    \brief   functions prototypes for USB CCID

    \version 2020-07-23, V3.0.0, firmware for GD32F1x0
    \version 2022-06-30, V3.1.0, firmware for GD32F1x0
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

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

#ifndef __CCID_ITF_H
#define __CCID_ITF_H

#include "ccid_core.h"

/* bulk-only command block wrapper */
#define ABDATA_SIZE                             261U
#define CCID_CMD_HEADER_SIZE                    10U
#define CCID_RESPONSE_HEADER_SIZE               10U

#define CCID_INT_BUFF_SIZ                       2U

#define CARD_SLOT_FITTED                        1U
#define CARD_SLOT_REMOVED                       0U

#define BULK_MAX_PACKET_SIZE                    64U
#define CCID_IN_EP_SIZE                         64U
#define INT_MAX_PACKET_SIZE                     8U
#define CCID_MESSAGE_HEADER_SIZE                10U
#define CCID_NUMBER_OF_SLOTS                    1U

/* following parameters used in PC_to_RDR_IccPowerOn */
#define VOLTAGE_SELECTION_AUTOMATIC             0x00U
#define VOLTAGE_SELECTION_3V                    0x02U
#define VOLTAGE_SELECTION_5V                    0x01U
#define VOLTAGE_SELECTION_1V8                   0x03U

#define PC_TO_RDR_ICCPOWERON                    0x62U
#define PC_TO_RDR_ICCPOWEROFF                   0x63U
#define PC_TO_RDR_GETSLOTSTATUS                 0x65U
#define PC_TO_RDR_XFRBLOCK                      0x6FU
#define PC_TO_RDR_GETPARAMETERS                 0x6CU
#define PC_TO_RDR_RESETPARAMETERS               0x6DU
#define PC_TO_RDR_SETPARAMETERS                 0x61U
#define PC_TO_RDR_ESCAPE                        0x6BU
#define PC_TO_RDR_ICCCLOCK                      0x6EU
#define PC_TO_RDR_T0APDU                        0x6AU
#define PC_TO_RDR_SECURE                        0x69U
#define PC_TO_RDR_MECHANICAL                    0x71U
#define PC_TO_RDR_ABORT                         0x72U
#define PC_TO_RDR_SETDATARATEANDCLOCKFREQUENCY  0x73U

#define RDR_TO_PC_DATABLOCK                     0x80U
#define RDR_TO_PC_SLOTSTATUS                    0x81U
#define RDR_TO_PC_PARAMETERS                    0x82U
#define RDR_TO_PC_ESCAPE                        0x83U
#define RDR_TO_PC_DATARATEANDCLOCKFREQUENCY     0x84U

#define RDR_TO_PC_NOTIFYSLOTCHANGE              0x50U
#define RDR_TO_PC_HARDWAREERROR                 0x51U

#define OFFSET_INT_BMESSAGETYPE                 0U
#define OFFSET_INT_BMSLOTICCSTATE               1U

/* LSb: (0b = no ICC present, 1b = ICC present) */
#define SLOT_ICC_PRESENT                        0x01U

/* MSb : (0b = no change, 1b = change) */
#define SLOT_ICC_CHANGE                         0x02U  

/* CCID bulk transfer state machine */
#define CCID_STATE_IDLE                         0U /* idle state */
#define CCID_STATE_DATA_OUT                     1U /* data out state */
#define CCID_STATE_RECEIVE_DATA                 2U
#define CCID_STATE_SEND_RESP                    3U
#define CCID_STATE_DATAIN                       4U
#define CCID_STATE_UNCORRECT_LENGTH             5U

#define DIR_IN                                  0U
#define DIR_OUT                                 1U
#define BOTH_DIR                                2U

#pragma pack(1)
typedef struct 
{
    uint8_t bMessageType;                       /* offset = 0*/
    uint32_t dwLength;                          /* offset = 1, the length field (dwLength) is the length
                                                   of the message not including the 10-byte header.*/
    uint8_t bSlot;                              /* offset = 5*/
    uint8_t bSeq;                               /* offset = 6*/
    uint8_t bSpecific_0;                        /* offset = 7*/
    uint8_t bSpecific_1;                        /* offset = 8*/
    uint8_t bSpecific_2;                        /* offset = 9*/
    uint8_t abData [ABDATA_SIZE];               /* offset = 10, for reference, the absolute 
                                                   maximum block size for a TPDU T=0 block is 260 bytes 
                                                   (5 bytes command; 255 bytes data), or for a TPDU T=1 
                                                   block is 259 bytes, or for a short APDU T=1 block is 
                                                   261 bytes, or for an extended APDU T=1 block is 65544 bytes.*/
} ccid_bulkout_data_t; 

typedef struct 
{
    uint8_t bMessageType;                       /* offset = 0*/
    uint32_t dwLength;                          /* offset = 1*/
    uint8_t bSlot;                              /* offset = 5, same as bulk-out message */
    uint8_t bSeq;                               /* offset = 6, same as bulk-out message */
    uint8_t bStatus;                            /* offset = 7, slot status */
    uint8_t bError;                             /* offset = 8, slot error */
    uint8_t bSpecific;                          /* offset = 9*/
    uint8_t abData[ABDATA_SIZE];                /* offset = 10*/
    uint16_t u16SizeToSend; 
}ccid_bulkin_data_t; 

#pragma pack()

typedef struct 
{
    __IO uint8_t SlotStatus;
    __IO uint8_t SlotStatusChange;
}ccid_slot_status_t; 

typedef struct 
{
    __IO uint8_t bAbortRequestFlag; 
    __IO uint8_t bSeq; 
    __IO uint8_t bSlot;
}usb_ccid_param_t;

typedef struct
{
    uint8_t CCID_bulk_state;
    usb_ccid_param_t usb_ccid_param;
    __IO uint8_t pre_xfer_compl_INT_IN;
    uint32_t usb_message_len;
    uint8_t* pointer_message_buff;
    ccid_slot_status_t CCID_slot_status;
    ccid_bulkin_data_t CCID_bulkin_data;
    ccid_bulkout_data_t CCID_bulkout_data;
    uint8_t bulkout_data_buff[BULK_MAX_PACKET_SIZE];
    uint8_t interrupt_message_buff[INT_MAX_PACKET_SIZE];
} usb_ccid_handler;

/* function declarations */
/* initialize the CCID USB layer */
void CCID_init(usb_dev *udev);
/* deinitialize the CCID machine */
void CCID_deinit(usb_dev *udev);
/* handle bulk IN & INT IN data stage */
void CCID_bulk_message_in(usb_dev *udev, uint8_t epnum);
/* process CCID OUT data */
void CCID_bulk_message_out(usb_dev *udev, uint8_t epnum);
/* parse the commands and process command */
void CCID_cmd_decode(usb_dev *udev);
/* prepare the request response to be sent to the host */
void transfer_data_request(usb_dev *udev, uint8_t* buffer, uint16_t len);
/* send the data on BULK-IN EP */
void CCID_response_send_data(usb_dev* udev, uint8_t* buffer, uint16_t len);
/* send the INT-IN data to the host */
void CCID_INT_message(usb_dev* udev);
/* receive the data from USB BULK-OUT buffer to pointer */
void CCID_receive_cmd_header(usb_dev *udev, uint8_t* pDst, uint8_t len);
/* provides the status of previous interrupt transfer status */
uint8_t CCID_IS_INT_transfer_complete(usb_dev *udev);
/* provides the status of previous interrupt transfer status */
void CCID_set_INT_transfer_status (usb_dev *udev, uint8_t xfer_status);

#endif /* __CCID_ITF_H */
