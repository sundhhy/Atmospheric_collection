#include "HMI_input_psd.h"
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

#include "ModelFactory.h"

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
输入密码
	000000
序列号: 89abcdef
*/
static ro_char psd_code_title[] = { "<text vx0=0 vy0=0>输入密码</>" };

static ro_char psd_password[] = { "<text vx0=8 vy0=24>000000=</>" };

static ro_char psd_enter_ser_NO[] = { "<text vx0=0 vy0=48>序列号:</>" };
static ro_char psd_data_ser_NO[] = { "<text vx0=64 vy0=48>89abcdef</>" };

#define PSD_TEMP_RAM_NUM					0

#define PSD_INPUT_PSD_SHT					arr_p_pool_shts[1]
#define PSD_SERNO_SHT						arr_p_pool_shts[3]

#define PSD_STR_PSD_MAX_NUM					5
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

typedef struct {
	char		str_ser_no[9];
	char		str_psd[7];
	uint8_t		arr_psd[3];
	uint8_t		focuse_byte;
	
	
	
}input_psd_mgr;


//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static HMI_input_psd *signal_psd;


//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	PSD_Init( HMI *self, void *arg);
static void	PSD_Show( HMI *self);
static void PSD_Hide( HMI *self );
static void PSD_initSheet( HMI *self );
static void PSD_Run(HMI *self);
static void	PSD_Init_focus(HMI *self);
static void PSD_Hit( HMI *self, char kcd);


static void PSD_choice(HMI *self, uint8_t cid);
static int PSD_Delay_time_hit(HMI *self, char kcd);

static void PSD_Show_foucs_byte(int byte);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
HMI_input_psd *Get_HMI_input_psd(void)
{
	if( signal_psd == NULL)
	{
		signal_psd = HMI_input_psd_new();
		if(signal_psd  == NULL) while(1);
	}
	
	return signal_psd;
}










CTOR( HMI_input_psd)
SUPER_CTOR( HMI);
FUNCTION_SETTING( HMI.init, PSD_Init);
FUNCTION_SETTING( HMI.hide, PSD_Hide);
FUNCTION_SETTING( HMI.initSheet, PSD_initSheet);
FUNCTION_SETTING( HMI.hmi_run, PSD_Run);

FUNCTION_SETTING( HMI.show, PSD_Show);
FUNCTION_SETTING( HMI.hitHandle, PSD_Hit);

FUNCTION_SETTING(HMI.init_focus, PSD_Init_focus);


END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
static void PSD_Run(HMI *self)
{
	
	
	
}


static int	PSD_Init( HMI *self, void *arg)
{
//	HMI 					*p_cmm;


	
//	p_cmm = CreateHMI( HMI_CMM);
//	p_cmm->init( p_cmm, NULL);
	self->flag = 0;
	//初始化界面组件

	
//	self->initSheet( self);

	
	return RET_OK;
	

}




static void PSD_initSheet( HMI *self )
{
	HMI_input_psd		*cthis = SUB_PTR( self, HMI, HMI_input_psd);
	Expr 		*p_exp ;
	input_psd_mgr	*p_psd;
	int 		h = 0;
	Model		*p_mdl;
	struct		tm		t;
	
	p_exp = ExpCreate( "text");
	
	
	
	p_exp->inptSht(p_exp, (void *)psd_code_title, arr_p_pool_shts[0]);
	p_exp->inptSht(p_exp, (void *)psd_password, arr_p_pool_shts[1]);
	p_exp->inptSht(p_exp, (void *)psd_enter_ser_NO, arr_p_pool_shts[2]);
	p_exp->inptSht(p_exp, (void *)psd_data_ser_NO, arr_p_pool_shts[3]);

	

	
	h = 0;
	
	
	Sheet_updown(arr_p_pool_shts[0], h++);
	Sheet_updown(arr_p_pool_shts[1], h++);
	Sheet_updown(arr_p_pool_shts[2], h++);
	Sheet_updown(arr_p_pool_shts[3], h++);
	
	
	
	HMI_Ram_init();
	arr_p_vram[0] = HMI_Ram_alloc(48);
	p_psd = (input_psd_mgr *)arr_p_vram[PSD_TEMP_RAM_NUM];
	
	sprintf(p_psd->str_ser_no, "%x", aci_sys.sys_conf.ser_no);
	PSD_SERNO_SHT->cnt.data = p_psd->str_ser_no;
	PSD_SERNO_SHT->cnt.len = 0;
	
	memset(p_psd->arr_psd, 0, sizeof(p_psd->arr_psd));
	Print_psd(p_psd->str_psd, p_psd->arr_psd);
	PSD_INPUT_PSD_SHT->cnt.data = p_psd->str_psd;
	PSD_INPUT_PSD_SHT->cnt.len = 0;
	
	
	self->init_focus(self);
	
	
	
	
	
	
	
	
	
	
	
}


static void PSD_Hide(HMI *self)
{
	int i;
	
	for( i = 3; i >= 0; i--) {		
		Sheet_updown(arr_p_sht_choices[i], -1);
	}
	

	
	
	

}	

static void	PSD_Init_focus(HMI *self) 
{
	input_psd_mgr *p_psd = (input_psd_mgr *)arr_p_vram[PSD_TEMP_RAM_NUM];
	
	p_psd->focuse_byte = 0;
}




static void	PSD_Show( HMI *self)
{

	input_psd_mgr *p_psd = (input_psd_mgr *)arr_p_vram[PSD_TEMP_RAM_NUM];
	Sheet_refresh(arr_p_pool_shts[0]);
	PSD_Show_foucs_byte(p_psd->focuse_byte);
}



static void PSD_Show_foucs_byte(int byte)
{
	uint16_t					txt_xsize, txt_ysize;
	//显示某个位置的焦点特效时把其他位置的焦点就清除掉了
	//如果输入-1，就清除掉其他焦点
	PSD_INPUT_PSD_SHT->cnt.effects = GP_CLR_EFF(PSD_INPUT_PSD_SHT->cnt.effects, EFF_FOCUS);
	PSD_INPUT_PSD_SHT->p_gp->vdraw(PSD_INPUT_PSD_SHT->p_gp, &PSD_INPUT_PSD_SHT->cnt, &PSD_INPUT_PSD_SHT->area);
	
	if(byte < 0)
		return;
	if(byte > PSD_STR_PSD_MAX_NUM)
		return;
	//显示焦点的那个字符
	PSD_INPUT_PSD_SHT->p_gp->getSize(PSD_INPUT_PSD_SHT->p_gp, PSD_INPUT_PSD_SHT->cnt.font, &txt_xsize, &txt_ysize);
	PSD_INPUT_PSD_SHT->cnt.effects = GP_SET_EFF(PSD_INPUT_PSD_SHT->cnt.effects, EFF_FOCUS);
	PSD_INPUT_PSD_SHT->cnt.data += byte;
	PSD_INPUT_PSD_SHT->cnt.len = 1;
	PSD_INPUT_PSD_SHT->area.x0 += txt_xsize * byte;
	PSD_INPUT_PSD_SHT->p_gp->vdraw(PSD_INPUT_PSD_SHT->p_gp, &PSD_INPUT_PSD_SHT->cnt, &PSD_INPUT_PSD_SHT->area);
	
	//恢复图层
	PSD_INPUT_PSD_SHT->cnt.data -= byte;
	PSD_INPUT_PSD_SHT->cnt.len = 0;	
	PSD_INPUT_PSD_SHT->area.x0 -= txt_xsize * byte;
		
}





static void PSD_Hit(HMI *self, char kcd)
{
	input_psd_mgr *p_psd = (input_psd_mgr *)arr_p_vram[PSD_TEMP_RAM_NUM];
	char *p = PSD_INPUT_PSD_SHT->cnt.data;
	HMI		*p_mt;
	switch(kcd)
	{
		case KEYCODE_LEFT:
			if(p_psd->focuse_byte > 0)
			{
				
				p_psd->focuse_byte --;
				
			}
			else
				p_psd->focuse_byte = PSD_STR_PSD_MAX_NUM;
				
			PSD_Show_foucs_byte(p_psd->focuse_byte);
			Flush_LCD();
				
			break;
		case KEYCODE_RIGHT:	
			if(p_psd->focuse_byte < PSD_STR_PSD_MAX_NUM)
			{
				p_psd->focuse_byte ++;
			}
			else
			{
				p_psd->focuse_byte = 0;
				
			}
				
				
			PSD_Show_foucs_byte(p_psd->focuse_byte);
			Flush_LCD();
			break;
		case KEYCODE_UP:
			
			Str_Calculations(p + p_psd->focuse_byte, 1,  OP_ADD, 1, 0, 9);
			PSD_Show_foucs_byte(p_psd->focuse_byte);
			Flush_LCD();
				
			break;
		case KEYCODE_DOWN:
			
			Str_Calculations(p + p_psd->focuse_byte, 1,  OP_SUB, 1, 0, 9);
			PSD_Show_foucs_byte(p_psd->focuse_byte);
			Flush_LCD();
				
			break;
		case KEYCODE_SWITCH:
			
			break;
		case KEYCODE_ENTER:
			Str_set_password(p_psd->arr_psd, p);
		
			if(p_psd->arr_psd[0] != aci_sys.sys_conf.password[0])
				break;
			if(p_psd->arr_psd[1] != aci_sys.sys_conf.password[2])
				break;
			if(p_psd->arr_psd[1] != aci_sys.sys_conf.password[2])
				break;
			
			p_mt = CreateHMI(HMI_MAINTAIN);
			self->switchHMI(self, p_mt);
		
			break;		
		case KEYCODE_ESC:
			self->switchBack(self);

			
			break;	
		
	}

	return;
}









