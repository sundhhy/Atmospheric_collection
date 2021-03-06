#ifndef _INC_HMI_start_sample_H_
#define _INC_HMI_start_sample_H_
#include "HMI.h"
#include "HMI_comm.h"
#include "Model.h"
//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <stdint.h>
//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

 //------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------
CLASS(HMI_start_sample)
{
	EXTENDS( HMI);
	IMPLEMENTS(mdl_observer);
};
//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------
HMI_start_sample *Get_HMI_start_sample(void);
#endif
