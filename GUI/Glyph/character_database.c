
//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "character_database.h"
#include "dev_lcd.h"
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

/*
目前只支持8*16的英文和16*16的汉字


*/

//const uint8_t  chd_number_5_7[10][6] = { \
//	0x00,0x3E,0x51,0x49,0x45,0x3E,
//	0x00,0x00,0x42,0x7F,0x40,0x00,
//	0x00,0x42,0x61,0x51,0x49,0x46,
//	0x00,0x21,0x41,0x45,0x4B,0x31,
//	0x00,0x18,0x14,0x12,0x7F,0x10,
//	0x00,0x27,0x45,0x45,0x45,0x39,
//	0x00,0x3C,0x4A,0x49,0x49,0x30,
//	0x00,0x01,0x01,0x79,0x07,0x0,
//	0x00,0x36,0x49,0x49,0x49,0x36,
//	0x00,0x06,0x49,0x49,0x29,0x1E,
//};
#define ASCII_OFFSET	32		//下面的ASCII中把前32个不能显示的去掉了
const uint8_t chd_ascii_8_16[190][8] = {
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/

{0x00,0x00,0x70,0xF8,0xF8,0x70,0x00,0x00},
{0x00,0x00,0x00,0x0D,0x0D,0x00,0x00,0x00},/*"!",1*/

{0x00,0x38,0x38,0x00,0x00,0x38,0x38,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*""",2*/

{0x00,0x20,0xF8,0xF8,0x20,0xF8,0xF8,0x20},
{0x00,0x02,0x0F,0x0F,0x02,0x0F,0x0F,0x02},/*"#",3*/

{0x00,0x30,0x78,0xCE,0x8E,0x18,0x10,0x00},
{0x00,0x04,0x0C,0x38,0x39,0x0F,0x06,0x00},/*"$",4*/

{0x18,0x3C,0x24,0xBC,0xD8,0x60,0x30,0x00},
{0x00,0x06,0x03,0x0D,0x1E,0x12,0x1E,0x0C},/*"%",5*/

{0x00,0xB0,0xF8,0x48,0x78,0x30,0x00,0x00},
{0x00,0x07,0x0F,0x08,0x09,0x07,0x0F,0x09},/*"&",6*/

{0x00,0x00,0x00,0x38,0x38,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"'",7*/

{0x00,0x00,0xC0,0xF0,0x38,0x08,0x00,0x00},
{0x00,0x00,0x07,0x1F,0x38,0x20,0x00,0x00},/*"(",8*/

{0x00,0x00,0x08,0x38,0xF0,0xC0,0x00,0x00},
{0x00,0x00,0x20,0x38,0x1F,0x07,0x00,0x00},/*")",9*/

{0x00,0x80,0xA0,0xE0,0xC0,0xE0,0xA0,0x80},
{0x00,0x00,0x02,0x03,0x01,0x03,0x02,0x00},/*"*",10*/

{0x00,0x80,0x80,0xE0,0xE0,0x80,0x80,0x00},
{0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x00},/*"+",11*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x2C,0x3C,0x1C,0x00,0x00},/*",",12*/

{0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"-",13*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x0C,0x0C,0x0C,0x00,0x00},/*".",14*/

{0x00,0x00,0x00,0x80,0xE0,0x78,0x18,0x00},
{0x00,0x18,0x1E,0x07,0x01,0x00,0x00,0x00},/*"/",15*/

{0x00,0x00,0xF0,0xF8,0x08,0x68,0xF8,0xF0},
{0x00,0x00,0x07,0x0F,0x0B,0x08,0x0F,0x07},/*"0",16*/

{0x00,0x20,0x20,0x30,0xF8,0xF8,0x00,0x00},
{0x00,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00},/*"1",17*/

{0x00,0x30,0x38,0x08,0x88,0xF8,0x70,0x00},
{0x00,0x0C,0x0E,0x0B,0x09,0x08,0x08,0x00},/*"2",18*/

{0x00,0x30,0x38,0x88,0x88,0xF8,0x70,0x00},
{0x00,0x06,0x0E,0x08,0x08,0x0F,0x07,0x00},/*"3",19*/

{0x00,0x00,0xF8,0xF8,0x00,0xE0,0xE0,0x00},
{0x00,0x03,0x03,0x02,0x02,0x0F,0x0F,0x02},/*"4",20*/

{0x00,0xF8,0xF8,0x88,0x88,0x88,0x08,0x00},
{0x00,0x08,0x08,0x08,0x0C,0x07,0x03,0x00},/*"5",21*/

{0x00,0xC0,0xE0,0x78,0x58,0xC8,0x80,0x00},
{0x00,0x07,0x0F,0x08,0x08,0x0F,0x07,0x00},/*"6",22*/

{0x00,0x08,0x08,0x88,0xE8,0x78,0x18,0x00},
{0x00,0x00,0x0E,0x0F,0x01,0x00,0x00,0x00},/*"7",23*/

{0x00,0x70,0xF8,0xC8,0x88,0xF8,0x70,0x00},
{0x00,0x07,0x0F,0x08,0x09,0x0F,0x07,0x00},/*"8",24*/

{0x00,0xF0,0xF8,0x08,0x08,0xF8,0xF0,0x00},
{0x00,0x00,0x09,0x0D,0x0F,0x03,0x01,0x00},/*"9",25*/

{0x00,0x00,0x00,0x60,0x60,0x60,0x00,0x00},
{0x00,0x00,0x00,0x0C,0x0C,0x0C,0x00,0x00},/*":",26*/

{0x00,0x00,0x00,0x60,0x60,0x60,0x00,0x00},
{0x00,0x00,0x00,0x2C,0x3C,0x1C,0x00,0x00},/*";",27*/

{0x00,0x80,0xC0,0x60,0x30,0x18,0x08,0x00},
{0x00,0x00,0x01,0x03,0x06,0x0C,0x08,0x00},/*"<",28*/

{0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00},
{0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00},/*"=",29*/

{0x00,0x08,0x18,0x30,0x60,0xC0,0x80,0x00},
{0x00,0x08,0x0C,0x06,0x03,0x01,0x00,0x00},/*">",30*/

{0x00,0x30,0x38,0x88,0xC8,0x78,0x30,0x00},
{0x00,0x00,0x00,0x0D,0x0D,0x00,0x00,0x00},/*"?",31*/

{0xF0,0xF8,0x08,0x88,0xC8,0x48,0xF8,0xF0},
{0x07,0x0F,0x08,0x09,0x0B,0x0A,0x0B,0x0B},/*"@",32*/

{0x00,0xE0,0xF0,0x18,0x18,0xF0,0xE0,0x00},
{0x00,0x0F,0x0F,0x01,0x01,0x0F,0x0F,0x00},/*"A",33*/

{0x00,0xF8,0xF8,0x88,0x88,0xF8,0x70,0x00},
{0x00,0x0F,0x0F,0x08,0x08,0x0F,0x07,0x00},/*"B",34*/

{0x00,0xF0,0xF8,0x08,0x08,0x38,0x30,0x00},
{0x00,0x07,0x0F,0x08,0x08,0x0E,0x06,0x00},/*"C",35*/

{0x00,0xF8,0xF8,0x08,0x18,0xF0,0xE0,0x00},
{0x00,0x0F,0x0F,0x08,0x0C,0x07,0x03,0x00},/*"D",36*/

{0x00,0xF8,0xF8,0x88,0x88,0x88,0x08,0x00},
{0x00,0x0F,0x0F,0x08,0x08,0x08,0x08,0x00},/*"E",37*/

{0x00,0xF8,0xF8,0x88,0x88,0x88,0x08,0x00},
{0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00},/*"F",38*/

{0x00,0xF0,0xF8,0x08,0x08,0x38,0x30,0x00},
{0x00,0x07,0x0F,0x08,0x09,0x0F,0x0F,0x00},/*"G",39*/

{0x00,0xF8,0xF8,0x80,0x80,0xF8,0xF8,0x00},
{0x00,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00},/*"H",40*/

{0x00,0x00,0x08,0xF8,0xF8,0x08,0x00,0x00},
{0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,0x00},/*"I",41*/

{0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0x00},
{0x00,0x06,0x0E,0x08,0x08,0x0F,0x07,0x00},/*"J",42*/

{0x00,0xF8,0xF8,0x80,0xE0,0x78,0x18,0x00},
{0x00,0x0F,0x0F,0x00,0x03,0x0F,0x0C,0x00},/*"K",43*/

{0x00,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00},
{0x00,0x0F,0x0F,0x08,0x08,0x08,0x08,0x00},/*"L",44*/

{0x00,0xF8,0xF8,0x20,0xC0,0x20,0xF8,0xF8},
{0x00,0x0F,0x0F,0x00,0x01,0x00,0x0F,0x0F},/*"M",45*/

{0x00,0xF8,0xF8,0x60,0xC0,0x80,0xF8,0xF8},
{0x00,0x0F,0x0F,0x00,0x00,0x01,0x0F,0x0F},/*"N",46*/

{0x00,0xF0,0xF8,0x08,0x08,0xF8,0xF0,0x00},
{0x00,0x07,0x0F,0x08,0x08,0x0F,0x07,0x00},/*"O",47*/

{0x00,0xF8,0xF8,0x88,0x88,0xF8,0x70,0x00},
{0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00},/*"P",48*/

{0x00,0xF0,0xF8,0x08,0x08,0xF8,0xF0,0x00},
{0x00,0x07,0x0F,0x08,0x18,0x3F,0x27,0x00},/*"Q",49*/

{0x00,0xF8,0xF8,0x88,0x88,0xF8,0x70,0x00},
{0x00,0x0F,0x0F,0x00,0x01,0x0F,0x0E,0x00},/*"R",50*/

{0x00,0x30,0x78,0xC8,0x88,0x18,0x10,0x00},
{0x00,0x04,0x0C,0x08,0x09,0x0F,0x06,0x00},/*"S",51*/

{0x00,0x08,0x08,0xF8,0xF8,0x08,0x08,0x00},
{0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00},/*"T",52*/

{0x00,0xF8,0xF8,0x00,0x00,0xF8,0xF8,0x00},
{0x00,0x07,0x0F,0x08,0x08,0x0F,0x07,0x00},/*"U",53*/

{0x00,0xF8,0xF8,0x00,0x00,0xF8,0xF8,0x00},
{0x00,0x03,0x07,0x0C,0x0C,0x07,0x03,0x00},/*"V",54*/

{0x00,0xF8,0xF8,0x00,0xC0,0x00,0xF8,0xF8},
{0x00,0x01,0x0F,0x0E,0x01,0x0E,0x0F,0x01},/*"W",55*/

{0x00,0x18,0x38,0xE0,0xC0,0x38,0x18,0x00},
{0x00,0x0E,0x0F,0x00,0x01,0x0F,0x0E,0x00},/*"X",56*/

{0x00,0x78,0xF8,0x80,0x80,0xF8,0x78,0x00},
{0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00},/*"Y",57*/

{0x00,0x08,0x08,0x88,0xC8,0x78,0x38,0x00},
{0x00,0x0E,0x0F,0x09,0x08,0x08,0x08,0x00},/*"Z",58*/

{0x00,0x00,0xF8,0xF8,0x08,0x08,0x00,0x00},
{0x00,0x00,0x7F,0x7F,0x40,0x40,0x00,0x00},/*"[",59*/

{0x00,0x18,0x78,0xE0,0x80,0x00,0x00,0x00},
{0x00,0x00,0x00,0x01,0x07,0x1E,0x18,0x00},/*"\",60*/

{0x00,0x00,0x08,0x08,0xF8,0xF8,0x00,0x00},
{0x00,0x00,0x40,0x40,0x7F,0x7F,0x00,0x00},/*"]",61*/

{0x00,0x08,0x0C,0x06,0x06,0x0C,0x08,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"^",62*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40},/*"_",63*/

{0x00,0x00,0x02,0x06,0x0E,0x08,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"`",64*/

{0x00,0x00,0x20,0x20,0x20,0xE0,0xC0,0x00},
{0x00,0x06,0x0F,0x09,0x09,0x0F,0x0F,0x00},/*"a",65*/

{0x00,0xF8,0xF8,0x20,0x20,0xE0,0xC0,0x00},
{0x00,0x0F,0x0F,0x08,0x08,0x0F,0x07,0x00},/*"b",66*/

{0x00,0xC0,0xE0,0x20,0x20,0x60,0x40,0x00},
{0x00,0x07,0x0F,0x08,0x08,0x0C,0x04,0x00},/*"c",67*/

{0x00,0xC0,0xE0,0x20,0x20,0xF8,0xF8,0x00},
{0x00,0x07,0x0F,0x08,0x08,0x0F,0x0F,0x00},/*"d",68*/

{0x00,0xC0,0xE0,0x20,0x20,0xE0,0xC0,0x00},
{0x00,0x07,0x0F,0x09,0x09,0x09,0x01,0x00},/*"e",69*/

{0x00,0x80,0xF0,0xF8,0x88,0x88,0x88,0x00},
{0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x00},/*"f",70*/

{0x00,0xC0,0xE0,0x20,0x20,0xE0,0xE0,0x00},
{0x00,0x47,0x4F,0x48,0x48,0x7F,0x3F,0x00},/*"g",71*/

{0x00,0xF8,0xF8,0x20,0x20,0xE0,0xC0,0x00},
{0x00,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00},/*"h",72*/

{0x00,0x20,0x20,0xEC,0xEC,0x00,0x00,0x00},
{0x00,0x08,0x08,0x0F,0x0F,0x08,0x08,0x00},/*"i",73*/

{0x00,0x00,0x20,0x20,0xEC,0xEC,0x00,0x00},
{0x00,0x40,0x40,0x40,0x7F,0x3F,0x00,0x00},/*"j",74*/

{0x00,0xF8,0xF8,0x00,0x80,0xE0,0x60,0x00},
{0x00,0x0F,0x0F,0x01,0x03,0x0E,0x0C,0x00},/*"k",75*/

{0x00,0x08,0x08,0xF8,0xF8,0x00,0x00,0x00},
{0x00,0x08,0x08,0x0F,0x0F,0x08,0x08,0x00},/*"l",76*/

{0x00,0xE0,0xE0,0x20,0xE0,0x20,0xE0,0xC0},
{0x00,0x0F,0x0F,0x00,0x07,0x00,0x0F,0x0F},/*"m",77*/

{0x00,0xE0,0xE0,0x20,0x20,0xE0,0xC0,0x00},
{0x00,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00},/*"n",78*/

{0x00,0xC0,0xE0,0x20,0x20,0xE0,0xC0,0x00},
{0x00,0x07,0x0F,0x08,0x08,0x0F,0x07,0x00},/*"o",79*/

{0x00,0xE0,0xE0,0x20,0x20,0xE0,0xC0,0x00},
{0x00,0x7F,0x7F,0x08,0x08,0x0F,0x07,0x00},/*"p",80*/

{0x00,0xC0,0xE0,0x20,0x20,0xE0,0xE0,0x00},
{0x00,0x07,0x0F,0x08,0x08,0x7F,0x7F,0x00},/*"q",81*/

{0x00,0xE0,0xE0,0x80,0x40,0x60,0x60,0x00},
{0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00},/*"r",82*/

{0x00,0xC0,0xE0,0x20,0x20,0x20,0x20,0x00},
{0x00,0x08,0x09,0x09,0x09,0x0F,0x06,0x00},/*"s",83*/

{0x00,0x20,0xF8,0xF8,0x20,0x20,0x20,0x00},
{0x00,0x00,0x07,0x0F,0x08,0x08,0x08,0x00},/*"t",84*/

{0x00,0xE0,0xE0,0x00,0x00,0xE0,0xE0,0x00},
{0x00,0x07,0x0F,0x08,0x08,0x0F,0x0F,0x00},/*"u",85*/

{0x00,0xE0,0xE0,0x00,0x00,0xE0,0xE0,0x00},
{0x00,0x03,0x07,0x0C,0x0C,0x07,0x03,0x00},/*"v",86*/

{0x00,0xE0,0xE0,0x00,0xC0,0x00,0xE0,0xE0},
{0x00,0x03,0x0F,0x0C,0x03,0x0C,0x0F,0x03},/*"w",87*/

{0x00,0x60,0xE0,0x80,0x80,0xE0,0x60,0x00},
{0x00,0x0C,0x0E,0x03,0x03,0x0E,0x0C,0x00},/*"x",88*/

{0x00,0xE0,0xE0,0x00,0x00,0xE0,0xE0,0x00},
{0x40,0x47,0x4F,0x68,0x38,0x1F,0x07,0x00},/*"y",89*/

{0x00,0x20,0x20,0x20,0xA0,0xE0,0x60,0x00},
{0x00,0x0C,0x0E,0x0B,0x09,0x08,0x08,0x00},/*"z",90*/

{0x00,0x00,0x80,0xF0,0x78,0x08,0x00,0x00},
{0x00,0x01,0x03,0x1E,0x3C,0x20,0x00,0x00},/*"{",91*/

{0x00,0x00,0x00,0xF8,0xF8,0x00,0x00,0x00},
{0x00,0x00,0x00,0x7F,0x7F,0x00,0x00,0x00},/*"|",92*/

{0x00,0x00,0x08,0x78,0xF0,0x80,0x00,0x00},
{0x00,0x00,0x20,0x3C,0x1E,0x03,0x01,0x00},/*"}",93*/

{0x30,0x18,0x08,0x18,0x30,0x20,0x30,0x18},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"~",94*/

};

#define STR_CHINESE "主菜单查询采样设置粉尘时间隔次数启动当前延调零大气湿度压力\
工况标体积配仪器显示密码修改灰亮定测量限保护控制停电加热开关水汽不参与计算维护\
记录版本倍率差其他℃方式恒温日期流机分实手"


const uint8_t chd_chanese_16_16[][8] = {
	

	
{0x00,0x08,0x08,0x08,0x08,0x08,0x09,0xFA},
{0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
{0x40,0x40,0x41,0x41,0x41,0x41,0x41,0x7F},
{0x41,0x41,0x41,0x41,0x41,0x40,0x40,0x00},/*"?",0*/

{0x04,0x04,0x44,0xC4,0x4F,0x44,0x44,0xC4},
{0x24,0x24,0x2F,0xB4,0x24,0x04,0x04,0x00},
{0x40,0x44,0x24,0x24,0x15,0x0C,0x04,0xFE},
{0x04,0x0C,0x15,0x24,0x24,0x44,0x40,0x00},/*"?",1*/

{0x00,0x00,0xF8,0x49,0x4A,0x4C,0x48,0xF8},
{0x48,0x4C,0x4A,0x49,0xF8,0x00,0x00,0x00},
{0x10,0x10,0x13,0x12,0x12,0x12,0x12,0xFF},
{0x12,0x12,0x12,0x12,0x13,0x10,0x10,0x00},/*"?",2*/

{0x40,0x44,0x24,0xA4,0x94,0x8C,0x84,0xFF},
{0x84,0x8C,0x94,0xA4,0x24,0x44,0x40,0x00},
{0x40,0x40,0x40,0x5F,0x4A,0x4A,0x4A,0x4A},
{0x4A,0x4A,0x4A,0x5F,0x40,0x40,0x40,0x00},/*"?",3*/

{0x40,0x40,0x42,0xCC,0x00,0x20,0xD0,0x4C},
{0x4B,0x48,0xC8,0x08,0x08,0xF8,0x00,0x00},
{0x00,0x00,0x00,0x7F,0x20,0x10,0x1F,0x12},
{0x12,0x12,0x5F,0x80,0x40,0x3F,0x00,0x00},/*"?",4*/

{0x00,0x00,0x04,0x14,0x64,0x04,0x0C,0xB4},
{0x02,0x02,0x42,0x33,0x02,0x00,0x00,0x00},
{0x40,0x41,0x21,0x11,0x09,0x05,0x03,0xFF},
{0x03,0x05,0x09,0x11,0x21,0x41,0x40,0x00},/*"?",5*/

{0x10,0x10,0xD0,0xFF,0x90,0x00,0x10,0x91},
{0x96,0x90,0xF0,0x90,0x94,0x93,0x10,0x00},
{0x04,0x03,0x00,0xFF,0x00,0x01,0x04,0x04},
{0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x00},/*"?",6*/

{0x40,0x40,0x42,0xCC,0x00,0x40,0xA0,0x9E},
{0x82,0x82,0x82,0x9E,0xA0,0x20,0x20,0x00},
{0x00,0x00,0x00,0x3F,0x90,0x88,0x40,0x43},
{0x2C,0x10,0x28,0x46,0x41,0x80,0x80,0x00},/*"?",7*/

{0x00,0x17,0x15,0xD5,0x55,0x57,0x55,0x7D},
{0x55,0x57,0x55,0xD5,0x15,0x17,0x00,0x00},
{0x40,0x40,0x40,0x7F,0x55,0x55,0x55,0x55},
{0x55,0x55,0x55,0x7F,0x40,0x40,0x40,0x00},/*"?",8*/

{0x44,0x58,0xC0,0xFF,0x50,0x4C,0x80,0x40},
{0xB0,0x8E,0x80,0x8F,0xB0,0x40,0x80,0x00},
{0x08,0x06,0x01,0xFF,0x01,0x06,0x80,0x40},
{0x30,0x0F,0x40,0x80,0x7F,0x00,0x00,0x00},/*"?",9*/

{0x00,0x40,0x20,0x10,0x0C,0x00,0x00,0x7F},
{0x00,0x00,0x04,0x08,0x10,0x60,0x00,0x00},
{0x40,0x40,0x44,0x44,0x44,0x44,0x44,0x7F},
{0x44,0x44,0x44,0x44,0x44,0x40,0x40,0x00},/*"?",10*/

{0x00,0xFC,0x84,0x84,0x84,0xFC,0x00,0x10},
{0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x00},
{0x00,0x3F,0x10,0x10,0x10,0x3F,0x00,0x00},
{0x01,0x06,0x40,0x80,0x7F,0x00,0x00,0x00},/*"?",11*/

{0x00,0xF8,0x01,0x06,0x00,0xF0,0x12,0x12},
{0x12,0xF2,0x02,0x02,0x02,0xFE,0x00,0x00},
{0x00,0xFF,0x00,0x00,0x00,0x1F,0x11,0x11},
{0x11,0x1F,0x00,0x40,0x80,0x7F,0x00,0x00},/*"?",12*/

{0x00,0xFE,0x22,0x5A,0x86,0x00,0x02,0x7A},
{0x4A,0x4A,0x4A,0x4A,0x4A,0x7A,0x02,0x00},
{0x00,0xFF,0x04,0x08,0x07,0x00,0xFF,0x09},
{0x0B,0x0D,0x79,0x0D,0x4B,0x89,0x7F,0x00},/*"?",13*/

{0x00,0x02,0x0C,0x80,0x60,0x80,0x40,0x30},
{0x0F,0xC8,0x08,0x08,0x28,0x18,0x00,0x00},
{0x02,0x02,0x7E,0x01,0x80,0x40,0x20,0x18},
{0x06,0x01,0x06,0x18,0x20,0x40,0x80,0x00},/*"?",14*/

{0x90,0x52,0x34,0x10,0xFF,0x10,0x34,0x52},
{0x80,0x70,0x8F,0x08,0x08,0xF8,0x08,0x00},
{0x82,0x9A,0x56,0x63,0x22,0x52,0x8E,0x00},
{0x80,0x40,0x33,0x0C,0x33,0x40,0x80,0x00},/*"?",15*/

{0x00,0x00,0x00,0xFC,0x44,0x44,0x44,0x45},
{0x46,0x44,0x44,0x44,0x44,0x7C,0x00,0x00},
{0x40,0x20,0x18,0x07,0x00,0xFC,0x44,0x44},
{0x44,0x44,0x44,0x44,0x44,0xFC,0x00,0x00},/*"?",16*/

{0x40,0x44,0xC4,0x44,0x44,0x44,0x40,0x10},
{0x10,0xFF,0x10,0x10,0x10,0xF0,0x00,0x00},
{0x10,0x3C,0x13,0x10,0x14,0xB8,0x40,0x30},
{0x0E,0x01,0x40,0x80,0x40,0x3F,0x00,0x00},/*"?",17*/

{0x00,0x40,0x42,0x44,0x58,0x40,0x40,0x7F},
{0x40,0x40,0x50,0x48,0xC6,0x00,0x00,0x00},
{0x00,0x40,0x44,0x44,0x44,0x44,0x44,0x44},
{0x44,0x44,0x44,0x44,0xFF,0x00,0x00,0x00},/*"?",18*/

{0x08,0x08,0xE8,0x29,0x2E,0x28,0xE8,0x08},
{0x08,0xC8,0x0C,0x0B,0xE8,0x08,0x08,0x00},
{0x00,0x00,0xFF,0x09,0x49,0x89,0x7F,0x00},
{0x00,0x0F,0x40,0x80,0x7F,0x00,0x00,0x00},/*"?",19*/

{0x04,0x84,0xC4,0xB4,0x8C,0x00,0x00,0xE4},
{0x04,0x04,0xFE,0x42,0x43,0x42,0x00,0x00},
{0x80,0x44,0x28,0x18,0x27,0x20,0x40,0x4F},
{0x48,0x48,0x4F,0x48,0x48,0x48,0x40,0x00},/*"?",20*/

{0x40,0x42,0xCC,0x00,0x00,0xFE,0x82,0x92},
{0x92,0xFE,0x92,0x92,0x82,0xFE,0x00,0x00},
{0x00,0x00,0x3F,0x10,0x88,0x7F,0x00,0x1E},
{0x12,0x12,0x12,0x5E,0x80,0x7F,0x00,0x00},/*"?",21*/

{0x10,0x0C,0x05,0x55,0x55,0x55,0x85,0x7F},
{0x85,0x55,0x55,0x55,0x05,0x14,0x0C,0x00},
{0x04,0x04,0x02,0x0A,0x09,0x29,0x2A,0x4C},
{0x48,0xA9,0x19,0x02,0x02,0x04,0x04,0x00},/*"?",22*/

{0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xFF},
{0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00},
{0x80,0x80,0x40,0x20,0x10,0x0C,0x03,0x00},
{0x03,0x0C,0x10,0x20,0x40,0x80,0x80,0x00},/*"?",23*/

{0x20,0x10,0x4C,0x47,0x54,0x54,0x54,0x54},
{0x54,0x54,0x54,0xD4,0x04,0x04,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x0F,0x30,0x40,0xF0,0x00},/*"?",24*/

{0x10,0x60,0x02,0x8C,0x00,0xFE,0x92,0x92},
{0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},
{0x04,0x04,0x7E,0x01,0x44,0x48,0x50,0x7F},
{0x40,0x40,0x7F,0x50,0x48,0x44,0x40,0x00},/*"?",25*/

{0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25},
{0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00},
{0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25},
{0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00},/*"?",26*/

{0x00,0x00,0xFE,0x02,0x82,0x82,0x82,0x82},
{0xFA,0x82,0x82,0x82,0x82,0x82,0x02,0x00},
{0x80,0x60,0x1F,0x40,0x40,0x40,0x40,0x40},
{0x7F,0x40,0x40,0x44,0x58,0x40,0x40,0x00},/*"?",27*/

{0x00,0x10,0x10,0x10,0x10,0x10,0xFF,0x10},
{0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00},
{0x00,0x80,0x40,0x20,0x18,0x06,0x01,0x00},
{0x20,0x40,0x80,0x40,0x3F,0x00,0x00,0x00},/*"?",28*/

{0x00,0x04,0x04,0x04,0x04,0x04,0x04,0xFC},
{0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00},
{0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3F},
{0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00},/*"?",29*/

{0x00,0x02,0x0C,0xC0,0x00,0x7E,0x42,0xC2},
{0x42,0x42,0xC2,0x42,0x7E,0x00,0x00,0x00},
{0x02,0x02,0x7F,0x80,0x40,0x20,0x18,0x07},
{0x00,0x00,0x3F,0x40,0x40,0x40,0x70,0x00},/*"?",30*/

{0x10,0x10,0xD0,0xFF,0x90,0x10,0x20,0x22},
{0x22,0x22,0xE2,0x22,0x22,0x22,0x20,0x00},
{0x04,0x03,0x00,0xFF,0x00,0x13,0x0C,0x03},
{0x40,0x80,0x7F,0x00,0x01,0x06,0x18,0x00},/*"?",31*/

{0x00,0x80,0x60,0xF8,0x07,0x10,0x10,0x10},
{0xD0,0xFF,0xD0,0x10,0x10,0x10,0x00,0x00},
{0x01,0x00,0x00,0xFF,0x10,0x08,0x04,0x0B},
{0x08,0xFF,0x08,0x0B,0x04,0x08,0x10,0x00},/*"?",32*/

{0x20,0x24,0x24,0xA4,0xFE,0x23,0x22,0x20},
{0xFC,0x04,0x04,0x04,0x04,0xFC,0x00,0x00},
{0x10,0x08,0x06,0x01,0xFF,0x01,0x06,0x80},
{0x63,0x19,0x01,0x01,0x09,0x33,0xC0,0x00},/*"?",33*/

{0xF2,0x12,0xFE,0x12,0xFE,0x12,0xF2,0x00},
{0x84,0x84,0x84,0x84,0xFC,0x00,0x00,0x00},
{0xFF,0x4A,0x49,0x48,0x49,0x49,0xFF,0x00},
{0x3F,0x40,0x40,0x40,0x41,0x40,0x70,0x00},/*"?",34*/

{0x00,0x80,0x60,0xF8,0x07,0x00,0x1C,0xE0},
{0x01,0x06,0x00,0xE0,0x1E,0x00,0x00,0x00},
{0x01,0x00,0x00,0xFF,0x00,0x80,0x40,0x20},
{0x13,0x0C,0x13,0x20,0x40,0x80,0x80,0x00},/*"?",35*/

{0x80,0x80,0x9E,0x92,0x92,0x92,0x9E,0xE0},
{0x80,0x9E,0xB2,0xD2,0x92,0x9E,0x80,0x00},
{0x08,0x08,0xF4,0x94,0x92,0x92,0xF1,0x00},
{0x01,0xF2,0x92,0x94,0x94,0xF8,0x08,0x00},/*"?",36*/

{0x00,0x00,0x00,0xFE,0x92,0x92,0x92,0x92},
{0x92,0x92,0x92,0xFE,0x00,0x00,0x00,0x00},
{0x40,0x42,0x44,0x58,0x40,0x7F,0x40,0x40},
{0x40,0x7F,0x40,0x50,0x48,0x46,0x40,0x00},/*"?",37*/

{0x40,0x40,0x42,0x42,0x42,0x42,0x42,0xC2},
{0x42,0x42,0x42,0x42,0x42,0x40,0x40,0x00},
{0x20,0x10,0x08,0x06,0x00,0x40,0x80,0x7F},
{0x00,0x00,0x00,0x02,0x04,0x08,0x30,0x00},/*"?",38*/

{0x10,0x8C,0x44,0x04,0xE4,0x04,0x95,0xA6},
{0x44,0x24,0x14,0x84,0x44,0x94,0x0C,0x00},
{0x02,0x02,0x7A,0x41,0x41,0x43,0x42,0x7E},
{0x42,0x42,0x42,0x43,0xF8,0x00,0x00,0x00},/*"?",39*/

{0x04,0x84,0xE4,0x5C,0x44,0xC4,0x00,0x02},
{0xF2,0x82,0x82,0x82,0xFE,0x80,0x80,0x00},
{0x02,0x01,0x7F,0x10,0x10,0x3F,0x00,0x08},
{0x08,0x08,0x08,0x48,0x88,0x40,0x3F,0x00},/*"?",40*/

{0x40,0x20,0xF8,0x07,0xF0,0xA0,0x90,0x4C},
{0x57,0x24,0xA4,0x54,0x4C,0x80,0x80,0x00},
{0x00,0x00,0xFF,0x00,0x1F,0x80,0x92,0x52},
{0x49,0x29,0x24,0x12,0x08,0x00,0x00,0x00},/*"?",41*/

{0x04,0x84,0x84,0x84,0x84,0xFC,0x40,0x30},
{0xCC,0x0B,0x08,0x08,0xF8,0x08,0x08,0x00},
{0x00,0x7F,0x20,0x10,0x10,0x08,0x80,0x40},
{0x21,0x16,0x08,0x16,0x21,0x40,0x80,0x00},/*"?",42*/

{0x08,0x08,0x08,0x88,0x78,0x0F,0x88,0x08},
{0xE8,0x08,0x08,0x08,0x08,0xC8,0x08,0x00},
{0x10,0x88,0x86,0x41,0x44,0x22,0x11,0x0C},
{0x03,0x0C,0x10,0x22,0x41,0x80,0x80,0x00},/*"?",43*/

{0x00,0x04,0x04,0x74,0x54,0x54,0x55,0x56},
{0x54,0x54,0x54,0x74,0x04,0x04,0x00,0x00},
{0x84,0x83,0x41,0x21,0x1D,0x05,0x05,0x05},
{0x05,0x05,0x7D,0x81,0x81,0x85,0xE3,0x00},/*"?",44*/

{0x10,0x0C,0x44,0x44,0x44,0x44,0x45,0xC6},
{0x44,0x44,0x44,0x44,0x44,0x14,0x0C,0x00},
{0x80,0x40,0x20,0x1E,0x20,0x40,0x40,0x7F},
{0x44,0x44,0x44,0x44,0x44,0x40,0x40,0x00},/*"?",45*/

{0x10,0x60,0x02,0x8C,0x00,0xFE,0x02,0xF2},
{0x02,0xFE,0x00,0xF8,0x00,0xFF,0x00,0x00},
{0x04,0x04,0x7E,0x01,0x80,0x47,0x30,0x0F},
{0x10,0x27,0x00,0x47,0x80,0x7F,0x00,0x00},/*"?",46*/

{0x20,0x20,0x20,0xBE,0xAA,0xAA,0xAA,0xAA},
{0xAA,0xAA,0xAA,0xBE,0x20,0x20,0x20,0x00},
{0x00,0x80,0x80,0xAF,0xAA,0xAA,0xAA,0xFF},
{0xAA,0xAA,0xAA,0xAF,0x80,0x80,0x00,0x00},/*"?",47*/

{0x00,0xFE,0x22,0x5A,0x86,0x00,0xFE,0x92},
{0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},
{0x00,0xFF,0x04,0x08,0x07,0x00,0xFF,0x40},
{0x20,0x03,0x0C,0x14,0x22,0x41,0x40,0x00},/*"?",48*/

{0x00,0x80,0x60,0xF8,0x07,0x00,0x3E,0x22},
{0x22,0xE2,0x22,0x22,0x3E,0x00,0x00,0x00},
{0x01,0x00,0x00,0xFF,0x20,0x11,0x09,0x05},
{0x03,0xFF,0x03,0x05,0x09,0x11,0x20,0x00},/*"?",49*/

{0x10,0x10,0x10,0xFF,0x10,0x90,0x00,0xF8},
{0x88,0x89,0x8E,0x88,0x88,0x88,0xF8,0x00},
{0x04,0x44,0x82,0x7F,0x81,0x40,0x30,0x0F},
{0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00},/*"?",50*/

{0x10,0x10,0x10,0xFF,0x90,0x20,0x98,0x48},
{0x28,0x09,0x0E,0x28,0x48,0xA8,0x18,0x00},
{0x02,0x42,0x81,0x7F,0x00,0x40,0x40,0x42},
{0x42,0x42,0x7E,0x42,0x42,0x42,0x40,0x00},/*"?",51*/

{0x40,0x50,0x4E,0x48,0x48,0xFF,0x48,0x48},
{0x48,0x40,0xF8,0x00,0x00,0xFF,0x00,0x00},
{0x00,0x00,0x3E,0x02,0x02,0xFF,0x12,0x22},
{0x1E,0x00,0x0F,0x40,0x80,0x7F,0x00,0x00},/*"?",52*/

{0x80,0x60,0xF8,0x07,0x00,0x04,0x74,0x54},
{0x55,0x56,0x54,0x54,0x74,0x04,0x00,0x00},
{0x00,0x00,0xFF,0x00,0x03,0x01,0x05,0x45},
{0x85,0x7D,0x05,0x05,0x05,0x01,0x03,0x00},/*"?",53*/

{0x00,0x00,0xF8,0x88,0x88,0x88,0x88,0xFF},
{0x88,0x88,0x88,0x88,0xF8,0x00,0x00,0x00},
{0x00,0x00,0x1F,0x08,0x08,0x08,0x08,0x7F},
{0x88,0x88,0x88,0x88,0x9F,0x80,0xF0,0x00},/*"?",54*/

{0x10,0x10,0x10,0xFF,0x10,0x10,0xF0,0x00},
{0x00,0xF8,0x08,0x08,0x08,0xF8,0x00,0x00},
{0x80,0x40,0x30,0x0F,0x40,0x80,0x7F,0x00},
{0x00,0x7F,0x20,0x20,0x20,0x7F,0x00,0x00},/*"?",55*/

{0x08,0x08,0x88,0xFF,0x48,0x48,0x00,0x08},
{0x48,0xFF,0x08,0x08,0xF8,0x00,0x00,0x00},
{0x81,0x65,0x08,0x07,0x20,0xC0,0x08,0x04},
{0x23,0xC0,0x03,0x00,0x23,0xC4,0x0F,0x00},/*"?",56*/

{0x80,0x82,0x82,0x82,0xFE,0x82,0x82,0x82},
{0x82,0x82,0xFE,0x82,0x82,0x82,0x80,0x00},
{0x00,0x80,0x40,0x30,0x0F,0x00,0x00,0x00},
{0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00},/*"?",57*/

{0x00,0x00,0x10,0x11,0x16,0x10,0x10,0xF0},
{0x10,0x10,0x14,0x13,0x10,0x00,0x00,0x00},
{0x81,0x81,0x41,0x41,0x21,0x11,0x0D,0x03},
{0x0D,0x11,0x21,0x41,0x41,0x81,0x81,0x00},/*"?",58*/

{0x00,0x20,0x20,0x20,0xA0,0x60,0x00,0xFF},
{0x60,0x80,0x40,0x20,0x18,0x00,0x00,0x00},
{0x20,0x10,0x08,0x06,0x01,0x40,0x80,0x7F},
{0x00,0x01,0x02,0x04,0x08,0x10,0x10,0x00},/*"?",59*/

{0x10,0x60,0x02,0x0C,0xC0,0x10,0x08,0x27},
{0x24,0x24,0x24,0x24,0x24,0x04,0x00,0x00},
{0x04,0x04,0x7C,0x03,0x00,0x00,0x01,0x01},
{0x01,0x01,0x01,0x01,0x3F,0x40,0xF0,0x00},/*"?",60*/

{0x00,0x02,0x02,0x02,0x02,0x82,0x42,0xF2},
{0x0E,0x42,0x82,0x02,0x02,0x02,0x00,0x00},
{0x10,0x08,0x04,0x02,0x01,0x00,0x00,0xFF},
{0x00,0x00,0x00,0x01,0x02,0x0C,0x00,0x00},/*"?",61*/

{0x00,0x20,0x20,0xA8,0x6C,0x2A,0x39,0x28},
{0xA8,0x2A,0x6C,0xA8,0x20,0x20,0x00,0x00},
{0x02,0x82,0x81,0x90,0x92,0x4A,0x49,0x45},
{0x24,0x22,0x10,0x08,0x01,0x02,0x02,0x00},/*"?",62*/

{0x00,0x00,0xE0,0x9F,0x88,0x88,0x88,0x88},
{0x88,0x88,0x88,0x88,0x88,0x08,0x00,0x00},
{0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08},
{0x08,0x48,0x80,0x40,0x3F,0x00,0x00,0x00},/*"?",63*/

{0x40,0x40,0x42,0xCC,0x00,0x40,0x40,0x40},
{0x40,0xFF,0x40,0x40,0x40,0x40,0x40,0x00},
{0x00,0x00,0x00,0x7F,0x20,0x10,0x00,0x00},
{0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},/*"?",64*/

{0x08,0x04,0xF3,0x52,0x56,0x52,0x52,0x50},
{0x54,0x53,0x52,0x56,0xF2,0x02,0x02,0x00},
{0x10,0x10,0x97,0x55,0x3D,0x15,0x15,0x15},
{0x15,0x15,0xFD,0x15,0x17,0x10,0x10,0x00},/*"?",65*/

{0x20,0x30,0xAC,0x63,0x20,0x58,0x20,0xF8},
{0x4F,0x48,0x49,0xFA,0x48,0x48,0x08,0x00},
{0x22,0x67,0x22,0x12,0x12,0x12,0x00,0xFF},
{0x22,0x22,0x22,0x3F,0x22,0x22,0x20,0x00},/*"?",66*/

{0x10,0x10,0x10,0xFF,0x10,0x90,0x00,0xF8},
{0x88,0x89,0x8E,0x88,0x88,0x88,0xF8,0x00},
{0x04,0x44,0x82,0x7F,0x81,0x40,0x30,0x0F},
{0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00},/*"?",67*/

{0x40,0x40,0x42,0xCC,0x00,0x00,0x00,0x84},
{0x84,0x84,0x84,0x84,0xFC,0x00,0x00,0x00},
{0x00,0x00,0x00,0x7F,0x20,0x10,0x00,0x3F},
{0x40,0x40,0x40,0x40,0x41,0x40,0x70,0x00},/*"?",68*/

{0x80,0x80,0x82,0x92,0x92,0x92,0x92,0x92},
{0x92,0x92,0x92,0xFE,0x80,0x80,0x80,0x00},
{0x40,0x40,0x22,0x24,0x10,0x48,0x80,0x7F},
{0x04,0x08,0x18,0x24,0x22,0x40,0x40,0x00},/*"?",69*/

{0x00,0xFE,0x20,0x20,0x3F,0x20,0x00,0xFC},
{0x24,0xE4,0x24,0x22,0x23,0xE2,0x00,0x00},
{0x80,0x7F,0x01,0x01,0xFF,0x80,0x60,0x1F},
{0x80,0x41,0x26,0x18,0x26,0x41,0x80,0x00},/*"?",70*/

{0x00,0x10,0x10,0x10,0x10,0xD0,0x30,0xFF},
{0x30,0xD0,0x10,0x10,0x10,0x10,0x00,0x00},
{0x10,0x08,0x04,0x02,0x09,0x08,0x08,0xFF},
{0x08,0x08,0x09,0x02,0x04,0x08,0x10,0x00},/*"?",71*/

{0x00,0x80,0x60,0xF8,0x07,0x80,0x94,0xE4},
{0x85,0x86,0x84,0xC4,0xB4,0x84,0x80,0x00},
{0x01,0x00,0x00,0xFF,0x00,0x00,0xFC,0x44},
{0x44,0x44,0x44,0x44,0x44,0xFC,0x00,0x00},/*"?",72*/

{0x00,0x14,0xA4,0x44,0x24,0x34,0xAD,0x66},
{0x24,0x94,0x04,0x44,0xA4,0x14,0x00,0x00},
{0x08,0x09,0x08,0x08,0x09,0x09,0x09,0xFD},
{0x09,0x09,0x0B,0x08,0x08,0x09,0x08,0x00},/*"?",73*/

{0x00,0x04,0x24,0x24,0x25,0x26,0xE4,0x3C},
{0x24,0x26,0x25,0x24,0x24,0x04,0x00,0x00},
{0x41,0x21,0x11,0x89,0x85,0x8B,0x89,0x89},
{0xF9,0x89,0x89,0x89,0x89,0x81,0x01,0x00},/*"?",74*/

{0x00,0x04,0x04,0x04,0xFF,0x24,0x24,0x24},
{0x24,0x24,0xFF,0x04,0x04,0x04,0x00,0x00},
{0x08,0x08,0x88,0x48,0x2F,0x09,0x09,0x09},
{0x09,0x09,0x2F,0x48,0x88,0x08,0x08,0x00},/*"?",75*/

{0x00,0x80,0x60,0xF8,0x87,0x80,0xFC,0x40},
{0x20,0xFF,0x10,0x08,0xF8,0x00,0x00,0x00},
{0x01,0x00,0x00,0xFF,0x00,0x00,0x3F,0x40},
{0x40,0x4F,0x40,0x44,0x47,0x40,0x78,0x00},/*"?",76*/

{0x06,0x09,0x09,0xE6,0xF8,0x0C,0x04,0x02},
{0x02,0x02,0x02,0x02,0x04,0x1E,0x00,0x00},
{0x00,0x00,0x00,0x07,0x1F,0x30,0x20,0x40},
{0x40,0x40,0x40,0x40,0x20,0x10,0x00,0x00},/*"?",77*/


{0x08,0x08,0x08,0x08,0x08,0xF8,0x89,0x8E},
{0x88,0x88,0x88,0x88,0x08,0x08,0x08,0x00},
{0x00,0x80,0x40,0x20,0x18,0x07,0x00,0x00},
{0x40,0x80,0x40,0x3F,0x00,0x00,0x00,0x00},/*方",78*/

{0x10,0x10,0x90,0x90,0x90,0x90,0x90,0x10},
{0x10,0xFF,0x10,0x10,0x11,0x16,0x10,0x00},
{0x00,0x20,0x60,0x20,0x3F,0x10,0x10,0x10},
{0x00,0x03,0x0C,0x10,0x20,0x40,0xF8,0x00},/*式",79*/

{0x00,0xE0,0x00,0xFF,0x10,0x20,0x02,0xF2},
{0x92,0x92,0x92,0x92,0x92,0xF2,0x02,0x00},
{0x01,0x00,0x00,0xFF,0x00,0x00,0x40,0x4F},
{0x44,0x44,0x44,0x44,0x44,0x4F,0x40,0x00},/*"?",80*/

{0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92},
{0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},
{0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42},
{0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00},/*"?",81*/

{0x00,0x00,0x00,0xFE,0x82,0x82,0x82,0x82},
{0x82,0x82,0x82,0xFE,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0xFF,0x40,0x40,0x40,0x40},
{0x40,0x40,0x40,0xFF,0x00,0x00,0x00,0x00},/*"日",82*/

{0x00,0x04,0xFF,0x24,0x24,0x24,0xFF,0x04},
{0x00,0xFE,0x22,0x22,0x22,0xFE,0x00,0x00},
{0x88,0x48,0x2F,0x09,0x09,0x19,0xAF,0x48},
{0x30,0x0F,0x02,0x42,0x82,0x7F,0x00,0x00},/*"期",83*/

{0x10,0x60,0x02,0x8C,0x00,0x44,0x64,0x54},
{0x4D,0x46,0x44,0x54,0x64,0xC4,0x04,0x00},
{0x04,0x04,0x7E,0x01,0x80,0x40,0x3E,0x00},
{0x00,0xFE,0x00,0x00,0x7E,0x80,0xE0,0x00},/*"流",84*/

{0x10,0x10,0xD0,0xFF,0x90,0x10,0x00,0xFE},
{0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00},
{0x04,0x03,0x00,0xFF,0x00,0x83,0x60,0x1F},
{0x00,0x00,0x00,0x3F,0x40,0x40,0x78,0x00},/*"机",85*/

{0x80,0x40,0x20,0x90,0x88,0x86,0x80,0x80},
{0x80,0x83,0x8C,0x10,0x20,0x40,0x80,0x00},
{0x00,0x80,0x40,0x20,0x18,0x07,0x00,0x40},
{0x80,0x40,0x3F,0x00,0x00,0x00,0x00,0x00},/*"分",86*/

{0x10,0x0C,0x04,0x84,0x14,0x64,0x05,0x06},
{0xF4,0x04,0x04,0x04,0x04,0x14,0x0C,0x00},
{0x04,0x84,0x84,0x44,0x47,0x24,0x14,0x0C},
{0x07,0x0C,0x14,0x24,0x44,0x84,0x04,0x00},/*"实",87*/


{0x00,0x00,0x24,0x24,0x24,0x24,0x24,0xFC},
{0x22,0x22,0x22,0x23,0x22,0x00,0x00,0x00},
{0x02,0x02,0x02,0x02,0x02,0x42,0x82,0x7F},
{0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x00},/*"手",88*/
};

const uint8_t chd_unkow_chinese[][8] ={
	{0x00,0x00,0x38,0x34,0x02,0x82,0xC2,0x7C},
{0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x37,0x37,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"?",0*/
};
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



//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
/*

只支持8*16英文和16_16汉字

*/
int CHD_Get_code(char *buf, char *c, short buf_size, char font, char size)
{
	
	
	
	char		*p_chinese = STR_CHINESE;
	uint16_t 	offset;
	uint16_t		count;
	
	if(IS_CHINESE( *c) == 0)
	{
		offset = *c - ASCII_OFFSET;
		offset *= 2;
		memcpy(buf, &chd_ascii_8_16[offset][0], 16);
		return 16;
		
	}
	
	//鏄剧ず姹夊瓧
	count = 0;
	//閫氳繃瀵规瘮unicode缂栫爜鏉ョ‘瀹氬亸绉讳綅缃�
	while(p_chinese[count])
	{
		//鍏堟瘮杈冪涓�涓紪鐮�
		if(c[0] != p_chinese[count])
		{
			count ++;
			continue;
			
		}
		
		//鍐嶆瘮杈冪浜屼釜缂栫爜锛屼袱涓紪鐮侀兘涓�鏍锋椂锛屾墠鍖归
		count ++;
		if(c[1] != p_chinese[count])
		{
			count ++;
			continue;
			
		}
		offset = count / 2;		//确定位置,一个汉字有2个编码
		offset *= 4;			//长度偏移
		memcpy(buf, &chd_chanese_16_16[offset][0], 32);
		return 32;
		
	}
	//找不到字模，显示问号
	
	memcpy(buf, &chd_unkow_chinese[0][0], 32);
	return 32;
	
	
	
	
	
}
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{
                                                                


