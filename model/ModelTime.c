#include "ModelTime.h"
#include <string.h>
#include "mem/CiiMem.h"
#include "sdhDef.h"
#include "system.h"
//#include "utils/rtc.h"
//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------

const	char	g_moth_day[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define MTM_SIMU_RTC			1		//ģ��RTC��ʱ���߶�
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static char s_timer[16];

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
/* Cycle/Sync Callback functions */
static char* MdlTime_to_string( Model *self, IN int aux, void *arg);
static int MdlTime_set_by_string( Model *self, IN int aux, void *arg);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
int MdlTime_init( Model *self, IN void *arg)
{

	struct  tm	*tm = &aci_sys.sys_time;

#if MTM_SIMU_RTC == 1	
	uint8_t	h,m,s;
	tm->tm_year = GetCompileYear();
	tm->tm_mon =  GetCompileMoth();
	tm->tm_mday = GetCompileDay();
	GetCompileTime(&h, &m, &s);
	tm->tm_hour = h;
	tm->tm_min = m;
	tm->tm_sec = h;
#else	
	System_time(tm);
	
#endif
	
	self->coreData = tm;
	self->crDt_len = sizeof( *tm);
	
	return RET_OK;
}

void MdlTime_run(Model *self)
{
	struct  tm	*tm = ( struct  tm	*) self->coreData;

#if MTM_SIMU_RTC == 1
	if(tm->tm_sec < 59)
		tm->tm_sec ++;
	else
	{
		
		tm->tm_sec = 0;
		tm->tm_min ++;
	}
	
	if(tm->tm_min >= 60)
	{
		tm->tm_min = 0;
		tm->tm_hour ++;
		
	}
	
	if(tm->tm_hour >= 24)
	{
		tm->tm_hour = 0;
		tm->tm_mday ++;
		
	}
#else
	System_time(tm);
#endif
	
	//180121 ʱ�Ӹ��µĻ� ����ʧ��
	aci_sys.lcd_sem_wait_ms = 200;
	self->notify( self);
	aci_sys.lcd_sem_wait_ms = 0xffffffff;;
	
}

int MdlTime_getData(  Model *self, IN int aux, void *arg) 
{
	struct  tm	*tm = ( struct  tm	*) self->coreData;
	struct  tm	*tm2 = NULL;
	uint32_t	*p_u32;
	
	if(arg == NULL)
		return ERR_PARAM_BAD;
	
//	System_time(tm);
	
	if(aux == TIME_TM)
	{
		
		tm2 = ( struct  tm	*) arg;
		tm2->tm_year = tm->tm_year;
		tm2->tm_mon = tm->tm_mon;
		tm2->tm_mday = tm->tm_mday;
		tm2->tm_hour = tm->tm_hour;
		tm2->tm_min = tm->tm_min;
		tm2->tm_sec = tm->tm_sec;
		
	}
	else if(aux == TIME_U32)
	{
		p_u32 = (uint32_t *)arg;
		*p_u32 = Time_2_u32(tm);
	}
	
	return RET_OK;
}

CTOR( ModelTime)
SUPER_CTOR( Model);
FUNCTION_SETTING( Model.init, MdlTime_init);
FUNCTION_SETTING( Model.run, MdlTime_run);

FUNCTION_SETTING( Model.getMdlData, MdlTime_getData);
FUNCTION_SETTING( Model.set_by_string, MdlTime_set_by_string);

FUNCTION_SETTING( Model.to_string, MdlTime_to_string);

END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{




static char* MdlTime_to_string( Model *self, IN int aux, void *arg)
{
	struct  tm	*p_tm= (struct  tm	*) self->coreData;
	switch(aux) {
		case 0:
			snprintf(s_timer, 16, "%02d-%02d %02d:%02d:%02d", p_tm->tm_mon, p_tm->tm_mday, p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
			return s_timer;
		case 1:
			if(arg == NULL)
				break;
			sprintf(arg, "%02d/%02d/%02d %02d:%02d:%02d", p_tm->tm_year, p_tm->tm_mon, p_tm->tm_mday, \
				p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
			return arg;
		default:break;
				

	}
	return s_timer;
	
}


static int MdlTime_set_by_string( Model *self, IN int aux, void *arg)
{
	struct  tm	t = {0};
	struct  tm	*p_tm= (struct  tm	*) self->coreData;
	char		*p = (char *)arg;
	short		i;
//	short		val;
	uint8_t		err;
//	char	s_data[4];
	switch(aux) {
		case 0:
			return RET_OK;
		case 1:
			//"%02d:%02d:%02d %02d:%02d:%02d", p_tm->tm_year, p_tm->tm_mon, p_tm->tm_mday, \
				p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec
			if(arg == NULL)
				break;
			
			t.tm_year = Get_str_data(p, "/", 0, &err);
			if(err)
				return ERR_PARAM_BAD;
			t.tm_mon = Get_str_data(p, "/", 1, &err);
			if(err)
				return ERR_PARAM_BAD;
			t.tm_mday = Get_str_data(p, "/", 2, &err);
			if(err)
				return ERR_PARAM_BAD;
			if(t.tm_mday > g_moth_day[t.tm_mon])
				return ERR_PARAM_BAD;
		
			
			i = strcspn(p, " ");
			p += i;
			
			t.tm_hour = Get_str_data(p, ":", 0, &err);
			if(err)
				return ERR_PARAM_BAD;
			t.tm_min = Get_str_data(p, ":", 1, &err);
			if(err)
				return ERR_PARAM_BAD;
			t.tm_sec = Get_str_data(p, ":", 2, &err);
			if(err)
				return ERR_PARAM_BAD;
			
			p_tm->tm_year = t.tm_year;
			p_tm->tm_mon = t.tm_mon;
			p_tm->tm_mday = t.tm_mday;
			p_tm->tm_hour = t.tm_hour;
			p_tm->tm_min = t.tm_min;
			p_tm->tm_sec = t.tm_sec;
			return System_set_time(p_tm);
		default:break;
				

	}
	return -1;
	
}
