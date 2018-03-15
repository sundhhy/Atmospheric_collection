#ifndef __INC_FM25_H
#define __INC_FM25_H
                     // RTOS object definitions

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include "hardwareConfig.h"
#include "system.h"
//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define FM25L04B					0		//512 * 8 bit
#define FM25L64B					1		//8192 * 8 bit
#define FM25_DEVTYPE				FM25L64B		

#define FM25CL64_WREN				0x06
#define FM25CL64_WRDI				0x04
#define FM25CL64_RDSR				0x05
#define FM25CL64_WRSR				0x01
#define FM25CL64_READ				0x03
#define FM25CL64_WRITE				0x02


#define FM25CL64_PROTECT 			0x0c
#define FM25CL64_UNPROTECT			0x00

#define	FM25_STATUS_WEL				2			//дʹ��
#define	FM25_STATUS_BP0				4			
#define	FM25_STATUS_BP1				8			
#define	FM25_STATUS_WPEN			0x80			



//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------


int FM25_init(void);


#endif

