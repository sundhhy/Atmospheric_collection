#include "HMIFactory.h"
#include "HMI_comm.h"
#include "HMI_main.h"
#include "HMI_config.h"
#include "HMI_instrument_setup.h"
#include "HMI_acquisition_setup.h"
#include "HMI_select_setting.h"
#include "HMI_configuring.h"
#include "HMI_query.h"
#include "HMI_start_sample.h"
#include "HMI_wait_sample.h"
#include "HMI_maintain.h"
#include "HMI_input_psd.h"
//提供 按键，事件，消息，窗口，报警，时间，复选框的图层
//这些图层可能会被其他界面所使用
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

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
HMI *CreateHMI(char hmi_type)
{
	HMI *p_hmi = NULL;
	switch( hmi_type)
	{
		case HMI_MAIN:
			p_hmi = SUPER_PTR(Get_HMI_main(), HMI);
			break; 
		case HMI_CMM:
			
			p_hmi = SUPER_PTR(GetCmmHMI(), HMI);
			break; 
		case HMI_CONFIG:
			
			p_hmi = SUPER_PTR(Get_hmi_config(), HMI);
			break;
		case HMI_SELECT_SETTING:
			
			p_hmi = SUPER_PTR(Get_HMI_select_setting(), HMI);
			break;
		case HMI_CONFIGURE:
			
			p_hmi = SUPER_PTR(Get_HMI_configuring(), HMI);
			break;
		case HMI_QUERY:
			p_hmi = SUPER_PTR(Get_HMI_query(), HMI);
			break;
		case HMI_START_SAMPLE:
			p_hmi = SUPER_PTR(Get_HMI_start_sample(), HMI);
			break;
		case HMI_WAIT_SAMPLE:
			p_hmi = SUPER_PTR(Get_HMI_wait_sample(), HMI);
			break;
		case HMI_MAINTAIN:
			p_hmi = SUPER_PTR(Get_HMI_maintain(), HMI);
			break;
		case HMI_INPUT_PSD:
			p_hmi = SUPER_PTR(Get_HMI_input_psd(), HMI);
			break;
//		case HMI_SETTING:
//			p_hmi = SUPER_PTR(Get_Setting_HMI(), HMI);
//			break;
//		case HMI_WINDOWS:
//			p_hmi = SUPER_PTR(Get_winHmi(), HMI);
//			break;
		default:
			break;
	}
	
	return p_hmi;
}

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//


















