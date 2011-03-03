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


class KnobDevice
{
public:
								KnobDevice(usb_device device,
									const char *description);
	virtual						~KnobDevice();

			status_t			ProbeDevice();

private:
			usb_device			fDevice;
				// Usb Device Instance
};


#endif

