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

//设置选项指用来指引设置条目的界面
typedef enum {
	e_I_instrument_setup,
	e_I_acquisition_setup,
	e_II_pressure_setup,
	e_II_timing_setup,		//计时方式
	e_II_other_setup,
	e_num_options,
	
	
	
	
}setup_tree;

//配置界面是具体的对配置项进行编辑的界面
typedef enum {

	e_num_config,
	
	
	
	
}config_tree;
									

#define 	OPTION_SHEET_ID(r, c)				((r << 4) | (c))
#define 	OPTION_GET_ROW(sht_id)				(sht_id >> 4)
#define 	OPTION_GET_COL(sht_id)				(sht_id & 0x0f)

#define 	OPTION_SELECT_SETUP(s)				(s | 0x100)
#define 	OPTION_SELECT_CONFIG(c)				(c | 0x200)
#define 	IS_SETUP(r)							((r & 0xf00) == 0x100)
#define 	IS_CONFIG(r)						((r & 0xf00) == 0x200)
#define 	GET_SELECT_DATA(r)					(r & 0xff)
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

//返回值-1，啥也不用做
//正值，表示要切换到某个设置界面或者配置界面
int CMP_OPT_Select(cmp_options_t *p_opt, uint8_t		sht_id);
#endif
