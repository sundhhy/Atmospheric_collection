/*
 * cycQueue.c
 *
 *  Created on: 2016-12-16
 *      Author: Administrator
 */
#include "usb_queue.h"
//#include "basis/sdhDebug.h"
//#include "basis/sdhError.h"
//#include "sdhDef.h"
#include <string.h>

int	CQ_Init (CycQueus_t *cq, uint8_t *buf, int size)
{
	cq->buf = buf;
	cq->size = size;
	return 0;
}
int	CQ_Len( CycQueus_t *cq)
{
	return ( ( cq->write - cq->read) & ( cq->size - 1));
	
}
int	CQ_Clean( CycQueus_t *cq)
{
	cq->read = 0;
	cq->write = 0;
	memset( (void *)cq->buf, 0, cq->size);
	return 0;
}

//����ʵ��д��ĳ���
//-1 ʧ��
int	CQ_Write( CycQueus_t *cq, uint8_t *data, int len)
{
	int i = 0;
	if( CQ_Len( cq) == ( cq->size - 1))	return -1;
	
	for( i = 0; i < len; i++)
	{
		if( CQ_Len( cq) == ( cq->size - 1))	
			break;
		
		cq->buf[ cq->write] = data[ i];
		CPU_IRQ_OFF;
		cq->write = ( cq->write + 1) & ( cq->size - 1);  //������ԭ�ӵ�
		CPU_IRQ_ON;
		
	}
	
	return i;
	
}

//����ʵ�ʶ�ȡ������
int	CQ_Read( CycQueus_t *cq, uint8_t *data, int len)
{
	int i = 0;
	for( i = 0; i < len; i++)
	{
		if( CQ_Len( cq) == 0)	
			break;
		data[ i] = cq->buf[ cq->read];
		CPU_IRQ_OFF;
		cq->read = ( cq->read + 1) & ( cq->size - 1);  //������ԭ�ӵ�
		CPU_IRQ_ON;
	}
	
	return i;
	
}

int	CQ_GetPtr( CycQueus_t *cq, uint8_t **ptr, int len)
{
	int cqlen = CQ_Len( cq);
	if( cqlen < len)
		len = cqlen;
	*ptr = cq->buf + cq->read;
	CPU_IRQ_OFF;
	cq->read = ( cq->read + len) & ( cq->size - 1);  //������ԭ�ӵ�
	CPU_IRQ_ON;
	return len;
}
