#include "HMI_config.h"
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

#include "HMIFactory.h"
#include "Component_option.h"

//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

/*
设置入口画面，
会向设置选择画面传递选项类型来确定切换的画面的功能

*/
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
配置
		仪器配置
		采样设置
*/
static ro_char cfg_code_title[] = { "<text vx0=0 vy0=0>配置</>" };

static ro_char cfg_code_choice_1_1[] = { "<text vx0=32 vy0=16>仪器配置</>" };
static ro_char cfg_code_choice_1_2[] = { "<text vx0=32 vy0=32>采样设置</>" };



//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static hmi_config *signal_him_config;
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	HMI_CFG_Init( HMI *self, void *arg);
static void	HMI_CFG_Show( HMI *self);
static void HMI_CFG_Hide( HMI *self );
static void HMI_CFG_Init_sheet( HMI *self );
static void HMI_CFG_Run(HMI *self);
static void	HMI_CFG_Init_focus(HMI *self);
static void HMI_CFG_Hit( HMI *self, char kcd);


static void HMI_CFG_choice(HMI *self, uint8_t choice_id);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
hmi_config *Get_hmi_config(void) 
{
	if(signal_him_config == NULL)
	{
		signal_him_config = hmi_config_new();
		if(signal_him_config  == NULL) while(1);	//故意卡死，方便出错后调试
	}
	
	return signal_him_config;
}










CTOR(hmi_config)
SUPER_CTOR(HMI);
FUNCTION_SETTING(HMI.init, HMI_CFG_Init);
FUNCTION_SETTING(HMI.hide, HMI_CFG_Hide);
FUNCTION_SETTING(HMI.initSheet, HMI_CFG_Init_sheet);
FUNCTION_SETTING(HMI.hmi_run, HMI_CFG_Run);

FUNCTION_SETTING(HMI.show, HMI_CFG_Show);
FUNCTION_SETTING(HMI.hitHandle, HMI_CFG_Hit);

FUNCTION_SETTING(HMI.init_focus, HMI_CFG_Init_focus);

END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int	HMI_CFG_Init( HMI *self, void *arg)
{




	self->flag = 0;

	


	return RET_OK;
	

}


static void HMI_CFG_Run(HMI *self)
{
	
	
	
}

static void HMI_CFG_Init_sheet( HMI *self )
{
//	hmi_config		*cthis = SUB_PTR( self, HMI, hmi_config);
	Expr 		*p_exp ;
	int 		h = 0;
	
	
	p_exp = ExpCreate( "text");
	
	p_exp->inptSht(p_exp, (void *)cfg_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)cfg_code_choice_1_1, arr_p_sht_choices[0]);
	p_exp->inptSht(p_exp, (void *)cfg_code_choice_1_2, arr_p_sht_choices[1]);

	
	//为可以被按键选择的图层分配ID
	arr_p_sht_choices[0]->sht_id = CMM_CHOICE_ID(0);
	arr_p_sht_choices[1]->sht_id = CMM_CHOICE_ID(1);

	
	h = 0;
	
	
	Sheet_updown(arr_p_pool_shts[0], h++);

	Sheet_updown(arr_p_sht_choices[0], h++);
	Sheet_updown(arr_p_sht_choices[1], h++);

	Sheet_updown(g_p_shtTime, h++);
//	MainHmi_Init_chnShet();
	self->init_focus(self);
	
}


static void HMI_CFG_Hide(HMI *self)
{

	
	Sheet_updown(g_p_shtTime, -1);
		
	Sheet_updown(arr_p_sht_choices[1], -1);
	Sheet_updown(arr_p_sht_choices[0], -1);
	Sheet_updown(arr_p_pool_shts[0], -1);
	
	self->clear_focus(self, self->p_fcuu->focus_row, self->p_fcuu->focus_col);
	Focus_free(self->p_fcuu);
	
}	

static void	HMI_CFG_Init_focus(HMI *self) 
{
	self->p_fcuu = Focus_alloc(2, 1);

		
	Focus_Set_sht(self->p_fcuu, 0, 0, arr_p_sht_choices[0]);
	Focus_Set_sht(self->p_fcuu, 1, 0, arr_p_sht_choices[1]);
//	Focus_Set_focus(self->p_fcuu, 0, 0);
		
}




static void	HMI_CFG_Show( HMI *self )
{

	
	Sheet_refresh(arr_p_pool_shts[0]);
	self->show_focus(self);
}




static void	HMI_CFG_Hit( HMI *self, char kcd)
{
	sheet *p_sht;
	char	change = 1;
	char	old_fcuu_row;
	char	old_fcuu_col;
	
	old_fcuu_row = self->p_fcuu->focus_row;
	old_fcuu_col = self->p_fcuu->focus_col;
	switch(kcd)
	{

		case KEYCODE_UP:
			Focus_move_up(self->p_fcuu);
			break;
		case KEYCODE_DOWN:
			Focus_move_down(self->p_fcuu);
			break;
		case KEYCODE_ENTER:
			
			change = 0;
			p_sht = Focus_Get_focus(self->p_fcuu);
			if(p_sht)
				HMI_CFG_choice(self, p_sht->sht_id);
			break;
		case KEYCODE_ESC:
			change = 0;
			self->switchBack(self);
			break;
		default:
			change = 0;
		
			break;	
		
	}
	
	if(change)
	{
		
		self->clear_focus(self, old_fcuu_row, old_fcuu_col);
		self->show_focus(self);
	}
	
}


//切换界面的一般处理方法
static void HMI_CFG_choice(HMI *self, uint8_t choice_id)
{
	HMI 				*p_target;
	
	if(CMM_IS_LEGAL_CID(choice_id) == 0)
		return;
	p_target = CreateHMI(HMI_SELECT_SETTING);
	
	if(p_target == NULL)
		return;
	
	if(choice_id == CMM_CHOICE_ID(0))
		p_target->arg[0] = OPTION_INSTRUMENT_SETUP;
	else
		p_target->arg[0] = OPTION_ACQUISITION_SETUP;
	
	self->switchHMI(self, p_target);

}	











