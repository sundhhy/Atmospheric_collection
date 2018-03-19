//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
#include "os/os_depend.h"

#include "configs/tdd_conf.h"
#include "configs/lcd_conf.h"
#include "unit_test.h"
#include "device.h"
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


//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------



//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
void Test_lcd(void)
{
	dev_lcd *p_lcd;
	Dev_open(DEVID_FM12864, (void *)&p_lcd);
	p_lcd->clear(CLR_WHITE);
	//µãÏß»æÖÆ²âÊÔ
	p_lcd->lcd_lightness(60);
	
	p_lcd->draw_geometry(GEO_LINE, ATT_NONE, CLR_BLACK, 0, 0, 63, 0);
	
	p_lcd->draw_geometry(GEO_LINE, ATT_NONE, CLR_BLACK, 0, 0, 0, 63);
	p_lcd->draw_geometry(GEO_LINE, ATT_NONE, CLR_BLACK, 63, 0, 63, 63);
	p_lcd->draw_geometry(GEO_LINE, ATT_NONE, CLR_BLACK, 0, 63, 63, 63);
	
	p_lcd->draw_geometry(GEO_LINE, ATT_NONE, CLR_BLACK, 0, 0, 63, 63);
	p_lcd->draw_geometry(GEO_LINE, ATT_NONE, CLR_BLACK, 0, 63, 63, 0);

	
	p_lcd->draw_geometry(GEO_LINE, ATT_NONE, CLR_BLACK, 0, 0, 127, 0);
	p_lcd->draw_geometry(GEO_LINE, ATT_NONE, CLR_BLACK, 127, 0, 127, 63);
	p_lcd->draw_geometry(GEO_LINE, ATT_NONE, CLR_BLACK, 0, 63, 127, 63);
	
	p_lcd->draw_geometry(GEO_LINE, ATT_NONE, CLR_BLACK, 0, 0, 127, 63);
	p_lcd->draw_geometry(GEO_LINE, ATT_NONE, CLR_BLACK, 0, 63, 127, 0);
	p_lcd->lcd_flush(0);
	delay_ms(500);
	p_lcd->clear(CLR_WHITE);
	p_lcd->draw_geometry(GEO_RECTANGLE, ATT_NONE, CLR_BLACK, 0, 0, 8, 8);
	p_lcd->lcd_flush(0);
	//²âÊÔÊý×ÖÏÔÊ¾
	delay_ms(500);
	p_lcd->clear(CLR_WHITE);
	p_lcd->dispaly_text(0, "0123456789", 0, 0, 0, FONT_12, COLOUR_BLACK);
	p_lcd->lcd_flush(0);
	
	p_lcd->dispaly_text(0, "é‡‡", 0, 0, 16, FONT_12, COLOUR_BLACK);
	p_lcd->lcd_flush(0);

//	p_lcd->lcd_lightness(0);
	
//	p_lcd->display_control_switch(1);
//	p_lcd->clear(COLOUR_BLACK);
//	p_lcd->lcd_lightness(50);
//	p_lcd->lcd_lightness(0);
//	p_lcd->clear(COLOUR_WHITE);
//	p_lcd->display_control_switch(1);
//	
//	p_lcd->dispaly_text(0, "test english 1 ", 0, 0, 0, FONT_12, COLOUR_BLACK);
//	p_lcd->dispaly_text(0, "test english 2", 0, 8, 0, FONT_16, COLOUR_BLACK);
//	p_lcd->dispaly_text(0, "test english 3", 0, 16, 0, FONT_24, COLOUR_BLACK);
//	p_lcd->lcd_flush();
//	
//	p_lcd->clear(COLOUR_BLACK);
//	p_lcd->dispaly_text(0, "ºº×Ö²âÊÔ 1", 0, 0, 0, FONT_12, COLOUR_WHITE);
//	p_lcd->dispaly_text(0, "ºº×Ö²âÊÔ 2", 0, 8, 0, FONT_16, COLOUR_WHITE);
//	p_lcd->dispaly_text(0, "ºº×Ö²âÊÔ 3", 0, 16, 0, FONT_24, COLOUR_WHITE);
//	p_lcd->lcd_flush();
//	
//	p_lcd->lcd_lightness(0);
//	p_lcd->lcd_lightness(100);
//	
//	p_lcd->display_control_switch(0);

	while(1);
}
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{

#endif
