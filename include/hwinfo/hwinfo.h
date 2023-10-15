#ifndef __CUSTOM_HWINFO_H__
#define __CUSTOM_HWINFO_H__

int custom_hwinfo_add(char *name, char *value);

struct EMI_SETTINGS {
	char *name;
	char id[16];
};

#endif
