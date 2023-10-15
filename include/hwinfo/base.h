#ifndef __CUSTOM_BASE_H__
#define __CUSTOM_BASE_H__

struct kobject * custom_sysfs_get(void);

/* when CUSTOM_HW_RW used for debug interface, data points to register base address */
/* when CUSTOM_HW_RW used for misc interface, data points to NULL */
typedef int (*CUSTOM_HW_RW)(unsigned char *buf, int len, void *data);

#ifdef CONFIG_CUSTOM_MISC
int custom_misc_add(char *name, CUSTOM_HW_RW read, CUSTOM_HW_RW write);
#endif

#endif
