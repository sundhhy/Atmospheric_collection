#ifndef __DEV_CHAR_H_
#define __DEV_CHAR_H_
#include "stdint.h"
#include "lw_oopc.h"
//字符型设备的接口
//包括uart设备


ABS_CLASS(dev_Char)
{
	dev_Char		*self;
	
	void (*set_self)(dev_Char *self);
	int (*open)(void *conf);
	int (*close)(void);
	int (*read)(void *buf, int rdLen);
	int (*write)(void *buf, int wrLen);
	int (*ioctol)(int cmd, ...);
	int (*test)(void *testBuf, int len);
	
};

int Open_dev_char( int major, int minor, void **dev);

#endif
