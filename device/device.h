#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <stdint.h>

#include "deviceId.h"
#include "dev_cmd.h"

#include "dev_char.h"
#include "dev_lcd.h"

typedef uint32_t devId_t;






//ʹ�ú��������ú���Ϊ�˱����ĸ�����
devId_t Dev_TYPE( devId_t devid);
devId_t Dev_MAJORID( devId_t devid);
devId_t Dev_MINORID( devId_t devid);



int Dev_open( devId_t devId, void **dev);

#endif
