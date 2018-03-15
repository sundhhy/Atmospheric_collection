/*
*********************************************************************************************************
*                                               				SunyVPR130-RG
* ��	     ��:V1.0									
* �޸�����:2007.1.19
* ��           ��: ConfigInfo.c
* ��           ��: ������Ϣ
* ��           д: fj
********************************************************************************************************
*/
/*
**************************************************************************************************
*										ͷ�ļ�����
**************************************************************************************************
*/
#include "ConfigInfo.h"

	
/*
*****************************************************************************************************************
*										��������
*****************************************************************************************************************
*/
_Configuration		ConfigPara;
_RdbFile_Head 		*RdbFile_Head;
_RdbBase_Record		*RdbBase_Record[RDB_BASE_MAXNUM];
_RdbSample_Record 	*RdbSample_Record[RDB_SAMPLE_MAXNUM];
_RdbRecord_Record 	*RdbRecord_Record[RDB_RECORD_MAXNUM];
_RdbAlarm_Record		*RdbAlarm_Record[RDB_ALARM_MAXNUM];
_SumMod			*SumMod[FUN_SUMMOD_MAXNUM];
_FluxMod				*FluxMod[FUN_FLUXMOD_MAXNUM];
_PidMod				*PidMod[FUN_PIDMOD_MAXNUM];
_FilterMod			*FilterMod[FUN_FILTERMOD_MAXNUM];
_TransformMod		*TranformMod[FUN_TRANFORMMOD_MAXNUM];
_SumRecord			SumRecord[FUN_SUMMOD_MAXNUM];
_SysParameter		SysPara;
_UsbParameter		UsbPara;
_PrintParameter            PrintPara;
Time				SysTime;
Time				PowerDownTime;
_CurveGroup       CurveGroup[4];
_AlarmLine_Info   g_AlarmLine_Info[4];
_OutConfig                   OutConfig;


/*
**************************************************************************
* �������ƣ�SaveConfigPara
* �����������
* �����������
* ����������������̬��Ϣ��FLASH
**************************************************************************
*/
void	SaveConfigPara(void)
{ 
	//Ϊ���ڲ�FLASH �����8K �Ŀռ�Ҳ������Ŵ��룬
	//��������Ŀռ䵽32K������̬��Ϣ���浽��������8K ��λ�ô�
	SPIWrite(CONFIGURATIONADDR, (uint8 *)&ConfigPara, SIZE_OF_CONFIGURATION);
}

/*
**************************************************************************
* �������ƣ�CompareRDBPara
* �����������
* ���������0:��ͬ  1:��ͬ2:����
* �����������Ƚ�RDB����,����RDB����ʵʱ�����Ӧ����
**************************************************************************
*/
uint8  CompareRDBPara(void)
{ 
   	uint32 i;
	uint32 NumOfDiff=0;
	_Configuration  ConfigPara1;
	uint32 *srcFLASH,*cmpRAM;
	
	SPIRead(CONFIGURATIONADDR, (uint8*)&ConfigPara1,SIZE_OF_CONFIGURATION);
	srcFLASH=(uint32*)&ConfigPara1;
	cmpRAM=(uint32*)&ConfigPara;
	
	for(i=0;i<SIZE_OF_CONFIGURATION/4;i=i+1)
	{/*��ȡ�ڴ��к��ڲ�FLASH�е�RDB������ͬ�������ҽ��ڲ�FLASH��RDB�����������ڴ���*/		
		if(*(cmpRAM+i)!=*(srcFLASH+i))  
			NumOfDiff++;
	}
	
	if(!NumOfDiff)   return 0;
	
	for(i=0;i<RDB_BASE_MAXNUM;i++)
	 {
		if(ConfigPara.RdbBase_Record[i].Value!=ConfigPara1.RdbBase_Record[i].Value)
		{
			if(NumOfDiff)
			NumOfDiff--;
			else return 2;
		}
		if(ConfigPara.RdbBase_Record[i].HisValue!=ConfigPara1.RdbBase_Record[i].HisValue)
		{
			if(NumOfDiff)
			NumOfDiff--;
			else return 2;
		}
	}
	
	for(i=0;i<RDB_SAMPLE_MAXNUM;i++)
	{
		if(ConfigPara.RdbSample_Record[i].Status!=ConfigPara1.RdbSample_Record[i].Status)
		{
			if(NumOfDiff)
			NumOfDiff--;
			else return 2;
		}
		if(ConfigPara.RdbSample_Record[i].SampleValue!=ConfigPara1.RdbSample_Record[i].SampleValue)
		{
			if(NumOfDiff)
			NumOfDiff--;
			else return 2;
		}
	}
	
	for(i=0;i<FUN_PIDMOD_MAXNUM;i++)
	{
		if(ConfigPara.PidMod[i].SetPos!=ConfigPara1.PidMod[i].SetPos)
		{
			if(NumOfDiff)
			NumOfDiff--;
			else return 2;
		}
	}
	
	for(i=0;i<FUN_FILTERMOD_MAXNUM;i++)
	{
		if(ConfigPara.FilterMod[i].OldValue!=ConfigPara1.FilterMod[i].OldValue)
		{
			if(NumOfDiff)
			NumOfDiff--;
			else return 2;
		}
	}

	if(NumOfDiff)  return 1;
	else return 0;
}


/*
**************************************************************************
* �������ƣ�MakeDefaultConfiguration
* �����������
* �����������
* ����������Ĭ�ϵ���̬��Ϣ
**************************************************************************
*/
void MakeDefaultConfiguration(void)
{
	uint8	i,*buf;
	uint8     *p;
	uint32	j;
	buf=(uint8 *)&ConfigPara;
	for(j=0;j<SIZE_OF_CONFIGURATION;j++) *buf++=0;	
	ConfigPara.RdbFile_Head.RdbNumber=16;
	ConfigPara.RdbFile_Head.AlarmNumber=16;
	ConfigPara.RdbFile_Head.SampleNumber=16;
	ConfigPara.RdbFile_Head.RecordNumber=16;
	for(i=0;i<RDB_BASE_MAXNUM;i++)
	{
              p=(uint8*)"ͨ��";
		ConfigPara.RdbBase_Record[i].TagID=i;	
		ConfigPara.RdbBase_Record[i].Name[0]=(uint8)*(p);
		ConfigPara.RdbBase_Record[i].Name[1]=(uint8)*(p+1);
		ConfigPara.RdbBase_Record[i].Name[2]=(uint8)*(p+2);
		ConfigPara.RdbBase_Record[i].Name[3]=(uint8)*(p+3);
		ConfigPara.RdbBase_Record[i].Name[4]=(i+1)/10+48;
		ConfigPara.RdbBase_Record[i].Name[5]=(i+1)%10+48;
		ConfigPara.RdbBase_Record[i].Name[6]='\0';
		ConfigPara.RdbBase_Record[i].Name[7]='1';
		ConfigPara.RdbBase_Record[i].Name[8]='1';
		ConfigPara.RdbBase_Record[i].UnitName[0]='1';
		ConfigPara.RdbBase_Record[i].UnitName[1]='1';
		ConfigPara.RdbBase_Record[i].UnitName[2]='1';
		ConfigPara.RdbBase_Record[i].UnitName[3]='1';
		ConfigPara.RdbBase_Record[i].UnitName[4]='1';
		ConfigPara.RdbBase_Record[i].UnitName[5]='\0';
		
		//========= ZP �޸�==================
		ConfigPara.RdbBase_Record[i].Point=3;
		ConfigPara.RdbBase_Record[i].MaxEVal=8500;
		ConfigPara.RdbBase_Record[i].MinEVal=-999;
		ConfigPara.RdbBase_Record[i].CommentInfoID = 19;
		//===================================
		
		//===========ZP �޸�================
		ConfigPara.RdbSample_Record[i].TagId=i;
		ConfigPara.RdbSample_Record[i].SingalType=Pt100Type;
		ConfigPara.RdbSample_Record[i].AppendInfo3=i;
		//================================
		
		ConfigPara.RdbRecord_Record[i].TagId=i;
		ConfigPara.RdbRecord_Record[i].RecordCycle=1;
		ConfigPara.RdbRecord_Record[i].RecordLen=8;
		//================================
		
		//========= ZP �޸�==================
		ConfigPara.RdbAlarm_Record[i].HiHiLimit=8500;
		ConfigPara.RdbAlarm_Record[i].HiLimit=8500;
		ConfigPara.RdbAlarm_Record[i].LoLoLimit=-999;
		ConfigPara.RdbAlarm_Record[i].LoLimit=-999;
		ConfigPara.RdbAlarm_Record[i].TagId=i;
		//================================
		
		ConfigPara.PidMod[i].Enable=0;
		ConfigPara.SumMod[i].Enable=0;
		ConfigPara.RdbBase_Record[i].DispColor=i;
		
		
	}
	


	for(i=0;i<FUN_FILTERMOD_MAXNUM;i++)
	{
		ConfigPara.FilterMod[i].FilterTime=0;
		ConfigPara.FilterMod[i].ZeroOffset_K=100;
		ConfigPara.FilterMod[i].ZeroOffset_B=0;
		ConfigPara.FilterMod[i].TagID=i;
	}
	for(i=0;i<FUN_FLUXMOD_MAXNUM;i++)
	{
		ConfigPara.FluxMod[i].Enable=0;
		ConfigPara.FluxMod[i].CompMethod=0;
		ConfigPara.FluxMod[i].CompressZf=1000;
		ConfigPara.FluxMod[i].CompressZf_Point=4;
		ConfigPara.FluxMod[i].CompressZn=1000;
		ConfigPara.FluxMod[i].CompressZn_Point=4;
		ConfigPara.FluxMod[i].FixedPress=1;
		ConfigPara.FluxMod[i].FixedPress_Point=3;
		ConfigPara.FluxMod[i].FixedTemp=200;
		ConfigPara.FluxMod[i].FixedTemp_Point=3;
		ConfigPara.FluxMod[i].FluxMethod=0;
		ConfigPara.FluxMod[i].FluxType=0;
		ConfigPara.FluxMod[i].FluxUnit=0;
		ConfigPara.FluxMod[i].Flux_K=1000;
		ConfigPara.FluxMod[i].Flux_KPoint=1;
		ConfigPara.FluxMod[i].Flux_M=1000;
		ConfigPara.FluxMod[i].Flux_MPoint=1;
		ConfigPara.FluxMod[i].FluxRangeh=9999;
		ConfigPara.FluxMod[i].Flux_Point=4;
		ConfigPara.FluxMod[i].FluxRangel=0;
	}
	for(i=0;i<FUN_PIDMOD_MAXNUM;i++)
	{
		ConfigPara.PidMod[i].ConstPos=0;
		ConfigPara.PidMod[i].ConstPos_P=3;
		ConfigPara.PidMod[i].DV=0;
		ConfigPara.PidMod[i].DV_Point=3;
		ConfigPara.PidMod[i].Enable=0;
		ConfigPara.PidMod[i].IS=1000;
		ConfigPara.PidMod[i].IS_Point=3;
		ConfigPara.PidMod[i].KP=1000;
		ConfigPara.PidMod[i].KP_Point=3;
		ConfigPara.PidMod[i].MV_TagID=NOTAGID;
		ConfigPara.PidMod[i].OriginalPos=0;
		ConfigPara.PidMod[i].Outrangeh=1000;
		ConfigPara.PidMod[i].Outrange_Point=3;
		ConfigPara.PidMod[i].Outrangel=0;
		ConfigPara.PidMod[i].PidMode=0;
		ConfigPara.PidMod[i].PosorNeg=0;
		ConfigPara.PidMod[i].PV_TagID=NOTAGID;
		ConfigPara.PidMod[i].SV=0;
		ConfigPara.PidMod[i].SV_Point=3;
		ConfigPara.PidMod[i].SV_b=0;
		ConfigPara.PidMod[i].SV_b_P=3;
		ConfigPara.PidMod[i].SV_K=100;
		ConfigPara.PidMod[i].SV_K_P=2;
		ConfigPara.PidMod[i].SV_Mode=0;
		ConfigPara.PidMod[i].TD=0;
		ConfigPara.PidMod[i].TD_Point=3;
		ConfigPara.PidMod[i].TI=9999;
		ConfigPara.PidMod[i].TI_Point=4;
		ConfigPara.PidMod[i].SV_TagID=NOTAGID;
		ConfigPara.PidMod[i].Name[0]='P';
		ConfigPara.PidMod[i].Name[1]='I';
		ConfigPara.PidMod[i].Name[2]='D';
		ConfigPara.PidMod[i].Name[3]='0';
		ConfigPara.PidMod[i].Name[4]=48+i+1;
		ConfigPara.PidMod[i].Name[5]='\0';


	}

	for(i=0;i<FUN_SUMMOD_MAXNUM;i++)
	{
		ConfigPara.SumMod[i].Enable=0;
		ConfigPara.SumMod[i].TagID=i;
		ConfigPara.SumMod[i].StartDay=1;
		ConfigPara.SumMod[i].StartMonth=1;
		ConfigPara.SumMod[i].StartYear=07;
	}
	for(i=0;i<FUN_TRANFORMMOD_MAXNUM;i++)
	{
		ConfigPara.TranformMod[i].Enable=0;
		ConfigPara.TranformMod[i].MV_TagID=NOTAGID;
		ConfigPara.TranformMod[i].PosorNeg=0;
		ConfigPara.TranformMod[i].PV_TagID=NOTAGID;
		ConfigPara.TranformMod[i].Rangeh=8500;
		ConfigPara.TranformMod[i].Rangel=-999;
	}
	SaveConfigPara();
}

