#include "HMI_acquisition_setup.h"
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
采样设置
	压力				计时方式
	恒温器			其他设置
*/
static ro_char has_code_title[] = { "<text vx0=0 vy0=0>采样设置</>" };

static ro_char has_code_choice_1_1[] = { "<text vx0=0 vy0=16 bkc=white>压力</>" };
static ro_char has_code_choice_1_2[] = { "<text vx0=56 vy0=16 bkc=white>计时方式</>" };
static ro_char has_code_choice_2_1[] = { "<text vx0=0 vy0=32 bkc=white>恒温器</>" };
static ro_char has_code_choice_2_2[] = { "<text vx0=56 vy0=32 bkc=white>其他设置</>" };


//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static HMI_acquisition_setup *signal_has;
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	HAS_Init( HMI *self, void *arg);
static void	HAS_Show( HMI *self);
static void HAS_Hide( HMI *self );
static void HAS_Init_sheet( HMI *self );
static void	HAS_Init_focus(HMI *self);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
HMI_acquisition_setup *Get_HMI_acquisition_setup(void)
{
	if( signal_has == NULL)
	{
		signal_has = HMI_acquisition_setup_new();
		if(signal_has  == NULL) while(1);
	}
	
	return signal_has;
}










CTOR( HMI_acquisition_setup)
SUPER_CTOR( HMI);
FUNCTION_SETTING( HMI.init, HAS_Init);
FUNCTION_SETTING( HMI.hide, HAS_Hide);
FUNCTION_SETTING( HMI.initSheet, HAS_Init_sheet);

FUNCTION_SETTING( HMI.show, HAS_Show);


FUNCTION_SETTING(HMI.init_focus, HAS_Init_focus);

END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int	HAS_Init( HMI *self, void *arg)
{

	self->flag = 0;

//	self->initSheet( self);
	
	return RET_OK;
	

}




static void HAS_Init_sheet( HMI *self )
{
//	HMI_acquisition_setup		*cthis = SUB_PTR( self, HMI, HMI_acquisition_setup);
	Expr 		*p_exp ;
	int 		h = 0;
	
	
	p_exp = ExpCreate( "text");
	
	p_exp->inptSht(p_exp, (void *)has_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)has_code_choice_1_1, arr_p_sht_choices[0]);
	p_exp->inptSht(p_exp, (void *)has_code_choice_1_2, arr_p_sht_choices[1]);
	p_exp->inptSht(p_exp, (void *)has_code_choice_2_1, arr_p_sht_choices[2]);
	p_exp->inptSht(p_exp, (void *)has_code_choice_2_2, arr_p_sht_choices[3]);
	//为可以被按键选择的图层分配ID
	arr_p_sht_choices[0]->sht_id = CMM_CHOICE_ID(0);
	arr_p_sht_choices[1]->sht_id = CMM_CHOICE_ID(1);
	arr_p_sht_choices[2]->sht_id = CMM_CHOICE_ID(2);
	arr_p_sht_choices[3]->sht_id = CMM_CHOICE_ID(3);
	arr_choice_key[0] = HMI_CONFIG;
	arr_choice_key[1] = HMI_NONE;
	arr_choice_key[2] = HMI_NONE;
	arr_choice_key[3] = HMI_NONE;
	h = 0;
	
	
	Sheet_updown(arr_p_pool_shts[0], h++);
	Sheet_updown(arr_p_sht_choices[0], h++);
	Sheet_updown(arr_p_sht_choices[1], h++);
	Sheet_updown(arr_p_sht_choices[2], h++);
	Sheet_updown(arr_p_sht_choices[3], h++);
	Sheet_updown(g_p_shtTime, h++);
//	MainHmi_Init_chnShet();
	self->init_focus(self);
	
}


static void HAS_Hide(HMI *self)
{
	int i;
	
	Sheet_updown(g_p_shtTime, -1);
	for( i = 3; i >= 0; i--) {		
		Sheet_updown(arr_p_sht_choices[i], -1);
	}
	Sheet_updown(arr_p_pool_shts[0], -1);;
	
	self->clear_focus(self, self->p_fcuu->focus_row, self->p_fcuu->focus_col);
	Focus_free(self->p_fcuu);
	
	arr_choice_key[0] = HMI_NONE;
	arr_choice_key[1] = HMI_NONE;
	arr_choice_key[2] = HMI_NONE;
	arr_choice_key[3] = HMI_NONE;
}	

static void	HAS_Init_focus(HMI *self) 
{
	self->p_fcuu = Focus_alloc(2, 2);

		
	Focus_Set_sht(self->p_fcuu, 0, 0, arr_p_sht_choices[0]);
	Focus_Set_sht(self->p_fcuu, 0, 1, arr_p_sht_choices[1]);
	Focus_Set_sht(self->p_fcuu, 1, 0, arr_p_sht_choices[2]);
	Focus_Set_sht(self->p_fcuu, 1, 1, arr_p_sht_choices[3]);
	
	Focus_Set_focus(self->p_fcuu, 0, 0);
		
}




static void	HAS_Show( HMI *self)
{

	
	Sheet_refresh(arr_p_pool_shts[0]);
	self->show_focus(self);
}














