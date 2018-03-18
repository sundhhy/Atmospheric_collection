//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef __INC_dev_lcd_H_
#define __INC_dev_lcd_H_
#include <stdint.h>
#include "lw_oopc.h"

//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define IS_CHINESE( c) ( c & 0x80)
#define IS_BR( c) ( c == '\n')
#define IS_TAB( c) ( c == '\r')


#define FONT_12			12
#define FONT_16			16
#define FONT_24			24
#define FONT_32			32
#define FONT_48			48
#define FONT_64			64
#define FONT_ERR		0xf

#define DEF_FONT		FONT_12

#define CHECK_FONT( font) (( font <= FONT_64) && ( font >= FONT_12))


#define COLOUR_RED			1
#define COLOUR_GREN			2
#define COLOUR_BLUE			3
#define COLOUR_YELLOW		4
#define COLOUR_BABYBLUE		5		//浅蓝色
#define COLOUR_GRAY			8
#define COLOUR_PURPLE		6	//紫色
#define COLOUR_WHITE		18
#define COLOUR_DEEPPURPLE	57
#define COLOUR_BLACK		63
#define COLOUR_OTHER(n)		n

#define DEF_COLOUR			COLOUR_WHITE
#define ERR_COLOUR			0

//对齐的值要保持线性，不要乱改
#define	ALIGN_LEFT					0		//左对齐
#define	ALIGN_MIDDLE				1		//居中对齐
#define	ALIGN_RIGHT					2		//右对齐对齐s
#define	ALIGN_MAX					3		//右对齐对齐s
#define ALIGN_ERR					0xff
#define ALIGN_DEFAULT				ALIGN_LEFT

#define SIZE_ERR					0xffff
#define SIZE_BOUNDARY					0xfffe

//#define EMPTY_RECTANGLE				0
//#define FILLED_RECTANGLE			1
//#define LINE						2

#define GEO_LINE					0
#define GEO_RECTANGLE				1
#define GEO_CYCLE					2
#define GEO_NUM						3

#define ATT_NONE				0
#define ATT_STRONG				1		//加粗
#define ATT_FULL_CORE			2		//实心,用于几何图形

#define CLR_BLACK				0
#define CLR_WHITE				1

//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------
//字符型设备的接口
//包括uart设备

typedef struct {
	int16_t		x1, y1;
	int16_t		x2, y2;
}scArea_t;


ABS_CLASS(dev_lcd)
{
	int (*init)( void);
	void (*clear)( int c);
	void (*display_control_switch)(int on_off);
	
	//font 高16位，字体编号，一般是0，除非有字体变化的需求，才会赋其他值; 低16位大小
	void (*dispaly_text)(char m, char *string,  int len, int x, int y, int font, char c);
	void (*draw_geometry)(char type_g, char attr, char clr, short x0, short y0, short x1, short y1);
	
	
	
	void (*lcd_flush)(void);
	void (*lcd_lightness)(uint8_t		pct);
	int (*lcd_ctl)(int cmd, ...);
//	int ( *label)( char *string,  int len, scArea_t *area, int font, char c, char ali);
//	void ( *BKColor)( char c);
//	int ( *Box)( int x1, int y1, int x2, int y2, char type, char c);
//	int ( *getStrSize)( int font, uint16_t *width, uint16_t *heigh);
//	void ( *getScrnSize)( uint16_t *xsize, uint16_t *ysize);
//	void ( *picture)( int x1, int y1, char num);
//	void ( *cutPicture)( short x1, short y1, short num, short px1, short py1, short w, short h);
//	void ( *bkPicture)( char m, int x1, int y1, char num);
//	void	(*done)(void);
//	void	(*icon)(int x1, int y1, char num, int xn, int yn, int n);
};
//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------
int Open_dev_lcd(int major, int minor, void **dev);



#endif
