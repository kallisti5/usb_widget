/*
 * Copyright 2004-2011 Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Alexander von Gluck, kallisti5@unixzen.com
 */


#include "KnobDevice.h"


KnobDevice::KnobDevice(usb_device device, const char *description)
	:
	fDevice(device),
	fName(description)
{

}


KnobDevice::~KnobDevice()
{

}


status_t
KnobDevice::ProbeDevice()
{

	return B_OK;
}
