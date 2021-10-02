#if defined(GD32F10x)
#include "gd32f10x.h"
#elif defined(GD32F1x0)
#include "gd32f1x0.h"
#elif defined (GD32F20x)
#include "gd32f20x.h"
#elif defined(GD32F3x0)
#include "gd32f3x0.h"
#elif defined(GD32F30x)
#include "gd32f30x.h"
#elif defined(GD32F4xx)
#include "gd32f4xx.h"
#elif defined(GD32F403)
#include "gd32f403.h"
#elif defined(GD32E10X)
#include "gd32e10x.h"
#elif defined(GD32E23x)
#include "gd32e23x.h"
#elif defined(GD32E50X)
#include "gd32e50x.h"
#else
#error "Unknown chip series"
#endif