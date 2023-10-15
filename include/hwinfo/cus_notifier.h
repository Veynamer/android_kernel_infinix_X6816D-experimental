#ifndef __CUSTOM_NOTIFIER_H__
#define __CUSTOM_NOTIFIER_H__

#include <linux/notifier.h>
#include <linux/export.h>

static BLOCKING_NOTIFIER_HEAD(cable_status_notifier_list);

int cable_unregister_client(struct notifier_block *nb);
int cable_notifier_call_chain(unsigned long val, void *v);
int cable_register_client(struct notifier_block *nb);

#endif//__CUSTOM_NOTIFIER_H__
