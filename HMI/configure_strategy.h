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

#define CST_RET_OUT_BOUNDARY				-1
#define CST_RET_RECOVERY_OLD_FOCUS			0		//只是移动焦点的时候使用
#define CST_RET_CLEAN_OLD_FOCUS				1		//焦点中的内容被修改时使用
#define CST_RET_FOCUS_HOLD					2		//焦点没有移动
// pos : 0 不变  1 第一个位置 2  最后的位置 3 加1 4 减1
#define CST_POS_HOLD		0
#define CST_POS_FIRST		1
#define CST_POS_LAST		2
#define CST_POS_ADD			3
#define CST_POS_SUB			4

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
	e_stg_set_display,
	e_stg_set_password,
	e_stg_set_pressuure,
	e_stg_set_prs_protect,
	e_stg_set_heater,
	e_stg_set_dust_flow,
	e_stg_set_vapor,
	e_stg_set_case_tmp,
	e_qry_atmosphere_A,
	e_qry_atmosphere_B,
	e_qry_dust,
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
