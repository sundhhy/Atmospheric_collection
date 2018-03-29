//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
#include "configure_strategy.h"
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------
extern strategy_t	cst_set_time;
extern strategy_t	cst_set_display;
extern strategy_t	cst_set_password;
extern strategy_t	cst_set_pressure;
extern strategy_t	cst_set_prs_protect;
extern strategy_t	cst_set_heater;
extern strategy_t	cst_set_dust_flow;
extern strategy_t	cst_set_vapor;
extern strategy_t cst_set_case_temperature;
extern strategy_t	cst_set_sample;
extern strategy_t cst_qry_atmosphere;
extern strategy_t	cst_qry_dust;
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


//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------



//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
strategy_t	*Get_strategy(uint8_t t)
{
	switch(t)
	{
		case e_stg_set_time:
			return &cst_set_time;
		case e_stg_set_display:
			return &cst_set_display;
		case e_stg_set_password:
			return &cst_set_password;
		case e_stg_set_pressuure:
			return &cst_set_pressure;
		case e_stg_set_prs_protect:
			return &cst_set_prs_protect;
		case e_stg_set_heater:
			return &cst_set_heater;
		case e_stg_set_dust_flow:
			return &cst_set_dust_flow;
		case e_stg_set_vapor:
			return &cst_set_vapor;
		case e_stg_set_sample:
			return &cst_set_sample;
		case e_stg_set_case_tmp:
			return &cst_set_case_temperature;
		case e_qry_atmosphere_A:
		case e_qry_atmosphere_B:
			return &cst_qry_atmosphere;
		case e_qry_dust:
			return &cst_qry_dust;
		default:
			return NULL;
		
		
	}
	
	
}
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{
