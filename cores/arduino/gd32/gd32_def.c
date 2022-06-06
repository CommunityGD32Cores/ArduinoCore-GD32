#include "gd32_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
WEAK void _Error_Handler(const char *msg, int val)
{
    (void) msg;
    (void) val;
    /* User can add his own implementation to report the HAL error return state */
    while (1) {
    }
}

#ifdef __cplusplus
}
#endif
