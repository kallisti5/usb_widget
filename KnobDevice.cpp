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
	fName(description),
	fBusy(false)
{
	fPayload->static_brightness = 0x255;
	fPayload->pulse_speed = 0x0;
	fPayload->pulse_table = 1;
	fPayload->pulse_asleep = 0x1;
	fPayload->pulse_awake = 0x0;
	fPayload->requires_update = true;
}


KnobDevice::~KnobDevice()
{

}


status_t
KnobDevice::ProbeDevice()
{
	LedPulse(0x80, 255, 1, 0);
	return B_OK;
}


status_t
KnobDevice::LedPulse(int brightness, int speed, int asleep, int awake)
{
	size_t written = 0;

	status_t result = gUSBModule->send_request(fDevice,
		USB_REQTYPE_VENDOR | USB_REQTYPE_INTERFACE_OUT,
		USB_REQUEST_CLEAR_FEATURE,
		SET_STATIC_BRIGHTNESS,
		UPDATE_STATIC_BRIGHTNESS,
		sizeof(&fPayload), fPayload,
		&written);

	return result;
}


