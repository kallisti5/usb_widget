/*
 * Copyright 2004-2011 Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Alexander von Gluck, kallisti5@unixzen.com
 */


#include "KnobDevice.h"
#include <support/ByteOrder.h>


KnobDevice::KnobDevice(usb_device device, const char *description)
	:
	fDevice(device),
	fName(description),
	fBusy(false)
{

}


KnobDevice::~KnobDevice()
{
	WriteState(SET_STATIC_BRIGHTNESS, 0x0);
}


status_t
KnobDevice::ProbeDevice()
{
	size_t written = 0;
	// device, uint8 requesttype, uint8 request,  uint16 value, uint16 index, uint16 length, void *data
	status_t result = gUSBModule->send_request(fDevice,
		USB_REQTYPE_INTERFACE_OUT | USB_REQTYPE_CLASS,
		USB_REQUEST_GET_INTERFACE,
		0,
		0, // bInterfaceNumber?
		0, NULL,
		&written);

	if (result != B_OK) {
		TRACE("%s: Couldn't communicate over USB to device!\n", __func__);
		return result;
	}

	// test our communications... gives a nice flicker during powerup
	LedPulse(0, 255, 0, 0);
	snooze(5000);
	LedPulse(255, 255, 1, 1);
	snooze(5000);
	LedPulse(0, 255, 1, 1);
	snooze(50000);
	LedPulse(255, 255, 1, 1);

	return B_OK;
}


status_t
KnobDevice::WriteState(uint16 parameter, uint16 value)
{
	if (fBusy == true)
		return B_BUSY;

	TRACE("%s: Parameter: 0x%" B_PRIX16 "; Value: 0x%" B_PRIX16 "\n",
		__func__, parameter, value);

	fBusy = true;
	// device, uint8 requesttype, uint8 request,  uint16 value, uint16 index, uint16 length, void *data
	status_t result = gUSBModule->send_request(fDevice,
		USB_REQTYPE_VENDOR | USB_REQTYPE_INTERFACE_OUT,
		USB_REQUEST_CLEAR_FEATURE,
		B_HOST_TO_LENDIAN_INT16(parameter),
		B_HOST_TO_LENDIAN_INT16(value),
		0, NULL, NULL);

	if (result != B_OK) {
		TRACE("%s: Couldn't communicate over USB to device!\n", __func__);
		return result;
	}

	fBusy = false;
	return result;
}


status_t
KnobDevice::LedPulse(uint16 brightness, uint16 speed, uint16 asleep, uint16 awake)
{
	WriteState(SET_PULSE_ASLEEP, asleep ? 1 : 0);
	WriteState(SET_PULSE_AWAKE, awake ? 1 : 0);

	uint16 operation;
	uint16 value;
	if (speed < 255) {
		operation = 0;			// divide
		value = 255 - speed;
	} else if (speed > 255) {
		operation = 2;			// multiply
		value = speed - 255;
	} else {
		operation = 1;			// normal speed
		value = 0;				// can be any value
	}
	WriteState((0 << 8) | SET_PULSE_MODE, (value << 8 | operation));

	WriteState(SET_STATIC_BRIGHTNESS, brightness);
	return B_OK;
}

