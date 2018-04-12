/*
 * cycQueue.h
 *
 *  Created on: 2016-12-16
 *      Author: Administrator
 */

#ifndef INC_usb_queue_H_
#define INC_usb_queue_H_
#include <stdint.h>


#define CQ_LENALL		0xffff

typedef struct {
	uint8_t		*buf;
	uint32_t	size;		//������2����
	uint16_t	read;		//��ǰ�Ķ�λ��
	uint16_t	write;		//��ǰ��дλ��	
}CycQueus_t;

#define CPU_IRQ_OFF	
#define CPU_IRQ_ON



int	CQ_Init( CycQueus_t *cq, uint8_t *buf, int size);
int	CQ_Clean( CycQueus_t *cq);
int	CQ_Len( CycQueus_t *cq);

int	CQ_Write( CycQueus_t *cq, uint8_t *data, int len);
int	CQ_Read( CycQueus_t *cq, uint8_t *data, int len);
int	CQ_GetPtr( CycQueus_t *cq, uint8_t **ptr, int len);
#endif /* CYCQUEUE_H_ */
