/*
 * Copyright 2004-2011 Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Alexander von Gluck, kallisti5@unixzen.com
 */


// for mutex or fake mutex
#ifdef HAIKU_TARGET_PLATFORM_HAIKU
#include <lock.h>
#else
#include "BeOSCompatibility.h"
#endif

#include "Driver.h"


static const char *sDeviceBaseName = "input/usb_widget/";
char *gDeviceNames[MAX_DEVICES + 1];
int32 api_version = B_CUR_DRIVER_API_VERSION;
KnobDevice* gKnobDevices[MAX_DEVICES];
usb_module_info *gUSBModule = NULL;

// Supported USB Widget Devices
usb_support_descriptor gSupportedDevices[] = {
	{ 0, 0, 0, 0x077d, 0x0410}
		// "Griffin PowerMate USB"
};


mutex gDriverLock;


// auto-release helper class
class DriverSmartLock {
public:
	DriverSmartLock()  { mutex_lock(&gDriverLock);   }
	~DriverSmartLock() { mutex_unlock(&gDriverLock); }
};


KnobDevice*
create_knob_device(usb_device device)
{
	const usb_device_descriptor *deviceDescriptor
		= gUSBModule->get_device_descriptor(device);

	if (deviceDescriptor == NULL) {
		TRACE("Error: Problem getting USB device descriptor.\n");
		return NULL;
	}

#define IDS(__vendor, __product) (((__vendor) << 16) | (__product))

	switch(IDS(deviceDescriptor->vendor_id, deviceDescriptor->product_id)) {
		case IDS(0x077d, 0x0410):
			return new KnobDevice(device, "Griffin PowerMate USB");
	}
	return NULL;
}


status_t
usb_widget_device_added(usb_device device, void **cookie)
{
	*cookie = NULL;

	DriverSmartLock driverLock; // released on exit

	KnobDevice *knobDevice = create_knob_device(device);
	if (knobDevice == 0) {
		return ENODEV;
	}

	//status_t status = knobDevice->SetupDevice(false);
	status_t status = B_OK;
	if (status < B_OK) {
		delete knobDevice;
		return status;
	}

	for (int32 i = 0; i < MAX_DEVICES; i++) {
		if (gKnobDevices[i] != NULL)
			continue;

		gKnobDevices[i] = knobDevice;
		*cookie = knobDevice;

		TRACE("Debug: New device is added at %ld.\n", i);
		return B_OK;
	}

	// no space for the device
	TRACE("Error: no more device entries availble.\n");

	delete knobDevice;
	return B_ERROR;
}


status_t
usb_widget_device_removed(void *cookie)
{
	TRACE("Debug: device was removed.\n");
	DriverSmartLock driverLock; // released on exit

	KnobDevice *device = (KnobDevice *)cookie;
	for (int32 i = 0; i < MAX_DEVICES; i++) {
		if (gKnobDevices[i] == device) {
			//if (device->IsOpen()) {
			//	// the device will be deleted upon being freed
			//	TRACE("Debug: Device %ld will be deleted upon being freed.\n", i);
			//	device->Removed();
			//} else {
				TRACE("Debug: Device at %ld will be deleted.\n", i);
				gKnobDevices[i] = NULL;
				delete device;
				TRACE("Debug: Device at %ld deleted.\n", i);
			//}
			break;
		}
	}

	return B_OK;
}


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

	TRACE("Init: Driver API Version: %lu\n", api_version);

	for (int32 i = 0; i < MAX_DEVICES; i++)
		gKnobDevices[i] = NULL;

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


