#include "dev_char.h"
#include "dev_id.h"


#include "dev_uart/dev_uart.h"
#include "dev_spi/dev_spi.h"
#include "dev_IIC/dev_IIC.h"

#include "dev_gpio/device_gpio.h"
#include "basis/sdhDebug.h"
#include "sdhDef.h"
#include "basis/sdhError.h"

int Open_dev_char( int major, int minor, void **dev)
{
	int ret = 0;
//	switch( major)
//	{
//		case DEVMAJOR_UART:
//			*dev = Get_DevUart( minor);
//			if( dev == NULL)
//			{
//				
//				ret = ERR_BAD_PARAMETER;
//			}
//			break;
//		case DEVMAJOR_GPIO:
//			*dev = Get_DevGpio( minor);
//			if( dev == NULL)
//			{
//				
//				ret = ERR_BAD_PARAMETER;
//			}
//		
//			break;
//		case DEVMAJOR_SPI:
//			*dev = Get_DevSpi( minor);
//			if( dev == NULL)
//			{
//				
//				ret = ERR_BAD_PARAMETER;
//			}
//		
//			break;
//		case DEVMAJOR_IIC:
//			*dev = Get_Dev_IIC( minor);
//			if( dev == NULL)
//			{
//				
//				ret = ERR_BAD_PARAMETER;
//			}
//		
//			break;
//		default:
//			ret = ERR_BAD_PARAMETER;
//			break;
//		
//		
//	}
	
	return ret;
}

void DEV_CHAR_set_self(dev_Char *self)
{
	self->self = self;
}

ABS_CTOR( dev_Char)
FUNCTION_SETTING(set_self, DEV_CHAR_set_self);

END_ABS_CTOR


