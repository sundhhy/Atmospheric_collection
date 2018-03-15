#include "time.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "basis/sdhError.h"
#include "mySystemClock.h"
#include "mem/CiiMem.h"

TIME_T g_time;



static char	Set_AlarmClock_flag[MAX_ALARM_TOP] = {0};
static uint32_t	Alarmtims_ms[MAX_ALARM_TOP] = {0};
static uint32_t	AlarmStart_ms[MAX_ALARM_TOP] = {0};

static time_task_manager *List_time_job_head;

uint32_t get_time_s(void)
{
	return g_time.time_s ;
	
}

uint64_t get_time_ms(void)
{
	uint64_t ms = g_time.time_s * 1000 + g_time.time_ms;
	return ms ;
	
}
//runtimes 0 ���޴����� ����0 ָ���Ĵ���
void regist_timejob( uint32_t period_us, time_job job, void *arg)
{
	time_task_manager *current_job;
	time_task_manager *next_job;
	if( List_time_job_head == NULL)
	{
		List_time_job_head = CALLOC(1,  sizeof( time_task_manager));
	}
	current_job = List_time_job_head;
	next_job = (time_task_manager *)current_job->next;
	while( next_job)
	{
		if( current_job->my_job == job)
		{
			current_job->count_us = 0;
			return;
		}
		current_job = next_job;
		next_job = (time_task_manager *)current_job->next;
	}
	
	//���ڵ��Ѿ���ռ�þ�������һ���ڵ�
	
	if( current_job->period_us)
	{
		current_job->next = malloc( sizeof( time_task_manager));
		current_job = (time_task_manager *)current_job->next;
		memset( current_job, 0, sizeof( time_task_manager));
	}
	
	current_job->period_us = period_us;
	current_job->my_job = job;
	current_job->arg = arg;
	current_job->next = NULL;
}

//һ��ֻ����3����һ������
//���Ѿ�������һ�������Ժ��ٴ��������ӽ�����ʱ����Ϊ������ʼʱ��
void set_alarmclock_s(int alarm_id, int sec)
{
	if( alarm_id >= MAX_ALARM_TOP)
		return ;
	
//	if( Set_AlarmClock_flag[alarm_id] == 0)
	{
		
		Alarmtims_ms[alarm_id] = sec * 1000;
		AlarmStart_ms[alarm_id] = g_time.time_ms + g_time.time_s *1000;
		Set_AlarmClock_flag[alarm_id] = 1;
	}
	
}

void set_alarmclock_ms(int alarm_id, int msec)
{
	if( alarm_id >= MAX_ALARM_TOP)
		return ;
	
//	if( Set_AlarmClock_flag[alarm_id] == 0)
	{
		
		Alarmtims_ms[alarm_id] = msec;

		AlarmStart_ms[alarm_id] = g_time.time_ms + g_time.time_s *1000;
		Set_AlarmClock_flag[alarm_id] = 1;
	}
	
}
int Ringing(int alarm_id)
{
	uint32_t now_ms = 0;
	if( alarm_id >= MAX_ALARM_TOP)
		return ERR_BAD_PARAMETER;
	if( Set_AlarmClock_flag[alarm_id])
	{
		now_ms = g_time.time_ms + g_time.time_s *1000;
		if( now_ms - AlarmStart_ms[alarm_id] >= Alarmtims_ms[alarm_id])
		{
			
			Set_AlarmClock_flag[alarm_id] = 0;
			return ERR_OK;
		}
		
	}
	return ERR_UNKOWN;
	
	
}

//Ӳ�����
void InitTimer(TIM_TypeDef *time, uint32_t us)
{
	
	
    TIM_TimeBaseInitTypeDef TIM_TimeBaseSturcture;			 //����TIM�ṹ�����
    TIM_DeInit(time);										 //��λʱ��TIM2

		g_time.period_us = us;
    TIM_TimeBaseSturcture.TIM_Period = us ;				  //��ʱ�����ڣ����ؼĴ���
    TIM_TimeBaseSturcture.TIM_Prescaler =  ( SystemCoreClock/1000000 - 1);				  //��Ƶ  1M
    TIM_TimeBaseSturcture.TIM_ClockDivision = 0x00;				//TIM_CKD_DIV1    
    TIM_TimeBaseSturcture.TIM_CounterMode = TIM_CounterMode_Up; //Ӌ����ʽ	   

    TIM_TimeBaseInit(time,&TIM_TimeBaseSturcture);
																//��ʼ��
    TIM_ClearFlag(time,TIM_FLAG_Update);						//������I
    TIM_ITConfig(time, TIM_IT_Update,ENABLE);
    TIM_Cmd(time, ENABLE);										//ʹ��
}

void TIM2_IRQHandler(void)          //��ʱ���ж�Լ10ms
{
	time_task_manager *current_job = List_time_job_head;
	

    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    { 
			g_time.count_us += g_time.period_us;
			if( g_time.count_us >= 1000)
			{
				g_time.time_ms ++;
				g_time.count_us -= 1000;
			}
		
			while( current_job)
			{
				current_job->count_us += g_time.period_us;
				if( current_job->count_us >=  current_job->period_us)
				{
					current_job->count_us = 0;
					
					
					
					current_job->my_job( current_job->arg);
					
					
					
					
				}
				current_job = (time_task_manager *)current_job->next;
			}

			if( g_time.time_ms >= 1000)
			{
				g_time.time_s ++;
				g_time.time_ms -= 1000;
				
			}   

//			TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

//��ʱ��1����һ�룬ԭ��δ֪
//void TIM1_BRK_UP_TRG_COM_IRQHandler(void)          //��ʱ���ж�Լ10ms
//{
//	time_task_manager *current_job = List_time_job_head;
//	

//    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
//    { 
//			g_time.count_us += g_time.period_us;
//			if( g_time.count_us >= 1000)
//			{
//				g_time.time_ms ++;
//				g_time.count_us -= 1000;
//			}
//		
//			while( current_job)
//			{
//				current_job->count_us += g_time.count_us;
//				if( current_job->count_us >=  current_job->period_us)
//				{
//					current_job->count_us = 0;
//					
//					
//					
//					current_job->my_job();
//					
//					
//					
//					
//				}
//				current_job = (time_task_manager *)current_job->next;
//			}

//			if( g_time.time_ms >= 1000)
//			{
//				g_time.time_s ++;
//				g_time.time_ms -= 1000;
//				
//			}   

////			TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
//			TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//    }
//}

void clean_time_flags(void)
{
    g_time.time_ms = 0;
    g_time.time_s = 0;
	g_time.count_us = 0;
   
}

void TimeRunms( int ms)
{
	set_alarmclock_ms(0, ms);
	while(1)
	{
		if( Ringing(0) == ERR_OK)
			break;
		 
	}
	
}

void time_test(void)
{
	while(1)
	{
		set_alarmclock_s(0, 10);
		while(1)
		{
			if( Ringing(0) == ERR_OK)
				break;
			 
		}
		
		
	}
	
	
}



