#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <hwinfo/base.h>


int ril_call_state = 0;	/* by default, we enable it */

struct custom_misc {
	struct list_head l;
	char *name;
	CUSTOM_HW_RW read;
	CUSTOM_HW_RW write;
	struct kobj_attribute attr;
};

static int ps_callstate_read(unsigned char *buf, int len, void *data)
{
	return sprintf(buf, "%d", ril_call_state);
}

static int ps_callstate_write(unsigned char *buf, int len, void *data)
{
	if (buf[0] == 0 || buf[0] == '0' || !strncmp(buf, "off", len))
		ril_call_state = 0;
	else if (buf[0] == 1 || buf[0] == '1' || !strncmp(buf, "on", len))
		ril_call_state = 1;
	else
		return -EINVAL;
	printk(KERN_CRIT "set ril_call_state enable %d\n", ril_call_state);
	return len;
}

static LIST_HEAD(custom_misc_array);

static ssize_t custom_misc_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	struct custom_misc *info = container_of(attr, struct custom_misc, attr);
	return info->read(buf, 1024, NULL);
}

static ssize_t custom_misc_store(struct kobject *obj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	struct custom_misc *info = container_of(attr, struct custom_misc, attr);
	return info->write((char *)buf, count, NULL);
}

static struct kobject *misc_kobj;

int custom_misc_add(char *name, CUSTOM_HW_RW read, CUSTOM_HW_RW write)
{
	struct custom_misc *list;
	list_for_each_entry(list, &custom_misc_array, l) {
		if (!strcmp(name, list->name)) {
			printk(KERN_ERR "FIXME custom misc interface %s already exist\n", name);
			return 1;
		}
	}
	list = kzalloc(sizeof(*list), GFP_KERNEL);
	if (!list)
		return -ENOMEM;
	list->name = name;
	list->read = read;
	list->write = write;
	list->attr.attr.name = name;
	list->attr.attr.mode = S_IRUGO | S_IWUSR | S_IWGRP;
	list->attr.show = custom_misc_show;
	list->attr.store = custom_misc_store;
	list_add_tail(&list->l, &custom_misc_array);
	return  sysfs_add_file_to_group(misc_kobj, &list->attr.attr, NULL);
}
EXPORT_SYMBOL(custom_misc_add);

static int __init custom_misc_init(void)
{
	int ret = 0;
	struct kobject *custom_kobj = custom_sysfs_get();
	if (!custom_kobj)
		return -ENODEV;
	misc_kobj = kobject_create_and_add("misc", custom_kobj);
	if (!misc_kobj) {
		printk(KERN_ERR "failed to create /sys/sr/misc\n");
		return -ENODEV;
	}
	
	return ret;
}
subsys_initcall(custom_misc_init);
