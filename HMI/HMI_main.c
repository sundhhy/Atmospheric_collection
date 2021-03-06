#include "HMI_main.h"
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
主菜单		<粉尘>
	设置		采样
	查询		维护
3-20 14：39：00
*/
static ro_char main_code_title[] = { "<text vx0=0 vy0=0 bkc=white>主菜单</>" };
static ro_char main_code_subtitle[] = { "<text vx0=72 vy0=0 bkc=white><粉尘></>" };

static ro_char main_code_choice_1_1[] = { "<text vx0=16 vy0=16 bkc=white>设置</>" };
static ro_char main_code_choice_1_2[] = { "<text vx0=80 vy0=16 bkc=white>采样</>" };
static ro_char main_code_choice_2_1[] = { "<text vx0=16 vy0=32 bkc=white>查询</>" };
static ro_char main_code_choice_2_2[] = { "<text vx0=80 vy0=32 bkc=white>维护</>" };


//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static HMI_main *signal_HMI_main;
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	HMN_Init( HMI *self, void *arg);
static void	HMN_Show( HMI *self);
static void HMN_Hide( HMI *self );
static void HMN_Init_sheet( HMI *self );
static void HMN_Run(HMI *self);
static void	HMN_Init_focus(HMI *self);
//static void HMI_Main_hit( HMI *self, char kcd);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
HMI_main *Get_HMI_main(void) 
{
	if( signal_HMI_main == NULL)
	{
		signal_HMI_main = HMI_main_new();
		if(signal_HMI_main  == NULL) while(1);
	}
	
	return signal_HMI_main;
}










CTOR( HMI_main)
SUPER_CTOR( HMI);
FUNCTION_SETTING( HMI.init, HMN_Init);
FUNCTION_SETTING( HMI.hide, HMN_Hide);
FUNCTION_SETTING( HMI.initSheet, HMN_Init_sheet);
FUNCTION_SETTING( HMI.hmi_run, HMN_Run);

FUNCTION_SETTING( HMI.show, HMN_Show);
//FUNCTION_SETTING( HMI.hitHandle, HMI_Main_hit);

FUNCTION_SETTING(HMI.init_focus, HMN_Init_focus);

END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int	HMN_Init( HMI *self, void *arg)
{
//	HMI 					*p_cmm;


	
//	p_cmm = CreateHMI( HMI_CMM);
//	p_cmm->init( p_cmm, NULL);
	self->flag = 0;
	//初始化界面组件

	
//	self->initSheet( self);

	
	return RET_OK;
	

}


static void HMN_Run(HMI *self)
{
	
	
	
}

static void HMN_Init_sheet( HMI *self )
{
//	HMI_main		*cthis = SUB_PTR( self, HMI, HMI_main);
	Expr 		*p_exp ;
	int 		h = 0;
	
	
	p_exp = ExpCreate( "text");
	
	p_exp->inptSht(p_exp, (void *)main_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)main_code_subtitle, arr_p_pool_shts[1]);
	p_exp->inptSht(p_exp, (void *)main_code_choice_1_1, arr_p_sht_choices[0]);
	p_exp->inptSht(p_exp, (void *)main_code_choice_1_2, arr_p_sht_choices[1]);
	p_exp->inptSht(p_exp, (void *)main_code_choice_2_1, arr_p_sht_choices[2]);
	p_exp->inptSht(p_exp, (void *)main_code_choice_2_2, arr_p_sht_choices[3]);
	
	//为可以被按键选择的图层分配ID
	arr_p_sht_choices[0]->sht_id = CMM_CHOICE_ID(0);
	arr_p_sht_choices[1]->sht_id = CMM_CHOICE_ID(1);
	arr_p_sht_choices[2]->sht_id = CMM_CHOICE_ID(2);
	arr_p_sht_choices[3]->sht_id = CMM_CHOICE_ID(3);
	arr_choice_key[0] = HMI_CONFIG;
	arr_choice_key[1] = HMI_START_SAMPLE;
	arr_choice_key[2] = HMI_QUERY;
	arr_choice_key[3] = HMI_INPUT_PSD;
	
	h = 0;
	
	
	Sheet_updown(arr_p_pool_shts[0], h++);
	Sheet_updown(arr_p_pool_shts[1], h++);
	Sheet_updown(arr_p_sht_choices[0], h++);
	Sheet_updown(arr_p_sht_choices[1], h++);
	Sheet_updown(arr_p_sht_choices[2], h++);
	Sheet_updown(arr_p_sht_choices[3], h++);
	Sheet_updown(g_p_shtTime, h++);
//	MainHmi_Init_chnShet();
	self->init_focus(self);
	
}


static void HMN_Hide(HMI *self)
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

static void	HMN_Init_focus(HMI *self) 
{
	self->p_fcuu = Focus_alloc(2, 2);

		
	Focus_Set_sht(self->p_fcuu, 0, 0, arr_p_sht_choices[0]);
	Focus_Set_sht(self->p_fcuu, 0, 1, arr_p_sht_choices[1]);
	Focus_Set_sht(self->p_fcuu, 1, 0, arr_p_sht_choices[2]);
	Focus_Set_sht(self->p_fcuu, 1, 1, arr_p_sht_choices[3]);
		
//	Focus_Set_focus(self->p_fcuu, 0, 0);
}




static void	HMN_Show( HMI *self)
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









