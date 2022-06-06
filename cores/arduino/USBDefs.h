#ifndef __USBDEFS_H
#define __USBDEFS_H

#include <stdint.h>

#define USB_EP_SIZE USBD_EP0_MAX_SIZE

#define USB_ENDPOINT_DIRECTION_MASK 0x80
#define USB_ENDPOINT_OUT(addr)      (lowByte((addr) | 0x00))
#define USB_ENDPOINT_IN(addr)       (lowByte((addr) | 0x80))

#define USB_ENDPOINT_TYPE_MASK        0x03
#define USB_ENDPOINT_TYPE_CONTROL     0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS 0x01
#define USB_ENDPOINT_TYPE_BULK        0x02
#define USB_ENDPOINT_TYPE_INTERRUPT   0x03

// bmRequestType
#define REQUEST_HOSTTODEVICE 0x00
#define REQUEST_DEVICETOHOST 0x80
#define REQUEST_DIRECTION    0x80

#define REQUEST_STANDARD     0x00
#define REQUEST_CLASS        0x20
#define REQUEST_VENDOR       0x40
#define REQUEST_TYPE         0x60

#define REQUEST_DEVICE       0x00
#define REQUEST_INTERFACE    0x01
#define REQUEST_ENDPOINT     0x02
#define REQUEST_OTHER        0x03
#define REQUEST_RECIPIENT    0x03

#define REQUEST_DEVICETOHOST_CLASS_INTERFACE    (REQUEST_DEVICETOHOST | REQUEST_CLASS | REQUEST_INTERFACE)
#define REQUEST_HOSTTODEVICE_CLASS_INTERFACE    (REQUEST_HOSTTODEVICE | REQUEST_CLASS | REQUEST_INTERFACE)
#define REQUEST_DEVICETOHOST_STANDARD_INTERFACE (REQUEST_DEVICETOHOST | REQUEST_STANDARD | REQUEST_INTERFACE)

#define D_IAD(_firstInterface, _count, _class, _subClass, _protocol)    \
	{ 8, 11, _firstInterface, _count, _class, _subClass, _protocol, 0 }

#define D_INTERFACE(_n,_numEndpoints,_class,_subClass,_protocol)        \
	{ 9, 4, _n, 0, _numEndpoints, _class,_subClass, _protocol, 0 }

#define D_ENDPOINT(_addr,_attr,_packetSize, _interval) \
	{ 7, 5, _addr,_attr,_packetSize, _interval }

#pragma pack(push, 1)
typedef struct {
    uint8_t len;
    uint8_t dtype;
    uint8_t number;
    uint8_t alternate;
    uint8_t numEndpoints;
    uint8_t interfaceClass;
    uint8_t interfaceSubClass;
    uint8_t protocol;
    uint8_t iInterface;
} InterfaceDescriptor;

typedef struct {
    uint8_t len;
    uint8_t dtype;
    uint8_t addr;
    uint8_t attr;
    uint16_t packetSize;
    uint8_t interval;
} EndpointDescriptor;

// Interface Association Descriptor
// Used to bind 2 interfaces together in CDC compostite device
typedef struct {
    uint8_t len;
    uint8_t dtype;
    uint8_t firstInterface;
    uint8_t interfaceCount;
    uint8_t functionClass;
    uint8_t funtionSubClass;
    uint8_t functionProtocol;
    uint8_t iInterface;
} IADDescriptor;
#pragma pack(pop)

#endif
