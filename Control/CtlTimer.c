#include "CtlTimer.h"
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "sdhDef.h"
#include "system.h"
#include "ModelFactory.h"
#include "utils/Storage.h"
#include "arithmetic/bit.h"
#include "channel_accumulated.h"
#ifdef NO_ASSERT
#include "basis/assert.h"
#else
#include "assert.h"
#endif
//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
//�����Ե�ִ������Ĺ���
//���ڶ�ʱ����������
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------

static uint8_t	chn_smp_time[8];			//ÿ��ͨ���Ĳɼ�ʱ�����
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static void Init_ctime( Controller *self, void *arg);


static void Ctime_periodic (void const *arg);

static osTimerId ctime_id;                                           // timer id
static osTimerDef (ctime, Ctime_periodic);
void Ctime_Allco_time(uint16_t  all_time, uint8_t need);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//



CTOR( CtlTimer)
SUPER_CTOR( Controller);
FUNCTION_SETTING( Controller.init, Init_ctime);
END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{

static void Init_ctime( Controller *self, void *arg)
{
	
	int	ret = RET_OK;
	CtlTimer	*cthis = SUB_PTR( self, Controller, CtlTimer);
	osStatus status;                                              // function return status
	ctime_id = osTimerCreate (osTimer(ctime), osTimerPeriodic, self);
  if (ctime_id != NULL) {    // Periodic timer created
    // start timer with periodic 1000ms interval
    status = osTimerStart (ctime_id, 1000);            
    if (status != osOK) {
      // Timer could not be started
			ret = ERR_OPT_FAILED;
    }
  } 
	else 
	{
		ret = ERR_OPT_FAILED;
		
	}
	
	
	
	assert(ret == RET_OK);
	Ctime_Allco_time(aci_sys.sys_conf.record_gap_s, NUM_CHANNEL);
	cthis->time_count = 0;
	time_smp = 0;
	
}

//ÿ��ִ��һ��
static void Ctime_periodic (void const *arg) 
{
  // add user code here
	CtlTimer	*cthis = SUB_PTR( arg, Controller, CtlTimer);
	Model 		*p_md;
	Storage		*stg = Get_storage();
	do_out_t		d;
	char			chn_name[7];
	char			i;
	
	cthis->time_count ++;
//	p_md = ModelCreate("time");
//	p_md->run(p_md);
//	
//	if(aci_sys.save_chg_flga & CHG_SYSTEM_CONF)
//	{
//		stg->wr_stored_data(stg, STG_SYS_CONF, &aci_sys.sys_conf, sizeof(aci_sys.sys_conf));
//		
//		aci_sys.save_chg_flga &= ~CHG_SYSTEM_CONF;
//		Ctime_Allco_time(aci_sys.sys_conf.record_gap_s, NUM_CHANNEL);
//	}
//	for(i = 0; i < NUM_CHANNEL; i++)
//	{
//		if(aci_sys.save_chg_flga & CHG_MODCHN_CONF(i))
//		{
//			
//			
//			
//			stg->wr_stored_data(stg, STG_CHN_CONF(i), NULL, 0);
//			aci_sys.save_chg_flga &= ~CHG_MODCHN_CONF(i);
//			
//		}
//		
//		
//	}
	
	
	
	
	
	
	
	if((cthis->time_count % 30) == 0)
		MdlChn_Read_code_end_temperature();

	for(i = 0; i < NUM_CHANNEL; i++)
	{
		if(chn_smp_time[i] != time_smp)
			continue;
		sprintf(chn_name,"chn_%d", i);
		p_md = ModelCreate(chn_name);
		p_md->run(p_md);
		
	}
	
	
	CNA_Run(1000);
	
	if(time_smp < aci_sys.sys_conf.record_gap_s)
	{
		time_smp ++;
	} 
	else
		time_smp = 0;
	
	for(i = 0; i < MAX_TOUCHSPOT; i++)
	{
		d.do_chn = i;
		if(aci_sys.DO_err & (1 << i))
		{
			
			d.val = 1;
			p_md->setMdlData(p_md, DO_output, &d);
		}
		else if(aci_sys.DO_val & (1 << i))		//�������
		{
			d.val = 0;
			p_md->setMdlData(p_md, DO_output, &d);
		}
		
	}
	aci_sys.DO_err = 0;
	
	
}

//todo:180121 Ŀǰ��ʵ�ַ�ʽ��ֻ����6ͨ���������
//��ÿ��ͨ���Ĳɼ�ʱ���ɢ����ÿ�εĴ����ɾ����ܵ�ƽ��
void Ctime_Allco_time(uint16_t  all_time, uint8_t need)
{
	
	int i = 0; 
	
	switch(all_time)
	{
		case 0:
		case 1:
			for(i = 0; i < 8; i++)
				chn_smp_time[i] = 0;
			break;
		case 2:
			chn_smp_time[0] = 0;
			chn_smp_time[1] = 0;
			chn_smp_time[2] = 0;
			chn_smp_time[3] = 1;
			chn_smp_time[4] = 1;
			chn_smp_time[5] = 1;
			break;
		case 3:
		case 4:
		case 5:
			chn_smp_time[0] = 0;
			chn_smp_time[1] = 0;
			chn_smp_time[2] = 1;
			chn_smp_time[3] = 1;
			chn_smp_time[4] = 2;
			chn_smp_time[5] = 2;
			break;
		default:
			chn_smp_time[0] = 0;
			chn_smp_time[1] = 1;
			chn_smp_time[2] = 2;
			chn_smp_time[3] = 3;
			chn_smp_time[4] = 4;
			chn_smp_time[5] = 5;
			break;
			
		
		
		
	}
	
	
}
