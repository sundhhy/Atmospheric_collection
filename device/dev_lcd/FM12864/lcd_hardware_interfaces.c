//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
#include "stm32f10x.h"
#include "lcd_hardware_interfaces.h"
#include "os/os_depend.h"
#include "sdhDef.h"
//模拟并口通信功能和PWM可调输出功能


//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define PORT_IN		0x00
#define PORT_OUT	0x01


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
#define DIR_IN		0
#define DIR_OUT		1


#define PWM_100   999

#define SET_LCD_E		GPIO_SetBits(PORT_LCD_E,PIN_LCD_E)	
#define CLR_LCD_E		GPIO_ResetBits(PORT_LCD_E,PIN_LCD_E)	
#define SET_LCD_RST		GPIO_SetBits(PORT_LCD_RST,PIN_LCD_RST)	
#define CLR_LCD_RST		GPIO_ResetBits(PORT_LCD_RST,PIN_LCD_RST)
#define SET_LCD_RW		GPIO_SetBits(PORT_LCD_RW,PIN_LCD_RW)	
#define CLR_LCD_RW		GPIO_ResetBits(PORT_LCD_RW,PIN_LCD_RW)	
#define SET_LCD_DI		GPIO_SetBits(PORT_LCD_DI,PIN_LCD_DI)	
#define CLR_LCD_DI		GPIO_ResetBits(PORT_LCD_DI,PIN_LCD_DI)

#define PWM_FUNC(chx)		arr_pwm_func[chx - 1]
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------
typedef void (*init_oc1)(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
typedef void (*config_preload)(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);
typedef void (*set_compare)(TIM_TypeDef* TIMx, uint16_t Compare1);
typedef struct {
	init_oc1					init;
	config_preload		config;
	set_compare			compare;

}pwm_init_func;	
//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------

static  const pwm_init_func arr_pwm_func[4] = { 
		{
			TIM_OC1Init,
			TIM_OC1PreloadConfig,
			TIM_SetCompare1,
		}, 
		{
			TIM_OC2Init,
			TIM_OC2PreloadConfig,
			TIM_SetCompare2,
		}, 
		{
			TIM_OC3Init,
			TIM_OC3PreloadConfig,
			TIM_SetCompare3,
		}, 
		{
			TIM_OC4Init,
			TIM_OC4PreloadConfig,
			TIM_SetCompare4,
		}, 
	
	
};

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static int LHI_Pulse(int duty);
static void LHI_TX_byte(uint8_t b);
static uint8_t LHI_RX_byte(void);

static void LHI_Set_parallel_direction(int in_out);
static void LHI_Lcd_E_edge(char edge);		//edge 0 下降沿 1 上升沿
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

void LHI_Init_pwm(TIM_TypeDef* timx, short chx, short default_duty)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
		
	//初始化定时器
	//72 * 10^6 / (999 + 1) / (6 + 1) = 10k
	TIM_TimeBaseStructure.TIM_Period = PWM_100; //自动装载寄存器的值
	TIM_TimeBaseStructure.TIM_Prescaler = 6; //TIMX预分频的值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
	TIM_TimeBaseInit(timx, &TIM_TimeBaseStructure); 
	
	//设置时钟的通道1为PWM输出
	//选择哪路通道是通过TIM_OC1Init,TIM_OC1PreloadConfig, TIM_SetCompare1中的数字来决定的...
	//如果选择通道2,就调用TIM_OC2Init,TIM_OC2PreloadConfig, TIM_SetCompare2
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//计数器小于比较值的时候，输出无效电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//输出比较极性低
	
	PWM_FUNC(chx).init(timx, &TIM_OCInitStructure);//????????????
	PWM_FUNC(chx).config(timx, TIM_OCPreload_Enable);  //?????TIM2?CCR2??????
	TIM_Cmd(timx, ENABLE);
	PWM_FUNC(chx).compare(timx,LHI_Pulse(default_duty));//??????50%?pwm??
}

int  LHI_Wait(int us)
{
	uint8_t s;
	while(1)
	{
		s = LHI_Read_status();
		if((s & LCD_STATUS_BUSY) == 0)
			break;
	}
	
	return RET_OK;
	
}
void LHI_Set_pwm_duty(TIM_TypeDef* timx, short chx,  short duty)
{
	
	PWM_FUNC(chx).compare(timx,LHI_Pulse(duty));//??????50%?pwm??
}

void LHI_Reset_lcd(void)
{
	
	LHI_Lcd_E_edge(1);
	CLR_LCD_RST;
	delay_ms(30);
	SET_LCD_RST;
	delay_ms(30);
}


int LHI_Send_cmd(uint8_t cmd)
{
	
	
	LHI_Set_parallel_direction(DIR_OUT);
	LHI_TX_byte(cmd);
	CLR_LCD_RW;
	CLR_LCD_DI;
	LHI_Lcd_E_edge(1);
	LHI_Set_parallel_direction(DIR_IN);
	return RET_OK;
}

int	LHI_Read_status(void)
{
	LHI_Set_parallel_direction(DIR_IN);
	CLR_LCD_DI;
	SET_LCD_RW;
	SET_LCD_E;
	return LHI_RX_byte();
}

int LHI_Write_vram(uint8_t *buf, int wr_len)
{
	int len = wr_len;
	
	
	
	while(len)
	{
		len --;
		LHI_Wait(1000);
		LHI_TX_byte(*buf++);
		CLR_LCD_RW;
		SET_LCD_DI;
		LHI_Lcd_E_edge(1);
	}
//	LHI_Set_parallel_direction(DIR_IN);
	
	return wr_len;
}
int LHI_Read_vram(uint8_t *buf, int rd_len)
{
	int len = rd_len;
	SET_LCD_RW;
	SET_LCD_DI;
	SET_LCD_E;
	while(len)
	{
		len --;
		*buf++ = LHI_RX_byte();
	}
	
	
	return rd_len;
}
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{

static int LHI_Pulse(int duty)
{
	
	
	return duty* PWM_100 / 1000;	
}

//因为用GPIO模拟并口，因此在输出时要确保不会导致其他GPIO引脚的输出被破坏
static void LHI_TX_byte(uint8_t b)
{
	uint16_t d;
	uint16_t tmp = 0;
	
	LHI_Set_parallel_direction(DIR_OUT);
	d = GPIO_ReadOutputData(PORT_LCD_DATAS);
	
	d &= (1 << PIN_LCD_DATAS_OFFSET) - 1; 
	tmp |= b << PIN_LCD_DATAS_OFFSET;
	
	d |= tmp;   
	
	
	GPIO_Write(PORT_LCD_DATAS,d);
	
	
	
	
}

static uint8_t LHI_RX_byte(void)
{
	uint16_t d;
	//高电平读取
//	LHI_Lcd_E_edge(1);
	
	d = GPIO_ReadInputData(PORT_LCD_DATAS);
	
	return d >> PIN_LCD_DATAS_OFFSET;
	
}


static void LHI_Set_parallel_direction(int in_out)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	if(in_out == DIR_IN)	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //输入
	else
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //2M时钟速度
	
	GPIO_InitStructure.GPIO_Pin = PIN_LCD_DATA_0 | PIN_LCD_DATA_1 | \
			PIN_LCD_DATA_2 | PIN_LCD_DATA_3 | 
			PIN_LCD_DATA_4 | PIN_LCD_DATA_5 | \
			PIN_LCD_DATA_6 | PIN_LCD_DATA_7;
	GPIO_Init(PORT_LCD_DATAS, &GPIO_InitStructure);
}

static void LHI_delay(int ns)
{
	//72Mhz
#define MIN_NS   13  //1000 ns / 72
	int i =ns / MIN_NS ;
	while(i)
		i --;
	
}

static void LHI_Lcd_E_edge(char edge)
{
	if(edge == 0)
	{
		CLR_LCD_E;
		SET_LCD_E;
		
	}
	else
	{
		SET_LCD_E;
		CLR_LCD_E;
	}
	//delay

	
}


