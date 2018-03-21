#include "HMI_config.h"
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
主菜单		<粉尘>
	设置		采样
	查询		维护
3-20 14：39：00
*/
static ro_char cfg_code_title[] = { "<text vx0=0 vy0=0>配置</>" };

static ro_char cfg_code_choice_1_1[] = { "<text vx0=32 vy0=16>仪器配置</>" };
static ro_char cfg_code_choice_1_2[] = { "<text vx0=88 vy0=16>采样设置</>" };



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

	
	self->initSheet( self);


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
	arr_choice_key[0] = HMI_INS_SETUP;
	arr_choice_key[1] = HMI_ACQ_SETUP;
	
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

		
}




static void	HMI_CFG_Show( HMI *self )
{

	
	Sheet_refresh(arr_p_pool_shts[0]);

}




static void	HMI_CFG_Hit( HMI *self, char kcd)
{

//	Button	*p = BTN_Get_Sington();

//	if( !strcmp( s, HMIKEY_UP) )
//	{

//	}
//	else if( !strcmp( s, HMIKEY_DOWN) )
//	{
//		
//	}
//	else if( !strcmp( s, HMIKEY_LEFT))
//	{
//		self->btn_backward(self);

//	}
//	else if( !strcmp( s, HMIKEY_RIGHT))
//	{

//		self->btn_forward(self);
//	}
//	
//	
//	
//	if( !strcmp( s, HMIKEY_ENTER))
//	{
//		p->hit();
////		self->btn_hit(self);
//	}
//	if( !strcmp( s, HMIKEY_ESC))
//	{
//		self->switchBack(self);
//	}
//	
}











