/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#define EP0      0
#define EPX_SIZE 64 // 64 for Full Speed, EPT size max is 1024

#if defined __cplusplus

#include "Arduino.h"
#include "api/Stream.h"
#include "api/RingBuffer.h"
#include "api/USBAPI.h"
#include "USBDescriptors.h"
#include "CDC.h"

//================================================================================
// USB

class USBDeviceClass {
public:
	USBDeviceClass() {};

	// USB Device API
	void init();
	bool end();
	bool attach();
	bool detach();
	void setAddress(uint32_t addr);

	bool configured();
	bool connected();

	void standby();

	// Setup API
	bool handleClassInterfaceSetup(arduino::USBSetup &setup);
	bool handleStandardSetup(arduino::USBSetup &setup);
	bool sendDescriptor(arduino::USBSetup &setup);

	// Control EndPoint API
	uint32_t sendControl(const void *data, uint32_t len);
	uint32_t sendControl(int /* ep */, const void *data, uint32_t len) { return sendControl(data, len); }
	uint32_t recvControl(void *data, uint32_t len);
	uint32_t sendConfiguration(uint32_t maxlen);
	bool sendStringDescriptor(const uint8_t *string, uint32_t maxlen);
	void initControl(int end);
	uint8_t SendInterfaces(uint32_t* total);
	void packMessages(bool val);

	// Generic EndPoint API
	void initEndpoints(void);
	void initEP(uint32_t ep, uint32_t type);

	uint32_t send(uint32_t ep, const void *data, uint32_t len);
	void sendZlp(uint32_t ep);
	uint32_t recv(uint32_t ep, void *data, uint32_t len);
	int recv(uint32_t ep);
	uint32_t available(uint32_t ep);
	void flush(uint32_t ep);
	void clear(uint32_t ep);
	void stall(uint32_t ep);

	// private?
	uint32_t armSend(uint32_t ep, const void *data, uint32_t len);
	uint8_t armRecvCtrlOUT(uint32_t ep);

	void ISRHandler();

private:
	bool initialized;
};
extern USBDeviceClass USBDevice;

#endif  // __cplusplus
