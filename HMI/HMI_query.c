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
����������Ļ���
����ʹ�ò�ͬ�����ò��ԣ���ʵ�ֲ�ͬ�����ù���


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
��ѯ����ṹ����
�ź�����		ʵʱֵ[��ǰҳ��/��ҳ��]
��Ŀ		ֵ
��Ŀ		ֵ
...


��ѯ����ʱͨ��swicth�������л��ź�����
��һ���л���������ʱ���ź����Ͷ��Ǵ���A
�źŵ��л�˳����:����A,����B���۳�
��ѯ���治��Ҫ������
*/
static ro_char qry_code_title[] = { "<text vx0=0 vy0=0> </>" };
static ro_char qry_code_record_index[] = { "<text vx0=72 vy0=0>0</>" };		//��¼������ֵӦ��ʱ0 - 999�ɣ���ʾ��ʱ��%3d������
static ro_char qry_code_page[] = { "<text vx0=88 vy0=0>[1/4]</>" };		//��ʾҳ��
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
	
	p_stg->init(&QRY_SIG_TYPE);		//�����������������Ǵ���A���Ǵ���B
	
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

//���ý����еĽ��������ֻ��ռλ����
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
	uint8_t	col_maxlen = 0;			//ĳһ�е��п�ȡ�����Ǹ��п�
	
	uint16_t	txt_xsize = 0;
	uint16_t	txt_ysize = 0;
	uint8_t		col_vx0 = 0;
	uint8_t		col_space = 1;				//�м�ࣺ��λ �ַ��Ŀ��
	
		
	if(p_query_stg == NULL)
		return;
	

	
	p_sht_text->p_gp->getSize(p_sht_text->p_gp, p_sht_text->cnt.font, &txt_xsize, &txt_ysize);
	
	if(cthis->cur_page < QRY_MAX_PAGES)
	{
		
		cthis->arr_start_row[cthis->cur_page] = cthis->entry_start_row;
	}
	
	//������ʾ������Ϊ���ܹ����ǰһ�е�����ȣ���������һ�е�x��λ��
	for(col = 0; col < SETTING_COL_MAX; col ++) {
		
		//��ÿһ�е���ʼx��λ�ü��㲢��¼����
		col_vx0 += col_maxlen * txt_xsize;
		cthis->col_vx0[col] = col_vx0;
		
		
		//��ʾһҳ�����е���
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
			
			//��x������жϣ�����Ų��£��Ͱ�x���������
			//������Ϊ��Щ����£�ĳһ��ʱ�޷����ж���ģ�����ڳ���ı�д��ʱ��Ҫ��֤x�����Ʋ��Ḳ��ǰһ�е�����
			if((p_sht_text->area.x0 + p_sht_text->cnt.len * txt_xsize) >= LCD_LONG)
				p_sht_text->area.x0 = LCD_LONG - p_sht_text->cnt.len * txt_xsize;

			
			p_sht_text->p_gp->vdraw(p_sht_text->p_gp, &p_sht_text->cnt, &p_sht_text->area);
			
		}
		
		
		col_maxlen += col_space;
	}
	
	//����ҳ��
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



//�л�ҳ��ʱ��������������ڱ༭��
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
		//�����Ϸ�ҳ��ʱ�򣬰Ѽ�¼�е���ʼ����ȡ����
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
		//�����·�ҳ��ʱ�򣬼�¼��ÿһҳ����ʼ����
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
			//����A/B �۳�֮����л�
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

















