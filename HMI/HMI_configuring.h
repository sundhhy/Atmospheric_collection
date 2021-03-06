#ifndef _INC_HMI_configuring_H_
#define _INC_HMI_configuring_H_
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
CLASS(HMI_configuring)
{
	EXTENDS( HMI);
	
	//每一列的x起始位置 数组成员要禨ETTING_ROW_MAX
	uint8_t		col_vx0[4];
	uint8_t		entry_start_row;
	char		none[3];		
	
};
//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------
HMI_configuring *Get_HMI_configuring(void) ;
#endif
