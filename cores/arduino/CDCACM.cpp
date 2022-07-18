#ifdef USBD_USE_CDC
#include "CDCACM.h"

#include "USBCore.h"

const uint8_t ACM_EP_MAXLEN = 0x10;

static uint8_t IN_ENDPOINT = 0;

static uint8_t cdc_flush_sof(usb_dev *usbd) {
    USB_Flush(IN_ENDPOINT);
    return 0;
}

usbd_int_cb_struct usb_inthandler = {
    cdc_flush_sof,
};

CDCACM_::CDCACM_(uint8_t firstInterface, uint8_t firstEndpoint)
{
    this->acmInterface = firstInterface;
    this->dataInterface = firstInterface + 1;
    this->acmEndpoint = firstEndpoint;
    this->outEndpoint = firstEndpoint + 1;
    this->inEndpoint = firstEndpoint + 2;
    IN_ENDPOINT = this->inEndpoint;

    *(EPDesc*)epBuffer(this->acmEndpoint) = EPDesc(USB_TRX_IN, USB_ENDPOINT_TYPE_INTERRUPT, ACM_EP_MAXLEN);
    *(EPDesc*)epBuffer(this->outEndpoint) = EPDesc(USB_TRX_OUT, USB_ENDPOINT_TYPE_BULK);
    *(EPDesc*)epBuffer(this->inEndpoint) = EPDesc(USB_TRX_IN, USB_ENDPOINT_TYPE_BULK);
}

int CDCACM_::getInterface()
{
    static CDCDescriptor desc = {
        D_IAD(this->acmInterface, 2, CDC_COMMUNICATION_INTERFACE_CLASS, CDC_ABSTRACT_CONTROL_MODEL, 0),

        // CDC communication interface
        D_INTERFACE(this->acmInterface, 1, CDC_COMMUNICATION_INTERFACE_CLASS, CDC_ABSTRACT_CONTROL_MODEL, 0),
        // Header (1.10 bcd)
        D_CDCCS(CDC_HEADER, 0x10, 0x01),
        // Device handles call management (not)
        D_CDCCS(CDC_CALL_MANAGEMENT, 1, 1),

        // SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE supported
        D_CDCCS4(CDC_ABSTRACT_CONTROL_MANAGEMENT, 6),

        // Communication interface is master, data interface is slave 0
        D_CDCCS(CDC_UNION, this->acmInterface, this->dataInterface),
        D_ENDPOINT(USB_ENDPOINT_IN(acmEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, ACM_EP_MAXLEN, 0x40),

        // CDC data interface
        D_INTERFACE(this->dataInterface, 2, CDC_DATA_INTERFACE_CLASS, 0, 0),
        D_ENDPOINT(USB_ENDPOINT_OUT(outEndpoint), USB_ENDPOINT_TYPE_BULK, USB_EP_SIZE, 0),
        D_ENDPOINT(USB_ENDPOINT_IN(inEndpoint), USB_ENDPOINT_TYPE_BULK, USB_EP_SIZE, 0)
    };

    return USBCore().sendControl(0, &desc, sizeof(desc));
}

bool CDCACM_::setup(arduino::USBSetup& setup)
{
    if (setup.wIndex != this->acmInterface) {
        return false;
    }

    if (setup.bmRequestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE) {
        if (setup.bRequest == CDC_GET_LINE_CODING) {
            USB_SendControl(TRANSFER_RELEASE, (const void*)&this->lc.lineCoding, sizeof(this->lc.lineCoding));
            return true;
        }
    } else if (setup.bmRequestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE) {
        if (setup.bRequest == CDC_SEND_BREAK) {
            this->breakValue = ((uint16_t)setup.wValueH << 8) | setup.wValueL;
        } else if (setup.bRequest == CDC_SET_LINE_CODING) {
            USB_RecvControl((void*)&this->lc.lineCoding, sizeof(this->lc.lineCoding));
        } else if (setup.bRequest == CDC_SET_CONTROL_LINE_STATE) {
            this->lineState = setup.wValueL;
            if (this->lineState > 0) {
                // setup a better handler that does automatic flushing
                usbd_int_fops = &usb_inthandler;
            } else {
                usbd_int_fops = nullptr;
            }
        }
        return true;
    }
    return false;
}

void CDCACM_::begin(uint32_t baud)
{
    this->begin(baud, 0);
}

void CDCACM_::begin(uint32_t baud, uint8_t config)
{
    this->peekBuffer = -1;
}

void CDCACM_::end()
{
}

CDCACM_::operator bool()
{
    return this->lineState > 0;
}

int CDCACM_::available()
{
    if (this->peekBuffer < 0) {
        return USB_Available(this->outEndpoint);
    }
    return 1 + USB_Available(this->outEndpoint);
}

int CDCACM_::peek()
{
    if (this->peekBuffer < 0) {
        this->peekBuffer = USB_Recv(this->outEndpoint);
    }
    return this->peekBuffer;
}

int CDCACM_::read()
{
    if (this->peekBuffer < 0) {
        return USB_Recv(this->outEndpoint);
    }

    auto rc = this->peekBuffer;
    this->peekBuffer = -1;
    return rc;
}

int CDCACM_::availableForWrite()
{
    return USB_SendSpace(this->inEndpoint);
}

size_t CDCACM_::write(uint8_t c)
{
    return this->write(&c, sizeof(c));
}

size_t CDCACM_::write(const uint8_t* d, size_t len)
{
    if (this->lineState <= 0) {
        this->setWriteError();
        return 0;
    }

    auto w = USB_Send(this->inEndpoint, d, len);
    if (w <= 0) {
        this->setWriteError();
        return 0;
    }
    return w;
}

void CDCACM_::flush()
{
    USB_Flush(this->inEndpoint);
}

CDCACM_& CDCACM()
{
    static CDCACM_ obj(0, 1);
    return obj;
}
#endif
