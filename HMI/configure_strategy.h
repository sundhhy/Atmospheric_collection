#ifndef _INC_configure_strategy_H_
#define _INC_configure_strategy_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <stdint.h>
#include "HMI.h"

//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------


/*

下面这个宏适用下面这种情况下
	col 	col
	clo		col
	...


*/
#define STG_RAM_NUM(row, col)  (row * 2 + col)		//通过行列来计算缓存的下标

typedef enum
{
	e_stg_set_time,
	
	
}strategy_type;
 //------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// global variable declarations


//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------
strategy_t	*Get_strategy(uint8_t t);
#endif
