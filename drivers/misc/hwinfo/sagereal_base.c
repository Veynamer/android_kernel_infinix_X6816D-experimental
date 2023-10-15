#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <hwinfo/base.h>

static struct kobject *custom_kobj;
struct kobject * custom_sysfs_get(void)
{
	if (!custom_kobj) {
		custom_kobj = kobject_create_and_add("sr", NULL);
		if (!custom_kobj) {
			printk(KERN_ERR "failed to create /sys/sr\n");
			return NULL;
		}
	}
	return custom_kobj;
}
