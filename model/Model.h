#ifndef __INC_Modle_H_
#define __INC_Modle_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <stdint.h>
#include "arithmetic/list.h"
#include "basis/macros.h"
#include "lw_oopc.h"

#include "sdhDef.h"
#ifdef NO_ASSERT
#include "basis/assert.h"
#else
#include "assert.h"
#endif
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
//aux : 0 ~ 0x10  ���õĹ���
//0x20 ~ 0x40 :��ͨ���õ� ������Ҫʹ��
#define AUX_DATA			0
#define AUX_UNIT			1
#define AUX_ALARM			2
#define AUX_PERCENTAGE		3	
#define AUX_SIGNALTYPE		4	


#define MDLID_TEST			0
#define MDLID_CHN(n)				(8 + n)			// 8 ~ 13
//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------

//��Ϊ�����ڶ���ʱʹ��/,����_����/
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
INTERFACE( Observer)
{
	int ( *update)( Observer *self, void *p_srcMdl);
	
	
};

ABS_CLASS( Model)
{
	List_T tObs;
	void	*coreData;
	void	*dataSource;
	Model	*teamMdl;
	short	crDt_len;
	char	uint;
	uint8_t	mdl_id;
	
	abstract int (*init)( Model *self, IN void *arg);
	void	(*run)(Model *self);
	int (*self_check)( Model *self);
	void (*attach)( Model *self, IN Observer *s);
	void (*detach)( Model *self, IN Observer *s);
	int	(*getMdlData)( Model *self, IN int aux, OUT void *arg);
	int	(*setMdlData)( Model *self, IN int aux, IN void *arg);
	int	(*addTmMdl)( Model *self, Model *m);
	int (*delTmMdl)( Model *self, Model *m);
	int (*modify_str_conf)( Model *self, IN int aux, char *s, int op, int val);		//������ʱ�޸�����ʹ��
	int (*set_by_string)( Model *self,IN int aux, void *arg);
//	int (*installDataSource)( Model *self, void *dsr);
	
	void (*notify)( Model *self);
	
	char *(*to_string)( Model *self,IN int aux, void *arg);		//��ֵת�����ַ���
	int  (*to_percentage)( Model *self, void *arg);		//��ֵת���ɰٷֱ�
};

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
//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------





#endif
