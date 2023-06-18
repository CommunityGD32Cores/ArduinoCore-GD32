/*
 * @file    portasm.h
 * @author  Frederic Pillon <frederic.pillon@st.com> for STMicroelectronics.
 * @brief   Include header of FreeRTOS portable layer file to match Arduino library format
 */

#ifndef _PORTASM_H_
#define _PORTASM_H_
#include "gd32_def.h"

#ifndef __CORTEX_M
#pragma GCC error  "no \"__CORTEX_M\" definition"
#endif

#if (__CORTEX_M == 33U)
/* TZ = 0 */
#include "../portable/ARM_CM33_NTZ/non_secure/portasm.h"
#endif

#if (__CORTEX_M == 23U)
/* TZ = 0 */
#include "../portable/ARM_CM23_NTZ/non_secure/portasm.h"
#endif

#endif //_PORTASM_H_