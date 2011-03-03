/*
 * Copyright 2004-2011 Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Alexander von Gluck, kallisti5@unixzen.com
 */
#ifndef _USB_WIDGET_DRIVER_H_
#define _USB_WIDGET_DRIVER_H_


#include <Drivers.h>
#include <KernelExport.h>
#include <OS.h>
#include <util/kernel_cpp.h>
#include <USB3.h>

#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "KnobDevice.h"

#define DRIVER_NAME "usb_widget"
#define TRACE(x...) dprintf(DRIVER_NAME ": " x)

#define MAX_DEVICES 4


extern usb_module_info *gUSBModule;

extern "C" {
status_t usb_widget_device_added(usb_device device, void **cookie);
status_t usb_widget_device_removed(void *cookie);

status_t init_hardware();
void uninit_driver();

const char **publish_devices();
device_hooks *find_device(const char *name);
}


#endif

