#ifdef GD32F30x
    #include "system_gd32f30x.c"
#elif defined(GD32F3x0)
    #include "system_gd32f3x0.c"
#elif defined(GD32F1x0)
#include "system_gd32f1x0.c"
#else 
#error "Unknown chip series!"
#endif