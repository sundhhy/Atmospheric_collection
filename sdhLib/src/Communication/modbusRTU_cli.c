/**
* @file 		modbusRTU_cli.c
* @brief		ʵ��modbusRTU��վ����.
* @details		֧������:3,6,16
* @author		author
* @date		date
* @version	A001
* @par Copyright (c): 
* 		XXX??
* @par History:         
*	version: author, date, desc\n
*	V0.1:sundh,17-01-15,����ʵ��0��3������
*/

//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
#include <string.h>
#include "Communication/modbusRTU_cli.h"
#include "arithmetic/crc.h"

#include "sdhDef.h"
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------




/*MODBUS ??????-----------------------------------------------------------------------*/
#define REG_LINE 	0												//���Ե�ַ
#define REG_MODBUS 	1												//MODBUD???? ???0?00001,1?10001,3?30001,4?40001
#define COIL_ON  	PIO_ON											//?????
#define COIL_OFF 	PIO_OFF											//?????
#define STATE_ON 	1												//?????
#define STATE_OFF 	0												//?????



/* Modbus err ************************************************************************/
#define MB_CMD_ERR		1
#define MB_ADDR_ERR		2
#define MB_DATA_ERR		3
#define MB_MB_ERR		4
//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static uint16_t Little_end_to_Big_end( uint16_t val16);
static uint16_t Big_end_to_Little_end( uint16_t val16);

//static uint16_t regType3_read(uint8_t *p, uint16_t reg_type);
//static int regType3_write(uint16_t hold_address, uint16_t reg_type, uint16_t val);
//static uint16_t regType4_read(uint16_t input_address, uint16_t reg_type);
//static uint16_t regType4_read(uint16_t input_address, uint16_t reg_type);
//static int regType4_write(uint16_t input_address, uint16_t reg_type, uint16_t val);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

uint8_t  MBC_Get_pkt_addr(uint8_t *command_buf)
{
	return command_buf[0];
}
uint16_t MBC_Decode_pkt(uint8_t *command_buf, int cmd_len, uint8_t *ack_buf, int ackbuf_len)
{
	int		ret = 0;
	uint16_t	*p_ram_u16;
	uint16_t i, startReg, regNum, data, ack_num;
	uint16_t crc16 = 0;
	uint16_t *p_playload = NULL;
	uint8_t  err = 0, byteNum = 0;
	for(i=0; i<6; i++) 
	{
		ack_buf[i] = command_buf[i];											//?????
	}	
	crc16 = Crc16( 	command_buf, 	( cmd_len - 2));
	//todo Э���Ϲ涨CRC�ǵ��ֽ���ǰ��Ȼ��ʵ�ʲ�����ȴ�Ǹ��ֽ���ǰ����Ҫ�ٴ�ȷ�ϣ����������ModbuScan��
	data = command_buf[ cmd_len - 1 ] + ( command_buf[ cmd_len - 2] << 8);		//read crc 
	if( crc16 != data)
	{
		err = MB_DATA_ERR;
	}
	else
	{
		startReg = command_buf[3] + (command_buf[2] << 8);						//??????
		regNum = command_buf[5] + (command_buf[4] << 8);							//???????
		switch(command_buf[1])
		{
			/*��ȡ��Coil״̬ ��0��			-------------------------------------------*/
			case READ_COIL:		//��Ч������
				err=MB_CMD_ERR;
				break;
			/*������״̬ ��1��----------------------------------------------------------------------*/
			case READ_STATE:														
				
				break;

			/*������Ĵ��� 3��------------------------------------------------------------------*/
			case READ_INPUT:	
				ret = MBC_reg_2_ram(startReg, regNum, READ_INPUT, &p_ram_u16);																	//regNum=127;	  ??????
//				if((startReg >= INPUT_SIZE) || (regNum >= 125) || ((startReg + regNum) > INPUT_SIZE) ) 
				if(ret != RET_OK)
				{
					err= MB_ADDR_ERR;	
					break;
				}
							
				
				
				if( ackbuf_len < (regNum*2  + 5))
				{
					
					err= MB_ADDR_ERR;	
					break;
				}
				
				ack_buf[2] = regNum*2;
				ack_num = 3;
					
				for(i=0; i<regNum; i++)
				{
					data = *p_ram_u16 ++;
					
			#ifdef CPU_LITTLE_END
					data = Little_end_to_Big_end( data);
			#endif
						
					ack_buf[ack_num] = data; 			
					ack_num++; 
					ack_buf[ack_num] = data >>8;				
					ack_num++;	
				}
			break;
		/*������Ĵ��� 4�������ּĴ���------------------------------------------------------------------*/
			case READ_HOLD:	
																					//regNum=127;	  ??????
				
				ret = MBC_reg_2_ram(startReg, regNum, READ_HOLD, NULL);																	//regNum=127;	  ??????
				if(ret != RET_OK)
				{
					err = MB_ADDR_ERR;	
					break;
				}
							
				
				if( ackbuf_len < (regNum*2  + 5))
				{
					
					err= MB_ADDR_ERR;	
					break;
				}
				ack_buf[2] = regNum*2;
				ack_num = 3;
				for(i=0; i<regNum; i++)
				{
					
					ret = MBC_reg_2_ram(startReg, 1, READ_HOLD, &p_ram_u16);
					data = *p_ram_u16;	
			#ifdef CPU_LITTLE_END
					data = Little_end_to_Big_end( data);
			#endif
					
						
					ack_buf[ack_num] = data; 			
					ack_num++; 
					ack_buf[ack_num] = data >>8;				
					ack_num++;	
					startReg ++;
				}
				break;
			/*д����Ĵ��� 4�������ּĴ���------------------------------------------------------------------*/
			case WRITE_1_HOLD:	
																					//regNum=127;	  ??????
				
				
				
							
				p_playload = (uint16_t *)(command_buf + 4);
				
				data = *p_playload;	
		#ifdef CPU_LITTLE_END
				data = Big_end_to_Little_end( data);
		#endif
				//�������ֵд��ram
				ret = MBC_reg_2_ram(startReg, regNum, WRITE_1_HOLD, &data);																	//regNum=127;	  ??????
				if(ret != RET_OK)
				{
					err = MB_ADDR_ERR;	
					break;
				}
				//��ram�лض�����
				ret = MBC_reg_2_ram(startReg, regNum, READ_HOLD, &data);																	//regNum=127;	  ??????
				if(ret != RET_OK)
				{
					err = MB_ADDR_ERR;	
					break;
				}
				data = *p_ram_u16;	
			#ifdef CPU_LITTLE_END
				data = Little_end_to_Big_end( data);
			#endif
				
				ack_num = 4;
				ack_buf[ack_num] = data; 			
				ack_num++; 
				ack_buf[ack_num] = data >>8 ;				
				ack_num++;	
				
				break;
			case WRITE_N_HOLD:	
				//����һ�£��Ĵ����ķ�Χ�Ƿ���ȷ																//regNum=127;	  ??????
				ret = MBC_reg_2_ram(startReg, regNum, WRITE_N_HOLD, NULL);																	//regNum=127;	  ??????
				if(ret != RET_OK)
				{
					err = MB_ADDR_ERR;	
					break;
				}
				byteNum = 	command_buf[ 6];
				if( byteNum != regNum * 2)
				{
					err = MB_DATA_ERR;	
					break;
				}
				
				for(i=0; i< byteNum; i+= 2)
				{
					data = 0;
					data = command_buf[ 7 + i ] + (command_buf[ 8 + i ] << 8);
		#ifdef CPU_LITTLE_END
					data = Big_end_to_Little_end( data);
		#endif				
					MBC_reg_2_ram(startReg, 1, WRITE_1_HOLD, &data);	
					startReg ++;
				}
				
				
				
				
				ack_num = 6;
				
				break;
				
			default:
				err = MB_CMD_ERR; 
			break;
		}
	}
	
	//todo:��crcУ����ҲҪ���ڽ�β��	
	if (err )
    {
		ack_buf[1] = ack_buf[1] | 0x80;											 //��Ч������
		ack_buf[2] = err;            
		ack_num = 3;
	}	
	
	
	crc16 = Crc16( 	ack_buf, 	ack_num);
	ack_buf[ack_num] = crc16 >>8; 	
	ack_num++;
	ack_buf[ack_num] = crc16 ; 	
	ack_num ++;	
	return ack_num;
	
}
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{


#ifdef CPU_LITTLE_END
static uint16_t Little_end_to_Big_end( uint16_t val16)
{
	uint8_t u16_h = val16  >> 8;
	uint8_t u16_l =  val16 & 0xff;
	uint16_t bendval = ( u16_l << 8) | u16_h;
	
	return bendval;
}

static uint16_t Big_end_to_Little_end( uint16_t val16)
{
	uint8_t u16_h = ( val16 ) ;
	uint8_t u16_l =  val16 >> 8;
	uint16_t lsb = ( u16_h << 8) | u16_l;
	
	return lsb;
}
#endif


//static uint16_t regType3_read(uint8_t	 *p, uint16_t reg_type)
//{
//	uint16_t tmp;
//	
//	
//	
//	if(reg_type==REG_MODBUS)
//	{
//		
//		tmp = *(p);
//#ifdef CPU_LITTLE_END
//		tmp = Little_end_to_Big_end( tmp);
//#endif	
//	}
//	else
//	{
//		tmp = *(p);
//	}

//	return tmp;
//}

//static int regType3_write(uint16_t hold_address, uint16_t reg_type, uint16_t val)
//{
//	char chn_flag = 0;
//	uint16_t tmp = val;
//	if(reg_type==REG_MODBUS)
//	{
//		if( hold_address > 40000)
//			hold_address-=40001;
//#ifdef CPU_LITTLE_END
//		tmp = Big_end_to_Little_end( tmp);
//#endif
//		
//		if( *(HOLD_ADDRESS + hold_address) != tmp)
//			chn_flag = 1;
//	}
//	else
//	{

//	}
//	
//	

//	*(HOLD_ADDRESS + hold_address) = tmp;

//	return ERR_OK;
//}


//static uint16_t regType4_read(uint16_t input_address, uint16_t reg_type)
//{
//	
//	uint16_t tmp;
//	if(reg_type==REG_MODBUS)
//	{
//		if( input_address > 30000)
//			input_address-=30001;
//		
//		tmp = *(INPUT_ADDRESS + input_address);
//#ifdef CPU_LITTLE_END
//		tmp = Little_end_to_Big_end( tmp);
//#endif	
//	}
//	else
//	{
//		tmp = *(INPUT_ADDRESS + input_address);
//		
//	}
//	  
//	return tmp;
//}

//static int regType4_write(uint16_t input_address, uint16_t reg_type, uint16_t val)
//{
//	uint16_t tmp = val;
//	if(reg_type==REG_MODBUS)
//		return ERR_FAIL;

//	*(INPUT_ADDRESS + input_address) = tmp;
//	return ERR_OK;
//}










