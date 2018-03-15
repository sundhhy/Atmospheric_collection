/*
*********************************************************************************************************
*                                               				SunyVPR130-RG
* ��	     ��:V1.0									
* �޸�����:2007.1.19
* ��           ��: ConfigInfo.h
* ��           ��: ������Ϣ
* ��           д: fj
********************************************************************************************************
*/
#ifndef	CONFIGINFO_H
#define	CONFIGINFO_H
/*
**************************************************************************************************
*										ͷ�ļ�����
**************************************************************************************************
*/
#include "RDB.h"
#include "Func.h"
/*
**************************************************************************************************
*										��������
**************************************************************************************************
*/

#define	RDBBASEADDR		(CONFIGURATIONADDR)		/*�����д�Ų�����Ϣ����ʼ��ַ*/
#define	SIZE_OF_SYSPARA	(sizeof(_SysParameter))
#define	SIZE_OF_GROUP		(sizeof(_CurveGroup)*4)
#define	SIZE_OF_ALARMLINEINFO		(sizeof(_AlarmLine_Info)*4)
/*
**************************************************************************************************
*										���Ͷ���
**************************************************************************************************
*/
/*ϵͳ��Ϣ����,��FRAM��Ϊϵͳ����Ԥ����44�ֽڵĿռ�,�ýṹ��С���ܲ���44�ֽ�*/
typedef struct 
{
	uint8	SysRom;		/*�Ǳ�FLASH����*/
	uint8	SysAlm;			/*������������*/
	uint8 	SysNum;		/*�Ǳ�����ͨ����*/
	uint8	SysAddr;		/*�Ǳ�ͨѶ��ַ*/
	uint8	CommBPS;		/*ͨѶ������*/
	uint8	ComType;		/*ͨѶ��ʽ*/
	uint8	Parity;			/*У�鷽ʽ*/
	uint8 	Tcode[3];		/*�û�����*/	
	uint8 	SysTD;			/*�û��趨��ͨ����*/
	uint8 	SysDBD;			/*�û��趨�ĶԱȶ�*/
	uint8	ScaleType;		/*��¼���*/
	uint8	RecordCycle;
	uint8	Cool;			/*����¶�*/
	uint8	BreakTC;		/*��ż����ʽ*/
	uint8	BreakRTD;		/*���账��ʽ*/
	uint8	USBFLAG;		/*�Ƿ���USB ����*/
	uint8	PrintFLAG;		/*�Ƿ��д�ӡ����*/
	uint8	SumFLAG;		/*�Ƿ��ۻ�����*/
	uint8	PIDFLAG;		/*�Ƿ���PID���ڹ���*/
	uint8	PID_SET_Enable;/*�Ƿ�������ڲ����޸�*/
	uint8       CoolMode;           /*��˲�����ʽ0:�ⲿ  1:�趨*/
	uint8       SoundEnable;      /*��������0:��ֹ  1:����*/
	uint8       GroupSwitchCycle;  /*���������Զ��л�ʱ��*/
	uint8       ComStatusDisEnable;/*ͨѶ״̬��ʾ����*/
	uint8       Ocode[3];            /*������̬����*/
	uint8       reverseee;
	uint16	SysCRC;			/*У����*/

}_SysParameter;
typedef struct {
	uint8 	UsbStatu;		/*״̬ 0:ֹͣ 1:���ڴ���*/
	uint8 	StarCh;			/*��ʼͨ����*/
	uint8 	EndCh;			/*��ֹͨ����*/
	uint8 	NowCh;			/*��ǰͨ����*/
	uint32 	StarDptr;		/*��ǰͨ������������ʼָ��*/
	uint32 	EndDptr;		/*��ǰͨ������������ָֹ��*/
	uint32 	NowDptr;		/*��ǰָ��*/
	uint16 	ProgressNum;	/*������״̬0-200*/
	Time	StatTime;		/*��ʼʱ��*/
	Time	EndTime;		/*��ֹʱ��*/
	uint32 	AddTimes;		/*�ܹ�����Ҫ�Ĵ��ʹ���*/
	uint32 	CurTimes;		/*��ǰ�Ĵ��ʹ���*/
	uint8 	FristFlag;		/*��һ�δ��ͱ�־*/
	uint8 	UsbIrq;			/*usb�жϱ�־*/
	uint8 	Finish;			/*��ɵİٷ���*/
	uint8 	ErrStatu;    		/*usb����״̬*/
	uint32     Length;			/*ͨ��������*/
	uint8 	InitWait;		/*USB��ʼ����ʱ*/
	uint8 	SecondFlag;
	uint8  	fInOut ;
	uint8  	Step;
	uint32  	TotalLength;
	uint8	FileName[12];	
}_UsbParameter;
typedef struct
{
	Time  	StartTime;             /*��ʼ��ӡʱ��*/
	Time  	EndTime;               /*��ֹ��ӡʱ��*/
	Time	NowTime;              /*��ǰ��ӡʱ��*/
	Time	PrintTime;             /*��ӡʱ��*/
	uint32     StartDptr;             /*��ǰͨ���������ݵ���ʼָ��*/
	uint32     EndDptr;                /*��ǰͨ������������ָֹ��*/
	uint32     NowDptr;              /*��ǰָ��*/
	uint8       PrintStatus;         /*0:ֹͣ  1:���ڴ�ӡ  2:Ҫ��ֹͣ 3:Ҫ���ӡ// 4:����ӡ*/								
	uint8       PrintType;	        /*0:����  1:ͼ��   2������*/
	uint8       CurveFlag;	        /*0~2:��һ�λ�����  3:�ǵ�һ�λ�����*/
	uint8       PrintTimeNum;	/*��ӡʱ��ĵڼ�λ  1~8   0 : ֹͣ*/
	uint16	LastValue;
	uint8       Printdata[6];
	uint8       PrintCycle;              /*��ӡ���*/
	uint8       PrintProgressNum;   /*��ӡ����*/
	uint8       PrintTagid;              /*��ӡͨ��*/
}_PrintParameter;

 typedef struct	
{
	 _RdbFile_Head 			RdbFile_Head;
	 _RdbBase_Record			RdbBase_Record[RDB_BASE_MAXNUM];
	 _RdbSample_Record 		RdbSample_Record[RDB_SAMPLE_MAXNUM];
	 _RdbRecord_Record 		RdbRecord_Record[RDB_RECORD_MAXNUM];
	 _RdbAlarm_Record		RdbAlarm_Record[RDB_ALARM_MAXNUM];
	 _SumMod				SumMod[FUN_SUMMOD_MAXNUM];
	 _FluxMod				FluxMod[FUN_FLUXMOD_MAXNUM];
	 _PidMod					PidMod[FUN_PIDMOD_MAXNUM];
	 _FilterMod				FilterMod[FUN_FILTERMOD_MAXNUM];
	 _TransformMod			TranformMod[FUN_TRANFORMMOD_MAXNUM];
}_Configuration;
typedef struct
{
  uint8 Dispcolor[4];		/*�������Ա��ʾ����ɫ*/
  uint8 Groupmember[4];	/*�������Ա*/
  uint8 DispOrNot[4];  		/*�������Ա�Ƿ�����1:��ʾ   0:����*/
} _CurveGroup;

typedef struct __AlarmLine_Info
{
	short AlarmLine_HH;
	short AlarmLine_H;
	short AlarmLine_L;
	short AlarmLine_LL;
	unsigned char Color_HH;
	unsigned char Color_H;
	unsigned char Color_L;
	unsigned char Color_LL;
	unsigned char LineThick_HH;
	unsigned char LineThick_H;
	unsigned char LineThick_L;
	unsigned char LineThick_LL;
}_AlarmLine_Info;


typedef struct
{
 uint8 Out_SysNum;
 uint8 Out_SysRom;
 uint8 Out_SysAlm;
 uint8 Out_PIDFLAG;
 uint8 Out_SumFLAG;
 uint8 Out_PrintFLAG;
 uint8 Out_USBFLAG;
 uint8  reverse;
} _OutConfig;
/*
**************************************************************************************************
*										�����ⲿʹ�ñ�������
**************************************************************************************************
*/
extern	_Configuration		ConfigPara;
extern	_RdbFile_Head 		*RdbFile_Head;
extern	_RdbBase_Record		*RdbBase_Record[RDB_BASE_MAXNUM];
extern	_RdbSample_Record 	*RdbSample_Record[RDB_SAMPLE_MAXNUM];
extern	_RdbRecord_Record 	*RdbRecord_Record[RDB_RECORD_MAXNUM];
extern	_RdbAlarm_Record		*RdbAlarm_Record[RDB_ALARM_MAXNUM];
extern	_SumMod			*SumMod[FUN_SUMMOD_MAXNUM];
extern	_FluxMod				*FluxMod[FUN_FLUXMOD_MAXNUM];
extern	_PidMod				*PidMod[FUN_PIDMOD_MAXNUM];
extern	_FilterMod			*FilterMod[FUN_FILTERMOD_MAXNUM];
extern	_TransformMod		*TranformMod[FUN_TRANFORMMOD_MAXNUM];
extern	_SumRecord			SumRecord[FUN_SUMMOD_MAXNUM];
extern	_SysParameter		SysPara;
extern	_UsbParameter		UsbPara;
extern	_PrintParameter            PrintPara;
extern	Time				SysTime;
extern	Time				PowerDownTime;
extern	_CurveGroup             	CurveGroup[4];

extern _AlarmLine_Info    g_AlarmLine_Info[4];

extern     _OutConfig                   OutConfig;
/*
**************************************************************************************************
*										�����ⲿʹ�ú�������
**************************************************************************************************
*/
extern	void  SaveConfigPara(void);
extern     uint8  CompareRDBPara(void);
extern	void MakeDefaultConfiguration(void);
#endif


