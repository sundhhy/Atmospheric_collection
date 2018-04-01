#include "HMI_maintain.h"
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
维护
	配置		标定
	记录		版本

*/
static ro_char mtn_code_title[] = { "<text vx0=0 vy0=0 bkc=white>维护</>" };

static ro_char mtn_code_choice_1_1[] = { "<text vx0=16 vy0=20 bkc=white>配置</>" };
static ro_char mtn_code_choice_1_2[] = { "<text vx0=80 vy0=20 bkc=white>标定</>" };
static ro_char mtn_code_choice_2_1[] = { "<text vx0=16 vy0=40 bkc=white>记录</>" };
static ro_char mtn_code_choice_2_2[] = { "<text vx0=80 vy0=40 bkc=white>版本</>" };


//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static HMI_maintain *signal_HMI_maintain;
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	HMT_Init( HMI *self, void *arg);
static void	HMT_Show( HMI *self);
static void HMT_Hide( HMI *self );
static void HMT_Init_sheet( HMI *self );
static void HMT_Run(HMI *self);
static void	HMT_Init_focus(HMI *self);
//static void HMI_Main_hit( HMI *self, char kcd);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
HMI_maintain *Get_HMI_maintain(void) 
{
	if( signal_HMI_maintain == NULL)
	{
		signal_HMI_maintain = HMI_maintain_new();
		if(signal_HMI_maintain  == NULL) while(1);
	}
	
	return signal_HMI_maintain;
}










CTOR( HMI_maintain)
SUPER_CTOR( HMI);
FUNCTION_SETTING( HMI.init, HMT_Init);
FUNCTION_SETTING( HMI.hide, HMT_Hide);
FUNCTION_SETTING( HMI.initSheet, HMT_Init_sheet);
FUNCTION_SETTING( HMI.hmi_run, HMT_Run);

FUNCTION_SETTING( HMI.show, HMT_Show);
//FUNCTION_SETTING( HMI.hitHandle, HMI_Main_hit);

FUNCTION_SETTING(HMI.init_focus, HMT_Init_focus);

END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int	HMT_Init( HMI *self, void *arg)
{

	self->flag = 0;


	
	return RET_OK;
	

}


static void HMT_Run(HMI *self)
{
	
	
	
}

static void HMT_Init_sheet( HMI *self )
{
//	HMI_maintain		*cthis = SUB_PTR( self, HMI, HMI_maintain);
	Expr 		*p_exp ;
	int 		h = 0;
	
	
	p_exp = ExpCreate( "text");
	
	p_exp->inptSht(p_exp, (void *)mtn_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)mtn_code_choice_1_1, arr_p_sht_choices[0]);
	p_exp->inptSht(p_exp, (void *)mtn_code_choice_1_2, arr_p_sht_choices[1]);
	p_exp->inptSht(p_exp, (void *)mtn_code_choice_2_1, arr_p_sht_choices[2]);
	p_exp->inptSht(p_exp, (void *)mtn_code_choice_2_2, arr_p_sht_choices[3]);
	
	//为可以被按键选择的图层分配ID
	arr_p_sht_choices[0]->sht_id = CMM_CHOICE_ID(0);
	arr_p_sht_choices[1]->sht_id = CMM_CHOICE_ID(1);
	arr_p_sht_choices[2]->sht_id = CMM_CHOICE_ID(2);
	arr_p_sht_choices[3]->sht_id = CMM_CHOICE_ID(3);
	arr_choice_key[0] = HMI_CONFIG;
	arr_choice_key[1] = HMI_START_SAMPLE;
	arr_choice_key[2] = HMI_QUERY;
	arr_choice_key[3] = HMI_NONE;
	
	h = 0;
	
	
	Sheet_updown(arr_p_pool_shts[0], h++);
	Sheet_updown(arr_p_pool_shts[1], h++);
	Sheet_updown(arr_p_sht_choices[0], h++);
	Sheet_updown(arr_p_sht_choices[1], h++);
	Sheet_updown(arr_p_sht_choices[2], h++);
	Sheet_updown(arr_p_sht_choices[3], h++);
	self->init_focus(self);
	
}


static void HMT_Hide(HMI *self)
{
	int i;
	
	Sheet_updown(g_p_shtTime, -1);
	for( i = 3; i >= 0; i--) {		
		Sheet_updown(arr_p_sht_choices[i], -1);
	}
	Sheet_updown(arr_p_pool_shts[1], -1);
	Sheet_updown(arr_p_pool_shts[0], -1);;
	
	self->clear_focus(self, self->p_fcuu->focus_row, self->p_fcuu->focus_col);
	Focus_free(self->p_fcuu);
	
	arr_choice_key[0] = HMI_NONE;
	arr_choice_key[1] = HMI_NONE;
	arr_choice_key[2] = HMI_NONE;
	arr_choice_key[3] = HMI_NONE;
}	

static void	HMT_Init_focus(HMI *self) 
{
	self->p_fcuu = Focus_alloc(2, 2);

		
	Focus_Set_sht(self->p_fcuu, 0, 0, arr_p_sht_choices[0]);
	Focus_Set_sht(self->p_fcuu, 0, 1, arr_p_sht_choices[1]);
	Focus_Set_sht(self->p_fcuu, 1, 0, arr_p_sht_choices[2]);
	Focus_Set_sht(self->p_fcuu, 1, 1, arr_p_sht_choices[3]);
		
//	Focus_Set_focus(self->p_fcuu, 0, 0);
}




static void	HMT_Show( HMI *self)
{

	
	Sheet_refresh(arr_p_pool_shts[0]);
	self->show_focus(self);
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









