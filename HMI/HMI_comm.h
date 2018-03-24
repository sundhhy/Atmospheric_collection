#ifndef _INC_commHMI_H_
#define _INC_commHMI_H_
#include "HMI.h"
#include "ModelFactory.h"
#include "sheet.h"
#include "system.h"
#include "hmi_conf.h"
#include "model_conf.h"
//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <stdint.h>
//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------




//条纹背景图片的属性,这是从背景图片中得到的数据
#define STRIPE_MAX_ROWS			11
#define STRIPE_VY0				34		//第一行宽2个像素点，就单独出来
#define STRIPE_VY1				50
#define STRIPE_SIZE_Y			16
#define STRIPE_CLR_1			PALLET_BLACK
#define STRIPE_CLR_2			PALLET_BLACK
#define STRIPE_CLR_FOCUSE		PALLET_BLACK

#define CMM_CHOICE_ID(n)			(n | SHT_ID_PRIVATE)
#define CMM_IS_LEGAL_CID(cid)			((cid >= SHT_ID_PRIVATE) && (cid < (SHT_ID_PRIVATE | 4)))
#define CMM_GET_CHOICE_NUM(cid)		(cid & 0xf)
 //------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------
CLASS( cmmHmi)
{
	Model			*p_mdlRtc;
//	shtctl			*p_shtctl;
//	sheet			*p_shtTime;
	EXTENDS( HMI);
	
	
	
};

//曲线管理器
typedef struct {
	uint8_t		num_points;
	uint8_t		current_idx;
	uint8_t		start_idx;
	uint8_t		flags;		//是否显示等属性
	uint8_t		colour;
	uint8_t		start_x;	
	uint8_t		direct;		//0:从左到右 1：从右到左
	uint8_t		step;		//每个坐标移动的距离
	uint8_t		points[CURVE_POINT];
}curve_ctl_t;
//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------
extern sheet			*arr_p_pool_shts[HMI_NUM_P_SHT];
extern sheet			*arr_p_sht_choices[HMI_NUM_P_CHOICE_SHT];
extern char 			arr_choice_key[HMI_NUM_P_CHOICE_SHT];
extern sheet			*g_p_shtTime;


//sheet
extern	sheet			*g_p_sht_bkpic, *g_p_sht_title, *g_p_text, *g_p_boxlist;
extern 	sheet			*g_p_cpic;

//extern 	sheet			*g_p_ico_memu, *g_p_ico_digital, *g_p_ico_bar,  *g_p_ico_trend, *g_p_cpic;
//extern 	sheet			*g_p_ico_pgup;
//extern 	sheet			*g_p_ico_pgdn;
//extern	sheet			*g_p_ico_eraseTool;
//extern	sheet			*g_p_ico_search;

extern  sheet			*g_arr_p_chnData[NUM_CHANNEL];
extern	sheet			*g_arr_p_chnUtil[NUM_CHANNEL];
extern	sheet			*g_arr_p_chnAlarm[NUM_CHANNEL];
extern	sheet  			*g_arr_p_check[NUM_CHANNEL]; 		//通道的勾选图标


//values
//extern 	keyboardHMI		*g_keyHmi;
extern 	ro_char 		news_cpic[];


//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------
cmmHmi *GetCmmHMI(void);

//extern void Build_ChnSheets(void);
extern int Stripe_clean_clr(int row);
extern int Stripe_vy(int row) ;


extern void Win_content(char *p_tips);
#endif
