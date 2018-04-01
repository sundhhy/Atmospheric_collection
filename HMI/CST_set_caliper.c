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

static int CLP_entry(int row, int col, void *pp_text);
static int CLP_init(void *arg);
static int CLP_key_up(void *arg);
static int CLP_key_dn(void *arg);
static int CLP_key_lt(void *arg);
static int CLP_key_rt(void *arg);
static int CLP_key_er(void *arg);
static void CLP_Reset_focus(void);
static int CLP_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf);
static int CLP_commit(void *arg);
static void CLP_Exit(void);

strategy_t	cst_set_caliper = {
	CLP_entry,
	CLP_init,
	STG_Duild_button,
	CLP_key_up,
	CLP_key_dn,
	CLP_key_lt,
	CLP_key_rt,
	CLP_key_er,
	CLP_Reset_focus,
	CLP_get_focusdata,	
	CLP_commit,
	CLP_Exit,
};


//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//
#define		THIS_STG	cst_set_caliper

#define		THIS_NUM_ROWS		3
#define		THIS_MAX_ROW		2
#define		THIS_MAX_COL		1

#define		NEED_NUM_RAM			7
#define 	CACHE_BUF_NUM			6
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
 static char *const set_caliper_entrys[THIS_NUM_ROWS] = {"温度", "计压", "差压"};
	
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
 static int CLP_modify(void *arg, int op);
 static int CLP_Col_1(int pos);		//焦点在编辑区的移动
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int CLP_entry(int row, int col, void *pp_text)
{
	char **pp = (char **)pp_text;
	system_conf_t	*p_s;
	if(row > THIS_MAX_ROW)
		return 0;
	if(col > THIS_MAX_COL)
		return 0;
	if(col == 0)
	{
		*pp = set_caliper_entrys[row];
		
	}
	else 
	{
		
		p_s =(system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
		Print_float(p_s->dust_flow, 0, 1, arr_p_vram[STG_RAM_NUM(0, col)]);
		strcat(arr_p_vram[STG_RAM_NUM(0, col)], "L/min");
		*pp = arr_p_vram[STG_RAM_NUM(row, col)];
	}
	return strlen(*pp);
	

}

static int CLP_init(void *arg)
{
	int i = 0;
	system_conf_t	*p_s;
	memset(&THIS_STG.sf, 0, sizeof(THIS_STG.sf));
	
	THIS_STG.p_stg_title = "粉尘流量";
	
	
	
	THIS_STG.stg_num_rows = THIS_NUM_ROWS;
	HMI_Ram_init();
	for(i = 0; i < NEED_NUM_RAM; i++) {
		
		arr_p_vram[i] = HMI_Ram_alloc(48);
		memset(arr_p_vram[i], 0, 48);
	}
	p_s =(system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	p_s->dust_flow = aci_sys.sys_conf.dust_flow;
	
	sprintf(arr_p_vram[STG_RAM_NUM(0, 0)], "  ");		//让显示配置的位置往右移动一点
	
	CLP_Reset_focus();
	aci_sys.key_weight = 1;
	
	return RET_OK;
}
static void CLP_Exit(void)
{
	
}
static int CLP_commit(void *arg)
{
	system_conf_t	*p_s;
	p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	aci_sys.sys_conf.dust_flow = p_s->dust_flow;
	
	return RET_OK;

}

//这里的编辑是对某一位进行的
//100.0 的百位被选中时，就是对百位上的数字进行编辑，其他位不变
//要考虑数值位数的变化
static int CLP_modify(void *arg, int op)
{
	
	
	strategy_focus_t *p_syf = &THIS_STG.sf;
	system_conf_t	*p_s;
	short						weight = 1;
	short						limit = 1;
	short						i;
	p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	
	i = p_syf->start_byte;
	while(arr_p_vram[STG_RAM_NUM(0, p_syf->f_col)][i] != 'L')
	{
		if(arr_p_vram[STG_RAM_NUM(0, p_syf->f_col)][i] != '.')
			weight *= 10;
		i ++;
	}
	weight /= 10;
	
	i = 0;
	while(arr_p_vram[STG_RAM_NUM(0, p_syf->f_col)][i] != 'L')
	{
		if(arr_p_vram[STG_RAM_NUM(0, p_syf->f_col)][i] != '.')
			limit *= 10;
		i++;
		
	}
	switch(p_syf->f_row)
	{
		
		case 0:
			
			p_s->dust_flow = Operate_in_tange(p_s->dust_flow, op, weight, 0, limit);
			Print_float(p_s->dust_flow, 0, 1, arr_p_vram[STG_RAM_NUM(0, p_syf->f_col)]);
			strcat(arr_p_vram[STG_RAM_NUM(0, p_syf->f_col)], "L/min");
				
			break;
		
		
	}
	
	//更新一下焦点的长度
	CLP_Col_1(CST_POS_HOLD);
	return CST_RET_CLEAN_OLD_FOCUS;
}

//正常移动范围内返回0
//超出范围：-1
//编辑区:
//100.0L/min

static int CLP_Col_1(int pos)
{
	int  ret = 0;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	char				*p = arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)];
	
	
	p_syf->num_byte = 1;
	
	switch(pos)
	{
		case CST_POS_HOLD:
			if(p[p_syf->start_byte] == '.')
				p_syf->start_byte --;
			break;
		case CST_POS_FIRST:
			p_syf->start_byte = 0;
			break;
		case CST_POS_LAST:
			p_syf->start_byte = strlen(p) - 6; //100.0L/min	排除L/min
			break;
		case CST_POS_ADD:
			if(p_syf->start_byte < (strlen(p) - 6))
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
				p_syf->start_byte = strlen(p) - 6;
			if(p[p_syf->start_byte] == '.')
				p_syf->start_byte --;
			break;		
		
		
	}
	
	
	
	return ret;
	
}

static void CLP_Reset_focus(void)
{
	THIS_STG.sf.f_col = 1;
	THIS_STG.sf.f_row = 0;
	THIS_STG.sf.start_byte = 0;
	THIS_STG.sf.num_byte = 1;
	
}

static int CLP_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf)
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

static int CLP_key_up(void *arg)
{
//	int ret = CST_RET_RECOVERY_OLD_FOCUS;
//	strategy_focus_t *p_syf = &THIS_STG.sf;
	
	//行的切换
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
	return CLP_modify(arg, OP_ADD);
}
static int CLP_key_dn(void *arg)
{
//	int ret = CST_RET_RECOVERY_OLD_FOCUS;
//	strategy_focus_t *p_syf = &THIS_STG.sf;
	
	//行的切换
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
	
	return CLP_modify(arg, OP_SUB);
}

static int CLP_key_lt(void *arg)
{
//	strategy_focus_t *p_syf = &THIS_STG.sf;
//	int ret = CST_RET_RECOVERY_OLD_FOCUS;
	
//	if(p_syf->f_col == 0)
//	{
//		
//		p_syf->f_col = 1;
//		CLP_Col_1(CST_POS_LAST);
//	}
//	else
//	{
//		if(CLP_Col_1(CST_POS_SUB) < 0)
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
	
	CLP_Col_1(CST_POS_SUB);
	return CST_RET_RECOVERY_OLD_FOCUS;
}

static int CLP_key_rt(void *arg)
{
//	strategy_focus_t *p_syf = &THIS_STG.sf;
//	int ret = RET_OK;
	
//	if(p_syf->f_col == 0)
//	{
//		
//		p_syf->f_col = 1;
//		CLP_Col_1(CST_POS_FIRST);
//	}
//	else
//	{
//		if(CLP_Col_1(CST_POS_ADD) < 0)
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
	
	CLP_Col_1(CST_POS_ADD);
	return CST_RET_RECOVERY_OLD_FOCUS;
}
static int CLP_key_er(void *arg)
{
	
	return CLP_commit(NULL);
	
}




