#ifndef RDB_H
#define	RDB_H
#include "config.h"
//#include "rtc.h"



/*ʵʱ���ݿ������Ϣ�ļ�����*/

#define	RDB_BASE_MAXNUM			16
#define	RDB_SAMPLE_MAXNUM  		16
#define	RDB_RECORD_MAXNUM		16
#define	RDB_ALARM_MAXNUM			16
#define	FUN_SUMMOD_MAXNUM		16
#define	FUN_SUMRECORD_MAXNUM	16
#define	FUN_FLUXMOD_MAXNUM		16
#define	FUN_PIDMOD_MAXNUM		8
#define	FUN_FILTERMOD_MAXNUM	16
#define	FUN_TRANFORMMOD_MAXNUM 8
#define 	SIZE_OF_RDBBASE 			(sizeof(_RdbBase_Record))
#define 	SIZE_OF_RDBSAMPLE 		(sizeof(_RdbSample_Record))
#define 	SIZE_OF_RDBRECORD			(sizeof(_RdbRecord_Record))
#define 	SIZE_OF_RDBALARM			(sizeof(_RdbAlarm_Record))
#define 	SIZE_OF_FILEHEAD			64
#define	SIZE_OF_SUMMOD			(sizeof(_SumMod))
#define	SIZE_OF_SumRecord			(sizeof(_SumRecord))
#define	SIZE_OF_FLUXMOD			(sizeof(_FluxMod))
#define	SIZE_OF_PIDMOD			(sizeof(_PidMod))
#define	SIZE_OF_TRANSFORMMOD	(sizeof(_TransformMod))
#define	SIZE_OF_FILTERMOD			(sizeof(_FilterMod))
#define	SIZE_OF_CONFIGURATION	(sizeof(_Configuration))

/*****************************************/
/*	RDB�ļ�ͷ��Ϣ�������ļ��汾��Rdb������Ŀ*/
typedef	struct 			/*64 RDB�ļ�ͷ��Ϣ�ṹ*/
{
	unsigned char		FileInfo[16];		/*00,16=16�ļ���Ϣ*/
	unsigned char		iVersion[4];		/*16,04=20 �汾��*/
	unsigned long		FileDateTime;	/*20,04=24 �ļ�����ʱ��*/
	unsigned long		FileRightFlag;	/*24,04=28 �ļ���ȷ��ʶ*/
	unsigned short	MaxRdbId;		/*28,02=30 Rdb�������*/
	unsigned short	RdbNumber;		/*30,02=32 Rdb������Ŀ*/
	unsigned short	CvNumber;		/*32,02=34 ת����Ϣ��Ŀ*/
	unsigned short	SampleNumber;	/*34,02=36 �ɼ���Ϣ��Ŀ*/
	unsigned short	AlarmNumber;	/*36,02=38 ������Ϣ��Ŀ*/
	unsigned short	RecordNumber;	/*38,02=40 ��¼��Ϣ��Ŀ*/
	unsigned short	DeviceNumber;	/*40,02=42 �豸��Ϣ��Ŀ*/
	unsigned short	PortNumber;		/*42,02=44 �˿���Ϣ��Ŀ*/
	unsigned char		nouse20[20];	/*44,20=64 ����*/
}_RdbFile_Head;

typedef struct _RdbBase_Record			/*64 RDB������Ϣ�ṹ*/
{
	signed long		TagID;			/*00,04=04��������	*/
	unsigned char  	Name[11];		/*04,16=20������		*/
	unsigned char		nouse[5];           /*����*/
	short			Value;			/*20,04=24����ֵ		*/
	short			Reserve4;
	unsigned char		VarType;		/*24,01=25��������	*/
	unsigned char		RecordProp;		/*25,01=26��������	*/
	unsigned char		RwProp;			/*26,01=27��д����	*/
	unsigned char		OpenProp;		/*27,01=28��������	*/
	float				InitValue;		/*28,04=32������ֵ	*/
	//float			MinEVal;		/*32,04=36��С����ֵ*/
	short			Point;			/*С��λ��*/
	short			MinEVal;			/*��������*/
	//float			MaxEVal;		/*36,04=40��󹤳�ֵ*/
	short			HisValue;		/*��ֵ*/
	short			MaxEVal;		/*��������*/		
	//float			MinRVal;			/*40,04=44��С����ֵ*/
	short                     MinRVal;	/*�����ź�����*/
	short			Reserve2;		/*����*/
	//float			MaxRVal;		/*44,04=48������ֵ*/
	short                     MaxRVal;    /*�����ź�����*/
	short			Reserve1;		/*����*/
	unsigned char		CommentInfoID;/*��λ*/
	unsigned char        DispColor;           /*��ʾ��ɫ*/
	signed short		CvInfoID;		/*50,02=52ת����Ϣ����*/
	signed short		SampleInfoID;	/*52,02=54�ɼ���Ϣ����*/
	signed short		AlarmInfoID;	/*54,02=56������Ϣ����*/
	signed short		OptionInfoID;	/*56,02=58��ʷ��¼����ȫ��Ϣ����*/
	unsigned char       UnitName[6];      /*58,06=59�Զ��嵥λ����*/
	
	       
}_RdbBase_Record;

typedef struct 		/*12 RDB�ɼ���Ϣ�ṹ*/
{
	uint16	TagId;							/*00,02=02 �ɼ�����*/
	int16	DeviceId;						/*02,02=04 �豸����*/
//	uint16	AppendInfo1;						/*04,02=06 ������Ϣ1��ͨ������*/
	uint8	SingalType;						/*ͨ������*/
	uint8	Status;							/*ͨ��״̬*/
	uint16	AppendInfo2;					/*06,02=08 ������Ϣ2��ģ���*/
	uint16	AppendInfo3;					/*08,02=10 ������Ϣ3��ͨ����*/
	uint16	SampleValue;					/*10,02=12 ������Ϣ4������ֵ*/
}_RdbSample_Record;
/*�ɼ���������¼Rdb�����ɼ���Ϣ����������0��ʼ���Ե�����*/
/* 	������Դ����¼Rdb������������Դ���б��أ�0x01�����ⲿ�豸��0x02����*/
/* 	�豸��������¼Rdb�����豸��Ϣ������������Ϊ-1ʱ��ʾ���豸��Ϣ����Ϊ���أ�������ݸ�ֵ��RDB�豸��Ϣ���ҵ���Ӧ�ļ�¼��*/
/* 	������Ϣ1����PLC��Ϊͨ�����ͣ���HMI��Ϊ�������ͣ���VPR��Ϊͨ�����ͣ�*/
/* 	������Ϣ2����PLC��Ϊģ��ţ���HMI��Ϊ�������ͣ���VPR��Ϊģ���*/
/* 	������Ϣ3����PLC��Ϊͨ���ţ���HMI��Ϊ������ַ����VPR��Ϊͨ���ţ�*/
/* 	������Ϣ4����HMI��Ϊ�����ɼ��ı���������*/

/*ʵʱ���ݿ���ʷ��¼��Ϣ�ļ�����*/
typedef struct 		/*16 RDB��¼��Ϣ�ṹ*/
{
	signed short		TagId;				/*00,02=02��¼����*/
	signed char		RecordMethod;		/*02,01=03 ��¼����*/
	unsigned char		TagPriority;			/*03,01=04 �������ȼ�*/
	unsigned long		RecordCycle;		/*04,04=08 ʱ����*/
	float				Sensitivity;			/*08,04=12������*/
	unsigned short	RecordLen;			/*12,04=16 ��¼����*/
	unsigned short	Reserver;
}_RdbRecord_Record;

typedef struct 				/*64 RDB������Ϣ�ṹ*/
{
	signed short		TagId;				/*00,02=02 ��������*/
	unsigned char		AlarmStatus;		/*02,01=03 ����״̬*/	
	unsigned char		AlarmAck;			/*03,01=04 ����ȷ��*/	
	unsigned short	AlarmNo;			/*04,02=06 �������*/
	signed char		AlarmGroup;			/*06,01=07 ������*/
	unsigned char		AlarmType;			/*07,01=08 ��������*/
	unsigned char		AlarmFlag;			/*08,01=09 ����*/
	unsigned char		AlarmEnable;		/*09,01=10 ����ʹ��*/
	unsigned short	DelayTime;			/*10,02=12 ��������ʱ��*/	
	short			Reserve1;
	short			HiHiLimit;
	//float				HiHiLimit;			/*12,04=16 �߸߱�����*/
	short			Reserve2;
	short			HiLimit;
	//float				HiLimit;				/*16,04=20 �߱�����*/
	short			Reserve3;
	short			LoLimit;
	//float				LoLimit;				/*20,04=24 �ͱ�����*/	
	short			Reserve4;
	short			LoLoLimit;
	//float				LoLoLimit;			/*24,04=28 �͵ͱ�����*/
	short			LimitDeadArea;		/*28,04=32 ��������*/	
	short			Reserver5;
	float				RocPctLimit;			/*32,04=36 �仯�ʱ�����*/	
	unsigned char		RocPctTime;			/*36,01=37 �仯�ʱ���ʱ����*/	
	unsigned char		HiHiAlarmNo;		/*37,01=38 �߸߱�����*/
	unsigned char		HiAlarmNo;			/*38,01=39 �߱�����*/
	unsigned char		LoAlarmNo;			/*39,01=40 �ͱ�����*/
	unsigned char		LoLoAlarmNo;		/*40,01=41 �͵ͱ�����*/
	unsigned char		RocPctNo;			/*41,01=42 �仯�ʱ�������*/
	unsigned char 	AlarmHListNo;		/*42,01 ��ǰ�����ڱ�������������*/
	unsigned char		AlarmLListNo;		/*43,01 ��ǰ�����ڱ�������������*/
	unsigned char		AlarmMsg[16];		/*44,16=60 ������Ϣ*/
	unsigned char		nouse4[4];			/*60,04=64 ����*/
}_RdbAlarm_Record;

/*������¼����*/
typedef struct	
{
	uint8	TagID;			/*��ӦRDB������*/
	Time 	StartAlmTime;	/*������ʼʱ��*/
	Time	CancelAlmTime;	/*��������ʱ��*/
	uint8	AlmType;		/*��������*/
	uint8	Confim;			/*����ȷ��*/
}_AlmRecord;
/**********************************************************************************/
/*����Ϊ�㷨�ṹ*/
/**********************************************************************************/
/*�ۻ��㷨�ṹ����*/
typedef struct 	/*8Byte*/
{
	uint8	TagID;			/*��Ӧ��RDB Tag*/
	uint8 	Enable;			/*�Ƿ�����ۻ�*/
	uint8 	StartYear;		/*�ۻ�����ʼ��*/
	uint8 	StartMonth;		/*�ۻ�����ʼ��*/
	uint8 	StartDay;		/*�ۻ�����ʼ��*/
	uint8	Reserve[3];
}_SumMod;
/*�ۻ��б�,Ӧ�洢��FeRAM��,��4352���ֽ�,��ʷ��¼ռ��170���ֽ�*/
 typedef struct 
{
	uint8	SumYear;
	uint8	SumMonth;
	uint16	Sumbuf_day[31][3];		/*���ۻ�*/
	uint16	Sumbuf_month[12][3];	/*���ۻ�*/
	uint16	Sumbuf_year[3];			/*���ۻ�*/
	uint16	Sumbuf_Sum[3];			/*���ۼ�*/
	uint16	Sum_remain;			/*�ۻ�����*/
}_SumRecord;
/*�����㷨,�����ṹ����*/
typedef struct	_FluxMod		/*40Byte*/
{
	uint8   	Enable; 				/*�Ƿ�����ͨ��*/
	uint8	CompMethod;		/*0:�����в���1:ѹ���Ƿ�ֵ2:�¶��Ƿ�ֵ*/
	uint8	FluxMethod;			/*����ģ��*/
	uint8	TempTagID;			/*������¶�ͨ��ʱ����Ӧ��RDB����ID*/
	
	uint8	PressTagID;			/*������¶�ͨ��ʱ����Ӧ��RDB����ID*/
	uint8	Flux_KPoint;			/*����ϵ��K��С�� λ��*/
	uint8	RatedPress_Point;	/*�ѹ��С����*/
	uint8	Flux_MPoint;		/*�����ܶ�С��λ��*/
	
	int16	Flux_M;				/*�����ܶ� */
	int16	Flux_K;	     			/*����ϵ�� K*/
	int16	RatedPress;			/*�ѹ��*/
	int16	RetadTemp;			/*��¶�*/
	
	uint8	RetadTemp_Point;	/*��¶�С����*/	
	uint8	FixedPress_Point;	/*ѹ����ֵС��*/
	uint8	CompressZf_Point;	/*ѹ��ϵ��Zf С��λ�� */
	uint8	FixedTemp_Point;	/*�¶ȶ�ֵ */
	
	int16	FixedPress;			/*ѹ����ֵ*/
	int16	FixedTemp;			/*�¶ȶ�ֵ */

	int16	CompressZf;			/*ѹ��ϵ��Zf */
	int16	CompressZn;		/*ѹ��ϵ��Zn*/

	uint8	CompressZn_Point;	/*ѹ��ϵ��ZnС��λ�� */	
	uint8	FluxType; 			/*��������*/
	uint8	Flux_Point;			/*��������С����*/
	uint8	FluxUnit;			/*������λ*/

       uint8       SquareEnable;		/*�Ƿ񿪷�*/
	uint8	OutPut_Tagid;		/*���RDB ������*/
	uint8	Reserve[2];

	int16	FluxRangeh;			/*������������*/
	int16	FluxRangel;			/*������������*/

}_FluxMod;
/*PID �����㷨�ṹ����*/
typedef struct 			/*36Byte*/
{	
	uint16	KP;					/*����ϵ��*/	
	uint16	TI;					/*����ʱ��*/
	
	uint8	KP_Point;			/* ����ϵ��С����λ�� ��1~5*/
	uint8	TI_Point;			/*����ʱ��С��λ��*/
	uint8	IS_Point;			/*���ַ����С����λ����3  */
	uint8	TD_Point;			/*΢��ʱ��С��λ��*/
	
	uint16	TD;					/*΢��ʱ��*/
	uint16	IS;					/*���ַ���,0~100 �ٷ���*/
		
	int16	SV;					/*�趨ֵ,������*/
	uint16	DV;					/*����,0~1�ٷ���*/
	
	uint8	SV_Point;			/*�趨ֵ��С����λ����3  */
	uint8	DV_Point;			/* ������С����λ����3 */
	uint8	Outrange_Point;		/* ���С����λ����3 */
	uint8	OriginalPos;			/*��λ��ֵ 0������  1���̶�ֵ  */
	
	uint16	Outrangeh;			/* ������� 0~100*/	
	uint16	Outrangel;			/*������ޣ�0~100  */
	

	uint16	ConstPos;			/*�̶�ֵ��0~1  */
	uint16	SetPos;				/*���ֵ  */
	
	uint8	ConstPos_P;			/*�̶�ֵ��С����λ����3  */
	uint8	PidMode;			/*�ֲ����ķ�ʽ 0���ֶ� 1���Զ�  */
	uint8	PosorNeg;			/*�������ã�0�������� 1��������  */
	uint8	SV_Mode;			/* ����ģʽ��0���ڸ�����1������� */

	int16	SV_K;
	uint8	SV_K_P;
	uint8	Reserve1;
	
	int16 	SV_b;
	uint8	SV_b_P;
	uint8	Reserve2;
	
	uint8	SV_TagID;			/*�����ͨ��   */
	uint8	PV_TagID;			/*����ͨ��   */
	uint8	Enable;
	uint8	MV_TagID;			/*���ͨ��*/

	uint8	Name[11];			/*��·����*/
	uint8       Reserve3[5];
}_PidMod;
typedef struct
{
	uint8	PV_TagID;			/*����ͨ��*/
	uint8	MV_TagID;			/*���ͨ��*/
	int16	Rangeh;				/*���Ͳ���ͨ��������*/
	int16	Rangel;				/*���Ͳ���ͨ��������*/
	uint8	PosorNeg;			/*����������*/
	uint8	Enable;	
}_TransformMod;
typedef	struct 
{
	float   	f_KP;		//�����ȸ���
	float		f_TI;
	float		f_TD;
	float		f_IS;
	float		f_SV;
	float		f_DV;
	float		f_Outrangeh;
	float		f_Outrangel;
	float		f_ConstPos;
	float		f_SetPos;
	float		f_SV_K;
	float		f_SV_b;
	
	float	f_EV;		// ƫ�� 
	float	f_EV1;		// �ϴ�ƫ�� 
	float	f_EV2;		// ���ϴ�ƫ�� 
	float	f_UV;		// ������ʱֵ 
	float	f_SM;		// �����ۻ�ַ 
	float	f_AV;		// ����
	float f_PV;		//����ֵ
}_PIDApp;
 typedef struct			/*12byte*/
{	uint8	TagID;
	uint8	FilterTime;			/*�˲�ʱ��*/
	uint16	OldValue;			/*�˲�ǰһ�β���ֵ*/
	uint8	SmallSignal;			/*С�ź��г�*/
	uint8       reveserve;
	int16	ZeroOffset_K;		/*���Ǩ��K*/
	int16	ZeroOffset_B;		/*���Ǩ��b*/
	uint8	Reserve[2];	
}_FilterMod;

//=============================================================================

extern	void RDBInit(uint8 *baseaddr);

#endif
