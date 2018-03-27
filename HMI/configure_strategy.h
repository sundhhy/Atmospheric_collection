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
#define CST_RET_RECOVERY_OLD_FOCUS			0		//ֻ���ƶ������ʱ��ʹ��
#define CST_RET_CLEAN_OLD_FOCUS				1		//�����е����ݱ��޸�ʱʹ��
#define CST_RET_FOCUS_HOLD					2		//����û���ƶ�
// pos : 0 ����  1 ��һ��λ�� 2  ����λ�� 3 ��1 4 ��1
#define CST_POS_HOLD		0
#define CST_POS_FIRST		1
#define CST_POS_LAST		2
#define CST_POS_ADD			3
#define CST_POS_SUB			4

/*

��������������������������
	col 	col
	clo		col
	...


*/
#define STG_RAM_NUM(row, col)  (row * 2 + col)		//ͨ�����������㻺����±�

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
