/**
* @file 		Ping_PongBuf.c
* @brief		ʵ��˫RAM
* @details		1. ��ping��pong��û������ʹ�õ�ʱ������ʹ��ping�� 
*				2. ��ping��pong����װ��������ʱ�����ݶ�û��Ӧ�ó���ȡ�ߵ�ʱ�򣬰�pong�Ļ������ݷ�����
* @author		author
* @date		date
* @version	A001
* @par Copyright (c): 
* 		XXX??
* @par History:         
*	version: author, date, desc\n
*/
#include "mem/Ping_PongBuf.h"
#include <string.h>
void  init_pingponfbuf( PPBuf_t *ppbuf, char *buf, int len , int sw)
{
	ppbuf->sw = sw;
	if( sw)
	{
		
		ppbuf->ping_buf = buf;
		ppbuf->buf_len = len/2;
		ppbuf->pong_buf = buf + ppbuf->buf_len;
		
		ppbuf->idleBuf[ 0] = ppbuf->ping_buf;
		ppbuf->idleBuf[ 1] = ppbuf->pong_buf;
		
		//����ʹ��ping_buf
		//��ʼ����ʱ�������buf����Ϊping_buf����Ϊ���ڵ�һ���л��ڴ��ʱ��
		//��dataBuf ��loadbufһ��
		ppbuf->loadBuf = ppbuf->ping_buf;
		ppbuf->dataBuf = NULL;	
	}
	else
	{
		ppbuf->ping_buf = buf;
		ppbuf->buf_len = len;
		ppbuf->pong_buf = NULL;
		
		ppbuf->idleBuf[ 0] = NULL;
		ppbuf->idleBuf[ 1] = NULL;
		ppbuf->loadBuf = ppbuf->ping_buf;
		ppbuf->dataBuf = ppbuf->ping_buf;	
		
	}
		
	
//	ppbuf->ping_status = PPBUF_STATUS_IDLE;
//	ppbuf->pong_status = PPBUF_STATUS_IDLE;
//	ppbuf->loading_buf = BUF_NONE;
//	ppbuf->playload_buf = BUF_NONE;
}
//
void switch_receivebuf( PPBuf_t *ppbuf, char **buf, short *len)
{
	int i = 0;
	if( ppbuf->sw == 0)
	{
		*buf = ppbuf->loadBuf;
		*len = ppbuf->buf_len;
		return ;
	}
	
	for( i = 0; i < NUM_PINGPONGBUF; i++)
	{
		if( ppbuf->idleBuf[ i])
		{
			
			ppbuf->dataBuf = ppbuf->loadBuf;
			ppbuf->loadBuf = ppbuf->idleBuf[ i];
			ppbuf->idleBuf[ i] = NULL;
			break;
		}
		
	}
	
	*buf = ppbuf->loadBuf;
	*len = ppbuf->buf_len;
	
}



char *get_playloadbuf( PPBuf_t *ppbuf)
{
	

	return ppbuf->dataBuf;
//	if( ppbuf->playload_buf == BUF_PING)
//	{
//		ppbuf->ping_status = PPBUF_STATUS_IDLE;
//		return ppbuf->ping_buf;
//		
//	}
//	else if( ppbuf->playload_buf == BUF_PONG)
//	{
//		ppbuf->pong_status = PPBUF_STATUS_IDLE;
//		return ppbuf->pong_buf;
//		
//	}
//	else
//	{
//		
//		return ppbuf->ping_buf;
//	}
}
void free_playloadbuf( PPBuf_t *ppbuf)
{
	int i = 0;
	
	
	if( ppbuf->dataBuf )
		memset( ppbuf->dataBuf, 0, ppbuf->buf_len);
	
	if( ppbuf->sw == 0)
	{
		
		return ;
	}
	
	
	for( i = 0; i < NUM_PINGPONGBUF; i++)
	{
		if( ppbuf->idleBuf[ i] == ppbuf->dataBuf)
		{	
			break;
		}
		if( ppbuf->idleBuf[ i] == NULL)
		{
			ppbuf->idleBuf[ i] = ppbuf->dataBuf;
			break;
		}
		
	}
	ppbuf->dataBuf = ppbuf->loadBuf;

//	if( ppbuf->playload_buf == BUF_PING)
//	{
//		memset( ppbuf->ping_buf, 0, ppbuf->ping_len);
//	}
//	else if( ppbuf->playload_buf == BUF_PONG)
//	{
//		memset( ppbuf->pong_buf, 0, ppbuf->pong_len);
//		
//	}
//	ppbuf->playload_buf = BUF_NONE;
	
}
//��ping��pong��װ�ر�־��Ϊ1��ʱ��
//ping���������δ����ȡ
//pong������Ϊ���ջ���һֱ�ڱ��������ݸ���
int get_loadbuflen( PPBuf_t *ppbuf)
{
//	if( ppbuf->playload_buf == BUF_PING)
//	{
		return ppbuf->buf_len;
//	}
//	else
//	{
//		return ppbuf->pong_len;
//	}
	
}
