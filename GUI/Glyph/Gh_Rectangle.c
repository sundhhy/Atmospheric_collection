#include "Gh_Rectangle.h"
#include "device.h"

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

static GhRectangle *signalGhRectangle;

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static void GhRec_vDraw( Glyph *self, dspContent_t *cnt, vArea_t *area);


//static void GhRec_Draw( Glyph *self, dspContent_t *cnt, dspArea_t *area);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

GhRectangle *Get_GhRectangle(void)
{
//	Glyph *gh;
	
	if( signalGhRectangle == NULL)
	{
		signalGhRectangle = GhRectangle_new();
		if(signalGhRectangle  == NULL) while(1);

		
		
	}
	return signalGhRectangle;
}

CTOR( GhRectangle)
SUPER_CTOR( Glyph);
//FUNCTION_SETTING( Glyph.draw, GhRec_Draw);
FUNCTION_SETTING( Glyph.vdraw, GhRec_vDraw);


END_CTOR


//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{













static void GhRec_vDraw( Glyph *self, dspContent_t *cnt, vArea_t *area)
{
	

}

















