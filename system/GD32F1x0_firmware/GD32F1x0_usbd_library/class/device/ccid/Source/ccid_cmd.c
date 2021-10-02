/*!
    \file    ccid_cmd.c
    \brief   CCID commands handler

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

#include "ccid_cmd.h"
#include "ccid_itf.h"

/*!
    \brief      PC_TO_RDR_ICCPOWERON message execution, apply voltage and get ATR
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status of the command execution 
*/
uint8_t PC_to_RDR_ICC_poweron(usb_dev *udev)
{
    uint8_t index;
    uint8_t error;
    uint8_t voltage;
    uint8_t sc_voltage = 0U;

    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    /* reset number of bytes in abData */
    ccid->CCID_bulkin_data.dwLength = 0U;  

    /* apply the ICC VCC, fills the response buffer with ICC ATR.
       this Command is returned with RDR_to_PC_DataBlock() */
    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_DWLENGTH | \
                                            CHK_PARAM_abRFU2 | \
                                            CHK_PARAM_CARD_PRESENT | \
                                            CHK_PARAM_ABORT);

    if(0U != error){
        return error;
    }

    /* voltage that is applied to the ICC
       00h: automatic voltage selection
       01h: 5.0 volts
       02h: 3.0 volts
       03h: 1.8 volts */

    /* ccid->CCID_bulkout_data.bSpecific_0 contains bPowerSelect */
    voltage = ccid->CCID_bulkout_data.bSpecific_0;

    if(voltage >= VOLTAGE_SELECTION_1V8){
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE);

        /* the voltage specified is out of spec */
        return SLOTERROR_BAD_POWERSELECT; 
    }

    /* correct voltage requested by the host */
    if((VOLTAGE_SELECTION_AUTOMATIC == voltage) || (VOLTAGE_SELECTION_3V == voltage)){
        /* voltage == 00 voltage automatic 
           voltage == 01 voltage automatic = 5.0V
           voltage == 02 voltage automatic = 3V
           voltage == 03 voltage automatic = 1.8V */
        sc_voltage = SC_VOLTAGE_3V;
    }else if(VOLTAGE_SELECTION_5V == voltage){
        sc_voltage = SC_VOLTAGE_5V;
    }

    sc_answer_to_reset(sc_voltage);

    /* check if the card has come to active state */
    error = CCID_check_command_params(udev, CHK_ACTIVE_STATE);

    if(0U != error){
        /* check if voltage is not automatic */
        if(0U != voltage){
            /* if specific voltage requested by host i.e 3V or 5V */
            return error;
        }else{
            /* automatic voltage selection requested by host */
            if(SC_VOLTAGE_5V != sc_voltage){
                /* if voltage selected was automatic and 5V is not yet tried */
                sc_voltage = SC_VOLTAGE_5V;
                sc_answer_to_reset(sc_voltage);

                /* check again the state */
                error = CCID_check_command_params(udev, CHK_ACTIVE_STATE);
                if (0U != error)
                return error;
            }else{
                /* voltage requested from host was 5V already */
                CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_INACTIVE);
                return error;
            }
        }
    }

    /* ATR is received, no error condition found */
    ccid->CCID_bulkin_data.dwLength = ATR_length;
    CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR, BM_ICC_PRESENT_ACTIVE);

    for(index = 0U; index < ATR_length; index++){
        /* copy the ATR to the response buffer */
        ccid->CCID_bulkin_data.abData[index] = sc_ATR_table[index];
    }

    return SLOT_NO_ERROR;
}

/*!
    \brief      ICC VCC is switched off 
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     error: status of the command execution
*/
uint8_t PC_to_RDR_ICC_poweroff(usb_dev *udev)
{
    /* the response to this command message is the RDR_to_PC_SlotStatus response message */
    uint8_t error;

    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_abRFU3 | \
                                            CHK_PARAM_DWLENGTH);

    if(0U != error){
        return error;
    }

    /* command is OK, check for card presence */ 
    if(sc_detect()){
        CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR,BM_ICC_PRESENT_INACTIVE);
    }else{
        CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR,BM_ICC_NO_ICC_PRESENT);
    }

    /* power off the card */
    sc_power_cmd(DISABLE);
    sc_set_state(SC_POWER_OFF);

    return SLOT_NO_ERROR;
}

/*!
    \brief      provides the slot status to the host 
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status of the command execution 
*/
uint8_t PC_to_RDR_get_slot_status(usb_dev *udev)
{
    uint8_t error;

    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_DWLENGTH | \
                                            CHK_PARAM_CARD_PRESENT | \
                                            CHK_PARAM_abRFU3);

    if(0U != error){
        return error;
    }

    CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR, BM_ICC_PRESENT_ACTIVE);

    return SLOT_NO_ERROR;
}

/*!
    \brief      handles the block transfer from host
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status of the command execution 
*/
uint8_t PC_to_RDR_xfr_block(usb_dev *udev)
{
    uint8_t error;
    uint16_t expected_len;

    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_CARD_PRESENT | \
                                            CHK_PARAM_abRFU3 | \
                                            CHK_PARAM_ABORT | \
                                            CHK_ACTIVE_STATE);

    if(0U != error){
        return error;
    }

    if (ccid->CCID_bulkout_data.dwLength > ABDATA_SIZE){
        /* check amount of data sent by host is > than memory allocated ? */
        return SLOTERROR_BAD_DWLENGTH;
    }

    /* wLevelParameter = size of expected data to be returned by the bulk-in endpoint */
    expected_len = (ccid->CCID_bulkout_data.bSpecific_2 << 8) | ccid->CCID_bulkout_data.bSpecific_1;

    ccid->CCID_bulkin_data.dwLength = (uint16_t)expected_len;

    error = sc_xfer_block(udev, &ccid->CCID_bulkout_data.abData[0], ccid->CCID_bulkout_data.dwLength, expected_len); 

    if(SLOT_NO_ERROR != error){
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE);
    }else{
        CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR, BM_ICC_PRESENT_ACTIVE);
        error = SLOT_NO_ERROR;
    }

    return error;
}

/*!
    \brief      provides the ICC parameters to the host.Response to this command 
                message is the RDR_to_PC_Parameters.
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status of the command execution 
*/
uint8_t PC_to_RDR_get_parameters(usb_dev *udev)
{
    uint8_t error;

    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_DWLENGTH | \
                                            CHK_PARAM_CARD_PRESENT | \
                                            CHK_PARAM_abRFU3);

    if(0U != error){
        return error;
    }

    CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR, BM_ICC_PRESENT_ACTIVE);

    return SLOT_NO_ERROR;
}

/*!
    \brief      set the ICC parameters to the default. Response to this command 
                message is the RDR_to_PC_Parameters.
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status of the command execution 
*/
uint8_t PC_to_RDR_reset_parameters(usb_dev *udev)
{
    uint8_t error;

    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_DWLENGTH | \
                                            CHK_PARAM_CARD_PRESENT | \
                                            CHK_PARAM_abRFU3 | \
                                            CHK_ACTIVE_STATE);
    if(0U != error){
        return error;
    }

    /* this command resets the slot parameters to their default values */
    ccid->CCID_bulkout_data.abData[0] = DEFAULT_FIDI;
    ccid->CCID_bulkout_data.abData[1] = DEFAULT_T01CONVCHECKSUM;
    ccid->CCID_bulkout_data.abData[2] = DEFAULT_EXTRA_GUARDTIME;
    ccid->CCID_bulkout_data.abData[3] = DEFAULT_WAITINGINTEGER;
    ccid->CCID_bulkout_data.abData[4] = DEFAULT_CLOCKSTOP;
    ccid->CCID_bulkout_data.abData[5] = 0x00U;
    ccid->CCID_bulkout_data.abData[6] = 0x00U;

    error = sc_set_params((protocol0_data_t*)(&(ccid->CCID_bulkout_data.abData[0])));

    if(SLOT_NO_ERROR != error){
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE);
    }else{
        CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR, BM_ICC_PRESENT_ACTIVE);

        error = SLOT_NO_ERROR;
    }

    return error;
}

/*!
    \brief      set the ICC parameters to the host defined parameters.
                response to this command message is the RDR_to_PC_Parameters.
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status of the command execution 
*/
uint8_t PC_to_RDR_set_parameters(usb_dev *udev)
{
    uint8_t error;

    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_CARD_PRESENT | \
                                            CHK_PARAM_abRFU2 | \
                                            CHK_ACTIVE_STATE);

    if(0U != error){
        return error;
    }

    error = SLOT_NO_ERROR;

    /* for protocol T=0 (bProtocolNum=0) (dwLength=00000005h) */
    if((5U == ccid->CCID_bulkout_data.dwLength) && (0U != ccid->CCID_bulkout_data.bSpecific_0)){
        error = SLOTERROR_BAD_PROTOCOLNUM;
    }

    /* for protocol T=1 (bProtocolNum=1) (dwLength=00000007h) */
    if((7U == ccid->CCID_bulkout_data.dwLength) && (1U != ccid->CCID_bulkout_data.bSpecific_0)){
        error = SLOTERROR_CMD_NOT_SUPPORTED;
    }

    /* for T0, waiting integer 0 supported */
    if(0U != ccid->CCID_bulkout_data.abData[3]){
        error = SLOTERROR_BAD_WAITINGINTEGER;
    }

    if(DEFAULT_CLOCKSTOP != ccid->CCID_bulkout_data.abData[4]){
        error = SLOTERROR_BAD_CLOCKSTOP;
    }

    if(SLOT_NO_ERROR != error){
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE);
    } 

    error = sc_set_params((protocol0_data_t*)(&(ccid->CCID_bulkout_data.abData[0])));

    if(SLOT_NO_ERROR != error){
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE);
    }else{
        CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR, BM_ICC_PRESENT_ACTIVE);
        error = SLOT_NO_ERROR;
    }

    return error;
}

/*!
    \brief      execute the escape command. This is user specific implementation
                Response to this command message is the RDR_to_PC_Escape.
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status of the command execution 
*/
uint8_t PC_to_RDR_escape(usb_dev *udev)
{
    uint8_t error;
    uint16_t size;

    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_CARD_PRESENT | \
                                            CHK_PARAM_abRFU3 | \
                                            CHK_PARAM_ABORT | \
                                            CHK_ACTIVE_STATE);

    if(0U != error){
        return error;
    }

    error = sc_execute_escape(&ccid->CCID_bulkout_data.abData[0],
                              ccid->CCID_bulkout_data.dwLength,
                              &ccid->CCID_bulkin_data.abData[0],
                              &size);

    ccid->CCID_bulkin_data.dwLength = size;

    if(SLOT_NO_ERROR != error){
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE);
    }else{
        CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR, BM_ICC_PRESENT_ACTIVE);
    }

    return error;
}

/*!
    \brief      execute the clock specific command from host.
                response to this command message is the RDR_to_PC_SlotStatus.
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status of the command execution 
*/
uint8_t PC_to_RDR_ICC_clock(usb_dev *udev)
{
    uint8_t error;
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_CARD_PRESENT | \
                                            CHK_PARAM_abRFU2 | \
                                            CHK_PARAM_DWLENGTH | \
                                            CHK_ACTIVE_STATE);

    if(0U != error){
        return error;
    }

    /* bClockCommand: 00h restarts Clock
                      01h Stops Clock in the state shown in the bClockStop
                      field of the PC_to_RDR_SetParameters command 
                      and RDR_to_PC_Parameters message. */
    if(ccid->CCID_bulkout_data.bSpecific_0 > 1U){
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE);
        return SLOTERROR_BAD_CLOCKCOMMAND;
    }

    error = sc_set_clock(ccid->CCID_bulkout_data.bSpecific_0);

    if(SLOT_NO_ERROR != error){
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE);
    }else{
        CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR, BM_ICC_PRESENT_ACTIVE);
    }

    return error;
}

/*!
    \brief      execute the abort command from host, this stops all bulk transfers from host 
                and ICC. response to this command message is the RDR_to_PC_SlotStatus.
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status of the command execution 
*/
uint8_t PC_to_RDR_abort(usb_dev *udev)
{
    uint8_t error;
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_abRFU3 | \
                                            CHK_PARAM_DWLENGTH);

    if(0U != error){
        return error;
    }

    CCID_cmd_abort(udev, ccid->CCID_bulkout_data.bSlot, ccid->CCID_bulkout_data.bSeq);
    CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR,BM_ICC_PRESENT_ACTIVE);

    return SLOT_NO_ERROR;
}

/*!
    \brief      execute the PC_TO_RDR_T0APDU command from host. response to 
                this command message is the RDR_to_PC_SlotStatus
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status of the command execution 
*/
uint8_t PC_to_RDR_T0_APDU(usb_dev *udev)
{
    uint8_t error;
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_CARD_PRESENT | \
                                            CHK_PARAM_DWLENGTH | \
                                            CHK_PARAM_ABORT);

    if(0U != error){
        return error;
    }

    if(ccid->CCID_bulkout_data.bSpecific_0 > 0x03U){
        /* bit 0 is associated with field bClassGetResponse
           bit 1 is associated with field bClassEnvelope
           other bits are RFU. */
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE); 

        return SLOTERROR_BAD_BMCHANGES;
    }

    error = sc_T0_APDU(ccid->CCID_bulkout_data.bSpecific_0, 
                       ccid->CCID_bulkout_data.bSpecific_1, 
                       ccid->CCID_bulkout_data.bSpecific_2);

    if(SLOT_NO_ERROR != error){
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE);
    }else{
        CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR, BM_ICC_PRESENT_ACTIVE);
    }

    return error;
}

/*!
    \brief      execute the PC_TO_RDR_MECHANICAL command from host. response 
                to this command message is the RDR_to_PC_SlotStatus.
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status of the command execution
*/
uint8_t PC_to_RDR_mechanical(usb_dev *udev)
{
    uint8_t error;
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_CARD_PRESENT | \
                                            CHK_PARAM_abRFU2 | \
                                            CHK_PARAM_DWLENGTH);

    if(0U != error){
        return error;
    }

    if (ccid->CCID_bulkout_data.bSpecific_0 > 0x05U){
        /* 01h: accept card
           02h: eject card
           03h: capture card
           04h: lock card
           05h: unlock card
         */
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE); 

        return SLOTERROR_BAD_BFUNCTION_MECHANICAL;
    }

    error = sc_mechanical(ccid->CCID_bulkout_data.bSpecific_0);

    if(SLOT_NO_ERROR != error){
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE);
    }else{
        CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR, BM_ICC_PRESENT_ACTIVE);
    }

    return error;
}

/*!
    \brief      set the required card frequency and data rate from the host. response 
                to this command message is the RDR_to_PC_DataRateAndClockFrequency.
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status of the command execution
*/
uint8_t PC_to_RDR_Set_DataRate_and_ClockFreq(usb_dev *udev)
{
    uint8_t error;
    uint32_t clockFrequency;
    uint32_t dataRate;
    uint32_t temp = 0U;
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_CARD_PRESENT | \
                                            CHK_PARAM_abRFU3);

    if(0U != error){
        return error;
    }

    if(0x08U != ccid->CCID_bulkout_data.dwLength){
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED,BM_ICC_PRESENT_ACTIVE);

        return SLOTERROR_BAD_LENTGH;
    }

    /* here we avoiding to an unaligned memory access*/ 
    temp = (ccid->CCID_bulkout_data.abData[0]) & 0x000000FFU;
    clockFrequency = temp;

    temp = (ccid->CCID_bulkout_data.abData[1]) & 0x000000FFU;
    clockFrequency |= temp << 8U;

    temp = (ccid->CCID_bulkout_data.abData[2]) & 0x000000FFU;
    clockFrequency |= temp << 16U;

    temp = (ccid->CCID_bulkout_data.abData[3]) & 0x000000FFU;
    clockFrequency |= temp << 24U;

    temp = (ccid->CCID_bulkout_data.abData[4]) & 0x000000FFU;
    dataRate = temp;

    temp = (ccid->CCID_bulkout_data.abData[5]) & 0x000000FFU;
    dataRate |= temp << 8U;

    temp = (ccid->CCID_bulkout_data.abData[6]) & 0x000000FFU;
    dataRate |= temp << 16U;

    temp = (ccid->CCID_bulkout_data.abData[7]) & 0x000000FFU;
    dataRate |= temp << 24U;

    error = sc_set_DataRate_and_ClockFreq(clockFrequency, dataRate);
    ccid->CCID_bulkin_data.bError = error;

    if(SLOT_NO_ERROR != error){
        ccid->CCID_bulkin_data.dwLength = 0U;
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE);
    }else{
        ccid->CCID_bulkin_data.dwLength = 8U;

        (ccid->CCID_bulkin_data.abData[0]) = clockFrequency & 0x000000FFU ;

        (ccid->CCID_bulkin_data.abData[1]) = (clockFrequency & 0x0000FF00U) >> 8U;

        (ccid->CCID_bulkin_data.abData[2]) = (clockFrequency & 0x00FF0000U) >> 16U;

        (ccid->CCID_bulkin_data.abData[3]) = (clockFrequency & 0xFF000000U) >> 24U;

        (ccid->CCID_bulkin_data.abData[4]) = dataRate & 0x000000FFU ;

        (ccid->CCID_bulkin_data.abData[5]) = (dataRate & 0x0000FF00U) >> 8U;

        (ccid->CCID_bulkin_data.abData[6]) = (dataRate & 0x00FF0000U) >> 16U;

        (ccid->CCID_bulkin_data.abData[7]) = (dataRate & 0xFF000000U) >> 24U;

        CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR, BM_ICC_PRESENT_ACTIVE);
    }

    return error;
}

/*!
    \brief      execute the secure command from the host. response to 
                this command message is the RDR_to_PC_DataBlock.
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status of the command execution
*/
uint8_t PC_TO_RDR_secure(usb_dev *udev)
{
    uint8_t error;
    uint8_t bBWI;
    uint16_t wLevelParameter;
    uint32_t responseLen;

    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    ccid->CCID_bulkin_data.dwLength = 0;

    error = CCID_check_command_params(udev, CHK_PARAM_SLOT | \
                                            CHK_PARAM_CARD_PRESENT | \
                                            CHK_PARAM_ABORT);

    if(0U != error){
        return error;
    }

    bBWI = ccid->CCID_bulkout_data.bSpecific_0;
    wLevelParameter = (ccid->CCID_bulkout_data.bSpecific_1 + ((uint16_t)ccid->CCID_bulkout_data.bSpecific_2 << 8));

    if((EXCHANGE_LEVEL_FEATURE == TPDU_EXCHANGE) || 
       (EXCHANGE_LEVEL_FEATURE == SHORT_APDU_EXCHANGE)){
        /* TPDU level & short APDU level, wLevelParameter is RFU, = 0000h */
        if(0U != wLevelParameter){
            CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE);
            error = SLOTERROR_BAD_LEVELPARAMETER;
            return error;
        }
    }

    error = sc_secure(ccid->CCID_bulkout_data.dwLength, bBWI, wLevelParameter, 
                      &ccid->CCID_bulkout_data.abData[0], &responseLen);

    ccid->CCID_bulkin_data.dwLength = responseLen;

    if(SLOT_NO_ERROR != error){
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_PRESENT_ACTIVE);
    }else{
        CCID_update_command_status(BM_COMMAND_STATUS_NO_ERROR, BM_ICC_PRESENT_ACTIVE);
    }

    return error;
}

/*!
    \brief      execute the abort command from bulk ep or from control ep.
                this stops all bulk transfers from host and ICC
    \param[in]  udev: pointer to USB device instance
    \param[in]  slot: slot number that host wants to abort
    \param[in]  seq: seq number for PC_to_RDR_Abort
    \param[out] none
    \retval     status of the command execution 
*/
uint8_t CCID_cmd_abort(usb_dev *udev, uint8_t slot, uint8_t seq)
{
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    if(slot >= CCID_NUMBER_OF_SLOTS){
        /* this error condition is possible only from CLASS_REQUEST, otherwise
           slot is already checked in parameters from PC_to_RDR_Abort request */

        /* Slot requested is more than supported by firmware */
        CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_NO_ICC_PRESENT);

        return SLOTERROR_BAD_SLOT;
    }

    if(1U == ccid->usb_ccid_param.bAbortRequestFlag){
        /* abort command was already received from class-req or PC_to_RDR */
        if((ccid->usb_ccid_param.bSeq == seq) && (ccid->usb_ccid_param.bSlot == slot)){
            /* CLASS specific request is already received, reset the abort flag */
            ccid->usb_ccid_param.bAbortRequestFlag = 0U;
        }
    }else{
        /* abort command was not received from class-req or PC_to_RDR, 
           so save them for next abort command to verify */
        ccid->usb_ccid_param.bAbortRequestFlag = 1U;
        ccid->usb_ccid_param.bSeq = seq;
        ccid->usb_ccid_param.bSlot = slot;
    }

    return 0U;
}

/*!
    \brief      provide the data block response to the host. response 
                for PC_to_RDR_IccPowerOn, PC_to_RDR_XfrBlock.
    \param[in]  udev: pointer to USB device instance
    \param[in]  errorCode: code to be returned to the host
    \param[out] none
    \retval     none
*/
void RDR_to_PC_data_block(usb_dev *udev, uint8_t errorCode)
{
    uint16_t length = CCID_RESPONSE_HEADER_SIZE;
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    ccid->CCID_bulkin_data.bMessageType = RDR_TO_PC_DATABLOCK; 
    ccid->CCID_bulkin_data.bError = errorCode;

    /* bChainParameter */
    ccid->CCID_bulkin_data.bSpecific = 0U;

    if(SLOT_NO_ERROR == errorCode){
        /* length specified in command */
        length += ccid->CCID_bulkin_data.dwLength;
    }

    transfer_data_request(udev, (uint8_t*) (&ccid->CCID_bulkin_data), length);
}

/*!
    \brief      provide the slot status response to the host. response for 
                - PC_to_RDR_ICC_poweroff
                - PC_to_RDR_GetSlotStatus
                - PC_to_RDR_IccClock
                - PC_to_RDR_T0APDU
                - PC_to_RDR_Mechanical
                Also the device sends this response message when it has completed 
                aborting a slot after receiving both the Class Specific ABORT request 
                and PC_to_RDR_Abort command message.
    \param[in]  udev: pointer to USB device instance
    \param[in]  errorCode: code to be returned to the host
    \param[out] none
    \retval     none
*/
void RDR_to_PC_slot_status(usb_dev *udev, uint8_t errorCode)
{
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    ccid->CCID_bulkin_data.bMessageType = RDR_TO_PC_SLOTSTATUS; 
    ccid->CCID_bulkin_data.dwLength = 0U;
    ccid->CCID_bulkin_data.bError = errorCode; 
    /* bClockStatus:
       - 00h clock running
       - 01h clock stopped in state L
       - 02h clock stopped in state H
       - 03h clock stopped in an unknown state
       - all other values are RFU. */
    ccid->CCID_bulkin_data.bSpecific = 0U;

    transfer_data_request(udev, (uint8_t*)(&ccid->CCID_bulkin_data), LEN_RDR_TO_PC_SLOTSTATUS);
}

/*!
    \brief      provide the data block response to the host. response for 
                - PC_to_RDR_GetParameters
                - PC_to_RDR_ResetParameters
                - PC_to_RDR_SetParameters
    \param[in]  udev: pointer to USB device instance
    \param[in]  errorCode: code to be returned to the host
    \param[out] none
    \retval     none
*/
void RDR_to_PC_parameters(usb_dev *udev, uint8_t errorCode)
{
    uint16_t length = CCID_RESPONSE_HEADER_SIZE;
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    ccid->CCID_bulkin_data.bMessageType = RDR_TO_PC_PARAMETERS; 
    ccid->CCID_bulkin_data.bError = errorCode; 

    if(SLOT_NO_ERROR == errorCode){ 
        ccid->CCID_bulkin_data.dwLength = LEN_PROTOCOL_STRUCT_T0;
        length += LEN_PROTOCOL_STRUCT_T0;
    }else{
        ccid->CCID_bulkin_data.dwLength = 0U;
    }

    ccid->CCID_bulkin_data.abData[0] = protocol0_data.bmFindexDindex;
    ccid->CCID_bulkin_data.abData[1] = protocol0_data.bmTCCKST0;
    ccid->CCID_bulkin_data.abData[2] = protocol0_data.bGuardTimeT0;
    ccid->CCID_bulkin_data.abData[3] = protocol0_data.bWaitingIntegerT0;
    ccid->CCID_bulkin_data.abData[4] = protocol0_data.bClockStop;

    /* bProtocolNum */
    ccid->CCID_bulkin_data.bSpecific = BPROTOCOL_NUM_T0;

    transfer_data_request(udev, (uint8_t*)(&ccid->CCID_bulkin_data), length);
}

/*!
    \brief      provide the escaped data block response to the host.
                response for PC_to_RDR_Escape.
    \param[in]  udev: pointer to USB device instance
    \param[in]  errorCode: code to be returned to the host
    \param[out] none
    \retval     none
*/
void RDR_to_PC_escape(usb_dev *udev, uint8_t errorCode)
{
    uint16_t length = CCID_RESPONSE_HEADER_SIZE;
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    ccid->CCID_bulkin_data.bMessageType = RDR_TO_PC_ESCAPE; 

    /* reserved for future use */
    ccid->CCID_bulkin_data.bSpecific = 0U;
    ccid->CCID_bulkin_data.bError = errorCode;

    if(SLOT_NO_ERROR == errorCode){
        /* length specified in command */
        length += ccid->CCID_bulkin_data.dwLength;
    }

    transfer_data_request(udev, (uint8_t*)(&ccid->CCID_bulkin_data), length);
}

/*!
    \brief      provide the clock and data rate information to host.
                response for PC_TO_RDR_SetDataRateAndClockFrequency.
    \param[in]  udev: pointer to USB device instance
    \param[in]  errorCode: code to be returned to the host
    \param[out] none
    \retval     none
*/
void RDR_to_PC_DataRate_and_ClockFreq(usb_dev *udev, uint8_t errorCode)
{
    uint16_t length = CCID_RESPONSE_HEADER_SIZE;
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    ccid->CCID_bulkin_data.bMessageType = RDR_TO_PC_DATARATEANDCLOCKFREQUENCY; 
    ccid->CCID_bulkin_data.bError = errorCode;

    /* reserved for future use */
    ccid->CCID_bulkin_data.bSpecific = 0U;

    if(SLOT_NO_ERROR == errorCode){
        /* length specified in command */
        length += ccid->CCID_bulkin_data.dwLength;
    }

    transfer_data_request(udev, (uint8_t*)(&ccid->CCID_bulkin_data), length);
}

/*!
    \brief      interrupt message to be sent to the host, checks the card presence 
                status and update the buffer accordingly.
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
void RDR_to_PC_notify_slot_change(usb_dev *udev)
{
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    ccid->interrupt_message_buff[OFFSET_INT_BMESSAGETYPE] = RDR_TO_PC_NOTIFYSLOTCHANGE;

    if(sc_detect()){
        /* SLOT_ICC_PRESENT 0x01: LSb(0b = no ICC present, 1b = ICC present)
           SLOT_ICC_CHANGE  0x02: MSb(0b = no change, 1b = change) */
        ccid->interrupt_message_buff[OFFSET_INT_BMSLOTICCSTATE] = SLOT_ICC_PRESENT | SLOT_ICC_CHANGE;
    }else{
        ccid->interrupt_message_buff[OFFSET_INT_BMSLOTICCSTATE] = SLOT_ICC_CHANGE;

        /* power off the card */
        sc_power_cmd(DISABLE);
        sc_set_state(SC_POWER_OFF);
    }
}

/*!
    \brief      updates the variable for the slot status 
    \param[in]  udev: pointer to USB device instance
    \param[in]  slot_status : slot status from the calling function 
    \param[out] none
    \retval     none
*/
void CCID_update_slot_status(usb_dev *udev, uint8_t slot_status)
{
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    ccid->CCID_slot_status.SlotStatus = slot_status;
}

/*!
    \brief      updates the variable for the slot change status 
    \param[in]  udev: pointer to USB device instance
    \param[in]  change_status: slot change status from the calling function 
    \param[out] none
    \retval     none
*/
void CCID_update_slot_change(usb_dev *udev, uint8_t change_status)
{
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    ccid->CCID_slot_status.SlotStatusChange = change_status;
}

/*!
    \brief      provides the value of the variable for the slot change status
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     slot change status
*/
uint8_t CCID_IS_slot_status_change(usb_dev *udev)
{
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    return ccid->CCID_slot_status.SlotStatusChange;
}

/*!
    \brief      checks the specific parameters requested by the function and update 
                status accordingly. this function is called from all.
    \param[in]  udev: pointer to USB device instance
    \param[in]  param_type : parameter enum to be checked by calling function
    \param[out] none
    \retval     status
*/
uint8_t CCID_check_command_params(usb_dev *udev, uint32_t param_type)
{
    uint32_t parameter = param_type;
    usb_ccid_handler *ccid = (usb_ccid_handler *)udev->class_data[USBD_CCID_INTERFACE];

    ccid->CCID_bulkin_data.bStatus = BM_ICC_PRESENT_ACTIVE | BM_COMMAND_STATUS_NO_ERROR;

    if(parameter & CHK_PARAM_SLOT){
        /* The slot number (bSlot) identifies which ICC slot is being addressed 
        by the message, if the CCID supports multiple slots. The slot number is 
        zero-relative, and is in the range of zero to FFh. */

        /* SLOT Number is 0 onwards, so always < CCID_NUMBER_OF_SLOTs */
        /* error condition !!! */
        if (ccid->CCID_bulkout_data.bSlot >= CCID_NUMBER_OF_SLOTS){
            /* slot requested is more than supported by firmware */
            CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_NO_ICC_PRESENT);
            return SLOTERROR_BAD_SLOT;
        }
    }

    if(parameter & CHK_PARAM_CARD_PRESENT){
        /* commands parameters OK, check the card status */  
        if(0U == sc_detect()){
            /* card is not detected */
            CCID_update_command_status(BM_COMMAND_STATUS_FAILED, BM_ICC_NO_ICC_PRESENT);

            return SLOTERROR_ICC_MUTE;
        }
    }

    /* check that dwlength is 0 */
    if(parameter & CHK_PARAM_DWLENGTH){
        if(0U != ccid->CCID_bulkout_data.dwLength){
            CCID_update_command_status(BM_COMMAND_STATUS_FAILED,BM_ICC_PRESENT_ACTIVE);

            return SLOTERROR_BAD_LENTGH;
        }
    }

    /* abRFU 2 : reserved for future use */
    if(parameter & CHK_PARAM_abRFU2){
        if ((0U != ccid->CCID_bulkout_data.bSpecific_1) ||
            (0U != ccid->CCID_bulkout_data.bSpecific_2)){
            CCID_update_command_status(BM_COMMAND_STATUS_FAILED,BM_ICC_PRESENT_ACTIVE);

            /* bSpecific_1 */
            return SLOTERROR_BAD_ABRFU_2B;
        }
    }

    if(parameter & CHK_PARAM_abRFU3){
        /* abRFU 3 : reserved for future use */
        if((0U != ccid->CCID_bulkout_data.bSpecific_0) || 
           (0U != ccid->CCID_bulkout_data.bSpecific_1) ||
           (0U != ccid->CCID_bulkout_data.bSpecific_2)){
            CCID_update_command_status(BM_COMMAND_STATUS_FAILED,BM_ICC_PRESENT_ACTIVE);

            return SLOTERROR_BAD_ABRFU_3B;
        }
    }

    if(parameter & CHK_PARAM_ABORT){
        if(ccid->usb_ccid_param.bAbortRequestFlag){
            CCID_update_command_status(BM_COMMAND_STATUS_FAILED,BM_ICC_PRESENT_INACTIVE);

            return SLOTERROR_CMD_ABORTED;
        }
    }

    if(parameter & CHK_ACTIVE_STATE){
        /* commands parameters OK, check the card status */  
        /* card is detected */
#if defined(SC_PROTOCOL_T0)
        if(SC_ACTIVE_ON_T0 != sc_get_state())
#elif defined(SC_PROTOCOL_T1)
        if(SC_ACTIVE_ON_T1 != sc_get_state())
#endif
        {
            /* check that from lower layers, the smart card come to known state */
            CCID_update_command_status(BM_COMMAND_STATUS_FAILED,BM_ICC_PRESENT_INACTIVE);

            return SLOTERROR_HW_ERROR;
        }
    }

    return 0;
}
