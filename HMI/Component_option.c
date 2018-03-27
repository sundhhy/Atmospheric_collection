#include "Component_option.h"
#include "HMIFactory.h"
#include "sdhDef.h"
#include "ExpFactory.h"
#include "format.h"
#include "chnInfoPic.h"
#include "focus.h"

#include "configure_strategy.h"


//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
/*

全部的设置选项界面上的选项

*/
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


/*

e_I_instrument_setup

仪器设置
	时间
	显示
	密码修改
*/
static ro_char his_code_title[] = { "<text vx0=0 vy0=0>仪器设置</>" };

static ro_char his_code_choice_1_1[] = { "<text vx0=16 vy0=16>时间</>" };
static ro_char his_code_choice_1_2[] = { "<text vx0=16 vy0=32>显示</>" };
static ro_char his_code_choice_1_3[] = { "<text vx0=16 vy0=48>密码修改</>" };


/*

e_I_acquisition_setup
采样设置
	压力				计时方式
	恒温器			其他设置
*/
static ro_char has_code_title[] = { "<text vx0=0 vy0=0>采样设置</>" };

static ro_char has_code_choice_1_1[] = { "<text vx0=0 vy0=16 bkc=white>压力</>" };
static ro_char has_code_choice_1_2[] = { "<text vx0=56 vy0=16 bkc=white>计时方式</>" };
static ro_char has_code_choice_2_1[] = { "<text vx0=0 vy0=32 bkc=white>恒温器</>" };
static ro_char has_code_choice_2_2[] = { "<text vx0=56 vy0=32 bkc=white>其他设置</>" };

/*

e_II_pressure_setup
压力设置
	大气压
	限压保护


*/
static ro_char hps_code_title[] = { "<text vx0=0 vy0=0>压力设置</>" };

static ro_char hps_code_choice_1_1[] = { "<text vx0=16 vy0=16 bkc=white>大气压</>" };
static ro_char hps_code_choice_1_2[] = { "<text vx0=16 vy0=32 bkc=white>限压保护</>" };

/*

e_II_timing_setup
压力设置
	大气压
	限压保护


*/
static ro_char hts_code_title[] = { "<text vx0=0 vy0=0>时间控制方式</>" };

static ro_char hts_code_choice_1_1[] = { "<text vx0=16 vy0=16 bkc=white>开机延时方式</>" };
static ro_char hts_code_choice_1_2[] = { "<text vx0=16 vy0=32 bkc=white>停电计时方式</>" };


/*

e_II_other_setup
其他设置
	标况
	水汽压
	粉尘流量

*/
static ro_char oth_code_title[] = { "<text vx0=0 vy0=0>其他设置</>" };

static ro_char oth_code_choice_1_1[] = { "<text vx0=16 vy0=16>标况温度</>" };
static ro_char oth_code_choice_1_2[] = { "<text vx0=16 vy0=32 bkc=white>水汽压</>" };
static ro_char oth_code_choice_1_3[] = { "<text vx0=16 vy0=48 bkc=white>粉尘流量</>" };
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------
typedef void (*option_init)(cmp_options_t *p_opt);
typedef int (*opt_select)(uint8_t );		//根据选中的位置来决定确定下一幅画面
typedef struct {
	
	option_init		opt_init;	
	opt_select		func_select;	
}opt_op_t;
//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------



static cmp_options_t			arr_options[e_num_options];
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static void INS_Init(cmp_options_t *p_opt);
static int INS_Select(uint8_t s_id);

static void ACQ_Init(cmp_options_t *p_opt);
static int ACQ_Select(uint8_t s_id);

static void PRS_Init(cmp_options_t *p_opt);
static int PRS_Select(uint8_t s_id);

static void TIM_Init(cmp_options_t *p_opt);
static int TIM_Select(uint8_t s_id);

static void OTH_Init(cmp_options_t *p_opt);
static int OTH_Select(uint8_t s_id);

static const opt_op_t 					arr_op[e_num_options] = \
	{ \
		{\
			INS_Init,\
			INS_Select, \
		},
		{\
			ACQ_Init,\
			ACQ_Select, \
		},
		{\
			PRS_Init,\
			PRS_Select, \
		},
		{\
			TIM_Init,\
			TIM_Select, \
		},
		{\
			OTH_Init,\
			OTH_Select, \
		},
	};
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
cmp_options_t *CMP_Get_option(uint8_t		ot)
{
	if(ot >= e_num_options)
		return NULL;
	arr_options[ot].opt_type = ot;
	return arr_options + ot;
	
}
void CMP_OPT_Init(cmp_options_t *p_opt)
{
	if(p_opt == NULL)
		return;

	
	arr_op[p_opt->opt_type].opt_init(p_opt);
	
}


int CMP_OPT_Select(cmp_options_t *p_opt, uint8_t		sht_id)
{
	
	if(p_opt == NULL)
		return -1;
	
	return arr_op[p_opt->opt_type].func_select(sht_id);
	
}


//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//


/***********************仪器配置********************************************/
static void INS_Init(cmp_options_t *p_opt)
{
	
	Expr 		*p_exp ;
	p_exp = ExpCreate( "text");
	
	p_exp->inptSht(p_exp, (void *)his_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)his_code_choice_1_1, arr_p_sht_choices[0]);
	p_exp->inptSht(p_exp, (void *)his_code_choice_1_2, arr_p_sht_choices[1]);
	p_exp->inptSht(p_exp, (void *)his_code_choice_1_3, arr_p_sht_choices[2]);
	

	
	p_opt->p_titl = arr_p_pool_shts[0];
	p_opt->p_optionals[0][0] = arr_p_sht_choices[0];
	p_opt->p_optionals[1][0] = arr_p_sht_choices[1];
	p_opt->p_optionals[2][0] = arr_p_sht_choices[2];
	
	
	p_opt->num_col = 1;
	p_opt->num_row = 3;
}

static int INS_Select(uint8_t s_id)
{
	uint8_t		s_row, s_col;
	
	s_row = OPTION_GET_ROW(s_id);
	s_col = OPTION_GET_COL(s_id);
	
	if((s_row == 0) && (s_col == 0))
	{
		return OPTION_SELECT_CONFIG(e_stg_set_time);
		
	}
	if((s_row == 1) && (s_col == 0))
	{
		return OPTION_SELECT_CONFIG(e_stg_set_display);
		
	}
	if((s_row == 2) && (s_col == 0))
	{
		return OPTION_SELECT_CONFIG(e_stg_set_password);
		
	}
		
	
	return -1;
}

/**********************采样设置********************************************/
static void ACQ_Init(cmp_options_t *p_opt)
{
	Expr 		*p_exp ;
	
	p_exp = ExpCreate( "text");
	p_exp->inptSht(p_exp, (void *)has_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)has_code_choice_1_1, arr_p_sht_choices[0]);
	p_exp->inptSht(p_exp, (void *)has_code_choice_1_2, arr_p_sht_choices[1]);
	p_exp->inptSht(p_exp, (void *)has_code_choice_2_1, arr_p_sht_choices[2]);
	p_exp->inptSht(p_exp, (void *)has_code_choice_2_2, arr_p_sht_choices[3]);
	
	

	
	p_opt->p_titl = arr_p_pool_shts[0];
	p_opt->p_optionals[0][0] = arr_p_sht_choices[0];
	p_opt->p_optionals[0][1] = arr_p_sht_choices[1];
	p_opt->p_optionals[1][0] = arr_p_sht_choices[2];
	p_opt->p_optionals[1][1] = arr_p_sht_choices[3];
	
	p_opt->num_col = 2;
	p_opt->num_row = 2;
	
}

static int ACQ_Select(uint8_t s_id)
{
	
	uint8_t		s_row, s_col;
	
	s_row = OPTION_GET_ROW(s_id);
	s_col = OPTION_GET_COL(s_id);
	
	if(s_row == 0 && s_col == 0)
	{
		return OPTION_SELECT_SETUP(e_II_pressure_setup);
		
	}
	
	if(s_row == 0 && s_col == 1)
	{
		return OPTION_SELECT_SETUP(e_II_timing_setup);
		
	}
	if(s_row == 1 && s_col == 0)
	{
		return OPTION_SELECT_CONFIG(e_stg_set_heater);
		
	}
	if(s_row == 1 && s_col == 1)
	{
		return OPTION_SELECT_SETUP(e_II_other_setup);
		
	}
	
	return -1;
}

/**********************压力设置********************************************/

static void PRS_Init(cmp_options_t *p_opt)
{
	Expr 		*p_exp ;
	
	p_exp = ExpCreate("text");
	p_exp->inptSht(p_exp, (void *)hps_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)hps_code_choice_1_1, arr_p_sht_choices[0]);
	p_exp->inptSht(p_exp, (void *)hps_code_choice_1_2, arr_p_sht_choices[1]);

	
	

	
	p_opt->p_titl = arr_p_pool_shts[0];
	p_opt->p_optionals[0][0] = arr_p_sht_choices[0];
	p_opt->p_optionals[1][0] = arr_p_sht_choices[1];

	
	
	p_opt->num_row = 2;
	p_opt->num_col = 1;
	
}

static int PRS_Select(uint8_t s_id)
{
	uint8_t		 s_row;
	
	s_row = OPTION_GET_ROW(s_id);
	
	if(s_row == 0)
	{
		return OPTION_SELECT_CONFIG(e_stg_set_pressuure);
		
	}
	if(s_row == 1)
	{
		return OPTION_SELECT_CONFIG(e_stg_set_prs_protect);
		
	}
	
	return -1;
}

/**********************时间控制方式********************************************/

static void TIM_Init(cmp_options_t *p_opt)
{
	Expr 		*p_exp ;
	
	p_exp = ExpCreate("text");
	p_exp->inptSht(p_exp, (void *)hts_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)hts_code_choice_1_1, arr_p_sht_choices[0]);
	p_exp->inptSht(p_exp, (void *)hts_code_choice_1_2, arr_p_sht_choices[1]);

	
	

	
	p_opt->p_titl = arr_p_pool_shts[0];
	p_opt->p_optionals[0][0] = arr_p_sht_choices[0];
	p_opt->p_optionals[1][0] = arr_p_sht_choices[1];

	
	
	p_opt->num_row = 2;
	p_opt->num_col = 1;
	
}

static int TIM_Select(uint8_t s_id)
{
	
	return -1;
}


/**********************其他设置********************************************/

static void OTH_Init(cmp_options_t *p_opt)
{
	Expr 		*p_exp ;
	
	p_exp = ExpCreate("text");
	p_exp->inptSht(p_exp, (void *)oth_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)oth_code_choice_1_1, arr_p_sht_choices[0]);
	p_exp->inptSht(p_exp, (void *)oth_code_choice_1_2, arr_p_sht_choices[1]);
	p_exp->inptSht(p_exp, (void *)oth_code_choice_1_3, arr_p_sht_choices[2]);
	
	

	
	p_opt->p_titl = arr_p_pool_shts[0];
	p_opt->p_optionals[0][0] = arr_p_sht_choices[0];
	p_opt->p_optionals[1][0] = arr_p_sht_choices[1];
	p_opt->p_optionals[2][0] = arr_p_sht_choices[2];

	
	
	p_opt->num_row = 3;
	p_opt->num_col = 1;
	
}

static int OTH_Select(uint8_t s_id)
{
	
	uint8_t		 s_row;
	
	s_row = OPTION_GET_ROW(s_id);
	
	if(s_row == 0)
	{
		return OPTION_SELECT_CONFIG(e_stg_set_case_tmp);
		
	}
	if(s_row == 1)
	{
		return OPTION_SELECT_CONFIG(e_stg_set_vapor);
		
	}
	if(s_row == 2)
	{
		return OPTION_SELECT_CONFIG(e_stg_set_dust_flow);
		
	}
	
	return -1;


}


