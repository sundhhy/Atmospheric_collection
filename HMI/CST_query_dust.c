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

static int QDS_entry(int row, int col, void *pp_text);
static int QDS_init(void *arg);
static int QDS_key_up(void *arg);
static int QDS_key_dn(void *arg);
static int QDS_key_lt(void *arg);
static int QDS_key_rt(void *arg);
static int QDS_key_er(void *arg);
static void QDS_Reset_focus(void);
static int QDS_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf);
static int QDS_commit(void *arg);
static void QDS_Exit(void);

strategy_t	cst_qry_dust = {
	QDS_entry,
	QDS_init,
	STG_Duild_button,
	QDS_key_up,
	QDS_key_dn,
	QDS_key_lt,
	QDS_key_rt,
	QDS_key_er,
	QDS_Reset_focus,
	QDS_get_focusdata,	
	QDS_commit,
	QDS_Exit,
};


//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//
/*
大气查询的纵狗有13行，前面9行时3个项目显示一页
最后4行要做到2行显示一页，因为是相关的，分开显示就不好了
*/
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define		THIS_STG	cst_qry_dust


#define		THIS_NUM_ROWS		12
#define		THIS_MAX_ROW		11
#define		THIS_MAX_COL		1

#define		NEED_NUM_RAM			13
#define 	CACHE_BUF_NUM			12


static char *const qds_entry_code[THIS_NUM_ROWS] = { \
	"开始:", \
	"采样次数:", \
	"设置次数:",\
	"湿度:", \
	"压力:", \
	"大气压:", \
		"工况", \
		"标况", \
		"设置", \
	"工况体积:",	\
	"标况体积:",		\
	"采样时间:"
};
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------
typedef struct {
	uint8_t		sig_type;		//0 : A 1:B
	uint8_t		none[3];
}qry_atm_t;
//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
	
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int QDS_entry(int row, int col, void *pp_text)
{
	char **pp = (char **)pp_text;
	qry_atm_t	*p_cache;
	
	uint32_t			sec_u32 = 100;
	mdl_data_t		mdd = {1000, 2};
	struct tm		t;
						
	if(row > THIS_MAX_ROW)
		return 0;
	if(col > THIS_MAX_COL)
		return 0;
	
	THIS_STG.row_in_page = 0xff;
	
		
	if(col == 0)
	{
		*pp = qds_entry_code[row];
		
	}
	else
	{
		p_cache = (qry_atm_t *)arr_p_vram[CACHE_BUF_NUM];
		
		switch(row)
		{
			
			case 0:
				MDL_Get_value(p_cache->sig_type, em_start_time_sec, &sec_u32);
				Sec_2_tm(sec_u32, &t);
				sprintf(arr_p_vram[row], "%02d-%02d %02d:%02d", t.tm_mday, t.tm_mon, t.tm_mday, t.tm_min);
				break;
			case 1:
				MDL_Get_value(p_cache->sig_type, em_sample_count, &sec_u32);
				sprintf(arr_p_vram[row], "%d", sec_u32);
				break;
			case 2:
				MDL_Get_value(p_cache->sig_type, em_set_up_count, &sec_u32);
				sprintf(arr_p_vram[row], "%d", sec_u32);
				break;
			case 3:
				MDL_Get_value(p_cache->sig_type, em_humidity, &mdd);
				Print_float(mdd.mdl_val, 5, mdd.num_point, arr_p_vram[row]);
				break;
			case 4:
				MDL_Get_value(p_cache->sig_type, em_pressure, &mdd);
				Print_float(mdd.mdl_val, 5, mdd.num_point, arr_p_vram[row]);
				strcat(arr_p_vram[row], "KPa");
				break;
			case 5:
				MDL_Get_value(p_cache->sig_type, em_atm_pressure, &mdd);
				Print_float(mdd.mdl_val, 5, mdd.num_point, arr_p_vram[row]);
				strcat(arr_p_vram[row], "KPa");
				break;
			case 6:
				MDL_Get_value(p_cache->sig_type, em_condition, &mdd);
				Print_float(mdd.mdl_val, 5, mdd.num_point, arr_p_vram[row]);
				strcat(arr_p_vram[row], "L/min");
				break;
			case 7:
				MDL_Get_value(p_cache->sig_type, em_standard_conditions, &mdd);
				Print_float(mdd.mdl_val, 5, mdd.num_point, arr_p_vram[row]);
				strcat(arr_p_vram[row], "L/min");
				break;
			case 8:
				MDL_Get_value(p_cache->sig_type, em_set_up_condition, &mdd);
				Print_float(mdd.mdl_val, 5, mdd.num_point, arr_p_vram[row]);
				strcat(arr_p_vram[row], "L/min");
				break;
			case 9:
				
				MDL_Get_value(p_cache->sig_type, em_condition_volume, &mdd);
				Print_float(mdd.mdl_val, 5, mdd.num_point, arr_p_vram[row]);
				strcat(arr_p_vram[row], "L");
				break;
			case 10:
				MDL_Get_value(p_cache->sig_type, em_standard_condition_volume, &mdd);
				Print_float(mdd.mdl_val, 5, mdd.num_point, arr_p_vram[row]);
				strcat(arr_p_vram[row], "L");
				break;
			case 11:
				
				MDL_Get_value(p_cache->sig_type, em_sample_time_sec, &sec_u32);
				sprintf(arr_p_vram[row], "%02d:%02d", sec_u32 / 60, sec_u32 % 60);
				break;
			default:
				return 0;
		}
		
		*pp = arr_p_vram[row];
	}
	

	return strlen(*pp);

}

static int QDS_init(void *arg)
{
	int i = 0;
	qry_atm_t	*p_cache;
	uint8_t		*p_u8 = (uint8_t *)arg;
	
	
	memset(&THIS_STG.sf, 0, sizeof(THIS_STG.sf));
	

	THIS_STG.p_stg_title = "粉尘";
	
	THIS_STG.row_in_page = 0xff;
	
	THIS_STG.stg_num_rows = THIS_NUM_ROWS;
	HMI_Ram_init();
	for(i = 0; i < NEED_NUM_RAM; i++) {
		
		arr_p_vram[i] = HMI_Ram_alloc(48);
		memset(arr_p_vram[i], 0, 48);
	}
	p_cache = (qry_atm_t *)arr_p_vram[CACHE_BUF_NUM];
	
	if(*p_u8 == e_qry_atmosphere_A)
		p_cache->sig_type = em_atmosphere_A;
	else
		p_cache->sig_type = em_atmosphere_B;
	
	
	aci_sys.key_weight = 1;
	
	return RET_OK;
}
static void QDS_Exit(void)
{
	
}
static int QDS_commit(void *arg)
{

	
	return RET_OK;

}






static void QDS_Reset_focus(void)
{
	
	
	

	
}

static int QDS_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf)
{


	return 0;
	
}

static int QDS_key_up(void *arg)
{

	return -1;
}
static int QDS_key_dn(void *arg)
{

	
	return -1;
}

static int QDS_key_lt(void *arg)
{

	
	
	return -1;
}

static int QDS_key_rt(void *arg)
{

	return -1;
}
static int QDS_key_er(void *arg)
{
	
	return -1;
	
}




