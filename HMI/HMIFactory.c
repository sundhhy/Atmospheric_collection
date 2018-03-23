#include "HMIFactory.h"
#include "HMI_comm.h"
#include "HMI_main.h"
#include "HMI_config.h"
#include "HMI_instrument_setup.h"
#include "HMI_acquisition_setup.h"
#include "HMI_select_setting.h"

//�ṩ �������¼�����Ϣ�����ڣ�������ʱ�䣬��ѡ���ͼ��
//��Щͼ����ܻᱻ����������ʹ��
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
			p_hmi = SUPER_PTR(Get_mainHmi(), HMI);
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
		case HMI_INS_SETUP:
			
			p_hmi = SUPER_PTR(Get_HMI_instrument_setup(), HMI);
			break;
		case HMI_ACQ_SETUP:
			p_hmi = SUPER_PTR(Get_HMI_acquisition_setup(), HMI);
			break;
//		case HMI_DATA:
//			p_hmi = SUPER_PTR(Get_dataHMI(), HMI);
//			break;
//		case HMI_NWS:
//			p_hmi = SUPER_PTR(Get_NewSlct_HMI(), HMI);
//			break;
//		case HMI_ACCM:
//			p_hmi = SUPER_PTR(Get_Accm_HMI(), HMI);
//			break;
//		case HMI_SETUP:
//			p_hmi = SUPER_PTR(Get_Setup_HMI(), HMI);
//			break;
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


















