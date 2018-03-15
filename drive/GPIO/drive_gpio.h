#ifndef __INC_drive_gpio_H__
#define __INC_drive_gpio_H__


//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include "stdint.h"
#include "stm32f10x_gpio.h"
#include "lw_oopc.h"
#include "dri_cmd.h"
#include "configs/hardware_conf.h"
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define GITP_RISINGEDGE		0			
#define GITP_FAILINGEDGE	1			
#define GITP_MAX			2			

//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------

//�ڴ����ж��д������ݣ�������Ҫ���ٴ���Ĳ���
typedef void (*irqHdl)( void *arg, int type, int encode);


//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------
CLASS( driveGpio)
{
	uint8_t		encode[4];
	void		*p_gpioBase;
	void		*p_cfg;
	EXTI_InitTypeDef		*p_exit;
	irqHdl		func_hdl;
	void		*hdl_arg;
	
	
	int ( *init)( driveGpio *self, void *p_base, void *cfg);
	int ( *deInit)( driveGpio *self);
	int ( *read)( driveGpio *self,char *p_n_val);
	int ( *write)( driveGpio *self, char n_val);
	int ( *control_irq)( driveGpio *self, int en);
//	int ( *ioctol)( driveGpio *self, int cmd, ...);
	
	void (*setIrqHdl)( driveGpio *self, irqHdl hdl);
	void (*setEncode)( driveGpio *self, int e);
	
	
	
	int ( *test)( driveGpio *self, void *testBuf, int bufSize);
	
	
};




#endif
