#include "HMI_query.h"
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
#include "Model.h"
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
查询画面结构如下
信号类型		实时值[当前页数/总页数]
条目		值
条目		值
...


查询界面时通过swicth按键来切换信号类型
第一次切换到本界面时的信号类型都是大气A
信号的切换顺序是:大气A,大气B，粉尘
查询界面不需要处理焦点
*/
static ro_char qry_code_title[] = { "<text vx0=0 vy0=0> </>" };
static ro_char qry_code_record_index[] = { "<text vx0=72 vy0=0>0</>" };		//记录索引的值应该时0 - 999吧，显示的时候%3d来对齐
static ro_char qry_code_page[] = { "<text vx0=88 vy0=0>[1/4]</>" };		//显示页码
static ro_char qry_code_text[] = { "<text vx0=0 vy0=16> </>" };

#define QRY_IDX_SHT				1
#define QRY_PAGE_SHT			2
#define QRY_TEXT_SHT			3

#define QRY_SIG_TYPE			aci_sys.hmi_mgr.hand_signal_type

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static HMI_query *signal_query;
static strategy_t *p_query_stg;
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	QRY_Init( HMI *self, void *arg);
static void	QRY_Show( HMI *self);
static void QRY_Hide( HMI *self );
static void QRY_Init_sheet( HMI *self );
static void	QRY_Init_focus(HMI *self);
static void	QRY_Clear_focus(HMI *self, uint8_t fouse_row, uint8_t fouse_col);
static void	QRY_Show_focus( HMI *self);
static void	QRY_Hit( HMI *self, char kcd);


static void QRY_Show_strategy(void);
static void QRY_Turn_page(HMI *self, int up_or_dn);


static int QRY_Exec_Sy_cmd(void *p_rcv, int cmd,  void *arg);
static uint8_t	QRY_Get_row_vy(uint8_t row);


static uint8_t	QRY_Switch_signal(uint8_t	old_sig);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
HMI_query *Get_HMI_query(void) 
{
	if( signal_query == NULL)
	{
		signal_query = HMI_query_new();
		if(signal_query  == NULL) while(1);
	}
	
	return signal_query;
}










CTOR( HMI_query)
SUPER_CTOR( HMI);
FUNCTION_SETTING( HMI.init, QRY_Init);
FUNCTION_SETTING( HMI.hide, QRY_Hide);
FUNCTION_SETTING( HMI.initSheet, QRY_Init_sheet);
FUNCTION_SETTING( HMI.show, QRY_Show);

FUNCTION_SETTING( HMI.hitHandle, QRY_Hit);


FUNCTION_SETTING(HMI.init_focus, QRY_Init_focus);
FUNCTION_SETTING(HMI.clear_focus, QRY_Clear_focus);
FUNCTION_SETTING(HMI.show_focus, QRY_Show_focus);
END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int	QRY_Init( HMI *self, void *arg)
{


	self->flag = 0;
	
	QRY_SIG_TYPE = e_qry_atmosphere_A;
	return RET_OK;
	

}




static void QRY_Init_sheet( HMI *self )
{
	HMI_query		*cthis = SUB_PTR( self, HMI, HMI_query);
	strategy_t			*p_stg;
	Expr 		*p_exp ;
	int 		h = 0;
	
	
	p_exp = ExpCreate( "text");
	
	p_exp->inptSht(p_exp, (void *)qry_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)qry_code_record_index, arr_p_pool_shts[QRY_IDX_SHT]);
	p_exp->inptSht(p_exp, (void *)qry_code_page, arr_p_pool_shts[QRY_PAGE_SHT]);
	p_exp->inptSht(p_exp, (void *)qry_code_text, arr_p_pool_shts[QRY_TEXT_SHT]);
	arr_p_pool_shts[QRY_PAGE_SHT]->cnt.data = cthis->page_buf;
	

	
	
	p_stg = Get_strategy(QRY_SIG_TYPE);
	if(p_stg == NULL)
	{
		self->flag |= HMI_FLAG_ERR;
		return;
		
	}
	p_stg->cmd_hdl = QRY_Exec_Sy_cmd;
	p_stg->p_cmd_rcv = self;
	
	p_stg->init(&QRY_SIG_TYPE);		//大气类型用于区分是大气A还是大气B
	
	cthis->cur_page = 0;
	
	h = 0;
	p_query_stg = p_stg;
	if(p_stg->p_stg_title)
	{
		arr_p_pool_shts[0]->cnt.data = p_stg->p_stg_title;
		arr_p_pool_shts[0]->cnt.len = strlen(p_stg->p_stg_title);
		Sheet_updown(arr_p_pool_shts[0], h++);
		
	}
	Sheet_updown(arr_p_pool_shts[QRY_IDX_SHT], h++);
	Sheet_updown(arr_p_pool_shts[QRY_PAGE_SHT], h++);

	self->init_focus(self);
	
}


static void QRY_Hide(HMI *self)
{
	
	Sheet_updown(arr_p_pool_shts[QRY_PAGE_SHT], -1);
	Sheet_updown(arr_p_pool_shts[QRY_IDX_SHT], -1);
	Sheet_updown(arr_p_pool_shts[0], -1);;
	
	self->clear_focus(self, self->p_fcuu->focus_row, self->p_fcuu->focus_col);
	
	p_query_stg = NULL;

}	

static void	QRY_Show( HMI *self)
{

	
	Sheet_refresh(arr_p_pool_shts[0]);
	
	QRY_Show_strategy();
	self->show_focus(self);
}


static void	QRY_Init_focus(HMI *self) 
{
	
	
}

//设置界面中的焦点操作，只是占位而已
static void		QRY_Clear_focus(HMI *self, uint8_t fouse_row, uint8_t fouse_col)
{
	
}



static void		QRY_Show_focus(HMI *self)
{
	
	
}






static void QRY_Show_strategy(void)
{

	HMI_query		*cthis = signal_query;
	sheet 				*p_sht_text = arr_p_pool_shts[QRY_TEXT_SHT];
	uint8_t	row = 0;
	uint8_t	col = 0;

	uint8_t	text_len = 0;
	uint8_t	col_maxlen = 0;			//某一列的列宽，取最大的那个列宽
	
	uint16_t	txt_xsize = 0;
	uint16_t	txt_ysize = 0;
	uint8_t		col_vx0 = 0;
	uint8_t		col_space = 1;				//列间距：单位 字符的宽度
	
		
	if(p_query_stg == NULL)
		return;
	

	
	p_sht_text->p_gp->getSize(p_sht_text->p_gp, p_sht_text->cnt.font, &txt_xsize, &txt_ysize);
	
	if(cthis->cur_page < QRY_MAX_PAGES)
	{
		
		cthis->arr_start_row[cthis->cur_page] = cthis->entry_start_row;
	}
	
	//按列显示，这是为了能够获得前一列的最大宽度，来决定下一列的x轴位置
	for(col = 0; col < SETTING_COL_MAX; col ++) {
		
		//把每一列的起始x轴位置计算并记录下来
		col_vx0 += col_maxlen * txt_xsize;
		cthis->col_vx0[col] = col_vx0;
		
		
		//显示一页中所有的行
		col_maxlen = 0;
		
		
		for(row = 0; row < SETTING_ROW_MAX; row ++) {
			if((p_query_stg->row_in_page > 0) && (row >= p_query_stg->row_in_page))
				continue;
			text_len = p_query_stg->entry_txt(row + cthis->entry_start_row, col, &p_sht_text->cnt.data);
			if(text_len == 0)
				continue;
			p_sht_text->cnt.len = text_len;
			if(col_maxlen < text_len)
				col_maxlen = text_len;
			
			p_sht_text->area.x0 = col_vx0; 
			p_sht_text->area.y0 = QRY_Get_row_vy(row);
			
			//对x轴进行判断，如果放不下，就把x轴进行左移
			//这是因为有些情况下，某一行时无法进行对齐的，因此在程序的编写的时候要保证x轴左移不会覆盖前一列的内容
			if((p_sht_text->area.x0 + p_sht_text->cnt.len * txt_xsize) >= LCD_LONG)
				p_sht_text->area.x0 = LCD_LONG - p_sht_text->cnt.len * txt_xsize;

			
			p_sht_text->p_gp->vdraw(p_sht_text->p_gp, &p_sht_text->cnt, &p_sht_text->area);
			
		}
		
		
		col_maxlen += col_space;
	}
	
	//更新页数
	if(p_query_stg->stg_num_rows >  SETTING_ROW_MAX)
	{
		sprintf(arr_p_pool_shts[QRY_PAGE_SHT]->cnt.data, "[%d/%d]", \
			cthis->cur_page + 1, p_query_stg->stg_num_rows / SETTING_ROW_MAX + 1);
		arr_p_pool_shts[QRY_PAGE_SHT]->p_gp->vdraw(arr_p_pool_shts[QRY_PAGE_SHT]->p_gp, \
			&arr_p_pool_shts[QRY_PAGE_SHT]->cnt, &arr_p_pool_shts[QRY_PAGE_SHT]->area);
	}
	
	Flush_LCD();
}

static int QRY_Exec_Sy_cmd(void *p_rcv, int cmd,  void *arg)
{
	HMI								*self = (HMI *)p_rcv;
	HMI_query					*cthis = SUB_PTR( self, HMI, HMI_query);
	sheet 							*p_sht_text = arr_p_pool_shts[QRY_TEXT_SHT];
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
				p_query_stg->entry_txt(p_pos->f_row, p_pos->f_col, &p_sht_text->cnt.data);
			if(p_sht_text->cnt.len == 0)
				break;
			p_sht_text->cnt.colour = PALLET_BLACK;
			p_sht_text->area.x0 = cthis->col_vx0[p_pos->f_col];
			p_sht_text->area.y0 = QRY_Get_row_vy(p_pos->f_row);
			
			p_sht_text->p_gp->vdraw(p_sht_text->p_gp, &p_sht_text->cnt, &p_sht_text->area);
		
		
			break;
		
		
		
	
		
	}
	
	return ret;
	
	
}



//切换页面时，将光标重新至于编辑区
static void QRY_Turn_page(HMI *self, int up_or_dn)
{
	HMI_query		*cthis = SUB_PTR( self, HMI, HMI_query);
	int				max_row = p_query_stg->row_in_page ;
	if(p_query_stg == NULL)
		return;
	
	if((max_row > SETTING_ROW_MAX) || (max_row == 0))
		max_row = SETTING_ROW_MAX;
	if(up_or_dn == 0) 
	{
		//在向上翻页的时候，把记录中的起始行数取出来
		if(cthis->cur_page > 0)
		{
			cthis->cur_page --;
			cthis->entry_start_row = cthis->arr_start_row[cthis->cur_page];
			
			
		}
		else 
		{
			return;
			
		}
	}
	else
	{
		//在向下翻页的时候，记录下每一页的起始行数
		if((cthis->entry_start_row + max_row) < p_query_stg->stg_num_rows)
		{
			cthis->arr_start_row[cthis->cur_page] = cthis->entry_start_row;
			cthis->entry_start_row += max_row;
			if(cthis->cur_page < QRY_MAX_PAGES)
			{
				
				cthis->cur_page ++;
				
			}
			
		}
		else
		{
			return;
			
		}
	} 
	
	CLR_LCD();

	self->show(self);

	Flush_LCD();
	
	return;
}



static void	QRY_Hit(HMI *self, char kcd)
{
	
	HMI_query		*cthis = SUB_PTR( self, HMI, HMI_query);

	switch(kcd)
	{
		case KEYCODE_LEFT:



			break;
		case KEYCODE_RIGHT:

			
			break;
		case KEYCODE_UP:
			QRY_Turn_page(self, 0);
			break;
		case KEYCODE_DOWN:
			QRY_Turn_page(self, 1);
			break;
		case KEYCODE_SWITCH:
			//大气A/B 粉尘之间的切换
			QRY_SIG_TYPE = QRY_Switch_signal(QRY_SIG_TYPE);
			cthis->entry_start_row = 0;
			
			self->switchHMI(self, self);
			
			break;
		case KEYCODE_ENTER:
			break;		
		case KEYCODE_ESC:
			self->switchBack(self);
			break;	

	}
	return;
}






static uint8_t	QRY_Get_row_vy(uint8_t row)
{
	if(row > SETTING_ROW_MAX)
		return 16;
	
	return (row + 1)* 16;
}

static uint8_t	QRY_Switch_signal(uint8_t	old_sig)
{
	if(old_sig == e_qry_atmosphere_A)
	{
		return e_qry_atmosphere_B;
		
	}
//	if(old_sig == e_qry_atmosphere_B)
//	{
//		return e_qry_dust;
//		
//	}
	
	return e_qry_atmosphere_A;
	
	
}

















