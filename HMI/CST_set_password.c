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

static int PSD_entry(int row, int col, void *pp_text);
static int PSD_init(void *arg);
static int PSD_key_up(void *arg);
static int PSD_key_dn(void *arg);
static int PSD_key_lt(void *arg);
static int PSD_key_rt(void *arg);
static int PSD_key_er(void *arg);
static void PSD_Reset_focus(void);
static int PSD_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf);
static int PSD_commit(void *arg);
static void PSD_Exit(void);

strategy_t	cst_set_password = {
	PSD_entry,
	PSD_init,
	STG_Duild_button,
	PSD_key_up,
	PSD_key_dn,
	PSD_key_lt,
	PSD_key_rt,
	PSD_key_er,
	PSD_Reset_focus,
	PSD_get_focusdata,	
	PSD_commit,
	PSD_Exit,
};



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
#define		THIS_STG	cst_set_password

#define		THIS_NUM_ROWS		1
#define		THIS_MAX_ROW		0

#define		NEED_NUM_RAM			3
#define 	CACHE_BUF_NUM			2
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
 static char *const set_password_entrys[THIS_NUM_ROWS] = {"密码"};
	
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
 static int PSD_modify(void *arg, int op);
 static int PSD_Col_1(int pos);		//焦点在编辑区的移动
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int PSD_entry(int row, int col, void *pp_text)
{
	char **pp = (char **)pp_text;
	if(row > THIS_MAX_ROW)
		return 0;
	if(col > 1)
		return 0;
	if(col == 0) {
		strcpy(arr_p_vram[STG_RAM_NUM(row, col)], set_password_entrys[row]);
		
		*pp = arr_p_vram[STG_RAM_NUM(row, col)];
		return strlen(*pp);
	} else if(col == 1) {
		arr_p_vram[CACHE_BUF_NUM][0] = aci_sys.sys_conf.password[0];
		arr_p_vram[CACHE_BUF_NUM][1] = aci_sys.sys_conf.password[1];
		sprintf(arr_p_vram[STG_RAM_NUM(0, 1)], "%02d%02d", arr_p_vram[CACHE_BUF_NUM][0], arr_p_vram[CACHE_BUF_NUM][1]);
			
		
		*pp = arr_p_vram[STG_RAM_NUM(0, 1)];
		return strlen(*pp);
	}
	return 0;
}

static int PSD_init(void *arg)
{
	int i = 0;
	memset(&THIS_STG.sf, 0, sizeof(THIS_STG.sf));
	
	THIS_STG.p_stg_title = "配置 密码";
	
	PSD_Reset_focus();
	
	THIS_STG.stg_num_rows = 3;
	HMI_Ram_init();
	for(i = 0; i < NEED_NUM_RAM; i++) {
		
		arr_p_vram[i] = HMI_Ram_alloc(48);
		memset(arr_p_vram[i], 0, 48);
	}
	
	aci_sys.key_weight = 1;
	
	return RET_OK;
}
static void PSD_Exit(void)
{
	
}
static int PSD_commit(void *arg)
{
	
	
	
	Str_set_password(arr_p_vram[STG_RAM_NUM(0, 1)]);
	return RET_OK;

}

static void PSD_Reset_focus(void)
{
	THIS_STG.sf.f_col = 0;
	THIS_STG.sf.f_row = 0;
	THIS_STG.sf.start_byte = 0;
	THIS_STG.sf.num_byte = 4;
	
}

static int PSD_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf)
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

static int PSD_key_up(void *arg)
{
	int ret = RET_OK;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	
	//行的切换
	if(p_syf->f_col == 0)
	{
//		if(p_syf->f_row > 0)
//			p_syf->f_row --;
//		else
//		{
//			p_syf->f_row = THIS_MAX_ROW;
//		}
		
		return ret;
	}
	return PSD_modify(arg, OP_ADD);
}
static int PSD_key_dn(void *arg)
{
	int ret = RET_OK;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	
	//行的切换
	if(p_syf->f_col == 0)
	{
//		if(p_syf->f_row < THIS_NUM_ROWS)
//			p_syf->f_row ++;
//		else
//		{
//			p_syf->f_row = 0;
//		}
		
		return ret;
	}
	
	return PSD_modify(arg, OP_SUB);
}

//正常移动范围内返回0
//超出范围：-1
//0000
static int PSD_Col_1(int pos)
{
	int  ret = 0;
	strategy_focus_t *p_syf = &THIS_STG.sf;
//	char				*p = arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)];
	p_syf->num_byte = 1;
	
	switch(pos)
	{
		
		case CST_POS_FIRST:
			p_syf->start_byte = 0;
			break;
		case CST_POS_LAST:
			p_syf->start_byte = 3;
			break;
		case CST_POS_ADD:
			if(p_syf->start_byte < 3)
			{
				p_syf->start_byte ++;
			}
			else
			{
				ret = -1;
				
			}
			break;
		case CST_POS_SUB:
			
			if(p_syf->start_byte > 0)
			{
				p_syf->start_byte --;
			}
			else
			{
				ret = -1;
				
			}
			
			break;		
		
		
	}
	
	
	
	return ret;
	
}
static int PSD_key_lt(void *arg)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	int ret = RET_OK;
	
	if(p_syf->f_col == 0)
	{
		
		p_syf->f_col = 1;
		PSD_Col_1(CST_POS_LAST);
	}
	else
	{
		if(PSD_Col_1(CST_POS_SUB) < 0)
		{
			
			
			p_syf->f_col = 0;
			p_syf->start_byte = 0;
			p_syf->num_byte = 4;
			
		}
		
		
	}
	
	
	return ret;
}

static int PSD_key_rt(void *arg)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	int ret = RET_OK;
	
	if(p_syf->f_col == 0)
	{
		
		p_syf->f_col = 1;
		PSD_Col_1(CST_POS_FIRST);
	}
	else
	{
		if(PSD_Col_1(CST_POS_ADD) < 0)
		{
			
			
			p_syf->f_col = 0;
			p_syf->start_byte = 0;
			p_syf->num_byte = 4;
			
		}
		
		
	}
	
	
	return ret;
}
static int PSD_key_er(void *arg)
{
	
	return PSD_commit(NULL);
	
}



static int PSD_modify(void *arg, int op)
{
	
	
	strategy_focus_t *p_syf = &THIS_STG.sf;
	char		*p = arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)];
	
	Str_Calculations(p + p_syf->start_byte, 1,  op, 1, 0, 9);

	
	
	return CST_RET_CLEAN_OLD_FOCUS;
}
