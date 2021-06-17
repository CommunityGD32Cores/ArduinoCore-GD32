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

#ifndef _GD32_DEF_
#define _GD32_DEF_
#include "gd32f30x.h"

/**
 * @brief GD32 core version number
 */
#define GD32_CORE_VERSION_MAJOR    (0x01U) /*!< [23:16] major version */
#define GD32_CORE_VERSION_MINOR    (0x00U) /*!< [15:8] minor version */
#define GD32_CORE_VERSION_PATCH    (0x00U) /*!< [7:0]  patch version */
#define GD32_CORE_VERSION          ((GD32_CORE_VERSION_MAJOR << 16U)\
                                    |(GD32_CORE_VERSION_MINOR << 8U)\
                                    |(GD32_CORE_VERSION_PATCH )
/**
 * Libc porting layers
 */
#if defined (  __GNUC__  ) /* GCC CS3 */
    #define WEAK __attribute__ ((weak))
#endif

#endif /*_GD32_DEF_ */
