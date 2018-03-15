#include "HMI_data.h"
#include "HMI_menu.h"
#include "HMIFactory.h"
#include "sdhDef.h"
#include "ExpFactory.h"
#include "format.h"
#include "chnInfoPic.h"
#include "ModelFactory.h"


//��״ͼ��y�����ϣ���100%��ʾ�Ļ���:71 -187 
//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------

HMI *g_p_dataHmi;
//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------





//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define		DATAHMI_BKPICNUM		"13"

#define		DATAHMI_TITLE	"���Ի���"

//static const char datahmi_code_bkPic[] =  {"<bpic vx0=0 vy0=0 m=0 >21</>" };
static const char datahmi_code_data[] = { "<text f=32 m=0 aux=0>100</>" };



//static const hmiAtt_t	barHmiAtt = { 4,4, COLOUR_GRAY, BARHMI_NUM_BTNROW, BARHMI_NUM_BTNCOL};


//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------


	
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int	Init_dataHMI( HMI *self, void *arg);
static void DataHmi_InitSheet( HMI *self );
static void DataHmi_HideSheet( HMI *self );

static void	DataHmi_Show( HMI *self);


static void	DataHmi_HitHandle( HMI *self, char *s);

//����






//static int DataHmi_MdlUpdata( Observer *self, void *p_srcMdl);
//static void Bulid_dataSheet( dataHMI *self);
//static void DataHmi_update(dataHMI *self);
//ͨ������
static void DataHmi_Init_chnSht(void);
static int DataHmi_Data_update(void *p_data, void *p_mdl);
static int DataHmi_Util_update(void *p_data, void *p_mdl);
static int DataHmi_Alarm_update(void *p_data, void *p_mdl);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

dataHMI *Get_dataHMI(void)
{
	static dataHMI *singal_dataHmi = NULL;
	if( singal_dataHmi == NULL)
	{
		singal_dataHmi = dataHMI_new();
		if(singal_dataHmi  == NULL) while(1);
		g_p_dataHmi = SUPER_PTR( singal_dataHmi, HMI);
		
	}
	
	return singal_dataHmi;
	
}

CTOR( dataHMI)
SUPER_CTOR( HMI);
FUNCTION_SETTING( HMI.init, Init_dataHMI);
FUNCTION_SETTING( HMI.initSheet, DataHmi_InitSheet);
FUNCTION_SETTING( HMI.hide, DataHmi_HideSheet);
FUNCTION_SETTING( HMI.show, DataHmi_Show);

FUNCTION_SETTING( HMI.hitHandle, DataHmi_HitHandle);

//FUNCTION_SETTING( Observer.update, DataHmi_MdlUpdata);

END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

static int	Init_dataHMI( HMI *self, void *arg)
{
//	dataHMI		*cthis = SUB_PTR( self, HMI, dataHMI);
//	Expr 			*p_exp ;
//	shtctl 			*p_shtctl = NULL;
	
//	p_shtctl = GetShtctl();
	
	//��ʼ������ͼƬ
//	p_exp = ExpCreate( "pic");
	
	self->flag = 0;
//	Bulid_dataSheet(cthis);

	//��ʼ������
//	self->init_focus(self);
//	cthis->flags = 0;
	
	return RET_OK;

}



static void DataHmi_InitSheet( HMI *self )
{
//	dataHMI			*cthis = SUB_PTR( self, HMI, dataHMI);
	int  h = 0;

	g_p_sht_bkpic->cnt.data = DATAHMI_BKPICNUM;

	g_p_sht_title->cnt.data = DATAHMI_TITLE;
	g_p_sht_title->cnt.len = strlen(DATAHMI_TITLE);
	
	Sheet_updown(g_p_sht_bkpic, h++);
	Sheet_updown(g_p_sht_title, h++);
	Sheet_updown(g_p_shtTime, h++);
//	for( i = 0; i < BARHMI_NUM_BARS; i++) {
//		Sheet_updown(g_arr_p_chnData[i], h++);
//		Sheet_updown(g_arr_p_chnUtil[i], h++);
//		Sheet_updown(g_arr_p_chnAlarm[i], h++);
//	}
	
	DataHmi_Init_chnSht();
	
	
}

static void DataHmi_HideSheet( HMI *self )
{
//	dataHMI			*cthis = SUB_PTR( self, HMI, dataHMI);
	
//	int i;
	

//	cthis->flags = 0;
//	for( i = BARHMI_NUM_BARS - 1; i >= 0; i--) {
//		Sheet_updown(g_arr_p_chnData[i], -1);
//		Sheet_updown(g_arr_p_chnUtil[i], -1);
//		Sheet_updown(g_arr_p_chnAlarm[i], -1);
//	}
	Sheet_updown(g_p_shtTime, -1);
	Sheet_updown(g_p_sht_title, -1);
	Sheet_updown(g_p_sht_bkpic, -1);
//	self->clear_focus(self, 0, 0);
}	





static void	DataHmi_Show( HMI *self )
{
//	dataHMI		*cthis = SUB_PTR( self, HMI, dataHMI);
	g_p_curHmi = self;
	
	
//	DataHmi_update( cthis);
	Sheet_refresh(g_p_sht_bkpic);
//	cthis->flags = 1;
//	self->show_focus( self, 0, 0);
}

static void	DataHmi_HitHandle( HMI *self, char *s)
{
//	dataHMI		*cthis = SUB_PTR( self, HMI, dataHMI);
//	shtCmd		*p_cmd;


	if( !strcmp( s, HMIKEY_ENTER))
	{
		self->switchHMI(self, g_p_HMI_menu);
		
	}
	if( !strcmp( s, HMIKEY_ESC))
	{
		self->switchBack(self);
	}
	
}

static void DataHmi_Init_chnSht(void)
{
	Expr 		*p_exp ;
//	Model		*p_mdl = NULL;
	int			i = 0;
	p_exp = ExpCreate( "text");
	for(i = 0; i < NUM_CHANNEL; i++) {
		p_exp->inptSht( p_exp, (void *)datahmi_code_data, g_arr_p_chnData[i]) ;
		
		
		
		
		g_arr_p_chnData[i]->cnt.colour = arr_clrs[i];
		
		
		
		
		g_arr_p_chnData[i]->update = DataHmi_Data_update;
		g_arr_p_chnUtil[i]->update = DataHmi_Util_update;
		g_arr_p_chnAlarm[i]->update = DataHmi_Alarm_update;

		//����Ϊ�˳�ʼ����ʱ�򣬾��������ݵõ���ȷ������
//		g_arr_p_chnData[i]->update(g_arr_p_chnData[i], NULL);
//		g_arr_p_chnAlarm[i]->update(g_arr_p_chnAlarm[i], NULL);
//		g_arr_p_chnUtil[i]->update(g_arr_p_chnUtil[i], NULL);
	
	}
}

//������ʾ��������������ʵ�λ��
//��������
//static void DataHmi_update(dataHMI *self)
//{
//	uint8_t		up_y = 30;
//	uint8_t		right_x = 160;
//	uint8_t		box_sizey = 70;		
//	uint8_t		box_sizex = 160;		
//	
//	//�����ܱ߽�Ŀ�϶
//	uint8_t		space_to_up = 		2;	
//	uint8_t		space_to_bottom = 	2;
//	uint8_t		space_to_left = 	8;	
//	uint8_t		space_to_right = 	8;
//	
//	char 			i = 0, j = 0;
//	uint16_t 		sizex = 0;
//	uint16_t 		sizey = 0;
//	sheet			*p_sht;
//	
//	//ˢ��һ������,����0 - 10000�������, ����ʱʹ��
////	self->arr_p_sht_data[i]->p_mdl->getMdlData( self->arr_p_sht_data[i]->p_mdl, 10000, NULL);		
//	
//	for( i = 0; i < 3; i++) { 
//		for( j = 0; j < 2; j++) {
//			
//			//������ֵ
//			p_sht = self->arr_p_sht_data[i * 2 + j];
//			p_sht->cnt.data = \
//				p_sht->p_mdl->to_string( p_sht->p_mdl, p_sht->cnt.mdl_aux, NULL);
//			p_sht->cnt.len = strlen( p_sht->cnt.data);
//			p_sht->p_gp->getSize( p_sht->p_gp, p_sht->cnt.font, &sizex, &sizey);
//			sizex = sizex * p_sht->cnt.len;	

//			p_sht->area.x0 = right_x +  (j ) * box_sizex - space_to_right - sizex;
//			p_sht->area.y0 = up_y + i * box_sizey + space_to_up;
//			
//			
//			
//			//���㵥λ������
//			p_sht = self->arr_p_sht_unit[i * 2 + j];
//			p_sht->cnt.data = \
//				p_sht->p_mdl->to_string( p_sht->p_mdl, p_sht->cnt.mdl_aux, NULL);
//			p_sht->cnt.len = strlen( p_sht->cnt.data);
//			p_sht->p_gp->getSize( p_sht->p_gp, p_sht->cnt.font, &sizex, &sizey);
//			sizex = sizex * p_sht->cnt.len;	

//			p_sht->area.x0 = right_x +  j * box_sizex - space_to_right - sizex;
//			p_sht->area.y0 = up_y + ( i + 1)* box_sizey - space_to_bottom - sizey;
//			
//			//���㱨����Ϣ������
//			
//			p_sht = self->arr_p_sht_alarm[i * 2 + j];
//			p_sht->cnt.data = \
//				p_sht->p_mdl->to_string( p_sht->p_mdl, p_sht->cnt.mdl_aux, NULL);
//			p_sht->cnt.len = strlen( p_sht->cnt.data);
//			p_sht->p_gp->getSize( p_sht->p_gp, p_sht->cnt.font, &sizex, &sizey);
//			sizex = sizex * p_sht->cnt.len;	
//			//������Ϣ������ʾ
//			p_sht->area.x0 = j * box_sizex + space_to_left;
//			p_sht->area.y0 = up_y + ( i + 1)* box_sizey - space_to_bottom - sizey;
//		}
//			
//	}
//	
//}

// 

//static void Bulid_dataSheet( dataHMI *self)
//{
//	
//	Expr 			*p_exp ;
//	shtctl 			*p_shtctl = NULL;
//	Model			*p_mdl = NULL;
//	short 			i;
//	
//	self->arr_p_sht_unit = g_arr_p_chnUtil;
//	self->arr_p_sht_alarm = g_arr_p_chnAlarm;
//	
//	p_shtctl = GetShtctl();

//	for( i = 0; i < BARHMI_NUM_BARS; i++) {
//		
//		
//		p_exp = ExpCreate("text");
//		self->arr_p_sht_data[i] = Sheet_alloc( p_shtctl);
//		p_exp->inptSht( p_exp, (void *)datahmi_code_data, self->arr_p_sht_data[i]) ;
//		
////		self->arr_p_sht_data[i]->p_mdl->attach( self->arr_p_sht_data[i]->p_mdl, ( Observer *)self->arr_p_sht_data[i]);		
//		self->arr_p_sht_data[i]->cnt.colour = arr_clrs[i];
//		
//	}
//	
//	//todo: �ĳ�ͨ��
//	p_mdl = ModelCreate("test");
//	p_mdl->attach(p_mdl, &self->Observer);
//	
//}


//static int DataHmi_MdlUpdata( Observer *self, void *p_srcMdl)
//{
//	dataHMI *cthis = SUB_PTR( self, Observer, dataHMI);
//	Model	*mdl = (Model *)p_srcMdl;
//	
//	if(cthis->flags == 0)
//		return RET_OK;
//	DataHmi_update( cthis);
//	Sheet_refresh( cthis->arr_p_sht_data[0]);
//	
//	return RET_OK;
//	
//}
//��������
static int DataHmi_Data_update(void *p_data, void *p_mdl)
{
	uint8_t		up_y = 30;
	uint8_t		right_x = 160;
	uint8_t		box_sizey = 70;		
	uint8_t		box_sizex = 160;		
	
	//�����ܱ߽�Ŀ�϶
	uint8_t		space_to_up = 		2;	
//	uint8_t		space_to_bottom = 	2;
//	uint8_t		space_to_left = 	8;	
	uint8_t		space_to_right = 	8;
	
	char 			row = 0, col = 0;
	uint16_t 		sizex = 0;
	uint16_t 		sizey = 0;
	
	sheet			*p_sht = (sheet *)p_data;
	
		
	if(IS_HMI_HIDE(g_p_dataHmi->flag))
		return 0;
	
	row = p_sht->id / 2;
	col = p_sht->id % 2;

	p_sht->cnt.data = \
		p_sht->p_mdl->to_string( p_sht->p_mdl, p_sht->cnt.mdl_aux, NULL);
	p_sht->cnt.len = strlen( p_sht->cnt.data);
	p_sht->p_gp->getSize( p_sht->p_gp, p_sht->cnt.font, &sizex, &sizey);
	sizex = sizex * p_sht->cnt.len;	

	p_sht->area.x0 = right_x +  (col ) * box_sizex - space_to_right - sizex;
	p_sht->area.y0 = up_y + row * box_sizey + space_to_up;
	
//	if(Sheet_is_hide(p_sht))
//		return 0;
	
	Sheet_force_slide( p_sht);
	return 0;
	
	
	
}

static int DataHmi_Util_update(void *p_data, void *p_mdl)
{
	uint8_t		up_y = 30;
	uint8_t		right_x = 160;
	uint8_t		box_sizey = 70;		
	uint8_t		box_sizex = 160;		
	
	//�����ܱ߽�Ŀ�϶
//	uint8_t		space_to_up = 		2;	
	uint8_t		space_to_bottom = 	2;
//	uint8_t		space_to_left = 	8;	
	uint8_t		space_to_right = 	8;
	
	char 			row = 0, col = 0;
	uint16_t 		sizex = 0;
	uint16_t 		sizey = 0;
	sheet			*p_sht = (sheet *)p_data;
	
		
	
	if(IS_HMI_HIDE(g_p_dataHmi->flag))
		return 0;
	
	row = p_sht->id / 2;
	col = p_sht->id % 2;

	p_sht->cnt.data = \
		p_sht->p_mdl->to_string( p_sht->p_mdl, p_sht->cnt.mdl_aux, NULL);
	p_sht->cnt.len = strlen( p_sht->cnt.data);
	p_sht->p_gp->getSize( p_sht->p_gp, p_sht->cnt.font, &sizex, &sizey);
	sizex = sizex * p_sht->cnt.len;	

	p_sht->area.x0 = right_x +  (col ) * box_sizex - space_to_right - sizex;
	p_sht->area.y0 = up_y  + (row + 1) * box_sizey  -( sizey + space_to_bottom);
	
//	if(Sheet_is_hide(p_sht))
//		return 0;
	
	Sheet_force_slide( p_sht);
	return 0;
	
}
static int DataHmi_Alarm_update(void *p_data, void *p_mdl)
{
	uint8_t		up_y = 30;
//	uint8_t		right_x = 160;
	uint8_t		box_sizey = 70;		
	uint8_t		box_sizex = 160;		
	
	//�����ܱ߽�Ŀ�϶
//	uint8_t		space_to_up = 		2;	
	uint8_t		space_to_bottom = 	2;
	uint8_t		space_to_left = 	8;	
//	uint8_t		space_to_right = 	8;
	
	char 			row = 0, col = 0;
	uint16_t 		sizex = 0;
	uint16_t 		sizey = 0;
	sheet			*p_sht = (sheet *)p_data;
		
	if(IS_HMI_HIDE(g_p_dataHmi->flag))
		return 0;
	
	row = p_sht->id / 2;
	col = p_sht->id % 2;

	p_sht->cnt.data = \
		p_sht->p_mdl->to_string( p_sht->p_mdl, p_sht->cnt.mdl_aux, NULL);
	p_sht->cnt.len = strlen( p_sht->cnt.data);
	p_sht->p_gp->getSize( p_sht->p_gp, p_sht->cnt.font, &sizex, &sizey);
	sizex = sizex * p_sht->cnt.len;	

	p_sht->area.x0 = (col ) * box_sizex + space_to_left;
	p_sht->area.y0 =  up_y  + (row + 1) * box_sizey  -( sizey + space_to_bottom);
	
//	if(Sheet_is_hide(p_sht))
//		return 0;
//	if(IS_HMI_HIDE(g_p_dataHmi->flag))
//		return 0;
	Sheet_force_slide( p_sht);
	return 0;
	
}


