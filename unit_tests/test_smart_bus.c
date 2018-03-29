//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
#include <string.h>
#include <stdio.h>

#include "os/os_depend.h"

#include "configs/tdd_conf.h"
#include "configs/lcd_conf.h"
#include "unit_test.h"
#include "device.h"
#include "utils/keyboard.h"
#include "Model.h"
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#if TDD_ON == 1
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
#define TSB_PAGE_MAX_NUM		7
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------
static struct {
	
	uint8_t		test_sig_type;
	uint8_t		test_page;
	uint8_t		none[2];
}mdl_test;


//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static KbTestOb 				*p_kbTestOb;

static dev_lcd 		*p_lcd;

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------

static void TSB_Display(void *arg);

static int 		KeyEvent( char num, keyMsg_t arr_msg[]);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
void Test_smart_bus(void)
{
	
	Keyboard		*p_kb;
	
	
	//按键初始化
	p_kb = GetKeyInsance();
	
	//借用一下内存:&aci_sys.lcd_cmd_bytes
	aci_sys.lcd_cmd_bytes = CONF_KEYSCAN_CYCLEMS;
	p_kb->init( p_kb, &aci_sys.lcd_cmd_bytes);
	aci_sys.lcd_cmd_bytes  = 0;

	p_kbTestOb = KbTestOb_new();
	
	
	Dev_open(DEVID_FM12864, (void *)&p_lcd);
	
	
	p_kbTestOb->setKeyHdl( p_kbTestOb, KeyEvent);
	p_kb->addOb( p_kb, ( keyObservice *)p_kbTestOb);
	osKernelStart ();                         // start thread execution 
	
	mdl_test.test_sig_type = em_atmosphere_A;
	mdl_test.test_page = 0;
	MDL_Attach(em_atmosphere_A, NULL, TSB_Display);
	MDL_Attach(em_atmosphere_B, NULL, TSB_Display);
	MDL_Attach(em_dust, NULL, TSB_Display);
	
	TSB_Display(NULL);

	while(1)
	{
		p_kb->run( p_kb);
		delay_ms( 200);
		MDL_Run(200);
		p_lcd->lcd_flush(0);
	}
	
	
}
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{


static int KeyEvent( char num, keyMsg_t arr_msg[])
{
	int i = 0;

	for( i = 0; i < num; i++)
	{

		switch(arr_msg[i].keyCode)
		{
			case KEYCODE_SWITCH:
				mdl_test.test_sig_type ++;
				
				if(mdl_test.test_sig_type >= em_signal_num)
					mdl_test.test_sig_type = em_atmosphere_A;
			
				TSB_Display(NULL);
				break;
			case KEYCODE_UP:
				if(mdl_test.test_page < TSB_PAGE_MAX_NUM)
					mdl_test.test_page ++;
				else
					mdl_test.test_page = 0;
				
				
				TSB_Display(NULL);
				break;
			case KEYCODE_DOWN:
				if(mdl_test.test_page > 0)
					mdl_test.test_page --;
				else
					mdl_test.test_page = TSB_PAGE_MAX_NUM;
				
				TSB_Display(NULL);
				break;
		}
	}

	return 0;

}



static void TSB_Display(void *arg)
{
	char		tsb_buf[64];
	
	p_lcd->clear(PALLET_WHITE);
	sprintf(tsb_buf, "AI [%d/%d]", mdl_test.test_page + 1, TSB_PAGE_MAX_NUM + 1);
	p_lcd->dispaly_text(0, tsb_buf, 0, 0, 0, FONT_12, PALLET_BLACK);
	p_lcd->dispaly_text(0, (char *)str_signal[mdl_test.test_sig_type], 0, 72, 0, FONT_12, PALLET_BLACK);
	
	switch(mdl_test.test_page)
	{
		case 0:
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Address);
			p_lcd->dispaly_text(0, "Addr", 0, 0, 16, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 16, FONT_12, PALLET_BLACK);
		
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Type);
			p_lcd->dispaly_text(0, "Type", 0, 0, 32, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 32, FONT_12, PALLET_BLACK);
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Status);
			p_lcd->dispaly_text(0, "Status", 0, 0, 48, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 48, FONT_12, PALLET_BLACK);
			break;
		case 1:
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].CMD);
			p_lcd->dispaly_text(0, "CMD", 0, 0, 16, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 16, FONT_12, PALLET_BLACK);
		
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].FLOW1);
			p_lcd->dispaly_text(0, "FLOW1", 0, 0, 32, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 32, FONT_12, PALLET_BLACK);
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].FLOW2);
			p_lcd->dispaly_text(0, "FLOW2", 0, 0, 48, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 48, FONT_12, PALLET_BLACK);
		
			break;
		case 2:
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].FLOW3);
			p_lcd->dispaly_text(0, "FLOW3", 0, 0, 16, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 16, FONT_12, PALLET_BLACK);
		
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Q0);
			p_lcd->dispaly_text(0, "Q0", 0, 0, 32, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 32, FONT_12, PALLET_BLACK);
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Q1);
			p_lcd->dispaly_text(0, "Q1", 0, 0, 48, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 48, FONT_12, PALLET_BLACK);
		
			break;	
		case 3:
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Q2);
			p_lcd->dispaly_text(0, "Q2", 0, 0, 16, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 16, FONT_12, PALLET_BLACK);
		
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Q3);
			p_lcd->dispaly_text(0, "Q3", 0, 0, 32, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 32, FONT_12, PALLET_BLACK);
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Temp);
			p_lcd->dispaly_text(0, "Temp", 0, 0, 48, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 48, FONT_12, PALLET_BLACK);
		
			break;		
		case 4:
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].PWM);
			p_lcd->dispaly_text(0, "PWM", 0, 0, 16, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 16, FONT_12, PALLET_BLACK);
		
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].FLOW2_K1);
			p_lcd->dispaly_text(0, "FLOW2_K1", 0, 0, 32, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 32, FONT_12, PALLET_BLACK);
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].FLOW2_K2);
			p_lcd->dispaly_text(0, "FLOW2_K2", 0, 0, 48, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 48, FONT_12, PALLET_BLACK);
		
			break;	
		case 5:
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].FLOW2_K3);
			p_lcd->dispaly_text(0, "FLOW2_K3", 0, 0, 16, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 16, FONT_12, PALLET_BLACK);
		
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].FLOW2_K4);
			p_lcd->dispaly_text(0, "FLOW2_K4", 0, 0, 32, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 32, FONT_12, PALLET_BLACK);
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].FLOW2_B);
			p_lcd->dispaly_text(0, "FLOW2_B", 0, 0, 48, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 48, FONT_12, PALLET_BLACK);
		
			break;	
		case 6:
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Q1_K);
			p_lcd->dispaly_text(0, "Q1_K", 0, 0, 16, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 16, FONT_12, PALLET_BLACK);
		
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Q1_B);
			p_lcd->dispaly_text(0, "Q1_B", 0, 0, 32, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 32, FONT_12, PALLET_BLACK);
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Q2_K);
			p_lcd->dispaly_text(0, "Q2_K", 0, 0, 48, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 48, FONT_12, PALLET_BLACK);
		
			break;	
		case 7:
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Q2_B);
			p_lcd->dispaly_text(0, "Q2_B", 0, 0, 16, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 16, FONT_12, PALLET_BLACK);
		
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Temp_K);
			p_lcd->dispaly_text(0, "Temp_K", 0, 0, 32, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 32, FONT_12, PALLET_BLACK);
		
			sprintf(tsb_buf, "%x",arr_sample_val[mdl_test.test_sig_type].Temp_B);
			p_lcd->dispaly_text(0, "Temp_B", 0, 0, 48, FONT_12, PALLET_BLACK);
			p_lcd->dispaly_text(0, tsb_buf, 0, 80, 48, FONT_12, PALLET_BLACK);
		
			break;	
	
	}
	
}

#endif
