//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions


#include "configs/hardware_conf.h"
#include "configs/tdd_conf.h"
#include "BSP/mySystemClock.h"
#include "BSP/pinmux.h"
#include "BSP/power.h"
#include "BSP/intrInit.h"

#include "unit_tests/unit_test.h"
#include "device.h"
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

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------

static void Init_device(void);
static void Init_LCD(void);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS

  // initialize peripherals here
	OpenPrpClock();
	Pin_init();
	NVIC_Configuration();
	PVD_Init();
	
	Init_device();
	

	#if TDD_ON == 1
	unit_test();
	#endif
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);

  osKernelStart ();                         // start thread execution 
	
	while(1)
	{
		
		
	}
}
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{
static void 	Init_LCD(void)
{
	dev_lcd *p_lcd;
	Dev_open(DEVID_FM12864, (void *)&p_lcd);
	p_lcd->init();

}

static void 	Init_device(void)
{
//	dev_Char	*p_dc;
	
//	p_dc = 
	
	Init_LCD();
}
