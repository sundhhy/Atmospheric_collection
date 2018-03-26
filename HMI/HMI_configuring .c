#include "HMI_configuring.h"
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
#include "hmi_conf.h"


#include "configure_strategy.h"

//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
/*
配置配置项的画面
可以使用不同的配置策略，来实现不同的配置功能


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
仪器设置
	时间
	显示
	密码修改
*/
static ro_char hst_code_title[] = { "<text vx0=0 vy0=0> </>" };
static ro_char hst_code_page[] = { "<text vx0=88 vy0=0>[1/4]</>" };		//显示页码
static ro_char hst_code_text[] = { "<text vx0=0 vy0=16> </>" };

#define CNF_PAGE_SHT			1
#define CNF_TEXT_SHT			2

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static HMI_configuring *signal_cnf;
static strategy_t *p_cfg_stg;
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	CNF_Init( HMI *self, void *arg);
static void	CNF_Show( HMI *self);
static void CNF_Hide( HMI *self );
static void CNF_Init_sheet( HMI *self );
static void	CNF_Init_focus(HMI *self);
static void	CNF_Clear_focus(HMI *self, uint8_t fouse_row, uint8_t fouse_col);
static void	CNF_Show_focus( HMI *self);
static void	CNF_Hit( HMI *self, char kcd);
static void	CNF_Long_hit( HMI *self, char kcd);


static void CNF_Show_strategy(void);
static void CNF_Turn_page(HMI *self, int up_or_dn);
//设置画面的焦点操作，都转到策略类的焦点操作
static void CNF_Show_strategy_focus(strategy_focus_t *p_syf, int opt);

static int CNF_Exec_Sy_cmd(void *p_rcv, int cmd,  void *arg);
static uint8_t	CNF_Get_row_vy(uint8_t row);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
HMI_configuring *Get_HMI_configuring(void)
{
	if( signal_cnf == NULL)
	{
		signal_cnf = HMI_configuring_new();
		if(signal_cnf  == NULL) while(1);
	}
	
	return signal_cnf;
}










CTOR( HMI_configuring)
SUPER_CTOR( HMI);
FUNCTION_SETTING( HMI.init, CNF_Init);
FUNCTION_SETTING( HMI.hide, CNF_Hide);
FUNCTION_SETTING( HMI.initSheet, CNF_Init_sheet);
FUNCTION_SETTING( HMI.show, CNF_Show);

FUNCTION_SETTING( HMI.hitHandle, CNF_Hit);
FUNCTION_SETTING( HMI.longpushHandle, CNF_Long_hit);


FUNCTION_SETTING(HMI.init_focus, CNF_Init_focus);
FUNCTION_SETTING(HMI.clear_focus, CNF_Clear_focus);
FUNCTION_SETTING(HMI.show_focus, CNF_Show_focus);
END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int	CNF_Init( HMI *self, void *arg)
{



	self->flag = 0;

	
	return RET_OK;
	

}




static void CNF_Init_sheet( HMI *self )
{
	HMI_configuring		*cthis = SUB_PTR( self, HMI, HMI_configuring);
	strategy_t			*p_stg;
	Expr 		*p_exp ;
	int 		h = 0;
	
	
	p_exp = ExpCreate( "text");
	
	p_exp->inptSht(p_exp, (void *)hst_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)hst_code_page, arr_p_pool_shts[CNF_PAGE_SHT]);
	p_exp->inptSht(p_exp, (void *)hst_code_text, arr_p_pool_shts[CNF_TEXT_SHT]);
	arr_p_pool_shts[CNF_PAGE_SHT]->cnt.data = cthis->page_buf;
	

	
	h = 0;
	p_stg = Get_strategy(self->arg[0]);
	if(p_stg == NULL)
	{
		self->flag |= HMI_FLAG_ERR;
		return;
		
	}
	p_stg->cmd_hdl = CNF_Exec_Sy_cmd;
	p_stg->p_cmd_rcv = self;
	
	p_stg->init(NULL);
	p_cfg_stg = p_stg;
	if(p_stg->p_stg_title)
	{
		arr_p_pool_shts[0]->cnt.data = p_stg->p_stg_title;
		arr_p_pool_shts[0]->cnt.len = strlen(p_stg->p_stg_title);
		Sheet_updown(arr_p_pool_shts[0], h++);
		
	}
//	Sheet_updown(arr_p_pool_shts[1], h++);

	self->init_focus(self);
	
}


static void CNF_Hide(HMI *self)
{
	

	Sheet_updown(arr_p_pool_shts[0], -1);;
	
	self->clear_focus(self, self->p_fcuu->focus_row, self->p_fcuu->focus_col);
	
	p_cfg_stg = NULL;

}	

static void	CNF_Show( HMI *self)
{

	
	Sheet_refresh(arr_p_pool_shts[0]);
	
	CNF_Show_strategy();
	self->show_focus(self);
}


static void	CNF_Init_focus(HMI *self) 
{
	
	if(p_cfg_stg)
		p_cfg_stg->reset_focus();
}

//设置界面中的焦点操作，只是占位而已
static void		CNF_Clear_focus(HMI *self, uint8_t fouse_row, uint8_t fouse_col)
{
	
}



static void		CNF_Show_focus(HMI *self)
{
	strategy_t				*p_sy = p_cfg_stg;
	
	CNF_Show_strategy_focus(&p_sy->sf, 1);
	
}


static void CNF_Show_strategy_focus(strategy_focus_t *p_syf, int opt)
{
	HMI_configuring				*cthis = signal_cnf;
	int 							f_data_len = 0;
	sheet 						*p_sht_text = arr_p_pool_shts[CNF_TEXT_SHT];
	uint16_t					txt_xsize, txt_ysize;
	char						str_clean[16];
	
	if(p_cfg_stg == NULL)
		return;
	
	
	p_sht_text->p_gp->getSize(p_sht_text->p_gp, p_sht_text->cnt.font, &txt_xsize, &txt_ysize);
	
	//获得焦点位于的内容与位置
	f_data_len = p_cfg_stg->get_focus_data(&p_sht_text->cnt.data, p_syf);
	
	//焦点可能只是字符串的一部分，因此要进行偏移
	p_sht_text->area.x0 = cthis->col_vx0[p_syf->f_col];
	p_sht_text->area.y0 = CNF_Get_row_vy(p_syf->f_row - cthis->entry_start_row);
	//显示
	
	
	if(opt == 0)
	{
		//显示全部数据
		p_sht_text->cnt.len = f_data_len;
		
		
		
	}
	if(opt == 1)
	{
		
		
		
		//根据焦点的位置计算在屏幕上的坐标
		
		
		
		//焦点可能只是字符串的一部分，因此要进行偏移
		p_sht_text->cnt.len = p_syf->num_byte;
		p_sht_text->cnt.data += p_syf->start_byte;
		p_sht_text->area.x0 +=  txt_xsize * p_syf->start_byte;
		p_sht_text->cnt.effects = GP_SET_EFF(p_sht_text->cnt.effects, EFF_FOCUS);
		
	}
	
	if(opt == 2)		
	{
		//清除内容
		memset(str_clean, ' ', sizeof(str_clean));
		p_sht_text->cnt.data = str_clean;
		p_sht_text->cnt.len = sizeof(str_clean);
		
		
	}
	
	p_sht_text->p_gp->vdraw(p_sht_text->p_gp, &p_sht_text->cnt, &p_sht_text->area);
	p_sht_text->cnt.effects = GP_CLR_EFF(p_sht_text->cnt.effects, EFF_FOCUS);
	
	
}



static void CNF_Show_strategy(void)
{

	HMI_configuring		*cthis = signal_cnf;
	sheet 				*p_sht_text = arr_p_pool_shts[CNF_TEXT_SHT];
	uint8_t	row = 0;
	uint8_t	col = 0;

	uint8_t	text_len = 0;
	uint8_t	col_maxlen = 0;			//某一列的列宽，取最大的那个列宽
	
	uint16_t	txt_xsize = 0;
	uint16_t	txt_ysize = 0;
	uint8_t		col_vx0 = 0;
	uint8_t		col_space = 1;				//列间距：单位 字符的宽度
	
		
	if(p_cfg_stg == NULL)
		return;
	

	
	p_sht_text->p_gp->getSize(p_sht_text->p_gp, p_sht_text->cnt.font, &txt_xsize, &txt_ysize);
	
	//按列显示，这是为了能够获得前一列的最大宽度，来决定下一列的x轴位置
	for(col = 0; col < SETTING_COL_MAX; col ++) {
		
		//把每一列的起始x轴位置计算并记录下来
		col_vx0 += col_maxlen * txt_xsize;
		cthis->col_vx0[col] = col_vx0;
		
		
		//显示一页中所有的行
		col_maxlen = 0;
		
		
		for(row = 0; row < SETTING_ROW_MAX; row ++) {
			
			text_len = p_cfg_stg->entry_txt(row + cthis->entry_start_row, col, &p_sht_text->cnt.data);
			if(text_len == 0)
				continue;
			p_sht_text->cnt.len = text_len;
			if(col_maxlen < text_len)
				col_maxlen = text_len;
			
			p_sht_text->area.x0 = col_vx0;
			p_sht_text->area.y0 = CNF_Get_row_vy(row);

			
			p_sht_text->p_gp->vdraw(p_sht_text->p_gp, &p_sht_text->cnt, &p_sht_text->area);
			
		}
		
		
		col_maxlen += col_space;
	}
	
	//更新页数
	if(p_cfg_stg->stg_num_rows >  SETTING_ROW_MAX)
		sprintf(arr_p_pool_shts[CNF_PAGE_SHT]->cnt.data, "[%d/%d]", \
			cthis->entry_start_row / SETTING_ROW_MAX + 1, p_cfg_stg->stg_num_rows / SETTING_ROW_MAX + 1);
	
	Flush_LCD();
}

static int CNF_Exec_Sy_cmd(void *p_rcv, int cmd,  void *arg)
{
	HMI								*self = (HMI *)p_rcv;
	HMI_configuring					*cthis = SUB_PTR( self, HMI, HMI_configuring);
	sheet 							*p_sht_text = arr_p_pool_shts[CNF_TEXT_SHT];
	strategy_focus_t				*p_pos;
	int 							ret = RET_OK;
	
	switch(cmd) {
		case sycmd_reflush:
			p_sht_text->cnt.colour = PALLET_BLACK;
			cthis->entry_start_row = 0;
			self->show(self);
			self->show_cmp(self);

			break;
		case sycmd_reflush_position:
			p_pos = (strategy_focus_t		*)arg;
		
			p_sht_text->cnt.len = \
				p_cfg_stg->entry_txt(p_pos->f_row, p_pos->f_col, &p_sht_text->cnt.data);
			if(p_sht_text->cnt.len == 0)
				break;
			p_sht_text->cnt.colour = PALLET_BLACK;
			p_sht_text->area.x0 = cthis->col_vx0[p_pos->f_col];
			p_sht_text->area.y0 = CNF_Get_row_vy(p_pos->f_row);
			
			p_sht_text->p_gp->vdraw(p_sht_text->p_gp, &p_sht_text->cnt, &p_sht_text->area);
		
		
			break;
		
		
		
	
		
	}
	
	return ret;
	
	
}



//切换页面时，将光标重新至于编辑区
static void CNF_Turn_page(HMI *self, int up_or_dn)
{
	HMI_configuring		*cthis = SUB_PTR( self, HMI, HMI_configuring);
	
	if(p_cfg_stg == NULL)
		return;
	
	if(up_or_dn == 0) 
	{
		cthis->entry_start_row -= SETTING_ROW_MAX;
	}
	else
	{
		cthis->entry_start_row += SETTING_ROW_MAX;
	} 
	
	CLR_LCD();

	p_cfg_stg->reset_focus();
	self->show(self);

	Flush_LCD();
	
	return;
}


//策略的按键处理要执行2个任务：焦点在策略内部时进行编辑设置项，或者焦点移动；焦点在策略外部时，上下按钮翻页
//上下键的复用：当焦点位于策略内的编辑区时，上下键用于编辑内容；否则则用于焦点的上下移动
//设置画面的焦点都是由设置策略来控制的
//不过设置策略可以通过返回错误来表示焦点跳出范围的功能

static void	CNF_Hit(HMI *self, char kcd)
{
	
//	HMI_configuring				*cthis = SUB_PTR( self, HMI, HMI_configuring);
	strategy_t				*p_sy = p_cfg_stg;
	strategy_focus_t	old_sf;
	int					ret = 0;
	//1 焦点在策略内部发送变化 2 焦点跳出策略 3 策略的焦点内容发生了变化
	uint8_t		sy_chgFouse = 0;

	old_sf.f_col = p_sy->sf.f_col;
	old_sf.f_row = p_sy->sf.f_row;
	old_sf.start_byte = p_sy->sf.start_byte;
	old_sf.num_byte = p_sy->sf.num_byte;



	switch(kcd)
	{
		case KEYCODE_LEFT:


			ret = p_sy->key_hit_lt(NULL);
			if(ret == CST_RET_RECOVERY_OLD_FOCUS) {
				
				sy_chgFouse = 1;
			} 
			else if(ret == CST_RET_CLEAN_OLD_FOCUS)
			{
				
				sy_chgFouse = 3;
			}
			else if(ret == CST_RET_FOCUS_HOLD)
			{
				
				sy_chgFouse = 0;
			}
			


			break;
		case KEYCODE_RIGHT:

			ret = p_sy->key_hit_rt(NULL);
			if(ret == CST_RET_RECOVERY_OLD_FOCUS) {
				
				sy_chgFouse = 1;
			} 
			else if(ret == CST_RET_CLEAN_OLD_FOCUS)
			{
				
				sy_chgFouse = 3;
			}
			else if(ret == CST_RET_FOCUS_HOLD)
			{
				
				sy_chgFouse = 0;
			}
			break;
		case KEYCODE_UP:
			aci_sys.key_weight = 1;
			if(aci_sys.hit_count < 10)
				aci_sys.hit_count ++;
			else
				aci_sys.hit_count = 0;
			ret = p_sy->key_hit_up(NULL);

			if(ret == CST_RET_RECOVERY_OLD_FOCUS) {
				//编辑区内的上下操作是用来修改参数的
				//因此在修改参数成功之后，要重新显示被选中区的值
				sy_chgFouse = 1;
			} 
			else if(ret == CST_RET_CLEAN_OLD_FOCUS)
			{
				
				sy_chgFouse = 3;
			}
			else if(ret == CST_RET_FOCUS_HOLD)
			{
				
				sy_chgFouse = 0;
			}
			
			else {
				CNF_Turn_page(self, 0);


			}
			break;
		case KEYCODE_DOWN:
			aci_sys.key_weight = 1;
			if(aci_sys.hit_count < 10)
				aci_sys.hit_count ++;
			else
				aci_sys.hit_count = 0;



			ret = p_sy->key_hit_dn(NULL);
			
			if(ret == CST_RET_RECOVERY_OLD_FOCUS) {
				
				sy_chgFouse = 1;
			} 
			else if(ret == CST_RET_CLEAN_OLD_FOCUS)
			{
				//编辑区内的上下操作是用来修改参数的
				//因此在修改参数成功之后，要重新显示被选中区的值
				sy_chgFouse = 3;
			}
			else if(ret == CST_RET_FOCUS_HOLD)
			{
				
				sy_chgFouse = 0;
			}
			else {

				CNF_Turn_page(self, 1);
			}
			break;
		case KEYCODE_SWITCH:
			//大气A/B 粉尘之间的切换
			break;
		case KEYCODE_ENTER:

			p_sy->key_hit_er(NULL) ;


			break;		
		case KEYCODE_ESC:
			self->switchBack(self);
			break;	

	}
	if(sy_chgFouse == 1) {
		CNF_Show_strategy_focus(&old_sf, 0);
		CNF_Show_strategy_focus(&p_sy->sf, 1);
	} else if(sy_chgFouse == 2){
		//光标从编辑区跳出
		CNF_Show_strategy_focus(&old_sf, 0);

	}

	//选中区域的内容被修改了，更新内容
	//要清除原来的内容，然后在重新显示新的内容
	else if(sy_chgFouse ==3){
		CNF_Show_strategy_focus(&old_sf, 2);
		CNF_Show_strategy_focus(&p_sy->sf, 0);
		CNF_Show_strategy_focus(&p_sy->sf, 1);
	}
	
	
	if(sy_chgFouse)
	{
		Flush_LCD();
	}


	return;
}



static void	CNF_Long_hit( HMI *self, char kcd)
{
//	HMI_configuring			*cthis = SUB_PTR( self, HMI, HMI_configuring);
	strategy_keyval_t	skt = {SY_KEYTYPE_LONGPUSH};
	strategy_t			*p_sy = p_cfg_stg;
	



	

	if(aci_sys.hit_count == 10)
	{
		if(aci_sys.key_weight < 10000)
			aci_sys.key_weight *= 10;

		aci_sys.hit_count = 0;
	}
	else
	{
		aci_sys.hit_count ++;

	}
	
	if(kcd == KEYCODE_UP)
	{
		p_sy->key_hit_up(&skt);
		
		
	}
	
	if(kcd == KEYCODE_DOWN)
	{
		p_sy->key_hit_dn(&skt);
		
	}
	
	

	//显示新的选中效果
	CNF_Show_strategy_focus(&p_sy->sf, 1);
	//180114 长按的时候尽快刷新
	Flush_LCD();
}


static uint8_t	CNF_Get_row_vy(uint8_t row)
{
	if(row > SETTING_ROW_MAX)
		return 16;
	
	return (row + 1)* 16;
}



















