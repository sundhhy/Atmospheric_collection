#include <stdint.h>
#include "Glyph.h"

#include <string.h>


#include "basis/sdhDebug.h"
#include "sdhDef.h"
#include "basis/sdhError.h"
#include "system.h"

#include "lcd_conf.h"
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

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------

static int Init( Glyph *self, dev_lcd *lcd);
static void Draw_self( Glyph *self);
static void vDraw( Glyph *self, dspContent_t *cnt, vArea_t *area);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
void Flush_LCD(void)
{
	flush_flag = 1;
	
	
}

void Stop_flush_LCD(void)
{
	flush_flag = 0;
	
	
}

void LCD_Run(void)
{
	if(flush_flag) {
		dev_lcd *lcd;
		Dev_open( LCD_DEVID, (void *)&lcd);
		lcd->lcd_flush(0);
		flush_flag = 0;
		
	}
	
}
void CLR_LCD(void)
{
	dev_lcd *lcd;
	Dev_open( LCD_DEVID, (void *)&lcd);
	lcd->clear(PALLET_WHITE);
	
}

int Set_effects(dspContent_t *p_cnt, int eff, int val) 
{
	int ret = RET_OK;
	switch( eff) 
	{
		case EFF_BKPIC:
			p_cnt->effects = GP_SET_EFF( p_cnt->effects, EFF_BKPIC);
			p_cnt->bkc = val;
			break;
			
		
	}
	
	
	return ret;
	
}

uint8_t ColorInvert( uint8_t clr)
{
	
	if( clr == PALLET_WHITE)
		return PALLET_BLACK;
	else
		return PALLET_WHITE;
}

int String2Colour( char *s)
{
	int c;

	if( strstr( s, "white") )
	{
		c = PALLET_WHITE;
		
	}
	else if( strstr( s, "black") )
	{
		c = PALLET_BLACK;
		
	}
	else
	{
		c = DEF_COLOUR;
	}
	return c;
}

int	String2CntEff( char *s)
{
	char s_eff[8] = {0};
	int eff = 0xff;
	
	if( GetKeyVal( s, "m", s_eff, 8)) {
		eff = atoi(s_eff);
	}
	
	
	return eff;
}

int	String2Bkc( char *s)
{
//	char *pp ;
	char colour[8] = {0};
//	char i = 0;
	if( GetKeyVal( s, "bkc", colour, 8))
	{
		
		return String2Colour( colour);
	}
	else
	{
		return ERR_COLOUR;
		
	}
}

int String2Clr( char *s)
{
	char *pp ;
	pp =  strstr( s, "clr=");
	if( pp == NULL)
		return ERR_COLOUR;
	
	pp ++;
	
	return String2Colour( pp);
}

int String2Font( char *s)
{
	int f = FONT_ERR;
	
	if( strstr( s, "f=12") )
	{
		f = FONT_12;
	}
	else if( strstr( s, "f=16") )
	{
		f = FONT_16;
	}
	else if( strstr( s, "f=24") )
	{
		f = FONT_24;
	}
	else if( strstr( s, "f=32") )
	{
		f = FONT_32;
	}
	else if( strstr( s, "f=48") )
	{
		f = FONT_48;
	}
	else if( strstr( s, "f=64") )
	{
		f = FONT_64;
	}
	
	
	return f;
}

int	String2Align( char *s)
{
	int ali = ALIGN_ERR;
	
	if( strstr( s, "l") )
	{
		ali = ALIGN_LEFT;
	}
	else if( strstr( s, "m") )
	{
		ali = ALIGN_MIDDLE;
	}
	else if( strstr( s, "r") )
	{
		ali = ALIGN_RIGHT;
	}
	
	return ali;
}





int	DefaultColour( void *arg)
{
	int c = 0;
	if( arg == NULL)
	{
		c = DEF_COLOUR;
		
	}
	
	
	return c;
	
}




//key=val or key = val 否则返回直接返回0
//以空格结尾
int GetKeyVal( char *s, char *key, char *val, short size)
{
	char *pp = s;
	char i = 0;
	
	memset( val, 0, size);
	while(1)
	{
		pp =  strstr( pp, key);
		if( pp == NULL)
			return 0;
		//防止出现截断的情况，如要查找ls=2, 却找到cols=2去了
		//或者出现x=2 时找到xail=2去了
		if( pp[-1] == ' ' && pp[strlen( key)] == '=')
			break;
		else
			pp ++;
	}
	pp += strlen( key);
	//去除空格
	while(1)
	{
		if( *pp == ' ')
			pp++;
		else
			break;
		
	}
	if( *pp != '=')
		return 0;
	pp ++;
	//去除空格
	while(1)
	{
		if( *pp == ' ')
			pp++;
		else
			break;
		
	}
	i = 0;
	while(1)
	{
		val[ i] = pp[i];
		i ++;
		if( pp[i] == ' ')
				break;
		if( i > ( size - 2))
			break;
		
	}
//	val[ i] = 0;
	return i;
}


ABS_CTOR( Glyph)
FUNCTION_SETTING( init, Init);
FUNCTION_SETTING( draw_self, Draw_self);
//FUNCTION_SETTING( setClu, SetClu);
//FUNCTION_SETTING( setBgC, SetBgC);
//FUNCTION_SETTING( setWidth, setWidth);
//FUNCTION_SETTING( setHeight, setHeight);
//FUNCTION_SETTING( clean, Clean);
//FUNCTION_SETTING( insert, Insert);
//FUNCTION_SETTING( draw, Draw);
FUNCTION_SETTING( vdraw, vDraw);

END_ABS_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{




static int Init( Glyph *self, dev_lcd *lcd)
{
	
//	self->myLcd = lcd;
//	self->font = DEF_FONT;
//	self->colour = DEF_COLOUR;
	return RET_OK;
}
//static int SetFont( Glyph *self, int font)
//{
//	if( CHECK_FONT(  font))
//	{
//		
//		return RET_OK;
//	}

//	return ERR_BAD_PARAMETER;
//	
//}
//static int SetClu( Glyph *self, int c)
//{

//	return RET_OK;
//	
//}



//static int SetBgC( Glyph *self, int c)
//{
////	self->disArg.bgC = c;
//	return RET_OK;
//	
//}

//static int setWidth( Glyph *self, uint16_t wd)
//{
//	
//	return RET_OK;
//	
//}
//static int setHeight( Glyph *self, uint16_t he)
//{
//	return RET_OK;
//	
//}


//static int Clean( Glyph *self)
//{
//	return RET_OK;;
//	
//}

//static void Insert( Glyph *self, void *context, int len)
//{
//	
//	
//}


//static void Draw( Glyph *self, dspContent_t *cnt, dspArea_t *area)
//{
//	
//	
//}

static void vDraw( Glyph *self, dspContent_t *cnt, vArea_t *area)
{
	
}

static void Draw_self( Glyph *self)
{
	
//	self->vdraw(self, &self->
}

//static int DrawArea( Glyph *self, short x1, short y1, short x2, short y2,  int len)
//{
//	return -1;
//	
//}







