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

static int DSP_entry(int row, int col, void *pp_text);
static int DSP_init(void *arg);
static int DSP_key_up(void *arg);
static int DSP_key_dn(void *arg);
static int DSP_key_lt(void *arg);
static int DSP_key_rt(void *arg);
static int DSP_key_er(void *arg);
static void DSP_Reset_focus(void);
static int DSP_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf);
static int DSP_commit(void *arg);
static void DSP_Exit(void);

strategy_t	cst_set_display = {
	DSP_entry,
	DSP_init,
	STG_Duild_button,
	DSP_key_up,
	DSP_key_dn,
	DSP_key_lt,
	DSP_key_rt,
	DSP_key_er,
	DSP_Reset_focus,
	DSP_get_focusdata,	
	DSP_commit,
	DSP_Exit,
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
#define		THIS_STG	cst_set_display

#define		THIS_NUM_ROWS		3
#define		THIS_MAX_ROW		2

#define		NEED_NUM_RAM		7
#define 	CACHE_BUF_NUM		6
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
 static char *const set_display_entrys[THIS_NUM_ROWS] = {"亮度", "灰度", "定时"};
	
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
 static int DSP_modify(void *arg, int op);
 static int DSP_Col_1(int pos);		//焦点在编辑区的移动
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int DSP_entry(int row, int col, void *pp_text)
{
	char **pp = (char **)pp_text;
	hmi_mgr_t	*p_h;
	if(row > THIS_MAX_ROW)
		return 0;
	if(col > 1)
		return 0;
	if(col == 0) {
		strcpy(arr_p_vram[STG_RAM_NUM(row, col)], set_display_entrys[row]);
		
		*pp = arr_p_vram[STG_RAM_NUM(row, col)];
		return strlen(*pp);
	} else if(col == 1) {
		
		p_h = (hmi_mgr_t	*)arr_p_vram[CACHE_BUF_NUM];
		
		if(row == 0)
		{
			p_h->lightness = aci_sys.hmi_mgr.lightness;
			Print_float(aci_sys.hmi_mgr.lightness, 4, 1, arr_p_vram[STG_RAM_NUM(row, col)]);
			
		}
		else if(row == 1){
			p_h->gray_levels = aci_sys.hmi_mgr.gray_levels;
			Print_float(aci_sys.hmi_mgr.gray_levels, 4, 1, arr_p_vram[STG_RAM_NUM(row, col)]);
			
		} 
		else
		{
			p_h->time_switch = aci_sys.hmi_mgr.time_switch;
			sprintf(arr_p_vram[STG_RAM_NUM(row, col)], "[%2d分]", aci_sys.hmi_mgr.time_switch);
			
		}
		*pp = arr_p_vram[STG_RAM_NUM(row, col)];
		return strlen(*pp);
	}
	
}

static int DSP_init(void *arg)
{
	int i = 0;
	memset(&THIS_STG.sf, 0, sizeof(THIS_STG.sf));
	
	THIS_STG.p_stg_title = "配置 显示";
	
	DSP_Reset_focus();
	
	THIS_STG.stg_num_rows = 3;
	HMI_Ram_init();
	for(i = 0; i < NEED_NUM_RAM; i++) {
		
		arr_p_vram[i] = HMI_Ram_alloc(48);
		memset(arr_p_vram[i], 0, 48);
	}
	
	aci_sys.key_weight = 1;
	
	return RET_OK;
}
static void DSP_Exit(void)
{
	
}
static int DSP_commit(void *arg)
{
	hmi_mgr_t	*p_h;
	
	p_h = (hmi_mgr_t	*)arr_p_vram[CACHE_BUF_NUM];
	
	aci_sys.hmi_mgr.time_switch = p_h->time_switch;
	aci_sys.hmi_mgr.gray_levels = p_h->gray_levels;
	aci_sys.hmi_mgr.lightness = p_h->lightness;
	
	
	
	
	return RET_OK;

}

static void DSP_Reset_focus(void)
{
	THIS_STG.sf.f_col = 0;
	THIS_STG.sf.f_row = 0;
	THIS_STG.sf.start_byte = 0;
	THIS_STG.sf.num_byte = 4;
	
}

static int DSP_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf)
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

static int DSP_key_up(void *arg)
{
	int ret = RET_OK;
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
		
		return ret;
	}
	return DSP_modify(arg, OP_ADD);
}
static int DSP_key_dn(void *arg)
{
	int ret = RET_OK;
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
		
		return ret;
	}
	
	return DSP_modify(arg, OP_SUB);
}

//正常移动范围内返回0
//超出范围：-1
//显示设置的编辑部分是个整体，不用考虑局部移动
static int DSP_Col_1(int pos)
{
	int  ret = 0;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	char				*p = arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)];
	p_syf->num_byte = strlen(p);
	p_syf->start_byte = 0;
	
	switch(pos)
	{
		
		case CST_POS_FIRST:
			p_syf->start_byte = 0;
			break;
		case CST_POS_LAST:
			p_syf->start_byte = 0;
			break;
		case CST_POS_ADD:
			ret = -1;
			break;
		case CST_POS_SUB:
			
			ret = -1;
			
			break;		
		
		
	}
	
	if(p_syf->f_row == 2)
	{
		p_syf->start_byte = 1;
		p_syf->num_byte -= 4;	//[0分] 要把"[分]"排除在外
		
	}
	
	return ret;
	
}
static int DSP_key_lt(void *arg)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	int ret = RET_OK;
	
	if(p_syf->f_col == 0)
	{
		
		p_syf->f_col = 1;
		DSP_Col_1(CST_POS_LAST);
	}
	else
	{
		if(DSP_Col_1(CST_POS_SUB) < 0)
		{
			
			
			p_syf->f_col = 0;
			p_syf->start_byte = 0;
			p_syf->num_byte = 4;
			
		}
		
		
	}
	
	
	return ret;
}

static int DSP_key_rt(void *arg)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	int ret = RET_OK;
	
	if(p_syf->f_col == 0)
	{
		
		p_syf->f_col = 1;
		DSP_Col_1(CST_POS_FIRST);
	}
	else
	{
		if(DSP_Col_1(CST_POS_ADD) < 0)
		{
			
			
			p_syf->f_col = 0;
			p_syf->start_byte = 0;
			p_syf->num_byte = 4;
			
		}
		
		
	}
	
	
	return ret;
}
static int DSP_key_er(void *arg)
{
	
	return DSP_commit(NULL);
	
}



static int DSP_modify(void *arg, int op)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	dev_lcd *p_lcd;
	
	
	hmi_mgr_t	*p_h;
	
	p_h = (hmi_mgr_t	*)arr_p_vram[CACHE_BUF_NUM];
	
	
	
	Dev_open(DEVID_FM12864, (void *)&p_lcd);
	
	

	switch(p_syf->f_row)
	{
		
		case 0:
			p_h->lightness = Operate_in_tange_keep(p_h->lightness, op, aci_sys.key_weight, MIN_LIGHTNESS, 1000);
			Print_float(p_h->lightness, 4, 1, arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)]);
			p_lcd->lcd_lightness(p_h->lightness);
			break;
		case 1:
			p_h->gray_levels = Operate_in_tange_keep(p_h->gray_levels, op, aci_sys.key_weight, 0, 1000);
			Print_float(p_h->gray_levels, 4, 1, arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)]);
			break;
		case 2:
			p_h->time_switch = Operate_in_tange(p_h->time_switch, op, aci_sys.key_weight, 0, 99);
			sprintf(arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)], "[%2d分]", p_h->time_switch);
			break;
	}
	//更新一下焦点的长度
	DSP_Col_1(CST_POS_HOLD);
	
	return CST_RET_CLEAN_OLD_FOCUS;
}
