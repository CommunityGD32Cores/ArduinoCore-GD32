/*!
    \file    ccid_cmd.h
    \brief   the header of CCID commands handler

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

#ifndef __USBD_CCID_CMD_H
#define __USBD_CCID_CMD_H

#include "ccid_core.h"

/* error codes for usb bulk in: bError */
#define SLOT_NO_ERROR                            0x81U
#define SLOTERROR_UNKNOWN                        0x82U

#define CCID_update_command_status(cmd_status, icc_status) ccid->CCID_bulkin_data.bStatus = ((cmd_status) | (icc_status))

/* Index of not supported / incorrect message parameter : 7Fh to 01h */
/* These Values are used for Return Types between Firmware Layers */
/* Failure of a command. The CCID cannot parse one parameter or the ICC is 
   not supporting one parameter. Then the Slot Error register contains the 
   index of the first bad parameter as a positive number (1-127). For instance,
   if the CCID receives an ICC command to an unimplemented slot, then the Slot 
   Error register shall be set to (index of bSlot field). */
#define SLOTERROR_BAD_LENTGH                    0x01U
#define SLOTERROR_BAD_SLOT                      0x05U
#define SLOTERROR_BAD_POWERSELECT               0x07U
#define SLOTERROR_BAD_PROTOCOLNUM               0x07U
#define SLOTERROR_BAD_CLOCKCOMMAND              0x07U
#define SLOTERROR_BAD_ABRFU_3B                  0x07U
#define SLOTERROR_BAD_BMCHANGES                 0x07U
#define SLOTERROR_BAD_BFUNCTION_MECHANICAL      0x07U
#define SLOTERROR_BAD_ABRFU_2B                  0x08U
#define SLOTERROR_BAD_LEVELPARAMETER            0x08U
#define SLOTERROR_BAD_FIDI                      0x0AU
#define SLOTERROR_BAD_T01CONVCHECKSUM           0x0BU
#define SLOTERROR_BAD_GUARDTIME                 0x0CU
#define SLOTERROR_BAD_WAITINGINTEGER            0x0DU
#define SLOTERROR_BAD_CLOCKSTOP                 0x0EU
#define SLOTERROR_BAD_IFSC                      0x0FU
#define SLOTERROR_BAD_NAD                       0x10U
#define SLOTERROR_BAD_DWLENGTH                  0x08U 

/* Table 6.2-2 Slot error register when bmCommandStatus = 1 */
#define SLOTERROR_CMD_ABORTED                   0xFFU
#define SLOTERROR_ICC_MUTE                      0xFEU
#define SLOTERROR_XFR_PARITY_ERROR              0xFDU
#define SLOTERROR_XFR_OVERRUN                   0xFCU
#define SLOTERROR_HW_ERROR                      0xFBU
#define SLOTERROR_BAD_ATR_TS                    0xF8U
#define SLOTERROR_BAD_ATR_TCK                   0xF7U
#define SLOTERROR_ICC_PROTOCOL_NOT_SUPPORTED    0xF6U
#define SLOTERROR_ICC_CLASS_NOT_SUPPORTED       0xF5U
#define SLOTERROR_PROCEDURE_BYTE_CONFLICT       0xF4U
#define SLOTERROR_DEACTIVATED_PROTOCOL          0xF3U
#define SLOTERROR_BUSY_WITH_AUTO_SEQUENCE       0xF2U
#define SLOTERROR_PIN_TIMEOUT                   0xF0U
#define SLOTERROR_PIN_CANCELLED                 0xEFU
#define SLOTERROR_CMD_SLOT_BUSY                 0xE0U
#define SLOTERROR_CMD_NOT_SUPPORTED             0x00U

#define DEFAULT_FIDI                            0x11U 
#define DEFAULT_T01CONVCHECKSUM                 0x00U
#define DEFAULT_EXTRA_GUARDTIME                 0x00U
#define DEFAULT_WAITINGINTEGER                  0x0AU
#define DEFAULT_CLOCKSTOP                       0x00U
#define DEFAULT_IFSC                            0x20U
#define DEFAULT_NAD                             0x00U
#define DEFAULT_DATA_RATE                       0x000025CDU
#define DEFAULT_CLOCK_FREQ                      0x00000E10U

/* Offset=0 bmICCStatus 2 bit  0, 1, 2
    0 - An ICC is present and active (power is on and stable, RST is inactive)
    1 - An ICC is present and inactive (not activated or shut down by hardware error)
    2 - No ICC is present
    3 - RFU
Offset=0 bmRFU 4 bits 0 RFU
Offset=6 bmCommandStatus 2 bits 0, 1, 2
    0 - Processed without error
    1 - Failed (error code provided by the error register)
    2 - Time Extension is requested
    3 - RFU */
#define BM_ICC_PRESENT_ACTIVE                   0x00U
#define BM_ICC_PRESENT_INACTIVE                 0x01U
#define BM_ICC_NO_ICC_PRESENT                   0x02U

#define BM_COMMAND_STATUS_OFFSET                0x06U
#define BM_COMMAND_STATUS_NO_ERROR              0x00U
#define BM_COMMAND_STATUS_FAILED                (0x01U << BM_COMMAND_STATUS_OFFSET)
#define BM_COMMAND_STATUS_TIME_EXTN             (0x02U << BM_COMMAND_STATUS_OFFSET)

/* defines for the CCID_CMD Layers */
#define LEN_RDR_TO_PC_SLOTSTATUS                10U
#define LEN_PROTOCOL_STRUCT_T0                  5U

#define BPROTOCOL_NUM_T0                        0U
#define BPROTOCOL_NUM_T1                        1U

/* error codes for RDR_TO_PC_HARDWAREERROR message: bHardwareErrorCode */
#define HARDWAREERRORCODE_OVERCURRENT           0x01U
#define HARDWAREERRORCODE_VOLTAGEERROR          0x02U
#define HARDWAREERRORCODE_OVERCURRENT_IT        0x04U
#define HARDWAREERRORCODE_VOLTAGEERROR_IT       0x08U

typedef enum 
{
    CHK_PARAM_SLOT = 1U,
    CHK_PARAM_DWLENGTH = (1U<<1U),
    CHK_PARAM_abRFU2 = (1U<<2U),
    CHK_PARAM_abRFU3 = (1U<<3U),
    CHK_PARAM_CARD_PRESENT = (1U<<4U),
    CHK_PARAM_ABORT = (1U<<5U),
    CHK_ACTIVE_STATE = (1U<<6U)
}ChkParam_t;

/* function declarations */
/* PC_TO_RDR_ICCPOWERON message execution, apply voltage and get ATR */
uint8_t PC_to_RDR_ICC_poweron(usb_dev *udev);
/* ICC VCC is switched off */
uint8_t PC_to_RDR_ICC_poweroff(usb_dev *udev);
/* provides the slot status to the host */
uint8_t PC_to_RDR_get_slot_status(usb_dev *udev);
/* handles the block transfer from host */
uint8_t PC_to_RDR_xfr_block(usb_dev *udev);
/* provides the ICC parameters to the host */
uint8_t PC_to_RDR_get_parameters(usb_dev *udev);
/* set the ICC parameters to the default */
uint8_t PC_to_RDR_reset_parameters(usb_dev *udev);
/* set the ICC parameters to the host defined parameters */
uint8_t PC_to_RDR_set_parameters(usb_dev *udev);
/* execute the escape command */
uint8_t PC_to_RDR_escape(usb_dev *udev);
/* execute the clock specific command from host */
uint8_t PC_to_RDR_ICC_clock(usb_dev *udev);
/* execute the abort command from host */
uint8_t PC_to_RDR_abort(usb_dev *udev);
/* execute the PC_TO_RDR_T0APDU command from host */
uint8_t PC_to_RDR_T0_APDU(usb_dev *udev);
/* execute the PC_TO_RDR_MECHANICAL command from host */
uint8_t PC_to_RDR_mechanical(usb_dev *udev);
/* set the required card frequency and data rate from the host */
uint8_t PC_to_RDR_Set_DataRate_and_ClockFreq(usb_dev *udev);
/* execute the secure command from the host */
uint8_t PC_TO_RDR_secure(usb_dev *udev);
/* execute the abort command from bulk endpoint or from control endpoint */
uint8_t CCID_cmd_abort(usb_dev *udev, uint8_t slot, uint8_t seq);
/* provide the data block response to the host */
void RDR_to_PC_data_block(usb_dev *udev, uint8_t errorCode);
/* provide the slot status response to the host */
void RDR_to_PC_slot_status(usb_dev *udev, uint8_t errorCode);
/* provide the data block response to the host */
void RDR_to_PC_parameters(usb_dev *udev, uint8_t errorCode);
/* provide the escaped data block response to the host */
void RDR_to_PC_escape(usb_dev *udev, uint8_t errorCode);
/* provide the clock and data rate information to host */
void RDR_to_PC_DataRate_and_ClockFreq(usb_dev *udev, uint8_t errorCode);
/* interrupt message to be sent to the host, checks the card presence status and update the buffer accordingly */
void RDR_to_PC_notify_slot_change(usb_dev *udev);
/* updates the variable for the slot status */
void CCID_update_slot_status (usb_dev *udev, uint8_t slot_status);
/* updates the variable for the slot change status */
void CCID_update_slot_change (usb_dev *udev, uint8_t change_status);
/* provides the value of the variable for the slot change status */
uint8_t CCID_IS_slot_status_change (usb_dev *udev);
/* checks the specific parameters requested by the function and update status accordingly */
uint8_t CCID_check_command_params(usb_dev *udev, uint32_t param_type);

#endif /* __USBD_CCID_CMD_H */
