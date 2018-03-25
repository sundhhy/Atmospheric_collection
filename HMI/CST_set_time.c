#include <stdint.h>
#include "configure_strategy.h"
#include "HMI.h"
#include "utils/PHN_string.h"
//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------

static int TIM_entry(int row, int col, void *pp_text);
static int TIM_init(void *arg);
static int TIM_key_up(void *arg);
static int TIM_key_dn(void *arg);
static int TIM_key_lt(void *arg);
static int TIM_key_rt(void *arg);
static int TIM_key_er(void *arg);
static void TIM_Reset_focus(void);
static int TIM_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf);
static int TIM_commit(void *arg);
static void TIM_Exit(void);

strategy_t	cst_set_time = {
	TIM_entry,
	TIM_init,
	STG_Duild_button,
	TIM_key_up,
	TIM_key_dn,
	TIM_key_lt,
	TIM_key_rt,
	TIM_key_er,
	TIM_Reset_focus,
	TIM_get_focusdata,	
	TIM_commit,
	TIM_Exit,
};

// pos : 0 不变  1 第一个位置 2  最后的位置 3 加1 4 减1
#define TIM_POS_HOLD		0
#define TIM_POS_FIRST		1
#define TIM_POS_LAST		2
#define TIM_POS_ADD			3
#define TIM_POS_SUB			4

//static int Cns_init(void *arg);
//static int Cns_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf);
//strategy_t	THIS_STG = {
//	ChnStrategy_entry,
//	Cns_init,
//	Cns_key_up,
//	Cns_key_dn,
//	Cns_key_lt,
//	Cns_key_rt,
//	Cns_key_er,
//	Cns_get_focusdata,
//};
//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//
#define		THIS_STG	cst_set_time
	
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
 static char *const set_time_entrys[2] = {"日期", "时间"};
	
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
 static int TIM_modify(void *arg, int op);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int TIM_entry(int row, int col, void *pp_text)
{
	char **pp = (char **)pp_text;
	if(row > 1)
		return 0;
	if(col > 1)
		return 0;
	if(col == 0) {
		strcpy(arr_p_vram[STG_RAM_NUM(row, col)], set_time_entrys[row]);
		
		*pp = arr_p_vram[STG_RAM_NUM(row, col)];
		return strlen(*pp);
	} else if(col == 1) {
		if(row == 0)
			sprintf(arr_p_vram[STG_RAM_NUM(row, col)], "%02d/%02d/%02d", \
				aci_sys.sys_time.tm_year, aci_sys.sys_time.tm_mon, aci_sys.sys_time.tm_mday);
		else {
			sprintf(arr_p_vram[STG_RAM_NUM(row, col)], "%02d:%02d:%02d", \
				aci_sys.sys_time.tm_hour, aci_sys.sys_time.tm_min, aci_sys.sys_time.tm_sec);
		}
		*pp = arr_p_vram[STG_RAM_NUM(row, col)];
		return strlen(*pp);
	}
	
}

static int TIM_init(void *arg)
{
	int i = 0;
	memset(&THIS_STG.sf, 0, sizeof(THIS_STG.sf));
	
	THIS_STG.p_stg_title = NULL;
	
	TIM_Reset_focus();
	
	THIS_STG.stg_num_rows = 2;
	HMI_Ram_init();
	for(i = 0; i < 4; i++) {
		
		arr_p_vram[i] = HMI_Ram_alloc(48);
		memset(arr_p_vram[i], 0, 48);
	}
	
	aci_sys.key_weight = 1;
	
	return RET_OK;
}
static void TIM_Exit(void)
{
	
}
static int TIM_commit(void *arg)
{
	char	*p;
	uint8_t		err;
	p = arr_p_vram[STG_RAM_NUM(0, 1)];
	
	aci_sys.sys_time.tm_year = Get_str_data(p, "/", 0, &err);
	aci_sys.sys_time.tm_mon = Get_str_data(p, "/", 1, &err);
	aci_sys.sys_time.tm_mday = Get_str_data(p, "/", 2, &err);
	
	p = arr_p_vram[STG_RAM_NUM(1, 1)];
	aci_sys.sys_time.tm_hour = Get_str_data(p, ":", 0, &err);
	aci_sys.sys_time.tm_min = Get_str_data(p, ":", 1, &err);
	aci_sys.sys_time.tm_sec = Get_str_data(p, ":", 2, &err);
	
	return RET_OK;

}

static void TIM_Reset_focus(void)
{
	THIS_STG.sf.f_col = 0;
	THIS_STG.sf.f_row = 0;
	THIS_STG.sf.start_byte = 0;
	THIS_STG.sf.num_byte = 4;
	
}

static int TIM_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	char		**pp_vram = (char **)pp_data;
	
	if(p_syf->f_row > 1) {
		return -1;
	}
	
	if(p_in_syf)
		p_syf = p_in_syf;
	
	
	
	*pp_vram = arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)];

	return strlen(*pp_vram);
	
}

static int TIM_key_up(void *arg)
{
	int ret = CST_RET_RECOVERY_OLD_FOCUS;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	
	//行的切换
	if(p_syf->f_col == 0)
	{
		if(p_syf->f_row > 0)
			p_syf->f_row --;
		else
		{
			p_syf->f_row = 1;
		}
		
		return ret;
	}
	return TIM_modify(arg, OP_ADD);
}
static int TIM_key_dn(void *arg)
{
	int ret = RET_OK;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	
	//行的切换
	if(p_syf->f_col == 0)
	{
		if(p_syf->f_row < 1)
			p_syf->f_row ++;
		else
		{
			p_syf->f_row = 0;
		}
		
		return ret;
	}
	
	return TIM_modify(arg, OP_SUB);
}

//正常移动范围内返回0
//超出范围：-1
//00/00/00
//00:00:00
static int TIM_Col_1(int pos)
{
	int  ret = 0;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	char				*p = arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)];
	p_syf->num_byte = 1;
	switch(pos)
	{
		case TIM_POS_HOLD:
			
			break;
		case TIM_POS_FIRST:
			p_syf->start_byte = 0;
			break;
		case TIM_POS_LAST:
			p_syf->start_byte = 7;
			break;
		case TIM_POS_ADD:
			if(p_syf->start_byte < 7)
			{
				p_syf->start_byte ++;
				if((p[p_syf->start_byte] > '9') || (p[p_syf->start_byte] < '0'))
					p_syf->start_byte ++;
			}
			else
			{
				p_syf->start_byte = 0;
				ret = -1;
			}
			break;
		case TIM_POS_SUB:
			if(p_syf->start_byte > 0)
			{
				p_syf->start_byte --;
				if((p[p_syf->start_byte] > '9') || (p[p_syf->start_byte] < '0'))
					p_syf->start_byte --;
				
			}
			else
			{
				p_syf->start_byte = 7;
				ret = -1;
			}
			break;		
		
		
	}
	
	return ret;
	
}
static int TIM_key_lt(void *arg)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	int ret = RET_OK;
	
	if(p_syf->f_col == 0)
	{
		
		p_syf->f_col = 1;
		TIM_Col_1(TIM_POS_LAST);
	}
	else
	{
		if(TIM_Col_1(TIM_POS_SUB) < 0)
		{
			
			
			p_syf->f_col = 0;
			p_syf->start_byte = 0;
			p_syf->num_byte = 4;
			
		}
		
		
	}
	
	
	return ret;
}

static int TIM_key_rt(void *arg)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	int ret = RET_OK;
	
	if(p_syf->f_col == 0)
	{
		
		p_syf->f_col = 1;
		TIM_Col_1(TIM_POS_FIRST);
	}
	else
	{
		if(TIM_Col_1(TIM_POS_ADD) < 0)
		{
			
			
			p_syf->f_col = 0;
			p_syf->start_byte = 0;
			p_syf->num_byte = 4;
			
		}
		
		
	}
	
	
	return ret;
}
static int TIM_key_er(void *arg)
{
	
	return TIM_commit(NULL);
	
}



static int TIM_modify(void *arg, int op)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	char		*p = arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)];
	
	
	Str_Calculations(p + p_syf->start_byte, 1,  op, 1, 0, 9);
	
	return CST_RET_CLEAN_OLD_FOCUS;
}
