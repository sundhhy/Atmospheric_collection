#include "HMI_start_sample.h"
#include "HMIFactory.h"
#include "sdhDef.h"
#include "ExpFactory.h"
#include "format.h"
#include "chnInfoPic.h"
#include "focus.h"
#include "Component_Button.h"
#include "Component_curve.h"
#include "os/os_depend.h"
#include "Component_tips.h"

#include "utils/keyboard.h"
#include "configure_strategy.h"

//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

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
��������		<�۳�>
	��ǰʱ�䣻13��41
	��ʱʱ��	00��00
		����		����
*/
static ro_char sts_code_title[] = { "<text vx0=0 vy0=0 bkc=white>��������</>" };
static ro_char sts_code_subtitle[] = { "<text vx0=72 vy0=0 bkc=white><�۳�></>" };

static ro_char sts_code_entry_1[] = { "<text vx0=8 vy0=16 bkc=white>��ǰʱ��=</>" };
static ro_char	sts_code_time[] = {"<text vx0=80 vy0=16 bkc=white>13:14</>"};


static ro_char sts_code_choice_1_1[] = { "<text vx0=8 vy0=32 bkc=white>��ʱʱ��</>" };
static ro_char	sts_code_choice_1_2[] = {"<text vx0=80 vy0=32 bkc=white>00:00</>"};

static ro_char sts_code_choice_2_1[] = { "<text vx0=16 vy0=48 bkc=white>����</>" };
static ro_char sts_code_choice_2_2[] = { "<text vx0=80 vy0=48 bkc=white>����</>" };

#define STS_TEMP_RAM_NUM					1

#define STS_SING_TYPE_SHT					arr_p_pool_shts[1]
#define STS_TIM_SHT							arr_p_pool_shts[3]
#define STS_DELAY_SHT						arr_p_sht_choices[0]
#define STS_DELAY_TIM_SHT					arr_p_sht_choices[1]

//#define STS_ID_DELAY_TIM_TEXT			0
//#define STS_ID_DELAY_TIM					1
#define STS_ID_ZERO					2
#define STS_ID_START				3
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------
typedef struct {
	char 		is_edit;		//��ǰ�����Ƿ��ڱ༭����
	char		focus_byte;
	char		handle_signal_type;
	
	char		none;
	
}sts_delay_time_t;
//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static HMI_start_sample *signal_start_sample;
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	STS_Init( HMI *self, void *arg);
static void	STS_Show( HMI *self);
static void STS_Hide( HMI *self );
static void STS_initSheet( HMI *self );
static void STS_Run(HMI *self);
static void	STS_Init_focus(HMI *self);
static void STS_hit( HMI *self, char kcd);


static void STS_choice(HMI *self, uint8_t cid);
static int STS_Delay_time_hit(HMI *self, char kcd);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
HMI_start_sample *Get_HMI_start_sample(void)
{
	if( signal_start_sample == NULL)
	{
		signal_start_sample = HMI_start_sample_new();
		if(signal_start_sample  == NULL) while(1);
	}
	
	return signal_start_sample;
}










CTOR( HMI_start_sample)
SUPER_CTOR( HMI);
FUNCTION_SETTING( HMI.init, STS_Init);
FUNCTION_SETTING( HMI.hide, STS_Hide);
FUNCTION_SETTING( HMI.initSheet, STS_initSheet);
FUNCTION_SETTING( HMI.hmi_run, STS_Run);

FUNCTION_SETTING( HMI.show, STS_Show);
FUNCTION_SETTING( HMI.hitHandle, STS_hit);

FUNCTION_SETTING(HMI.init_focus, STS_Init_focus);

END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
static void STS_Run(HMI *self)
{
	
	
	
}


static int	STS_Init( HMI *self, void *arg)
{
//	HMI 					*p_cmm;


	
//	p_cmm = CreateHMI( HMI_CMM);
//	p_cmm->init( p_cmm, NULL);
	self->flag = 0;
	//��ʼ���������

	
//	self->initSheet( self);

	
	return RET_OK;
	

}




static void STS_initSheet( HMI *self )
{
//	HMI_start_sample		*cthis = SUB_PTR( self, HMI, HMI_start_sample);
	Expr 		*p_exp ;
	sts_delay_time_t	*p_d;
	int 		h = 0;
	
	
	p_exp = ExpCreate( "text");
	
	

	
	p_exp->inptSht(p_exp, (void *)sts_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)sts_code_subtitle, arr_p_pool_shts[1]);
	p_exp->inptSht(p_exp, (void *)sts_code_entry_1, arr_p_pool_shts[2]);
	p_exp->inptSht(p_exp, (void *)sts_code_time, arr_p_pool_shts[3]);

	p_exp->inptSht(p_exp, (void *)sts_code_choice_1_1, arr_p_sht_choices[0]);
	p_exp->inptSht(p_exp, (void *)sts_code_choice_1_2, arr_p_sht_choices[1]);
	p_exp->inptSht(p_exp, (void *)sts_code_choice_2_1, arr_p_sht_choices[2]);
	p_exp->inptSht(p_exp, (void *)sts_code_choice_2_2, arr_p_sht_choices[3]);
	
	//Ϊ���Ա�����ѡ���ͼ�����ID
	

//	arr_p_sht_choices[0]->sht_id = STS_ID_DELAY_TIM_TEXT;
//	arr_p_sht_choices[1]->sht_id = STS_ID_DELAY_TIM;
	arr_p_sht_choices[2]->sht_id = STS_ID_ZERO;
	arr_p_sht_choices[3]->sht_id = STS_ID_START;

	
	h = 0;
	
	
	Sheet_updown(arr_p_pool_shts[0], h++);
	Sheet_updown(arr_p_pool_shts[1], h++);
	Sheet_updown(arr_p_pool_shts[2], h++);
	Sheet_updown(arr_p_pool_shts[3], h++);
	Sheet_updown(arr_p_sht_choices[0], h++);
	Sheet_updown(arr_p_sht_choices[1], h++);
	Sheet_updown(arr_p_sht_choices[2], h++);
	Sheet_updown(arr_p_sht_choices[3], h++);
	Sheet_updown(g_p_shtTime, h++);
	self->init_focus(self);
	
	
	aci_sys.hmi_mgr.hand_signal_type = em_atmosphere_A;
	STS_SING_TYPE_SHT->cnt.data = (char *)str_signal[aci_sys.hmi_mgr.hand_signal_type];
	STS_SING_TYPE_SHT->cnt.len = 0;	//ʹ��Ĭ�ϳ���
	
	//������ʱʱ������
	HMI_Ram_init();
	arr_p_vram[0] = HMI_Ram_alloc(48);
	arr_p_vram[1] = HMI_Ram_alloc(48);
	
	p_d = (sts_delay_time_t *)arr_p_vram[STS_TEMP_RAM_NUM];
	p_d->is_edit = 0;
	p_d->handle_signal_type = em_atmosphere_A;
	
	STS_DELAY_TIM_SHT->cnt.data = arr_p_vram[0];
	STS_DELAY_TIM_SHT->cnt.len = 0;
	sprintf(STS_DELAY_TIM_SHT->cnt.data, "%02d:%02d", \
		arr_sample_conf[p_d->handle_signal_type].sample_delay_hour, \
		arr_sample_conf[p_d->handle_signal_type].sample_delay_min
	);
	
	
}


static void STS_Hide(HMI *self)
{
	int i;
	
	Sheet_updown(g_p_shtTime, -1);
	for( i = 3; i >= 0; i--) {		
		Sheet_updown(arr_p_sht_choices[i], -1);
	}
	for( i = 3; i >= 0; i--) {		
		Sheet_updown(arr_p_pool_shts[i], -1);
	}

	
	self->clear_focus(self, self->p_fcuu->focus_row, self->p_fcuu->focus_col);
	Focus_free(self->p_fcuu);
	

}	

static void	STS_Init_focus(HMI *self) 
{
	self->p_fcuu = Focus_alloc(2, 2);

		
	Focus_Set_sht(self->p_fcuu, 0, 0, arr_p_sht_choices[0]);
	Focus_Set_sht(self->p_fcuu, 0, 1, arr_p_sht_choices[1]);
	Focus_Set_sht(self->p_fcuu, 1, 0, arr_p_sht_choices[2]);
	Focus_Set_sht(self->p_fcuu, 1, 1, arr_p_sht_choices[3]);
		
//	Focus_Set_focus(self->p_fcuu, 0, 0);
}




static void	STS_Show( HMI *self)
{

	
	Sheet_refresh(arr_p_pool_shts[0]);
	self->show_focus(self);
}




static void	STS_hit( HMI *self, char kcd)
{
	sheet *p_sht;
	HMI		*p_configuring;
	char	focus_change = 0;
	char	old_fcuu_row;
	char	old_fcuu_col;
	
	old_fcuu_row = self->p_fcuu->focus_row;
	old_fcuu_col = self->p_fcuu->focus_col;
	
	//����ʱʱ����һ�У�Ҫ���⴦��Ҫ�ܹ���ʱ���ÿһ�����ֶ��ܵ���ѡ�в�ʵ�ֱ༭
	if(self->p_fcuu->focus_row == 0)
	{
		if(STS_Delay_time_hit(self, kcd) == RET_OK)
			return;
		
	}
	switch(kcd)
	{
		case KEYCODE_LEFT:
			Focus_move_left(self->p_fcuu);
			focus_change = 1;
			break;
		case KEYCODE_RIGHT:
			Focus_move_left(self->p_fcuu);
			focus_change = 1;
			break;
		case KEYCODE_UP:
			Focus_move_up(self->p_fcuu);
			focus_change = 1;
			break;
		case KEYCODE_DOWN:
			Focus_move_down(self->p_fcuu);
			focus_change = 1;
			break;
		case KEYCODE_SWITCH:
			aci_sys.hmi_mgr.hand_signal_type = HMI_Switch_signal(aci_sys.hmi_mgr.hand_signal_type);
			STS_SING_TYPE_SHT->cnt.data = (char *)str_signal[aci_sys.hmi_mgr.hand_signal_type];
			self->show(self);
			break;
		case KEYCODE_ENTER:
			p_sht = Focus_Get_focus(self->p_fcuu);
			if(p_sht)
				STS_choice(self, p_sht->sht_id);
			break;		
		case KEYCODE_ESC:
			
			
			p_configuring = CreateHMI(HMI_CONFIGURE);
			HMI_Change_last_HMI(CreateHMI(HMI_MAIN));		//�����ò������淵�ص�ʱ��ֱ�ӷ��ص��������ϣ���Ҫ���ص�������
			p_configuring->arg[0] = e_stg_set_sample;
			self->switchHMI(self, p_configuring);
			break;	
		
	}


	if(focus_change)
	{
		
		self->clear_focus(self, old_fcuu_row, old_fcuu_col);
		self->show_focus(self);
	}
	
}

//��������ʱ������һ���ھͷ���RET_OK
//��֪����RET_FAILED
//�Բ��ܴ���İ�����Ҳ����RET_FAILED
//�༭���ǣ�00:00
#define STS_TIM_MAX_BYTE			4
static void STS_Show_delay_entry_foucs(int focus)
{
	if(focus)
		STS_DELAY_SHT->cnt.effects = GP_SET_EFF(STS_DELAY_SHT->cnt.effects, EFF_FOCUS);
	else
		STS_DELAY_SHT->cnt.effects = GP_CLR_EFF(STS_DELAY_SHT->cnt.effects, EFF_FOCUS);
	
	STS_DELAY_SHT->p_gp->vdraw(STS_DELAY_SHT->p_gp, &STS_DELAY_SHT->cnt, &STS_DELAY_SHT->area);
	STS_DELAY_SHT->cnt.effects = GP_CLR_EFF(STS_DELAY_SHT->cnt.effects, EFF_FOCUS);
}

static void STS_Show_delay_time_foucs(int byte)
{
	uint16_t					txt_xsize, txt_ysize;
	//��ʾĳ��λ�õĽ�����Чʱ������λ�õĽ�����������
	//�������-1�����������������
	STS_DELAY_TIM_SHT->cnt.effects = GP_CLR_EFF(STS_DELAY_TIM_SHT->cnt.effects, EFF_FOCUS);
	STS_DELAY_TIM_SHT->p_gp->vdraw(STS_DELAY_TIM_SHT->p_gp, &STS_DELAY_TIM_SHT->cnt, &STS_DELAY_TIM_SHT->area);
	
	if(byte < 0)
		return;
	if(byte > STS_TIM_MAX_BYTE)
		return;
	//��ʾ������Ǹ��ַ�
	STS_DELAY_TIM_SHT->p_gp->getSize(STS_DELAY_TIM_SHT->p_gp, STS_DELAY_TIM_SHT->cnt.font, &txt_xsize, &txt_ysize);
	STS_DELAY_TIM_SHT->cnt.effects = GP_SET_EFF(STS_DELAY_TIM_SHT->cnt.effects, EFF_FOCUS);
	STS_DELAY_TIM_SHT->cnt.data += byte;
	STS_DELAY_TIM_SHT->cnt.len = 1;
	STS_DELAY_TIM_SHT->area.x0 += txt_xsize * byte;
	STS_DELAY_TIM_SHT->p_gp->vdraw(STS_DELAY_TIM_SHT->p_gp, &STS_DELAY_TIM_SHT->cnt, &STS_DELAY_TIM_SHT->area);
	
	//�ָ�ͼ��
	STS_DELAY_TIM_SHT->cnt.data -= byte;
	STS_DELAY_TIM_SHT->cnt.len = 0;	
	STS_DELAY_TIM_SHT->area.x0 -= txt_xsize * byte;
		
}
static int STS_Delay_time_hit(HMI *self, char kcd)
{
	int ret = RET_OK;
	sts_delay_time_t		*p_d;
	char *p = STS_DELAY_TIM_SHT->cnt.data;
	p_d = (sts_delay_time_t *)arr_p_vram[STS_TEMP_RAM_NUM];
	
	switch(kcd)
	{
		case KEYCODE_LEFT:
			
		//�ڱ༭���ⲿ��ʱ�򣬽��������༭���ڵ�����λ��
		//�ڱ༭���ڵ�ʱ�򣬽���λ�������С��������㵽���һ��λ�ã��������༭��
			if(p_d->is_edit == 0)
			{
				p_d->is_edit = 1;
				p_d->focus_byte = STS_TIM_MAX_BYTE;
				STS_Show_delay_entry_foucs(0);
				STS_Show_delay_time_foucs(p_d->focus_byte);
				Flush_LCD();
			}
			else
			{
				if(p_d->focus_byte > 0)
				{
					
					p_d->focus_byte --;
					if(p[p_d->focus_byte] == ':')
						p_d->focus_byte --;
					
					STS_Show_delay_time_foucs(p_d->focus_byte);
					Flush_LCD();
				}
				else
				{
					p_d->is_edit = 0;
					STS_Show_delay_time_foucs(-1);
					STS_Show_delay_entry_foucs(1);
					Flush_LCD();
				}
				
				
			}
			break;
		case KEYCODE_RIGHT:
			//�ڱ༭���ⲿ��ʱ�򣬽��������༭���ڵ���ʼλ��
		//�ڱ༭���ڵ�ʱ�򣬽���λ���������ӣ�������㵽������λ�ã��������༭��
			if(p_d->is_edit == 0)
			{
				p_d->is_edit = 1;
				p_d->focus_byte = 0;
				STS_Show_delay_entry_foucs(0);
				STS_Show_delay_time_foucs(p_d->focus_byte);
				Flush_LCD();
			}
			else
			{
				if(p_d->focus_byte < STS_TIM_MAX_BYTE)
				{
					
					p_d->focus_byte ++;
					if(p[p_d->focus_byte] == ':')
						p_d->focus_byte ++;
					STS_Show_delay_time_foucs(p_d->focus_byte);
					Flush_LCD();
				}
				else
				{
					p_d->is_edit = 0;
					STS_Show_delay_time_foucs(-1);
					STS_Show_delay_entry_foucs(1);
					Flush_LCD();
				}
				
				
			}
			break;
		case KEYCODE_UP:
			//���������ʱ��������ϣ����Ǳ༭����
			//����Ͳ�����
			if(p_d->is_edit)
			{
				Str_Calculations(p + p_d->focus_byte, 1,  OP_ADD, 1, 0, 9);
				STS_Show_delay_time_foucs(p_d->focus_byte);
				Flush_LCD();
				if(p_d->focus_byte < 3)
					arr_sample_conf[p_d->handle_signal_type].sample_delay_hour = Get_str_data(p,":", 0, NULL);
				else
					arr_sample_conf[p_d->handle_signal_type].sample_delay_min = Get_str_data(p,":", 1, NULL);
				
			}
			else
			{
				ret = RET_FAILED;
				
			}
			break;
		case KEYCODE_DOWN:
			if(p_d->is_edit)
			{
				Str_Calculations(p + p_d->focus_byte, 1,  OP_SUB, 1, 0, 9);
				STS_Show_delay_time_foucs(p_d->focus_byte);
				Flush_LCD();
				if(p_d->focus_byte < 3)
					arr_sample_conf[p_d->handle_signal_type].sample_delay_hour = Get_str_data(p,":", 0, NULL);
				else
					arr_sample_conf[p_d->handle_signal_type].sample_delay_min = Get_str_data(p,":", 1, NULL);
			}
			else
			{
				ret = RET_FAILED;
				
			}
			break;
		case KEYCODE_SWITCH:
			ret = RET_FAILED;
			
			break;
		case KEYCODE_ENTER:
			ret = RET_FAILED;
			break;		
		case KEYCODE_ESC:
			
			ret = RET_FAILED;
			
			break;	
		
	}

	return ret;
}

static void STS_choice(HMI *self, uint8_t cid)
{
	sts_delay_time_t		*p_d;
	p_d = (sts_delay_time_t *)arr_p_vram[STS_TEMP_RAM_NUM];
	switch(cid)
	{
//		case STS_ID_DELAY_TIM_TEXT:
//			
//			break;
//		case STS_ID_DELAY_TIM:
//			
//			break;
		case STS_ID_ZERO:
			arr_sample_conf[p_d->handle_signal_type].is_zero = 1;
			arr_sample_conf[p_d->handle_signal_type].is_start = 0;
			break;
		case STS_ID_START:
			arr_sample_conf[p_d->handle_signal_type].is_zero = 0;
			arr_sample_conf[p_d->handle_signal_type].is_start = 1;
			break;
	}
	
}








