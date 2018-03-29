#include "Model.h"
#include "sdhDef.h"
#include "device.h"
#include <string.h>

#include "Communication/smart_bus.h"
#include "os/os_depend.h"
#include "system.h"
//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
const Except_T model_Failed = { "MDL Failed" };
const char *str_signal[em_signal_num] = {"<大气A>", "<大气B>", "<粉尘>"};

#define SLAVE_UART				DEVID_UART1
#define MDL_WAIT_SLAVE		10

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------

sample_conf_t	arr_sample_conf[NUM_CHANNEL];

slave_sample_data_t arr_sample_val[em_signal_num];
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
typedef struct {
	
	void 				*arg;
	mdl_cb			func_mdl_change;
	
}mdl_change_hdl_t;
//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
struct {
	uint16_t		mdl_count_ms;
	
	uint16_t		set_update;		//每个位对应一个通道是否采样过了,在初始化和所有的通道都被采样了以后，清零
	
	mdl_change_hdl_t		arr_hdl[em_signal_num];
	
	
}mdl_run_mgr;

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
/* Cycle/Sync Callback functions */

static int MDL_Need_sample(uint8_t st);
static void DoUpdate(void **x, void *cl);
static void MDL_Update_slave_data(uint8_t *buf, short buf_size, uint8_t st);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//







	


void MDL_Clone_samp_conf(sample_conf_t *p_dst, sample_conf_t *p_src)
{
	p_dst->sample_setup_hour = p_src->sample_setup_hour;
	p_dst->sample_setup_min = p_src->sample_setup_min;
	p_dst->sample_gap_hour = p_src->sample_gap_hour;
	p_dst->sample_gap_min = p_src->sample_gap_min;
	p_dst->sample_delay_hour = p_src->sample_delay_hour;
	p_dst->sample_delay_min = p_src->sample_delay_min;
	p_dst->sample_max_count = p_src->sample_max_count;
	
}

void MDL_Init(uint8_t	st)
{
	dev_Char 		*slave_uart = NULL;
	
	Dev_open(SLAVE_UART, ( void *)&slave_uart);
	slave_uart->ioctol(slave_uart, DEVCMD_SET_TXWAITTIME_MS, 100);
	slave_uart->ioctol(slave_uart, DEVCMD_SET_RXWAITTIME_MS, 10);
	
}



void MDL_Get_value(uint8_t	st, uint8_t aux, void	*out_val)
{
	
	sample_conf_t					*p_smc = &arr_sample_conf[st];
	uint32_t							*p_u32 = (uint32_t *)out_val;
	mdl_data_t						*p_mdl_val = (mdl_data_t *)out_val;
	slave_sample_data_t		*p_sample = &arr_sample_val[st];
	switch(aux)
	{
		

		case em_start_time_sec:
			*p_u32 = p_smc->first_sample_hour * 3600 + p_smc->first_sample_min * 60;
			break;
		case em_add_up_time_sec:
			*p_u32 = p_smc->add_up_sample_time_sec;
			break;
		case em_set_up_time_sec:
			*p_u32 = p_smc->sample_setup_hour * 3600 + p_smc->sample_setup_min * 60;
			break;
		case em_sample_time_sec:
			*p_u32 = p_smc->sample_hour * 3600 + p_smc->sample_min * 60;
			break;
		case em_sample_count:
			*p_u32 = p_smc->sample_count;
			break;
		case em_set_up_count:
			*p_u32 = p_smc->sample_max_count;
			break;
		case em_humidity:
			p_mdl_val->mdl_val = p_sample->Temp;
			p_mdl_val->num_point = 2;
			break;
		case em_pressure:
			p_mdl_val->mdl_val = p_sample->Q2;
			p_mdl_val->num_point = 2;
			break;
		case em_atm_pressure:
			p_mdl_val->mdl_val = p_sample->Q0;
			p_mdl_val->num_point = 2;
			break;
		case em_condition:
			p_mdl_val->mdl_val = p_sample->FLOW2;
			p_mdl_val->num_point = 2;
			break;
		case em_standard_conditions:
			p_mdl_val->mdl_val = p_sample->FLOW1;
			p_mdl_val->num_point = 2;
			break;
		case em_set_up_condition:
			p_mdl_val->mdl_val = p_sample->FLOW3;
			p_mdl_val->num_point = 2;
			break;
		case em_condition_volume:
			p_mdl_val->mdl_val = p_sample->FLOW3;
			p_mdl_val->num_point = 2;
			break;
		case em_standard_condition_volume:
			p_mdl_val->mdl_val = p_sample->FLOW3;
			p_mdl_val->num_point = 2;
			break;
		
	}

	
	
}

void MDL_Run(int cycle_ms)
{
	int 			i;
	uint8_t		bus_buf[128];
//	uint8_t		send_status ;		
	//每秒更新从机的采样数据，每次只更新一路
	mdl_run_mgr.mdl_count_ms += cycle_ms;
	
	if(mdl_run_mgr.mdl_count_ms < 1000)
		goto sample;
	mdl_run_mgr.mdl_count_ms = 0;
	for(i = 0; i < em_signal_num; i++)
	{
		if(mdl_run_mgr.set_update == ((1 << em_signal_num) - 1))
		{
			//全部更新过了就重头开始更新
			mdl_run_mgr.set_update = 0;
		}
		
		if((mdl_run_mgr.set_update & (1 << i)))		//已经更新过了就不更新
			continue;
		
		MDL_Update_slave_data(bus_buf, 128, i);
		mdl_run_mgr.set_update |= 1 << i;
		
		break;
		
	}
	
	
	sample:
	//采集处理
	for(i = 0; i < em_signal_num; i++)
	{
		//判断各个通道是否到了启动
		if(MDL_Need_sample(i) != RET_OK)
			continue;
		
			
		
		/*需要采集的话，就进行采集处理 */
		
		
		
		//向从机发送采集命令
		
		//等待从机返回，如果超时则将本次采集作废
		
		//将从机返回的数据给传感器数据处理模块处理
		
		
		
		//更新采集时间和采集次数
		
	}
	
	
	//校零处理
}

//一种信号，只支持一个观察者
void MDL_Attach(uint8_t	st, void *arg, mdl_cb cb)
{
	
	if(st >= em_signal_num)
		return ;
	mdl_run_mgr.arr_hdl[st].arg = arg;
	mdl_run_mgr.arr_hdl[st].func_mdl_change = cb;
}



void Mdl_attach(  Model *self, Observer *s)
{
	
	self->tObs = List_push( self->tObs, s);
	
	
}

void Mdl_detach(  Model *self, Observer *s)
{
	
	
	
}

int Mdl_getData(  Model *self, IN int aux, void *arg) 
{
	memcpy( self->coreData, arg, self->crDt_len);
	return RET_OK;
}

int Mdl_setData(  Model *self, IN int aux,  void *arg) 
{
	memcpy( arg, self->coreData, self->crDt_len);
	return RET_OK;
}


int	Mdl_addTmMdl( Model *self, Model *m)
{
	self->teamMdl = m;
	return RET_OK;

}	

int Mdl_delTmMdl( Model *self, Model *m)
{
	self->teamMdl = NULL;
	return RET_OK;
}
//int Mdl_installDataSource( Model *self, void *dsr)
//{
//	self->dataSource = dsr;
//	return RET_OK;
//}

void Mdl_notify (Model *self)
{
	
	
	
	List_map(self->tObs, DoUpdate, self);
}

ABS_CTOR( Model)
FUNCTION_SETTING( attach, Mdl_attach);
FUNCTION_SETTING( detach, Mdl_detach);
FUNCTION_SETTING( getMdlData, Mdl_getData);
FUNCTION_SETTING( setMdlData, Mdl_setData);
FUNCTION_SETTING( addTmMdl, Mdl_addTmMdl);
FUNCTION_SETTING( delTmMdl, Mdl_delTmMdl);
//FUNCTION_SETTING( installDataSource, Mdl_installDataSource);
FUNCTION_SETTING( notify, Mdl_notify);

END_ABS_CTOR

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{




static int MDL_Need_sample(uint8_t st)
{
	
	sample_conf_t *p_smp_conf = &arr_sample_conf[st];
	struct tm 		*p_cur_tim = &aci_sys.sys_time; 
	
	if(p_smp_conf->is_start == 0)
		goto exit;
	
	/*判断时间有没有到 */
	
	//延时时间要为0才行
		//延时时间的减少，放在定时器任务里面做
	if((p_smp_conf->sample_delay_min) || (p_smp_conf->sample_delay_hour))
		goto exit;

	//当前时间超过设置的采样时间
	if(p_cur_tim->tm_hour > p_smp_conf->sample_gap_hour)
		return RET_OK;
	
	if((p_cur_tim->tm_hour == p_smp_conf->sample_setup_hour) && \
		(p_cur_tim->tm_min >= p_smp_conf->sample_setup_min))
		return RET_OK;
	
	
	
	exit:
	return RET_FAILED;
	
}

//如果采样成功返回0

static void MDL_Update_slave_data(uint8_t *buf, short buf_size, uint8_t st)
{
	int 				pkt_len = 0;
	dev_Char 		*slave_uart = NULL;
	
	Dev_open(SLAVE_UART, ( void *)&slave_uart);
	
	pkt_len = SmBus_Read(st, 0 + st * sizeof(slave_sample_data_t), sizeof(slave_sample_data_t), buf, buf_size);
	
	if(pkt_len <= 0)
		return;
	
	if( slave_uart->write(slave_uart, buf, pkt_len) != RET_OK)
		goto err;
	delay_ms(MDL_WAIT_SLAVE * 2);

	pkt_len = slave_uart->read(slave_uart, buf, buf_size);
	if(pkt_len <= 0)
		goto err;
	if(SmBus_decode(SMBUS_CMD_READ, buf, &arr_sample_val[st], sizeof(slave_sample_data_t)) != RET_OK)
		goto err;

	if(mdl_run_mgr.arr_hdl[st].func_mdl_change == NULL)
		return;
	mdl_run_mgr.arr_hdl[st].func_mdl_change(mdl_run_mgr.arr_hdl[st].arg);
	err:
	 return;
}


static void DoUpdate(void **x, void *cl)
{
	Observer *s = ( Observer *)*x;
	s->update(s, cl);
	
}


