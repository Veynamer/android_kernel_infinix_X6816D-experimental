#ifndef __CUSTOM_USB_CB_H__
#define __CUSTOM_USB_CB_H__

typedef	int (*usb_callback)(struct notifier_block *nb,
			unsigned long usb_status, void *data);
void register_usb_callback(usb_callback cb);
#endif
