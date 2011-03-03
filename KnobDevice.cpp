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
}


status_t
KnobDevice::ProbeDevice()
{
	size_t written = 0;
	status_t result = gUSBModule->send_request(fDevice,
		0x21,
		0x0a,
		0,
		0,
		0, NULL,
		&written);

	return LedPulse(0x80, 255, 1, 0);
}


status_t
KnobDevice::WriteState(int parameter, int value)
{
	if (fBusy == true)
		return B_BUSY;

	TRACE("RequestType = 0x%X\n", USB_REQTYPE_VENDOR | USB_REQTYPE_INTERFACE_OUT);
	TRACE("Request = 0x%X\n", USB_REQUEST_CLEAR_FEATURE);
	TRACE("Index = 0x%04x\n", cpu_to_le16(parameter));
	TRACE("Value = 0x%04x\n", cpu_to_le16(value));

	fBusy = true;
	// device, uint8 requesttype, uint8 request,  uint16 value, uint16 index, uint16 length, void *data
	size_t written = 0;
	status_t result = gUSBModule->send_request(fDevice,
		USB_REQTYPE_VENDOR | USB_REQTYPE_INTERFACE_OUT,
		USB_REQUEST_CLEAR_FEATURE,
		cpu_to_le16(parameter),
		cpu_to_le16(value),
		0, NULL,
		&written);

	TRACE("Bytes Written= %d\n", written);

	fBusy = false;
	return result;
}


status_t
KnobDevice::LedPulse(int brightness, int speed, int asleep, int awake)
{
	WriteState(SET_PULSE_ASLEEP, 1);
	WriteState(SET_PULSE_AWAKE, 1);
	WriteState(SET_PULSE_MODE, 1);
	WriteState(SET_STATIC_BRIGHTNESS, 0x80);
	//WriteState(0x0002, 0x0001);
	//WriteState(0x0003, 0x0000);
	//WriteState(0x0004, 0x0001);
	//WriteState(0x0001, 0x0080);
	return B_OK;
}

