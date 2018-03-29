#include "HMI_wait_sample.h"
#include "HMIFactory.h"
#include "sdhDef.h"
#include "ExpFactory.h"
#include "format.h"
#include "chnInfoPic.h"
#include "focus.h"
#include "Component_Button.h"
#include "Component_curve.h"
#include "os/os_depend.h"
#include "Component_tips.h"

#include "utils/keyboard.h"

//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define CHN_ROW			2
#define CHN_COL			3
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


/*
等待采样
	当前次数
	剩余时间：22：21
*/
static ro_char hws_code_title[] = { "<text vx0=0 vy0=0 bkc=white>等待采样</>" };
static ro_char sts_code_subtitle[] = { "<text vx0=72 vy0=0 bkc=white><粉尘></>" };
static ro_char hws_code_count_text[] = { "<text vx0=16 vy0=16 bkc=white>当前次数:</>" };
static ro_char hws_code_count_data[] = { "<text vx0=16 vy0=16 bkc=white>1</>" };
static ro_char hws_code_legt_time_text[] = { "<text vx0=80 vy0=16 bkc=white>剩余时间:</>" };
static ro_char hws_code_legt_time_data[] = { "<text vx0=80 vy0=16 bkc=white>22:21</>" };


#define STS_SING_TYPE_SHT					arr_p_pool_shts[1]
#define STS_COUNT_SHT							arr_p_pool_shts[3]
#define STS_TIME_LWFT_SHT						arr_p_sht_choices[5]


//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static HMI_wait_sample *signal_HMI_wait_sample;
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	HWS_Init( HMI *self, void *arg);
static void	HWS_Show( HMI *self);
static void HWS_Hide( HMI *self );
static void HWS_Init_sheet( HMI *self );
static void HWS_Run(HMI *self);
static void	HWS_Init_focus(HMI *self);
//static void HMI_Main_hit( HMI *self, char kcd);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
HMI_wait_sample *Get_HMI_wait_sample(void) 
{
	if( signal_HMI_wait_sample == NULL)
	{
		signal_HMI_wait_sample = HMI_wait_sample_new();
		if(signal_HMI_wait_sample  == NULL) while(1);
	}
	
	return signal_HMI_wait_sample;
}










CTOR( HMI_wait_sample)
SUPER_CTOR( HMI);
FUNCTION_SETTING( HMI.init, HWS_Init);
FUNCTION_SETTING( HMI.hide, HWS_Hide);
FUNCTION_SETTING( HMI.initSheet, HWS_Init_sheet);
FUNCTION_SETTING( HMI.hmi_run, HWS_Run);

FUNCTION_SETTING( HMI.show, HWS_Show);
//FUNCTION_SETTING( HMI.hitHandle, HMI_Main_hit);

FUNCTION_SETTING(HMI.init_focus, HWS_Init_focus);

END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int	HWS_Init( HMI *self, void *arg)
{
//	HMI 					*p_cmm;


	
//	p_cmm = CreateHMI( HMI_CMM);
//	p_cmm->init( p_cmm, NULL);
	self->flag = 0;
	//初始化界面组件

	
//	self->initSheet( self);

	
	return RET_OK;
	

}


static void HWS_Run(HMI *self)
{
	
	
	
}

static void HWS_Init_sheet( HMI *self )
{
//	HMI_wait_sample		*cthis = SUB_PTR( self, HMI, HMI_wait_sample);
	Expr 		*p_exp ;
	int 		h = 0;
	
	


	
	
	
	p_exp = ExpCreate( "text");
	
	
	
	p_exp->inptSht(p_exp, (void *)hws_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)sts_code_subtitle, arr_p_pool_shts[1]);

	
	
	p_exp->inptSht(p_exp, (void *)hws_code_count_text, arr_p_pool_shts[2]);
	p_exp->inptSht(p_exp, (void *)hws_code_count_data, arr_p_pool_shts[3]);
	p_exp->inptSht(p_exp, (void *)hws_code_legt_time_text, arr_p_pool_shts[4]);
	p_exp->inptSht(p_exp, (void *)hws_code_legt_time_data, arr_p_pool_shts[5]);

	


	
	h = 0;
	
	
	Sheet_updown(arr_p_pool_shts[0], h++);
	Sheet_updown(arr_p_pool_shts[1], h++);
	Sheet_updown(arr_p_pool_shts[2], h++);
	Sheet_updown(arr_p_pool_shts[3], h++);
	Sheet_updown(arr_p_pool_shts[4], h++);
	Sheet_updown(arr_p_pool_shts[5], h++);


	
}


static void HWS_Hide(HMI *self)
{
	int i;
	
	for( i = 5; i >= 0; i--) {		
		Sheet_updown(arr_p_sht_choices[i], -1);
	}
	
	
	
	
}	

static void	HWS_Init_focus(HMI *self) 
{
	
}




static void	HWS_Show( HMI *self)
{

	
	
}




//static void	HMI_Main_hit( HMI *self, char kcd)
//{
//	sheet *p_sht;
//	switch(kcd)
//	{
//		case KEYCODE_LEFT:
//			Focus_move_left(self->p_fcuu);
//			break;
//		case KEYCODE_RIGHT:
//			Focus_move_left(self->p_fcuu);
//			break;
//		case KEYCODE_UP:
//			Focus_move_up(self->p_fcuu);
//			break;
//		case KEYCODE_DOWN:
//			Focus_move_down(self->p_fcuu);
//			break;
//		case KEYCODE_SWITCH:
//			//大气A/B 粉尘之间的切换
//			break;
//		case KEYCODE_ENTER:
//			p_sht = Focus_Get_focus(self->p_fcuu);
//			if(p_sht)
//				HMI_choice(self, p_sht->sht_id);
//			break;		
//		case KEYCODE_ESC:
//			
//		
//			break;	
//		
//	}

//	
//	
//}









