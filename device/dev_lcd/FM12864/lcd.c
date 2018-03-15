#include "stm32f10x.h"
#include "ZikuDeclare.h"
#include "LCD.h"
#include "integer.h"
#include "IODefine.h"
#include "GlobalDeclare.h"
#include "StructDefine.h"

#define PORT_IN		0x00
#define PORT_OUT	0x01

/**********************************************************
*****�������ƣ�unsigned char BITSWP8FUN(unsigned char data)
*****�������ܣ���һ����λ�ַ���4λ���4λ�ߵ�
*****��ڲ�������Ҫ��ת���ַ�
*****���ڲ�������ת����ַ�
***********************************************************/
unsigned char BITSWP8FUN(unsigned char data)
{
      unsigned char p[8];
      unsigned char i;
      unsigned char j=0x01;
      unsigned char m=0x00;
      for(i=0;i<8;i++)
      {
          p[i]=data&j;
          j=j<<1;
      }
      j=7;
      for(i=0;i<4;i++)
      {
         p[i]=p[i]<<j;
         p[7-i]=p[7-i]>>j;
         j=j-2;
      }
      for(i=0;i<8;i++)
        m|=p[i];
      
      return m;
}
//*************************************************
//-------------------------------------------------
//���ܣ���ʱ�ӳ���0.04ms
//��ڣ���
//���ڣ���
//-------------------------------------------------
void Delay3(void)
{
    unsigned short i;
    unsigned short j;
    i=20;//20
    j=2;//2
    while(i--)
    {
      while(j--);
      j=2;
    }
}


//*************************************************
//-------------------------------------------------
//���ܣ�дLCDһ���ַ�����
//��ڣ���
//���ڣ���
//-------------------------------------------------
void LCD_PORT_Write(UCHAR LCD_Data)
{
	USHORT PortData;
	PortData = GPIO_ReadOutputData(GPIOE);	     //�ȶ�ȡGPIO��ֵ��Ȼ����д��
  PortData&=0x00FF;                            //PE8��PE15
	PortData +=(((USHORT)LCD_Data<<8)&0xff00);   //��ø�8bit
	GPIO_Write(GPIOE,PortData);
}
/*********************************************************
***�������ƣ�LCD_PORT_DIR(UCHAR DIR)
***��������: ����GPIOE �˿�
***��ڲ�������
***���ڲ�������
**********************************************************/
void LCD_PORT_DIR(UCHAR DIR)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|
									GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	
	if(DIR == PORT_IN)                                      //���ó�����
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //����
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������	
	}
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;       //2Mʱ���ٶ�
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
///////////////////////////////
//-----------------------------------------------------------------------------------
//I��LCD���������TIMERB�Ŀ����жϳ���
//-----------------------------------------------------------------------------------
//--CS1 H CS2 L    ѡ������
//--CS1 L CS2 H    ѡ������
void LCD_Set_Block(UCHAR block)
{
 switch(block)
 {
  case 1:   SET_LCD_CS1;		    //ѡ������
            CLR_LCD_CS2;
            break;
   								              //ѡ������
  case 2:   CLR_LCD_CS1;
            SET_LCD_CS2;
            break;
						                    //ѡ��ȫ��
  case 3:   
            SET_LCD_CS1;
            SET_LCD_CS2;
            break;
	
  default:                      //ѡ��ȫ��
		       SET_LCD_CS1;       
           SET_LCD_CS2;
	break; 
 }
}
/********************************************
*****�������ƣ�W_Lcd_Inst(UCHAR Inst_order)
*****�������ܣ�LCDдָ���
*****��ڲ�������Ҫд��ָ��
*****���ڲ�������
*********************************************/
void W_Lcd_Inst(UCHAR Inst_order)
{
 LCD_PORT_DIR(PORT_OUT);                 //���ó����ģʽ	      
 LCD_PORT_Write(Inst_order);
 CLR_LCD_DI;                             //DI L ѡ��дָ��
 CLR_LCD_RW;
 SET_LCD_E;
 //Delay3();
 CLR_LCD_E;
 LCD_PORT_DIR(PORT_IN);
}


/********************************************
******�������ƣ�LCD_Set_X_Addr(UCHAR page)
******�������ܣ�����X��ַ
******��ڲ�������
******���ڲ�������
*********************************************/
void LCD_Set_X_Addr(UCHAR page)
{
 page |= 0XB8;                               // Page ��ַ��ʼ0xB8
 W_Lcd_Inst(page);
}
/********************************************
******�������ƣ�LCD_Set_Y_Addr(UCHAR column)
******�������ܣ�����X��ַ
******��ڲ�������
******���ڲ�������
*********************************************/
void LCD_Set_Y_Addr(UCHAR column)
{
 column |=0X40;                              //Col ��ַ��ʼ0x40
 W_Lcd_Inst(column);
}

/********************************************
******�������ƣ�LCD_Status_Read(void)
******�������ܣ���ȡLCD�˿�״̬
******��ڲ�������
******���ڲ�������ȡ�Ķ˿ڽ��
*********************************************/
UCHAR LCD_Status_Read(void)                  //��ȡҺ��״̬
{
	UCHAR Lcd_status;
	LCD_PORT_DIR(PORT_IN);
	CLR_LCD_DI;         
	SET_LCD_RW;
	SET_LCD_E;
	Lcd_status = (UCHAR)((GPIO_ReadInputData(GPIOE)&0xff00)>>8);   //��ö˿��ϵ�����
	return Lcd_status;
}

/********************************************
******�������ƣ�LCD_Wait(void)
******�������ܣ�LCD��æ����
******��ڲ�������
******���ڲ�������
*********************************************/
void LCD_Wait(void)
{
	UCHAR Lcd_status;
	do
	{
		Lcd_status=LCD_Status_Read();
	}
	while((Lcd_status&0x90) != 0);                                 //��æ������Ҫ���ǳ�ʱ���˳�
}
/********************************************
******�������ƣ�LCD_Write_Byte(UCHAR data)
******�������ܣ�LCDд���ֽں���
******��ڲ�������
******���ڲ�������
*********************************************/
void LCD_Write_Byte(UCHAR data)
{
	LCD_PORT_DIR(PORT_OUT);
	//data=BITSWP8FUN(data);    //����,���跴ת
	LCD_PORT_Write(data);
	SET_LCD_DI;                 //DI H��ʾд����
	CLR_LCD_RW;
	SET_LCD_E;
	//Delay3();
	CLR_LCD_E;
	LCD_PORT_DIR(PORT_IN);
}
/********************************************
******�������ƣ�UCHAR LCD_Read_Byte(void)
******�������ܣ�LCD���ֽں���
******��ڲ�������
******���ڲ�������ȡ���ֽ�
*********************************************/
UCHAR LCD_Read_Byte(void)
{
	UCHAR data=0;
	LCD_PORT_DIR(PORT_IN);
	SET_LCD_DI;
	SET_LCD_RW;
	SET_LCD_E;
	data = (UCHAR)((GPIO_ReadInputData(GPIOE)&0xff00)>>8);
//data=BITSWP8FUN(data);   
	return data; 
}
/********************************************
******�������ƣ�LCD_SetXY1(UCHAR x, UCHAR y)
******�������ܣ����� X Y��ַ
******��ڲ�����X��ַ  Y��ַ
******���ڲ�������
*********************************************/ 
void LCD_SetXY1(UCHAR x, UCHAR y)
{
 UCHAR x_addr;
 UCHAR y_addr;
 
 x_addr = x/8;	   
 y_addr = y%64;	 // ����Y��ַ
 
 if(y<64)
 {
  LCD_Set_Block(1);	     //YС��64��������
 }
 
 else if(y<128)
 {
  LCD_Set_Block(2);	     //YС��128��������
 }
 
 
// else
// {
//  LCD_Set_Block(3);	    //YС��192��������
// }
 
	LCD_Wait();
	LCD_Set_X_Addr(x_addr);
  LCD_Wait();
  LCD_Set_Y_Addr(y_addr);
  LCD_Wait();
}
/********************************************************************
******�������ƣ�LCD_SetXY(UCHAR x, UCHAR y)
******�������ܣ����� X Y��ַ (��ΪMR�����ǵߵ��õģ�������Ҫ��ôŪ)
******��ڲ�����X��ַ  Y��ַ
******���ڲ�������
*********************************************************************/
void LCD_SetXY(UCHAR x, UCHAR y)
{
  //  y=191-y;                           
  //  x=63-x;
    LCD_SetXY1(x,y);
}
/********************************************************************
******�������ƣ�LCD_One_BlockClear(UCHAR block)
******�������ܣ�LCD��������
******��ڲ�������Ҫ������������ң�   block   ֻ��ѡ�� 1 2 
******���ڲ�������
*********************************************************************/
void LCD_One_BlockClear(UCHAR block)
{
 UCHAR j,k=0;
 
 LCD_Set_Block(block);
 LCD_Wait();
	
 for(j=0;j<8;j++)
 {
  LCD_Set_X_Addr(j);
  LCD_Wait();
  LCD_Set_Y_Addr(0);
  LCD_Wait();
 
  for(k=0;k<64;k++)
  {
   LCD_Write_Byte(0x0);
   LCD_Wait();
  }

 }
}
/********************************************************************
******�������ƣ�void LCD_Clear(void)
******�������ܣ�LCD��ȫ������
******��ڲ�������
******���ڲ�������
*********************************************************************/
void LCD_Clear(void)
{
  UCHAR i;
	
// for(i=1;i<4;i++)
// {
//  LCD_One_BlockClear(i);
// }
 
  for(i=1;i<3;i++)        //����������
 {
  LCD_One_BlockClear(i);
 }
 
}
//*******************************************
//-------------------------------------------
//����:������,���϶���д����ֽ���ʾ
//��ڣ�������xx(0~191),������yy(0~7),��ʾ����one_byte,�ֽ���
//���ڣ���
//-------------------------------------------
void Write_Lcd_Bytes(UCHAR xx,UCHAR yy,UCHAR *Data_addr,UCHAR count)
{
    UCHAR Datad;
    yy=yy*8;
    do
    {       
        LCD_SetXY1(yy, xx);
        Datad=*Data_addr;	        //����Ҫ��ʾ�����ݸ�ֵ
      if(Smloc_F==0)			        //Smloc_F���ڿ��Ʒ�д
    	LCD_Write_Byte(Datad);
    	else
    	LCD_Write_Byte(~Datad);	    //��д
    	Data_addr++;
    	xx++;
    	if(xx==0xff)
    	{
				    xx=0;
    	      yy++;
    	}
    }while(--count);
    
}

//*******************************************
//-------------------------------------------
//��������:void Write_Lcd_Bytes_CutDeal(uint8 xx,uint8 yy,uint8 Datad,uint8 count)
//��������:�����ң����ϵ��¼��ж���ֽ���ʾ
//���:������xx(0~191),������yy(0~7),��ʾ����one_byte,�ֽ���count
//����:��
//-------------------------------------------
void Write_Lcd_Bytes_CutDeal(UCHAR xx,UCHAR yy,UCHAR Datad,UCHAR count)
{    
    yy=yy*8;
    
    do
    {       
        LCD_SetXY1(yy, xx);        
        if(Smloc_F==0)
    	LCD_Write_Byte(Datad);
    	else
    	LCD_Write_Byte(~Datad);    	
    	xx++;
    	if(xx==0xff)
    	{
    	    xx=0;
    	    yy++;
    	}
    }while(--count);
    
}
//******************************************
//------------------------------------------
//�������ƣ�void InitLCD(void)
//�������ܣ�    LCD��ʼ��
//��ڣ���
//���ڣ���
//------------------------------------------
void InitLCD(void)
{
  UCHAR a;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* PE8~15 �������� */
	/* PE7 Enable Һ��ʹ���ź�*/
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|
									GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //2Mʱ���ٶ�
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*PA0  LCD_PWM�ź����*/
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //2Mʱ���ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*PB0  Control_Back �����������*/
	/*PB1  DI�����ź����*/
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //2Mʱ���ٶ�
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	//PD13 CS1 PD15 CS2 PD14 RST
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13|GPIO_Pin_15|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //2Mʱ���ٶ�
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//PC5 RW�����ź�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //2Mʱ���ٶ�
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  CLR_LCD_RST;                        //��λҺ����
	Delay3();
	SET_LCD_RST;
	Delay3();
	Delay3();
	
	CLR_LCD_CS1;       
  SET_LCD_CS2; 
  a=0x3F;                             //����ʾ  ����
  W_Lcd_Inst(a);
  a=0xC0;                              
  W_Lcd_Inst(a);                      //start line
    
  CLR_LCD_CS2;          
  SET_LCD_CS1;
  a=0x3F;
  W_Lcd_Inst(a);                      //����ʾ  ����
  a=0xC0;
  W_Lcd_Inst(a);
	
  LCD_Clear();                        //����
}

//****************************************************
//----------------------------------------------------
//�������ƣ�Write_LCD_CChina(UCHAR x,UCHAR y,UCHAR *p)
//����:дһ������
//���:������,������,�ֿ��ַ
//����:��
//----------------------------------------------------
void Write_LCD_CChina(UCHAR x,UCHAR y,UCHAR *p)
{
	
  Write_Lcd_Bytes(x,y,p,16);	  //дһ�����ֵ��ϰ벿��
	y=y+1;
	p=p+16;
	Write_Lcd_Bytes(x,y,p,16);	     //дһ�����ֵ��°벿��
	y=y-1;						  //X��ַ��λ
	x=x+16;
	p=p+16;
}

//************************************************
//------------------------------------------------
//�������ƣ�void  HEX_BCD(USHORT x)
//��������:HEXת��Ϊʮ����
//��ڲ���:x
//���ڲ���:TempTemp[2---0]
//------------------------------------------------
void  HEX_BCD(USHORT x)
{
	TempTemp[2]=x/100;
	x=x%100;
	TempTemp[1]=x/10;
	x=x%10;
	TempTemp[0]=x;
}

//************************************************
//------------------------------------------------
//�������ƣ�void  LONG_HEX_BCD(unsigned long x)
//��������:HEXת��Ϊʮ����
//��ڲ���:x
//���ڲ���:TempTemp[4---0]
//------------------------------------------------
void  LONG_HEX_BCD(unsigned long x)
{
	TempTemp[4]=x/10000;
	x=x%10000;
	TempTemp[3]=x/1000;
	x=x%1000;
	TempTemp[2]=x/100;
	x=x%100;
	TempTemp[1]=x/10;
	x=x%10;
	TempTemp[0]=x;
}

//****************************************************
//----------------------------------------------------
//�������ƣ�void Dis4_5_One(UCHAR x,UCHAR y,UCHAR val)
//��������:��ʾһ��4*5������
//��ڲ���:������,������,ֵ
//���ڲ���:��
//-----------------------------------------------------
void Dis4_5_One(UCHAR x,UCHAR y,UCHAR val)
{
        UCHAR *p;
        switch(val)
        {
            case 0 :p=(UCHAR *)&NUM4_5_0;break;    //0 
            case 1 :p=(UCHAR *)&NUM4_5_1;break;    //1 
            case 2 :p=(UCHAR *)&NUM4_5_2;break;    //2
            case 3 :p=(UCHAR *)&NUM4_5_3;break;    //3
            case 4 :p=(UCHAR *)&NUM4_5_4;break;    //4
            case 5 :p=(UCHAR *)&NUM4_5_5;break;    //5
            case 6 :p=(UCHAR *)&NUM4_5_6;break;    //6
            case 7 :p=(UCHAR *)&NUM4_5_7;break;    //7
            case 8 :p=(UCHAR *)&NUM4_5_8;break;    //8
            case 9 :p=(UCHAR *)&NUM4_5_9;break;    //9
            default:break;
        }
	Write_Lcd_Bytes(x,y,p,3);		
}


//************************************************
//------------------------------------------------
//�������ƣ�void Dis5_7_One(UCHAR x,UCHAR y,UCHAR val)
//���ܹ���:��ʾһ��5*7������
//��ڲ���:������,������,ֵ
//���ڲ���:��
//------------------------------------------------
void Dis5_7_One(UCHAR x,UCHAR y,UCHAR val)
{
        UCHAR *p,k;
        k=0;
        switch(val)
        {
            case W0W:p=(UCHAR *)&NUM5_7_0;break;
            case W1W:p=(UCHAR *)&NUM5_7_1;break;
            case W2W:p=(UCHAR *)&NUM5_7_2;break;
            case W3W:p=(UCHAR *)&NUM5_7_3;break;
            case W4W:p=(UCHAR *)&NUM5_7_4;break;
            case W5W:p=(UCHAR *)&NUM5_7_5;break;
            case W6W:p=(UCHAR *)&NUM5_7_6;break;
            case W7W:p=(UCHAR *)&NUM5_7_7;break;
            case W8W:p=(UCHAR *)&NUM5_7_8;break;
            case W9W:p=(UCHAR *)&NUM5_7_9;break;
            case WdianW:p=(UCHAR *)&NUM5_7_dian;k=1;break;            
            case WyigongW:p=(UCHAR *)&NUM5_7_yigong;break;
            case WkonggeW:p=(UCHAR *)&NUM5_7_kongge;break;
            case WmaohaoW:p=(UCHAR *)&NUM5_7_maohao;break;
            case WAW:p=(UCHAR *)&CHAR_A;break;
            case WVW:p=(UCHAR *)&CHAR_V;break;
            case WHW:p=(UCHAR *)&CHAR_H;break;
            case WIW:p=(UCHAR *)&CHAR_I;break;
            case WLW:p=(UCHAR *)&CHAR_L;break;
            case WOW:p=(UCHAR *)&CHAR_O;break; 
            case WSW:p=(UCHAR *)&CHAR_S;break;
            case WMW:p=(UCHAR *)&CHAR_M;break;
            case WUW:p=(UCHAR *)&CHAR_U;break; 
            case WTW:p=(UCHAR *)&CHAR_T;break; 
            case WNW:p=(UCHAR *)&CHAR_N;break;        
            case WpercentW:p=(UCHAR *)&NUM5_7_percent;break;
            
            default:break;
        }
        if(k==0)
	Write_Lcd_Bytes(x,y,p,6);	
	else
	Write_Lcd_Bytes(x,y,p,4);	
}

//************************************************
//------------------------------------------------
//��������:void CutDealDis(uint8 x,uint8 y,uint8 z,uint16 CNT)
//��������:
//��ڲ���:x,y,z,CNT
//���ڲ���:��
//------------------------------------------------
void CutDealDis(UCHAR x,UCHAR y,UCHAR z,USHORT CNT)
{
        UCHAR p;
        p=0x00;
        if(z==0)//8x16
        {           
           Write_Lcd_Bytes_CutDeal(x,y,p,CNT);  
           Write_Lcd_Bytes_CutDeal(x,y+1,p,CNT);  
        }
        else if(z==1)//24x32
        {
           
           Write_Lcd_Bytes_CutDeal(x,y,p,CNT); 
           Write_Lcd_Bytes_CutDeal(x,y+1,p,CNT);
           Write_Lcd_Bytes_CutDeal(x,y+2,p,CNT); 
           Write_Lcd_Bytes_CutDeal(x,y+3,p,CNT); 
        }
}
//************************************************
//------------------------------------------------
//����:��ʾһ��6*11���ַ�
//���:������,������,ֵ
//����:��
//------------------------------------------------
void Dis6_11_One(UCHAR x,UCHAR y,UCHAR val)
{
        UCHAR *p,k,a;
        k=0;
        switch(val)
        {
            case W0W :p=(UCHAR *)NUM6_11_0;break;
            case W1W :p=(UCHAR *)NUM6_11_1;break;
            case W2W :p=(UCHAR *)NUM6_11_2;break;
            case W3W :p=(UCHAR *)NUM6_11_3;break;
            case W4W :p=(UCHAR *)NUM6_11_4;break;
            case W5W :p=(UCHAR *)NUM6_11_5;break;
            case W6W :p=(UCHAR *)NUM6_11_6;break;
            case W7W :p=(UCHAR *)NUM6_11_7;break;
            case W8W :p=(UCHAR *)NUM6_11_8;break;
            case W9W :p=(UCHAR *)NUM6_11_9;break;
						case WleftKH: p=(UCHAR *)NUM6_11_lefKuoH;break;
					  case WrightKH: p=(UCHAR *)NUM6_11_rhtKuoH;break;
					  case WEqualW:  p =(UCHAR *)NUM6_11_EquaL; break;
					  case WFleftKH: p =(UCHAR *)NUM6_11_FleftKH; break;
					  case WFrightKH:p =(UCHAR *)NUM6_11_FrightKH; break;
					
            case WdianW:p=(UCHAR *)NUM6_11_dian;
						if(Dismode==16||Dismode==17||Dismode==18||Dismode==19||Dismode==20||Dismode==21||Dismode==22)    //Dismode��Ҫȷ��
						{
							k=0; 
						}
						else
						{
							k=1;
						}
            break;
						
            case WyigongW:p=(UCHAR *)NUM6_11_yigong;break;            
            case WkonggeW:p=(UCHAR *)NUM6_11_kongge;break;
            case WmaohaoW:p=(UCHAR *)NUM6_11_maohao;break;
		      	case WxiaokonggeW:p=(UCHAR *)NUM6_11_kongge;k=1;break;
			      case WbarW:p=(UCHAR *)NUM6_11_bar;break;
            case WAW:p=(UCHAR *)WORD_A;break;
            case WBW:p=(UCHAR *)WORD_B;break;
            case WCW:p=(UCHAR *)WORD_C;break;
            case WDW:p=(UCHAR *)WORD_D;break;
            case WEW:p=(UCHAR *)WORD_E;break;
            case WFW:p=(UCHAR *)WORD_F;break;
            case WGW:p=(UCHAR *)WORD_G;break;
            case WHW:p=(UCHAR *)WORD_H;break;
            case WIW:p=(UCHAR *)WORD_I;break;           
            case WJW:p=(UCHAR *)WORD_J;break;
            case WKW:p=(UCHAR *)WORD_K;break;
            case WLW:p=(UCHAR *)WORD_L;break;
            case WMW:p=(UCHAR *)WORD_M;break;
            case WNW:p=(UCHAR *)WORD_N;break;
            case WOW:p=(UCHAR *)WORD_O;break;
            case WPW:p=(UCHAR *)WORD_P;break;
            case WRW:p=(UCHAR *)WORD_R;break;
            case WSW:p=(UCHAR *)WORD_S;break;
            case WTW:p=(UCHAR *)WORD_T;break;
            case WUW:p=(UCHAR *)WORD_U;break;
            case WVW:p=(UCHAR *)WORD_V;break;
            case WWW:p=(UCHAR *)WORD_W;break;
            case WYW:p=(UCHAR *)WORD_Y;break;
            case WZW:p=(UCHAR *)WORD_Z;break;
            case WQW:p=(UCHAR *)WORD_Q;break;
            case WXW:p=(UCHAR *)WORD_X;break;
            case WxW:p=(UCHAR *)WORD_x;break;
            case WdW:p=(UCHAR *)WORD_d;break;
            case WvW:p=(UCHAR *)WORD_v;break;
            case WwW:p=(UCHAR *)WORD_w;break;
            case WjW:p=(UCHAR *)WORD_j;break;
            case WaW:p=(UCHAR *)WORD_a;break;
            case WcW:p=(UCHAR *)WORD_c;break;
            case WfW:p=(UCHAR *)WORD_f;break;
            case WgW:p=(UCHAR *)WORD_g;break;
            case WhW:p=(UCHAR *)WORD_h;break;
            case WiW:p=(UCHAR *)WORD_i;break;
            case WkW:p=(UCHAR *)WORD_k;break;
            case WmW:p=(UCHAR *)WORD_m;break;
            case WnW:p=(UCHAR *)WORD_n;break;
            case WpW:p=(UCHAR *)WORD_p;break;
            case WrW:p=(UCHAR *)WORD_r;break;
            case WsW:p=(UCHAR *)WORD_s;break;
            case WtW:p=(UCHAR *)WORD_t;break;
            case WuW:p=(UCHAR *)WORD_u;break;
            case WzW:p=(UCHAR *)WORD_z;break;
            case WlifangW:p=(UCHAR *)WORD_lifang;break;
            case WpingfangW:p=(UCHAR *)WORD_pingfang;break;
            case Wxiab2W:p=(UCHAR *)WORD_xiab2;break;
            case WxiegongW:p=(UCHAR *)WORD_xiegong;break;
            case WpercentW:p=(UCHAR *)WORD_percent;break;
            case WduW:p=(UCHAR *)WORD_du;break;
            default:break;
        }
        if(k==0)
            a=8;
        else
            a=4;

        Write_Lcd_Bytes(x,y,p,a);
        y=y+1;
        Write_Lcd_Bytes(x,y,p+8,a);
}

//------------------------------------------------
//����:��ʾһ��24*32������
//���:������,������,ֵ
//����:��
//------------------------------------------------
void Dis24_32_One(UCHAR x,UCHAR y,UCHAR val)
{
        UCHAR *p,k,a;
        k=0;
        switch(val)
        {
            case 0 :p=(UCHAR *)NUM24_32_0;break;
            case 1 :p=(UCHAR *)NUM24_32_1;break;
            case 2 :p=(UCHAR *)NUM24_32_2;break;
            case 3 :p=(UCHAR *)NUM24_32_3;break;
            case 4 :p=(UCHAR *)NUM24_32_4;break;
            case 5 :p=(UCHAR *)NUM24_32_5;break;
            case 6 :p=(UCHAR *)NUM24_32_6;break;
            case 7 :p=(UCHAR *)NUM24_32_7;break;
            case 8 :p=(UCHAR *)NUM24_32_8;break;
            case 9 :p=(UCHAR *)NUM24_32_9;break;
            case 10 :p=(UCHAR *)NUM24_32_dian;k=1;break;
            case 11:p=(UCHAR *)NUM24_32_yigong;break;
            case 12:p=(UCHAR *)NUM24_32_kongge;break;   
            case 13:p=(UCHAR *)Huan_24_32;break;  
            case 14:p=(UCHAR *)Ying_24_32;break;
            case 15:p=(UCHAR *)Shi_24_32;break;
            case 16:p=(UCHAR *)Yong_24_32;break;
			case WxiaokonggeW:p=(UCHAR *)NUM24_32_kongge;k=1;break;	
            default:break;
        }
        if(k==0)
            a=24;
        else
            a=8;

        Write_Lcd_Bytes(x,y,p,a);
        y=y+1;
        p=p+24;
        Write_Lcd_Bytes(x,y,p,a);
        y=y+1;
        p=p+24;
        Write_Lcd_Bytes(x,y,p,a);
        y=y+1;
        p=p+24;
        Write_Lcd_Bytes(x,y,p,a);
        
}
//--------------------------------------------------
//�������ƣ� void DisTwo2(UCHAR x,UCHAR y,UCHAR val)
//��������:��ʾ��λ4*5//ʱ��ר��
//��ڲ���:����,ֵ
//���ڲ���:��
//------------------------------------------------
void DisTwo2(UCHAR x,UCHAR y,UCHAR val)
{
        HEX_BCD((USHORT)val);
        if(val<10)
        {
            Write_Lcd_Bytes(x,y,(UCHAR *)NUM5_7_kongge,2);
            Dis4_5_One(x+2,y,TempTemp[0]);
            Write_Lcd_Bytes(x+5,y,(UCHAR *)NUM5_7_kongge,3);
        }
        else
        {
            Dis4_5_One(x,y,TempTemp[1]);
            Write_Lcd_Bytes(x+3,y,(UCHAR *)NUM5_7_kongge,1);
            Dis4_5_One(x+4,y,TempTemp[0]);
	}
}


//----------------------------------------------------------
//�������ƣ�void Pcent(signed long RangeL,signed long RangeH)
//�������ܣ���ʾ75%��25%
//��ڲ�������
//���ڲ�������
//-----------------------------------------------------------
void Pcent(signed long RangeL,signed long RangeH)			   //��ע�����������㷨û�п�����
{
    signed long max,min,temp;
    UCHAR i;
    
    max=RangeL;		     //RangelΪ��������
    min=RangeH;			   //RangelΪ��������
    for(i=0;i<180;i++)
    {
        if(DisCurveBuf[i]>RangeH)
            DisCurveBuf[i]=RangeH;
        if(DisCurveBuf[i]<RangeL)
            DisCurveBuf[i]=RangeL;
        if(max<DisCurveBuf[i])
          max=DisCurveBuf[i];	    //��ȡ����ֵ���ֵ
        if(min>DisCurveBuf[i])		//��ȡ����ֵ��Сֵ
          min=DisCurveBuf[i];
    }
    temp=RangeH-RangeL;			    //���̲�ֵ
    max=(fp32)(max-RangeL)/temp*100+1;
    if(max>100)
      max=100;
    min=(fp32)(min-RangeL)/temp*100;
    
    perl=min/5;
    perl=perl*5;
    
    perh=(100-max)/5;
    perh=perh*5;
    perh=100-perh;
    temp=perh-perl;	                   //��ðٷֱ����ֵ����Сֵ�Ĳ�ֵ
    if(perh<=15)
    {
        temp=20;
        if(perh<20)
           perh=20;
    }
    else if(perh<=35)
    {
        temp=40;
        if(perh<40)
           perh=40;
    }
    else if(perh<=55)
    {
        temp=60;
        if(perh<60)
           perh=60;
    }
    else if(perh<=75)
    {
        temp=80;
        if(perh<80)
           perh=80;
    }
    else if(perh<=100)
    {
        temp=100;
        if(perh<100)
           perh=100;
    }
    perl=perh-temp;
	
	  DisTwo2(0,3,perh-temp/4);
    DisTwo2(0,6,perl+temp/4);

    Write_Lcd_Bytes(8,3,(UCHAR *)&NUM4_5_percent,4);
    Write_Lcd_Bytes(8,6,(UCHAR *)&NUM4_5_percent,4);
}

//------------------------------------------------
//�������ƣ�void DisTwo1(UCHAR x,UCHAR y,UCHAR val) 
//��������:��ʾ��λ5*7
//��ڲ���:����,ֵ
//���ڲ���:��
//------------------------------------------------
void DisTwo1(UCHAR x,UCHAR y,UCHAR val)
{
	HEX_BCD((USHORT)val);
	Dis5_7_One(x,y,TempTemp[1]);
	Dis5_7_One(x+6,y,TempTemp[0]);
}

//---------------------------------------------------
//�������ƣ�void DisTwo(UCHAR x,UCHAR y,UCHAR val)
//��������:��ʾ��λ6*11
//��ڲ���:����,ֵ
//���ڲ���:��
//---------------------------------------------------
void DisTwo(UCHAR x,UCHAR y,UCHAR val)
{
	HEX_BCD((USHORT)val);
	Dis6_11_One(x,y,TempTemp[1]);
	Dis6_11_One(x+8,y,TempTemp[0]);
}
//------------------------------------------------
//�������ƣ�void DisThree(UCHAR x,UCHAR y,USHORT val)
//��������:��ʾ��λ6*11
//��ڲ���:����,ֵ
//���ڲ���:��
//-------------------------------------------------
void DisThree(UCHAR x,UCHAR y,USHORT val)
{
	HEX_BCD((USHORT)val);
	Dis6_11_One(x,y,TempTemp[2]);
	Dis6_11_One(x+8,y,TempTemp[1]);
	Dis6_11_One(x+16,y,TempTemp[0]);
}

//---------------------------------------------------------
//�������ƣ�void PutDisval(signed long val,UCHAR pointt,UCHAR n)
//�������ܣ���һ���з��ŵĲ��Ҵ�С�����nλ���ֱ����TempTemp��
//��ڲ�����Val,pointt,n
//���ڲ�������
//---------------------------------------------------------  
void PutDisval(signed long val,UCHAR pointt,UCHAR n)		 
{
	UCHAR sign,a,i,j;
	unsigned long temp;
	for(i=0;i<7;i++)
	{
		TempTemp[i] = WkonggeW;
	}
	if((n<7)&&(pointt<=n))	         //��ע:���if ����޷����
	{
		sign=0;
		temp=(unsigned long)val;
		if(val<0)					//����
		{
			sign=1;
			temp=(~val)+1;			 //����һ��һ������ʽ��ţ�������Ҫ�ǽ�������λ������λ���䷴������ټ�һ
		}
		LONG_HEX_BCD(temp);			//�õ�5���ֽڵ�BCD��
		if(pointt > 0)				//��С����
		{
			a=n-pointt;					//С������ǰ��λ��
			j=n;
			TempTemp[7]=n+1;           //��1��ԭ������Ϊ��һ��С����
			for(i=0;i<a;i++)
			{
				TempTemp[j]=TempTemp[j-1];
				j--;
			}
			TempTemp[j]=WdianW;			//����С�����λ�ò���С����
			TempTemp[7]=5;			    //��ʾ��һ����Чλ֮ǰλ������д��������
			for(i=n;i>j+1;i--)
			{
				if(TempTemp[i]==0)
				{
					TempTemp[i]=WkonggeW;
					TempTemp[7]=i;			//��ʾ��һ����Чλ֮ǰλ������д��������
				}
				else
				{
					break;
				}
			}
			TempTemp[n+1] = WxiaokonggeW;
		}
		else		                 
		{
			TempTemp[7]=5;			//��ʾ��һ����Чλ֮ǰλ������д��������
			for(i=n;i>0;i--)
			{
				if((TempTemp[i]==0)||(TempTemp[i]==WkonggeW))
				{
					TempTemp[i]=WkonggeW;
					TempTemp[7]=i;			//��ʾ��һ����Чλ֮ǰλ������д��������
				}
				else
				{
					break;
				}
			}
			TempTemp[n+1] = WxiaokonggeW;	//û��С�����ʱ��Ϊ����ʾ����С����ģ������λ��ʾһ��С�ո�
		}
		if(sign==1)
			TempTemp[TempTemp[7]]=WyigongW;
		else
			TempTemp[TempTemp[7]]=WkonggeW;
	}
}
//-------------------------------------------------------------------------------------
//���ܣ���ʾ��С�����nλ��
//��ڣ�TempTemp[n-1~0],С����λ��,x,y���꣬λ��n,    ����k:0��ʾ6*11��1��ʾ24*32,2��ʾ5*7
//���ڣ�
//-------------------------------------------------------------------------------------
void Dis_nseg_Data(UCHAR x,UCHAR y,signed long Val,UCHAR point,UCHAR n,UCHAR k)
{
	UCHAR i,j;
	PutDisval(Val,point,n);	      
	j=n;
	for(i=0;i<n+1;i++)
	{
		if(k==0)
		{
			Dis6_11_One(x,y,TempTemp[j]);	  
			if(TempTemp[j]==WdianW)
				x=x+4;
			else
				x=x+8;
			j=j-1;
		}
		else if(k==1)
		{
			Dis24_32_One(x,y,TempTemp[j]);
			if(TempTemp[j]==WdianW)
				x=x+12;
			else
				x=x+24;
			j=j-1;
		}
		else if(k==2)
		{
			Dis5_7_One(x,y,TempTemp[j]);
			if(TempTemp[j]==WdianW)
				x=x+4;
			else
				x=x+6;
			j=j-1;
		}
	}
}

//------------------------------------------------
//����:��ʾ����
//���:����,ֵ,���壺k  0��ʾ6*11��1��ʾ24*32
//����:��
//------------------------------------------------
void DisData(UCHAR x,UCHAR y,signed long val,UCHAR point,UCHAR n,UCHAR k)
{
	UCHAR i,j;
	PutDisval(val,point,n);	 
	j=n+1;
	if(k==0)	//6*11
	{
		x=x+8*n;
	}
	else if(k==1)
	{
		x=x+24*n;
	}
	else if(k==2)
	{
	 	x=x+6*n;
	}
	for(i=0;i<n+1;i++)
	{
		if(k==0)
		{
			Dis6_11_One(x,y,TempTemp[i]);
			if((TempTemp[i+1]==WdianW)||(TempTemp[i+1]==WxiaokonggeW))
				x=x-4;
			else
				x=x-8;
		}
		else if(k==1)
		{
			Dis24_32_One(x,y,TempTemp[i]);
			if((TempTemp[i+1]==WdianW)||(TempTemp[i+1]==WxiaokonggeW))
				x=x-8;
			else
				x=x-24;
		}
		else if(k==2)
		{
			Dis5_7_One(x,y,TempTemp[i]);   //�������j �ǲ���������?         //j-->i   20180223
			if((TempTemp[i+1]==WdianW)||(TempTemp[i+1]==WxiaokonggeW))
				x=x-4;
			else
				x=x-6;
		}
	}	    
}


//************************************************
//------------------------------------------------
//�������ƣ�void DisUnit(UCHAR x,UCHAR y,UCHAR unitvale)
//��������:��ʾ��λ
//��ڲ���:����,��λ���
//���ڲ���:��
//------------------------------------------------
void DisUnit(UCHAR x,UCHAR y,UCHAR unitvale)
{
  UCHAR i;
	for(i=0;i<7;i++)
	{
		Dis6_11_One(x,y,unittab[unitvale][i]);
		x=x+7;
	}
}

//**********************************************
//----------------------------------------------
//�������ƣ�void DisType(UCHAR x,UCHAR y,UCHAR typevale)
//�������ܣ���ʾ�ź�����
//��ڲ��������ꡢ�ַ�
//���ڲ����� ��
//----------------------------------------------
void DisType(UCHAR x,UCHAR y,UCHAR typevale)
{
  UCHAR i;
	for(i=0;i<7;i++)
	{
		Dis6_11_One(x,y,typetab[typevale][i]);
		x=x+8;
	}
}


//************************************************
//------------------------------------------------
//���ܣ�������
//��ڣ�������
//���ڣ�
//------------------------------------------------
void Dis_Heng_Line(UCHAR y,UCHAR begin,UCHAR end)
{
    UCHAR data,i,a,b;
    data=0;
    a=y/8;		      //x��ַ
    b=y%8;
    switch(b)
    {
        case 0: data |=BIT0;break;
        case 1: data |=BIT1;break;
        case 2: data |=BIT2;break;
        case 3: data |=BIT3;break;
        case 4: data |=BIT4;break;
        case 5: data |=BIT5;break;
        case 6: data |=BIT6;break;
        case 7: data |=BIT7;break;
        default:break;
    }
    for(i=begin;i<=end;i++)				              //���ڻ�һ������
    {
        Write_Lcd_Bytes(i,a,(UCHAR *)&data,1);        
    }
}

//****************************************************
//----------------------------------------------------
//�������ƣ� void Dis_Shu_dian(UCHAR x,UCHAR begin,UCHAR end,UCHAR Curve_F)
//��������:  �����������
//��ڲ���:������
//���ڲ���:��
//----------------------------------------------------
void Dis_Shu_dian(UCHAR x,UCHAR begin,UCHAR end,UCHAR Curve_F)	 
{
      UCHAR i,a,b;
      for(i=begin;i<=end;i++)
      {
          a=i/8;	
          b=i%8;
          if(a<8)
          {
              switch(b)
              {
                  case 0: TempTemp[a] |=BIT0;break;	//����
                  case 1: TempTemp[a] |=BIT1;break;	//����
                  case 2: TempTemp[a] |=BIT2;break;	//����
                  case 3: TempTemp[a] |=BIT3;break;	//����
                  case 4: TempTemp[a] |=BIT4;break;	//����
                  case 5: TempTemp[a] |=BIT5;break;	//����
                  case 6: TempTemp[a] |=BIT6;break;	//����
                  case 7: TempTemp[a] |=BIT7;break; //����
                  default:  break;
              }
          }
      }

      i=0;
      while(TempTemp[i]==0)
      {
          i++;		              
      }
      a=i;
      i=7;
      while(TempTemp[i]==0)
      {
          i--;
      }						     
      b=i;
      for(i=a;i<=b;i++)			//��i=a��ʼ   ��i=6	֮����ʾ����
      {      
              Write_Lcd_Bytes(x,i,(UCHAR *)&TempTemp[i],1);     //�������������     
      }
}

//************************************************
//------------------------------------------------
//����:�����������
//���:�����꣬��END
//����:��
//------------------------------------------------
void Dis_Shu_Line(UCHAR x,UCHAR begin,UCHAR end)		       
{
    UCHAR i;
    for(i=0;i<8;i++)
    {
        TempTemp[i]=0;
    }
    Dis_Shu_dian(x,begin,end,0);
}

//************************************************
//------------------------------------------------
//����:�����߻������������
//���:�����꣬��END
//����:��
//------------------------------------------------
void Dis_Shu_Line1(UCHAR x,UCHAR begin,UCHAR end)  
{
    UCHAR i;
    for(i=0;i<8;i++)
    {
        TempTemp[i]=0;
    }
    TempTemp[2]=0x01;
    TempTemp[7]=0x80;
    if(x%3==0)
    {
        TempTemp[3]=0x08;
        TempTemp[6]=0x08;
    }
    Dis_Shu_dian(x,begin,end,1);
}

//********************************************
//--------------------------------------------
//�������ƣ�void Rectangle(UCHAR x,UCHAR y)
//���ܣ������Ŀ�
//��ڣ�����
//���ڣ���
//--------------------------------------------
void Rectangle(UCHAR x,UCHAR y)
{
    Write_Lcd_Bytes(x,y,(UCHAR *)RELAY_OPEN,8);
}

//********************************************
//--------------------------------------------
//�������ƣ�void Rectangle1(UCHAR x,UCHAR y)
//�������ܣ���ʵ�Ŀ�
//��ڲ���������
//���ڲ�������
//--------------------------------------------
void Rectangle1(UCHAR x,UCHAR y)
{
       Write_Lcd_Bytes(x,y,(UCHAR *)RELAY_CLOSE,8);
}
//******************************************
//------------------------------------------

//�������ƣ�void Dis_PID(UCHAR x,UCHAR y)
//�������ܣ���ʾ6*11��PID�ַ�
//��ڲ���������
//���ڲ�������
//------------------------------------------
//******************************************
void Dis_PID(UCHAR x,UCHAR y)
{
      Dis6_11_One(x,y,WPW);
      Dis6_11_One(x+8,y,WIW);
      Dis6_11_One(x+16,y,WDW);
}

//*****************************
//-----------------------------
//�������ƣ�void Dis_LOOP(UCHAR x,UCHAR y)
//�������ܣ���ʾLOOP
//��ڲ���������
//���ڲ�������
//-----------------------------
void Dis_LOOP(UCHAR x,UCHAR y)
{
      Dis6_11_One(x,y,WLW);
      Dis6_11_One(x+8,y,WOW);
      Dis6_11_One(x+16,y,WOW);
      Dis6_11_One(x+24,y,WPW);
}

//*****************************
//-----------------------------
//�������ƣ�void Dis_CH(UCHAR x,UCHAR y)
//�������ܣ���ʾ6*11��CH�ַ�
//��ڲ���������
//���ڲ�������
//-----------------------------
void Dis_CH(UCHAR x,UCHAR y)
{
      Dis6_11_One(x,y,WCW);
      Dis6_11_One(x+8,y,WHW);
}

//*************************************************
//-------------------------------------------------
//�������ƣ�void Write_nKongge(UCHAR x,UCHAR y,UCHAR counter)
//�������ܣ�����д�ո�
//��ڲ����������꣬�ո����
//���ڲ�������
//-------------------------------------------------
void Write_nKongge(UCHAR x,UCHAR y,UCHAR counter)
{
    UCHAR i;
    for(i=0;i<counter;i++)
    {
        Dis6_11_One(x,y,WkonggeW);
        x=x+8;
    }
}

//********************************
//--------------------------------
//�������ƣ�void DisSmloc(UCHAR i)
//���ܣ�����ñ�־
//��ڣ����ֵ
//���ڣ���
//--------------------------------
void DisSmloc(UCHAR i)
{
    if(Smloc==i)
	Smloc_F=1;	      //��д
    else
	Smloc_F=0;		  //����д
}

//***********************************************************************
//-----------------------------------------------------------------------
//�������ƣ�UCHAR CAL_curve(UCHAR j,signed long RangeL,signed long RangeH)
//���ܣ�����DisCurveBuf�����ֵ�������߻���������
//��ڣ�j ����  ��Χ����  ��Χ����
//���ڣ�������
//-----------------------------------------------------------------------
UCHAR CAL_curve(UCHAR j,signed long RangeL,signed long RangeH)	  // (�������ڵ�������100-200�ķ�Χ)
{														          //����DisCurveBuf[j]==150;  ��ô��ʾӦ�������м�����ߵ�����
    UCHAR data;
    signed long temp,temp1,tempr;							     
    tempr=RangeH-RangeL;   //���̷�Χ
    temp=DisCurveBuf[j]-((SHORT)((fp32)perl/100*tempr)+RangeL);	  //�����  ֣��˵���������������ģ����޵�ֵ�ڷ�Χ�İ�ֱ�  15%*(200-100)+100=115	 tempֵ=35	 ��������ֵ�ڷ�Χ�ڵ�ֵ�Ƕ��
    temp1=(signed long)((fp32)(perh-perl)/100*tempr);			  //��������ֵ���������еĴ�С+��������
    if(temp1==0)												  //˵���������������ģ�(75%-15%)*100=60
       temp1=1;													  //(��������-��������)*���̷�Χ
    data=63-(UCHAR)((fp32)temp/temp1*47+0.5);                     // data=63-(35/60)*47 =35              (ʵ�ʵĲ���ֵ���������̷�Χ�ڵı�ֵ)*��ʾ�ķ�Χ��� 0.5��Ҫ��������������    
    return data;
}


//************************************************
//------------------------------------------------
//���ܣ���180�������
//��ڣ�
//���ڣ�
//------------------------------------------------
void DisCurve(signed long RangeL,signed long RangeH)	 
{
    UCHAR data,data1,i,j;
    data=CAL_curve(179,RangeL,RangeH);    
    Dis_Shu_Line1(14,data,data);		   
    j=178;
    for(i=13;i<128;i++)		   //192-->128 	                   				
    {
        data1=CAL_curve(j,RangeL,RangeH);
        if(data==data1)
        { 
            Dis_Shu_Line1(i,data,data);
        }
        else if(data<data1)
        {
            Dis_Shu_Line1(i,data+1,data1);
            data=data1;  
        } 
        else if(data>data1)
        {
            Dis_Shu_Line1(i,data1,data-1); 
            data=data1;
        }
        j--; 
    }
}

//*************************************************
//-------------------------------------------------
//����:����������ʾ����
//���:��
//����:��
//-------------------------------------------------
#if SOFT==Chinese
/*�������˵�*/


/*���˵�   <�۳�>*/
/* ����     ����*/
/* ��ѯ     ά��*/
/* 01-21  13:32:53 */
void Main_Menu_display(void)
{
	
 // if(Dismode!=OldDismode)       //���˵�
	{
		
		Write_LCD_CChina(8,0,(UCHAR *)ZHU);    //���˵�   
		Write_LCD_CChina(24,0,(UCHAR *)CAI_1);   
		Write_LCD_CChina(40,0,(UCHAR *)DAN);
		Dis6_11_One(72,0, WleftKH);      // < �ո��м���Ҫ��ʾ����A ����B ���߷۳�
		Dis6_11_One(104,0,WrightKH);     // >
		
	}
	
	DisSmloc(0);
	Write_LCD_CChina(24,2,(UCHAR *)SHE);                //����
	Write_LCD_CChina(40,2,(UCHAR *)ZHI3);  
	DisSmloc(1);
	Write_LCD_CChina(80,2,(UCHAR *)CAI);                //����
	Write_LCD_CChina(96,2,(UCHAR *)YANG);  
	DisSmloc(2);
	Write_LCD_CChina(24,4,(UCHAR *)CHA_1);              //��ѯ
	Write_LCD_CChina(40,4,(UCHAR *)XUN_1);
	DisSmloc(3);                                       
	Write_LCD_CChina(80,4,(UCHAR *)WEI_W);              //ά��
	Write_LCD_CChina(96,4,(UCHAR *)HU);
	
	//��������ʾʱʱ��,lulj 20180227	
	//Dis5_7DatandtimeNew(16,7,'ϵͳʱ��');             lulj 20180227 
	 Smloc_F=0; 
	
	 OldDismode=Dismode;	                              //�����Dismodeֵ
		
	
}

/*��������*/

/* ��������  <�۳�> */
/* ����ʱ��  01:00  */
/* ���ʱ��  90:00  */
/* ���� 1           */
void Sample_Set_display(void)
{
	
	 //if(Dismode!=OldDismode)       //�������ò˵�
	{
		Write_LCD_CChina(8,0,(UCHAR *)CAI);//��������
		Write_LCD_CChina(24,0,(UCHAR *)YANG);
		Write_LCD_CChina(40,0,(UCHAR *)SHE);
		Write_LCD_CChina(56,0,(UCHAR *)ZHI3);
		
		Dis6_11_One(80,0, WleftKH);      // < �ո��м���Ҫ��ʾ����A ����B ���߷۳�
		Dis6_11_One(112,0,WrightKH);     // >
		
		Write_Lcd_Bytes(88,2,(UCHAR *)NUM5_7_maohao,5);  //��ʾ :
		Write_Lcd_Bytes(88,4,(UCHAR *)NUM5_7_maohao,5);  //��ʾ :
	}
	
	 	DisSmloc(0);
		Write_LCD_CChina(8,2,(UCHAR *)CAI);         //����ʱ��
		Write_LCD_CChina(24,2,(UCHAR *)YANG);
		Write_LCD_CChina(40,2,(UCHAR *)SHI);
		Write_LCD_CChina(56,2,(UCHAR *)JIAN);
		DisSmloc(1);
	  //Dis6_11_One(80,2,'����ʱ��-ʱ');
		DisSmloc(2);
	  //Dis6_11_One(96,2,'����ʱ��-��');
		DisSmloc(3);
		Write_LCD_CChina(8,4,(UCHAR *)JIAN);        //���ʱ��
		Write_LCD_CChina(24,4,(UCHAR *)GE);
		Write_LCD_CChina(40,4,(UCHAR *)SHI); 
		Write_LCD_CChina(56,4,(UCHAR *)JIAN);
		DisSmloc(4);
	  //Dis6_11_One(80,4,'���ʱ��-ʱ');
		DisSmloc(5);
		//Dis6_11_One(96,4,'���ʱ��-��');
		DisSmloc(6);
		Write_LCD_CChina(8,6,(UCHAR *)CI_1);       //����
		Write_LCD_CChina(24,6,(UCHAR *)SHU1);
		DisSmloc(7);
		//��ʾ���� 
		DisSmloc(8);
		
	  Smloc_F=0; 
	 OldDismode=Dismode;	                              //�����Dismodeֵ
	
	
}


/*��������*/

/*��������  <�۳�>  */
/*��ǰʱ�� = 13:41  */
/*��ʱʱ��   00:00  */
/*  ����      ����  */
void Start_Sample_display(void)
{
		//if(Dismode!=OldDismode)       //���������˵�
		{
		 Write_LCD_CChina(0,0,(UCHAR *)QI_1);//��������
		 Write_LCD_CChina(16,0,(UCHAR *)DONG);
		 Write_LCD_CChina(32,0,(UCHAR *)CAI);
		 Write_LCD_CChina(48,0,(UCHAR *)YANG);
			
		 Dis6_11_One(80,0, WleftKH);    // < �ո��м���Ҫ��ʾ����A ����B ���߷۳�
		 Dis6_11_One(112,0,WrightKH);    // >
		
			Dis6_11_One(96,2,WmaohaoW);
			Dis6_11_One(96,4,WmaohaoW);
//		 Write_Lcd_Bytes(96,3,(UCHAR *)NUM5_7_maohao,5);  //��ʾ :
//		 Dis6_11_One(96,2,NUM6_11_EquaL);
//		 Write_Lcd_Bytes(96,4,(UCHAR *)NUM5_7_maohao,5);  //��ʾ :
		}
		
		Write_LCD_CChina(0,2,(UCHAR *)DANG); //��ǰʱ�� =
		Write_LCD_CChina(16,2,(UCHAR *)QIAN_1);
		Write_LCD_CChina(32,2,(UCHAR *)SHI);
		Write_LCD_CChina(48,2,(UCHAR *)JIAN);
		Dis6_11_One(64,2,WEqualW);
		
	  DisSmloc(0);
		Write_LCD_CChina(0,4,(UCHAR *)YAN_2);                  //��ʱʱ��
		Write_LCD_CChina(16,4,(UCHAR *)SHI);
		Write_LCD_CChina(32,4,(UCHAR *)SHI);
		Write_LCD_CChina(48,4,(UCHAR *)JIAN);
		DisSmloc(1);
		Write_LCD_CChina(16,6,(UCHAR *)TIAO);                   //����
		Write_LCD_CChina(32,6,(UCHAR *)LING);
		DisSmloc(2);
		Write_LCD_CChina(80,6,(UCHAR *)QI_1);
		Write_LCD_CChina(96,6,(UCHAR *)DONG);
		DisSmloc(3);
		Smloc_F=0; 
		OldDismode=Dismode;	                              //�����Dismodeֵ
	
}


/* ����A       87[1/5] */
/* ��ʼ: 1-10  14:12   */
/* �ɼ�����:  1        */
/* ���ô���:  1        */
/* ����A       87[1/5] */
/* ��ʼ: 1-10  14:12   */
/* �ɼ�����:  1        */
/* ���ô���:  1        */
void  First_Set_display(void)
{
	//if(Dismode!=OldDismode)       //���������˵�
	{
		Dis6_11_One(88,0, WFleftKH);    //  [
		Dis6_11_One(112,0,WFrightKH);    // ]
		Write_LCD_CChina(0,2,(UCHAR *)KAI);  //��ʼ
		Write_LCD_CChina(16,2,(UCHAR *)SHI_1);
		Dis6_11_One(32,2,WmaohaoW);          //:
		Write_LCD_CChina(0,4,(UCHAR *)CAI); //��������
		Write_LCD_CChina(16,4,(UCHAR *)YANG);
		Write_LCD_CChina(32,4,(UCHAR *)CI_1);
		Write_LCD_CChina(48,4,(UCHAR *)SHU1);
		Dis6_11_One(64,4,WmaohaoW);          //:
		Write_LCD_CChina(0,6,(UCHAR *)SHE);  //���ô���
		Write_LCD_CChina(16,6,(UCHAR *)ZHI3);
		Write_LCD_CChina(32,6,(UCHAR *)CI_1);
		Write_LCD_CChina(48,6,(UCHAR *)SHU1);
		Dis6_11_One(64,6,WmaohaoW);          //:

	}
	
}


/*�۳�        64[2/4]  */
/*�¶�:       12.66 C  */
/*ѹ��:       -2.19kpa */
/*����ѹ:     102.04kpa*/

void  Second_Set_display(void)
{
	//if(Dismode!=OldDismode)       
	{
		 Dis6_11_One(88,0, WFleftKH);    //  [
		 Dis6_11_One(112,0,WFrightKH);   //  ]
		 Write_LCD_CChina(0,2,(UCHAR *)WEN_1);
		 Write_LCD_CChina(32,2,(UCHAR *)DU);
		 Dis6_11_One(48,2,WmaohaoW);          //:
		 DisUnit(88,2,19);                    //��
		 Write_LCD_CChina(0,4,(UCHAR *)YA);   //ѹ��
		 Write_LCD_CChina(32,4,(UCHAR *)LI_1);
		 Dis6_11_One(48,4,WmaohaoW);          //:
		 DisUnit(88,4,13);                    //kpa
		 Write_LCD_CChina(0,6,(UCHAR *)DA);   //����ѹ
		 Write_LCD_CChina(16,6,(UCHAR *)QI_2);
		 Write_LCD_CChina(32,6,(UCHAR *)YA);
		 Dis6_11_One(48,6,WmaohaoW);          //:
		 DisUnit(88,6,13);                    //kpa

	}
}


/*�۳�        64[2/4]  */
/*����:       0.072L/m */
/*���:       0.068L/m */
/*����:       0.100L/m */

void  Third_Set_display(void)
{
	//if(Dismode!=OldDismode)  
	{
	   Dis6_11_One(88,0, WFleftKH);    //  [
		 Dis6_11_One(112,0,WFrightKH);   //  ]
		 Write_LCD_CChina(0,2,(UCHAR *)GONG);
		 Write_LCD_CChina(20,2,(UCHAR *)KUANG);
		 DisUnit(72,2,7);                    //L/min
		 Write_LCD_CChina(0,4,(UCHAR *)BIAO);
		 Write_LCD_CChina(20,4,(UCHAR *)KUANG);
		 DisUnit(72,4,7);                    //L/min
		 Write_LCD_CChina(0,6,(UCHAR *)SHE);
		 Write_LCD_CChina(20,6,(UCHAR *)ZHI3);
		 DisUnit(72,6,7);                    //L/min
		
	}
}


//ά���������

/*ά������: 000000*/

void Enter_maintain_mima_display(void)
{
	if(Dismode!=OldDismode)
	{
		LCD_Clear();                 //����
		Write_LCD_CChina(32,2,(UCHAR *)WEI_W); //ά������    
		Write_LCD_CChina(48,2,(UCHAR *)HU);
		Write_LCD_CChina(64,2,(UCHAR *)MI);
		Write_LCD_CChina(80,2,(UCHAR *)MA);
		Dis6_11_One(100,2, WmaohaoW);          // :
	}
	DisSmloc(1);
	DisTwo(32,4,Syspar1.Mcode[0]);	        //xx
	DisSmloc(2);
	DisTwo(64,4,Syspar1.Mcode[1]);	        //xx
  DisSmloc(3);
	DisTwo(96,4,Syspar1.Mcode[2]);	        //xx
	Smloc_F=0;
	OldDismode=Dismode;	                    //�����Dismodeֵ
}



void Enter_mima_display(void)
{
        if(Dismode!=OldDismode)
        {
            LCD_Clear();                        //����
            Write_LCD_CChina(48,0,(UCHAR *)SHU);//������̬����
            Write_LCD_CChina(64,0,(UCHAR *)RU);
            Write_LCD_CChina(80,0,(UCHAR *)SHE);
            Write_LCD_CChina(96,0,(UCHAR *)ZHI3);
            Write_LCD_CChina(112,0,(UCHAR *)MI);
            Write_LCD_CChina(128,0,(UCHAR *)MA);
            Dis5_7_One(85,7,WVW);
            Dis5_7_One(91,7,W1W);
            Dis5_7_One(97,7,WdianW);
            Dis5_7_One(102,7,W1W);
            //Dis5_7_One(108,7,W0W);
            //Dis5_7_One(114,7,W0W);
	}
	DisSmloc(1);
	DisTwo(56,3,Syspar1.Mcode[0]);	   //xx
	DisSmloc(2);
	DisTwo(88,3,Syspar1.Mcode[1]);	   //xx
	DisSmloc(3);
	DisTwo(120,3,Syspar1.Mcode[2]);	   //xx
	DisSmloc(0);
	Write_LCD_CChina(0,6,(UCHAR *)TUI);//�˳�
	Write_LCD_CChina(16,6,(UCHAR *)CHU);
	DisSmloc(4);
	Write_LCD_CChina(160,6,(UCHAR *)JIN);//����
	Write_LCD_CChina(176,6,(UCHAR *)RU);
	Smloc_F=0;
	OldDismode=Dismode;	                 //�����Dismodeֵ
}
#endif


//*************************************************
//-------------------------------------------------
//����:����̬��ʾ����
//���:��
//����:��
//-------------------------------------------------
#if SOFT==Chinese
void Zhu_zutai_display(void)
{
        if(Dismode!=OldDismode)
        {
            LCD_Clear();
            Dis_Shu_Line(33,0,63);	   //��Y��ַ����33λ�û�һ������
            Dis_Shu_Line(34,0,63);	   //��Y��ַ����34λ�û�һ������
	}
	DisSmloc(0);
	Write_LCD_CChina(0,6,(UCHAR *)TUI);//�˳�
	Write_LCD_CChina(16,6,(UCHAR *)CHU);
	DisSmloc(1);
	Write_LCD_CChina(37,1,(UCHAR *)SHU);//����
	Write_LCD_CChina(53,1,(UCHAR *)RU);
	DisSmloc(2);
	Write_LCD_CChina(78,1,(UCHAR *)BAO1);//����
	Write_LCD_CChina(94,1,(UCHAR *)JING);
	DisSmloc(3);
	Write_LCD_CChina(119,1,(UCHAR *)TONG1);//ͨѶ
	Write_LCD_CChina(135,1,(UCHAR *)XUN);
	DisSmloc(4);	
	Write_LCD_CChina(159,1,(UCHAR *)XI);//ϵͳ
	Write_LCD_CChina(175,1,(UCHAR *)TONG);
	DisSmloc(5);	
	Write_LCD_CChina(37,4,(UCHAR *)WANG);//����
	Write_LCD_CChina(53,4,(UCHAR *)LUO);
    DisSmloc(6);	
	Write_LCD_CChina(78,4,(UCHAR *)BEI);//����
	Write_LCD_CChina(94,4,(UCHAR *)FEN);
    //DisSmloc(7);	
	//Write_LCD_CChina(119,4,(UCHAR *)BEI);//����	 		 //������������
	//Write_LCD_CChina(135,4,(UCHAR *)FEN);
	Smloc_F=0;
	OldDismode=Dismode;
}
#endif

#if SOFT==English
void Zhu_zutai_display(void)
{
        if(Dismode!=OldDismode)
        {
            LCD_Clear();
            Dis_Shu_Line(29,0,63);
            Dis_Shu_Line(30,0,63);
	}
        DisSmloc(0);
	Dis6_11_One(0,6,WEW);//Exit
	Dis6_11_One(8,6,WxW);
	Dis6_11_One(15,6,WiW);
	Dis6_11_One(20,6,WtW);
	DisSmloc(1);
	Dis6_11_One(36,1,WIW);//Input
        Dis6_11_One(42,1,WnW);
        Dis6_11_One(49,1,WpW);
        Dis6_11_One(57,1,WuW);
        Dis6_11_One(64,1,WtW);
	DisSmloc(2);
	Dis6_11_One(82,1,WOW);//Output
        Dis6_11_One(90,1,WuW);
        Dis6_11_One(97,1,WtW);
        Dis6_11_One(103,1,WpW);
        Dis6_11_One(111,1,WuW);
        Dis6_11_One(118,1,WtW);
	DisSmloc(3);
	Dis6_11_One(136,1,WCW);//Control
        Dis6_11_One(143,1,WoW);
        Dis6_11_One(150,1,WnW);
        Dis6_11_One(156,1,WtW);
        Dis6_11_One(161,1,WrW);
        Dis6_11_One(167,1,WoW);
	Dis6_11_One(174,1,WlW);
	DisSmloc(4);
	Dis6_11_One(36,4,WAW);//Alarm
        Dis6_11_One(44,4,WlW);
        Dis6_11_One(49,4,WaW);
        Dis6_11_One(56,4,WrW);
        Dis6_11_One(63,4,WmW);
	DisSmloc(5);
	Dis6_11_One(82,4,WCW);//Comm
        Dis6_11_One(89,4,WoW);
        Dis6_11_One(97,4,WmW);
        Dis6_11_One(105,4,WmW);
	DisSmloc(6);	
	Dis6_11_One(136,4,WSW);//System
        Dis6_11_One(142,4,WyW);
        Dis6_11_One(149,4,WsW);
        Dis6_11_One(155,4,WtW);
        Dis6_11_One(160,4,WeW);
        Dis6_11_One(168,4,WmW);
	Smloc_F=0;
        OldDismode=Dismode;
}
#endif
//*************************************************
//-------------------------------------------------
//����:ϵͳ��̬��ʾ����
//���:��
//����:��
//-------------------------------------------------
#if SOFT==Chinese
void Xitong_zutai_display(void)
{
        if(Dismode!=OldDismode)
        {
            LCD_Clear();
            Write_LCD_CChina(0,0,(UCHAR *)XI);//ϵͳ
            Write_LCD_CChina(16,0,(UCHAR *)TONG);
            Dis_Shu_Line(33,0,63);    //��Y��ַ����33λ�û�һ������        
            Dis_Shu_Line(34,0,63);	  //��Y��ַ����34λ�û�һ������
            Write_LCD_CChina(37,0,(UCHAR *)RI);//����
            Write_LCD_CChina(53,0,(UCHAR *)QI);
            Write_LCD_CChina(37,2,(UCHAR *)SHI);//ʱ��
	    Write_LCD_CChina(53,2,(UCHAR *)JIAN);
	    Write_LCD_CChina(37,4,(UCHAR *)MI);//����
	    Write_LCD_CChina(53,4,(UCHAR *)MA);
	    Write_LCD_CChina(37,6,(UCHAR *)DUI);//�Աȶ�
	    Write_LCD_CChina(53,6,(UCHAR *)BI);
	    Write_LCD_CChina(69,6,(UCHAR *)DU);
	    Dis6_11_One(128,6,WpercentW);		 //��ʾһ���ٷֺ�
	}
	DisSmloc(0);
	Write_LCD_CChina(0,6,(UCHAR *)FAN);//����
	Write_LCD_CChina(16,6,(UCHAR *)HUI);

	DisSmloc(1);
        DisTwo(77,0,Systime.Year);  //��
        Smloc_F=0;
        Dis6_11_One(93,0,WyigongW);
	DisSmloc(2);       
	DisTwo(101,0,Systime.Month);    //��
	Smloc_F=0;
	Dis6_11_One(117,0,WyigongW);
	DisSmloc(3);
	DisTwo(125,0,Systime.Day);      //��
	DisSmloc(4);
	DisTwo(77,2,Systime.Hour);      //ʱ
	Smloc_F=0;
        Dis6_11_One(93,2,WmaohaoW);	//ð�Ų��÷�д
        DisSmloc(5);
	DisTwo(101,2,Systime.Minute);   //��
	Smloc_F=0;					   	//ð�Ų��÷�д
	Dis6_11_One(117,2,WmaohaoW);
	DisSmloc(6);
	DisTwo(125,2,Systime.Second);   //��
	DisSmloc(7);
	DisTwo(77,4,Syspar.Ncode[0]);
	DisSmloc(8);
	DisTwo(101,4,Syspar.Ncode[1]);
	DisSmloc(9);
	DisTwo(125,4,Syspar.Ncode[2]);
	DisSmloc(10);
	Dis_nseg_Data(93,6,(SHORT)Syspar.PWM,0,3,0);//����ռ�ձ�
	Smloc_F=0;
	OldDismode=Dismode;
}
#endif

#if SOFT==English
void Xitong_zutai_display(void)
{
        if(Dismode!=OldDismode)
        {
            LCD_Clear();
            
            Dis_Shu_Line(160,50,60);
            Dis_Shu_Line(161,50,60);

            Dis6_11_One(0,0,WDW);//Date
            Dis6_11_One(7,0,WaW);
            Dis6_11_One(14,0,WtW);
            Dis6_11_One(19,0,WeW);
            
	    Dis6_11_One(0,2,WTW);//Time
            Dis6_11_One(7,2,WiW);
            Dis6_11_One(12,2,WmW);
            Dis6_11_One(19,2,WeW);

	    Dis6_11_One(0,4,WCW);//Code
            Dis6_11_One(7,4,WoW);
            Dis6_11_One(14,4,WdW);
            Dis6_11_One(22,4,WeW);

	    Dis6_11_One(0,6,WCW);//Contrast
            Dis6_11_One(7,6,WoW);
            Dis6_11_One(14,6,WnW);
            Dis6_11_One(20,6,WtW);
            Dis6_11_One(25,6,WrW);
            Dis6_11_One(31,6,WaW);
            Dis6_11_One(38,6,WsW);
            Dis6_11_One(44,6,WtW);
	    
	    Dis6_11_One(97,6,WpercentW);
	}
	DisSmloc(0);
        Dis6_11_One(163,6,WBW);//Back
        Dis6_11_One(170,6,WaW);
        Dis6_11_One(177,6,WcW);
        Dis6_11_One(184,6,WkW);
	DisSmloc(1);
        DisTwo(41,0,Systime.Year);//��
        Smloc_F=0;
        Dis6_11_One(57,0,WyigongW);
	DisSmloc(2);       
	DisTwo(65,0,Systime.Month);//��
	Smloc_F=0;
	Dis6_11_One(81,0,WyigongW);
	DisSmloc(3);
	DisTwo(89,0,Systime.Day);//��
	DisSmloc(4);
	DisTwo(41,2,Systime.Hour);//ʱ
	Smloc_F=0;
        Dis6_11_One(57,2,WmaohaoW);
        DisSmloc(5);
	DisTwo(65,2,Systime.Minute);//��
	Smloc_F=0;
	Dis6_11_One(81,2,WmaohaoW);
	DisSmloc(6);
	DisTwo(89,2,Systime.Second);//��
	DisSmloc(7);
	DisTwo(41,4,Syspar.Ncode[0]);
	DisSmloc(8);
	DisTwo(65,4,Syspar.Ncode[1]);
	DisSmloc(9);
	DisTwo(89,4,Syspar.Ncode[2]);
	DisSmloc(10);
	Dis_nseg_Data(65,6,(SHORT)Syspar.CCR2_Number,0,3,0);
	Smloc_F=0;
	OldDismode=Dismode;
}
#endif
//*************************************************
//-------------------------------------------------
//����:ͨ����̬��ʾ����
//���:��
//����:��
//-------------------------------------------------
void Tongdao_zutai1_display(void)
{        
    if(Dismode==3)
    {    
        if(Dismode!=OldDismode)	              
        {
            LCD_Clear();
            Write_LCD_CChina(0,0,(UCHAR *)SHU);//����
            Write_LCD_CChina(16,0,(UCHAR *)RU);
            Dis_Shu_Line(33,0,63);
            Dis_Shu_Line(34,0,63);
    
            Write_LCD_CChina(37,0,(UCHAR *)TONG1);//ͨ��
            Write_LCD_CChina(53,0,(UCHAR *)DAO);
            Write_LCD_CChina(101,0,(UCHAR *)XIN);//�ź�
            Write_LCD_CChina(117,0,(UCHAR *)HAO);
            Write_LCD_CChina(37,2,(UCHAR *)LIANG);//����
            Write_LCD_CChina(53,2,(UCHAR *)CHENG);
            Write_LCD_CChina(37,4,(UCHAR *)DAN);//��λ
            Write_LCD_CChina(53,4,(UCHAR *)WEI);
            Write_LCD_CChina(136,4,(UCHAR *)KAI);//����
            Write_LCD_CChina(152,4,(UCHAR *)FANG);
            Write_LCD_CChina(37,6,(UCHAR *)JI);//��¼���
            Write_LCD_CChina(53,6,(UCHAR *)LU);
            Write_LCD_CChina(69,6,(UCHAR *)JIAN);
            Write_LCD_CChina(85,6,(UCHAR *)GE);
            Dis6_11_One(141,6,WSW);
		}
	DisSmloc(0);
	Write_LCD_CChina(0,6,(UCHAR *)FAN);//����
	Write_LCD_CChina(16,6,(UCHAR *)HUI);	
	DisSmloc(1);
	Dis6_11_One(77,0,Chn+1);              
	DisSmloc(2);						//ͨ����
	DisType(135,0,ChannelConfig[Chn].Type); //�ź�����
	DisSmloc(3);
	Dis6_11_One(73,2,WkonggeW);		        
 	Dis6_11_One(81,2,WkonggeW);
	Dis6_11_One(89,2,WkonggeW);
	Dis6_11_One(97,2,WkonggeW);
	Dis6_11_One(105,2,WkonggeW);
	Dis6_11_One(113,2,WkonggeW);          
	DisData(73,2,ChannelConfig[Chn].RangeL,ChannelConfig[Chn].Point,5,0);  //��ʾ��������
	Smloc_F=0;
	Dis6_11_One(129,2,WyigongW);            //��ʾ"-"
	DisSmloc(4);
	Dis6_11_One(142,2,WkonggeW);
	Dis6_11_One(150,2,WkonggeW);
	Dis6_11_One(158,2,WkonggeW);
	Dis6_11_One(166,2,WkonggeW);
	Dis6_11_One(174,2,WkonggeW);
	Dis6_11_One(182,2,WkonggeW);
	DisData(142,2,ChannelConfig[Chn].RangeH,ChannelConfig[Chn].Point,5,0);
	DisSmloc(5);
	DisUnit(73,4,ChannelConfig[Chn].Unit);	         //��ʾ��λ
	DisSmloc(6);
	if(ChannelConfig[Chn].Kaifang_F==0)			  	 //xiao bug
		Write_LCD_CChina(176,4,(UCHAR *)FOU);
	if(ChannelConfig[Chn].Kaifang_F==1)
		Write_LCD_CChina(176,4,(UCHAR *)SHI1);
	DisSmloc(7);
	//LONG_HEX_BCD(ChannelConfig[Chn].Record.RecInt);
	Dis_nseg_Data(109,6,(SHORT)ChannelRunValue[Chn].RecInt,0,3,0);	   //��ʾ��¼���   s
	Smloc_F=0;	      
	OldDismode=Dismode;
    }
    else if(Dismode==16)		      //��δ������������
    {        
        DisSmloc(0);
        if(LiChengTemp[0]==12)
			Dis6_11_One(73,2,WyigongW);
        else 
			Dis6_11_One(73,2,WkonggeW);
        DisSmloc(1);
        Dis6_11_One(81,2,LiChengTemp[1]);
        DisSmloc(2);
        Dis6_11_One(89,2,LiChengTemp[2]);
        DisSmloc(3);
        Dis6_11_One(97,2,LiChengTemp[3]);
        DisSmloc(4);
        Dis6_11_One(105,2,LiChengTemp[4]);
        DisSmloc(5);
        Dis6_11_One(113,2,LiChengTemp[5]);
        Smloc_F=0;	
    }
    else if(Dismode==17)
    {
        DisSmloc(0);
        if(LiChengTemp[0]==12)Dis6_11_One(142,2,WyigongW);
        else Dis6_11_One(142,2,WkonggeW);
        DisSmloc(1);
        Dis6_11_One(150,2,LiChengTemp[1]);
        DisSmloc(2);
        Dis6_11_One(158,2,LiChengTemp[2]);
        DisSmloc(3);
        Dis6_11_One(166,2,LiChengTemp[3]);
        DisSmloc(4);
        Dis6_11_One(174,2,LiChengTemp[4]);
        DisSmloc(5);
        Dis6_11_One(182,2,LiChengTemp[5]); 
        Smloc_F=0;	
    }
}


//************************************************
//------------------------------------------------
//����:ͨ����̬2����
//���:��
//����:��
//------------------------------------------------
void	Tongdao_zutai2_display(void)
{
        if(Dismode!=OldDismode)
        {
            LCD_Clear();
            Write_LCD_CChina(0,0,(UCHAR *)SHU);//����
            Write_LCD_CChina(16,0,(UCHAR *)RU);
            Dis_Shu_Line(33,0,63);
            Dis_Shu_Line(34,0,63);
            Write_LCD_CChina(37,0,(UCHAR *)LV1);//�˲�ʱ��
            Write_LCD_CChina(53,0,(UCHAR *)BO);
            Write_LCD_CChina(69,0,(UCHAR *)SHI);
            Write_LCD_CChina(85,0,(UCHAR *)JIAN);
            Dis6_11_One(133,0,WSW);
            Write_LCD_CChina(37,2,(UCHAR *)XIAO);//С�ź��г�
            Write_LCD_CChina(53,2,(UCHAR *)XIN);
            Write_LCD_CChina(69,2,(UCHAR *)HAO);
            Write_LCD_CChina(85,2,(UCHAR *)QIE);
            Write_LCD_CChina(101,2,(UCHAR *)CHU1);
            Dis6_11_One(162,2,WpercentW);        //��ʾС�ź��г����ֺ���İٷֺ�
            Write_LCD_CChina(37,4,(UCHAR *)LING);//���Ǩ��
            Write_LCD_CChina(53,4,(UCHAR *)DIAN);
            Write_LCD_CChina(69,4,(UCHAR *)QIAN);
            Write_LCD_CChina(85,4,(UCHAR *)YI1);
	}
	DisSmloc(8);
	Dis_nseg_Data(109,0,(SHORT)ChannelConfig[Chn].Filtime,0,2,0);     //��ʾ�˲�ʱ��
	DisSmloc(9);
	//LONG_HEX_BCD(ChannelConfig[Chn].Cutoff);
	Dis_nseg_Data(125,2,(SHORT)ChannelConfig[Chn].Cutoff,1,3,0);	   //��ʾС�ź��г�ֵ
	DisSmloc(10);
	DisData(109,4,ChannelConfig[Chn].Zeromove,ChannelConfig[Chn].Point,5,0); //��ʾ���Ǩ��ֵ
        Smloc_F=0;
	if((ChannelConfig[Chn].Type<6)||(ChannelConfig[Chn].Type>12))
	{
		Write_LCD_CChina(37,6,(UCHAR *)DUAN);   //�����ż����
		if(ChannelConfig[Chn].Type<6)
		Write_LCD_CChina(53,6,(UCHAR *)OU);
		if(ChannelConfig[Chn].Type>11)
		Write_LCD_CChina(53,6,(UCHAR *)ZU1);
		Write_LCD_CChina(69,6,(UCHAR *)CHU2);
		Write_LCD_CChina(85,6,(UCHAR *)LI);
		DisSmloc(11);
		if(ChannelConfig[Chn].CutDeal==0)	     //ChannelConfig[Chn].CutDeal ���ֵ��CutDeHisInt(i);	 �������ᵽ
		{
			Write_LCD_CChina(109,6,(UCHAR *)BAO);//����
			Write_LCD_CChina(125,6,(UCHAR *)CHI);
		}
		if(ChannelConfig[Chn].CutDeal==1)
		{
			Write_LCD_CChina(109,6,(UCHAR *)QI1);//���
			Write_LCD_CChina(125,6,(UCHAR *)DIAN);
		}
		if(ChannelConfig[Chn].CutDeal==2)
		{
			Write_LCD_CChina(109,6,(UCHAR *)ZHONG);//�յ�
			Write_LCD_CChina(125,6,(UCHAR *)DIAN);
		}
		Smloc_F=0;
	}
	OldDismode=Dismode;
}



//*************************************************
//-------------------------------------------------
//����:������̬��ʾ����
//���:��
//����:��
//-------------------------------------------------
#if SOFT==Chinese
void Baojing_zutai_display(void)
{
          if(Dismode==8)
          {
                if(Dismode!=OldDismode)
                {
                    LCD_Clear();
                    Write_LCD_CChina(0,0,(UCHAR *)BAO1);//����
                    Write_LCD_CChina(16,0,(UCHAR *)JING);
                    Dis_Shu_Line(33,0,63);
                    Dis_Shu_Line(34,0,63);
                    Write_LCD_CChina(37,0,(UCHAR *)TONG1);//ͨ��
                    Write_LCD_CChina(53,0,(UCHAR *)DAO);
                    Write_LCD_CChina(85,0,(UCHAR *)BAO1);//�����ز�
                    Write_LCD_CChina(101,0,(UCHAR *)JING);
                    Write_LCD_CChina(117,0,(UCHAR *)HUI);
                    Write_LCD_CChina(133,0,(UCHAR *)CHA);
                    Dis6_11_One(183,0,WpercentW);
                    Dis6_11_One(85,2,WHW);		       //HI
                    Dis6_11_One(92,2,WIW);
                    Dis6_11_One(163,2,WHW);			   //HH
                    Dis6_11_One(171,2,WHW);
                    Dis6_11_One(85,4,WLW);			   //LO
                    Dis6_11_One(92,4,WOW);
                    Dis6_11_One(163,4,WLW);			   //LL
                    Dis6_11_One(171,4,WLW);
                    Write_LCD_CChina(37,6,(UCHAR *)BIAN);//�仯��
                    Write_LCD_CChina(53,6,(UCHAR *)HUA);
                    Write_LCD_CChina(69,6,(UCHAR *)LV);
                    Dis6_11_One(90,6,WAW);			   //AV
                    Dis6_11_One(97,6,WVW);
                    Dis6_11_One(155,6,WxiegongW);
                    Dis6_11_One(163,6,WSW);
		}
		DisSmloc(0);
		Write_LCD_CChina(0,6,(UCHAR *)FAN);//����
		Write_LCD_CChina(16,6,(UCHAR *)HUI);
		DisSmloc(1);
	        Dis6_11_One(73,0,Chn+1);	 //��ʾͨ����
		DisSmloc(2);
		//LONG_HEX_BCD(ChannelConfig[Chn].Alarm.Almdead);
		Dis_nseg_Data(151,0,(SHORT)ChannelConfig[Chn].Alarm.Almdead,1,3,0);	 //�����ز������  0.0%
		//Dis_3seg(151,0);	
		DisSmloc(3);
		DisData(37,2,ChannelConfig[Chn].Alarm.HiAlm,ChannelConfig[Chn].Point,5,0);//����
		DisSmloc(4);
		Dis6_11_One(104,2,ChannelConfig[Chn].Alarm.HiAlmNo);	                  //�߱��������
		DisSmloc(5);
		DisData(115,2,ChannelConfig[Chn].Alarm.HiHiAlm,ChannelConfig[Chn].Point,5,0);//������
		DisSmloc(6);
		Dis6_11_One(184,2,ChannelConfig[Chn].Alarm.HiHiAlmNo);				     //�߸߱��������
		DisSmloc(7);
		DisData(37,4,ChannelConfig[Chn].Alarm.LoAlm,ChannelConfig[Chn].Point,5,0);//����
		DisSmloc(8);
		Dis6_11_One(104,4,ChannelConfig[Chn].Alarm.LoAlmNo);			          //�ͱ��������
		DisSmloc(9);
		DisData(115,4,ChannelConfig[Chn].Alarm.LoLoAlm,ChannelConfig[Chn].Point,5,0);//������	
		DisSmloc(10);
		Dis6_11_One(184,4,ChannelConfig[Chn].Alarm.LoLoAlmNo);					  //�͵ͱ��������
	        DisSmloc(11);
		DisData(108,6,ChannelConfig[Chn].Alarm.ChangeAlm,ChannelConfig[Chn].Point,5,0);
	        DisSmloc(12);
	        Dis6_11_One(184,6,ChannelConfig[Chn].Alarm.ChangeAlmNo);
	        Smloc_F=0;
		OldDismode=Dismode;
          }
          else if(Dismode==18)//HI
          {
                DisSmloc(0);
                if(LiChengTemp[0]==12)Dis6_11_One(37,2,WyigongW);			//���̷�Χ???
                else Dis6_11_One(37,2,WkonggeW);
                DisSmloc(1);
                Dis6_11_One(45,2,LiChengTemp[1]);
                DisSmloc(2);
                Dis6_11_One(53,2,LiChengTemp[2]);
                DisSmloc(3);
                Dis6_11_One(61,2,LiChengTemp[3]);
                DisSmloc(4);
                Dis6_11_One(69,2,LiChengTemp[4]);
                DisSmloc(5);
                Dis6_11_One(77,2,LiChengTemp[5]);
                Smloc_F=0;	
          }
          else if(Dismode==19)//HH									       	 //���̷�Χ???
          {
                DisSmloc(0);
                if(LiChengTemp[0]==12)Dis6_11_One(115,2,WyigongW);	         //��ʾ-  ���Ա�ʾ����
                else Dis6_11_One(115,2,WkonggeW);
                DisSmloc(1);
                Dis6_11_One(123,2,LiChengTemp[1]);
                DisSmloc(2);
                Dis6_11_One(131,2,LiChengTemp[2]);
                DisSmloc(3);
                Dis6_11_One(139,2,LiChengTemp[3]);
                DisSmloc(4);
                Dis6_11_One(147,2,LiChengTemp[4]);
                DisSmloc(5);
                Dis6_11_One(155,2,LiChengTemp[5]);
                Smloc_F=0;	

          }															       	//���̷�Χ???
          else if(Dismode==20)//LO
          {
                DisSmloc(0);
                if(LiChengTemp[0]==12)Dis6_11_One(37,4,WyigongW);
                else Dis6_11_One(37,4,WkonggeW);
                DisSmloc(1);
                Dis6_11_One(45,4,LiChengTemp[1]);
                DisSmloc(2);
                Dis6_11_One(53,4,LiChengTemp[2]);
                DisSmloc(3);
                Dis6_11_One(61,4,LiChengTemp[3]);
                DisSmloc(4);
                Dis6_11_One(69,4,LiChengTemp[4]);
                DisSmloc(5);
                Dis6_11_One(77,4,LiChengTemp[5]);
                Smloc_F=0;	

          }															       	//���̷�Χ???
          else if(Dismode==21)//LL
          {
                DisSmloc(0);
                if(LiChengTemp[0]==12)Dis6_11_One(115,4,WyigongW);
                else Dis6_11_One(115,4,WkonggeW);
                DisSmloc(1);
                Dis6_11_One(123,4,LiChengTemp[1]);
                DisSmloc(2);
                Dis6_11_One(131,4,LiChengTemp[2]);
                DisSmloc(3);
                Dis6_11_One(139,4,LiChengTemp[3]);
                DisSmloc(4);
                Dis6_11_One(147,4,LiChengTemp[4]);
                DisSmloc(5);
                Dis6_11_One(155,4,LiChengTemp[5]);
                Smloc_F=0;	
            
          }
          else if(Dismode==22)//AV
          {															      	//���̷�Χ???
                DisSmloc(0);
                if(LiChengTemp[0]==12)Dis6_11_One(108,6,WyigongW);
                else Dis6_11_One(108,6,WkonggeW);
                DisSmloc(1);
                Dis6_11_One(116,6,LiChengTemp[1]);
                DisSmloc(2);
                Dis6_11_One(124,6,LiChengTemp[2]);
                DisSmloc(3);
                Dis6_11_One(132,6,LiChengTemp[3]);
                DisSmloc(4);
                Dis6_11_One(140,6,LiChengTemp[4]);
                DisSmloc(5);
                Dis6_11_One(148,6,LiChengTemp[5]);
                Smloc_F=0;	
          }
}
#endif

//-------------------------------------------------
//����:ͨѶ��̬��ʾ����
//���:��
//����:��
//-------------------------------------------------
void Tongxun_zutai_display(void)
{
	if(Dismode!=OldDismode)
	{
		LCD_Clear();
		Write_LCD_CChina(0,0,(UCHAR *)TONG1);//ͨѶ
		Write_LCD_CChina(16,0,(UCHAR *)XUN);
		Dis_Shu_Line(33,0,63);
		Dis_Shu_Line(34,0,63);
		Write_LCD_CChina(37,0,(UCHAR *)BEN);//����
		Write_LCD_CChina(53,0,(UCHAR *)JI3);
		Write_LCD_CChina(69,0,(UCHAR *)DI);//��ַ
		Write_LCD_CChina(85,0,(UCHAR *)ZHI1);
		Write_LCD_CChina(53,2,(UCHAR *)BO);//������
		Write_LCD_CChina(69,2,(UCHAR *)TE);
		Write_LCD_CChina(85,2,(UCHAR *)LV);
		Write_LCD_CChina(37,4,(UCHAR *)JI4);//��ż
		Write_LCD_CChina(53,4,(UCHAR *)OU);
		Write_LCD_CChina(69,4,(UCHAR *)JIAO);//У��
		Write_LCD_CChina(85,4,(UCHAR *)YAN1);
		Write_LCD_CChina(53,6,(UCHAR *)TING);//ֹͣλ
		Write_LCD_CChina(69,6,(UCHAR *)ZHI);
		Write_LCD_CChina(85,6,(UCHAR *)WEI);
	}
	DisSmloc(0);
	Write_LCD_CChina(0,6,(UCHAR *)FAN);//����
	Write_LCD_CChina(16,6,(UCHAR *)HUI);
	DisSmloc(1);
	DisTwo(109,0,Syspar.SysAddr);
	DisSmloc(2);
	if(Syspar.BandRate==0)
	{
		Dis6_11_One(109,2,WkonggeW);
		Dis6_11_One(117,2,W1W);		     //1200
		Dis6_11_One(125,2,W2W);
	}
	if(Syspar.BandRate==1)
	{
		Dis6_11_One(109,2,WkonggeW);
	    Dis6_11_One(117,2,W4W);
	    Dis6_11_One(125,2,W8W);
	}
	if(Syspar.BandRate==2)			      //4800
	{
		Dis6_11_One(109,2,WkonggeW);
	    Dis6_11_One(117,2,W9W);
	    Dis6_11_One(125,2,W6W);
	}
	if(Syspar.BandRate==3)
	{								      //19200
		Dis6_11_One(109,2,W1W);
	    Dis6_11_One(117,2,W9W);
	    Dis6_11_One(125,2,W2W);
	}
	if(Syspar.BandRate==4)			     
	{								      // 38400
		Dis6_11_One(109,2,W3W);
	    Dis6_11_One(117,2,W8W);
	    Dis6_11_One(125,2,W4W);
	}
	if(Syspar.BandRate==5)
	{								       //57600
		Dis6_11_One(109,2,W5W);
	    Dis6_11_One(117,2,W7W);
	    Dis6_11_One(125,2,W6W);
	}
	DisTwo(133,2,W0W);
	DisSmloc(3);						    
	if(Syspar.Parity == 0)			         //��У��
	{
		Write_LCD_CChina(109,4,(UCHAR *)WU);	
	}									  
	else if(Syspar.Parity == 1)			     //��У��
	{
		Write_LCD_CChina(109,4,(UCHAR *)JI4);	
	}
	else 
	{
		Write_LCD_CChina(109,4,(UCHAR *)OU); //żУ��	
	}
	DisSmloc(4);
	if(Syspar.StopBits == 2)
	{
		Dis6_11_One(109,6,W2W);		        //2λֹͣλ
	}
	else
	{									    //1λֹͣλ
		Dis6_11_One(109,6,W1W);	
	}	
	Smloc_F=0;
	OldDismode=Dismode;
}
//-------------------------------------------------
//����:������̬��ʾ����
//���:��
//����:��
//-------------------------------------------------
void Wangluo_zutai_display(void)
{
	if(Dismode == WANGLUO_ZUTAI)
	{
		if(Dismode!=OldDismode)
		{
			LCD_Clear();
			Write_LCD_CChina(0,0,(UCHAR *)WANG);//����
			Write_LCD_CChina(16,0,(UCHAR *)LUO);
			Dis_Shu_Line(33,0,63);	            //��ʾ2����
    	    Dis_Shu_Line(34,0,63);
			Write_LCD_CChina(37,0,(UCHAR *)DI);//��ַ
			Write_LCD_CChina(53,0,(UCHAR *)ZHI1);
			Dis6_11_One(95,0,WdianW);
			Dis6_11_One(127,0,WdianW);
			Dis6_11_One(159,0,WdianW);
			Write_LCD_CChina(37,2,(UCHAR *)YAN);//����
			Write_LCD_CChina(53,2,(UCHAR *)MA);
			Dis6_11_One(95,2,WdianW);
			Dis6_11_One(127,2,WdianW);
			Dis6_11_One(159,2,WdianW);
			Write_LCD_CChina(37,4,(UCHAR *)WANG);//Ĭ������
			Write_LCD_CChina(53,4,(UCHAR *)GUAN);
			Dis6_11_One(95,4,WdianW);
			Dis6_11_One(127,4,WdianW);
			Dis6_11_One(159,4,WdianW);
			Write_LCD_CChina(37,6,(UCHAR *)DUAN1);//�˿�
			Write_LCD_CChina(53,6,(UCHAR *)KOU);
		}
		DisSmloc(0);
		Write_LCD_CChina(0,6,(UCHAR *)FAN);//����
		Write_LCD_CChina(16,6,(UCHAR *)HUI);
		DisSmloc(1);
		DisThree(71,0,Syspar.IPADDR.MYIP1);
		DisSmloc(2);
		DisThree(103,0,Syspar.IPADDR.MYIP2);
		DisSmloc(3);
		DisThree(135,0,Syspar.IPADDR.MYIP3);
		DisSmloc(4);
		DisThree(167,0,Syspar.IPADDR.MYIP4);
		DisSmloc(5);
		DisThree(71,2,Syspar.IPADDR.MYMSK1);
		DisSmloc(6);
		DisThree(103,2,Syspar.IPADDR.MYMSK2);
		DisSmloc(7);
		DisThree(135,2,Syspar.IPADDR.MYMSK3);
		DisSmloc(8);
		DisThree(167,2,Syspar.IPADDR.MYMSK4);
		DisSmloc(9);
		DisThree(71,4,Syspar.IPADDR.MYGW1);
		DisSmloc(10);
		DisThree(103,4,Syspar.IPADDR.MYGW2);
		DisSmloc(11);
		DisThree(135,4,Syspar.IPADDR.MYGW3);
		DisSmloc(12);
		DisThree(167,4,Syspar.IPADDR.MYGW4);
		DisSmloc(13);
		DisThree(71,6,Syspar.TCPPORT);
		Smloc_F=0;
	}
	else if(Dismode==70)//IP1
	{
		DisSmloc(0);
		Dis6_11_One(71,0,LiChengTemp[0]);
		DisSmloc(1);
		Dis6_11_One(79,2,LiChengTemp[1]);
		DisSmloc(2);
		Dis6_11_One(87,2,LiChengTemp[1]);
		Smloc_F=0;	
	}
	Smloc_F=0;
	OldDismode=Dismode;
}
void TingZhiJiLu_display(void)
{
	if(Dismode!=OldDismode)
	{
		LCD_Clear();
		Write_LCD_CChina(0,0,(UCHAR *)WANG);//����
		Write_LCD_CChina(16,0,(UCHAR *)LUO);
		Dis_Shu_Line(33,0,63);
	    Dis_Shu_Line(34,0,63);
//		LCD_WriteGraph(8,50,48,128,0,StopREC);
	}
	DisSmloc(0);
	Write_LCD_CChina(90,4,(UCHAR *)QUE);//ȷ��
	Write_LCD_CChina(106,4,(UCHAR *)DING);
	DisSmloc(1);
	Write_LCD_CChina(140,4,(UCHAR *)QU1);//ȡ��
	Write_LCD_CChina(156,4,(UCHAR *)XIAO1);

	Smloc_F=0;
	OldDismode=Dismode;
}
void TingZhiJiLu1_display(void)
{
	if(Dismode!=OldDismode)
	{
		LCD_Clear();
		Write_LCD_CChina(0,0,(UCHAR *)WANG);//����
		Write_LCD_CChina(16,0,(UCHAR *)LUO);
		Dis_Shu_Line(33,0,63);              //��ʾ2������
	    Dis_Shu_Line(34,0,63);
	//	LCD_WriteGraph(8,50,48,128,0,StopREC);
	}
	DisSmloc(0);
	Write_LCD_CChina(0,6,(UCHAR *)FAN);//����
	Write_LCD_CChina(16,6,(UCHAR *)HUI);

	Smloc_F=0;
	OldDismode=Dismode;
}

void USB_zutai_display1(void)
{
        if(Dismode!=OldDismode)
        {
            LCD_Clear();
            Write_LCD_CChina(0,0,(UCHAR *)BEI);	      //����
            Write_LCD_CChina(16,0,(UCHAR *)FEN);
            Dis_Shu_Line(33,0,63);			          //��ʾ2������
            Dis_Shu_Line(34,0,63);           
            Dis6_11_One(37,0,WUW);
            Dis6_11_One(45,0,WSW);
            Dis6_11_One(53,0,WBW);
            Write_LCD_CChina(61,0,(UCHAR *)ZHUANG);// ״̬
            Write_LCD_CChina(77,0,(UCHAR *)TAI);
            Dis6_11_One(93,0,WmaohaoW);			   // :	 
            
            Write_LCD_CChina(37,2,(UCHAR *)CAO);   // ������ʾ
            Write_LCD_CChina(53,2,(UCHAR *)ZUO);
            Write_LCD_CChina(69,2,(UCHAR *)TI);//
            Write_LCD_CChina(85,2,(UCHAR *)SHI5);//
            Dis6_11_One(101,2,WmaohaoW);
            
            Write_LCD_CChina(37,4,(UCHAR *)KE);//     ��������
            Write_LCD_CChina(53,4,(UCHAR *)YONG);
            Write_LCD_CChina(69,4,(UCHAR *)RONG);//
            Write_LCD_CChina(85,4,(UCHAR *)LIANG);
            Dis6_11_One(101,4,WmaohaoW);
            Dis6_11_One(175,4,WMW);			      //MB
            Dis6_11_One(183,4,WBW);
            
            Write_LCD_CChina(37,6,(UCHAR *)ZONG);//		������
            Write_LCD_CChina(53,6,(UCHAR *)RONG);//
            Write_LCD_CChina(69,6,(UCHAR *)LIANG);
            Dis6_11_One(85,6,WmaohaoW);
            Dis6_11_One(175,6,WMW);			    //MB
            Dis6_11_One(183,6,WBW);
        }
        if(UsbPara.USBStatuFlag==0)//δ����
        {
           Write_LCD_CChina(109,0,(UCHAR *)WEI1);
           Write_LCD_CChina(125,0,(UCHAR *)JIU);//
           Write_LCD_CChina(141,0,(UCHAR *)XU);//
        }
        else
        {
           Write_LCD_CChina(109,0,(UCHAR *)JIU);  //����
           Write_LCD_CChina(125,0,(UCHAR *)XU);   //
           Dis6_11_One(141,0,WkonggeW);
           Dis6_11_One(149,0,WkonggeW);
        }
        
        if(UsbPara.USBCaoZuoFlag==0)//
        {
           Write_LCD_CChina(109,2,(UCHAR *)BU1);  //���ɲ���
           Write_LCD_CChina(125,2,(UCHAR *)KE);
           Write_LCD_CChina(141,2,(UCHAR *)CAO);
           Write_LCD_CChina(157,2,(UCHAR *)ZUO);
        }
        else
        {
           Write_LCD_CChina(109,2,(UCHAR *)KE);	//���Բ���
           Write_LCD_CChina(125,2,(UCHAR *)YI2);// 
           Write_LCD_CChina(141,2,(UCHAR *)CAO);
           Write_LCD_CChina(156,2,(UCHAR *)ZUO);
        }
        
        if(UsbPara.USBStatuFlag==0)//δ����
        {
           Dis6_11_One(109,4,WyigongW); 
           Dis6_11_One(117,4,WyigongW); 
           Dis6_11_One(125,4,WyigongW); 
           Dis6_11_One(133,4,WyigongW); 
           Dis6_11_One(141,4,WyigongW); 
           Dis6_11_One(149,4,WyigongW); 
           Dis6_11_One(157,4,WyigongW); 
              
           Dis6_11_One(109,6,WyigongW); 
           Dis6_11_One(117,6,WyigongW); 
           Dis6_11_One(125,6,WyigongW); 
           Dis6_11_One(133,6,WyigongW); 
           Dis6_11_One(141,6,WyigongW); 
           Dis6_11_One(149,6,WyigongW); 
           Dis6_11_One(157,6,WyigongW);
        }
        else										 //����״̬
        {
           
           DisData(109,4,UsbPara.USBKeYongRongLiang,0,5,0); 
           DisData(109,6,UsbPara.USBZongRongLiang,0,5,0); 
           Dis6_11_One(161,4,WkonggeW);
           Dis6_11_One(161,6,WkonggeW);
        }
        DisSmloc(0);
        Write_LCD_CChina(0,6,(UCHAR *)FAN);         //����
        Write_LCD_CChina(16,6,(UCHAR *)HUI);
        OldDismode=Dismode;
        Smloc_F=0;
        
}
void USB_zutai_display2(void)
{
	if (Dismode != OldDismode)
    {
        LCD_Clear();
        Write_LCD_CChina(0, 0, (UCHAR*)BEI);       //����
        Write_LCD_CChina(16, 0, (UCHAR*)FEN);
        Dis_Shu_Line(33, 0, 63);	        //��ʾ2������
        Dis_Shu_Line(34, 0, 63);
        Write_LCD_CChina(37, 0, (UCHAR*)QI1);	   //��ʼ����
        Write_LCD_CChina(53, 0, (UCHAR*)SHI6);
        Write_LCD_CChina(69, 0, (UCHAR*)RI);
        Write_LCD_CChina(85, 0, (UCHAR*)QI);
        Dis6_11_One(101, 0, WmaohaoW);
        Dis6_11_One(125, 0, WyigongW);
        Dis6_11_One(149, 0, WyigongW);
        Write_LCD_CChina(37, 2, (UCHAR*)JIE);	   //��������
        Write_LCD_CChina(53, 2, (UCHAR*)SU);
        Write_LCD_CChina(69, 2, (UCHAR*)RI);
        Write_LCD_CChina(85, 2, (UCHAR*)QI);
        Dis6_11_One(101, 2, WmaohaoW);
        Dis6_11_One(125, 2, WyigongW);
        Dis6_11_One(149, 2, WyigongW);
        Write_LCD_CChina(37, 4, (UCHAR*)TONG1);	    //ͨ��ѡ�� 
        Write_LCD_CChina(53, 4, (UCHAR*)DAO);
        Write_LCD_CChina(69, 4, (UCHAR*)XUAN);
        Write_LCD_CChina(85, 4, (UCHAR*)ZE);
    }
    DisSmloc(1);
    DisTwo(109, 0, UsbPara.StartTime.Year);	        //��ʼ��
    DisSmloc(2);
    DisTwo(133, 0, UsbPara.StartTime.Month);	    //��ʼ��
    DisSmloc(3);
    DisTwo(157, 0, UsbPara.StartTime.Day);		    //��ʼ��
    DisSmloc(4);
    DisTwo(109, 2, UsbPara.StopTime.Year);		    //������
    DisSmloc(5);
    DisTwo(133, 2, UsbPara.StopTime.Month);			//������
    DisSmloc(6);
    DisTwo(157, 2, UsbPara.StopTime.Day);			//������
    DisSmloc(7);
    if (UsbPara.CH1SaveEn)	                        //��ʼͨ����          
        Write_LCD_CChina(109, 4, (UCHAR*)XUANZHONG);    
    else
        Write_LCD_CChina(109, 4, (UCHAR*)DAFANGGE);
    DisSmloc(8);
    if (UsbPara.CH2SaveEn)						    //��ʼͨ����   
        Write_LCD_CChina(129, 4, (UCHAR*)XUANZHONG);
    else
        Write_LCD_CChina(129, 4, (UCHAR*)DAFANGGE);
    DisSmloc(9);
    if (UsbPara.CH3SaveEn)						    //��ʼͨ����   
        Write_LCD_CChina(149, 4, (UCHAR*)XUANZHONG);
    else
        Write_LCD_CChina(149, 4, (UCHAR*)DAFANGGE);
    DisSmloc(10);
    if (UsbPara.CH4SaveEn)						    //��ʼͨ����   
        Write_LCD_CChina(169, 4, (UCHAR*)XUANZHONG);
    else
        Write_LCD_CChina(169, 4, (UCHAR*)DAFANGGE);
    DisSmloc(11);
	if(CopyFileRuning ==1)
	{
        Write_LCD_CChina(0,6,(UCHAR *)FAN);//����
        Write_LCD_CChina(16,6,(UCHAR *)HUI);
	}
	else
	{
		Write_LCD_CChina(37,6,(UCHAR *)QUE);    //ȷ������
		Write_LCD_CChina(53,6,(UCHAR *)DING);
		Write_LCD_CChina(69,6,(UCHAR *)BAO);
		Write_LCD_CChina(85,6,(UCHAR *)CUN);
	}
    OldDismode = Dismode;
    Smloc_F = 0;
	if (UsbPara.UsbStatu == 0)
   	{
       	Dis6_11_One(117, 6, WkonggeW);
        Dis6_11_One(125, 6, WkonggeW);
   	    Dis6_11_One(133, 6, WkonggeW);
       	Dis6_11_One(141, 6, WkonggeW);
        Dis6_11_One(149, 6, WOW);		    //��ʾOK
   	    Dis6_11_One(157, 6, WKW);
       	Dis6_11_One(165, 6, WkonggeW);
        Dis6_11_One(173, 6, WkonggeW);
   	}
//	if((CopyFileRuning == 1)&&(UsbPara.UsbStatu == 1))
//	{
//		CopyBar(CopyedFileSize*19/TotalFileSize);	    //û�����
//	}
}
//**************************
//--------------------------
//���ܣ���ʾ����
//��ڣ�������x,������y,�׵�ַp
//--------------------------
void Dis5_7_date(UCHAR x,UCHAR y,UCHAR *p)
{
        if((Dismode==104)&&(Run_Set_F==1))
          DisSmloc(0);
        DisTwo1(x,y,*p);
        Smloc_F=0;
        Write_Lcd_Bytes(x+12,y,(UCHAR *)NUM5_7_yigong,5);
        if((Dismode==104)&&(Run_Set_F==1))
          DisSmloc(1);
        DisTwo1(x+17,y,*(p+1));
        Smloc_F=0;
        Write_Lcd_Bytes(x+29,y,(UCHAR *)NUM5_7_yigong,5);
        if((Dismode==104)&&(Run_Set_F==1))
          DisSmloc(2);
        DisTwo1(x+34,y,*(p+2));
        Smloc_F=0;
}

//**************************
//--------------------------
//���ܣ���ʾʱ��
//��ڣ�������x,������y,�׵�ַp
//--------------------------        
void Dis5_7_time(UCHAR x,UCHAR y,UCHAR *p)       
{        
        if((Dismode==104)&&(Run_Set_F==1))
          DisSmloc(3);
        DisTwo1(x,y,*p);
        Smloc_F=0;
        Write_Lcd_Bytes(x+12,y,(UCHAR *)NUM5_7_maohao,5);
        if((Dismode==104)&&(Run_Set_F==1))
          DisSmloc(4);
        DisTwo1(x+17,y,*(p+1));
        Smloc_F=0;
        Write_Lcd_Bytes(x+29,y,(UCHAR *)NUM5_7_maohao,5);
        if((Dismode==104)&&(Run_Set_F==1))
          DisSmloc(5);
        DisTwo1(x+34,y,*(p+2));
        Smloc_F=0;
}


//**************************
//--------------------------
//���ܣ���ʾ5*7�����ں�ʱ��
//��ڣ�������x,������y,ʱ���׵�ַ
//���ڣ���
//--------------------------
void Dis5_7Time(UCHAR x,UCHAR y,UCHAR *p)
{
      Dis5_7_date(x,y,p);
      Dis5_7_time(x,y+1,p+3);
}


//***************************
//---------------------------
//����: ������ʾ����
//���: ������x,������y,�׵�ַ
//��ʾ����: ��-ʱ
//����: lulj   2018-02-27
//---------------------------
void Dis5_7_dataNew(UCHAR x,UCHAR y,UCHAR *p)
{
	 Smloc_F=0;    //lulj 20180226
	 DisTwo1(x,y,*p);
	 Write_Lcd_Bytes(x+12,y,(UCHAR *)NUM5_7_yigong,5);      
	 DisTwo1(x+17,y,*(p+1));
	 Write_Lcd_Bytes(x+29,y,(UCHAR *)NUM5_7_yigong,5);
}

//***************************
//---------------------------
//����:������ʾʱ��
//���:������x,������y,�׵�ַ
//��ʾ����: ʱ:��:��
//����:lulj 2018-02-27
//---------------------------
void Dis5_7_timeNew(UCHAR x,UCHAR y,UCHAR *p)
{
	Smloc_F =0;
	DisTwo1(x,y,*p);                    //��ʾ ʱ
	Write_Lcd_Bytes(x+12,y,(UCHAR *)NUM5_7_maohao,5);
	DisTwo1(x+17,y,*(p+1));             //��ʾ ��
	Write_Lcd_Bytes(x+29,y,(UCHAR *)NUM5_7_maohao,5);
	DisTwo1(x+34,y,*(p+2));             //��ʾ ��
}



//**************************
//--------------------------
//���ܣ���ʾ5*7�����ں�ʱ��
//��ڣ�������x,������y,ʱ���׵�ַ
//���ڣ���
//����: lulj  2018-2-27
//--------------------------
void Dis5_7DatandtimeNew(UCHAR x,UCHAR y,UCHAR *p)
{
	 Dis5_7_dataNew(x,y,p);                   //��ʾ����  �� ��
	 Dis5_7_timeNew(x+48,y,p+2);              //��ʾʱ:��:��
	
}





//*************************************************
//-------------------------------------------------
//���ܣ�������ͼ
//��ڣ���ǰֵ�����ޣ����޾�Ϊ��0~190�������������ڻ�������,�������
//-------------------------------------------------
void BangtuH(UCHAR val,UCHAR sv,UCHAR hh,UCHAR h,UCHAR l,UCHAR ll,UCHAR x,UCHAR y,UCHAR k)
{
      UCHAR i;
      for(i=0;i<190;i++)
      {
          TempTemp[0]=0;
          if((i==0)||(i==189))
            TempTemp[0]=0xFF;
          else
            TempTemp[0]=0x81;
          if((i==ll)||(i==l))
            TempTemp[0] |=0xFf;
          if((i==ll+1)||(i==l+1))
            TempTemp[0] |=0xBf;
          if((i==ll+2)||(i==l+2))
            TempTemp[0] |=0x8f;
          if((i==ll+3)||(i==l+3))
            TempTemp[0] |=0x83;
          if((i==hh-3)||(i==h-3))
            TempTemp[0] |=0x83;  
          if((i==hh-2)||(i==h-2))
            TempTemp[0] |=0x8f;
          if((i==hh-1)||(i==h-1))
            TempTemp[0] |=0xBf;
          if((i==hh)||(i==h))
            TempTemp[0] |=0xFf;
          if(k==1)
          {
              if((i==sv-2)||(i==sv+2))
                TempTemp[0] |=0x8D;
              if((i==sv-1)||(i==sv+1))
                TempTemp[0] |=0xBD;
              if(i==sv)
                TempTemp[0] |=0xFD;
          }   
          if(i%19==18)
            TempTemp[0] |= 0xC0;
          if(i%38==37)
            TempTemp[0] |= 0xE0;
          if((i>0)&&(i<=val)&&(i!=189))
            TempTemp[0] ^= 0x7E;
          Write_Lcd_Bytes(x+i,y,(UCHAR *)&TempTemp[0],1);
          
//          MainDoUART();
      }
}

//*************************************************
//-------------------------------------------------
//���ܣ����߰�ͼ
//��ڣ�MV������
//-------------------------------------------------
void BangtuL(UCHAR mv,UCHAR x,UCHAR y)
{
    UCHAR i;
    for(i=0;i<190;i++)
    {
        TempTemp[0]=0x04;
        if((i==mv-2)||(i==mv+2))
          TempTemp[0] |=0x20;
        if((i==mv-1)||(i==mv+1))
          TempTemp[0] |=0x30;
        if(i==mv)
          TempTemp[0] |=0x38;
        if(i%19==18)
            TempTemp[0] |= 0x08;
        if((i%38==37)||(i==0))
          TempTemp[0] |= 0x18;
        Write_Lcd_Bytes(x+i,y,(UCHAR *)&TempTemp[0],1);
    }
}     
//*********************************
//���ܣ����԰�ͼ�����
//--------------------------------
UCHAR CAL_Bangtu(signed long x)
{
    UCHAR y;
    y=(UCHAR)(((fp32)x-(fp32)ChannelConfig[Channo].RangeL)/(ChannelConfig[Channo].RangeH-ChannelConfig[Channo].RangeL)*189);
    return y;
}

//*************************************************
//-------------------------------------------------
//���ܣ���ͨ�����Ի���
//��ڣ�
//-------------------------------------------------
void Shuxian4_Run_display(void)
{
    unsigned char i;
    if (Dismode != OldDismode)
    {
        LCD_Clear();
        if (Syspar.Pvnum == 4)
        {
            Smloc_F = 1;
            Dis_CH(0, 0);
            Dis6_11_One(16, 0, 1);
            Dis6_11_One(24, 0, WmaohaoW);

            Dis_CH(0, 2);
            Dis6_11_One(16, 2, 2);
            Dis6_11_One(24, 2, WmaohaoW);

            Dis_CH(0, 4);
            Dis6_11_One(16, 4, 3);
            Dis6_11_One(24, 4, WmaohaoW);
 
            Dis_CH(0, 6);
            Dis6_11_One(16, 6, 4);
            Dis6_11_One(24, 6, WmaohaoW);
            Smloc_F = 0;
            DisUnit(81, 0, ChannelConfig[0].Unit);
            DisUnit(81, 2, ChannelConfig[1].Unit);
            DisUnit(81, 4, ChannelConfig[2].Unit);
            DisUnit(81, 6, ChannelConfig[3].Unit);
            Write_Lcd_Bytes(160, 4, (UCHAR*)CHAR_K, 6);
            Write_Lcd_Bytes(166, 4, (UCHAR*)NUM5_7_1, 6);
            Write_Lcd_Bytes(160, 5, (UCHAR*)CHAR_K, 6);
            Write_Lcd_Bytes(166, 5, (UCHAR*)NUM5_7_2, 6);
            Write_Lcd_Bytes(160, 6, (UCHAR*)CHAR_K, 6);
            Write_Lcd_Bytes(166, 6, (UCHAR*)NUM5_7_3, 6);
            Write_Lcd_Bytes(160, 7, (UCHAR*)CHAR_K, 6);
            Write_Lcd_Bytes(166, 7, (UCHAR*)NUM5_7_4, 6);
        }
        else if (Syspar.Pvnum == 3)
        {
            Smloc_F = 1;
            Dis_CH(0, 0);
            Dis6_11_One(16, 0, 1);
            Dis6_11_One(24, 0, WmaohaoW);

            Dis_CH(0, 3);
            Dis6_11_One(16, 3, 2);
            Dis6_11_One(24, 3, WmaohaoW);

            Dis_CH(0, 6);
            Dis6_11_One(16, 6, 3);
            Dis6_11_One(24, 6, WmaohaoW);
            Smloc_F = 0;
            DisUnit(81, 0, ChannelConfig[0].Unit);
            DisUnit(81, 3, ChannelConfig[1].Unit);
            DisUnit(81, 6, ChannelConfig[2].Unit);
            Write_Lcd_Bytes(160, 4, (UCHAR*)CHAR_K, 6);
            Write_Lcd_Bytes(166, 4, (UCHAR*)NUM5_7_1, 6);
            Write_Lcd_Bytes(160, 5, (UCHAR*)CHAR_K, 6);
            Write_Lcd_Bytes(166, 5, (UCHAR*)NUM5_7_2, 6);
            Write_Lcd_Bytes(160, 6, (UCHAR*)CHAR_K, 6);
            Write_Lcd_Bytes(166, 6, (UCHAR*)NUM5_7_3, 6);
            Write_Lcd_Bytes(160, 7, (UCHAR*)CHAR_K, 6);
            Write_Lcd_Bytes(166, 7, (UCHAR*)NUM5_7_4, 6);
        }
        else if (Syspar.Pvnum == 2)
        {
            Dis_CH(0, 0);
            Dis6_11_One(16, 0, 1);
            DisUnit(0, 2, ChannelConfig[0].Unit);
            Dis_CH(0, 4);
            Dis6_11_One(16, 4, 2);
            DisUnit(0, 6, ChannelConfig[1].Unit);
        }
    }
    if (Syspar.Pvnum == 2)		//2ͨ��������ʾ
    {
       	//ͨ��1
		if ((ChannelConfig[0].Type < 6) || (ChannelConfig[0].Type > 11))
        {
            if (ChannelRunValue[0].CutDealF == 1)
            {
                if (ChannelCutDealFlag[0] == 1)
                {
                    DisData(48, 0, ChannelRunValue[0].Value, ChannelConfig[0].Point, 5, 1);
                }
                else if (ChannelCutDealFlag[0] == 2)
                {
                    CutDealDis(48, 0, 1, 144);
                }
            }
            else
            {
                DisData(48, 0, ChannelRunValue[0].Value, ChannelConfig[0].Point, 5, 1);
            }
        }
        else
        {
            DisData(48, 0, ChannelRunValue[0].Value, ChannelConfig[0].Point, 5, 1);
        }
		//ͨ��2 
        if ((ChannelConfig[1].Type < 6) || (ChannelConfig[1].Type > 11))
        {
            if (ChannelRunValue[1].CutDealF == 1)
            {
                if (ChannelCutDealFlag[1] == 1)
                {
                    DisData(48, 4, ChannelRunValue[1].Value, ChannelConfig[1].Point, 5, 1);
                }
                else if (ChannelCutDealFlag[1] == 2)
                {
                    CutDealDis(48, 4, 1, 144);
                }
            }
            else
            {
                DisData(48, 4, ChannelRunValue[1].Value, ChannelConfig[1].Point, 5, 1);
            }
        }
        else
        {
            DisData(48, 4, ChannelRunValue[1].Value, ChannelConfig[1].Point, 5, 1);
        }
    }
    else if (Syspar.Pvnum == 3)		//����ͨ����ʾ����
    {
        Dis5_7Time(146, 0, (UCHAR*) &Systime.Year);
        if (Syspar1.DO_REL &BIT7)
        {
            Rectangle1(180, 4);
        }
        else
        {
            Rectangle(180, 4);
        }
        if (Syspar1.DO_REL &BIT6)
        {
            Rectangle1(180, 5);
        }
        else
        {
            Rectangle(180, 5);
        }
        if (Syspar1.DO_REL &BIT5)
        {
            Rectangle1(180, 6);
        }
        else
        {
            Rectangle(180, 6);
        }
        if (Syspar1.DO_REL &BIT4)
        {
            Rectangle1(180, 7);
        }
        else
        {
            Rectangle(180, 7);
        }
        for (i = 0; i < 3; i++)
        {
            if (ChannelRunValue[i].LLALM == 1)
            {
                Write_Lcd_Bytes(130, i *3, (UCHAR*)CHAR_L, 6);
                Write_Lcd_Bytes(136, i *3, (UCHAR*)CHAR_L, 6);
            }
            else if (ChannelRunValue[i].HHALM == 1)
            {
                Write_Lcd_Bytes(130, i *3, (UCHAR*)CHAR_H, 6);
                Write_Lcd_Bytes(136, i *3, (UCHAR*)CHAR_H, 6);
            }
            else
            {
                Write_Lcd_Bytes(130, i *3, (UCHAR*)NUM5_7_kongge, 6);
                Write_Lcd_Bytes(136, i *3, (UCHAR*)NUM5_7_kongge, 6);
            }

            if (ChannelRunValue[i].LALM == 1)
            {
                Write_Lcd_Bytes(130, i *3+1, (UCHAR*)CHAR_L, 6);
                Write_Lcd_Bytes(136, i *3+1, (UCHAR*)CHAR_O, 6);
            }
            else if (ChannelRunValue[i].HALM == 1)
            {
                Write_Lcd_Bytes(130, i *3+1, (UCHAR*)CHAR_H, 6);
                Write_Lcd_Bytes(136, i *3+1, (UCHAR*)CHAR_I, 6);
            }
            else
            {
                Write_Lcd_Bytes(130, i *3+1, (UCHAR*)NUM5_7_kongge, 6);
                Write_Lcd_Bytes(136, i *3+1, (UCHAR*)NUM5_7_kongge, 6);
            }
        }
        if ((ChannelConfig[0].Type < 6) || (ChannelConfig[0].Type > 11))
        {
            if (ChannelRunValue[0].CutDealF == 1)
            {
                if (ChannelCutDealFlag[0] == 1)
                {
                    DisData(32, 0, ChannelRunValue[0].Value, ChannelConfig[0].Point, 5, 0);
                }
                else if (ChannelCutDealFlag[0] == 2)
                {
                    CutDealDis(32, 0, 0, 48);
                }
            }
            else
            {
                DisData(32, 0, ChannelRunValue[0].Value, ChannelConfig[0].Point, 5, 0);
            }
        }
        else
        {
            DisData(32, 0, ChannelRunValue[0].Value, ChannelConfig[0].Point, 5, 0);
        }
        if ((ChannelConfig[1].Type < 6) || (ChannelConfig[1].Type > 11))
        {
            if (ChannelRunValue[1].CutDealF == 1)
            {
                if (ChannelCutDealFlag[1] == 1)
                {
                    DisData(32, 3, ChannelRunValue[1].Value, ChannelConfig[1].Point, 5, 0);
                }
                else if (ChannelCutDealFlag[1] == 2)
                {
                    CutDealDis(32, 3, 0, 48);
                }
            }
            else
            {
                DisData(32, 3, ChannelRunValue[1].Value, ChannelConfig[1].Point, 5, 0);
            }
        }
        else
        {
            DisData(32, 3, ChannelRunValue[1].Value, ChannelConfig[1].Point, 5, 0);
        }
        if ((ChannelConfig[2].Type < 6) || (ChannelConfig[2].Type > 11))
        {
            if (ChannelRunValue[2].CutDealF == 1)
            {
                if (ChannelCutDealFlag[2] == 1)
                {
                    DisData(32, 6, ChannelRunValue[2].Value, ChannelConfig[2].Point, 5, 0);
                }
                else if (ChannelCutDealFlag[2] == 2)
                {
                    CutDealDis(32, 6, 0, 48);
                }
            }
            else
            {
                DisData(32, 6, ChannelRunValue[2].Value, ChannelConfig[2].Point, 5, 0);
            }
        }
        else
        {
            DisData(32, 6, ChannelRunValue[2].Value, ChannelConfig[2].Point, 5, 0);
        }

        //DisData(32,0,ChannelRunValue[0].Value,ChannelConfig[0].Point,4,0);
        //DisData(32,3,ChannelRunValue[1].Value,ChannelConfig[1].Point,4,0);
        //DisData(32,6,ChannelRunValue[2].Value,ChannelConfig[2].Point,4,0);
    }
    else if (Syspar.Pvnum == 4)		//�ĸ�ͨ����ʾ����
    {
        Dis5_7Time(146, 0, (UCHAR*) &Systime.Year);	 //��ʾϵͳʱ��
        if (Syspar1.DO_REL &BIT7)
        {
            Rectangle1(180, 4);
        }
        else
        {
            Rectangle(180, 4);
        }
        if (Syspar1.DO_REL &BIT6)
        {
            Rectangle1(180, 5);
        }
        else
        {
            Rectangle(180, 5);
        }
        if (Syspar1.DO_REL &BIT5)
        {
            Rectangle1(180, 6);
        }
        else
        {
            Rectangle(180, 6);
        }
        if (Syspar1.DO_REL &BIT4)
        {
            Rectangle1(180, 7);
        }
        else
        {
            Rectangle(180, 7);
        }
        for (i = 0; i < 4; i++)						//������ʾ
        {
            if (ChannelRunValue[i].LLALM == 1)
            {
                Write_Lcd_Bytes(130, i *2, (UCHAR*)CHAR_L, 6);
                Write_Lcd_Bytes(136, i *2, (UCHAR*)CHAR_L, 6);
            }
            else if (ChannelRunValue[i].HHALM == 1)
            {
                Write_Lcd_Bytes(130, i *2, (UCHAR*)CHAR_H, 6);
                Write_Lcd_Bytes(136, i *2, (UCHAR*)CHAR_H, 6);
            }
            else
            {
                Write_Lcd_Bytes(130, i *2, (UCHAR*)NUM5_7_kongge, 6);
                Write_Lcd_Bytes(136, i *2, (UCHAR*)NUM5_7_kongge, 6);
            }

            if (ChannelRunValue[i].LALM == 1)
            {
                Write_Lcd_Bytes(130, i *2+1, (UCHAR*)CHAR_L, 6);
                Write_Lcd_Bytes(136, i *2+1, (UCHAR*)CHAR_O, 6);
            }
            else if (ChannelRunValue[i].HALM == 1)
            {
                Write_Lcd_Bytes(130, i *2+1, (UCHAR*)CHAR_H, 6);
                Write_Lcd_Bytes(136, i *2+1, (UCHAR*)CHAR_I, 6);
            }
            else
            {
                Write_Lcd_Bytes(130, i *2+1, (UCHAR*)NUM5_7_kongge, 6);
                Write_Lcd_Bytes(136, i *2+1, (UCHAR*)NUM5_7_kongge, 6);
            }
        }
        if ((ChannelConfig[0].Type < 6) || (ChannelConfig[0].Type > 11))
        {
            if (ChannelRunValue[0].CutDealF == 1)
            {							
                if (ChannelCutDealFlag[0] == 1)
                {
                    DisData(32, 0, ChannelRunValue[0].Value, ChannelConfig[0].Point, 5, 0);
                }
                else if (ChannelCutDealFlag[0] == 2)
                {
                    CutDealDis(32, 0, 0, 48);
                }
            }
            else
            {
                DisData(32, 0, ChannelRunValue[0].Value, ChannelConfig[0].Point, 5, 0);
            }
        }
        else
        {
            DisData(32, 0, ChannelRunValue[0].Value, ChannelConfig[0].Point, 5, 0);
        }
        if ((ChannelConfig[1].Type < 6) || (ChannelConfig[1].Type > 11))
        {
            if (ChannelRunValue[1].CutDealF == 1)
            {
                if (ChannelCutDealFlag[1] == 1)
                {
                    DisData(32, 2, ChannelRunValue[1].Value, ChannelConfig[1].Point, 5, 0);
                }
                else if (ChannelCutDealFlag[1] == 2)
                {
                    CutDealDis(32, 2, 0, 48);
                }
            }
            else
            {
                DisData(32, 2, ChannelRunValue[1].Value, ChannelConfig[1].Point, 5, 0);
            }
        }
        else
        {
            DisData(32, 2, ChannelRunValue[1].Value, ChannelConfig[1].Point, 5, 0);
        }
        if ((ChannelConfig[2].Type < 6) || (ChannelConfig[2].Type > 11))
        {
            if (ChannelRunValue[2].CutDealF == 1)
            {
                if (ChannelCutDealFlag[2] == 1)
                {
                    DisData(32, 4, ChannelRunValue[2].Value, ChannelConfig[2].Point, 5, 0);
                }
                else if (ChannelCutDealFlag[2] == 2)
                {
                    CutDealDis(32, 4, 0, 48);
                }
            }
            else
            {
                DisData(32, 4, ChannelRunValue[2].Value, ChannelConfig[2].Point, 5, 0);
            }
        }
        else
        {
            DisData(32, 4, ChannelRunValue[2].Value, ChannelConfig[2].Point, 5, 0);
        }
        if ((ChannelConfig[3].Type < 6) || (ChannelConfig[3].Type > 11))
        {
            if (ChannelRunValue[3].CutDealF == 1)
            {
                if (ChannelCutDealFlag[3] == 1)
                {
                    DisData(32, 6, ChannelRunValue[3].Value, ChannelConfig[3].Point, 5, 0);
                }
                else if (ChannelCutDealFlag[3] == 2)
                {
                    CutDealDis(32, 6, 0, 48);
                }
            }
            else
            {
                DisData(32, 6, ChannelRunValue[3].Value, ChannelConfig[3].Point, 5, 0);
            }
        }
        else
        {
            DisData(32, 6, ChannelRunValue[3].Value, ChannelConfig[2].Point, 5, 0);
        }
    }
    OldDismode = Dismode;
}
//*************************************************
//-------------------------------------------------
//���ܣ���ͨ�����Ի���
//��ڣ�
//���ڣ�
//-------------------------------------------------
void Shuxian_Run_display(void)
{     
      UCHAR v,hh,h,l,ll;
      if(Dismode!=OldDismode)
      {
          LCD_Clear(); 
          Write_Lcd_Bytes(0,4,(UCHAR *)CHAR_K,6); 		//K1
          Write_Lcd_Bytes(6,4,(UCHAR *)NUM5_7_1,6); 
          Write_Lcd_Bytes(0,5,(UCHAR *)CHAR_K,6); 	    //K2
          Write_Lcd_Bytes(6,5,(UCHAR *)NUM5_7_2,6);        
          Write_Lcd_Bytes(0,6,(UCHAR *)CHAR_K,6); 		//K3
          Write_Lcd_Bytes(6,6,(UCHAR *)NUM5_7_3,6); 
          Write_Lcd_Bytes(0,7,(UCHAR *)CHAR_K,6); 		//K4
          Write_Lcd_Bytes(6,7,(UCHAR *)NUM5_7_4,6);        
      }
      Smloc_F=0;
      Dis_CH(0,0);	    //��ʾCH
      Dis6_11_One(16,0,Channo+1); //��ʾͨ����
      DisUnit(92,0,ChannelConfig[Channo].Unit);
      Dis5_7Time(146,0,(UCHAR *)&Systime.Year);
      
      if((ChannelConfig[Channo].Type<6)||(ChannelConfig[Channo].Type>11))
      {
        if(ChannelRunValue[Channo].CutDealF==1)
        {
          if(ChannelCutDealFlag[Channo]==1)
          {
            DisData(48,4,ChannelRunValue[Channo].Value,ChannelConfig[Channo].Point,5,1);            
          }
          else if(ChannelCutDealFlag[Channo]==2)
          {
             CutDealDis(48,4,1,144);             
          }
        }
        else
        {
           DisData(48,4,ChannelRunValue[Channo].Value,ChannelConfig[Channo].Point,5,1);          
        }
      }
      else
      {
        DisData(48,4,ChannelRunValue[Channo].Value,ChannelConfig[Channo].Point,5,1);        
      }
               
      //DisData(64,4,ChannelRunValue[Channo].Value,ChannelConfig[Channo].Point,4,1);
      v=CAL_Bangtu(ChannelRunValue[Channo].Value);      
      hh=CAL_Bangtu(ChannelConfig[Channo].Alarm.HiHiAlm);
      h=CAL_Bangtu(ChannelConfig[Channo].Alarm.HiAlm);
      l=CAL_Bangtu(ChannelConfig[Channo].Alarm.LoAlm);
      ll=CAL_Bangtu(ChannelConfig[Channo].Alarm.LoLoAlm);    
      BangtuH(v,0,hh,h,l,ll,1,2,0);									   //����������ʾ��ͼ
             
      if(AM==0)
      {
          Write_Lcd_Bytes(0,3,(UCHAR *)CHAR_A,6); 
      }
      else
      {
          Write_Lcd_Bytes(0,3,(UCHAR *)CHAR_M,6);
      }
      if(Syspar1.DO_REL&BIT7)
      	Rectangle1(16,4);
      else
      	Rectangle(16,4);
      if(Syspar1.DO_REL&BIT6)
      	Rectangle1(16,5);
      else
      	Rectangle(16,5);
      if(Syspar1.DO_REL&BIT5)
      	Rectangle1(16,6);
      else
      	Rectangle(16,6);
      if(Syspar1.DO_REL&BIT4)
      	Rectangle1(16,7);
      else
      	Rectangle(16,7);
      if(ChannelRunValue[Channo].LLALM==1)
      {
        Write_Lcd_Bytes(30,0,(UCHAR *)CHAR_L,6); 
        Write_Lcd_Bytes(36,0,(UCHAR *)CHAR_L,6);
      }
      else if(ChannelRunValue[Channo].HHALM==1)
      {
        Write_Lcd_Bytes(30,0,(UCHAR *)CHAR_H,6); 
        Write_Lcd_Bytes(36,0,(UCHAR *)CHAR_H,6);
      }
      else 
      {
        Write_Lcd_Bytes(30,0,(UCHAR *)NUM5_7_kongge,6); 
        Write_Lcd_Bytes(36,0,(UCHAR *)NUM5_7_kongge,6); 
      }
      
      if(ChannelRunValue[Channo].LALM==1)
      {
        Write_Lcd_Bytes(30,1,(UCHAR *)CHAR_L,6); 
        Write_Lcd_Bytes(36,1,(UCHAR *)CHAR_O,6);
      }
      else if(ChannelRunValue[Channo].HALM==1)
      {
        Write_Lcd_Bytes(30,1,(UCHAR *)CHAR_H,6); 
        Write_Lcd_Bytes(36,1,(UCHAR *)CHAR_I,6);
      }
      else 
      {
        Write_Lcd_Bytes(30,1,(UCHAR *)NUM5_7_kongge,6); 
        Write_Lcd_Bytes(36,1,(UCHAR *)NUM5_7_kongge,6); 
      }
      OldDismode=Dismode;
}


//***************************************
//---------------------------------------
//���ܣ�ʵʱ���ߺ���ʷ������ͬ�����ӳ���
//��ڣ�
//���ڣ�
//---------------------------------------
void Curve_Same(signed long RangeL,signed long RangeH)
{
	Pcent(RangeL,RangeH);
	Smloc_F=0;
	Dis_CH(0,0);
	Dis6_11_One(16,0,Channo+1);
	if(Dismode==SHISHI_RUN)
	{
		if((ChannelConfig[Channo].Type<6)||(ChannelConfig[Channo].Type>11))
		{
			if(ChannelRunValue[Channo].CutDealF==1)
			{
				if(ChannelCutDealFlag[Channo]==1)
				{
					DisData(44,0,ChannelRunValue[Channo].Value,ChannelConfig[Channo].Point,5,0);
				}
            	else if(ChannelCutDealFlag[Channo]==2)
            	{
					CutDealDis(44,0,0,48);
            	}
			}
			else
          	{
				DisData(44,0,ChannelRunValue[Channo].Value,ChannelConfig[Channo].Point,5,0);
          	}
		}
		else
		{
			DisData(44,0,ChannelRunValue[Channo].Value,ChannelConfig[Channo].Point,5,0);
		}
		DisUnit(92,0,ChannelConfig[Channo].Unit);              
	}
	else if(Dismode==LISHI_RUN)
	{
		Write_nKongge(44,0,5);		//д�ո�ǿ�Ʋ���
		DisData(44,0,DisCurveBuf[0],RecordHead.Point,5,0);
		DisUnit(92,0,RecordHead.Unit);
    }  
	
	DisTwo2(0,7,Shibiao[ChannelConfig[Channo].DisplayTimes_index][ChannelConfig[Channo].RecInt_index]&0x7f);
	Smloc_F=0;
	if((Shibiao[ChannelConfig[Channo].DisplayTimes_index][ChannelConfig[Channo].RecInt_index]&0x80)==0)
    {
		Write_Lcd_Bytes(8,7,(UCHAR *)SCHAR_m,4);
	}
	if((Shibiao[ChannelConfig[Channo].DisplayTimes_index][ChannelConfig[Channo].RecInt_index]&0x80)==0x80)
	{
		Write_Lcd_Bytes(8,7,(UCHAR *)SCHAR_h,4);     
	}
	DisCurve(RangeL,RangeH);
}

//*************************************************
//-------------------------------------------------	 
//���ܣ�ʵʱ���߻���
//��ڣ�
//���ڣ�
//-------------------------------------------------
void Shishi_Run_display(void)
{
	if(Dismode!=OldDismode)
	{
		LCD_Clear();
		Dis_Heng_Line(16,0,12);	 //��ʾһ��С����
		Dis_Shu_Line(12,16,63);	 //��ʾ����
		Smloc_F=1;
		Write_Lcd_Bytes(0,4,(UCHAR *)Curve_R,11); 
		Write_Lcd_Bytes(0,5,(UCHAR *)Curve_curve,11); 
		Smloc_F=0;  
	}      
	Dis5_7Time(146,0,(UCHAR *)&Systime.Year);
	Curve_Same(ChannelConfig[Channo].RangeL,ChannelConfig[Channo].RangeH);
	if(AM==0)
	{
		Write_Lcd_Bytes(0,2,(UCHAR *)CHAR_A_,6); 
	}
	else
	{
		Write_Lcd_Bytes(0,2,(UCHAR *)CHAR_M_,6);
	}
	if(ChannelRunValue[Channo].LLALM==1)
	{
		Write_Lcd_Bytes(30,0,(UCHAR *)CHAR_L,6); 
		Write_Lcd_Bytes(36,0,(UCHAR *)CHAR_L,6);
	}
	else if(ChannelRunValue[Channo].HHALM==1)
	{
		Write_Lcd_Bytes(30,0,(UCHAR *)CHAR_H,6); 
		Write_Lcd_Bytes(36,0,(UCHAR *)CHAR_H,6);
	}
	else 
	{
		Write_Lcd_Bytes(30,0,(UCHAR *)NUM5_7_kongge,6); 
		Write_Lcd_Bytes(36,0,(UCHAR *)NUM5_7_kongge,6); 
	}
	if(ChannelRunValue[Channo].LALM==1)
	{
		Write_Lcd_Bytes(30,1,(UCHAR *)CHAR_L,6); 
		Write_Lcd_Bytes(36,1,(UCHAR *)CHAR_O,6);
	}
	else if(ChannelRunValue[Channo].HALM==1)
	{
		Write_Lcd_Bytes(30,1,(UCHAR *)CHAR_H,6); 
		Write_Lcd_Bytes(36,1,(UCHAR *)CHAR_I,6);
	}
	else 
	{
		Write_Lcd_Bytes(30,1,(UCHAR *)NUM5_7_kongge,6); 
		Write_Lcd_Bytes(36,1,(UCHAR *)NUM5_7_kongge,6); 
	}
	OldDismode=Dismode;   
}

//*************************************************
//-------------------------------------------------
//���ܣ���ʷ���߻���
//��ڣ�
//���ڣ�
//-------------------------------------------------
void Lishi_Run_display(void)
{
	unsigned char i;
	if(Dismode!=OldDismode)
	{
		LCD_Clear();
		Dis_Heng_Line(16,0,12);	     
		Dis_Shu_Line(12,16,63);
		Smloc_F=1;
		Write_Lcd_Bytes(0,4,(UCHAR *)Curve_H,11); 
		Write_Lcd_Bytes(0,5,(UCHAR *)Curve_curve,11);
		Smloc_F=0;
	}
	Dis5_7Time(146,0,(UCHAR *)&Syspar1.Htime.Year);
	if(NO_Data_Flag == 0)
	{
		Curve_Same(RecordHead.RangeL,RecordHead.RangeH);                  
	}
	else
	{
		for(i=0;i<180;i++)
		{
			DisCurveBuf[i] = 0;	
		}
		Curve_Same(RecordHead.RangeL,RecordHead.RangeH);
		Write_LCD_CChina(64,4,(UCHAR *)WU);//����ʷ����
		Write_LCD_CChina(80,4,(UCHAR *)LI1);
		Write_LCD_CChina(96,4,(UCHAR *)SHI3);
		Write_LCD_CChina(112,4,(UCHAR *)SHU1);
		Write_LCD_CChina(128,4,(UCHAR *)JU);

	}	
	OldDismode=Dismode; 
}

//*************************************************
//-------------------------------------------------
//���ܣ�����һ������
//��ڣ�
//���ڣ�
//-------------------------------------------------
void Baojing_Run_display(void)
{
      UCHAR i,j,a,k;
      if(Dismode!=OldDismode)
      {
          LCD_Clear();
      }
              
      if(page==0)
      {
          k=1;
          a=Syspar1.Almptr-1;
          if(a==0xff)
            a=15;
      }          
      if(page==1)
      {
          k=9;
          a=Syspar1.Almptr-1;
          if(a==0xff)
            a=15;
          if(a>=8)
              a=a-8;
          else if(a<8)
            a=a+8;
      }
      i=a;
      j=0;
      do
      {
          if(Syspar1.AlmRecord[i].AlmType==1)
          {
              DisTwo1(0,j,k);
              Dis5_7_One(15,j,WLW);
              Dis5_7_One(21,j,WLW);
          }
          else if(Syspar1.AlmRecord[i].AlmType==2)
          {
              DisTwo1(0,j,k);
              Dis5_7_One(15,j,WLW);
              Dis5_7_One(21,j,WOW);
          }
          else if(Syspar1.AlmRecord[i].AlmType==3)
          {
              DisTwo1(0,j,k);
              Dis5_7_One(15,j,WHW);
              Dis5_7_One(21,j,WIW);
          }
          else if(Syspar1.AlmRecord[i].AlmType==4)
          {
              DisTwo1(0,j,k);
              Dis5_7_One(15,j,WHW);
              Dis5_7_One(21,j,WHW);
          }
          else if(Syspar1.AlmRecord[i].AlmType==5)
          {
              DisTwo1(0,j,k);
              Dis5_7_One(15,j,WAW);
              Dis5_7_One(21,j,WVW);
          }
          else
          {
              break;
          }
          Dis5_7_One(30,j,Syspar1.AlmRecord[i].AlmChn+1);
          Dis5_7_date(39,j,(UCHAR *)&Syspar1.AlmRecord[i].AlmFromTime.Year);
          Dis5_7_time(88,j,(UCHAR *)&Syspar1.AlmRecord[i].AlmFromTime.Hour);
          if(Syspar1.AlmRecord[i].AlmToTime.Hour==0xAA)
          {
              Write_Lcd_Bytes(137,j,(UCHAR *)NUM5_7_kongge,3);
              for(a=0;a<5;a++)
                 Write_Lcd_Bytes(140+8*a,j,(UCHAR *)CHAR_XINGHAO,8);
              Write_Lcd_Bytes(180,j,(UCHAR *)NUM5_7_kongge,3);          
          }
          else
              Dis5_7_time(137,j,(UCHAR *)&Syspar1.AlmRecord[i].AlmToTime.Hour);
          if(Smloc==(k-1))
              Smloc_F=1;
          if(Syspar1.AlmRecord[i].AlmEnt==0)
              Write_Lcd_Bytes(186,j,(UCHAR *)NUM5_7_kongge,6);
          else if(Syspar1.AlmRecord[i].AlmEnt==1)
              Write_Lcd_Bytes(186,j,(UCHAR *)NUM5_7_ENT,6);
          Smloc_F=0;
                    
          j=j+1;
          k=k+1;
          i=i-1;
          if(i==0xff)
          i=15;
      }while(j<8);
      
//      MainDoUART();
      if(j<8)
      {
          for(i=j;i<8;i++)
          {
              for(k=0;k<192;k=k+6)
              {
                  Dis5_7_One(k,i,WkonggeW);
              }
          }
      }     
      OldDismode=Dismode;
}
//************************************************
//------------------------------------------------
//���ܣ��������Ի���
//------------------------------------------------
void Test_AD_display(void)
{
      if(Dismode!=OldDismode)
      {
          LCD_Clear();
          Dis_CH(0,0);
          Dis6_11_One(16,0,1);
          Dis_CH(0,2);
          Dis6_11_One(16,2,2);
          Dis_CH(0,4);
          Dis6_11_One(16,4,3);
          Dis_CH(0,6);
          Dis6_11_One(16,6,4);
      }
      DisSmloc(0);
      DisType(24,0,ChannelConfig[0].Type);
      Smloc_F=0;
      DisSmloc(1);       
      DisType(24,2,ChannelConfig[1].Type);
      Smloc_F=0;
      DisSmloc(2);
      DisType(24,4,ChannelConfig[2].Type);
      Smloc_F=0;
      DisSmloc(3);
      DisType(24,6,ChannelConfig[3].Type);
      Smloc_F=0;

	  Dis6_11_One(84,0,WBW);
	  Dis6_11_One(136,0,WCW);
	DisData(92,0,ChannelRunValue[0].B_AD_Value,0,4,0);
	DisData(144,0,ChannelRunValue[0].C_AD_Value,0,4,0);
	DisData(92,2,ChannelRunValue[1].B_AD_Value,0,4,0);
	DisData(144,2,ChannelRunValue[1].C_AD_Value,0,4,0);
	DisData(92,4,ChannelRunValue[2].B_AD_Value,0,4,0);
	DisData(144,4,ChannelRunValue[2].C_AD_Value,0,4,0);
	DisData(92,6,ChannelRunValue[3].B_AD_Value,0,4,0);
	DisData(144,6,ChannelRunValue[3].C_AD_Value,0,4,0);
      OldDismode=Dismode;
}
void Test_AI_display(void)
{
	unsigned char i;
	signed long Test_AI_Temp[4];
	if(Dismode!=OldDismode)
	{
		LCD_Clear();
		Dis_CH(0,0);
		Dis6_11_One(16,0,1);
		Dis_CH(0,2);
		Dis6_11_One(16,2,2);
		Dis_CH(0,4);
		Dis6_11_One(16,4,3);
		Dis_CH(0,6);
		Dis6_11_One(16,6,4);
	}
	DisSmloc(0);
	DisType(24,0,ChannelConfig[0].Type);
	Smloc_F=0;
	DisSmloc(1);       
	DisType(24,2,ChannelConfig[1].Type);
	Smloc_F=0;
	DisSmloc(2);
	DisType(24,4,ChannelConfig[2].Type);
	Smloc_F=0;
	DisSmloc(3);
	DisType(24,6,ChannelConfig[3].Type);
	Smloc_F=0;
	for(i=0;i<4;i++)
	{
		Test_AI_Temp[i] = (signed long)(ChannelRunValue[i].InputValue*100);	  	
	}
	DisData(100,0,Test_AI_Temp[0],2,5,0);
	DisData(100,2,Test_AI_Temp[1],2,5,0);
	DisData(100,4,Test_AI_Temp[2],2,5,0);
	DisData(100,6,Test_AI_Temp[3],2,5,0);
	OldDismode=Dismode;
}
/*****************************************
// �������ƣ� void BiaoDing_display(void)
// �������ܣ� �궨����
// ��ڲ����� ��
// ���ڲ����� �� 
***********************************/
void BiaoDing_display(void)
{
	if(Dismode!=OldDismode)
	{
		LCD_Clear();
		Write_LCD_CChina(0,0,(UCHAR *)BIAO);//�궨
		Write_LCD_CChina(16,0,(UCHAR *)DING);
		Dis_Shu_Line(33,0,63);
		Dis_Shu_Line(34,0,63);
		Write_LCD_CChina(37,0,(UCHAR *)TONG1);//ͨ��
		Write_LCD_CChina(53,0,(UCHAR *)DAO);
		Write_LCD_CChina(101,0,(UCHAR *)XIN);//�ź�
		Write_LCD_CChina(117,0,(UCHAR *)HAO);
		Write_LCD_CChina(50,2,(UCHAR *)BIAO);//�궨
		Write_LCD_CChina(66,2,(UCHAR *)DING);
		Write_LCD_CChina(37,4,(UCHAR *)CAI);//����
		Write_LCD_CChina(53,4,(UCHAR *)YANG);
		Write_LCD_CChina(69,4,(UCHAR *)ZHI4);//ֵ
		Dis6_11_One(85,4,WBW);							  
		Dis6_11_One(141,4,WCW);
		Write_LCD_CChina(37,6,(UCHAR *)BIAO);//�궨
		Write_LCD_CChina(53,6,(UCHAR *)DING);
		Write_LCD_CChina(69,6,(UCHAR *)ZHI4);//ֵ
	}
	Smloc_F=0;
	if(BiaoDing.Type == BIAODING_0_20MV) 	//��ʾ0~20mV�궨ֵ
	{
		if(BiaoDing.L0H1 == 0)			   	//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BD20MV_Low,0,4,0);
		else							   	//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BD20MV_High,0,4,0);
	}
	else if(BiaoDing.Type == BIAODING_0_100MV) 	//��ʾ0~100mV�궨ֵ
	{
		if(BiaoDing.L0H1 == 0)			   	//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BD100MV_Low,0,4,0);
		else							   	//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BD100MV_High,0,4,0);
	}
	else if(BiaoDing.Type == BIAODING_0_5V) 	//��ʾ0~5V�궨ֵ
	{
		if(BiaoDing.L0H1 == 0)			   	//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BD5V_Low,0,4,0);
		else							   	//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BD5V_High,0,4,0);
	}
	else if(BiaoDing.Type == BIAODING_0_10V) 	//��ʾ0~10V�궨ֵ
	{
		if(BiaoDing.L0H1 == 0)			   	//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BD10V_Low,0,4,0);
		else							   	//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BD10V_High,0,4,0);
	}
	else if(BiaoDing.Type == BIAODING_0_20MA) 	//��ʾ0~20mA�궨ֵ
	{
		if(BiaoDing.L0H1 == 0)			   	//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BD20MA_Low,0,4,0);
		else							   	//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BD20MA_High,0,4,0);
	}
	else if(BiaoDing.Type == BIAODING_PT100)   //��ʾPT100�궨ֵ
	{
		if(BiaoDing.L0H1 == 0)			   	//��ʾ����
		{
			DisData(93,6,ChannelConfig[BiaoDing.CH].BDPt100_Low,0,4,0);
		}
		else
		{							   		//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BDPt100_High,0,4,0);
		}
	}
	else if(BiaoDing.Type == BIAODING_CU50)	  //��ʾCu50�궨ֵ
	{
		if(BiaoDing.L0H1 == 0)			   	//��ʾ����
		{
			DisData(93,6,ChannelConfig[BiaoDing.CH].BDCu50_Low,0,4,0);
		}
		else
		{							   		//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BDCu50_High,0,4,0);
		}
	}
	else if(BiaoDing.Type == BIAODING_PT1000) //��ʾPT1000�궨ֵ
	{
		if(BiaoDing.L0H1 == 0)			   	//��ʾ����
		{
			DisData(93,6,ChannelConfig[BiaoDing.CH].BDPt1000_Low,0,4,0);
		}
		else
		{							   		//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BDPt1000_High,0,4,0);
		}
	}
	else if(BiaoDing.Type == BIAODING_0_1000MV) 	//��ʾ0~1000mV�궨ֵ
	{
		if(BiaoDing.L0H1 == 0)			   	//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BD1000MV_Low,0,4,0);
		else							   	//��ʾ����
			DisData(93,6,ChannelConfig[BiaoDing.CH].BD1000MV_High,0,4,0);
	}
	DisData(93,4,ChannelRunValue[BiaoDing.CH].B_AD_Value,0,4,0);	 //��ʾ����ֵ
	DisData(148,4,ChannelRunValue[BiaoDing.CH].C_AD_Value,0,4,0);
	DisSmloc(0);
	Write_LCD_CChina(0,6,(UCHAR *)FAN);//����
	Write_LCD_CChina(16,6,(UCHAR *)HUI);
	DisSmloc(1);
	Dis6_11_One(77,0,BiaoDing.CH+1);
	DisSmloc(2);
	if(BiaoDing.Type == BIAODING_0_100MV)	   		                //�任�ź�����
		ChannelConfig[BiaoDing.CH].Type = TYPE_0_100MV;
	else if(BiaoDing.Type == BIAODING_0_5V)
		ChannelConfig[BiaoDing.CH].Type = TYPE_0_5V;
	else if(BiaoDing.Type == BIAODING_0_10V)
		ChannelConfig[BiaoDing.CH].Type = TYPE_0_10V;
	else if(BiaoDing.Type == BIAODING_0_20MA)
		ChannelConfig[BiaoDing.CH].Type = TYPE_0_20MA;
	else if(BiaoDing.Type == BIAODING_PT100)
		ChannelConfig[BiaoDing.CH].Type = TYPE_PT100;
	else if(BiaoDing.Type == BIAODING_CU50)
		ChannelConfig[BiaoDing.CH].Type = TYPE_CU50;
	else if(BiaoDing.Type == BIAODING_PT1000)
		ChannelConfig[BiaoDing.CH].Type = TYPE_PT1000;
	else if(BiaoDing.Type == BIAODING_0_1000MV)	   	
		ChannelConfig[BiaoDing.CH].Type = TYPE_0_1000MV;
	else
		ChannelConfig[BiaoDing.CH].Type = TYPE_0_20MV;
	DisType(135,0,ChannelConfig[BiaoDing.CH].Type);
	DisSmloc(3);
	if(BiaoDing.L0H1==0)
		Write_LCD_CChina(82,2,(UCHAR *)XIA);   //��
	else
		Write_LCD_CChina(82,2,(UCHAR *)SHANG); //��
	Write_LCD_CChina(98,2,(UCHAR *)XIAN);//��
	DisSmloc(4);
	Write_LCD_CChina(150,2,(UCHAR *)QUE);//ȷ��
	Write_LCD_CChina(166,2,(UCHAR *)DING);
	OldDismode=Dismode;
}
//*************************************************
//���ܣ��Ǳ����û���
//-------------------------------------------------
void VPRConfig_display(void)
{
      if(Dismode!=OldDismode)
      {
          LCD_Clear();
          Dis_Shu_Line(33,0,63);
          Dis_Shu_Line(34,0,63);
          Write_LCD_CChina(37,1,(UCHAR *)SHU);//����
	  Write_LCD_CChina(53,1,(UCHAR *)RU);
	  Write_LCD_CChina(37,5,(UCHAR *)BAO1);//����
	  Write_LCD_CChina(53,5,(UCHAR *)JING);
          Dis6_11_One(96,1,WUW);
          Dis6_11_One(104,1,WSW);
          Dis6_11_One(112,1,WBW);
	  //Write_LCD_CChina(96,1,(UCHAR *)SHU);//���
	  //Write_LCD_CChina(112,1,(UCHAR *)CHU);
	  //Write_LCD_CChina(96,3,(UCHAR *)KONG);//����
	  //Write_LCD_CChina(112,3,(UCHAR *)ZHI);
	  Write_LCD_CChina(96,5,(UCHAR *)TONG1);//ͨѶ
	  Write_LCD_CChina(112,5,(UCHAR *)XUN);
      } 
      DisSmloc(0);
      Write_LCD_CChina(0,6,(UCHAR *)TUI);//�˳�
      Write_LCD_CChina(16,6,(UCHAR *)CHU);
      DisSmloc(1);
      Dis6_11_One(73,1,Syspar.Pvnum);
      DisSmloc(2);
      Dis6_11_One(73,5,Syspar.Relnum);
      DisSmloc(3);
      if((Syspar.Flag&0x01)==0)
          Write_LCD_CChina(134,1,(UCHAR *)FOU);
      else
          Write_LCD_CChina(134,1,(UCHAR *)SHI1);
      /*DisSmloc(4);
      if((Syspar.Flag&0x02)==0)
          Write_LCD_CChina(134,3,(UCHAR *)FOU);
      else
          Write_LCD_CChina(134,3,(UCHAR *)SHI1);*/
      DisSmloc(4);
      if((Syspar.Flag&0x04)==0)
          Write_LCD_CChina(134,5,(UCHAR *)FOU);
      else
          Write_LCD_CChina(134,5,(UCHAR *)SHI1);
      Smloc_F=0;
      OldDismode=Dismode;
}
//*************************************************
//-------------------------------------------------
//����:��ʾ������
//���:��
//����:��
//-------------------------------------------------
void MainDisplay(void)
{       
	if(Dismode==ENTER_MIMA)				// ��������
		Enter_mima_display();
	if(Dismode==ZHU_ZUTAI)				//����̬����
		Zhu_zutai_display();
	if(Dismode==XITONG_ZUTAI)				//ϵͳ��̬
		Xitong_zutai_display();
	if((Dismode==TONGDAO_ZUTAI1)||(Dismode==16)||(Dismode==17))	//ͨ����̬����1
		Tongdao_zutai1_display();
	if(Dismode==TONGDAO_ZUTAI2)		//ͨ����̬����2
		Tongdao_zutai2_display();
	if(Dismode==5)
		USB_zutai_display1();
	if(Dismode==6)
		USB_zutai_display2();
	if(Dismode==WANGLUO_ZUTAI)
		Wangluo_zutai_display();
	if(Dismode==BAOJING_ZUTAI||(Dismode==18)||(Dismode==19)||(Dismode==20)||(Dismode==21)||(Dismode==22))
		Baojing_zutai_display();
	if(Dismode==TONGXUN_ZUTAI)
		Tongxun_zutai_display();
	if(Dismode==YIBIAO_PEIZHI)
		VPRConfig_display();
	if(Dismode==BIAODING)
		BiaoDing_display();
	if(Dismode==XIANSHI_AD)			//��ʾ����AD���ݻ���
		Test_AD_display();
	if(Dismode==XIANSHI_AI)	  		//��ʾ�����ź�ģ��������
		Test_AI_display();
	if(Dismode==TINGZHIJILU)
		TingZhiJiLu_display();
	if(Dismode==TINGZHIJILU1)
		TingZhiJiLu1_display();
		
	if(Dismode==SHUXIAN_RUN) 	//��ͨ�����Ի��� 
		Shuxian_Run_display();
	if(Dismode==SHISHI_RUN)		//ʵʱ���߻���
		Shishi_Run_display();
	if(Dismode==LISHI_RUN)	   	//��ʷ���߻���
		Lishi_Run_display();
	if(Dismode==BAOJING_RUN)	//��������
		Baojing_Run_display();
	if(Dismode==SHUXIAN4_RUN)	//4ͨ�����Ի���
		Shuxian4_Run_display();
	

	if(Dismode==100)
	{    
	    if(Dismode!=OldDismode);
            //MenuCB.MenuNewFlag=1;
	    //pppp=(UCHAR *)Picture;
	    //a=*pppp;
	    
	    //HMI_MenuRefreshFlag = 1;///////////////////////////////////////////////
	   // MenuDisp();////////////////////////////////////////////////////////////
	    	    
	    OldDismode=Dismode;
	}
//        MainDoUART(); 
}


//*************************************************
//-------------------------------------------------
//���ܣ�����ʵʱ����
//��ڣ�
//���ڣ�
//-------------------------------------------------	 //�������û�����
void Put_SHISHI_Data(void)
{
    UCHAR i;
    if(Dismode==SHISHI_RUN)
    {
        for(i=CURVE_NUM;i>0;i--)
        {
            DisCurveBuf[i]=DisCurveBuf[i-1];   
        }
        DisCurveBuf[0]=ChannelRunValue[Channo].Value;  
    }   
}

void ADValue_Display(void)
{
}

void InitPCF8563_Display(void)
{
	Write_LCD_CChina(  0,0,(UCHAR *)CHU3);//��ʼ��
	Write_LCD_CChina( 16,0,(UCHAR *)SHI6);
	Write_LCD_CChina( 32,0,(UCHAR *)HUA);
	Write_LCD_CChina( 48,0,(UCHAR *)SHI);  //ʱ��
	Write_LCD_CChina( 64,0,(UCHAR *)ZHONG1);
	Write_LCD_CChina( 80,0,(UCHAR *)DDD);  //...
	Write_LCD_CChina( 96,0,(UCHAR *)DDD);
	Write_LCD_CChina(112,0,(UCHAR *)DDD);
	Write_LCD_CChina(128,0,(UCHAR *)DDD);
}
void InitNET_Display(void)
{
	Write_LCD_CChina(  0,2,(UCHAR *)CHU3);//��ʼ��
	Write_LCD_CChina( 16,2,(UCHAR *)SHI6);
	Write_LCD_CChina( 32,2,(UCHAR *)HUA);
	Write_LCD_CChina( 48,2,(UCHAR *)WANG);  //����
	Write_LCD_CChina( 64,2,(UCHAR *)KA);
	Write_LCD_CChina( 80,2,(UCHAR *)DDD);  //...
	Write_LCD_CChina( 96,2,(UCHAR *)DDD);
	Write_LCD_CChina(112,2,(UCHAR *)DDD);
	Write_LCD_CChina(128,2,(UCHAR *)DDD);
}
void InitPar_Display(void)
{
	Write_LCD_CChina(  0,4,(UCHAR *)CHU3);//��ʼ��
	Write_LCD_CChina( 16,4,(UCHAR *)SHI6);
	Write_LCD_CChina( 32,4,(UCHAR *)HUA);
	Write_LCD_CChina( 48,4,(UCHAR *)XI);  //ϵͳ
	Write_LCD_CChina( 64,4,(UCHAR *)TONG);
	Write_LCD_CChina( 80,4,(UCHAR *)CAN);  //����
	Write_LCD_CChina( 96,4,(UCHAR *)SHU1);
	Write_LCD_CChina(112,4,(UCHAR *)DDD);
	Write_LCD_CChina(128,4,(UCHAR *)DDD);
}
void InitFATFS_Display(void)
{
	Write_LCD_CChina(  0,6,(UCHAR *)CHU3);//��ʼ��
	Write_LCD_CChina( 16,6,(UCHAR *)SHI6);
	Write_LCD_CChina( 32,6,(UCHAR *)HUA);
	Write_LCD_CChina( 48,6,(UCHAR *)WEN);  //�ļ�
	Write_LCD_CChina( 64,6,(UCHAR *)JIAN1);
	Write_LCD_CChina( 80,6,(UCHAR *)XI);  //ϵͳ
	Write_LCD_CChina( 96,6,(UCHAR *)TONG);
	Write_LCD_CChina(112,6,(UCHAR *)DDD);
	Write_LCD_CChina(128,6,(UCHAR *)DDD);
}

void OK_Display(unsigned char Hang)
{
	Write_LCD_CChina(144,Hang,(UCHAR *)CHENG1);//�ɹ�
	Write_LCD_CChina(160,Hang,(UCHAR *)GONG);
}
void ERROR_Display(unsigned char Hang)
{
	Write_LCD_CChina(144,Hang,(UCHAR *)SHI7);//ʧ��
	Write_LCD_CChina(160,Hang,(UCHAR *)BAI);
}
