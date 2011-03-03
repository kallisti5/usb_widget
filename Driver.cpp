/*
 * Copyright 2004-2011 Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Alexander von Gluck, kallisti5@unixzen.com
 */

#include "Driver.h"

static const char *sDeviceBaseName = "input/usb_widget/";
int32 api_version = B_CUR_DRIVER_API_VERSION;
usb_module_info *gUSBModule = NULL;

usb_support_descriptor gSupportedDevices[] = {
	{ 0, 0, 0, 0x077d, 0x0410}
		// "Griffin PowerMate USB"
};


//#pragma mark -


status_t
init_hardware()
{
	return B_OK;
}


status_t
init_driver()
{
	status_t status = get_module(B_USB_MODULE_NAME,
		(module_info **)&gUSBModule);
	if (status < B_OK)
		return status;

	load_settings();

	TRACE("Init: Driver API Version: %lu\n", api_version);

	for (int32 i = 0; i < MAX_DEVICES; i++)
		gBeceemDevices[i] = NULL;

	gDeviceNames[0] = NULL;
	mutex_init(&gDriverLock, DRIVER_NAME"_devices");

	static usb_notify_hooks notifyHooks = {
		&usb_widget_device_added,
		&usb_widget_device_removed
	};

	gUSBModule->register_driver(DRIVER_NAME, gSupportedDevices,
		sizeof(gSupportedDevices) / sizeof(usb_support_descriptor), NULL);
	gUSBModule->install_notify(DRIVER_NAME, &notifyHooks);
	return B_OK;
}

