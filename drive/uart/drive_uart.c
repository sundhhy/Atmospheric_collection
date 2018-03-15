/**
* @file 		gprs_uart.c
* @brief		�ṩΪgprsģ�����Ĵ��ڹ���.
* @details	
* @author		sundh
* @date		16-09-20
* @version	A001
* @par Copyright (c): 
* 		XXX??
* @par History:         
*	version: author, date, desc\n
* 	A001:sundh,16-09-20,��ʼ������
*/
#include "stdint.h"
#include "uart/drive_uart.h"
#include "os/os_depend.h"
#include "basis/sdhError.h"
#include <stdarg.h>
#include <string.h>
#include "sdhDef.h"
#include "mem/CiiMem.h"



//�����жϳ����У��ҵ���Ӧ���豸
static driveUart	*devArry[NUM_UARTS];


static void UartDma_Init( driveUart *self);
static void DRI_Uart_clean_idle(USART_TypeDef* USARTx);

static int UartInit( driveUart *self, void *cfg)
{
	CfgUart_t *myCfg = ( CfgUart_t *)cfg;
	devArry[ myCfg->uartNum] = self;
	self->cfg = cfg;
	self->rxCache = calloc( 1, UART_RXCACHE_SIZE);
	
	if(myCfg->opt_mode != UART_MODE_CPU)
	{
		self->txCache = calloc( 1, UART_TXCACHE_SIZE);
		init_pingponfbuf( &self->ctl.pingpong,(char *) self->rxCache, UART_RXCACHE_SIZE, TURE);
	}
	
	
	USART_Cmd( self->devUartBase, DISABLE);
	USART_DeInit( self->devUartBase);
	
	USART_Init( self->devUartBase, myCfg->cfguart);
	if(myCfg->opt_mode == UART_MODE_DMA)
	{
		
		UartDma_Init( self);
		
	}

	if(myCfg->opt_mode != UART_MODE_CPU)
	{
//		USART_ClearFlag( self->devUartBase,USART_IT_IDLE );
		
		USART_ITConfig( self->devUartBase, USART_IT_RXNE, ENABLE);
		USART_ITConfig( self->devUartBase, USART_IT_IDLE, ENABLE);
	}
	
	if(myCfg->opt_mode == UART_MODE_DMA)
		USART_DMACmd( self->devUartBase, USART_DMAReq_Tx, ENABLE);  // ����DMA����
	if(myCfg->opt_mode == UART_MODE_DMA)
		USART_DMACmd( self->devUartBase, USART_DMAReq_Rx, ENABLE); // ����DMA����
	
	USART_Cmd( self->devUartBase, ENABLE);
	self->ctl.rx_block = 1;
	self->ctl.tx_block = 1;
	self->ctl.rx_waittime_ms = 100;
	self->ctl.tx_waittime_ms = 100;
	
	if(myCfg->opt_mode != UART_MODE_CPU)
	{
		USART_ClearFlag( self->devUartBase,USART_IT_RXNE );
		DRI_Uart_clean_idle(self->devUartBase);
	}
	return ERR_OK;
	
}


void DRI_Uart_Change_baud(driveUart *p_dri, int new_baud)
{
	
	CfgUart_t *myCfg = ( CfgUart_t *)p_dri->cfg;
	
	if(myCfg->cfguart->USART_BaudRate == new_baud)
		return;
	myCfg->cfguart->USART_BaudRate = new_baud;
	
	USART_Cmd( p_dri->devUartBase, DISABLE);
	USART_DeInit( p_dri->devUartBase);
	
	USART_Init(p_dri->devUartBase, myCfg->cfguart);
	
	
	
}


static int UartDeInit( driveUart *self)
{
	CfgUart_t *myCfg = ( CfgUart_t *)self->cfg;
	free( self->rxCache);
	if(myCfg->opt_mode != UART_MODE_CPU)
		free( self->txCache);

	
	USART_Cmd( self->devUartBase, DISABLE);
	USART_DeInit( self->devUartBase);
	

	
	self->rxLedHdl = NULL;
	self->txLedHdl = NULL;
	self->rxIdp = NULL;
	self->txIdp = NULL;
	self->rxWait = NULL;
	self->txWait = NULL;
	self->rxPost = NULL;
	self->txPost = NULL;
	
	return ERR_OK;
	
}

/*!
**
**
** @param data 
** @param size 
** @return
**/
static int UartWrite( driveUart *self, void *buf, int wrLen)
{
	CfgUart_t 	*myCfg = ( CfgUart_t *)self->cfg;
	int 		ret;
	uint8_t 	*sendbuf = buf;
	int 		count = 0;
//	uint32_t	cndtr = 0;

//	if(myCfg->opt_mode == UART_MODE_DMA)
//	{
//		cndtr = myCfg->dma->dma_tx_base->CNDTR;
//		while(myCfg->dma->dma_tx_base->CNDTR)
//			count ++;
//		
//	}
//	else if(myCfg->opt_mode == UART_MODE_INTR)
//	{
//		while(self->ctl.sendingCount < self->ctl.sendingLen)
//		{
//			count ++;
//		}
//			
//			
//	}

//	if(count)
//	{
//		count = 0;
// 		return ERR_BUSY;
//	}
	if( buf == NULL || wrLen == 0)
		return ERR_BAD_PARAMETER;
	if(self->status == UARTSTATUS_TXBUSY)
		return ERR_BUSY;
	
	self->ioctol( self, DRICMD_SET_DIR_TX);
	
	if((myCfg->opt_mode != UART_MODE_CPU))
	{
		
		self->status = UARTSTATUS_TXBUSY;
		if((wrLen  < UART_TXCACHE_SIZE))
		{
//			memset( self->txCache, 0, UART_TXCACHE_SIZE);
			memcpy( self->txCache, buf, wrLen);
			sendbuf = self->txCache;
		
		}
		else
		{
			sendbuf = buf;
			
		}
		
		
		if(myCfg->opt_mode == UART_MODE_DMA)
		{
			myCfg->dma->dma_tx_base->CMAR = (uint32_t)sendbuf;
			myCfg->dma->dma_tx_base->CNDTR = (uint16_t)wrLen; 
			DMA_Cmd( myCfg->dma->dma_tx_base, ENABLE);        //��ʼDMA����
		}
		else	//intr
		{
			self->ctl.intrSendingBuf = sendbuf;
			self->ctl.sendingCount = 0;
			self->ctl.sendingLen = wrLen;
			USART_SendData(self->devUartBase, sendbuf[0]);
			USART_ITConfig(self->devUartBase, USART_IT_TXE, ENABLE);
			
		}
		

		if( self->ctl.tx_block)
		{
			if( self->txWait)
			{
				
				ret = self->txWait( self, self->ctl.tx_waittime_ms);
			}
			
			
			if ( ret > 0) 
			{

				return ERR_OK;
			}
			
			return ERR_DEV_TIMEOUT;
			
		}
	
	}	//intr or dma
	else		//cpu
	{
		while( count < wrLen)
		{
			USART_SendData( self->devUartBase, sendbuf[count]);
			while( USART_GetFlagStatus( self->devUartBase, USART_FLAG_TXE) == RESET){};
			count ++;
		}
		
	}
	
	return ERR_OK;
}



/*!
**
**
** @param data 
** @param size 
** @return
**/
static int UartRead( driveUart *self, void *buf, int rdLen)
{
	int  ret = 1;
	uint32_t	 safe_count = 0;
	int len = rdLen;
	uint8_t *playloadbuf ;
	CfgUart_t *myCfg = ( CfgUart_t *)self->cfg;
	if( buf == NULL)
		return ERR_BAD_PARAMETER;
	
	if(myCfg->opt_mode == UART_MODE_CPU)
	{
		len = 0;
		playloadbuf = (uint8_t *)buf;
		safe_count = self->ctl.rx_waittime_ms * 1000;
		if(safe_count == 0)
			safe_count = 0xffffffff;
		while(len < rdLen)
		{
			if(USART_GetITStatus( self->devUartBase, USART_IT_RXNE) == SET) 
			{
				playloadbuf[len] = USART_ReceiveData(self->devUartBase);
				len ++;
			}
			if((safe_count != 0xffffffff) && (safe_count > 0))
				safe_count --;
			else
				break;
		}
		
		return len;
	}
	
	if( self->rxWait)
	{
		
		ret = self->rxWait( self, self->ctl.rx_waittime_ms);
	}
	
	if( ret > 0)
	{
		if(myCfg->uartNum == 3)
		{
			safe_count = 0;
			
		}
		if( len > self->ctl.recv_size)
			len = self->ctl.recv_size;
		playloadbuf = (uint8_t *)get_playloadbuf( &self->ctl.pingpong);
		memset( buf, 0, rdLen);
		memcpy( buf, playloadbuf, len);
		memset( playloadbuf, 0, len);
		free_playloadbuf( &self->ctl.pingpong);
//		LED_com->turnon(LED_com);
//		if( T485Rxirq_cb != NULL && len)
//			T485Rxirq_cb->cb( NULL,  T485Rxirq_cb->arg);
		self->ctl.recv_size = 0;
		return len;
	}
	
	return 0;
}

//�����ڵĻ����������
//���ص�ǰ��������ݳ���
static int UartTakeUpPlayloadBuf( driveUart *self, void **data)
{
	int  ret = 1;
	int len = 0;
	char *playloadbuf ;

	if( self->rxWait)
	{
		
		ret = self->rxWait( self, self->ctl.rx_waittime_ms);
	}
	
	if( ret > 0)
	{
		
		len = self->ctl.recv_size;
		playloadbuf = get_playloadbuf( &self->ctl.pingpong);
		self->ctl.recv_size = 0;
		*data = playloadbuf;
		return len;
	}
	
	return 0;
}

int UartGiveBackPlayloadBuf( driveUart *self, void *data)
{
	free_playloadbuf( &self->ctl.pingpong);
	return 0;
}


/*!
**
**
** @param size
**
** @return
**/
static int  UartIoctol( driveUart *self, int cmd, ...)
{
	int int_data;
	va_list arg_ptr; 
	va_start(arg_ptr, cmd); 
	
	switch(cmd)
	{
		case DRICMD_DISABLE_TXBLOCK:
			self->ctl.tx_block = 0;
			break;
		case DRICMD_ENABLE_TXBLOCK:
			self->ctl.tx_block = 1;
			break;
		case DRICMD_DISABLE_RXBLOCK:
			self->ctl.rx_block = 0;
			self->ctl.rx_waittime_ms = 0;
			break;
		case DRICMD_ENABLE_RXBLOCK:
			self->ctl.rx_block = 1;
			
			break;
		case DRICMD_SET_TXWAITTIME_MS:
			int_data = va_arg(arg_ptr, int);
			va_end(arg_ptr); 
			self->ctl.tx_waittime_ms = int_data;
			break;
		case DRICMD_SET_RXWAITTIME_MS:
			int_data = va_arg(arg_ptr, int);
			va_end(arg_ptr); 
			self->ctl.rx_waittime_ms = int_data;
			break;
		case DRICMD_SET_DIR_RX:
			if((self->dirPin) && (self->dirPin->Port))
			{
				GPIO_ResetBits( self->dirPin->Port, self->dirPin->pin);
				
			}
			break;
		case DRICMD_SET_DIR_TX:
			if((self->dirPin) && (self->dirPin->Port))
			{
				GPIO_SetBits( self->dirPin->Port, self->dirPin->pin);
				
			}
			break;
			

		
		
		default: break;
		
	}
	
	return RET_OK;
}




static int UartTest( driveUart *self, void *buf, int size)
{
	char *pp = NULL;
//	char	*pdata = NULL;
  int len;
	
	strcpy( buf, "Serial 485 test\n" );
	self->ioctol( self, DRICMD_SET_TXWAITTIME_MS, 10);
	self->ioctol( self, DRICMD_SET_RXWAITTIME_MS, 1000);
	
	
	self->write( self, buf, strlen(buf));
	while(1)
	{
		
	
		len = self->read( self, buf, size);
		pp = strstr((const char*)buf,"OK");
		if(pp)
			return ERR_OK;
		
		if( len > 0)
		{
			self->write( self, buf, len);
		}
		else 
		{
			strcpy( buf, "Serial 485 test\n" );
			self->write( self, buf, strlen(buf));
		}
		delay_ms(100);
//		len = self->takeUpPlayloadBuf( self, ( void *)&pdata);
//		pp = strstr((const char*)pdata,"OK");
//		if(pp)
//			return ERR_OK;
//		
//		if( len > 0)
//		{
//			self->write( self, pdata, len);
//		}
//		
//		self->giveBackPlayloadBuf( self, ( void *)pdata);
		
		
		
	}
	
}


static void UartSetLedHdl( driveUart *self, int rxOrTx, ledHdl hdl)
{
	if( rxOrTx == DRCT_RX)
	{
		self->rxLedHdl = hdl;
	}
	else if( rxOrTx == DRCT_TX)
	{
		self->txLedHdl = hdl;
	} 
	
	
}

static void UartSetIdp( driveUart *self, int rxOrTx, uartIdp idp, void *arg)
{
	if( rxOrTx == DRCT_RX)
	{
		self->rxIdp = idp;
		self->argRxIdp = arg;
	}
	else if( rxOrTx == DRCT_TX)
	{
		self->txIdp = idp;
		self->argTxIdp = arg;
	} 
	
	
}
static void UartSetWaitSem( driveUart *self, int rxOrTx, waitSem wait)
{
	if( rxOrTx == DRCT_RX)
	{
		self->rxWait = wait;
	}
	else if( rxOrTx == DRCT_TX)
	{
		self->txWait = wait;
	} 
	
	
}
static void UartSetPostSem( driveUart *self, int rxOrTx, postSem post)
{
	if( rxOrTx == DRCT_RX)
	{
		self->rxPost = post;
	}
	else if( rxOrTx == DRCT_TX)
	{
		self->txPost = post;
	} 
	
}

/*! gprs uart dma Configuration*/
void UartDma_Init( driveUart *self)
{
	CfgUart_t *myCfg = ( CfgUart_t *)self->cfg;
	USART_TypeDef *devUartBase = ( USART_TypeDef *)self->devUartBase;
	
	DMA_InitTypeDef DMA_InitStructure;	
	short rxbuflen;
	char *rxbuf;

    
//=DMA_Configuration==============================================================================//	
/*--- UART_Tx_DMA_Channel DMA Config ---*/

    DMA_Cmd( myCfg->dma->dma_tx_base, DISABLE);                           // �ر�DMA
    DMA_DeInit( myCfg->dma->dma_tx_base);                                 // �ָ���ʼֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&devUartBase->DR);// �����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)self->txCache;        
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                      // ���ڴ浽����
    DMA_InitStructure.DMA_BufferSize = 0;                 	//UART_TXCACHE_SIZE   
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // �����ַ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // �ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // �������ݿ��1B
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // �ڴ��ַ���1B
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // ���δ���ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;                 // ���ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            // �ر��ڴ浽�ڴ�ģʽ
    DMA_Init( myCfg->dma->dma_tx_base, &DMA_InitStructure);               // 
    
	DMA_ClearFlag( myCfg->dma->dma_tx_flag );                                 // �����־
	DMA_Cmd( myCfg->dma->dma_tx_base, DISABLE); 												// �ر�DMA
    DMA_ITConfig( myCfg->dma->dma_tx_base, DMA_IT_TC, ENABLE);            // ����������ж�

   

/*--- UART_Rx_DMA_Channel DMA Config ---*/

 
	switch_receivebuf( &self->ctl.pingpong, &rxbuf, &rxbuflen);
    DMA_Cmd( myCfg->dma->dma_rx_base, DISABLE);                           
    DMA_DeInit( myCfg->dma->dma_rx_base);                                 
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&devUartBase->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)rxbuf;         
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                     
    DMA_InitStructure.DMA_BufferSize = rxbuflen;                     
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;                 
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            
    DMA_Init( myCfg->dma->dma_rx_base, &DMA_InitStructure);               
    DMA_ClearFlag( myCfg->dma->dma_rx_flag);      
//	DMA_ITConfig(DMA_s485_usart.dma_rx_base, DMA_IT_TC, ENABLE); 	 // ����������ж�

    DMA_Cmd( myCfg->dma->dma_rx_base, ENABLE);                            

   

}
static void DRI_Uart_clean_idle(USART_TypeDef* USARTx)
{
	uint8_t 	clear_idle = clear_idle;
	clear_idle = USARTx->SR;
	clear_idle = USARTx->DR;
	USART_ReceiveData( USARTx); // Clear IDLE interrupt flag bit
	
}
void Usart_irq( driveUart *thisDev)
{
	short rxbuflen;
	char *rxbuf;
	USART_TypeDef *devUartBase = ( USART_TypeDef *)thisDev->devUartBase;
	//���500����115200�Ĳ����ʣ�CPUʱ����48M�������ͨ����αƽ���õ�
//	int i = 500;
	
	CfgUart_t 	*myCfg = ( CfgUart_t *)thisDev->cfg;
//	uint8_t 	clear_idle = clear_idle;
//	uint16_t	cntdr = 0;
	
	if(USART_GetITStatus( thisDev->devUartBase, USART_IT_IDLE) != RESET)  // �����ж�
	{
		
		DMA_Cmd( myCfg->dma->dma_rx_base, DISABLE);       // �ر�DMA
		DMA_ClearFlag(  myCfg->dma->dma_rx_flag );           // ���DMA��־
		thisDev->ctl.recv_size = get_loadbuflen( &thisDev->ctl.pingpong)  - \
		DMA_GetCurrDataCounter(  myCfg->dma->dma_rx_base); //��ý��յ����ֽ�
		
		switch_receivebuf( &thisDev->ctl.pingpong, &rxbuf, &rxbuflen);
		myCfg->dma->dma_rx_base->CMAR = (uint32_t)rxbuf;
		myCfg->dma->dma_rx_base->CNDTR = rxbuflen;
		DMA_Cmd(  myCfg->dma->dma_rx_base, ENABLE);
		
	
//		clear_idle = devUartBase->SR;
//		clear_idle = devUartBase->DR;
//		USART_ReceiveData( devUartBase); // Clear IDLE interrupt flag bit

		DRI_Uart_clean_idle(devUartBase);

		if( thisDev->rxLedHdl)
			thisDev->rxLedHdl(thisDev);
		if( thisDev->rxPost)
			thisDev->rxPost(thisDev);
		if( thisDev->rxIdp)
			thisDev->rxIdp( thisDev->argRxIdp);
		
	}
	if(USART_GetITStatus( thisDev->devUartBase, USART_IT_RXNE) == SET) 
	{
		//���RXNE��־����ֹ�ڽ��յ����ݳ��ȳ���dma�Ľ��ճ���֮��û���κ�Ӧ��ȥ�������ݶ���ɴ�����Զ�����ж�״̬
		USART_ReceiveData( thisDev->devUartBase);
	}
	//����ж���DMA��������ж��п���
	if(USART_GetITStatus( thisDev->devUartBase, USART_IT_TC) == SET) 
	{
		
			
		USART_ClearFlag( thisDev->devUartBase,USART_IT_TC );
		USART_ITConfig( thisDev->devUartBase, USART_IT_TC, DISABLE);
		
		thisDev->status = 0;
//		if(myCfg->dma->dma_tx_base->CNDTR > 0) {
//			cntdr = myCfg->dma->dma_tx_base->CNDTR;
//			return;
//		}
		if( thisDev->txPost)
			thisDev->txPost(thisDev);
		if( thisDev->txIdp)
			thisDev->txIdp( thisDev->argTxIdp);
		
		thisDev->ioctol( thisDev, DRICMD_SET_DIR_RX);
		thisDev->status = UARTSTATUS_IDLE;
		
	}
	if(myCfg->opt_mode == UART_MODE_INTR)
	{
		if(USART_GetITStatus( thisDev->devUartBase, USART_IT_TXE) == SET)  // �����ж�
		{
			

			thisDev->ctl.sendingCount ++;
				
		
			//������ɺ�رշ�չ�жϣ����ⷢ�ͼĴ������˾ͻ�����ж�
			if( thisDev->ctl.sendingCount >= thisDev->ctl.sendingLen)
			{
				USART_ITConfig( thisDev->devUartBase, USART_IT_TXE, DISABLE);
				if( thisDev->txPost)
					thisDev->txPost(thisDev);
			}			
			else
				USART_SendData( thisDev->devUartBase, thisDev->ctl.intrSendingBuf[thisDev->ctl.sendingCount] );
			
		
			
		}
	}
	

	
}

void USART1_IRQHandler(void)
{
	
	driveUart	*thisDev = devArry[0];
	
	Usart_irq( thisDev);
}

void UART4_IRQHandler(void)
{
	
	driveUart	*thisDev = devArry[3];
	
	Usart_irq( thisDev);
}




//�жϴ�����������Դ�봮�ڵİ������ѡ���豸
void DMA1_Channel4_IRQHandler(void)
{
	driveUart	*thisDev = devArry[0];
	CfgUart_t *myCfg = ( CfgUart_t *)thisDev->cfg;
//	int i = 1000;
	if(DMA_GetITStatus(DMA1_FLAG_TC4))		//2�Ƿ����ж�
	{
		
		DMA_ClearFlag( myCfg->dma->dma_tx_flag);         // �����־
		DMA_Cmd( myCfg->dma->dma_tx_base, DISABLE);   // �ر�DMAͨ��
		
		//�ȵ�Ӳ�������еļĴ��������봮��
//		while(!USART_GetFlagStatus( thisDev->devUartBase, USART_FLAG_TXE));
		
		//��������δ��ɷ���
//		if( USART_GetFlagStatus( thisDev->devUartBase, USART_FLAG_TXE))
//		{
			USART_ClearFlag( thisDev->devUartBase,USART_IT_TC );
			USART_ITConfig( thisDev->devUartBase, USART_IT_TC, ENABLE);
//		}
//		else
//		{
//			
//			thisDev->ioctol( thisDev, DRICMD_SET_DIR_RX);
//		}
		
//		if( thisDev->txPost)
//			thisDev->txPost(thisDev);
//		if( thisDev->txIdp)
//			thisDev->txIdp( thisDev->argTxIdp);
//		thisDev->status = UARTSTATUS_IDLE;
//		while(i)
//			i --;
//		thisDev->ioctol( thisDev, DRICMD_SET_DIR_RX);
		
	
	}
}


void USART2_IRQHandler(void)
{
	
	driveUart	*thisDev = devArry[1];
	Usart_irq( thisDev);


}

void USART3_IRQHandler(void)
{
	
	driveUart	*thisDev = devArry[2];
	
	Usart_irq( thisDev);
}


//�жϴ�����������Դ�봮�ڵİ������ѡ���豸
void DMA1_Channel7_IRQHandler(void)
{
	driveUart	*thisDev = devArry[1];
	CfgUart_t *myCfg = ( CfgUart_t *)thisDev->cfg;
	if(DMA_GetITStatus(DMA1_FLAG_TC7))
	{

		DMA_ClearFlag( myCfg->dma->dma_tx_flag);         // �����־
		DMA_Cmd( myCfg->dma->dma_tx_base, DISABLE);   // �ر�DMAͨ��
		
		USART_ClearFlag( thisDev->devUartBase,USART_IT_TC );
		USART_ITConfig( thisDev->devUartBase, USART_IT_TC, ENABLE);
		
//		while( USART_GetFlagStatus( thisDev->devUartBase, USART_FLAG_TXE) == RESET){};
//		if( thisDev->txPost)
//			thisDev->txPost(thisDev);

//		if( thisDev->txIdp)
//			thisDev->txIdp( thisDev->argTxIdp);
//		thisDev->status = UARTSTATUS_IDLE;
//		thisDev->ioctol( thisDev, DRICMD_SET_DIR_RX);
		
	}
}

//?D??'|��3�D�?�?Y��?'�?'??��?��?�?�??�'????��?
void DMA1_Channel2_IRQHandler(void)
{
	driveUart	*thisDev = devArry[2];
	CfgUart_t *myCfg = ( CfgUart_t *)thisDev->cfg;
	if(DMA_GetITStatus(DMA1_FLAG_TC2))
	{

		DMA_ClearFlag( myCfg->dma->dma_tx_flag);         // ??3y��??
		DMA_Cmd( myCfg->dma->dma_tx_base, DISABLE);   // 1?�?DMA��
		
		USART_ClearFlag( thisDev->devUartBase,USART_IT_TC );
		USART_ITConfig( thisDev->devUartBase, USART_IT_TC, ENABLE);
		
//		while( USART_GetFlagStatus( thisDev->devUartBase, USART_FLAG_TXE) == RESET){};
//		if( thisDev->txPost)
//			thisDev->txPost(thisDev);

//		if( thisDev->txIdp)
//			thisDev->txIdp( thisDev->argTxIdp);
//		thisDev->status = UARTSTATUS_IDLE;
//		thisDev->ioctol( thisDev, DRICMD_SET_DIR_RX);
		
	}
}

void DMA2_Channel4_5_IRQHandler(void)
{
	driveUart	*thisDev = devArry[3];
	CfgUart_t *myCfg = ( CfgUart_t *)thisDev->cfg;
	if(DMA_GetITStatus(DMA2_FLAG_TC5))
	{

		DMA_ClearFlag( myCfg->dma->dma_tx_flag);         // ??3y��??
		DMA_Cmd( myCfg->dma->dma_tx_base, DISABLE);   // 1?�?DMA��
		
		USART_ClearFlag( thisDev->devUartBase,USART_IT_TC );
		USART_ITConfig( thisDev->devUartBase, USART_IT_TC, ENABLE);

		
	}
}

CTOR( driveUart)
FUNCTION_SETTING( init, UartInit);
FUNCTION_SETTING( deInit, UartDeInit);
FUNCTION_SETTING( read, UartRead);
FUNCTION_SETTING( takeUpPlayloadBuf, UartTakeUpPlayloadBuf);
FUNCTION_SETTING( giveBackPlayloadBuf, UartGiveBackPlayloadBuf);
FUNCTION_SETTING( write, UartWrite);
FUNCTION_SETTING( ioctol, UartIoctol);
FUNCTION_SETTING( setLedHdl, UartSetLedHdl);
FUNCTION_SETTING( setIdp, UartSetIdp);
FUNCTION_SETTING( setWaitSem, UartSetWaitSem);
FUNCTION_SETTING( setPostSem, UartSetPostSem);
FUNCTION_SETTING( test, UartTest);
END_CTOR

