/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include <stdio.h>
#include <string.h>
#include "stm32f10x_usart.h"

#include "hardwareConfig.h"
#include "mySystemClock.h"
#include "pinmux.h"
#include "power.h"
#include "intrInit.h"
#include "sys_cmd.h"
#include "Modbus_app.h"

#include "basis/macros.h"
#include "basis/sdhError.h"
#include "device.h"
#include "HMI/HMIFactory.h"
#include "HMI/Component_tips.h"

#include "Usb.h"

#include "control/CtlKey.h"
#include "control/CtlTimer.h"
#include "utils/time.h"
#include "utils/keyboard.h"
#include "utils/rtc.h"


#include "tdd_conf.h"
#include "unit_tests/unit_test.h"


#include "os/os_depend.h"
//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------
//系统版本号
//主板本号自己制定
//系统编译时的月份和日作为子版本
short  g_majorVer = 1;		
short  g_childVer;
		
//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define LCD_NOKIE		0

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static void ThrdKeyRun (void const *argument);                             // thread function
osThreadId tid_Thread_key;                                          // thread id
osThreadDef (ThrdKeyRun, osPriorityNormal, 1, 0);                   // thread object

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------

static int	Main_USB_event(int type);
static void 	Init_device(void);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

/*
 * main: initialize and start the system
 */

//extern cmd_run_t 	cmd_run;


int main (void) {
	Keyboard		*p_kb;
	Controller		*p_control;
	
	Model 			*p_mdl_time;
	CMP_tips 		*p_tips;
	uint8_t			main_count_1s = 0;
	uint8_t			old_sys_flag = aci_sys.sys_flag;
	
  // initialize peripherals here

	//BSP的初始化
	OpenPrpClock();
	Pin_init();
	NVIC_Configuration();
	
	PVD_Init();

	Init_device();
	
	
	System_init();
	
	
	p_kb = GetKeyInsance();
//	
	//借用一下内存:&aci_sys.lcd_cmd_bytes
	aci_sys.lcd_cmd_bytes = CONF_KEYSCAN_CYCLEMS;
	p_kb->init( p_kb, &aci_sys.lcd_cmd_bytes);
	aci_sys.lcd_cmd_bytes  = 0;
	
	tid_Thread_key = osThreadCreate (osThread(ThrdKeyRun), p_kb);
	if (!tid_Thread_key) return(-1);

#if TDD_ON == 1
	unit_test();
#endif
	
	osKernelInitialize (); 
	Init_Cmd_Thread();
//	// initialize CMSIS-RTOS
//	//各个外设驱动模块初始化
//	
//		
//	//控制器初始化


//	
//	

//	//界面初始化
	HMI_Init();
//	//按键初始化
	

//	//创建控制器
	p_control = SUPER_PTR( Get_CtlKey(), Controller);
	p_control->init(p_control, p_kb);
//	
//	p_control = SUPER_PTR(CtlTimer_new(), Controller);
//	if(p_control == NULL) while(1);
//	p_control->init(p_control, NULL);
//	
////	创建线程
//	

//	
	p_mdl_time = ModelCreate("time");
//	
//	USB_Rgt_event_hdl(Main_USB_event);
	osKernelStart ();  

//	MBA_Init();
//	
//	aci_sys.sys_flag |= SYSFLAG_POWEON;
//	p_tips = TIP_Get_Sington();
	
	
//	p_tips->show_ico_tips(1, -1);

	while(1)
	{
		osDelay(100);
		main_count_1s ++;
		
//		if(old_sys_flag != aci_sys.sys_flag)
//		{
//			if(aci_sys.sys_flag & SYSFLAG_POWEROFF)
//			{
//				p_tips->show_ico_tips(1, -1);
//				
//			}
//			else if(aci_sys.sys_flag & SYSFLAG_EFS_NOTREADY)
//			{
//				p_tips->show_ico_tips(1, -1);
//				
//			}
//			else if(aci_sys.sys_flag & SYSFLAG_ERR)
//			{
//				p_tips->show_ico_tips(2, -1);
//				
//			}
//			else
//			{
//				
//				p_tips->clear_ico_tips(1);
//				p_tips->clear_ico_tips(2);
//			}
//			old_sys_flag = aci_sys.sys_flag;
//		}
		if(main_count_1s >= 5)
		{
			p_mdl_time->run(p_mdl_time);
			main_count_1s = 0;
		}
//		USB_Run();
		LCD_Run();
//		osThreadYield (); 

//		if(cmd_run.func)
//			cmd_run.func(cmd_run.arg);

	}

  
}

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{



static void ThrdKeyRun (void const *argument) {
	Keyboard	*p_kb = ( Keyboard	*)argument ;
	while (1) { 
		delay_ms(CONF_KEYSCAN_CYCLEMS);		
		p_kb->run( p_kb);
		osThreadYield ();                                           // suspend thread
	}
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData( DEBUG_USART, (uint8_t) ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus( DEBUG_USART, USART_FLAG_TC) == RESET)
    {
    }

    return ch;
}

int fgetc(FILE *f /*stream*/)
{
	 /* Loop until rx not empty */
    while (USART_GetFlagStatus( DEBUG_USART, USART_FLAG_RXNE) == RESET)
    {
    }
	
	return USART_ReceiveData( DEBUG_USART);
}


void HardFault_Handler()
{
	while(1) {
	;
	}
	
}

static int	Main_USB_event(int type)
{
	CMP_tips *p_tips = TIP_Get_Sington();
	if(type == et_ready)
	{
		p_tips->show_ico_tips(0, -1);
		
	}
	else if(type == et_remove)
	{
		p_tips->clear_ico_tips(0);
		
	}
	
	return 0;
}

static void 	Init_LCD(void)
{
	dev_lcd *p_lcd;
	Dev_open(DEVID_FM12864, (void *)&p_lcd);
	p_lcd->init();
	if(aci_sys.hmi_mgr.lightness == 0)
		aci_sys.hmi_mgr.lightness = DEFAULT_LIGHTNESS; 
	if(aci_sys.hmi_mgr.gray_levels == 0)
		aci_sys.hmi_mgr.gray_levels = DEFAULT_CONTRAST; 
	p_lcd->lcd_lightness(aci_sys.hmi_mgr.lightness);
	p_lcd->clear(PALLET_WHITE);
	
}

static void 	Init_device(void)
{
	dev_Char 			*p_dev_char;
	util_rtc			*rtc;

	
	
	Dev_open(DEVID_UART1, ( void *)&p_dev_char);
	
	//初始化usb的中断引脚和spi
	Dev_open(DEVID_GPIO_USB_INIT, (void *)&p_dev_char);
	Dev_open(DEVID_SPI1, (void *)&p_dev_char);
	
	
	
	rtc = Get_Rtc();
	rtc->init(rtc, NULL);
	
//	if(USB_Init(NULL) != RET_OK)
//	{
//		aci_sys.sys_flag |= SYSFLAG_ERR;
//	}
	
	Init_LCD();
}
