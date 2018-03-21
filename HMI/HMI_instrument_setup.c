#include "HMI_instrument_setup.h"
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
仪器设置
	时间
	显示
	密码修改
*/
static ro_char his_code_title[] = { "<text vx0=0 vy0=0>仪器设置</>" };

static ro_char his_code_choice_1_1[] = { "<text vx0=16 vy0=16>时间</>" };
static ro_char his_code_choice_1_2[] = { "<text vx0=16 vy0=32>显示</>" };
static ro_char his_code_choice_1_3[] = { "<text vx0=16 vy0=48>密码修改</>" };


//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static HMI_instrument_setup *signal_his;
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	HIS_Init( HMI *self, void *arg);
static void	HIS_Show( HMI *self);
static void HIS_Hide( HMI *self );
static void HIS_Init_sheet( HMI *self );
static void	HIS_Init_focus(HMI *self);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
HMI_instrument_setup *Get_HMI_instrument_setup(void) 
{
	if( signal_his == NULL)
	{
		signal_his = HMI_instrument_setup_new();
		if(signal_his  == NULL) while(1);
	}
	
	return signal_his;
}










CTOR( HMI_instrument_setup)
SUPER_CTOR( HMI);
FUNCTION_SETTING( HMI.init, HIS_Init);
FUNCTION_SETTING( HMI.hide, HIS_Hide);
FUNCTION_SETTING( HMI.initSheet, HIS_Init_sheet);

FUNCTION_SETTING( HMI.show, HIS_Show);


FUNCTION_SETTING(HMI.init_focus, HIS_Init_focus);

END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int	HIS_Init( HMI *self, void *arg)
{



	self->flag = 0;

//	self->initSheet( self);
	
	return RET_OK;
	

}




static void HIS_Init_sheet( HMI *self )
{
//	HMI_instrument_setup		*cthis = SUB_PTR( self, HMI, HMI_instrument_setup);
	Expr 		*p_exp ;
	int 		h = 0;
	
	
	p_exp = ExpCreate( "text");
	
	p_exp->inptSht(p_exp, (void *)his_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)his_code_choice_1_1, arr_p_sht_choices[0]);
	p_exp->inptSht(p_exp, (void *)his_code_choice_1_2, arr_p_sht_choices[1]);
	p_exp->inptSht(p_exp, (void *)his_code_choice_1_3, arr_p_sht_choices[2]);
	
	//为可以被按键选择的图层分配ID
	arr_p_sht_choices[0]->sht_id = CMM_CHOICE_ID(0);
	arr_p_sht_choices[1]->sht_id = CMM_CHOICE_ID(1);
	arr_p_sht_choices[2]->sht_id = CMM_CHOICE_ID(2);
	arr_choice_key[0] = HMI_CONFIG;
	arr_choice_key[1] = HMI_NONE;
	arr_choice_key[2] = HMI_NONE;
	
	h = 0;
	
	
	Sheet_updown(arr_p_pool_shts[0], h++);
	Sheet_updown(arr_p_sht_choices[0], h++);
	Sheet_updown(arr_p_sht_choices[1], h++);
	Sheet_updown(arr_p_sht_choices[2], h++);
	Sheet_updown(g_p_shtTime, h++);
//	MainHmi_Init_chnShet();
	self->init_focus(self);
	
}


static void HIS_Hide(HMI *self)
{
	int i;
	
	Sheet_updown(g_p_shtTime, -1);
	for( i = 2; i >= 0; i--) {		
		Sheet_updown(arr_p_sht_choices[i], -1);
	}
	Sheet_updown(arr_p_pool_shts[0], -1);;
	
	self->clear_focus(self, self->p_fcuu->focus_row, self->p_fcuu->focus_col);
	Focus_free(self->p_fcuu);
	
	arr_choice_key[0] = HMI_NONE;
	arr_choice_key[1] = HMI_NONE;
	arr_choice_key[2] = HMI_NONE;
}	

static void	HIS_Init_focus(HMI *self) 
{
	self->p_fcuu = Focus_alloc(1, 3);

		
	Focus_Set_sht(self->p_fcuu, 0, 0, arr_p_sht_choices[0]);
	Focus_Set_sht(self->p_fcuu, 0, 1, arr_p_sht_choices[1]);
	Focus_Set_sht(self->p_fcuu, 0, 2, arr_p_sht_choices[2]);
		
}




static void	HIS_Show( HMI *self)
{

	
	Sheet_refresh(arr_p_pool_shts[0]);
	self->show_focus(self);
}














