//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
#include "sys_cmd.h"
#include "system.h"
#include "Communication/modbusRTU_cli.h"
#include "sdhDef.h"
#include "device.h"
#include "hardwareConfig.h"
#include "channel_accumulated.h"
#include <string.h>

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------

rcd_chn_accumlated_t	arr_chn_acc[NUM_CHANNEL];
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
static void CNA_u64_add(uint16_t *p_u16, int32_t val, char num_u16);

		
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
int CNA_Init(void)
{
	int 				i;
	struct  tm			t;
	Storage			*stg = Get_storage();
	//�Ӵ洢���ж�ȡ�洢���ۻ���Ϣ
	System_time(&t);
	for(i = 0 ;i < NUM_CHANNEL; i++)
	{
		STG_Set_file_position(STG_CHN_SUM(i), STG_DRC_READ, 0);
		
		stg->rd_stored_data(stg, STG_CHN_SUM(i), arr_chn_acc + i, sizeof(rcd_chn_accumlated_t));
		if(arr_chn_acc[i].sum_start_year == 0)
		{

			arr_chn_acc[i].sum_start_year = t.tm_year;
		}	
		if(arr_chn_acc[i].sum_start_month == 0)
		{

			arr_chn_acc[i].sum_start_month = t.tm_mon;
		}	
		if(arr_chn_acc[i].sum_start_day == 0)
		{

			arr_chn_acc[i].sum_start_day = t.tm_mday;
		}			
		
	}
	
	
	
	return RET_OK;
}


void CNA_Run(int cyc_ms)
{
	short 					chn_num, i;
	uint32_t				cur_time_s;
	uint32_t				start_time_s;
	struct  tm			t = {0};
	Model_chn				*p_mc;
	int32_t				sum = 0;
	uint32_t				temp = 0;
	cur_time_s = Time_2_u32(&SYS_TIME);
	for(chn_num = 0; chn_num < NUM_CHANNEL; chn_num ++)
	{
		t.tm_year = arr_chn_acc[chn_num].sum_start_year;
		t.tm_mon = arr_chn_acc[chn_num].sum_start_month;
		t.tm_mday = arr_chn_acc[chn_num].sum_start_day;
		t.tm_hour = 0;
		t.tm_min = 0;
		t.tm_sec = 0;
		start_time_s = Time_2_u32(&t);
		//�жϵ�ǰʱ���Ƿ��Ѿ��������õĿ�ʼ�ۻ�ʱ��
		if(cur_time_s <= start_time_s)
			continue;
		
		if(SYS_TIME.tm_year != arr_chn_acc[chn_num].sum_year)
		{
			//��ݱ仯��ʱ�򣬰����ۼƺ������
			arr_chn_acc[chn_num].sum_year=SYS_TIME.tm_year;
			
			for(i=0;i<12;i++) 
			{
				arr_chn_acc[chn_num].accumlated_month[i][0]=0;
				arr_chn_acc[chn_num].accumlated_month[i][1]=0;
				arr_chn_acc[chn_num].accumlated_month[i][2]=0;      			   	     	
			} 
			arr_chn_acc[chn_num].accumlated_year[0]=0; 
			arr_chn_acc[chn_num].accumlated_year[1]=0;
			arr_chn_acc[chn_num].accumlated_year[2]=0;  
			
		}
		if(SYS_TIME.tm_mon != arr_chn_acc[chn_num].sum_month)
		{
			
			//���·ݱ仯ʱ�����ۻ����
			arr_chn_acc[chn_num].sum_month=SYS_TIME.tm_mon;
			for(i=0;i<31;i++)
			{
				arr_chn_acc[chn_num].accumlated_day[i][0]=0;
				arr_chn_acc[chn_num].accumlated_day[i][1]=0;
				arr_chn_acc[chn_num].accumlated_day[i][2]=0;      			    	
			} 
		}
		
		
		if((SYS_TIME.tm_hour == 0)&&(SYS_TIME.tm_min == 0)&&(SYS_TIME.tm_sec == 0))
		{ //�µ�һ�쿪ʼ��ʱ��Ҫ����һЩ������        	   
			if(SYS_TIME.tm_mday == 1)
			{
			//�µ�һ���·�  
				arr_chn_acc[chn_num].sum_month=SYS_TIME.tm_mon;

				for(i=0;i<31;i++)
				{
					arr_chn_acc[chn_num].accumlated_day[i][0]=0;
					arr_chn_acc[chn_num].accumlated_day[i][1]=0;
					arr_chn_acc[chn_num].accumlated_day[i][2]=0;      			    	
				} 
				if(SYS_TIME.tm_mon==1)
				{
				//�µ�һ��
					arr_chn_acc[chn_num].sum_year=SYS_TIME.tm_year;

					for(i=0;i<12;i++) 
					{
						arr_chn_acc[chn_num].accumlated_month[i][0]=0;
						arr_chn_acc[chn_num].accumlated_month[i][1]=0;
						arr_chn_acc[chn_num].accumlated_month[i][2]=0;      			   	     	
					} 
					arr_chn_acc[chn_num].accumlated_year[0]=0; 
					arr_chn_acc[chn_num].accumlated_year[1]=0;
					arr_chn_acc[chn_num].accumlated_year[2]=0;  
				}
			}      		
		}
		
		p_mc = Get_Mode_chn(chn_num);
		temp = p_mc->chni.value + arr_chn_acc[chn_num].accumlated_remain;
		//���ݵ�λ������ÿ�������
		
		
		
		switch(p_mc->chni.unit)
		{
			case eu_Nm3_h:
			case eu_m3_h:
			case eu_L_h:
			case eu_kg_h:
				sum = temp / 3600;
				arr_chn_acc[chn_num].accumlated_remain = temp % 3600;
				break;
			case eu_m3_min:
			case eu_L_min:
			case eu_kg_min:
			case eu_r_min:
				sum = temp / 60;
				arr_chn_acc[chn_num].accumlated_remain = temp % 60;
				break;
			default:
				sum = temp;
				arr_chn_acc[chn_num].accumlated_remain = 0;
				break;
			
		}
		
		CNA_u64_add(arr_chn_acc[chn_num].accumlated_day[SYS_TIME.tm_mday - 1], sum, 3);
		CNA_u64_add(arr_chn_acc[chn_num].accumlated_month[SYS_TIME.tm_mon - 1], sum, 3);
		CNA_u64_add(arr_chn_acc[chn_num].accumlated_year, sum, 3);
		CNA_u64_add(arr_chn_acc[chn_num].accumlated_total, sum, 3);
		
		//д��flash
		CNA_Commit(chn_num);

	}
	
	
	
}

void CNA_Print_enable(char *s, char	enable)
{
	if(enable)
	{
		
		sprintf(s, "��");
	}
	else
	{
		
		sprintf(s, "��");
	}
	
	
}

//prec ��ʾ�м�λС��
uint64_t CNA_arr_u16_2_u64(uint16_t *p_u16, char num_data)
{
	
	uint64_t rst = 0;
	int 		i;
	
	for(i = num_data - 1; i >= 0; i--)
	{
		rst <<= 16;
		rst |= p_u16[i];
		
	}
	
//	return rst;
	return 0xffffffffffff;
}

//pos С����λ��
//�ַ����ܳ��Ȳ�����14
//�����С���㣬���������ֳ��Ȳ�����12
void CNA_Print_acc_val(uint16_t *p_val, char *s, char pos)
{
	
	
	unsigned  long templong1,templong2;
	uint16_t temd1,temd2,temd3;
	uint8_t i,j,d;	
	
	uint8_t  m;	
	uint8_t  first_data_num = 0;
	uint8_t	 num_bytes = 0;
	
	#define STR_BYTES   14
	#define STR_END_NUM	13 // STR_BYTES - 1 
	temd1=*p_val;
	temd2=*(p_val+1);
	temd3=*(p_val+2);

//	temd1 = 0xffff;
//	temd2 = 0xffff;
//	temd3 = 0xffff;
		
	for(i=0;i<STR_BYTES;i++)
	{
	    *(s+i)= 0;
	}	
	templong1=(unsigned long )temd1*0x10000+temd2;
	j=0;
	
	while((templong1!=0)||(temd3!=0))
	{
		d=templong1%10;
		templong1/=10;	
		templong2=(unsigned long)d*0x10000+temd3;
		temd3=templong2/10;
		*(s+STR_END_NUM-j)=templong2%10 + '0';
		j++;
		if(j > STR_END_NUM)
			break;
	}
	
	if(j == 0)
	{
		s[STR_END_NUM] = '0';
		j = 1;
	}
	
	
	num_bytes = j;
	first_data_num = STR_BYTES - j;
	
	//����С����
	if(pos == 0)
		goto aligin_left;
	m = pos;
	first_data_num --; //Ҫ��С������һλ����
	num_bytes ++;
	
	for(i=0;i<STR_END_NUM - m;i++)
	{
		*(s+i)=*(s+1+i);
	}
	*(s+i)= '.';
	//�������.1����������������С����ǰ��Ҫ��0
	if(*(s+i - 1)== 0) 
	{
		*(s+i - 1)= '0';
		first_data_num --; //Ҫ��С������һλ����
		num_bytes ++;
		
	}
	aligin_left:
	//�������
	
	if(num_bytes == STR_BYTES)
		return;
	
	
	
		//������������ƶ�
	for(i = 0; i < num_bytes; i++)
	{
		*(s + i) = *(s + first_data_num + i);
		
	}
	//��β�����ո�
	for(;i < STR_BYTES; i++)
	{
		
		*(s + i) = ' ';
	}
}



int		CNA_Commit(char chn_num)
{
	Storage			*stg = Get_storage();
	
	STG_Set_file_position(STG_CHN_SUM(chn_num), STG_DRC_WRITE, 0);
	stg->wr_stored_data(stg, STG_CHN_SUM(chn_num), arr_chn_acc + chn_num, sizeof(rcd_chn_accumlated_t));
	
	return RET_OK;

}

int		CNA_Clear(char chn_num)
{
	
	char *p_clear;
	Storage			*stg = Get_storage();
	
	p_clear = (char *)(arr_chn_acc + chn_num);
	p_clear += 6;
	
	memset(p_clear, 0, sizeof(rcd_chn_accumlated_t) - 6);
	
	STG_Set_file_position(STG_CHN_SUM(chn_num), STG_DRC_WRITE, 0);
	stg->wr_stored_data(stg, STG_CHN_SUM(chn_num), arr_chn_acc + chn_num, sizeof(rcd_chn_accumlated_t));
	
	return RET_OK;

}

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{
//ǰ������:num_u16 > 2
//���ֽ���ǰ
static void CNA_u64_add(uint16_t *p_u16, int32_t val, char num_u16)
{
	uint32_t old_val;
	uint32_t tmp_u32;
	if(num_u16 < 2)
		return;
	num_u16 --;
	
	tmp_u32 = p_u16[num_u16 - 1] * 0x10000 + p_u16[num_u16];
	
	old_val = tmp_u32;
	tmp_u32 += val;
	
	p_u16[num_u16] = tmp_u32 & 0xffff;
	p_u16[num_u16 - 1] = tmp_u32 >> 16;
	
	if(tmp_u32 >= old_val)		//û�з�����λ
	{
		
		return;
	}
		
	//�����˽�λ
	num_u16 --;
	while(num_u16)
	{
		old_val = p_u16[num_u16 - 1];
		p_u16[num_u16 - 1] ++;
		
		if(p_u16[num_u16 - 1] < old_val)
		{
			//������λ
			num_u16 --;
			
		}
		else
			break;
		
	}
	
	
	
	
}

