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


#define SET_STATIC_BRIGHTNESS	0x01
#define SET_PULSE_ASLEEP		0x02
#define SET_PULSE_AWAKE			0x03
#define SET_PULSE_MODE			0x04

#if __BYTE_ORDER == __BIG_ENDIAN
#define cpu_to_le16(x) (B_SWAP_INT16(x))
#else
#define cpu_to_le16(x) (x)
#endif


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
			status_t			WriteState(int parameter, int value);
			status_t			LedPulse(int brightness, int speed,
									int asleep, int awake);

private:
			usb_device			fDevice;
			const char*			fName;
	volatile bool				fBusy;
};


#endif

