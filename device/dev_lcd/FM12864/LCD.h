#ifndef		__LCD_H
#define		__LCD_H
#define	ENTER_MIMA		0
#define	ZHU_ZUTAI		1
#define	XITONG_ZUTAI	2
#define TONGDAO_ZUTAI1	3
#define	TONGDAO_ZUTAI2 	4
#define BEIFEN			5

#define WANGLUO_ZUTAI	7
#define	BAOJING_ZUTAI	8
#define TONGXUN_ZUTAI	9
#define	YIBIAO_PEIZHI	15

#define	BIAODING		50
#define	XIANSHI_AI		51
#define	XIANSHI_AD 		52

#define TINGZHIJILU		60
#define TINGZHIJILU1	61

#define	SHUXIAN_RUN 	101
#define	SHISHI_RUN 		103
#define LISHI_RUN 		104
#define	BAOJING_RUN 	105
#define SHUXIAN4_RUN 	107

/*#define
#define
#define*/
void W_Lcd_Inst(unsigned char Inst_order);
void LCD_Wait(void);
void LCD_Write_Byte(unsigned char data);

void InitLCD(void);
void MainDisplay(void);
void Dis6_11_One(unsigned char  x,unsigned char y,unsigned char val);
void Delay3(void);
void ADValue_Display(void);
void Put_SHISHI_Data(void);
void LCD_Clear(void);
unsigned char LCD_Read_Byte(void);
void Write_LCD_CChina(unsigned char x,unsigned char y,unsigned char *p);

#endif
