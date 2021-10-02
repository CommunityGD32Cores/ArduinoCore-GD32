#ifdef GD32F30x
    #include "system_gd32f30x.c"
#elif defined(GD32F3x0)
    #include "system_gd32f3x0.c"
#else "Unknown chip series!"
#endif