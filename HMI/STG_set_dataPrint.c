#include <stdint.h>
#include "HMI_striped_background.h"

//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------

static int Data_print_Strategy_entry(int row, int col, void *pp_text);
static int DPT_key_up(void *arg);
static int DPT_key_dn(void *arg);
static int DPT_key_lt(void *arg);
static int DPT_key_rt(void *arg);
static int DPT_key_er(void *arg);
static int DPT_init(void *arg);
static int DPT_get_focusdata(void *pp_data,  strategy_focus_t *p_in_syf);
static int DPT_commit(void *arg);
static void DBT_Exit(void);
strategy_t	g_dataPrint_strategy = {
	Data_print_Strategy_entry,
	DPT_init,
	STY_Duild_button,
	DPT_key_up,
	DPT_key_dn,
	DPT_key_lt,
	DPT_key_rt,
	DPT_key_er,
	DPT_get_focusdata,
	DPT_commit,
	DBT_Exit,
};
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
 static char *const arr_p_dataPrint_entry[6] = {"ͨ���ţ�", "��ӡ���ͣ�", "��ӡ�����", "��ʼʱ�䣺", "��ֹʱ�䣺",\
	  "��ӡ����"
 };
	
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

static int Data_print_Strategy_entry(int row, int col, void *pp_text)
{
	char **pp = (char **)pp_text;
	if(col == 0) {
		
		if(row > 5)
			return 0;
		*pp = arr_p_dataPrint_entry[row];
		return strlen(arr_p_dataPrint_entry[row]);
	} 
	
	return 0;
}

static int DPT_init(void *arg)
{
	return RET_OK;
}
static void DBT_Exit(void)
{
	
}
static int DPT_key_up(void *arg)
{
	return RET_OK;
}
static int DPT_key_dn(void *arg)
{
	return RET_OK;
}
static int DPT_key_lt(void *arg)
{
	return RET_OK;
}
static int DPT_key_rt(void *arg)
{
	return RET_OK;
}
static int DPT_key_er(void *arg)
{
	return RET_OK;
}

static int DPT_get_focusdata(void *pp_data,  strategy_focus_t *p_in_syf)
{
	return RET_OK;
}
static int DPT_commit(void *arg)
{
	return RET_OK;
}


