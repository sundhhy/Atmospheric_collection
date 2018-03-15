#include "Config.h"	 
#include <stdio.h>
#include "GlobalDeclare.h"
// TIM1ģ���ʼ��
void InitTIM1(void)
{
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    //TIM1�������������ã�����PWMƵ�ʣ�
    //Ƶ��=TIM1_CLK/(ARR+1)
    TIM_BaseInitStructure.TIM_Period = 100-1;			//102.88KHz			
    TIM_BaseInitStructure.TIM_Prescaler = 72-1;		  	//TIM1ʱ��=72Mhz/7
    TIM_BaseInitStructure.TIM_ClockDivision = 0;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
    //����ARR��Ӱ�ӼĴ�����ֱ�����������¼��Ÿ������ã�
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    //TIM1_OC1ģ�����ã�����1ͨ��ռ�ձȣ�
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 50;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    //����CCR1�Ĵ�����Ӱ�ӼĴ�����ֱ�����������¼��Ÿ������ã�
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    
    //TIM1����
    TIM_Cmd(TIM1, ENABLE);
    //TIM1_OCͨ�����PWM��һ��Ҫ�ӣ�
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
void InitPWM(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* PE9 PWM��� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //2Mʱ���ٶ�
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);	//TIM1ȫ����ӳ��
	InitTIM1();
}
void PWM_PluseWide(USHORT Duty)
{
	unsigned short PWM;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_Cmd(TIM1, DISABLE);
	PWM = 70 + ((USHORT)Duty*20/100);	//PWM��70%~90%��Χ�ڱ仯
	//TIM1_OC1ģ�����ã�����1ͨ��ռ�ձȣ�
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = PWM;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    //����CCR1�Ĵ�����Ӱ�ӼĴ�����ֱ�����������¼��Ÿ������ã�
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_Cmd(TIM1, ENABLE);
}
// TIM4ģ���ʼ��
void InitTIM4(void)
{
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    //TIM1�������������ã�����PWMƵ�ʣ�
    //Ƶ��=TIM1_CLK/(ARR+1)
    TIM_BaseInitStructure.TIM_Period = 100-1;			//102.88KHz			
    TIM_BaseInitStructure.TIM_Prescaler = 4-1;		  	//TIM1ʱ��=72Mhz/7
    TIM_BaseInitStructure.TIM_ClockDivision = 0;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure);
    //����ARR��Ӱ�ӼĴ�����ֱ�����������¼��Ÿ������ã�
    TIM_ARRPreloadConfig(TIM4, ENABLE);

    //TIM1_OC1ģ�����ã�����1ͨ��ռ�ձȣ�
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse =90;
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    //����CCR4�Ĵ�����Ӱ�ӼĴ�����ֱ�����������¼��Ÿ������ã�
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
    
    //TIM1����
    TIM_Cmd(TIM4, ENABLE);
    //TIM1_OCͨ�����PWM��һ��Ҫ�ӣ�
    TIM_CtrlPWMOutputs(TIM4, ENABLE);
}
