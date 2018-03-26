#include <stdint.h>
#include "configure_strategy.h"
#include "HMI.h"
#include "utils/PHN_string.h"
#include "lcd_conf.h"
//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------

static int DSF_entry(int row, int col, void *pp_text);
static int DSF_init(void *arg);
static int DSF_key_up(void *arg);
static int DSF_key_dn(void *arg);
static int DSF_key_lt(void *arg);
static int DSF_key_rt(void *arg);
static int DSF_key_er(void *arg);
static void DSF_Reset_focus(void);
static int DSF_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf);
static int DSF_commit(void *arg);
static void DSF_Exit(void);

strategy_t	cst_set_dust_flow = {
	DSF_entry,
	DSF_init,
	STG_Duild_button,
	DSF_key_up,
	DSF_key_dn,
	DSF_key_lt,
	DSF_key_rt,
	DSF_key_er,
	DSF_Reset_focus,
	DSF_get_focusdata,	
	DSF_commit,
	DSF_Exit,
};


//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//
#define		THIS_STG	cst_set_dust_flow

#define		THIS_NUM_ROWS		1
#define		THIS_MAX_ROW		0

#define		NEED_NUM_RAM			2
#define 	CACHE_BUF_NUM			1
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
// static char *const set_dust_flow_entrys[THIS_NUM_ROWS] = {"�۳�����"};
	
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
 static int DSF_modify(void *arg, int op);
 static int DSF_Col_1(int pos);		//�����ڱ༭�����ƶ�
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int DSF_entry(int row, int col, void *pp_text)
{
	char **pp = (char **)pp_text;
	system_conf_t	*p_s;
	if(row > THIS_MAX_ROW)
		return 0;
	if(col > 0)
		return 0;
	
	p_s->dust_flow = aci_sys.sys_conf.dust_flow;
	Print_float(p_s->dust_flow, 4, 1, arr_p_vram[STG_RAM_NUM(0, 0)]);
	strcat(arr_p_vram[STG_RAM_NUM(0, 0)], "L/min");
	*pp = arr_p_vram[STG_RAM_NUM(row, 1)];
	return strlen(*pp);
	
//	if(col == 0) {
//		strcpy(arr_p_vram[STG_RAM_NUM(row, col)], set_dust_flow_entrys[row]);
//		
//		*pp = arr_p_vram[STG_RAM_NUM(row, col)];
//		return strlen(*pp);
//	} else if(col == 1) {
//		
//		p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
//		switch(row)
//		{
//			case 0:
//				p_s->dust_flow = aci_sys.sys_conf.dust_flow;
//				Print_float(p_s->dust_flow, 4, 1, arr_p_vram[STG_RAM_NUM(0, 0)]);
//				strcat(arr_p_vram[STG_RAM_NUM(0, 1)], "L/min");
//				break;
//		
//		
//			
//		}
//		
//		
//		*pp = arr_p_vram[STG_RAM_NUM(row, 1)];
//		return strlen(*pp);
//	}
//	return 0;
}

static int DSF_init(void *arg)
{
	int i = 0;
	memset(&THIS_STG.sf, 0, sizeof(THIS_STG.sf));
	
	THIS_STG.p_stg_title = "�۳�����";
	
	DSF_Reset_focus();
	
	THIS_STG.stg_num_rows = THIS_NUM_ROWS;
	HMI_Ram_init();
	for(i = 0; i < NEED_NUM_RAM; i++) {
		
		arr_p_vram[i] = HMI_Ram_alloc(48);
		memset(arr_p_vram[i], 0, 48);
	}
	
	aci_sys.key_weight = 1;
	
	return RET_OK;
}
static void DSF_Exit(void)
{
	
}
static int DSF_commit(void *arg)
{
	system_conf_t	*p_s;
	p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	aci_sys.sys_conf.dust_flow = p_s->dust_flow;
	
	return RET_OK;

}

//����ı༭�Ƕ�ĳһλ���е�
//100.0 �İ�λ��ѡ��ʱ�����Ƕ԰�λ�ϵ����ֽ��б༭������λ����
//Ҫ������ֵλ���ı仯
static int DSF_modify(void *arg, int op)
{
	
	
	strategy_focus_t *p_syf = &THIS_STG.sf;
	system_conf_t	*p_s;
	int						weight = 1;
	short						i, j;
	p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	
	j = strlen(arr_p_vram[STG_RAM_NUM(0, 0)]) - 6; //100.0L/min	�ų�L/min	�� С����
	for(i = p_syf->start_byte; i < j; i ++)
		weight *= 10;
	
	switch(p_syf->f_row)
	{
		
		case 0:
			
			p_s->dust_flow = Operate_in_tange(p_s->dust_flow, op, weight, 0, weight * 10 - 1);
			Print_float(p_s->dust_flow, 4, 1, arr_p_vram[STG_RAM_NUM(0, 0)]);
			strcat(arr_p_vram[STG_RAM_NUM(0, 0)], "L/min");
				
			break;
		
		
	}
	
	//����һ�½���ĳ���
	DSF_Col_1(CST_POS_HOLD);
	return CST_RET_CLEAN_OLD_FOCUS;
}

//�����ƶ���Χ�ڷ���0
//������Χ��-1
//�༭��:
//100.0L/min

static int DSF_Col_1(int pos)
{
	int  ret = 0;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	char				*p = arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)];
	
	
	p_syf->num_byte = 1;
	
	switch(pos)
	{
		
		case CST_POS_FIRST:
			p_syf->start_byte = 0;
			break;
		case CST_POS_LAST:
			p_syf->start_byte = strlen(p) - 5; //100.0L/min	�ų�L/min
			break;
		case CST_POS_ADD:
			if(p_syf->start_byte < (strlen(p) - 5))
				p_syf->start_byte ++;
			else
				p_syf->start_byte = 0;
			if(p[p_syf->start_byte] == '.')
				p_syf->start_byte ++;
			break;
		case CST_POS_SUB:
			if(p_syf->start_byte)
				p_syf->start_byte --;
			else
				p_syf->start_byte = strlen(p) - 5;
			if(p[p_syf->start_byte] == '.')
				p_syf->start_byte --;
			break;		
		
		
	}
	
	
	
	return ret;
	
}

static void DSF_Reset_focus(void)
{
	THIS_STG.sf.f_col = 0;
	THIS_STG.sf.f_row = 0;
	THIS_STG.sf.start_byte = 0;
	THIS_STG.sf.num_byte = 1;
	
}

static int DSF_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	char		**pp_vram = (char **)pp_data;
	
	if(p_syf->f_row > THIS_MAX_ROW) {
		return -1;
	}
	
	if(p_in_syf)
		p_syf = p_in_syf;
	
	
	
	*pp_vram = arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)];

	return strlen(*pp_vram);
	
}

static int DSF_key_up(void *arg)
{
//	int ret = CST_RET_RECOVERY_OLD_FOCUS;
//	strategy_focus_t *p_syf = &THIS_STG.sf;
	
	//�е��л�
//	if(p_syf->f_col == 0)
//	{
//		if(p_syf->f_row > 0)
//			p_syf->f_row --;
//		else
//		{
//			p_syf->f_row = THIS_MAX_ROW;
//		}
//		
//		if(p_syf->f_row == 1)
//			p_syf->num_byte = 8;
//		else
//			p_syf->num_byte = 4;
//		
//		return ret;
//	}
	return DSF_modify(arg, OP_ADD);
}
static int DSF_key_dn(void *arg)
{
//	int ret = CST_RET_RECOVERY_OLD_FOCUS;
//	strategy_focus_t *p_syf = &THIS_STG.sf;
	
	//�е��л�
//	if(p_syf->f_col == 0)
//	{
//		if(p_syf->f_row < THIS_MAX_ROW)
//			p_syf->f_row ++;
//		else
//		{
//			p_syf->f_row = 0;
//		}
//		if(p_syf->f_row == 1)
//			p_syf->num_byte = 8;
//		else
//			p_syf->num_byte = 4;
//		
//		return ret;
//	}
	
	return DSF_modify(arg, OP_SUB);
}

static int DSF_key_lt(void *arg)
{
//	strategy_focus_t *p_syf = &THIS_STG.sf;
	int ret = CST_RET_RECOVERY_OLD_FOCUS;
	
//	if(p_syf->f_col == 0)
//	{
//		
//		p_syf->f_col = 1;
//		DSF_Col_1(CST_POS_LAST);
//	}
//	else
//	{
//		if(DSF_Col_1(CST_POS_SUB) < 0)
//		{
//			
//			
//			p_syf->f_col = 0;
//			p_syf->start_byte = 0;
//			if(p_syf->f_row == 1)
//				p_syf->num_byte = 8;
//			else
//				p_syf->num_byte = 4;
//			
//		}
//		
//		
//	}
	
	DSF_Col_1(CST_POS_SUB);
	return ret;
}

static int DSF_key_rt(void *arg)
{
//	strategy_focus_t *p_syf = &THIS_STG.sf;
	int ret = RET_OK;
	
//	if(p_syf->f_col == 0)
//	{
//		
//		p_syf->f_col = 1;
//		DSF_Col_1(CST_POS_FIRST);
//	}
//	else
//	{
//		if(DSF_Col_1(CST_POS_ADD) < 0)
//		{
//			
//			
//			p_syf->f_col = 0;
//			p_syf->start_byte = 0;
//			if(p_syf->f_row == 1)
//				p_syf->num_byte = 8;
//			else
//				p_syf->num_byte = 4;
//			
//		}
//		
//		
//	}
	
	DSF_Col_1(CST_POS_ADD);
	return ret;
}
static int DSF_key_er(void *arg)
{
	
	return DSF_commit(NULL);
	
}




