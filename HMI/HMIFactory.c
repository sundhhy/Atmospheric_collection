#include "HMIFactory.h"

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
HMI *CreateHMI( int hmiType)
{
	HMI *p_hmi = NULL;
	switch( hmiType)
	{
		case HMI_MAIN:
			p_hmi = SUPER_PTR(Get_mainHmi(), HMI);
			break; 
		case HMI_CMM:
			
			p_hmi = SUPER_PTR(GetCmmHMI(), HMI);
			break; 
		case HMI_KYBRD:
			
			p_hmi = SUPER_PTR(GetkeyboardHMI(), HMI);
			break;
		case HMI_MENU:
			
			p_hmi = SUPER_PTR(GetmenuHMI(), HMI);
			break;
		case HMI_BAR:
			p_hmi = SUPER_PTR(Get_barGhHMI(), HMI);
			break;
		case HMI_DATA:
			p_hmi = SUPER_PTR(Get_dataHMI(), HMI);
			break;
		case HMI_RLT_TREND:
			p_hmi = SUPER_PTR(Get_RLT_trendHMI(), HMI);
			break;
		case HMI_NWS:
			p_hmi = SUPER_PTR(Get_NewSlct_HMI(), HMI);
			break;
//		case HMI_NEWS_ALARM:
//			p_hmi = SUPER_PTR(Get_News_Alarm_HMI(), HMI);
//			break;
//		case HMI_NEWS_POWER_DOWN:
//			p_hmi = SUPER_PTR(Get_PwrDn_HMI(), HMI);
//			break;
//		case HMI_HISTORY:
//			p_hmi = SUPER_PTR(Get_History_HMI(), HMI);
//			break;
		case HMI_ACCM:
			p_hmi = SUPER_PTR(Get_Accm_HMI(), HMI);
			break;
		case HMI_SETUP:
			p_hmi = SUPER_PTR(Get_Setup_HMI(), HMI);
			break;
		case HMI_SETTING:
			p_hmi = SUPER_PTR(Get_Setting_HMI(), HMI);
			break;
		case HMI_WINDOWS:
			p_hmi = SUPER_PTR(Get_winHmi(), HMI);
			break;
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


















