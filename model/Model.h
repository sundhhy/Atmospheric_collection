#ifndef __INC_Modle_H_
#define __INC_Modle_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <stdint.h>
#include "arithmetic/list.h"
#include "basis/macros.h"
#include "lw_oopc.h"
#include "model_conf.h"
#include "slave_data.h"
#include "sdhDef.h"
#ifdef NO_ASSERT
#include "basis/assert.h"
#else
#include "assert.h"
#endif
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
typedef enum {
	em_start_time_sec,
	em_add_up_time_sec,
	em_set_up_time_sec,
	em_sample_time_sec,
	em_sample_count,
	em_set_up_count,
	em_humidity,
	em_pressure,
	em_atm_pressure,
	em_condition,
	em_standard_conditions,
	em_set_up_condition,
	em_condition_volume,
	em_standard_condition_volume,
	em_aux_num
}mdl_aux_t;

typedef enum {
	em_atmosphere_A,
	em_atmosphere_B,
	em_dust,
	em_signal_num
}mdl_signal_t;



#define MDL_OBS_NUM			4


/************@Deprecated**********************************/
//aux : 0 ~ 0x10  公用的功能
//0x20 ~ 0x40 :给通道用的 其他不要使用
#define AUX_DATA			0
#define AUX_UNIT			1
#define AUX_ALARM			2
#define AUX_PERCENTAGE		3	
#define AUX_SIGNALTYPE		4	


#define MDLID_TEST			0
#define MDLID_CHN(n)				(8 + n)			// 8 ~ 13
/***********************************************************/
//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------

typedef struct {
	int		mdl_val;
	char	num_point;		//小数点位数
	char	none[3];
}mdl_data_t;
//因为不能在定义时使用/,故用_代替/
typedef enum {
	eu_Nm3_h = 0,
	eu_m3_h,
	eu_m3_min,
	eu_m3_s,
	eu_L_h,
	eu_L_min,
	eu_L_s,
	eu_kg_h,
	eu_kg_min,
	eu_kg_s,
	eu_oC,
	eu_Pa,
	eu_MPa,
	eu_kgf_cm2,
	eu_Bar,
	eu_mmH2O,
	eu_mmHg,
	eu_percent,			//%
	eu_ppm,
	eu_pH,
	eu_r_min,
	eu_Hz,
	eu_kHz,
	eu_mA,
	eu_A,
	eu_kA,
	eu_mV,
	eu_V,
	eu_kV,
	eu_W,
	eu_kW,
	eu_MW,
	eu_Var,
	eu_kVar,
	eu_MVar,
	eu_J,
	eu_kJ,
	eu_uS_cm,
	eu_J_kgoC,
	eu_kWh,
	eu_ug_L,
	eu_max
}e_unit_t;

//因为采样的时间信息应该是要存储的，因此就都放在一起了
typedef struct {
	uint32_t	add_up_sample_time_sec;
	uint8_t		sample_setup_hour, sample_setup_min;					//设置的采样时间
	uint8_t		sample_gap_hour, sample_gap_min;
	uint8_t		sample_delay_hour, sample_delay_min;
	
	//动态变化的时间
	uint8_t		first_sample_hour, first_sample_min;
	uint8_t		sample_hour, sample_min;
	char			is_zero;
	char			is_start;
	
	
	
	uint16_t	sample_max_count;
	uint16_t	sample_count;

	
}sample_conf_t;

typedef struct {
	
	uint16_t  zero;
	
	
}caliper_conf_t;

INTERFACE(mdl_observer)
{
	int ( *update)( mdl_observer *self, void *p_srcMdl);
	
	
};

ABS_CLASS( Model)
{


	
	void	*coreData;
	void	*dataSource;
	
	mdl_observer	*arr_obs[MDL_OBS_NUM];
	
	short	crDt_len;
	char	uint;
	uint8_t	mdl_id;
	
	abstract int (*init)( Model *self, IN void *arg);
	void	(*run)(Model *self);
	int (*self_check)( Model *self);
	int (*attach)( Model *self, IN mdl_observer *s);
	void (*detach)( Model *self, IN int fd);
	int	(*getMdlData)( Model *self, IN int aux, OUT void *arg);
	int	(*setMdlData)( Model *self, IN int aux, IN void *arg);
	int (*modify_str_conf)( Model *self, IN int aux, char *s, int op, int val);		//在设置时修改配置使用
	int (*set_by_string)( Model *self,IN int aux, void *arg);
//	int (*installDataSource)( Model *self, void *dsr);
	
	void (*notify)( Model *self);
	
	char *(*to_string)( Model *self,IN int aux, void *arg);		//数值转换成字符串
	int  (*to_percentage)( Model *self, void *arg);		//数值转换成百分比
};



typedef void (*mdl_cb)(void *arg);
//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------
extern const Except_T model_Failed;
extern const	char	g_moth_day[12];
extern const char *str_signal[em_signal_num];

extern sample_conf_t	arr_sample_conf[NUM_CHANNEL];
extern struct slave_sample_data_t	arr_AI[NUM_CHANNEL];

extern slave_sample_data_t arr_sample_val[em_signal_num];
//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------

void MDL_Get_value(uint8_t	st, uint8_t aux, void	*out_val);

void MDL_Clone_samp_conf(sample_conf_t *p_dst, sample_conf_t *p_src);

void MDL_Run(int cycle_ms);
void MDL_Init(uint8_t	st);

void MDL_Attach(uint8_t	st, void *arg, mdl_cb cb);


#endif
