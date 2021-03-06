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
#define HMI_MAIN									1

#define HMI_CMM										2

#define	HMI_CONFIG								3
#	define HMI_SELECT_SETTING				0x10
#		define	HMI_CONFIGURE							0x20

#define	HMI_QUERY									4
#define	HMI_MAINTAIN						5

#define	HMI_START_SAMPLE					6
#define	HMI_WAIT_SAMPLE						7

#define HMI_INPUT_PSD						8

#define	HMI_NONE				0xff
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
HMI *CreateHMI(char hmi_type);

#endif
