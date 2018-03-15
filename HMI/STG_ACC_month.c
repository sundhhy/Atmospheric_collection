#include <stdint.h>
#include "HMI_striped_background.h"
#include "utils/Storage.h"
#include "os/os_depend.h"
#include "channel_accumulated.h"

//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define ST_SELF		g_st_acc_month

#define SAM_RAM_ACC_SDATA				0
#define SAM_RAM_ACC_SCHN				1
#define SAM_RAM_SDATE					2
#define SAM_RAM_SDAY					3
#define SAM_RAM_SCHN_NUM				4
#define SAM_RAM_UNIT						5
#define SAM_RAM_MAX							6
//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------

static int SAM_Entry(int row, int col, void *pp_text);
static int SAM_Init(void *arg);
static void SAM_Build_component(void *arg);
static int SAM_Key_UP(void *arg);
static int SAM_Key_DN(void *arg);
static int SAM_Key_LT(void *arg);
static int SAM_Key_RT(void *arg);
static int SAM_Key_ET(void *arg);
static int SAM_Get_focus_data(void *pp_data, strategy_focus_t *p_in_syf);
static int SAM_Commit(void *arg);
static void SAM_Exit(void);

strategy_t	ST_SELF = {
	SAM_Entry,
	SAM_Init,
	SAM_Build_component,
	SAM_Key_UP,
	SAM_Key_DN,
	SAM_Key_LT,
	SAM_Key_RT,
	SAM_Key_ET,
	SAM_Get_focus_data,	
	SAM_Commit,
	SAM_Exit,
};



//static int Cns_init(void *arg);
//static int Cns_get_focusdata(void *pp_data, strategy_focus_t *p_in_syf);
//strategy_t	g_chn_strategy = {
//	ChnStrategy_entry,
//	Cns_init,
//	Cns_key_up,
//	Cns_key_dn,
//	Cns_key_lt,
//	Cns_key_rt,
//	Cns_key_er,
//	Cns_get_focusdata,
//};
//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
	
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static void SAM_Print_ACC(char m, char type, char *s);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int SAM_Entry(int row, int col, void *pp_text)
{
	char 						**pp = (char **)pp_text;
	Model_chn				*p_mc = Get_Mode_chn(g_setting_chn);
	Model						*p_md = SUPER_PTR(p_mc, Model);
	short						r = 0, pic_num = 0;
	char						month = 0;
	if(col > 3)		
		return 0;
	
	r = row % STRIPE_MAX_ROWS;		//���ƽ����ϵ�������11
	pic_num = row / STRIPE_MAX_ROWS + 1;  //�ڼ�������
	//���㵱ǰ�� ���¶�Ӧ������
	if(row > 0)
		month = row - pic_num ;		
	if(month > 13)
		return 0;
	if((r > 1) && (col > 1))
	{
		//��0��1����4��
		//������ֻ��2��
		return 0;
		
	}
	
	
	
	
	
	if(r == 0)
	{
		//��ʾ�ۻ����·���ͨ����
		switch(col)
		{
			case 0:
				*pp = "���";
				break;
			case 1:
				sprintf(arr_p_vram[SAM_RAM_SDATE], "%02d", arr_chn_acc[g_setting_chn].sum_year);
				*pp = arr_p_vram[SAM_RAM_SDATE];
				break;
			case 2:
				*pp = "ͨ��";
				break;
			case 3:
				sprintf(arr_p_vram[SAM_RAM_ACC_SCHN], "%d", g_setting_chn);
				*pp = arr_p_vram[SAM_RAM_ACC_SCHN];
				break;
			
			
		}
		
		
	}
	else if(r == 1)
	{
		switch(col)
		{
			case 0:
				SAM_Print_ACC(month, SAM_RAM_SDAY, arr_p_vram[SAM_RAM_SDAY]);
				*pp = arr_p_vram[SAM_RAM_SDAY];
				break;
			case 1:
				SAM_Print_ACC(month, SAM_RAM_ACC_SDATA, arr_p_vram[SAM_RAM_ACC_SDATA]);
				*pp = arr_p_vram[SAM_RAM_ACC_SDATA];
				break;
			case 2:
				*pp = "��λ";
				break;
			case 3:
				p_md->to_string(p_md, AUX_UNIT, arr_p_vram[SAM_RAM_UNIT]);
				*pp = arr_p_vram[SAM_RAM_UNIT];
				break;
			
		}
		
	}
	else 
	{
		

		
		//��ʾ���� ���ۻ�
		if(col == 0)
		{

			SAM_Print_ACC(month, SAM_RAM_SDAY, arr_p_vram[SAM_RAM_SDAY]);
			*pp = arr_p_vram[SAM_RAM_SDAY];
			
		}
		else 
		{
			SAM_Print_ACC(month, SAM_RAM_ACC_SDATA, arr_p_vram[SAM_RAM_ACC_SDATA]);
			*pp = arr_p_vram[SAM_RAM_ACC_SDATA];
		}
	}
	

	return strlen(*pp);		
}

//�����߱�֤mС��14
static void SAM_Print_ACC(char m, char type, char *s)
{
	if(m < 12)
	{
		if(type ==  SAM_RAM_ACC_SDATA)
			CNA_Print_acc_val(arr_chn_acc[g_setting_chn].accumlated_month[m], s, 1);
		else 
			sprintf(s, "%02d", m + 1);
	}
	else if(m == 12)
	{
		
		if(type ==  SAM_RAM_ACC_SDATA)
			CNA_Print_acc_val(arr_chn_acc[g_setting_chn].accumlated_year, s, 1);
		else 
			sprintf(s, "���ۼ�");
		
	}
	else if(m == 13)
	{
		if(type ==  SAM_RAM_ACC_SDATA)
			CNA_Print_acc_val(arr_chn_acc[g_setting_chn].accumlated_total, s, 1);
		else 
			sprintf(s, "���ۼ�");
		
		
	}

	
}

static int SAM_Init(void *arg)
{
	int i = 0;
	
	
	HMI_Ram_init();
	for(i = 0; i < SAM_RAM_MAX; i++) {
		arr_p_vram[i] = HMI_Ram_alloc(48);
		memset(arr_p_vram[i], 0, 48);
	}
	
	g_setting_chn = 0;
	
	return RET_OK;
}

static void SAM_Build_component(void *arg)
{
	Button			*p_btn = BTN_Get_Sington();

	p_btn->build_each_btn(0, BTN_TYPE_MENU, Setting_btn_hdl, arg);
		
}
static void SAM_Exit(void)
{
	
}
static int SAM_Commit(void *arg)
{
	return RET_OK;
	
}

static int SAM_Get_focus_data(void *pp_data, strategy_focus_t *p_in_syf)
{

	

	return 0;
	
}

static int SAM_Key_UP(void *arg)
{
	
	g_setting_chn ++;
	if(g_setting_chn == NUM_CHANNEL)
		g_setting_chn = 0;
	
	ST_SELF.cmd_hdl(ST_SELF.p_cmd_rcv, sycmd_reflush, NULL);
	
	return -1;
}
static int SAM_Key_DN(void *arg)
{
	if(g_setting_chn)
		g_setting_chn --;
	else
		g_setting_chn = NUM_CHANNEL - 1;
	ST_SELF.cmd_hdl(ST_SELF.p_cmd_rcv, sycmd_reflush, NULL);
	return -1;
}
static int SAM_Key_LT(void *arg)
{
	
	return -1;
}

static int SAM_Key_RT(void *arg)
{
	
	return -1;
}
static int SAM_Key_ET(void *arg)
{
	return -1;
	
}






