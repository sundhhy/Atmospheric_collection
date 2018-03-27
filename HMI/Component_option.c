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

ȫ��������ѡ������ϵ�ѡ��

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

��������
	ʱ��
	��ʾ
	�����޸�
*/
static ro_char his_code_title[] = { "<text vx0=0 vy0=0>��������</>" };

static ro_char his_code_choice_1_1[] = { "<text vx0=16 vy0=16>ʱ��</>" };
static ro_char his_code_choice_1_2[] = { "<text vx0=16 vy0=32>��ʾ</>" };
static ro_char his_code_choice_1_3[] = { "<text vx0=16 vy0=48>�����޸�</>" };


/*

e_I_acquisition_setup
��������
	ѹ��				��ʱ��ʽ
	������			��������
*/
static ro_char has_code_title[] = { "<text vx0=0 vy0=0>��������</>" };

static ro_char has_code_choice_1_1[] = { "<text vx0=0 vy0=16 bkc=white>ѹ��</>" };
static ro_char has_code_choice_1_2[] = { "<text vx0=56 vy0=16 bkc=white>��ʱ��ʽ</>" };
static ro_char has_code_choice_2_1[] = { "<text vx0=0 vy0=32 bkc=white>������</>" };
static ro_char has_code_choice_2_2[] = { "<text vx0=56 vy0=32 bkc=white>��������</>" };

/*

e_II_pressure_setup
ѹ������
	����ѹ
	��ѹ����


*/
static ro_char hps_code_title[] = { "<text vx0=0 vy0=0>ѹ������</>" };

static ro_char hps_code_choice_1_1[] = { "<text vx0=16 vy0=16 bkc=white>����ѹ</>" };
static ro_char hps_code_choice_1_2[] = { "<text vx0=16 vy0=32 bkc=white>��ѹ����</>" };

/*

e_II_timing_setup
ѹ������
	����ѹ
	��ѹ����


*/
static ro_char hts_code_title[] = { "<text vx0=0 vy0=0>ʱ����Ʒ�ʽ</>" };

static ro_char hts_code_choice_1_1[] = { "<text vx0=16 vy0=16 bkc=white>������ʱ��ʽ</>" };
static ro_char hts_code_choice_1_2[] = { "<text vx0=16 vy0=32 bkc=white>ͣ���ʱ��ʽ</>" };


/*

e_II_other_setup
��������
	���
	ˮ��ѹ
	�۳�����

*/
static ro_char oth_code_title[] = { "<text vx0=0 vy0=0>��������</>" };

static ro_char oth_code_choice_1_1[] = { "<text vx0=16 vy0=16>����¶�</>" };
static ro_char oth_code_choice_1_2[] = { "<text vx0=16 vy0=32 bkc=white>ˮ��ѹ</>" };
static ro_char oth_code_choice_1_3[] = { "<text vx0=16 vy0=48 bkc=white>�۳�����</>" };
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------
typedef void (*option_init)(cmp_options_t *p_opt);
typedef int (*opt_select)(uint8_t );		//����ѡ�е�λ��������ȷ����һ������
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


/***********************��������********************************************/
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

/**********************��������********************************************/
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

/**********************ѹ������********************************************/

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

/**********************ʱ����Ʒ�ʽ********************************************/

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


/**********************��������********************************************/

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


