/*
 * @file    FreeRTOSConfig.h
 * @author  Frederic Pillon <frederic.pillon@st.com> for STMicroelectronics.
 * @brief   Include header file to match Arduino library format
 */

#ifndef _ARDUINO_FREERTOS_CONFIG_H
#define _ARDUINO_FREERTOS_CONFIG_H

/* completely controlled config file*/
#if __has_include("GD32FreeRTOSConfig.h")
    #include "GD32FreeRTOSConfig.h"
#else
    /* extra options plus default file */
    #if __has_include("GD32FreeRTOSConfig_extra.h")
        #include "GD32FreeRTOSConfig_extra.h"
    #endif
    #include "../config/FreeRTOSConfig_Default.h"
#endif

#endif /* _ARDUINO_FREERTOS_CONFIG_H */
