#include "HMI_select_setting.h"
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
#include "Component_option.h"

//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
/*

设置选择界面，提供设置项的显示及按键操作功能。
通过向该界面的hmi->arg[0]的选项类型来获得要显示的图层
本界面主要处理选项的按键动作
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

#define SELECT_HISTORY_NUM			8		//记录8个历史

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static HMI_select_setting *signal_his;

static uint8_t		arr_select_history[SELECT_HISTORY_NUM];
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	HSS_Init( HMI *self, void *arg);
static void	HSS_Show( HMI *self);
static void HSS_Hide( HMI *self );
static void HSS_Init_sheet( HMI *self );
static void	HSS_Init_focus(HMI *self);

static void HSS_Hit( HMI *self, char kcd);


static void HSS_Push_type(uint8_t t);
static uint8_t HSS_Pop_type(void);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
HMI_select_setting *Get_HMI_select_setting(void) 
{
	if( signal_his == NULL)
	{
		signal_his = HMI_select_setting_new();
		if(signal_his  == NULL) while(1);
	}
	
	return signal_his;
}




CTOR( HMI_select_setting)
SUPER_CTOR( HMI);
FUNCTION_SETTING( HMI.init, HSS_Init);
FUNCTION_SETTING( HMI.hide, HSS_Hide);
FUNCTION_SETTING( HMI.initSheet, HSS_Init_sheet);

FUNCTION_SETTING( HMI.show, HSS_Show);
FUNCTION_SETTING( HMI.hitHandle, HSS_Hit);


FUNCTION_SETTING(HMI.init_focus, HSS_Init_focus);

END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int	HSS_Init( HMI *self, void *arg)
{


	int i;
	
	for(i = 0; i < SELECT_HISTORY_NUM; i++)
	{
		arr_select_history[i] = 0xff;
		
	}
	
	self->flag = 0;

	
	return RET_OK;
	

}




static void HSS_Init_sheet( HMI *self )
{
	short h;
	char i,j;
	cmp_options_t *p_opt = CMP_Get_option(self->arg[0]);
	
	if(p_opt == NULL)
	{
		self->flag |= HMI_FLAG_ERR;
		return;
		
	}
	CMP_OPT_Init(p_opt);
	h = 0;
	Sheet_updown(p_opt->p_titl, h++);
	for(i = 0; i < p_opt->num_row; i++)
	{
		for(j = 0; j < p_opt->num_col; j++)
		{
			
			Sheet_updown(p_opt->p_optionals[i][j], h++);
			p_opt->p_optionals[i][j]->sht_id = OPTION_SHEET_ID(i,j);
		}
	}
	
	self->init_focus(self);
	
}


static void HSS_Hide(HMI *self)
{
	short i,j;
	
	cmp_options_t *p_opt;
	
	
	if(self->flag & HMI_FLAG_ONCE_USE_ARG_1)
	{
		p_opt = CMP_Get_option(self->arg[1]);
		self->flag &=~ HMI_FLAG_ONCE_USE_ARG_1;
		
		
	}
	else
	{
		p_opt = CMP_Get_option(self->arg[0]);
		
	}
		

	if(p_opt == NULL)
	{
		self->flag |= HMI_FLAG_ERR;
		return;
		
	}
	
	for(i = p_opt->num_row - 1; i >= 0; i--)
	{
		for(j = p_opt->num_col - 1; j >= 0; j --)
		{
			
			Sheet_updown(p_opt->p_optionals[i][j], -1);
			
		}
	}
	Sheet_updown(p_opt->p_titl, -1);
	
	self->clear_focus(self, self->p_fcuu->focus_row, self->p_fcuu->focus_col);
	Focus_free(self->p_fcuu);
	self->p_fcuu = NULL;

}	

static void	HSS_Init_focus(HMI *self) 
{
	short i, j;
	cmp_options_t *p_opt = CMP_Get_option(self->arg[0]);
	
	


	
	self->p_fcuu = Focus_alloc(p_opt->num_row, p_opt->num_col);
	

	
	for(i = 0; i < p_opt->num_row; i++)
	{
		for(j = 0; j < p_opt->num_col; j++)
		{
			Focus_Set_sht(self->p_fcuu, i, j, p_opt->p_optionals[i][j]);
			
		}
	}	
	
	Focus_Set_focus(self->p_fcuu, 0, 0);
}




static void	HSS_Show( HMI *self)
{
	cmp_options_t *p_opt = CMP_Get_option(self->arg[0]);
	
	Sheet_refresh(p_opt->p_titl);
	self->show_focus(self);
}

static void HSS_Hit( HMI *self, char kcd)
{
	sheet 					*p_sht;
	HMI						*p_configure;
	cmp_options_t 			*p_opt = CMP_Get_option(self->arg[0]);
	int						ret = 0;
	char					change = 1;
	char					old_fcuu_row;
	char					old_fcuu_col;
	uint8_t					last_type = 0xff;
	
	old_fcuu_row = self->p_fcuu->focus_row;
	old_fcuu_col = self->p_fcuu->focus_col;
	
	if(p_opt == NULL)
		return;
	
	switch(kcd)
	{
		case KEYCODE_LEFT:
			Focus_move_left(self->p_fcuu);
			break;
		case KEYCODE_RIGHT:
			Focus_move_left(self->p_fcuu);
			break;
		case KEYCODE_UP:
			Focus_move_up(self->p_fcuu);
			break;
		case KEYCODE_DOWN:
			Focus_move_down(self->p_fcuu);
			break;
		case KEYCODE_ENTER:
			change = 0;
			p_sht = Focus_Get_focus(self->p_fcuu);
			if(p_sht == NULL)
				break;
			
			ret = CMP_OPT_Select(p_opt, p_sht->sht_id);
			if(ret < 0)
				break;
			
			//设置界面被修改，就重现加载设置项
			if(IS_SETUP(ret))
			{
				p_opt = CMP_Get_option(GET_SELECT_DATA(ret));
				if(p_opt == NULL)
					break;
				self->flag |= HMI_FLAG_ONCE_USE_ARG_1;
				self->arg[1] = self->arg[0];		//这是为了在重新显示的时候，在清除上一个选项时使用
				HSS_Push_type(self->arg[1]);		//记录下来，用于返回时使用
				
				
				self->arg[0] = GET_SELECT_DATA(ret);		//更新设置项类型
				
				self->switchHMI(self, self);
				
			}
			//是配置界面的话，要切换到配置界面上去
			else if(IS_CONFIG(ret))
			{
				p_configure = CreateHMI(HMI_CONFIGURE);
				p_configure->arg[0] =  GET_SELECT_DATA(ret);
				
				self->switchHMI(self, p_configure);
			}
			
			break;		
		case KEYCODE_ESC:
			change = 0;
			last_type = HSS_Pop_type();
			if(last_type == 0xff)
				self->switchBack(self);
			else
			{
				//切换到上一次
				self->flag |= HMI_FLAG_ONCE_USE_ARG_1;
				self->arg[1] = self->arg[0];		//这是为了在重新显示的时候，在清除上一个选项时使用
				
				
				self->arg[0] = last_type;		//更新设置项类型
				
				self->switchHMI(self, self);
				
			}
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


static void HSS_Push_type(uint8_t t)
{
	int i = 0;
	
	while(i < SELECT_HISTORY_NUM)
	{
		
		if(arr_select_history[i] == 0xff)
			break;
		i ++;
	}
	
	if(i == SELECT_HISTORY_NUM)
		i = SELECT_HISTORY_NUM - 1;
	
	arr_select_history[i] = t;
}
static uint8_t HSS_Pop_type(void)
{
	uint8_t r = 0xff;
	int i = 0;
	while(i < SELECT_HISTORY_NUM)
	{
		
		if(arr_select_history[i] == 0xff)
			break;
		i ++;
	}
	
	
	
	if(i > 0)
	{
		i --;
		r = arr_select_history[i];
		
		arr_select_history[i] = 0xff;
	}
	
	return r;
	
}









