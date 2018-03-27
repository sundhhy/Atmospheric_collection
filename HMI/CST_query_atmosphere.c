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

static int QAT_entry(int row, int col, void *pp_text);
static int QAT_init(void *arg);
static int QAT_key_up(void *arg);
static int QAT_key_dn(void *arg);
static int QAT_key_lt(void *arg);
static int QAT_key_rt(void *arg);
static int QAT_key_er(void *arg);
static void QAT_Reset_focus(void);
static int QAT_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf);
static int QAT_commit(void *arg);
static void QAT_Exit(void);

strategy_t	cst_qry_atmosphere = {
	QAT_entry,
	QAT_init,
	STG_Duild_button,
	QAT_key_up,
	QAT_key_dn,
	QAT_key_lt,
	QAT_key_rt,
	QAT_key_er,
	QAT_Reset_focus,
	QAT_get_focusdata,	
	QAT_commit,
	QAT_Exit,
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
#define		THIS_STG	cst_qry_atmosphere


#define		THIS_NUM_ROWS		13
#define		THIS_MAX_ROW		12
#define		THIS_MAX_COL		1

#define		NEED_NUM_RAM			14
#define 	CACHE_BUF_NUM			13


static char *const qat_entry_code[13] = { \
	"开始:", \
	"采样次数:", \
	"设置次数:",\
	"湿度:", \
	"压力:", \
	"大气压:", \
		"工况", \
		"标况", \
		"设置", \
	"工体:",	\
	"标体:",		\
	"累计时长:",		\
	"设置时长:"		\
	};
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------
typedef struct {
	uint8_t		sig_type;		//0 : A 1:B
	uint8_t		last_show_row;		//上一次显示的行，用于最后4行的2 * 2显示
	uint8_t		none;
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

static int QAT_entry(int row, int col, void *pp_text)
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
	if((row >= 9) && (row <= 12)) 
		THIS_STG.row_in_page = 2;
		
	if(col == 0)
	{
		*pp = qat_entry_code[row];
		
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
				strcat(arr_p_vram[row], "L/min");
				break;
			case 10:
				MDL_Get_value(p_cache->sig_type, em_standard_condition_volume, &mdd);
				Print_float(mdd.mdl_val, 5, mdd.num_point, arr_p_vram[row]);
				strcat(arr_p_vram[row], "L/min");
				break;
			case 11:
				MDL_Get_value(p_cache->sig_type, em_add_up_time_sec, &sec_u32);
				sprintf(arr_p_vram[row], "%02d:%02d", sec_u32 / 60, sec_u32 % 60);
				break;
			case 12:
				MDL_Get_value(p_cache->sig_type, em_set_up_time_sec, &sec_u32);
				sprintf(arr_p_vram[row], "%02d:%02d", sec_u32 / 60, sec_u32 % 60);
				break;
			default:
				return 0;
		}
		
		*pp = arr_p_vram[row];
	}
	

	return strlen(*pp);

}

static int QAT_init(void *arg)
{
	int i = 0;
	qry_atm_t	*p_cache;
	uint8_t		*p_u8 = (uint8_t *)arg;
	
	
	memset(&THIS_STG.sf, 0, sizeof(THIS_STG.sf));
	
	if(*p_u8 == e_qry_atmosphere_A)
		THIS_STG.p_stg_title = "大气A";
	else
		THIS_STG.p_stg_title = "大气B";
	
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
	p_cache->last_show_row = 0;
	
	
	aci_sys.key_weight = 1;
	
	return RET_OK;
}
static void QAT_Exit(void)
{
	
}
static int QAT_commit(void *arg)
{

	
	return RET_OK;

}






static void QAT_Reset_focus(void)
{
	
	
	

	
}

static int QAT_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf)
{


	return 0;
	
}

static int QAT_key_up(void *arg)
{

	return -1;
}
static int QAT_key_dn(void *arg)
{

	
	return -1;
}

static int QAT_key_lt(void *arg)
{

	
	
	return -1;
}

static int QAT_key_rt(void *arg)
{

	return -1;
}
static int QAT_key_er(void *arg)
{
	
	return -1;
	
}




