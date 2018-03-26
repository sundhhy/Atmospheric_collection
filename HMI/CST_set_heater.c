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

static int HTR_entry(int row, int col, void *pp_text);
static int HTR_init(void *arg);
static int HTR_key_up(void *arg);
static int HTR_key_dn(void *arg);
static int HTR_key_lt(void *arg);
static int HTR_key_rt(void *arg);
static int HTR_key_er(void *arg);
static void HTR_Reset_focus(void);
static int HTR_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf);
static int HTR_commit(void *arg);
static void HTR_Exit(void);

strategy_t	cst_set_heater = {
	HTR_entry,
	HTR_init,
	STG_Duild_button,
	HTR_key_up,
	HTR_key_dn,
	HTR_key_lt,
	HTR_key_rt,
	HTR_key_er,
	HTR_Reset_focus,
	HTR_get_focusdata,	
	HTR_commit,
	HTR_Exit,
};


//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//
#define		THIS_STG	cst_set_heater

#define		THIS_NUM_ROWS		2
#define		THIS_MAX_ROW		2

#define		NEED_NUM_RAM			5
#define 	CACHE_BUF_NUM			4
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
 static char *const set_heater_entrys[THIS_NUM_ROWS] = {"开关", "设置温度"};
	
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
 static int HTR_modify(void *arg, int op);
 static int HTR_Col_1(int pos);		//焦点在编辑区的移动
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int HTR_entry(int row, int col, void *pp_text)
{
	char **pp = (char **)pp_text;
	system_conf_t	*p_s;
	if(row > THIS_MAX_ROW)
		return 0;
	if(col > 1)
		return 0;
	if(col == 0) {
		strcpy(arr_p_vram[STG_RAM_NUM(row, col)], set_heater_entrys[row]);
		
		*pp = arr_p_vram[STG_RAM_NUM(row, col)];
		return strlen(*pp);
	} else if(col == 1) {
		
		p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
		switch(row)
		{
			case 0:
				p_s->heater_switch = aci_sys.sys_conf.heater_switch;
				Print_able(arr_p_vram[STG_RAM_NUM(0, 1)]	, p_s->heater_switch);
				break;
			case 1:
				p_s->heater_temperature = aci_sys.sys_conf.heater_temperature;
				Print_float(p_s->heater_temperature, 4, 1, arr_p_vram[STG_RAM_NUM(1, 1)]);
				strcat(arr_p_vram[STG_RAM_NUM(1, 1)], "℃");
				break;
		
			
		}
		
		
		*pp = arr_p_vram[STG_RAM_NUM(row, 1)];
		return strlen(*pp);
	}
	return 0;
}

static int HTR_init(void *arg)
{
	int i = 0;
	memset(&THIS_STG.sf, 0, sizeof(THIS_STG.sf));
	
	THIS_STG.p_stg_title = "配置加热器";
	
	HTR_Reset_focus();
	
	THIS_STG.stg_num_rows = THIS_NUM_ROWS;
	HMI_Ram_init();
	for(i = 0; i < NEED_NUM_RAM; i++) {
		
		arr_p_vram[i] = HMI_Ram_alloc(48);
		memset(arr_p_vram[i], 0, 48);
	}
	
	aci_sys.key_weight = 1;
	
	return RET_OK;
}
static void HTR_Exit(void)
{
	
}
static int HTR_commit(void *arg)
{
	system_conf_t	*p_s;
	p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	aci_sys.sys_conf.heater_switch = p_s->heater_switch;
	aci_sys.sys_conf.heater_temperature = p_s->heater_temperature;
	
	return RET_OK;

}

static int HTR_modify(void *arg, int op)
{
	
	
	strategy_focus_t *p_syf = &THIS_STG.sf;
	system_conf_t	*p_s;
	p_s = (system_conf_t *)arr_p_vram[CACHE_BUF_NUM];
	
	switch(p_syf->f_row)
	{
		case 0:
			p_s->heater_switch = Operate_in_tange(p_s->heater_switch, op, 1, 0, 1);
			Print_able(arr_p_vram[STG_RAM_NUM(0, 1)]	, p_s->heater_switch);
			break;
		case 1:
			p_s->heater_temperature = Operate_in_tange(p_s->heater_temperature, op, aci_sys.key_weight, 0, 1000);
			Print_float(p_s->heater_temperature, 4, 1, arr_p_vram[STG_RAM_NUM(1, 1)]);
			strcat(arr_p_vram[STG_RAM_NUM(1, 1)], "℃");
				
			break;
		
		
	}
	
	//更新一下焦点的长度
	HTR_Col_1(CST_POS_HOLD);
	return CST_RET_CLEAN_OLD_FOCUS;
}

static void HTR_Reset_focus(void)
{
	THIS_STG.sf.f_col = 0;
	THIS_STG.sf.f_row = 0;
	THIS_STG.sf.start_byte = 0;
	THIS_STG.sf.num_byte = 4;
	
}

static int HTR_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf)
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

static int HTR_key_up(void *arg)
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
		
		if(p_syf->f_row == 1)
			p_syf->num_byte = 8;
		else
			p_syf->num_byte = 4;
		
		return ret;
	}
	return HTR_modify(arg, OP_ADD);
}
static int HTR_key_dn(void *arg)
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
		if(p_syf->f_row == 1)
			p_syf->num_byte = 8;
		else
			p_syf->num_byte = 4;
		
		return ret;
	}
	
	return HTR_modify(arg, OP_SUB);
}

//正常移动范围内返回0
//超出范围：-1
//编辑区:
//[启用]
//26.9℃
static int HTR_Col_1(int pos)
{
	int  ret = 0;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	char				*p = arr_p_vram[STG_RAM_NUM(p_syf->f_row, p_syf->f_col)];
	
	
	if(p_syf->f_row == 0)
	{
		p_syf->num_byte = strlen(p) - 2;		//去掉[]
		p_syf->start_byte = 1;
		
	}
	else
	{
		p_syf->num_byte = strlen(p) - 2;		//去掉℃
		p_syf->start_byte = 0;
		
	}
	
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
static int HTR_key_lt(void *arg)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	int ret = CST_RET_RECOVERY_OLD_FOCUS;
	
	if(p_syf->f_col == 0)
	{
		
		p_syf->f_col = 1;
		HTR_Col_1(CST_POS_LAST);
	}
	else
	{
		if(HTR_Col_1(CST_POS_SUB) < 0)
		{
			
			
			p_syf->f_col = 0;
			p_syf->start_byte = 0;
			if(p_syf->f_row == 1)
				p_syf->num_byte = 8;
			else
				p_syf->num_byte = 4;
			
		}
		
		
	}
	
	
	return ret;
}

static int HTR_key_rt(void *arg)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	int ret = RET_OK;
	
	if(p_syf->f_col == 0)
	{
		
		p_syf->f_col = 1;
		HTR_Col_1(CST_POS_FIRST);
	}
	else
	{
		if(HTR_Col_1(CST_POS_ADD) < 0)
		{
			
			
			p_syf->f_col = 0;
			p_syf->start_byte = 0;
			if(p_syf->f_row == 1)
				p_syf->num_byte = 8;
			else
				p_syf->num_byte = 4;
			
		}
		
		
	}
	
	
	return ret;
}
static int HTR_key_er(void *arg)
{
	
	return HTR_commit(NULL);
	
}




