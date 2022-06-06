#ifndef _GD_USB_H
#define _GD_USB_H

#include "usbd_core.h"
#include "usbd_lld_core.h"

extern usb_dev usbd;

void usb_init(usb_desc*, usb_class*);
void usb_connect();
void usb_disconnect();

#endif
