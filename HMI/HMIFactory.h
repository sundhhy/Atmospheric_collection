#ifndef _INC_HMIfactory_H_
#define _INC_HMIfactory_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <stdint.h>
#include "HMI.h"

//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define HMI_MAIN				1
#define HMI_CMM					2
#define	HMI_CONFIG			3


//#define HMI_MENU				4
//#define HMI_BAR					5
//#define HMI_DATA				6
//#define HMI_NWS					8
//#define HMI_ACCM				12
//#define HMI_SETUP				13
//#define HMI_SETTING				14
//#define HMI_WINDOWS				15
 //------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// global variable declarations


//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------
HMI *CreateHMI( int type);

#endif
