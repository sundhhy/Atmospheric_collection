//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef __INC_lcd_conf_H_
#define __INC_lcd_conf_H_
//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

#define LCD_LONG				128
#define LCD_WIDE				64
#define LCD_PIX_BIT			1				//每个像素需要的缓存bit数
#define LCD_BUF_SIZE		(1024)		

#define DEFAULT_LIGHTNESS		60		//默认亮度



//调色板颜色定义， 与LCD无关
#define PALLET_BLACK				0
#define PALLET_WHITE				1

#define PALLET_CLEAR				0xfe		//透明色
#define DEF_COLOUR			PALLET_BLACK
#define ERR_COLOUR			PALLET_CLEAR
//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------

#endif
