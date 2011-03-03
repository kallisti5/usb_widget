/*
 * Copyright 2004-2011 Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Alexander von Gluck, kallisti5@unixzen.com
 */
#ifndef _USB_KNOB_DRIVER_H_
#define _USB_KNOB_DRIVER_H_


#include "Driver.h"


#define SET_STATIC_BRIGHTNESS		0x01
#define SET_PULSE_ASLEEP			0x02
#define SET_PUSLE_AWAKE				0x03
#define SET_PULSE_MODE				0x04

#define UPDATE_STATIC_BRIGHTNESS	(1<<0)
#define UPDATE_PULSE_ASLEEP			(1<<1)
#define UPDATE_PULSE_AWAKE			(1<<2)
#define UPDATE_PULSE_MODE			(1<<3)


class KnobDevice
{
public:
								KnobDevice(usb_device device,
									const char *description);
	virtual						~KnobDevice();

			status_t			ProbeDevice();
			const char*			Name()
									{ return fName; }

private:
			usb_device			fDevice;
			const char*			fName;
				// Device name
	volatile bool				fBusy;
				// Is currently busy?
};


#endif

