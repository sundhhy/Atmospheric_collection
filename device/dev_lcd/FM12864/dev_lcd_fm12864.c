#include "device.h"
#include <stdint.h>
#include <string.h>

#include "os/os_depend.h"
#include "basis/sdhDebug.h"
#include "sdhDef.h"
#include "basis/sdhError.h"

#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "system.h"   

#include "configs/lcd_conf.h"
#include "dev_lcd_fm12864.h"

#include "lcd_hardware_interfaces.h"
#include "character_database.h"
//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

/*
FM12864的缓存排列：

  y0 -----左半屏----- y63 | y0 -------右半屏------ y63
x0
|
|
|
|
x7

由于这种排列形式与我认为的横向为x轴，纵向为y轴的一般情况不一样，所以我在
本驱动中FM_Coordinate_converter对接口中的x,y进行了转换。
这样外部接口在使用的时候，依然把x作为横轴，y作为纵轴

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
#define FM_VRAM(x, y) fm_vram[(x) >> 3][y]

#define FM12864_WIDE			64
#define FM12864_WIDE_8			8		//8 * 8 = 64
#define FM12864_HALF_LONG		64
#define FM12864_LONG				128

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------
typedef void (*fm_draw_geo)(char attr, char clr, short vx0, short vy0, short vx1, short vy1);


//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------

//x是行坐标，y是列坐标
static uint8_t fm_vram[FM12864_WIDE_8][FM12864_LONG];
static fm_draw_geo arr_func_draw_geo[GEO_NUM];

struct {
	short chg_x0, chg_y0;
	short chg_x1, chg_y1;
}fm_vram_mgr;

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static	int FM_Init(void);
static	void FM_Clear(int c);
static	void FM_Switch(int on_off);
static	void FM_Text(char m, char *str,  int len, int x, int y, int font, char c);
static	void FM_Flush(char all);
static	void FM_Lightness(uint16_t		pct);
static	void FM_Contrast(uint16_t		pct);

static  int	 FM_Get_size(int font, uint16_t *width, uint16_t *heigh);
static	void FM_Set_backcolor( char c);
static  int  FM_Lcd_ctl(int cmd, ...);
static void FM_Draw_geometry(char type_g, char attr, char clr, short x0, short y0, short x1, short y1);

static void FM_Draw_line(char attr, char clr, short vx0, short vy0, short vx1, short vy1);
static void FM_Draw_rectangle(char attr, char clr, short vx0, short vy0, short vx1, short vy1);
static void FM_Draw_cycle(char attr, char clr, short vx0, short vy0, short vx1, short vy1);

static void FM_Coordinate_converter(short in_x, short in_y, short *o_vx, short *o_vy);
static void FM_Set_vxy(short vx, short vy);
static void FM_Init_change_area(void);
static void FM_Update_change_area(short chg_vx, short chg_vy);


//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//




CTOR(lcd_fm12864)
SUPER_CTOR(dev_lcd);
FUNCTION_SETTING(dev_lcd.init, FM_Init);
FUNCTION_SETTING(dev_lcd.clear, FM_Clear);
FUNCTION_SETTING(dev_lcd.display_control_switch, FM_Switch);
FUNCTION_SETTING(dev_lcd.dispaly_text, FM_Text);
FUNCTION_SETTING(dev_lcd.lcd_flush, FM_Flush);
FUNCTION_SETTING(dev_lcd.lcd_lightness, FM_Lightness);
FUNCTION_SETTING(dev_lcd.lcd_contrast, FM_Contrast);

FUNCTION_SETTING(dev_lcd.lcd_ctl, FM_Lcd_ctl);
FUNCTION_SETTING(dev_lcd.get_size, FM_Get_size);
FUNCTION_SETTING(dev_lcd.set_backcolor, FM_Set_backcolor);
FUNCTION_SETTING(dev_lcd.draw_geometry, FM_Draw_geometry);

END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{
static	int FM_Init(void)
{
//	uint8_t		s = 0;
	
	
	arr_func_draw_geo[GEO_LINE] = FM_Draw_line;
	arr_func_draw_geo[GEO_RECTANGLE] = FM_Draw_rectangle;
	arr_func_draw_geo[GEO_CYCLE] = FM_Draw_cycle;
	
	LHI_SELECT_NONE;
	
	LHI_Init_pwm(LCD_PWM_CONTRAST, 1, DEFAULT_CONTRAST);
	LHI_Init_pwm(LCD_PWM_LIGHTNESS, 3, DEFAULT_LIGHTNESS);
	LHI_Reset_lcd();
	
	LHI_SELECT_LEFT;
//	s = LHI_Read_status();

	
	
	LHI_Send_cmd(FM_CMD_UNLOCK);
	
	
//	s = LHI_Read_status();
//	if((s & FM_STATUS_UNLOCK) == 0)
//		goto err;
	
	LHI_Send_cmd(FM_CMD_START_LINE(0));
	
	
	
//	s = 0;
	LHI_SELECT_RIGHT;
	LHI_Send_cmd(FM_CMD_UNLOCK);
	LHI_Send_cmd(FM_CMD_START_LINE(0));
//	s = LHI_Read_status();
//	if((s & FM_STATUS_UNLOCK) == 0)
//		goto err;
	LHI_SELECT_NONE;
//	LHI_LCD_ON;
	return RET_OK;
//err:
//	return RET_FAILED;
}
static	void FM_Clear(int c)
{
	short i;
	uint8_t  buf[64] = {0xff};

	
	if(c == PALLET_BLACK)
	{
		memset(buf, 0xff, 64);
		memset(fm_vram, 0xff, sizeof(fm_vram));
	}
	else
	{
		memset(buf, 0, 64);
		memset(fm_vram, 0, sizeof(fm_vram));	
	}
	LHI_SELECT_LEFT;
	
	for(i = 0; i < FM12864_WIDE_8; i++)
	{
		
		LHI_Send_cmd(FM_CMD_SET_VX(i));
		LHI_Send_cmd(FM_CMD_SET_VY(0));
		LHI_Write_vram(buf, 64);
			
	}
	
	LHI_SELECT_RIGHT;

	for(i = 0; i < FM12864_WIDE_8; i++)
	{
		
		LHI_Send_cmd(FM_CMD_SET_VX(i));
		LHI_Send_cmd(FM_CMD_SET_VY(0));
		LHI_Write_vram(buf, 64);
			
	}
	
	FM_Init_change_area();
	
	LHI_SELECT_NONE;
}
	

static	void FM_Switch(int on_off)
{
//	if(on_off)
//	{
////		LHI_LCD_ON;
//		FM_Lightness(on_off);
//	}
//	else
//	{
//		FM_Lightness(0);
////		LHI_LCD_OFF;
//		
//	}
	
	
}
static	void FM_Text(char m, char *str,  int len, int x, int y, int font, char c)
{
	char	code[32];
	short	code_len;
	short	half_mask;
	short 	vx, vy;
	short		i, j;
	FM_Coordinate_converter(x, y, &vx, &vy);
	if(len == 0)
		len = strlen(str);
//	FM_Set_vxy(vx, vy);
	while(*str && len)
	{
		code_len = CHD_Get_code(code, str, 32, 0, 0);
		if(IS_CHINESE(str[0]))
		{
			str += 2;
			len -= 2;
		}
		else
		{
			str ++;
			len --;
		}
		
		j = vx;
		if(code_len <= 0)
			continue;
		//这个算法只能用于2的幂的长度，如果要支持8*16 16*16以外的字体，要换算法
		half_mask = (code_len >> 1) - 1;
		for(i = 0; i < code_len; i++)
		{
			if(((i & half_mask) == 0) && ( i > 0))
			{
				j += 8;
				vy -= code_len >> 1;
				
				if(j > FM12864_WIDE)
					break;
				
			}
			
			if(vy >= FM12864_LONG)
				break;
			
			//todo: 反显的判断机制不是很好，但是目前够用了
			if(c == PALLET_WHITE)
				FM_VRAM(j, vy) = ~code[i];
			else
				FM_VRAM(j, vy) = code[i];
			FM_Update_change_area(j, vy);
//			LHI_Write_vram(&FM_VRAM(vx, vy), 1);
			vy ++;
			
			
			
		}
		
		
		
	}
	
	
}



static void FM_Draw_geometry(char type_g, char attr, char clr, short x0, short y0, short x1, short y1)
{
	short vx0, vy0, vx1, vy1;
	
	if(type_g >= GEO_NUM)
		return;
	if(y1 >= LCD_WIDE)
		return;
	if(y0 >= LCD_WIDE)
		return;
	if(x1 >= LCD_LONG)
		return;
	if(x0 >= LCD_LONG)
		return;
	
	FM_Coordinate_converter(x0, y0, &vx0, &vy0);
	FM_Coordinate_converter(x1, y1, &vx1, &vy1);
	arr_func_draw_geo[type_g](attr, clr, vx0, vy0, vx1, vy1);
//	short vx0, vy0, vx1, vy1;
//	short x, y;
//	uint8_t d = 0xff;
//	FM_Set_vxy(0, 0);
//	LHI_Write_vram(&d, 1);
//	FM_Set_vxy(63, 127);
//	LHI_Write_vram(&d, 1);
//	FM_Coordinate_converter(x0, y0, &vx0, &vy0);
//	FM_Coordinate_converter(x1, y1, &vx1, &vy1);
//	for(x = vx0; x < vx1; x++)
//	{
//		for(y = vy0;y < vy1; y++)
//		{
//			
//			
//		}
//		
//	}
	
	
}
	
static	void FM_Flush(char all)
{
	short i;
	short limit_y;
	if(all)
	{


		fm_vram_mgr.chg_x0 = 0;
		fm_vram_mgr.chg_y0 = 0;
		
		fm_vram_mgr.chg_x1 = FM12864_WIDE_8;
		fm_vram_mgr.chg_y1 = FM12864_LONG;
		
	}
	
	//因为LCD是分左右两半，因此为了方便操作，也先刷左屏，再刷右屏
	
	if(fm_vram_mgr.chg_y0 > FM12864_HALF_LONG)
		goto flush_right;
	if(fm_vram_mgr.chg_y1 < FM12864_HALF_LONG)
		limit_y = fm_vram_mgr.chg_y1 ;
	else
		limit_y = FM12864_HALF_LONG - 1;
	
	//刷左半屏
	for(i = fm_vram_mgr.chg_x0; i <= fm_vram_mgr.chg_x1; i ++)
	{
		FM_Set_vxy(i, fm_vram_mgr.chg_y0);		//y轴方向只需设置一次就够了，LCD内部会自动加1
//		for(j = fm_vram_mgr.chg_y0;j <= limit_y;j ++)
//		{
//			
//			LHI_Write_vram(&FM_VRAM(i, j), 1);
//			
//		}
		LHI_Write_vram(&fm_vram[i][fm_vram_mgr.chg_y0], limit_y - fm_vram_mgr.chg_y0 + 1);
		
	}
	
	flush_right:
	if(fm_vram_mgr.chg_y1 < FM12864_HALF_LONG)
		goto exit;
	
	if(fm_vram_mgr.chg_y0 > FM12864_HALF_LONG)
		limit_y = fm_vram_mgr.chg_y0 ;
	else
		limit_y = FM12864_HALF_LONG;
	
	//刷右半屏
	for(i = fm_vram_mgr.chg_x0; i <= fm_vram_mgr.chg_x1; i ++)
	{
		FM_Set_vxy(i, limit_y);
//		for(j = limit_y;j <= fm_vram_mgr.chg_y1;j ++)
//		{
//			
//			LHI_Write_vram(&FM_VRAM(i, j), 1);
//			
//		}
		LHI_Write_vram(&fm_vram[i][limit_y], fm_vram_mgr.chg_y1 - limit_y + 1);
	}
	exit:
	FM_Init_change_area();
}

static void FM_Init_change_area(void)
{
	fm_vram_mgr.chg_x0 = FM12864_WIDE_8;
	fm_vram_mgr.chg_y0 = FM12864_LONG;

	fm_vram_mgr.chg_x1 = 0;
	fm_vram_mgr.chg_y1 = 0;
	
}

static void FM_Update_change_area(short chg_vx, short chg_vy)
{
	chg_vx >>= 3;
	if(fm_vram_mgr.chg_x0 > chg_vx)
		fm_vram_mgr.chg_x0 = chg_vx;
	if(fm_vram_mgr.chg_y0 > chg_vy)
		fm_vram_mgr.chg_y0 = chg_vy;
	
	if(fm_vram_mgr.chg_x1 < chg_vx)
		fm_vram_mgr.chg_x1 = chg_vx;
	if(fm_vram_mgr.chg_y1 < chg_vy)
		fm_vram_mgr.chg_y1 = chg_vy;
	
	
}
static	void FM_Lightness(uint16_t		pct)
{
	if(pct > 1000)
		pct = 1000;
	LHI_Set_pwm_duty(LCD_PWM_LIGHTNESS, pct);
}

static	void FM_Contrast(uint16_t		pct)
{
	if(pct > 1000)
		pct = 1000;
	LHI_Set_pwm_duty(LCD_PWM_CONTRAST, pct);
}

static  int	 FM_Get_size(int font, uint16_t *width, uint16_t *heigh)
{
	*width = 8;
	*heigh = 16;
	return RET_OK;
}


static	void FM_Set_backcolor( char c)
{

}

static int  FM_Lcd_ctl(int cmd, ...)
{
	
	
	return 0;
}

//根据x，计算字节内的Bit
static char FM_Cal_bit(short vx0)
{
	return vx0 % 8;
	
}




static void FM_Draw_line(char attr, char clr, short vx0, short vy0, short vx1, short vy1)
{
	int i; 
	int dx, dy;	
	int	len;
	int x, y;
//	uint8_t  *p_vram;
	uint8_t bit = 0;
//	uint8_t val;

//	if(vy0 == vy1)
//	{
//		//单色
//		if(clr == PALLET_BLACK)
//		
//			val = 0xff;
//			
//		
//		else
//			val = 0;
//		for(i = vx0; i < vx1; i ++)
//		{
//			
//			FM_VRAM(i, vy0) = val;
//			FM_Set_vxy(i, vy0);
//			LHI_Write_vram(&FM_VRAM(i, vy0), 1);
//		}
//			
//		
//		return;
//		
//	}
	
	//单色
	if(clr == PALLET_BLACK)
		clr = 1;
	else
		clr = 0;
	if(vx0 == vx1)
	{
		bit = FM_Cal_bit(vx0);
		for(i = vy0; i <= vy1; i ++)
		{
			
			FM_VRAM(vx0, i) &= ~( 1 << bit);
			FM_VRAM(vx0, i) |= ( clr << bit);
			FM_Update_change_area(vx0, i);
//			FM_Set_vxy(vx0, i);
//			LHI_Write_vram(&FM_VRAM(vx0, i), 1);
		}
		
		return;
	}
	
	//坐标放到1024倍，再移动
	//参考自：30天自制操作系统的画线部分
	dy = vy1 - vy0;
	dx = vx1 - vx0;
	
	if(dx > dy)
	{
		len = dx;
		if(vx0 > vx1)
			dx = -1024;
		else
			dx = 1024;
		
		if(vy0 <= vy1)
			dy = ((vy1 - vy0 + 1) << 10) / len;
		else
			dy = ((vy1 - vy0 - 1) << 10) / len;
		
	}
	else
	{
		
		len = dy + 1;
		if(vy0 > vy1)
			dy = -1024;
		else
			dy = 1024;
		
		if(vx0 <= vx1)
			dx = ((vx1 - vx0 + 1) << 10) / len;
		else
			dx = ((vx1 - vx0 - 1) << 10) / len;
		
	}
	
	
//	if(dy > dx)
//	{
//		len = dy;
//		if(vy0 > vy1)
//			dy = -1024;
//		else
//			dy = 1024;
//		
//		if(vx0 <= vx1)
//			dx = ((vx1 - vx0 + 1) << 10) / len;
//		else
//			dx = ((vx1 - vx0 - 1) << 10) / len;
//		
//	}
//	else
//	{
//		
//		len = dx + 1;
//		if(vx0 > vx1)
//			dx = -1024;
//		else
//			dx = 1024;
//		
//		if(vy0 <= vy1)
//			dy = ((vy1 - vy0 + 1) << 10) / len;
//		else
//			dy = ((vy1 - vy0 - 1) << 10) / len;
//		
//	}
	
	x = vx0 << 10;
	y = vy0 << 10;
	for(i = 0; i < len; i ++)
	{
		
		
		bit = FM_Cal_bit(x >> 10);
		
		FM_VRAM(x >> 10, y >> 10) &= ~( 1 << bit);
		FM_VRAM(x >> 10, y >> 10) |= ( clr << bit);
		FM_Update_change_area(x >> 10, y >> 10);
//		FM_Set_vxy(x >> 10, y >> 10);
//		LHI_Write_vram(&FM_VRAM(x >> 10, y >> 10), 1);
		
		x += dx;
		y += dy;
		
		if((x < 0) || (y < 0))
			break;
		
	}
	
}
static void FM_Draw_rectangle(char attr, char clr, short vx0, short vy0, short vx1, short vy1)
{
	FM_Draw_line(ATT_NONE, clr, vx0, vy0, vx0, vy1);
	FM_Draw_line(ATT_NONE, clr, vx0, vy0, vx1, vy0);
	FM_Draw_line(ATT_NONE, clr, vx1, vy0, vx1, vy1);
	FM_Draw_line(ATT_NONE, clr, vx0, vy1, vx1, vy1);
	
}
static void FM_Draw_cycle(char attr, char clr, short vx0, short vy0, short vx1, short vy1)
{
	
	
}


static void FM_Coordinate_converter(short in_x, short in_y, short *o_vx, short *o_vy)
{
	*o_vy = in_x;
	*o_vx = in_y;
	
	
}

static void FM_Set_vxy(short vx, short vy)
{
	short x_addr;
	short y_addr;

	x_addr = vx;	   
	y_addr = vy%64;	 // 设置Y地址

	if(vy<64)
	{
		LHI_SELECT_LEFT;	     //Y小于64点亮左屏
	}
	else if(vy<128)
	{
		LHI_SELECT_RIGHT;	     //Y小于128点亮右屏
	}
	LHI_Wait(1000);
	LHI_Send_cmd(FM_CMD_SET_VX(x_addr));
	LHI_Wait(1000);
	LHI_Send_cmd(FM_CMD_SET_VY(y_addr));
	LHI_Wait(1000);
	
}


