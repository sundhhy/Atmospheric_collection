//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef __INC_ACI_system_H__
#define __INC_ACI_system_H__

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

typedef struct {
	int			hmi_sem;
}hmi_mgr_t;

typedef struct 
{
	uint32_t		sys_flag;
	char	flag_lcd_flush;
	char	none[3];
	
		//����
	uint16_t				key_weight;
	uint16_t				hit_count;
	
	hmi_mgr_t				hmi_mgr;
}system_t;
//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------
extern system_t aci_sys;
//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------


#endif
