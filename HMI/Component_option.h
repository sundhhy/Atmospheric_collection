#ifndef _INC_Component_option_H_
#define _INC_Component_option_H_
#include "HMI.h"
#include "HMI_comm.h"

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <stdint.h>
//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define 	OPTION_INSTRUMENT_SETUP				0
#define 	OPTION_ACQUISITION_SETUP			1

#define		NUM_OPTION										2

#define 	OPTION_SHEET_ID(r, c)					((r << 4) | (c))
#define 	OPTION_GET_ROW(sht_id)				(sht_id >> 4)
#define 	OPTION_GET_COL(sht_id)				(sht_id & 0x0f)
 //------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------



typedef struct {
	sheet		*p_titl;
	sheet		*p_optionals[3][2];		//最多支持3行2列选项
	
	uint8_t	num_row;
	uint8_t	num_col;
	
	uint8_t	opt_type;
	uint8_t	none;
	
	
}cmp_options_t;

//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------
cmp_options_t *CMP_Get_option(uint8_t		ot);
void CMP_OPT_Init(cmp_options_t *p_opt);
void CMP_OPT_Select(cmp_options_t *p_opt, uint8_t		sht_id);
#endif
