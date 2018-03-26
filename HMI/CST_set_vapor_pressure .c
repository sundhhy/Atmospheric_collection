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

static int VPP_entry(int row, int col, void *pp_text);
static int VPP_init(void *arg);
static int VPP_key_up(void *arg);
static int VPP_key_dn(void *arg);
static int VPP_key_lt(void *arg);
static int VPP_key_rt(void *arg);
static int VPP_key_er(void *arg);
static void VPP_Reset_focus(void);
static int VPP_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf);
static int VPP_commit(void *arg);
static void VPP_Exit(void);

strategy_t	cst_set_vapor = {
	VPP_entry,
	VPP_init,
	STG_Duild_button,
	VPP_key_up,
	VPP_key_dn,
	VPP_key_lt,
	VPP_key_rt,
	VPP_key_er,
	VPP_Reset_focus,
	VPP_get_focusdata,	
	VPP_commit,
	VPP_Exit,
};


//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//
/*
→ [不 计 算]
  [参与计算]
为了方便显示特效，把箭头及说明作为同一列
*/
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define		THIS_STG	cst_set_vapor


#define		THIS_NUM_ROWS		2
#define		THIS_MAX_ROW		1
#define		THIS_MAX_COL		0

#define		NEED_NUM_RAM			3
#define 	CACHE_BUF_NUM			2

//用行号作为第一个下标索引
//用aci_sys.sys_conf.vapor_calculate作为第二个下标索引
static char *const vpp_texts[2][2] = { \
		{{"→ [不 计 算]"}, {"   [不 计 算]"}},
		{{"  [参与计算]"}, {"→  [参与计算]"}},
	};
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
	
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
 static int VPP_modify(void *arg, int op);
 static int VPP_Col_1(int pos);		//焦点在编辑区的移动
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int VPP_entry(int row, int col, void *pp_text)
{
	char **pp = (char **)pp_text;
	system_conf_t	*p_s;
	if(row > THIS_MAX_ROW)
		return 0;
	if(col > THIS_MAX_COL)
		return 0;
	
	
	
	p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	if(aci_sys.sys_conf.vapor_calculate > 1)
		aci_sys.sys_conf.vapor_calculate = 1;
	p_s->vapor_calculate = aci_sys.sys_conf.vapor_calculate;
	*pp = vpp_texts[row][p_s->vapor_calculate];
	return strlen(*pp);

}

static int VPP_init(void *arg)
{
	int i = 0;
	system_conf_t	*p_s;
	
	
	memset(&THIS_STG.sf, 0, sizeof(THIS_STG.sf));
	
	THIS_STG.p_stg_title = "计算饱和水汽压";
	
	
	
	THIS_STG.stg_num_rows = THIS_NUM_ROWS;
	HMI_Ram_init();
	for(i = 0; i < NEED_NUM_RAM; i++) {
		
		arr_p_vram[i] = HMI_Ram_alloc(48);
		memset(arr_p_vram[i], 0, 48);
	}
	p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	p_s->vapor_calculate = aci_sys.sys_conf.vapor_calculate;
	VPP_Reset_focus();
	
	aci_sys.key_weight = 1;
	
	return RET_OK;
}
static void VPP_Exit(void)
{
	
}
static int VPP_commit(void *arg)
{
	system_conf_t	*p_s;
	p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	aci_sys.sys_conf.vapor_calculate = p_s->vapor_calculate;
	
	return RET_OK;

}

//
//→ [不 计 算]
//  [参与计算]
static int VPP_modify(void *arg, int op)
{
	
	
	strategy_focus_t *p_syf = &THIS_STG.sf;
	system_conf_t	*p_s;
//	int						weight = 1;
//	short						i, j;
	p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	

	
	p_s->vapor_calculate = p_syf->f_row;
	
	
	//更新一下焦点的长度
	VPP_Col_1(CST_POS_HOLD);
	return CST_RET_RECOVERY_OLD_FOCUS;
}

/*

		{{"→ [不 计 算]"}, {"   [不 计 算]"}},
		{{"  [参与计算]"}, {"→  [参与计算]"}},

*/
static int VPP_Col_1(int pos)
{
	int  ret = 0;
	strategy_focus_t *p_syf = &THIS_STG.sf;
//	char				*p = arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)];
	
	//跳过→ [ ]
	p_syf->start_byte = 5;
	p_syf->num_byte = strlen("→  [参与计算]") - 6; 
	
	
	return ret;
}

static void VPP_Reset_focus(void)
{
	
	system_conf_t	*p_s;

	p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	
	THIS_STG.sf.f_col = 0;
	if(p_s->vapor_calculate)
	{
		THIS_STG.sf.f_row = 1;
		
	}
	else
	{
		THIS_STG.sf.f_row = 0;
		
	}
	VPP_Col_1(CST_POS_HOLD);
	

	
}

static int VPP_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	char		**pp_vram = (char **)pp_data;
	system_conf_t	*p_s;

	p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	if(p_syf->f_row > THIS_MAX_ROW) {
		return -1;
	}
	
	if(p_in_syf)
		p_syf = p_in_syf;
	
	
	
	*pp_vram = vpp_texts[p_syf->f_row][p_s->vapor_calculate];;

	return strlen(*pp_vram);
	
}

static int VPP_key_up(void *arg)
{
//	int ret = CST_RET_RECOVERY_OLD_FOCUS;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	
	//行的切换
//	if(p_syf->f_col == 0)
//	{
		if(p_syf->f_row > 0)
			p_syf->f_row --;
		else
		{
			p_syf->f_row = THIS_MAX_ROW;
		}
//		
//		if(p_syf->f_row == 1)
//			p_syf->num_byte = 8;
//		else
//			p_syf->num_byte = 4;
//		
//		return ret;
//	}
	return VPP_modify(arg, OP_ADD);
}
static int VPP_key_dn(void *arg)
{
//	int ret = CST_RET_RECOVERY_OLD_FOCUS;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	
	//行的切换
//	if(p_syf->f_col == 0)
//	{
		if(p_syf->f_row < THIS_MAX_ROW)
			p_syf->f_row ++;
		else
		{
			p_syf->f_row = 0;
		}
//		if(p_syf->f_row == 1)
//			p_syf->num_byte = 8;
//		else
//			p_syf->num_byte = 4;
//		
//		return ret;
//	}
	
	return VPP_modify(arg, OP_SUB);
}

static int VPP_key_lt(void *arg)
{
//	strategy_focus_t *p_syf = &THIS_STG.sf;
//	int ret = CST_RET_RECOVERY_OLD_FOCUS;
//	
//	if(p_syf->f_col == 0)
//	{
//		
//		p_syf->f_col = 1;
//		VPP_Col_1(CST_POS_LAST);
//	}
//	else
//	{
//		if(VPP_Col_1(CST_POS_SUB) < 0)
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
	
	
	return CST_RET_FOCUS_HOLD;
}

static int VPP_key_rt(void *arg)
{
//	strategy_focus_t *p_syf = &THIS_STG.sf;
//	int ret = RET_OK;
//	
//	if(p_syf->f_col == 0)
//	{
//		
//		p_syf->f_col = 1;
//		VPP_Col_1(CST_POS_FIRST);
//	}
//	else
//	{
//		if(VPP_Col_1(CST_POS_ADD) < 0)
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
//	
	
	return CST_RET_FOCUS_HOLD;
}
static int VPP_key_er(void *arg)
{
	
	return VPP_commit(NULL);
	
}




