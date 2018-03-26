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

static int PRP_entry(int row, int col, void *pp_text);
static int PRP_init(void *arg);
static int PRP_key_up(void *arg);
static int PRP_key_dn(void *arg);
static int PRP_key_lt(void *arg);
static int PRP_key_rt(void *arg);
static int PRP_key_er(void *arg);
static void PRP_Reset_focus(void);
static int PRP_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf);
static int PRP_commit(void *arg);
static void PRP_Exit(void);

strategy_t	cst_set_prs_protect = {
	PRP_entry,
	PRP_init,
	STG_Duild_button,
	PRP_key_up,
	PRP_key_dn,
	PRP_key_lt,
	PRP_key_rt,
	PRP_key_er,
	PRP_Reset_focus,
	PRP_get_focusdata,	
	PRP_commit,
	PRP_Exit,
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
#define		THIS_STG	cst_set_prs_protect

#define		THIS_NUM_ROWS		3
#define		THIS_MAX_ROW		2

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
 static char *const set_password_entrys[THIS_NUM_ROWS] = {"粉尘", "大气A", "大气B"};
	
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
 static int PRP_modify(void *arg, int op);
 static int PRP_Col_1(int pos);		//焦点在编辑区的移动
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int PRP_entry(int row, int col, void *pp_text)
{
	char **pp = (char **)pp_text;
	atmosphere_conf_t	*p_a;
	if(row > THIS_MAX_ROW)
		return 0;
	if(col > 1)
		return 0;
	if(col == 0) {
		strcpy(arr_p_vram[STG_RAM_NUM(row, col)], set_password_entrys[row]);
		
		*pp = arr_p_vram[STG_RAM_NUM(row, col)];
		return strlen(*pp);
	} else if(col == 1) {
		
		p_a = (atmosphere_conf_t *)arr_p_vram[CACHE_BUF_NUM];
		switch(row)
		{
			case 0:
				p_a->prtct_dust = aci_sys.atm_conf.prtct_dust;
				Print_atmosphere_param(&p_a->prtct_dust, arr_p_vram[STG_RAM_NUM(0, 1)]	, 48, ea_prtct_dust);
				break;
			case 1:
				p_a->prtct_atmsphr_A = aci_sys.atm_conf.prtct_atmsphr_A;
				Print_atmosphere_param(&p_a->prtct_atmsphr_A, arr_p_vram[STG_RAM_NUM(1, 1)]	, 48, ea_prtct_atmsphr_A);
				break;
			case 2:
				p_a->prtct_atmsphr_B = aci_sys.atm_conf.prtct_atmsphr_B;
				Print_atmosphere_param(&p_a->prtct_atmsphr_B, arr_p_vram[STG_RAM_NUM(2, 1)]	, 48, ea_prtct_atmsphr_B);
				break;
			
		}
		
		
		*pp = arr_p_vram[STG_RAM_NUM(row, 1)];
		return strlen(*pp);
	}
	return 0;
}

static int PRP_init(void *arg)
{
	int i = 0;
	memset(&THIS_STG.sf, 0, sizeof(THIS_STG.sf));
	
	THIS_STG.p_stg_title = "设置压力保护";
	
	PRP_Reset_focus();
	
	THIS_STG.stg_num_rows = THIS_NUM_ROWS;
	HMI_Ram_init();
	for(i = 0; i < NEED_NUM_RAM; i++) {
		
		arr_p_vram[i] = HMI_Ram_alloc(48);
		memset(arr_p_vram[i], 0, 48);
	}
	
	aci_sys.key_weight = 1;
	
	return RET_OK;
}
static void PRP_Exit(void)
{
	
}
static int PRP_commit(void *arg)
{
	atmosphere_conf_t	*p_a;
	p_a = (atmosphere_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	
	aci_sys.atm_conf.prtct_dust =  p_a->prtct_dust;
	aci_sys.atm_conf.prtct_atmsphr_A =  p_a->prtct_atmsphr_A;
	aci_sys.atm_conf.prtct_atmsphr_B =  p_a->prtct_atmsphr_B;
	
	return RET_OK;

}

static void PRP_Reset_focus(void)
{
	THIS_STG.sf.f_col = 0;
	THIS_STG.sf.f_row = 0;
	THIS_STG.sf.start_byte = 0;
	THIS_STG.sf.num_byte = 4;
	
}

static int PRP_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf)
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

static int PRP_key_up(void *arg)
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
			p_syf->f_row = THIS_MAX_ROW;
		}
		
		if(p_syf->f_row > 0)
			p_syf->num_byte = 5;
		else
			p_syf->num_byte = 4;
		
		return ret;
	}
	return PRP_modify(arg, OP_ADD);
}
static int PRP_key_dn(void *arg)
{
	int ret = CST_RET_RECOVERY_OLD_FOCUS;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	
	//行的切换
	if(p_syf->f_col == 0)
	{
		if(p_syf->f_row < THIS_MAX_ROW)
			p_syf->f_row ++;
		else
		{
			p_syf->f_row = 0;
		}
		if(p_syf->f_row > 0)
			p_syf->num_byte = 5;
		else
			p_syf->num_byte = 4;
		
		return ret;
	}
	
	return PRP_modify(arg, OP_SUB);
}

//正常移动范围内返回0
//超出范围：-1
//0.00KPa
static int PRP_Col_1(int pos)
{
	int  ret = 0;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	char				*p = arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)];
	p_syf->num_byte = strlen(p) - 3;		//去掉KPa
	p_syf->start_byte = 0;
	switch(pos)
	{
		
//		case CST_POS_FIRST:
//			p_syf->start_byte = 1;
//			break;
//		case CST_POS_LAST:
//			p_syf->start_byte = 1;
//			break;
		case CST_POS_ADD:
			
			ret = -1;
				
			
			break;
		case CST_POS_SUB:
			
			
			ret = -1;
				
			
			
			break;		
		
		
	}
	
	
	
	return ret;
	
}
static int PRP_key_lt(void *arg)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	int ret = CST_RET_RECOVERY_OLD_FOCUS;
	
	if(p_syf->f_col == 0)
	{
		
		p_syf->f_col = 1;
		PRP_Col_1(CST_POS_LAST);
	}
	else
	{
		if(PRP_Col_1(CST_POS_SUB) < 0)
		{
			
			
			p_syf->f_col = 0;
			p_syf->start_byte = 0;
			p_syf->num_byte = 4;
			
		}
		
		
	}
	
	
	return ret;
}

static int PRP_key_rt(void *arg)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	int ret = RET_OK;
	
	if(p_syf->f_col == 0)
	{
		
		p_syf->f_col = 1;
		PRP_Col_1(CST_POS_FIRST);
	}
	else
	{
		if(PRP_Col_1(CST_POS_ADD) < 0)
		{
			
			
			p_syf->f_col = 0;
			p_syf->start_byte = 0;
			p_syf->num_byte = 4;
			
		}
		
		
	}
	
	
	return ret;
}
static int PRP_key_er(void *arg)
{
	
	return PRP_commit(NULL);
	
}



static int PRP_modify(void *arg, int op)
{
	
	
	strategy_focus_t *p_syf = &THIS_STG.sf;
	atmosphere_conf_t	*p_a;
	p_a = (atmosphere_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	
	switch(p_syf->f_row)
	{
		case 0:
			p_a->prtct_dust = Operate_in_tange(p_a->prtct_dust, op, aci_sys.key_weight, 0, 20000);
			Print_atmosphere_param(&p_a->prtct_dust, arr_p_vram[STG_RAM_NUM(p_syf->f_row, 1)]	, 48, ea_prtct_dust);
			break;
		case 1:
			p_a->prtct_atmsphr_A = Operate_in_tange(p_a->prtct_atmsphr_A, op, aci_sys.key_weight, 0, 20000);
			Print_atmosphere_param(&p_a->prtct_atmsphr_A, arr_p_vram[STG_RAM_NUM(p_syf->f_row, 1)]	, 48, ea_prtct_atmsphr_A);
				
			break;
		case 2:
			p_a->prtct_atmsphr_B = Operate_in_tange(p_a->prtct_atmsphr_B, op, aci_sys.key_weight, 0, 20000);
			Print_atmosphere_param(&p_a->prtct_atmsphr_B, arr_p_vram[STG_RAM_NUM(p_syf->f_row, 1)]	, 48, ea_prtct_atmsphr_B);
			break;
		
	}
	
	//更新一下焦点的长度
	PRP_Col_1(CST_POS_HOLD);
	return CST_RET_CLEAN_OLD_FOCUS;
}
