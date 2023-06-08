/*!
    \file    ccid_itf.c
    \brief   CCID interface driver

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

#include "ccid_itf.h"
#include <string.h>

/* local function prototypes ('static') */
static void CCID_receive_cmd_header(usb_dev *udev, uint8_t* pDst, uint8_t len);
static uint8_t CCID_IS_INT_transfer_complete(usb_dev *udev);
static void CCID_set_INT_transfer_status (usb_dev *udev, uint8_t xfer_status);

/*!
    \brief      initialize the CCID USB layer
    \param[in]  udev: pointer to USB core instance
    \param[out] none
    \retval     none
*/
void CCID_init(usb_dev* udev)
{
    static usb_ccid_handler ccid_handler;

    memset((void *)&ccid_handler, 0, sizeof(usb_ccid_handler));

    udev->class_data[USBD_CCID_INTERFACE] = (void *)&ccid_handler;

    /* transfer complete status */
    CCID_set_INT_transfer_status(udev, 1U);

    /* update slot change */
    CCID_update_slot_change(udev, 1U);

    /* prepare EP to receive first cmd */
    usbd_ep_recev(udev, CCID_BULK_OUT_EP, ccid_handler.bulkout_data_buff, CCID_DATA_PACKET_SIZE);
}

/*!
    \brief      deinitialize the CCID machine
    \param[in]  udev: pointer to USB core instance
    \param[out] none
    \retval     none
*/
void CCID_deinit(usb_dev* udev)
{
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    ccid->CCID_bulk_state = CCID_STATE_IDLE;
}

/*!
    \brief      handle bulk IN & INT IN data stage 
    \param[in]  udev: pointer to USB core instance
    \param[in]  epnum: endpoint identifier
    \param[out] none
    \retval     none
*/
void CCID_bulk_message_in(usb_dev* udev, uint8_t epnum)
{
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    if((CCID_BULK_IN_EP & 0x7FU) == epnum){
        /* handle bulk transfer IN data completion */
        switch (ccid->CCID_bulk_state){
        case CCID_STATE_SEND_RESP:
            ccid->CCID_bulk_state = CCID_STATE_IDLE;

            /* prepare EP to receive first cmd */
            usbd_ep_recev(udev, CCID_BULK_OUT_EP, ccid->bulkout_data_buff, CCID_DATA_PACKET_SIZE);
            break;

        default:
            break;
        }
    }else if((CCID_INT_IN_EP & 0x7FU) == epnum){
        /* transfer complete status */
        CCID_set_INT_transfer_status(udev, 1U);
    }
}

/*!
    \brief      process CCID OUT data
    \param[in]  udev: pointer to USB core instance
    \param[in]  epnum: endpoint identifier
    \param[out] none
    \retval     none
*/
void CCID_bulk_message_out(usb_dev *udev, uint8_t epnum)
{
    uint16_t dataLen;

    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    dataLen = udev->transc_out[epnum].xfer_count;

    switch (ccid->CCID_bulk_state){
    case CCID_STATE_IDLE:
        if(0x0 == dataLen){
            /* zero length packet received */
            ccid->CCID_bulk_state = CCID_STATE_IDLE;
        }else if(dataLen >= CCID_MESSAGE_HEADER_SIZE){
            /* store for future use */
            ccid->usb_message_len = dataLen;

            /* expected data length packet received */
            ccid->pointer_message_buff = (uint8_t*) &ccid->CCID_bulkout_data;

            /* fill CCID_BulkOut data buffer from usb buffer */
            CCID_receive_cmd_header(udev, ccid->pointer_message_buff, dataLen); 

            /* refer: 6 CCID messages
               the response messages always contain the exact same slot number, 
               and sequence number fields from the header that was contained in 
               the Bulk-OUT command message. */
            ccid->CCID_bulkin_data.bSlot = ccid->CCID_bulkout_data.bSlot;
            ccid->CCID_bulkin_data.bSeq = ccid->CCID_bulkout_data.bSeq;

            if(dataLen < CCID_DATA_PACKET_SIZE){
                /* short message, less than the EP Out Size, execute the command,
                   if parameter like dwLength is too big, the appropriate command will 
                   give an error */
                CCID_cmd_decode(udev);
            }else{
                /* long message, receive additional data with command */
                if(ccid->CCID_bulkout_data.dwLength > ABDATA_SIZE){
                    /* check if length of data to be sent by host is > buffer size */

                    /* too long data received, error */
                    ccid->CCID_bulk_state = CCID_STATE_UNCORRECT_LENGTH;
                }else{
                    /* expect more data on out EP */
                    ccid->CCID_bulk_state = CCID_STATE_RECEIVE_DATA;
                    ccid->pointer_message_buff += dataLen;  /* Point to new offset */

                    /* prepare EP to receive next cmd */
                    usbd_ep_recev(udev, CCID_BULK_OUT_EP, (uint8_t *)&ccid->bulkout_data_buff[0], CCID_DATA_PACKET_SIZE);
      
                }
            }
        }
        break;

    case CCID_STATE_RECEIVE_DATA:
        ccid->usb_message_len += dataLen;

        if(dataLen < CCID_DATA_PACKET_SIZE){
            /* short message, less than the EP Out Size, execute the command,
               if parameter like dwLength is too big, the appropriate command will 
               give an error */

            /* full command is received, process the command */
            CCID_receive_cmd_header(udev, ccid->pointer_message_buff, dataLen);
            CCID_cmd_decode(udev); 
        }else if(CCID_DATA_PACKET_SIZE == dataLen){
            if (ccid->usb_message_len < (ccid->CCID_bulkout_data.dwLength + CCID_CMD_HEADER_SIZE)){
                /* copy data */
                CCID_receive_cmd_header(udev, ccid->pointer_message_buff, dataLen);

                /* increment the pointer to receive more data */
                ccid->pointer_message_buff += dataLen;

                /* prepare EP to receive next cmd */
                usbd_ep_recev(udev, CCID_BULK_OUT_EP, (uint8_t *)&ccid->bulkout_data_buff[0], CCID_DATA_PACKET_SIZE);
            }else if (ccid->usb_message_len == (ccid->CCID_bulkout_data.dwLength + CCID_CMD_HEADER_SIZE)){
                /* full command is received, process the command */
                CCID_receive_cmd_header(udev, ccid->pointer_message_buff, dataLen);
                CCID_cmd_decode(udev);
            }else{
                /* too long data received, error */
                ccid->CCID_bulk_state = CCID_STATE_UNCORRECT_LENGTH;
            }
        }
        break;

    case CCID_STATE_UNCORRECT_LENGTH:
        ccid->CCID_bulk_state = CCID_STATE_IDLE;
        break;

    default:
        break;
    }
}

/*!
    \brief      parse the commands and process command
    \param[in]  udev: pointer to USB core instance
    \param[out] none
    \retval     none
*/
void CCID_cmd_decode(usb_dev* udev)
{
    uint8_t errorCode;

    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    switch (ccid->CCID_bulkout_data.bMessageType){
    case PC_TO_RDR_ICCPOWERON:
        errorCode = PC_to_RDR_ICC_poweron(udev);
        RDR_to_PC_data_block(udev, errorCode);
        break;

    case PC_TO_RDR_ICCPOWEROFF:
        errorCode = PC_to_RDR_ICC_poweroff(udev);
        RDR_to_PC_slot_status(udev, errorCode);
        break;

    case PC_TO_RDR_GETSLOTSTATUS:
        errorCode = PC_to_RDR_get_slot_status(udev);
        RDR_to_PC_slot_status(udev, errorCode);
        break;

    case PC_TO_RDR_XFRBLOCK:
        errorCode = PC_to_RDR_xfr_block(udev);
        RDR_to_PC_data_block(udev, errorCode);
        break;

    case PC_TO_RDR_GETPARAMETERS:
        errorCode = PC_to_RDR_get_parameters(udev);
        RDR_to_PC_parameters(udev, errorCode);
        break;

    case PC_TO_RDR_RESETPARAMETERS:
        errorCode = PC_to_RDR_reset_parameters(udev);
        RDR_to_PC_parameters(udev, errorCode);
        break;

    case PC_TO_RDR_SETPARAMETERS:
        errorCode = PC_to_RDR_set_parameters(udev);
        RDR_to_PC_parameters(udev, errorCode);
        break;

    case PC_TO_RDR_ESCAPE:
        errorCode = PC_to_RDR_escape(udev);
        RDR_to_PC_escape(udev, errorCode);
        break;

    case PC_TO_RDR_ICCCLOCK:
        errorCode = PC_to_RDR_ICC_clock(udev);
        RDR_to_PC_slot_status(udev, errorCode);
        break;

    case PC_TO_RDR_ABORT:
        errorCode = PC_to_RDR_abort(udev);
        RDR_to_PC_slot_status(udev, errorCode);
        break;

    case PC_TO_RDR_T0APDU:
        errorCode = PC_to_RDR_T0_APDU(udev);
        RDR_to_PC_slot_status(udev, errorCode);
        break;

    case PC_TO_RDR_MECHANICAL:
        errorCode = PC_to_RDR_mechanical(udev);
        RDR_to_PC_slot_status(udev, errorCode);
        break;

    case PC_TO_RDR_SETDATARATEANDCLOCKFREQUENCY:
        errorCode = PC_to_RDR_Set_DataRate_and_ClockFreq(udev);
        RDR_to_PC_DataRate_and_ClockFreq(udev, errorCode);
        break;

    case PC_TO_RDR_SECURE:
        errorCode = PC_TO_RDR_secure(udev);
        RDR_to_PC_data_block(udev, errorCode);
        break;

    default:
        RDR_to_PC_slot_status(udev, SLOTERROR_CMD_NOT_SUPPORTED);
        break;
    }

     /* decide for all commands */
    if (CCID_STATE_SEND_RESP == ccid->CCID_bulk_state){
        CCID_response_send_data(udev, (uint8_t*)&ccid->CCID_bulkin_data, ccid->CCID_bulkin_data.u16SizeToSend);
    }
}

/*!
    \brief      prepare the request response to be sent to the host
    \param[in]  udev: pointer to USB core instance
    \param[in]  buffer: pointer to the data buffer to send
    \param[in]  len: number of bytes to send
    \param[out] none
    \retval     none
*/
void transfer_data_request(usb_dev *udev, uint8_t* buffer, uint16_t len)
{
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    /* update global variables */
    ccid->CCID_bulkin_data.u16SizeToSend = len;
    ccid->CCID_bulk_state = CCID_STATE_SEND_RESP;
}

/*!
    \brief      send the data on BULK-IN EP 
    \param[in]  udev: pointer to USB core instance
    \param[in]  buffer: pointer to the data buffer to send
    \param[in]  len: number of bytes to send
    \param[out] none
    \retval     none
*/
void CCID_response_send_data(usb_dev* udev, uint8_t* buffer, uint16_t len)
{
    usbd_ep_send(udev, CCID_BULK_IN_EP, buffer, len);
}

/*!
    \brief      send the INT-IN data to the host
    \param[in]  udev: pointer to USB core instance
    \param[out] none
    \retval     none
*/
void CCID_INT_message(usb_dev* udev)
{
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    /* check if there is change in smart card slot status */
    if(CCID_IS_slot_status_change(udev) && CCID_IS_INT_transfer_complete(udev)){
        /* Check slot status is changed. Card is removed/fitted */
        RDR_to_PC_notify_slot_change(udev);

        /* reset the status */
        CCID_set_INT_transfer_status(udev, 0U);

        /* reset the status of slot change */
        CCID_update_slot_change(udev, 0U);

        usbd_ep_send(udev, CCID_INT_IN_EP, ccid->interrupt_message_buff, 2U);
    }
}

/*!
    \brief      receive the data from USB BULK-OUT buffer to pointer
    \param[in]  udev: pointer to USB core instance
    \param[in]  pDst: destination address to copy the buffer
    \param[in]  len: length of data to copy
    \param[out] none
    \retval     none
*/
void CCID_receive_cmd_header(usb_dev *udev, uint8_t* pDst, uint8_t len)
{
    uint32_t counter;
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    for(counter = 0U; counter < len; counter++){
        *pDst++ = ccid->bulkout_data_buff[counter];
    }
}

/*!
    \brief      provides the status of previous interrupt transfer status
    \param[in]  udev: pointer to USB core instance
    \param[in]  none
    \param[out] none
    \retval     pre_xfer_compl_INT_IN: value of the previous transfer status
*/
uint8_t CCID_IS_INT_transfer_complete(usb_dev *udev)
{
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    return ccid->pre_xfer_compl_INT_IN;
}

/*!
    \brief      provides the status of previous interrupt transfer status
    \param[in]  udev: pointer to USB core instance
    \param[in]  xfer_status: value of the interrupt transfer status to set
    \param[out] none
    \retval     none
*/
void CCID_set_INT_transfer_status (usb_dev *udev, uint8_t xfer_status)
{
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    ccid->pre_xfer_compl_INT_IN = xfer_status;
}
