#include "Component_option.h"
#include "HMIFactory.h"
#include "sdhDef.h"
#include "ExpFactory.h"
#include "format.h"
#include "chnInfoPic.h"
#include "focus.h"

#include "Component_tips.h"



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

OPTION_INSTRUMENT_SETUP

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

OPTION_ACQUISITION_SETUP
��������
	ѹ��				��ʱ��ʽ
	������			��������
*/
static ro_char has_code_title[] = { "<text vx0=0 vy0=0>��������</>" };

static ro_char has_code_choice_1_1[] = { "<text vx0=0 vy0=16 bkc=white>ѹ��</>" };
static ro_char has_code_choice_1_2[] = { "<text vx0=56 vy0=16 bkc=white>��ʱ��ʽ</>" };
static ro_char has_code_choice_2_1[] = { "<text vx0=0 vy0=32 bkc=white>������</>" };
static ro_char has_code_choice_2_2[] = { "<text vx0=56 vy0=32 bkc=white>��������</>" };


//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------
typedef void (*option_init)(cmp_options_t *p_opt);
typedef void (*opt_select)(uint8_t );		//����ѡ�е�λ��������ȷ����һ������
typedef struct {
	
	option_init		opt_init;	
	opt_select		func_select;	
}opt_op_t;
//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------



static cmp_options_t			arr_options[NUM_OPTION];
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static void INS_Init(cmp_options_t *p_opt);
static void INS_Select(uint8_t s_id);

static void ACQ_Init(cmp_options_t *p_opt);
static void ACQ_Select(uint8_t s_id);


static opt_op_t 					arr_op[NUM_OPTION] = \
	{ \
		{\
			INS_Init,\
			INS_Select, \
		},
		{\
			ACQ_Init,\
			ACQ_Select, \
		},
	};
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
cmp_options_t *CMP_Get_option(uint8_t		ot)
{
	if(ot >= NUM_OPTION)
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


void CMP_OPT_Select(cmp_options_t *p_opt, uint8_t		sht_id)
{
	
	if(p_opt == NULL)
		return;
	
	arr_op[p_opt->opt_type].func_select(sht_id);
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
	
//	arr_p_sht_choices[0]->sht_id = CMM_CHOICE_ID(0);
//	arr_p_sht_choices[1]->sht_id = CMM_CHOICE_ID(1);
//	arr_p_sht_choices[2]->sht_id = CMM_CHOICE_ID(2);
	
	p_opt->num_col = 1;
	p_opt->num_row = 3;
}

static void INS_Select(uint8_t s_id)
{
	uint8_t		s_row, s_col;
	
	s_row = OPTION_GET_ROW(s_id);
	s_col = OPTION_GET_COL(s_id);
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
	
	
//	arr_p_sht_choices[0]->sht_id = CMM_CHOICE_ID(0);
//	arr_p_sht_choices[1]->sht_id = CMM_CHOICE_ID(1);
//	arr_p_sht_choices[2]->sht_id = CMM_CHOICE_ID(2);
//	arr_p_sht_choices[3]->sht_id = CMM_CHOICE_ID(3);
	
	p_opt->num_col = 2;
	p_opt->num_row = 2;
	
}

static void ACQ_Select(uint8_t s_id)
{
	
	
}








