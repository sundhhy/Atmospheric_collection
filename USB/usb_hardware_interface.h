//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef __INC_usb_hardware_interface_H__
#define __INC_usb_hardware_interface_H__


//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------

int CH376_Init_gpio(void);
void CH376_Disable_irq(void);
void CH376_Enable_irq(void);

#endif

