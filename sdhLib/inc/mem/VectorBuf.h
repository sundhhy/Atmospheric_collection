#ifndef __BUFMANAGER_H_
#define __BUFMANAGER_H_
#include <stdint.h>

//��������
//ÿһ֡���ݵ�ͷ������֡���ݵĳ���
//��ѡ���ڻ��治��ʱ���Ƕ��������ݻ��Ƕ���δ��ȡ������
#define DROP_OLDDATA	0xcd
#define DROP_NEWDATA	0x3a

#define IS_DROPNEWDATA( p_vbm)  ( p_vbm->drop == DROP_NEWDATA)
#define IS_DROPOLDDATA( p_vbm)  ( p_vbm->drop == DROP_OLDDATA)

#define VBM_FRAMEHEAD_LEN		( sizeof( frameHead_t))
#define VBM_FILLBYTE_LEN( rawLen)		\
( ( VBM_FRAMEHEAD_LEN - ( ( rawLen) & ( VBM_FRAMEHEAD_LEN - 1))) & ( VBM_FRAMEHEAD_LEN - 1) )
	
#define VBM_ADD_WRINDEX( p_vbm, num) { \
	p_vbm->wrIndex += num;	\
	p_vbm->wrIndex &= ( p_vbm->totalLen - 1);\
}
#define VBM_ADD_RDINDEX( p_vbm, num) { \
	p_vbm->rdIndex += num;	\
	p_vbm->rdIndex &= ( p_vbm->totalLen - 1);\
}

typedef struct
{
	uint16_t	frameLen;
}frameHead_t;
	

typedef struct
{
	
	uint16_t	rdIndex;
	uint16_t	wrIndex;
	uint16_t	freeLen;
	uint16_t	totalLen;	//�ڴ�ĳ��ȱ�����2����
	int			drop;
	char*		buf;
}vectorBufManager_t;

//�ڴ�ĳ��ȱ�����2����
int VecBuf_Init( vectorBufManager_t* p_vbm, char* buf, uint16_t len, int drop);
//����д������ݳ���
int VecBuf_write( vectorBufManager_t* p_vbm, char* data, uint16_t len); 
//���ض�ȡ�����ݳ���
int VecBuf_read( vectorBufManager_t* p_vbm, char* buf, uint16_t bufsize);
#endif
