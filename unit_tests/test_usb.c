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
#include "USB/Usb.h"
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
#define TUSB_PAGE_MAX_NUM		7
#define TUSB_16MB						16 * 1024 * 1024 


//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------
static struct {
	char			*usb_buf;
	short			usb_buf_size;
	
	uint8_t		count;

	uint8_t		write_file;
	
}test_usb;


//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static KbTestOb 				*p_kbTestOb;

static dev_lcd 		*p_lcd;

static usb_conf_t			td_usb_conf;
//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------

static void TUSB_Display(void *arg);

static int 		KeyEvent( char num, keyMsg_t arr_msg[]);
static int		Usb_event(int type);
static void		TUSB_Write_file(char *file, int size);	
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
void Test_usb(void)
{
	
	Keyboard		*p_kb;
	char				file_name[16];
	
	SystemCoreClockUpdate();
	
	Dev_open(DEVID_FM12864, (void *)&p_lcd);

	
	//按键初始化
	p_kb = GetKeyInsance();
	
	//借用一下内存:&aci_sys.lcd_cmd_bytes
//	aci_sys.lcd_cmd_bytes = CONF_KEYSCAN_CYCLEMS;
//	p_kb->init( p_kb, &aci_sys.lcd_cmd_bytes);
//	aci_sys.lcd_cmd_bytes  = 0;

	p_kbTestOb = KbTestOb_new();
	p_kbTestOb->setKeyHdl( p_kbTestOb, KeyEvent);
	p_kb->addOb( p_kb, ( keyObservice *)p_kbTestOb);
	osKernelStart ();                         // start thread execution 
	
	p_lcd->clear(PALLET_WHITE);
	p_lcd->dispaly_text(0, "Init USB:", 0, 0, 0, FONT_12, PALLET_BLACK);
	
	
	td_usb_conf.cpu_clk_hz = SystemCoreClock;
	if(USB_Init(&td_usb_conf) == 0)
	{
		p_lcd->dispaly_text(0, "success", 0, 80, 0, FONT_12, PALLET_BLACK);
		
	}
	else
	{
		p_lcd->dispaly_text(0, "failed", 0, 80, 0, FONT_12, PALLET_BLACK);
		
	}
	USB_Rgt_event_hdl(Usb_event);
	test_usb.count = 0;
	test_usb.write_file = 0;
	test_usb.usb_buf_size = USB_MAX_WRITE_BYTE;
	test_usb.usb_buf = malloc(test_usb.usb_buf_size);
	
	
	while(1)
	{
		USB_Run();
		
		
		p_lcd->lcd_flush(0);
//		p_kb->run( p_kb);
		delay_ms( 200);
		if(test_usb.write_file)
		{
			sprintf(file_name, "TEST_%d.bin", test_usb.count);
			p_lcd->lcd_flush(0);
			TUSB_Write_file(file_name, TUSB_16MB);
			test_usb.write_file = 0;
		}
		
	}
	
	
}
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{

//把int的值从0开始写入文件，知道文件的大小大于等于size
static void		TUSB_Write_file(char *file, int size)
{
	int	fd;
	int wr_data = 0;
	int	max = size / sizeof(int);
	int	*p_buf;
	float	prc = 0.0;
	short	i = 0, limit;
	char	lcd_buf[32];
	
	limit = test_usb.usb_buf_size / sizeof(int);
	p_buf = (int *)test_usb.usb_buf;
	
	
	p_lcd->dispaly_text(0, file, 0, 0, 32, FONT_12, PALLET_BLACK);

	
	//创建文件
	fd = USB_Create_file(file, USB_FM_READ | USB_FM_WRITE | USB_FM_COVER);
	if(fd < 0)
	{
		p_lcd->dispaly_text(0, "fail", 0, 88, 32, FONT_12, PALLET_BLACK);
		return;
	}
	
	
	//写入数据
	p_lcd->lcd_flush(0);
	while(wr_data < max)
	{
		//复位要操作的缓存的位置
		
		i = 0;
		
		//尽可能多的把缓存填满
		while(i < limit)
		{
			wr_data ++;
			p_buf[i] = wr_data;
			i++;
			
		}
		
		//把缓存的数据写入文件
		USB_Write_file(fd, test_usb.usb_buf, i * sizeof(int));
		//提示写入的数据量
		prc = (wr_data + 1)* 4 * 100;
		prc = prc / size;
		sprintf(lcd_buf, "%%%.1f", prc);
		p_lcd->dispaly_text(0, lcd_buf, 0, 88, 32, FONT_12, PALLET_BLACK);
		p_lcd->lcd_flush(0);
		if(test_usb.write_file == 0)
			break;
	}
	
	//关闭文件
	size = (wr_data + 1) * sizeof(int);
	if(USB_Colse_file(fd) == 0)
	{
		p_lcd->dispaly_text(0, "wr", 0, 0, 48, FONT_12, PALLET_BLACK);
		
		prc = size / 1024 / 1024;
		sprintf(lcd_buf, "%.2fMB, done",  prc);
		p_lcd->dispaly_text(0, lcd_buf, 0, 24, 48, FONT_12, PALLET_BLACK);
	}
	else
	{
		p_lcd->dispaly_text(0, "close fail", 0, 0, 48, FONT_12, PALLET_BLACK);
	}
	p_lcd->lcd_flush(0);
}

static int KeyEvent( char num, keyMsg_t arr_msg[])
{
	int i = 0;

	for( i = 0; i < num; i++)
	{

		switch(arr_msg[i].keyCode)
		{
			case KEYCODE_SWITCH:
				
			
				TUSB_Display(NULL);
				break;
			case KEYCODE_ESC:
				test_usb.write_file = 0 ;
				break;
			case KEYCODE_ENTER:
				test_usb.write_file = 1;
				test_usb.count ++;
				break;
		}
	}

	return 0;

}



static void TUSB_Display(void *arg)
{
	char		tsb_buf[64];
	
	p_lcd->clear(PALLET_WHITE);
	sprintf(tsb_buf, "USB Test:");
	p_lcd->dispaly_text(0, tsb_buf, 0, 0, 0, FONT_12, PALLET_BLACK);
	
	
	
}

static int	Usb_event(int type)
{
	if(type == et_ready)
	{
		
		p_lcd->dispaly_text(0, "Find u disk   ", 0, 0, 16, FONT_12, PALLET_BLACK);
		
		
	}
	else if(type == et_remove)
	{
		p_lcd->dispaly_text(0, "Remove u disk", 0, 0, 16, FONT_12, PALLET_BLACK);
		
	}
	
	return 0;
}




#endif
