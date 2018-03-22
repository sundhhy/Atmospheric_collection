#include "HMI_setting.h"
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
//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
/*
设置界面，设置的内容根据不同的设置策略而变化


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
static ro_char hst_code_title[] = { "<text vx0=0 vy0=0></>" };
static ro_char hst_code_page[] = { "<text vx0=88 vy0=0>[1/4]</>" };		//显示页码
static ro_char hst_code_text[] = { "<text vx0=0 vy0=16></>" };

#define HST_PAGE_SHT			1
#define HST_TEXT_SHT			2

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static HMI_setting *signal_hst;
static strategy_t		*p_hst_stg;		//当前真正设置的内容
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	HST_Init( HMI *self, void *arg);
static void	HST_Show( HMI *self);
static void HST_Hide( HMI *self );
static void HST_Init_sheet( HMI *self );
static void	HST_Init_focus(HMI *self);
static void	HST_Clear_focus(HMI *self, uint8_t fouse_row, uint8_t fouse_col);
static void	HST_Show_focus( HMI *self);
static void	HST_Hit( HMI *self, char kcd);
static void	HST_Long_hit( HMI *self, char kcd);


static void HST_Show_strategy(void);
static void HST_Turn_page(HMI *self, int up_or_dn);
//设置画面的焦点操作，都转到策略类的焦点操作
static void HST_Show_strategy_focus(strategy_focus_t *p_syf, int opt);

static uint8_t	HST_Get_row_vy(uint8_t row);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
HMI_setting *Get_HMI_setting(void) 
{
	if( signal_hst == NULL)
	{
		signal_hst = HMI_setting_new();
		if(signal_hst  == NULL) while(1);
	}
	
	return signal_hst;
}

void HMI_HST_Setting_strategy(void *p_hst_stgg)
{
	
	p_hst_stg = p_hst_stgg;
	
}








CTOR( HMI_setting)
SUPER_CTOR( HMI);
FUNCTION_SETTING( HMI.init, HST_Init);
FUNCTION_SETTING( HMI.hide, HST_Hide);
FUNCTION_SETTING( HMI.initSheet, HST_Init_sheet);
FUNCTION_SETTING( HMI.show, HST_Show);

FUNCTION_SETTING( HMI.hitHandle, HST_Hit);
FUNCTION_SETTING( HMI.longpushHandle, HST_Long_hit);


FUNCTION_SETTING(HMI.init_focus, HST_Init_focus);
FUNCTION_SETTING(HMI.clear_focus, HST_Clear_focus);
FUNCTION_SETTING(HMI.show_focus, HST_Show_focus);
END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int	HST_Init( HMI *self, void *arg)
{



	self->flag = 0;

	
	return RET_OK;
	

}




static void HST_Init_sheet( HMI *self )
{
	HMI_setting		*cthis = SUB_PTR( self, HMI, HMI_setting);
	Expr 		*p_exp ;
	int 		h = 0;
	
	
	p_exp = ExpCreate( "text");
	
	p_exp->inptSht(p_exp, (void *)hst_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)hst_code_page, arr_p_pool_shts[HST_PAGE_SHT]);
	p_exp->inptSht(p_exp, (void *)hst_code_text, arr_p_pool_shts[HST_TEXT_SHT]);
	arr_p_pool_shts[HST_TEXT_SHT]->cnt.data = cthis->page_buf;
	

	
	h = 0;
	
	
	Sheet_updown(arr_p_pool_shts[0], h++);
//	Sheet_updown(arr_p_pool_shts[1], h++);

	self->init_focus(self);
	
}


static void HST_Hide(HMI *self)
{
	

	Sheet_updown(arr_p_pool_shts[0], -1);;
	
	self->clear_focus(self, self->p_fcuu->focus_row, self->p_fcuu->focus_col);
	


	p_hst_stg = NULL;
}	

static void	HST_Show( HMI *self)
{

	
	Sheet_refresh(arr_p_pool_shts[0]);
	self->show_focus(self);
	HST_Show_strategy();
}


static void	HST_Init_focus(HMI *self) 
{
	if(p_hst_stg)
		p_hst_stg->reset_focus();
}

//设置界面中的焦点操作，只是占位而已
static void		HST_Clear_focus(HMI *self, uint8_t fouse_row, uint8_t fouse_col)
{

	
}



static void		HST_Show_focus(HMI *self)
{
	
}


static void HST_Show_strategy_focus(strategy_focus_t *p_syf, int opt)
{
	HMI_setting				*cthis = signal_hst;
	int 							f_data_len = 0;
	sheet 						*p_sht_text = arr_p_pool_shts[HST_TEXT_SHT];
	uint16_t					txt_xsize, txt_ysize;
	
	if(p_hst_stg == NULL)
		return;
	
	
	p_sht_text->p_gp->getSize(p_sht_text->p_gp, p_sht_text->cnt.font, &txt_xsize, &txt_ysize);
	
	//获得焦点位于的内容与位置
	f_data_len = p_hst_stg->get_focus_data(p_sht_text->cnt.data, p_syf);
	
	//根据焦点的位置计算在屏幕上的坐标
	
	p_sht_text->cnt.len = f_data_len;
	
	//焦点可能只是字符串的一部分，因此要进行偏移
	p_sht_text->cnt.data += p_syf->start_byte;
	p_sht_text->area.x0 = cthis->col_vx0[p_syf->f_col] + txt_xsize * p_syf->start_byte;
	p_sht_text->area.y0 = HST_Get_row_vy(p_syf->f_row - cthis->entry_start_row);
	
	//显示
	if(opt)
		p_sht_text->cnt.effects = GP_SET_EFF(p_sht_text->cnt.effects, EFF_FOCUS);
	p_sht_text->p_gp->vdraw(p_sht_text->p_gp, &p_sht_text->cnt, &p_sht_text->area);
	p_sht_text->cnt.effects = GP_CLR_EFF(p_sht_text->cnt.effects, EFF_FOCUS);
	
	
}



static void HST_Show_strategy(void)
{

	HMI_setting		*cthis = signal_hst;
	sheet 				*p_sht_text = arr_p_pool_shts[HST_TEXT_SHT];
	uint8_t	row = 0;
	uint8_t	col = 0;

	uint8_t	text_len = 0;
	uint8_t	col_maxlen = 0;			//某一列的列宽，取最大的那个列宽
	
	uint16_t	txt_xsize = 0;
	uint16_t	txt_ysize = 0;
	uint8_t		col_vx0 = 0;
	uint8_t		col_space = 2;				//列间距：单位 字符的宽度
	
		
	if(p_hst_stg == NULL)
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
			
			text_len = p_hst_stg->entry_txt(row + cthis->entry_start_row, col, &p_sht_text->cnt.data);
			if(text_len == 0)
				continue;
			p_sht_text->cnt.len = text_len;
			if(col_maxlen < text_len)
				col_maxlen = text_len;
			
			p_sht_text->area.x0 = col_vx0;
			p_sht_text->area.y0 = HST_Get_row_vy(row);

			
			p_sht_text->p_gp->vdraw(p_sht_text->p_gp, &p_sht_text->cnt, &p_sht_text->area);
			
		}
		
		
		col_maxlen += col_space;
	}
	
	//更新页数
	sprintf(arr_p_pool_shts[HST_PAGE_SHT]->cnt.data, "[%d/%d]", \
		cthis->entry_start_row / SETTING_ROW_MAX + 1, p_hst_stg->stg_num_rows / SETTING_ROW_MAX + 1);
	
}


//切换页面时，将光标重新至于编辑区
static void HST_Turn_page(HMI *self, int up_or_dn)
{
	HMI_setting		*cthis = SUB_PTR( self, HMI, HMI_setting);
	
	if(p_hst_stg == NULL)
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

	p_hst_stg->reset_focus();
	self->show(self);

	Flush_LCD();
	
	return;
}


//策略的按键处理要执行2个任务：焦点在策略内部时进行编辑设置项，或者焦点移动；焦点在策略外部时，上下按钮翻页
//上下键的复用：当焦点位于策略内的编辑区时，上下键用于编辑内容；否则则用于焦点的上下移动
//设置画面的焦点都是由设置策略来控制的
//不过设置策略可以通过返回错误来表示焦点跳出范围的功能

static void	HST_Hit(HMI *self, char kcd)
{
	
	HMI_setting				*cthis = SUB_PTR( self, HMI, HMI_setting);
	strategy_t				*p_sy = p_hst_stg;
	strategy_focus_t	old_sf;
	
	//1 焦点在策略内部发送变化 2 焦点跳出策略 3 策略的焦点内容发生了变化
	uint8_t		sy_chgFouse = 0;

	old_sf.f_col = p_sy->sf.f_col;
	old_sf.f_row = p_sy->sf.f_row;
	old_sf.start_byte = p_sy->sf.start_byte;
	old_sf.num_byte = p_sy->sf.num_byte;
	
	
	
	switch(kcd)
	{
		case KEYCODE_LEFT:
			
				
					if(p_sy->key_hit_lt(NULL) == RET_OK) {
					
						sy_chgFouse = 1;
					} else {
						sy_chgFouse = 2;
						
					}
			
		
			break;
		case KEYCODE_RIGHT:
			
				
				if(p_sy->key_hit_rt(NULL) == RET_OK) {
				
					sy_chgFouse = 1;
				}  else {
					sy_chgFouse = 2;
					
					
					
				}
		
			break;
		case KEYCODE_UP:
			aci_sys.key_weight = 1;
			if(aci_sys.hit_count < 10)
				aci_sys.hit_count ++;
			else
				aci_sys.hit_count = 0;
			
				
				if(p_sy->key_hit_up(NULL) == RET_OK) {
					//编辑区内的上下操作是用来修改参数的
					//因此在修改参数成功之后，要重新显示被选中区的值
					sy_chgFouse = 3;
				} 
			 else {
				 HST_Turn_page(self, 0);
				
				
			}
			
		case KEYCODE_DOWN:
			aci_sys.key_weight = 1;
			if(aci_sys.hit_count < 10)
				aci_sys.hit_count ++;
			else
				aci_sys.hit_count = 0;
			
			
				
				if(p_sy->key_hit_dn(NULL) == RET_OK) {
					
					sy_chgFouse = 3;
				} 
			 else {
				
				 HST_Turn_page(self, 1);
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
		HST_Show_strategy_focus(&old_sf, 0);
		HST_Show_strategy_focus(&p_sy->sf, 1);
	} else if(sy_chgFouse == 2){
		//光标从编辑区跳出
		HST_Show_strategy_focus(&old_sf, 0);

	}
	
	//选中区域的内容被修改了，更新内容
	else if(sy_chgFouse ==3){

		HST_Show_strategy_focus(&p_sy->sf, 1);
	}
	

		return;
}



static void	HST_Long_hit( HMI *self, char kcd)
{
	HMI_setting			*cthis = SUB_PTR( self, HMI, HMI_setting);
	strategy_keyval_t	skt = {SY_KEYTYPE_LONGPUSH};
	strategy_t			*p_sy = p_hst_stg;
	



	

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
	HST_Show_strategy_focus(&p_sy->sf, 1);
	//180114 长按的时候尽快刷新
	Flush_LCD();
}


static uint8_t	HST_Get_row_vy(uint8_t row)
{
	if(row > SETTING_ROW_MAX)
		return 16;
	
	return (row + 1)* 16;
}




















