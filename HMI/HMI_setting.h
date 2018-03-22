#ifndef _INC_HMI_setting_H_
#define _INC_HMI_setting_H_
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

 //------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------
CLASS(HMI_setting)
{
	EXTENDS( HMI);
	
	//每一列的x起始位置 数组成员要禨ETTING_ROW_MAX
	uint8_t		col_vx0[4];
	uint8_t		entry_start_row;
	char			page_buf[7];		//用于显示页数，如[1/4]
	
};
//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------
HMI_setting *Get_HMI_setting(void) ;
void HMI_HST_Setting_strategy(void *p_stg);
#endif
