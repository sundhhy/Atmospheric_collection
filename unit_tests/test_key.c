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

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static KbTestOb 				*p_kbTestOb;
static dev_lcd 		*p_lcd;
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------


static int 		KeyEvent( char num, keyMsg_t arr_msg[]);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
void Test_key(void)
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
	p_lcd->clear(PALLET_WHITE);
	p_lcd->dispaly_text(0, "key test", 0, 32, 0, FONT_12, PALLET_BLACK);
	
	p_kbTestOb->setKeyHdl( p_kbTestOb, KeyEvent);
	p_kb->addOb( p_kb, ( keyObservice *)p_kbTestOb);
	osKernelStart ();                         // start thread execution 
	while(1)
	{
		p_kb->run( p_kb);
		delay_ms( 200);
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
	char buf[32];
	char buf2[64];
	
	
	sprintf(buf, "key num %d ", num);
	p_lcd->clear(PALLET_WHITE);
	p_lcd->dispaly_text(0, "key test", 0, 32, 0, FONT_12, PALLET_BLACK);
	p_lcd->dispaly_text(0, buf, 0, 0, 16, FONT_12, PALLET_BLACK);
	

	for( i = 0; i < num; i++)
	{
		memset( buf, 0, sizeof( buf));
		memset( buf2, 0, sizeof( buf2));
		Keycode2Str( arr_msg[i].keyCode, 32, buf);
		strcat( buf2, buf);
		Keyevnet2Str( arr_msg[i].eventCode, 32, buf);
		strcat( buf2, " : ");
		strcat( buf2, buf);
		
		
		p_lcd->dispaly_text(0, buf2, 0, 0, 16 * ( i + 2) , FONT_12, PALLET_BLACK);
	}
	
	
	return 0;
	
	
	
}

#endif
