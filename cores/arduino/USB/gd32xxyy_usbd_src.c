#include "usbd_int.h"
#include "usbd_std.h"
#include "usbd_core.h"

#ifdef GD32F30x
	#include "usbd_core.c"
	#include "usbd_int.c"
	#include "usbd_pwr.c"
	#include "usbd_std.c"
#endif
