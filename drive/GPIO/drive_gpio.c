
#include "stdint.h"
#include "GPIO/drive_gpio.h"
#include "stm32f10x_exti.h"

#include "basis/sdhError.h"
#include <stdarg.h>
#include <string.h>
#include "sdhDef.h"

//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define EXTI_MAX		16
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------
//用在中断程序中，找到对应的驱动
const uint32_t arr_exti_line[EXTI_MAX] = { EXTI_Line0, EXTI_Line1, EXTI_Line2, \
	EXTI_Line3, EXTI_Line4, EXTI_Line5, EXTI_Line6,EXTI_Line7, EXTI_Line8,\
	EXTI_Line9, EXTI_Line10, EXTI_Line11, EXTI_Line12, EXTI_Line13, EXTI_Line14, EXTI_Line15};

static driveGpio	*arr_driGpio[NUMPIN];

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
/* Cycle/Sync Callback functions */

static int GpioInit( driveGpio *self, void *p_base, void *cfg);
static int GpioDeInit( driveGpio *self);
static int GpioWrite( driveGpio *self, char ch_val);
static int GpioRead( driveGpio *self,char *p_ch_val);
static int GpioTest( driveGpio *self, void *buf, int size);
static void GpioSetIrqHdl( driveGpio *self, irqHdl hdl);
static void GpioSetEncode( driveGpio *self, int e);
static int Gpio_control_irq( driveGpio *self, int en);
static void ExtiIrq(uint8_t el);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//


CTOR( driveGpio)
FUNCTION_SETTING( init, GpioInit);
FUNCTION_SETTING( deInit, GpioDeInit);
FUNCTION_SETTING( read, GpioRead);
FUNCTION_SETTING( write, GpioWrite);
FUNCTION_SETTING( setIrqHdl, GpioSetIrqHdl);
FUNCTION_SETTING( setEncode, GpioSetEncode);
FUNCTION_SETTING(control_irq, Gpio_control_irq);

FUNCTION_SETTING( test, GpioTest);
END_CTOR


void EXTI0_IRQHandler(void)
{
	ExtiIrq(0);
	EXTI_ClearITPendingBit(EXTI_Line0);
}
void EXTI1_IRQHandler(void)
{
    ExtiIrq(1);   
	EXTI_ClearITPendingBit(EXTI_Line1);
}
void EXTI2_IRQHandler(void)
{
	ExtiIrq(2);
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI3_IRQHandler(void)
{
	ExtiIrq(3);
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI4_IRQHandler(void)
{
	ExtiIrq(4);
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void EXTI9_5_IRQHandler(void)
{
	if( EXTI_GetITStatus( EXTI_Line5))
	{
		ExtiIrq(5);
       EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if( EXTI_GetITStatus( EXTI_Line6))
	{
		ExtiIrq(6);
       EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if( EXTI_GetITStatus( EXTI_Line7))
	{
		ExtiIrq(7);
       EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if( EXTI_GetITStatus( EXTI_Line8))
	{
		ExtiIrq(8);
       EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if( EXTI_GetITStatus( EXTI_Line9))
	{
		ExtiIrq(9);
       EXTI_ClearITPendingBit(EXTI_Line9);
	}
}

void EXTI15_10_IRQHandler(void)
{
   if( EXTI_GetITStatus( EXTI_Line10))
	{
		ExtiIrq(10);
       EXTI_ClearITPendingBit(EXTI_Line10);
	}
	if( EXTI_GetITStatus( EXTI_Line11))
	{
		ExtiIrq(11);
       EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if( EXTI_GetITStatus( EXTI_Line12))
	{
		ExtiIrq(12);
       EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if( EXTI_GetITStatus( EXTI_Line13))
	{
		ExtiIrq(13);
       EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if( EXTI_GetITStatus( EXTI_Line14))
	{
		ExtiIrq(14);
       EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if( EXTI_GetITStatus( EXTI_Line15))
	{
		ExtiIrq(15);
       EXTI_ClearITPendingBit(EXTI_Line15);
	}
}

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{






static int GpioInit( driveGpio *self, void *p_base, void *cfg)
{
	gpio_pins	*p_gpio = ( gpio_pins *)cfg;
	arr_driGpio[p_gpio->gpio_num] = self;
	
	self->p_cfg = cfg;
	
	
	//不需要中断
	if(p_gpio->exti_line > EXTI_MAX)
		goto exit;
	if( p_gpio->direction == GPIO_DIR_OUT)
		goto exit;
	
	
	
	self->p_exit = malloc(sizeof(EXTI_InitTypeDef));
	self->p_gpioBase = p_base;
	
	
	
	
	EXTI_StructInit(self->p_exit);
	
	
	self->p_exit->EXTI_Line = arr_exti_line[ p_gpio->exti_line];
	self->p_exit->EXTI_LineCmd = ENABLE;
	self->p_exit->EXTI_Mode = EXTI_Mode_Interrupt;
	
	switch( p_gpio->irqType)
	{
		
		case GPIO_IRQ_FAILING:
			self->p_exit->EXTI_Trigger = EXTI_Trigger_Falling;
			break;
		case GPIO_IRQ_RISING:
			self->p_exit->EXTI_Trigger = EXTI_Trigger_Rising;
			break;
		case GPIO_IRQ_BOTHEDGE:
			self->p_exit->EXTI_Trigger = EXTI_Trigger_Rising_Falling;
			break;
		default:
			goto exit;
		
	}
	GPIO_EXTILineConfig( p_gpio->portSource, p_gpio->pinSource);
	EXTI_ClearITPendingBit( arr_exti_line[ p_gpio->exti_line]);
	EXTI_Init( self->p_exit);

	exit:
	return ERR_OK;
	
}


static int GpioDeInit( driveGpio *self)
{
	
	
	self->func_hdl = NULL;
	
	return ERR_OK;
	
}

static int Gpio_control_irq( driveGpio *self, int en)
{
	gpio_pins	*p_gpio = self->p_cfg;
	if(self->p_exit == NULL)
		return ERR_OK;
	
	if(en)
	{
		self->p_exit->EXTI_LineCmd = ENABLE;
	}
	else 
	{
		self->p_exit->EXTI_LineCmd = DISABLE;
	}
	GPIO_EXTILineConfig( p_gpio->portSource, p_gpio->pinSource);
	EXTI_ClearITPendingBit( arr_exti_line[ p_gpio->exti_line]);
	EXTI_Init( self->p_exit);
	return ERR_OK;

}

/*!
**
**
** @param data 
** @param size 
** @return
**/
static int GpioWrite( driveGpio *self, char ch_val)
{
//	gpio_pins	*p_gpio = ( gpio_pins *)self->p_cfg;

	return ERR_OK;
}



/*!
**
**
** @param data 
** @param size 
** @return
**/
static int GpioRead( driveGpio *self,char *p_ch_val)
{
	gpio_pins	*p_gpio = ( gpio_pins *)self->p_cfg;
	*p_ch_val = GPIO_ReadInputDataBit( p_gpio->Port,p_gpio->pin);
	return ERR_OK;
}








static int GpioTest( driveGpio *self, void *buf, int size)
{
	
	return RET_OK;
}


static void GpioSetIrqHdl( driveGpio *self, irqHdl hdl)
{
	
	self->func_hdl = hdl;
	
	
}

static void GpioSetEncode( driveGpio *self, int e)
{
	self->encode[0] = e;
	
}

static void ExtiIrq(uint8_t el) 
{
//	char type;
	
	driveGpio *p_gpio = NULL;
	char pin;
	
	//找到中断线匹配的引脚
	for(pin = 0; pin < NUMPIN; pin ++)
	{
		if(arr_driGpio[pin] == NULL)
			continue;
		if(arr_driGpio[pin]->p_cfg->exti_line == el)
		{
			p_gpio = arr_driGpio[pin];
			break;
			
		}
		
		
	}
	
	if( p_gpio == NULL)
		return;
	
	if( p_gpio->func_hdl)
	{
		if( p_gpio->read( p_gpio, &pin) != RET_OK)
			return;
		//当前电平的状态来判断是上升沿还是下降沿产生的中断
		if( pin == 1)
			p_gpio->func_hdl( p_gpio->hdl_arg, GITP_RISINGEDGE, p_gpio->encode[0]);
		
		if( pin == 0)
			p_gpio->func_hdl( p_gpio->hdl_arg, GITP_FAILINGEDGE, p_gpio->encode[0]);
	}
	
}













