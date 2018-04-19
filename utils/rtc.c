#include "rtc.h"
#include "utils/rtc_pcf8563.h"

#include "sdhDef.h"
//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------

static util_rtc *single_rtc;
static struct  tm test_tm;
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
/* Cycle/Sync Callback functions */


//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{


util_rtc *Get_Rtc(void)
{
	if(single_rtc)
		return single_rtc;
	single_rtc = SUPER_PTR(Pcf8563_new(), util_rtc);;
	return single_rtc;
}

int UtlRtc_init( util_rtc *self, IN void *arg)
{
	struct  tm *tm = &test_tm;
	uint8_t	h,m,s;
	tm->tm_year = GetCompileYear();
	tm->tm_mon =  GetCompileMoth();
	tm->tm_mday = GetCompileDay();
	GetCompileTime(&h, &m, &s);
	tm->tm_hour = h;
	tm->tm_min = m;
	tm->tm_sec = h;
	return RET_OK;
}
int UtlRtc_get( util_rtc *self, OUT struct  tm *tm)
{
	
	
	
	
//	if(test_tm.tm_sec < 59)
//		test_tm.tm_sec ++;
//	else
//	{
//		
//		test_tm.tm_sec = 0;
//		test_tm.tm_min ++;
//	}
	
	test_tm.tm_min ++;
	if(test_tm.tm_min >= 60)
	{
		test_tm.tm_min = 0;
		test_tm.tm_hour ++;
		
	}
	
	if(test_tm.tm_hour >= 24)
	{
		test_tm.tm_hour = 0;
		test_tm.tm_mday ++;
		
	}
	
	tm->tm_year = test_tm.tm_year;
	tm->tm_mon =  test_tm.tm_mon;
	tm->tm_mday = test_tm.tm_mday;
	tm->tm_hour = test_tm.tm_hour;
	tm->tm_min = test_tm.tm_min;
	tm->tm_sec = test_tm.tm_sec;
	
	return RET_OK;
}


int	UtlRtc_set( util_rtc *self, IN struct  tm *tm)
{
	return RET_OK;
	
}


int	UtlRtc_readReg( util_rtc *self, IN uint8_t	reg, OUT uint8_t val[], uint8_t num)
{
	
	return RET_OK;
}
	

int	UtlRtc_writeReg( util_rtc *self, IN uint8_t	reg, IN uint8_t val[], uint8_t num)
{
	
	return RET_OK;
}




ABS_CTOR( util_rtc)
FUNCTION_SETTING( init, UtlRtc_init);
FUNCTION_SETTING( get, UtlRtc_get);
FUNCTION_SETTING( set, UtlRtc_set);
FUNCTION_SETTING( readReg, UtlRtc_readReg);
FUNCTION_SETTING( writeReg, UtlRtc_writeReg);

END_ABS_CTOR
