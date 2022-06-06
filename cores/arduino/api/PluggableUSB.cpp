/*
  PluggableUSB.cpp
  Copyright (c) 2015 Arduino LLC

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#if USBCON
#include "PluggableUSB.h"

#include "USBCore.h"

using namespace arduino;

PluggableUSB_::PluggableUSB_(uint8_t firstIf, uint8_t firstEp) {
  this->lastIf = firstIf;
  this->lastEp = firstEp;
  this->rootNode = nullptr;
  this->totalEP = EP_COUNT;
}

int PluggableUSB_::getInterface(uint8_t* interfaceCount)
{
  int sent = 0;
  PluggableUSBModule* node;
  for (node = rootNode; node; node = node->next) {
    int res = node->getInterface(interfaceCount);
    if (res < 0)
      return -1;
    sent += res;
  }
  return sent;
}

int PluggableUSB_::getDescriptor(USBSetup& setup)
{
  PluggableUSBModule* node;
  for (node = rootNode; node; node = node->next) {
    int ret = node->getDescriptor(setup);
    // ret!=0 -> request has been processed
    if (ret)
      return ret;
  }
  return 0;
}

void PluggableUSB_::getShortName(char *iSerialNum)
{
  PluggableUSBModule* node;
  for (node = rootNode; node; node = node->next) {
    iSerialNum += node->getShortName(iSerialNum);
  }
  *iSerialNum = 0;
}

bool PluggableUSB_::setup(USBSetup& setup)
{
  PluggableUSBModule* node;
  for (node = rootNode; node; node = node->next) {
    if (node->setup(setup)) {
      return true;
    }
  }
  return false;
}

bool PluggableUSB_::plug(PluggableUSBModule *node)
{
  if ((lastEp + node->numEndpoints) > totalEP) {
    return false;
  }

  if (!rootNode) {
    rootNode = node;
  } else {
    PluggableUSBModule *current = rootNode;
    while (current->next) {
      current = current->next;
    }
    current->next = node;
  }

  node->pluggedInterface = lastIf;
  node->pluggedEndpoint = lastEp;
  lastIf += node->numInterfaces;
  for (uint8_t i = 0; i < node->numEndpoints; i++) {
    *(unsigned int*)(epBuffer(lastEp)) = node->endpointType[i];
    lastEp++;
  }
  return true;
  // restart USB layer???
}

uint8_t PluggableUSB_::ifCount() {
  return this->lastIf;
}

uint8_t PluggableUSB_::epCount() {
  return this->lastEp;
}

PluggableUSB_& PluggableUSB()
{
#ifdef USBD_USE_CDC
  static PluggableUSB_ obj(2, 4);
#else
  static PluggableUSB_ obj(0, 1);
#endif
  return obj;
}

#endif