#include "Gh_Pic.h"
#include "device.h"
#include "os/os_depend.h"

//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

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
static GhPic *signalGhPic;


//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------

static void GhPic_vDraw( Glyph *self, dspContent_t *cnt, vArea_t *area);


//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
GhPic *Get_GhPic(void)
{
//	Glyph *gh;
	
	if( signalGhPic == NULL)
	{
		signalGhPic = GhPic_new();
		if(signalGhPic  == NULL) while(1);
		
	}
	

	return signalGhPic;
}

CTOR( GhPic)
SUPER_CTOR( Glyph);

FUNCTION_SETTING( Glyph.vdraw, GhPic_vDraw);


END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{

static void GhPic_vDraw( Glyph *self, dspContent_t *cnt, vArea_t *area)
{
	I_dev_lcd *lcd;
	char num;
	Dev_open( LCD_DEVID, (void *)&lcd);

	num = atoi( cnt->data);
	
	if( cnt->subType == SUBTYPE_CPIC) {
		lcd->cutPicture( area->x0 + area->offset_x, area->y0 + area->offset_y, num, \
		area->x0, area->y0, ( area->x1 - area->x0), ( area->y1 - area->y0));
	} else if( cnt->subType == SUBTYPE_BPIC) {
		lcd->bkPicture( cnt->bkc, area->x0, area->y0, num);
	}  else if( cnt->subType == SUBTYPE_ICON) {
		lcd->icon(area->x0, area->y0, num, area->xn, area->yn, area->n);
	}
	else {
		
		lcd->picture( area->x0, area->y0, num);
	}
	
	


}











