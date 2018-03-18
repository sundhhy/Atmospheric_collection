#include "stm32f10x.h"

#include "pinmux.h"
#include "configs/hardware_conf.h"



void Pin_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	/***** LCD ****************/
	GPIO_InitStructure.GPIO_Pin = PIN_LCD_DI; 				//PB.1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//  �������
	GPIO_Init(PORT_LCD_DI, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_RW; 				//PC.5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//  �������
	GPIO_Init(PORT_LCD_RW, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_E; 				//PE.7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//  �������
	GPIO_Init(PORT_LCD_E, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_CS1; 				    //PD.13
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//  �������
	GPIO_Init(PORT_LCD_CS1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_PWM; 				//PA.0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	//  �������
	GPIO_Init(PORT_LCD_PWM, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_RST; 				//PD.14
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//  �������
	GPIO_Init(PORT_LCD_RST, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_BACKEN; 				//PB.0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//  �������
	GPIO_Init(PORT_LCD_BACKEN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_CS2; 				//PD.15
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//  �������
	GPIO_Init(PORT_LCD_CS2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_DATA_0; 				//PE.8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//  �������
	GPIO_Init(PORT_LCD_DATA_0, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_DATA_1; 				//PE.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//  �������
	GPIO_Init(PORT_LCD_DATA_1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_DATA_2; 				//PE.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//  �������
	GPIO_Init(PORT_LCD_DATA_2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_DATA_3; 				//PE.11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//  �������
	GPIO_Init(PORT_LCD_DATA_3, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_DATA_4; 				//PE.12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//  �������
	GPIO_Init(PORT_LCD_DATA_4, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_DATA_5; 				//PE.13
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//  �������
	GPIO_Init(PORT_LCD_DATA_5, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_DATA_6; 				//PE.14
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//  �������
	GPIO_Init(PORT_LCD_DATA_6, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_DATA_7; 				//PE.15
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//  �������
	GPIO_Init(PORT_LCD_DATA_7, &GPIO_InitStructure);
	
//	/*** pwr ***/
//	GPIO_InitStructure.GPIO_Pin = PIN_PWR;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//    GPIO_Init( PORT_PWR, &GPIO_InitStructure);
//	
//	/******* flash spi pin	************************************/
//	
//	GPIO_InitStructure.GPIO_Pin = PIN_FSH_nWP; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(PORT_FSH_nWP, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = PIN_FM25_nCS;                   
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(PORT_FM25_nCS, &GPIO_InitStructure);
//	GPIO_SetBits(PORT_FM25_nCS, PIN_FM25_nCS);
//	
//	GPIO_InitStructure.GPIO_Pin = PIN_W25Q_nCS;                   
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(PORT_W25Q_nCS, &GPIO_InitStructure);
//	GPIO_SetBits(PORT_W25Q_nCS, PIN_W25Q_nCS);
//	
//	GPIO_InitStructure.GPIO_Pin = PIN_W25Q_SO;       
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(PORT_W25Q_SO, &GPIO_InitStructure);

//    GPIO_InitStructure.GPIO_Pin = PIN_W25Q_SI;                   
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(PORT_W25Q_SI, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = PIN_W25Q_SCK;        
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(PORT_W25Q_SCK, &GPIO_InitStructure);


//	GPIO_InitStructure.GPIO_Pin = PIN_FM25_SO;       
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(PORT_FM25_SO, &GPIO_InitStructure);

//    GPIO_InitStructure.GPIO_Pin = PIN_FM25_SI;                   
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(PORT_FM25_SI, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = PIN_FM25_SCK;        
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(PORT_FM25_SCK, &GPIO_InitStructure);
//	/******* IIC *******************************************/
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IIC_SDA;                   
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
//    GPIO_Init( GPIO_PORT_IIC_SDA, &GPIO_InitStructure);	
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IIC_SCL;        
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
//    GPIO_Init( GPIO_PORT_IIC_SCL, &GPIO_InitStructure);
//	
//	GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);
//	/*********** usb 	************************************/
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_USBRESET;                   
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(GPIO_PORT_USBRESET, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_POWER;                   
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(GPIO_PORT_POWER, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_USBINT;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//    GPIO_Init( GPIO_PORT_USBINT, &GPIO_InitStructure);
//	/*********** usb spi1	************************************/

//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SPI1_MOSI;       
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init( GPIO_PORT_SPI1, &GPIO_InitStructure);
////	GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);

//    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SPI1_MISO;                   
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init( GPIO_PORT_SPI1, &GPIO_InitStructure);
////	GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SPI1_SCK;        
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init( GPIO_PORT_SPI1, &GPIO_InitStructure);
////	GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);

//    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SPI1_NSS;                   
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init( GPIO_PORT_SPI1, &GPIO_InitStructure);
//	
//	/*********************************************************/
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_UART1TX;        //tx
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
//    GPIO_Init( GPIO_PORT_UART1TX, &GPIO_InitStructure);
//	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);

//    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_UART1RX;                   //rx
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init( GPIO_PORT_UART1RX, &GPIO_InitStructure);
//	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_UART2TX;        //tx
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init( GPIO_PORT_UART2TX, &GPIO_InitStructure);

//    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_UART2RX;                   //rx
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init( GPIO_PORT_UART2RX, &GPIO_InitStructure);
////		GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_UART3TX;        //tx
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init( GPIO_PORT_UART3TX, &GPIO_InitStructure);
//	
//    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_UART3RX;                   //rx
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init( GPIO_PORT_UART3RX, &GPIO_InitStructure);
//	
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_UART4TX;        //tx
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init( GPIO_PORT_UART4TX, &GPIO_InitStructure);
//	
//    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_UART4RX;                   //rx
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init( GPIO_PORT_UART4RX, &GPIO_InitStructure);
//	
//	/*********** key pins	************************************/
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY_RIGHT;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//    GPIO_Init( GPIO_PORT_KEY_RIGHT, &GPIO_InitStructure);
//	
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY_LEFT;
////    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
////    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//    GPIO_Init( GPIO_PORT_KEY_LEFT, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY_UP;
////    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
////    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//    GPIO_Init( GPIO_PORT_KEY_UP, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY_DOWN;
////    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
////    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//    GPIO_Init( GPIO_PORT_KEY_DOWN, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY_ENTER;
////    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
////    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//    GPIO_Init( GPIO_PORT_KEY_ENTER, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY_ESC;
////    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
////    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//    GPIO_Init( GPIO_PORT_KEY_ESC, &GPIO_InitStructure);
	
	
	
}
