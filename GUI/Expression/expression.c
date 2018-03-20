

/*
expression :: = 

variable = [ A-Z, a-z]*



*/




/*��д
���棡 name=val �ĵȺ�ǰ�������пո񣡣�����
clr : colour
f : font
bu : button
UDT��д
pa	: 	paragraph����
rct :	rectangle ����

scr : 	Screen region ��Ļ����
sel	:	selection 	ѡ������
x/yali :  aligning ���뷽ʽ

cg	:	columnGap �м��
ls	:	lineSpacing	�м��
bx/by : ͼ����x/y���ϵĳ���
vx0,vy0:	����Ļ�ϵ���ʼ����
grap: 		����������߿�ļ��
m		: ����ͼƬ���
mdl : Ҫ�󶨵�ģ�͵�����
aux:	��ģ����صĲ���

gr	:	grid
bndx1/y1/x2/y2 : bonduary x1/y1/x2/y2
spr: separator�ָ���

pmx/y : permille x/y�᳤��ռ��Ļ��x/y��ߴ��ǧ�ֱ�
*/

/*

��ͼԪ���Լ̳и�ͼԪ�����ԣ�����ɫ������ɫ������
�����ͼԪ�Լ�ָ�������ԣ�������ʹ���Լ��ƶ���
����Լ�û��ָ���������Ҹ�ͼԪ�����ԣ������ͼԪû���ṩ����ʹ��ϵͳĬ�ϵ�����

*/



















//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
#include "expression.h"
#include <string.h>
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
const Except_T Exp_Failed = { "[expression]" };

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------
//ÿ�ֱ��ʽ�ڽ���ʱʹ�ã���������ͱ���ָ�Ϊ0
char expTempBUf[TEMPBUF_LEN];



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

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
//��Ч�����ȼ���
int Set_shtContextAtt( char *p_att, sheet *p_sht)
{
	
	char tmpbuf[8] = {0};
	p_sht->cnt.colour = String2Clr( p_att);
	
	p_sht->cnt.bkc  = String2CntEff( p_att);
	if( p_sht->cnt.bkc == 0xff) {
		p_sht->cnt.bkc  = String2Bkc( p_att);
	} else {
		p_sht->cnt.effects = GP_SET_EFF( p_sht->cnt.effects, EFF_BKPIC) ;
	}
	
	p_sht->cnt.font = String2Font( p_att);
	
	if( GetKeyVal( p_att, "mdl", tmpbuf, 8))
	{
		p_sht->p_mdl = ModelCreate(tmpbuf); 
		
		
	}
//	else {
//		p_sht->p_mdl = NULL;
//		
//	}
	
	if( GetKeyVal( p_att, "aux", tmpbuf, 8))
	{
		p_sht->cnt.mdl_aux = atoi(tmpbuf); 
		
		
	}
	else {
		p_sht->cnt.mdl_aux = 0xff;
		
	}
	
	return SET_ATT_OK;
}

int Set_shtAreaAtt( char *p_att, sheet *p_sht)
{
	int 	ret = SET_ATT_OK;
	
	char 	tmpbuf[4] = {0};


	GetKeyVal( p_att, "xali", tmpbuf, 4);
	p_sht->area.alix = String2Align( tmpbuf);
	GetKeyVal( p_att, "yali", tmpbuf, 4);
	p_sht->area.aliy = String2Align( tmpbuf);
	
	if( GetKeyVal( p_att, "vx0", tmpbuf, 4))
	{
		p_sht->area.x0 = atoi( tmpbuf);
	}
	if( GetKeyVal( p_att, "vx1", tmpbuf, 4))
	{
		p_sht->area.x1 = atoi( tmpbuf);
	}
	if( GetKeyVal( p_att, "vy0", tmpbuf, 4))
	{
		p_sht->area.y0 = atoi( tmpbuf);
	}
	if( GetKeyVal( p_att, "vy1", tmpbuf, 4))
	{
		p_sht->area.y1 = atoi( tmpbuf);
	}
	
	if( GetKeyVal( p_att, "grap", tmpbuf, 4))
	{
		p_sht->area.grap = atoi( tmpbuf);
		
	}
	else
	{
		p_sht->area.grap  = 0;
	}
	
	if( GetKeyVal( p_att, "cg", tmpbuf, 4))
	{
//		p_sht->subAtt.subColGrap = atoi( tmpbuf);
		p_sht->area.offset_x = atoi( tmpbuf);
	}
	else
	{
//		p_sht->subAtt.subColGrap = 0;
		p_sht->area.offset_x = 0;
	}
	
	
	if( GetKeyVal( p_att, "rg", tmpbuf, 4))
	{
//		p_sht->subAtt.subRowGrap  = atoi( tmpbuf);
		p_sht->area.offset_y = atoi( tmpbuf);
	}
	else
	{
//		p_sht->subAtt.subRowGrap = 0;
		p_sht->area.offset_y = 0;
	}
	
	if( GetKeyVal( p_att, "bx", tmpbuf, 4))
	{
		p_sht->bxsize = atoi( tmpbuf);
	}
	else
	{
		ret |= SET_ATT_BSIZEFAIL;
	}
	if( GetKeyVal( p_att, "by", tmpbuf, 4))
	{
		p_sht->bysize = atoi( tmpbuf);
	}
	else
	{
		ret |= SET_ATT_BSIZEFAIL;
	}
	
	if( GetKeyVal( p_att, "xn", tmpbuf, 4))
	{
		p_sht->area.xn = atoi( tmpbuf);
	}
	else
	{
		p_sht->area.xn = 0;
	}
	if( GetKeyVal( p_att, "yn", tmpbuf, 4))
	{
		p_sht->area.yn = atoi( tmpbuf);
	}
	else
	{
		p_sht->area.yn = 0;
	}
	if( GetKeyVal( p_att, "n", tmpbuf, 4))
	{
		p_sht->area.n = atoi( tmpbuf);
	}
	else
	{
		p_sht->area.n = 0;
	}
	
	return ret;
}

ABS_CTOR( Expr)


END_ABS_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{




