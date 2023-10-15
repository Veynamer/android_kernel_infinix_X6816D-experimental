#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <hwinfo/base.h>
#include <hwinfo/hwinfo.h>

struct custom_hwinfo {
	struct list_head l;
	char *name;
	char *value;
	struct kobj_attribute attr;
};

static LIST_HEAD(custom_hwinfo_array);

static ssize_t custom_hwinfo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	struct custom_hwinfo *info = container_of(attr, struct custom_hwinfo, attr);
	return sprintf(buf, "%s", info->value);
}

static struct kobject *hwinfo_kobj;

int custom_hwinfo_add(char *name, char *value)
{
	struct custom_hwinfo *list;
	list_for_each_entry(list, &custom_hwinfo_array, l) {
		if (strcmp(name, list->name))
			continue;
		if (strcmp(value, list->value)) {
			printk(KERN_ERR "custom_hwinfo reset %s\n", name);
			list->value = value;
		}
		return 1;
	}
	list = kzalloc(sizeof(*list), GFP_KERNEL);
	if (!list)
		return -ENOMEM;
	list->name = name;
	list->value = value;
	list->attr.attr.name = name;
	list->attr.attr.mode = S_IRUGO;
	list->attr.show = custom_hwinfo_show;
	list_add_tail(&list->l, &custom_hwinfo_array);
	return  sysfs_add_file_to_group(hwinfo_kobj, &list->attr.attr, NULL);
}
EXPORT_SYMBOL(custom_hwinfo_add);

extern struct EMI_SETTINGS custom_known_emi[];
extern int custom_known_emi_count;

int custom_emi_detect(char *id, int len)
{
	int i,j=0;
    char *mmc_cid;
    
    mmc_cid = kmalloc(25, GFP_KERNEL);
    memset(mmc_cid, 0 , 25);
    
    for (i = 0;i <9;i++) {
        j += sprintf(mmc_cid +j, "%02x", id[i]);
    }    
    custom_hwinfo_add("current_memory", mmc_cid);
  /*         
	if (len > 15)
		len = 15;
	for (i = 0; i < custom_known_emi_count; i++) {
		if (!memcmp(id, custom_known_emi[i].id, len)) {
			custom_hwinfo_add("current_memory", custom_known_emi[i].name);
			printk(KERN_ERR "found memory %s\n", custom_known_emi[i].name);
			return 1;
		}
	}*/
	return 0;
}
EXPORT_SYMBOL(custom_emi_detect);

static int __init custom_emi_init(void)
{
	int i;
	int count, temp;
	char *buf;
	const char **values;
	struct device_node *np;

	np = of_find_node_by_path("/chosen");
	if (!np)
		return -EINVAL;

	count = of_property_read_string_array(np, "custom,emi-support", NULL, 0);
	if (count < 0)
		return count;
	values = kcalloc(count, sizeof(*values), GFP_KERNEL);
	if (!values)
		return -ENOMEM;
	count = of_property_read_string_array(np, "custom,emi-support", values, count);
	if (count < 0)
		return -EINVAL;

	buf = kmalloc(256, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;
	temp = 0;
	for (i = 0; i < count; i++)
		temp += sprintf(buf + temp, "%s\n", values[i]);
	custom_hwinfo_add("all_memory", buf);
	kfree(values);
	return 0;
}
module_init(custom_emi_init);

static int __init custom_hwinfo_init(void)
{
	struct kobject *custom_kobj = custom_sysfs_get();
	if (!custom_kobj)
		return -ENODEV;

	hwinfo_kobj = kobject_create_and_add("hwinfo", custom_kobj);
	if (!hwinfo_kobj) {
		printk(KERN_ERR "failed to create /sys/sr/hwinfo\n");
		return -ENODEV;
	}
	custom_hwinfo_add("Product_Customer", "HMD");

	return 0;
}
subsys_initcall(custom_hwinfo_init);
