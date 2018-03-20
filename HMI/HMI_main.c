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

HMI *g_p_mainHmi;

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
static ro_char main_code_title[] = { "<text vx0=0 vy0=0>主设置</>" };
static ro_char main_code_subtitle[] = { "<text vx0=72 vy0=0>[粉尘]</>" };

static ro_char main_code_choice_1_1[] = { "<text vx0=16 vy0=16>设置</>" };
static ro_char main_code_choice_1_2[] = { "<text vx0=80 vy0=16>采样</>" };
static ro_char main_code_choice_2_1[] = { "<text vx0=16 vy0=32>查询</>" };
static ro_char main_code_choice_2_2[] = { "<text vx0=80 vy0=32>维护</>" };

#define MAIN_CHOICE_ID(n)			(n | SHT_ID_PRIVATE)
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static mainHmi *signal_mainHmi;
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	Init_mainHmi( HMI *self, void *arg);
static void	MainHmiShow( HMI *self);
static void MainHmiHide( HMI *self );
static void MaininitSheet( HMI *self );
static void HMI_MAIN_Run(HMI *self);
static void	Main_Init_focus(HMI *self);
static void Main_HMI_hit( HMI *self, char *s);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
mainHmi *Get_mainHmi(void) 
{
	if( signal_mainHmi == NULL)
	{
		signal_mainHmi = mainHmi_new();
		if(signal_mainHmi  == NULL) while(1);
		g_p_mainHmi = SUPER_PTR( signal_mainHmi, HMI);
	}
	
	return signal_mainHmi;
}










CTOR( mainHmi)
SUPER_CTOR( HMI);
FUNCTION_SETTING( HMI.init, Init_mainHmi);
FUNCTION_SETTING( HMI.hide, MainHmiHide);
FUNCTION_SETTING( HMI.initSheet, MaininitSheet);
FUNCTION_SETTING( HMI.hmi_run, HMI_MAIN_Run);

FUNCTION_SETTING( HMI.show, MainHmiShow);
FUNCTION_SETTING( HMI.hitHandle, Main_HMI_hit);

FUNCTION_SETTING(HMI.init_focus, Main_Init_focus);

END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int	Init_mainHmi( HMI *self, void *arg)
{
//	HMI 					*p_cmm;


	
//	p_cmm = CreateHMI( HMI_CMM);
//	p_cmm->init( p_cmm, NULL);
	self->flag = 0;
	//初始化界面组件

	
	self->initSheet( self);

	
	return RET_OK;
	

}


static void HMI_MAIN_Run(HMI *self)
{
	
	
	
}

static void MaininitSheet( HMI *self )
{
//	mainHmi		*cthis = SUB_PTR( self, HMI, mainHmi);
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
	arr_p_sht_choices[0]->id = MAIN_CHOICE_ID(0);
	arr_p_sht_choices[1]->id = MAIN_CHOICE_ID(1);
	arr_p_sht_choices[2]->id = MAIN_CHOICE_ID(2);
	arr_p_sht_choices[3]->id = MAIN_CHOICE_ID(3);
	
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


static void MainHmiHide(HMI *self)
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
	
}	

static void	Main_Init_focus(HMI *self) 
{
	self->p_fcuu = Focus_alloc(2, 2);

		
	Focus_Set_sht(self->p_fcuu, 0, 0, arr_p_sht_choices[0]);
	Focus_Set_sht(self->p_fcuu, 0, 1, arr_p_sht_choices[1]);
	Focus_Set_sht(self->p_fcuu, 1, 0, arr_p_sht_choices[2]);
	Focus_Set_sht(self->p_fcuu, 1, 1, arr_p_sht_choices[3]);
		
}




static void	MainHmiShow( HMI *self )
{

	
	Sheet_refresh(arr_p_pool_shts[0]);

}




static void	Main_HMI_hit( HMI *self, char *s)
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
	
	
	
//	if( !strcmp( s, HMIKEY_ENTER))
//	{
//		p->hit();
//	}
//	if( !strcmp( s, HMIKEY_ESC))
//	{
//		self->switchBack(self);
//	}
	
}









