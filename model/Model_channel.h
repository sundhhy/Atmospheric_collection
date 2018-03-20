#ifndef __INC_Model_channel_H_
#define __INC_Model_channel_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include "Model.h"
#include "system.h"
#include "model_conf.h"
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
//#define AUX_CHN_record_MB  					0x10
//#define AUX_CHN_lower_limit  				0x10
//#define AUX_CHN_upper_limit  				0x11
//#define AUX_CHN_small_signal  				0x12
//#define AUX_CHN_K							0x13
//#define AUX_CHN_B							0x14

#define MDHCHN_CHN_NUM		0x20
#define MAX_TOUCHSPOT		3

#define ALM_HH					0x40
#define ALM_HI					0x20
#define ALM_LO					0x10
#define ALM_LL					0x08
#define ALM_CODE_HH			1
#define ALM_CODE_HI			2
#define ALM_CODE_LO			3
#define ALM_CODE_LL			4


//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------
typedef enum {
	chnaux_record_mb = 0x10,
	chnaux_tag_NO,
	chnaux_filter_ts,
	chnaux_lower_limit,
	chnaux_upper_limit,
	chnaux_small_signal,
	chnaux_k,
	chnaux_b,
	alarm_hh,
	alarm_hi,
	alarm_lo,
	alarm_ll,
	tchspt_hh,
	tchspt_hi,
	tchspt_lo,
	tchspt_ll,
	alarm_backlash,
	DO_output
}e_chn_aux_t;


//#define B_o         	 0  	/*B���ȵ�ż*/
//#define E_o        		 1  	/*E���ȵ�ż*/
//#define J_o        		 2    	/*J���ȵ�ż*/
//#define K_o         	 3    	/*K���ȵ�ż*/
//#define S_o         	 4    	/*S���ȵ�ż*/
//#define T_o        		 5    	/*T���ȵ�ż*/
//#define Pt100          	 6    	/*Pt100�ȵ���*/
//#define Cu50           	 7    	/*Cu50�ȵ���*/
//#define mA0_10			 8		/*0~10���������ź�*/
//#define mA4_20			 9		/*4~20���������ź�*/
//#define V_5			 	 10		/*0~5�����ź�*/
//#define V_1_5			 11		/*1��5�����ź�*/
//#define mV_20      	 	 12		/*0~20������ѹ�ź�*/
//#define mV_100     	 	 13		/*0��100������ѹ�ź�*

typedef enum {
	 
	
	
	
	
	
	AI_B,
	AI_E,
	AI_J,
	AI_K,	
	AI_S,
	AI_T,
	AI_Pt100,
	AI_Cu50,
	AI_0_10_mA,
	AI_4_20_mA,
	AI_0_5_V,
	AI_1_5_V,
	AI_0_20_mV,
	AI_0_100_mV,
	
	
//	AI_0_400_ohm,
//	PI_0_30_kHz,
//	DI_8_30_V,
//	DI_0_5_V,
//	AO_4_20_mA,
	es_max,
}e_signal_t;

typedef struct {
	uint8_t			chn_NO;
	uint8_t			tag_NO;
	uint8_t			signal_type;
	uint8_t			unit;
	uint8_t			MB;			//��¼������M
	uint8_t			filter_time_s;
	uint8_t			decimal;
	uint8_t			flag_err;		//0 �޴��� 1 ����ʧ��    8
	short				small_signal;					//10
	int16_t		lower_limit, upper_limit;		//	14
	
	/*
	�����޵�Ĭ��ֵ���ź������й�
	const int16 SingleTypeInfo[TYPENUMS][3]={
	{4,250,1820},		B
	{4,-148,1000},		E
	{4,-148,1200},		J
	{4,-148,1370},		K
	{4,-50,1760},		S
	{3,-400,4000},		T
	{3,0,1000},			0 - 20 mv
	{3,0,1000},			0 - 10 mv
	{3,0,1000},			0 - 5V
	{3,0,1000},			1 - 5V
	{3,0,1000},			0 - 10mA
	{3,0,1000},			4 - 20mA
	{3,-999,8500},		pt100
	{3,-500,1500},		Cu50

	};
		
	
	
	*/

	//K��С����̶�Ϊ2��B��С����������ź����͵�С����һ��
	short				k,b;				//18
	/******************************************/
	int16_t			value;					//20
	uint16_t		sample_value;				//	22
	uint8_t			smp_flag;				//23	0 ����ֵ��Ч  1 ����ֵ��Ч
	uint8_t			decimal_places;		//ֻ�в��µĲ���1λСʱ��������������
}chn_info_t;

typedef struct {
	int16_t				alarm_hh;
	int16_t				alarm_hi;
	int16_t				alarm_lo;
	int16_t				alarm_ll;
	
	//�����������
	uint8_t				touch_spot_hh;
	uint8_t				touch_spot_hi;
	uint8_t				touch_spot_lo;
	uint8_t				touch_spot_ll;
	
	uint8_t				alarm_backlash;		//�����ز�  0 - 10.0%
	
	uint8_t				alm_flag;
	/*
	0x40:�߸߱�
	0x20:�߱�
	0x10:�ͱ�
	0x08:�͵ͱ�
	*/
	
	
	
	uint8_t				num_alms_in_stg;		//�洢���еı�������
	uint8_t				none;
}chn_alarm_t;
	
typedef struct {
	uint8_t			do_chn;
	uint8_t			val;
	uint8_t			none[2];
}do_out_t;
	

typedef struct
{
	uint8_t			alarm_backlash;		//�����ز�  0 - 10.0%			29
	uint8_t			tag_NO;
	uint8_t			signal_type;
	uint8_t			unit;
	uint8_t			MB;			//��¼������M
	uint8_t			filter_time_s;
	uint8_t			decimal;
	uint8_t			none;		//0 �޴��� 1 ����ʧ��
	short				small_signal;
	int16_t			lower_limit, upper_limit;		//
	short				k,b;													//16
	
	short				alarm_hh;
	short				alarm_hi;
	short				alarm_lo;
	short				alarm_ll;											//24
	
	//�����������
	uint8_t				touch_spot_hh;
	uint8_t				touch_spot_hi;
	uint8_t				touch_spot_lo;
	uint8_t				touch_spot_ll;						//28
	
//	uint8_t				alarm_backlash;		//�����ز�  0 - 10.0%			29
//	uint8_t				none[3];							//32
	
}mdl_chn_save_t;		//Ҫ�������Ϣ

CLASS(Model_chn)
{
		
	EXTENDS( Model);
	chn_info_t		chni;
	chn_alarm_t		alarm;
	struct {
		uint8_t		alm_hh_index;
		uint8_t		alm_hi_index;
		uint8_t		alm_lo_index;
		uint8_t		alm_ll_index;
	}alarm_mgr;
	
	char 	*str_buf;
	char 	*unit_buf;
	char 	*alarm_buf;
};

//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------
extern Model		*arr_p_mdl_chn[NUM_CHANNEL];
//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------
Model_chn *Get_Mode_chn(int n);
void MdlChn_Read_code_end_temperature();
int	MdlChn_save_data(uint8_t chn_num, mdl_chn_save_t *p);		//��Ҫ�洢�����ݷ���p�����س���
void MdlChn_default_conf(int chn_num);
void MdlChn_default_alarm(int chn_num);
void MdlChn_Clean_Alamr(int chn_num);
int MdlChn_Commit_conf(int chn_num);			//�ύ�޸�,���ͨ��������д��洢�ͷ��͸�ͨ����
uint8_t  MdlChn_Cal_prc( Model *self, int val);


#endif
