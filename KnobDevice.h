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
#include <support/ByteOrder.h>


// device command codes
#define SET_STATIC_BRIGHTNESS	0x01
#define SET_PULSE_ASLEEP		0x02
#define SET_PULSE_AWAKE			0x03
#define SET_PULSE_MODE			0x04


class KnobDevice
{
public:
								KnobDevice(usb_device device,
									const char *description);
	virtual						~KnobDevice();

			status_t			ProbeDevice();
			const char*			Name()
									{ return fName; }

protected:
			status_t			WriteState(uint16 parameter, uint16 value);
			status_t			LedPulse(uint16 brightness, uint16 speed,
									uint16 asleep, uint16 awake);

private:
			usb_device			fDevice;
			const char*			fName;
	volatile bool				fBusy;
};


#endif

