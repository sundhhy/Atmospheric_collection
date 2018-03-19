//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef __INC_lcd_hardware_interfaces_H_
#define __INC_lcd_hardware_interfaces_H_
#include "configs/hardware_conf.h"



//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define LCD_STATUS_BUSY				0x80
#define LCD_STATUS_UNLOCK			0x20
#define LCD_STATUS_RST				0x10

#define LHI_LCD_ON GPIO_SetBits(PORT_LCD_BACKEN,PIN_LCD_BACKEN)
#define LHI_LCD_OFF GPIO_ResetBits(PORT_LCD_BACKEN,PIN_LCD_BACKEN)



#define LHI_SELECT_LEFT {\
	GPIO_ResetBits(PORT_LCD_CS2,PIN_LCD_CS2); \
	GPIO_SetBits(PORT_LCD_CS1,PIN_LCD_CS1); \
}
#define LHI_SELECT_RIGHT {\
	GPIO_ResetBits(PORT_LCD_CS1,PIN_LCD_CS1); \
	GPIO_SetBits(PORT_LCD_CS2,PIN_LCD_CS2); \
}

#define LHI_SELECT_NONE {\
	GPIO_ResetBits(PORT_LCD_CS1,PIN_LCD_CS1); \
	GPIO_ResetBits(PORT_LCD_CS2,PIN_LCD_CS2); \
}
//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------

void LHI_Init_pwm(short default_duty);
void LHI_Set_pwm_duty(short duty);
void LHI_Reset_lcd(void);
int  LHI_Wait(int us);

int LHI_Send_cmd(uint8_t cmd);
int	LHI_Read_status(void);
int LHI_Write_vram(uint8_t *buf, int wr_len);
int LHI_Read_vram(uint8_t *buf, int rd_len);
#endif

