#include <stdint.h>
#include "configure_strategy.h"
#include "HMI.h"
#include "utils/PHN_string.h"
#include "lcd_conf.h"
#include "Model.h"
//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------

static int SSM_entry(int row, int col, void *pp_text);
static int SSM_init(void *arg);
static int SSM_key_up(void *arg);
static int SSM_key_dn(void *arg);
static int SSM_key_lt(void *arg);
static int SSM_key_rt(void *arg);
static int SSM_key_er(void *arg);
static void SSM_Reset_focus(void);
static int SSM_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf);
static int SSM_commit(void *arg);
static void SSM_Exit(void);

strategy_t	cst_set_sample = {
	SSM_entry,
	SSM_init,
	STG_Duild_button,
	SSM_key_up,
	SSM_key_dn,
	SSM_key_lt,
	SSM_key_rt,
	SSM_key_er,
	SSM_Reset_focus,
	SSM_get_focusdata,	
	SSM_commit,
	SSM_Exit,
};


//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//
#define		THIS_STG	cst_set_sample

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
typedef struct {
	uint8_t		sig_type;		//0 : A 1:B 
	uint8_t		none[3];
	sample_conf_t		tmp_smp;
}ssm_t;
//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
 static char *const set_sample_entrys[THIS_NUM_ROWS] = {"采样时间", "间隔时间", "次数    "};
	
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
 static int SSM_modify(void *arg, int op);
 static int SSM_Col_1(int pos);		//焦点在编辑区的移动
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int SSM_entry(int row, int col, void *pp_text)
{
	char **pp = (char **)pp_text;
	ssm_t	*p_s;
	if(row > THIS_MAX_ROW)
		return 0;
	if(col > THIS_MAX_COL)
		return 0;
	if(col == 0)
	{
		*pp = set_sample_entrys[row];
		
	}
	else 
	{
		
		p_s =(ssm_t *)arr_p_vram[CACHE_BUF_NUM];
		switch(row)
		{
			case 0:
				
				sprintf(arr_p_vram[row], "%02d:%02d", p_s->tmp_smp.sample_hour,  p_s->tmp_smp.sample_min);
				break;
			case 1:
				
				sprintf(arr_p_vram[row], "%02d:%02d", p_s->tmp_smp.sample_gap_hour,  p_s->tmp_smp.sample_gap_min);
				break;
			
			case 2:
				
				sprintf(arr_p_vram[row], "%d", p_s->tmp_smp.sample_max_count);
				break;
		}
		
		
		*pp = arr_p_vram[row];
	}
	return strlen(*pp);
	

}

static int SSM_init(void *arg)
{
	int i = 0;
	ssm_t	*p_s;
	memset(&THIS_STG.sf, 0, sizeof(THIS_STG.sf));
	
	THIS_STG.p_stg_title = "采样设置";
	
	
	
	THIS_STG.stg_num_rows = THIS_NUM_ROWS;
	HMI_Ram_init();
	for(i = 0; i < NEED_NUM_RAM; i++) {
		
		arr_p_vram[i] = HMI_Ram_alloc(48);
		memset(arr_p_vram[i], 0, 48);
	}
	p_s =(ssm_t *)arr_p_vram[CACHE_BUF_NUM];
	p_s->sig_type = *(uint8_t *)arg;
	
	MDL_Clone_samp_conf(&p_s->tmp_smp, &arr_sample_conf[p_s->sig_type]);
	
	
	
	SSM_Reset_focus();
	aci_sys.key_weight = 1;
	
	return RET_OK;
}
static void SSM_Exit(void)
{
	
}
static int SSM_commit(void *arg)
{
	ssm_t	*p_s;
	p_s = (ssm_t *)arr_p_vram[CACHE_BUF_NUM];
	MDL_Clone_samp_conf(&arr_sample_conf[p_s->sig_type], &p_s->tmp_smp);
	return RET_OK;

}


static int SSM_modify(void *arg, int op)
{
	
	
	strategy_focus_t *p_syf = &THIS_STG.sf;
	ssm_t	*p_s;
	char	*p = arr_p_vram[p_syf->f_row];
//	short		weight;
//	char		up_limit, low_limit ;
	p_s = (ssm_t *)arr_p_vram[CACHE_BUF_NUM];
	
	
	if(p_syf->f_row == 2)
	{
		p_s->tmp_smp.sample_max_count = Operate_in_tange(p_s->tmp_smp.sample_max_count, op, aci_sys.key_weight, 0, RECORD_MAX_NUM);
		sprintf(arr_p_vram[p_syf->f_row], "%d", p_s->tmp_smp.sample_max_count);
		goto exit;
	}
	
	Str_Calculations(p + p_syf->start_byte, 1,  op, 1, 0, 9);
	
	//00:00
//	if((p_syf->start_byte == 0) || (p_syf->start_byte == 3))
//	{
//		weight = 10;
//		up_limit = 90;
//		
//	}
//	else
//	{
//		weight = 1;
//		up_limit = 9;
//	}
	
	
	
	switch(p_syf->f_row)
	{
		
		case 0:
			
		
		
			if(p_syf->start_byte < 3)
			{
				p_s->tmp_smp.sample_hour = Get_str_data(p,":", 0, NULL);
//				p_s->tmp_smp.sample_hour = Operate_in_tange(p_s->tmp_smp.sample_hour, op, weight, 0, up_limit);
//				sprintf(arr_p_vram[p_syf->f_row], "%02d:%02d", p_s->tmp_smp.sample_hour,p_s->tmp_smp.sample_min);
			}
			else
			{
				p_s->tmp_smp.sample_min = Get_str_data(p,":", 1, NULL);
//				p_s->tmp_smp.sample_min = Operate_in_tange(p_s->tmp_smp.sample_min, op, weight, 0, up_limit);
//				sprintf(arr_p_vram[p_syf->f_row], "%02d:%02d", p_s->tmp_smp.sample_hour,p_s->tmp_smp.sample_min);
				
			}
				
			break;
		case 1:
			if(p_syf->start_byte < 3)
			{
				p_s->tmp_smp.sample_gap_hour = Get_str_data(p,":", 0, NULL);
//				p_s->tmp_smp.sample_gap_hour = Operate_in_tange(p_s->tmp_smp.sample_gap_hour, op, weight, 0, up_limit);
//				sprintf(arr_p_vram[p_syf->f_row], "%02d:%02d", p_s->tmp_smp.sample_gap_hour, p_s->tmp_smp.sample_gap_min);
			}
			else
			{
				p_s->tmp_smp.sample_gap_min = Get_str_data(p,":", 1, NULL);
//				p_s->tmp_smp.sample_gap_min = Operate_in_tange(p_s->tmp_smp.sample_gap_min, op, weight, 0, up_limit);
//				sprintf(arr_p_vram[p_syf->f_row], "%02d:%02d", p_s->tmp_smp.sample_gap_hour, p_s->tmp_smp.sample_gap_min);

				
			}
		
		
			break;
		
		
	}
	
	exit:
	//更新一下焦点的长度
	SSM_Col_1(CST_POS_HOLD);
	return CST_RET_CLEAN_OLD_FOCUS;
}

//正常移动范围内返回0
//超出范围：-1
//编辑区:
//14:13
//00:00
//10000

static int SSM_Col_1(int pos)
{
	int  ret = 0;
	strategy_focus_t *p_syf = &THIS_STG.sf;
	char				*p = arr_p_vram[p_syf->f_row];
	int					max_byte = strlen(p) - 1;
	
	if(p_syf->f_row == 2) 
	{
		//整体
		p_syf->start_byte = 0;
		p_syf->num_byte = strlen(p); 
		if( (pos == CST_POS_ADD) || (pos == CST_POS_ADD))
		{
			
			return -1;
		}
		return 0;
	}
	
	
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
			p_syf->start_byte = max_byte; 
			break;
		case CST_POS_ADD:
			if(p_syf->start_byte < max_byte)
				p_syf->start_byte ++;
			else
			{
				p_syf->start_byte = 0;
				ret = -1;
			}
			if(p[p_syf->start_byte] == ':')
				p_syf->start_byte ++;
			break;
		case CST_POS_SUB:
			if(p_syf->start_byte)
				p_syf->start_byte --;
			else
			{
				p_syf->start_byte = max_byte;
				ret = -1;
			}
			if(p[p_syf->start_byte] == ':')
				p_syf->start_byte --;
			break;		
		
		
	}
	
	
	
	return ret;
	
}

static void SSM_Reset_focus(void)
{
//	strategy_focus_t *p_syf = &THIS_STG.sf;
	THIS_STG.sf.f_col = 0;
	THIS_STG.sf.f_row = 0;
	THIS_STG.sf.start_byte = 0;
	THIS_STG.sf.num_byte = 8;
	
}

static int SSM_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
	char		**pp_vram = (char **)pp_data;
	
	if(p_syf->f_row > THIS_MAX_ROW) {
		return -1;
	}
	
	if(p_in_syf)
		p_syf = p_in_syf;
	
	
	if(p_syf->f_col == 0)
	{
		
		*pp_vram = set_sample_entrys[p_syf->f_row];
	}
	else
	{
		*pp_vram = arr_p_vram[p_syf->f_row];
	}

	return strlen(*pp_vram);
	
}

static int SSM_key_up(void *arg)
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
		
		if(p_syf->f_row < 2)
			p_syf->num_byte = 8;
		else
			p_syf->num_byte = 4;
		
		return ret;
	}
	return SSM_modify(arg, OP_ADD);
}
static int SSM_key_dn(void *arg)
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
		if(p_syf->f_row < 2)
			p_syf->num_byte = 8;
		else
			p_syf->num_byte = 4;
		
		return ret;
	}
	
	return SSM_modify(arg, OP_SUB);
}

static int SSM_key_lt(void *arg)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
//	int ret = CST_RET_RECOVERY_OLD_FOCUS;
	
	if(p_syf->f_col == 0)
	{
		
		p_syf->f_col = 1;
		SSM_Col_1(CST_POS_LAST);
	}
	else
	{
		if(SSM_Col_1(CST_POS_SUB) < 0)
		{
			
			
			p_syf->f_col = 0;
			p_syf->start_byte = 0;
			if(p_syf->f_row < 2)
				p_syf->num_byte = 8;
			else
				p_syf->num_byte = 4;
			
		}
		
		
	}
	
	return CST_RET_RECOVERY_OLD_FOCUS;
}

static int SSM_key_rt(void *arg)
{
	strategy_focus_t *p_syf = &THIS_STG.sf;
//	int ret = RET_OK;
	
	if(p_syf->f_col == 0)
	{
		
		p_syf->f_col = 1;
		SSM_Col_1(CST_POS_FIRST);
		
	}
	else
	{
		if(SSM_Col_1(CST_POS_ADD) < 0)
		{
			
			
			p_syf->f_col = 0;
			p_syf->start_byte = 0;
			if(p_syf->f_row < 2)
				p_syf->num_byte = 8;
			else
				p_syf->num_byte = 4;
			
			
			
		}
		
			
			
		
		
	}
	
	return CST_RET_RECOVERY_OLD_FOCUS;
}
static int SSM_key_er(void *arg)
{
	
	return SSM_commit(NULL);
	
}




