#include "Config.h"	 
#include <stdio.h>
#include "GlobalDeclare.h"
// TIM1模块初始化
void InitTIM1(void)
{
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    //TIM1基本计数器设置（设置PWM频率）
    //频率=TIM1_CLK/(ARR+1)
    TIM_BaseInitStructure.TIM_Period = 100-1;			//102.88KHz			
    TIM_BaseInitStructure.TIM_Prescaler = 72-1;		  	//TIM1时钟=72Mhz/7
    TIM_BaseInitStructure.TIM_ClockDivision = 0;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
    //启用ARR的影子寄存器（直到产生更新事件才更改设置）
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    //TIM1_OC1模块设置（设置1通道占空比）
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 50;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    //启用CCR1寄存器的影子寄存器（直到产生更新事件才更改设置）
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    
    //TIM1开启
    TIM_Cmd(TIM1, ENABLE);
    //TIM1_OC通道输出PWM（一定要加）
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
void InitPWM(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* PE9 PWM输出 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //2M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);	//TIM1全部重映像
	InitTIM1();
}
void PWM_PluseWide(USHORT Duty)
{
	unsigned short PWM;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_Cmd(TIM1, DISABLE);
	PWM = 70 + ((USHORT)Duty*20/100);	//PWM在70%~90%范围内变化
	//TIM1_OC1模块设置（设置1通道占空比）
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = PWM;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    //启用CCR1寄存器的影子寄存器（直到产生更新事件才更改设置）
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_Cmd(TIM1, ENABLE);
}
// TIM4模块初始化
void InitTIM4(void)
{
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    //TIM1基本计数器设置（设置PWM频率）
    //频率=TIM1_CLK/(ARR+1)
    TIM_BaseInitStructure.TIM_Period = 100-1;			//102.88KHz			
    TIM_BaseInitStructure.TIM_Prescaler = 4-1;		  	//TIM1时钟=72Mhz/7
    TIM_BaseInitStructure.TIM_ClockDivision = 0;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure);
    //启用ARR的影子寄存器（直到产生更新事件才更改设置）
    TIM_ARRPreloadConfig(TIM4, ENABLE);

    //TIM1_OC1模块设置（设置1通道占空比）
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse =90;
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    //启用CCR4寄存器的影子寄存器（直到产生更新事件才更改设置）
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
    
    //TIM1开启
    TIM_Cmd(TIM4, ENABLE);
    //TIM1_OC通道输出PWM（一定要加）
    TIM_CtrlPWMOutputs(TIM4, ENABLE);
}
