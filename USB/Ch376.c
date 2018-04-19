//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------

//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//
#include <string.h>		//NULL 
#include "Ch376.h"
#include "usb_hardware_interface.h"

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------




//#define xWriteCH376Data(d)		(ch376_dev->write(ch376_dev, &d, 1))
//#define xWriteCH376Data_p(ptr, n)		(ch376_dev->write(ch376_dev, ptr, n))
//#define xReadCH376Data(d)		(ch376_dev->read(ch376_dev, &d, 1))
//#define xReadCH376Data_p(ptr, n)		(ch376_dev->read(ch376_dev, ptr, n))
//#define	xEndCH376Cmd( )			p_ch376->usb_cs_off()//����CH376����,������SPI�ӿڷ�ʽ
//#define CH376_CMD_PORT	(ch376_dev->write(ch376_dev, &d, 1))

#define CH376_DATABUF_SIZE			64
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static usb_op_t	*p_ch376;
static uint8_t DataBuff[CH376_DATABUF_SIZE];
static uplevel_intr ch376_up_irq = NULL;

#define xWriteCH376Data(d)		(p_ch376->usb_write_bytes(&d, 1))
#define xWriteCH376Data_p(ptr, n)		(p_ch376->usb_write_bytes(ptr, n))
#define xReadCH376Data(d)		(p_ch376->usb_read_bytes(&d, 1))
#define xReadCH376Data_p(ptr, n)		(p_ch376->usb_read_bytes(ptr, n))
#define	xEndCH376Cmd( )			p_ch376->usb_cs_off()//����CH376����,������SPI�ӿڷ�ʽ
#define	xStartCH376Cmd( )			p_ch376->usb_cs_on()//��ʼCH376����,������SPI�ӿڷ�ʽ
#define CH376_CMD_PORT	(p_ch376->usb_write_bytes(&d, 1))

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------

//static void p_ch376->usb_delay_ms(unsigned long time);
//static void Delay10us(unsigned long time);
static void xWriteCH376Data_u32(uint32_t data_u32);
static void xWriteCH376Cmd(uint8_t mCmd);
static uint8_t CH376GetIntStatus(void);
static uint8_t	CH376SendCmdWaitInt( uint8_t mCmd );
static uint8_t Wait376Interrupt(int set_irq )  ;
static uint8_t	CH376ReadVar8( uint8_t var );  /* ��CH376оƬ�ڲ���8λ���� */
static void	CH376WriteHostBlock( uint8_t *buf, uint8_t len );
static uint8_t	CH376DiskReqSense( void );  
static void	CH376SetFileName(char *name );
static uint8_t	CH376SendCmdDatWaitInt( uint8_t mCmd, uint8_t mDat )  ;
static void	CH376WriteVar32(uint8_t var, uint32_t dat );  /* дCH376оƬ�ڲ���32λ���� */
static uint32_t	CH376Read32bitDat( void );
static  void Ch376_intr(void);
static uint8_t	CH376DiskWriteSec( uint8_t *buf, uint32_t iLbaStart, uint8_t iSectorCount );
static  uint8_t	CH376WriteReqBlock( uint8_t *buf );
static void	CH376WriteOfsBlock(uint8_t * buf, uint8_t ofs, uint8_t len );
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
int	Init_Ch376(void *op, uplevel_intr up_irq)
{
	int ret = 0;
	
	p_ch376 = (usb_op_t *)op;
	p_ch376->usb_irq_cb = Ch376_intr;
	p_ch376->usb_reset();
//	ret = Dev_open(DEVID_GPIO_USB_INIT, (void *)&ch376_int);
	p_ch376->usb_set_irq(0);
//	
//	ret = Dev_open(dev_id, (void *)&ch376_dev);
//	
//	ch376_int->ioctol(ch376_int, DEVCMD_SET_IRQHDL, Ch376_intr, (void *)NULL);
//	HRst_Ch376();
	ch376_up_irq = up_irq;
 
	ret = mInitCH376Host();
	
	p_ch376->usb_set_irq(1);
	
	if( CH376GetIntStatus() == USB_INT_CONNECT)
	{
		
		ch376_up_irq(USB_INT_CONNECT);
	}
	return ret;
	
}

//void Power_Ch376(int on)
//{
//	if(on) 
//	{
//		SET_CH376PWR_LOW;
//	}
//	else 
//	{
//		
//		
//		SET_CH376PWR_HIGH;
//	}
//	
//}

//void p_ch376->usb_set_irq(int ed)
//{
//	if(ed) 
//	{
//		ch376_int->ioctol(ch376_int,DEVCMD_ENABLE_IRQ);
//		
//	}
//	else 
//	{
//		
//		ch376_int->ioctol(ch376_int,DEVCMD_DISABLE_IRQ);
//	}
//	
//	
//}

//void HRst_Ch376(void)
//{
//	SET_CH376RST_HIGH;
//	p_ch376->usb_delay_ms(100);
//	
//	SET_CH376RST_LOW;
//	p_ch376->usb_delay_ms(100);
//	
//	p_ch376->usb_cs_off();
//	p_ch376->usb_delay_ms(100);
//	
////	
//	
////	uint8_t	s = 0;
////	

////	p_ch376->usb_cs_off();
////	s = CH376SendCmdWaitInt(CMD00_RESET_ALL);
////	xEndCH376Cmd();
////	p_ch376->usb_delay_ms(50);			//ͨ��35ms�����

//}

//��ʼ��CH376
uint8_t mInitCH376Host(void)
{
	uint8_t res = 0;
	uint8_t	usb_data;

	//���ͨѶ�ӿ�
	
	xWriteCH376Cmd(CMD11_CHECK_EXIST);
	p_ch376->usb_delay_ms(100);
	usb_data = 0x65;
	xWriteCH376Data(usb_data);
	p_ch376->usb_delay_ms(100);
	xReadCH376Data(res);
	p_ch376->usb_cs_off();
	if (res != 0x9A)
	{
		return (ERR_USB_UNKNOWN);
	}


	//��ȡоƬ���̼��汾
	xWriteCH376Cmd(CMD01_GET_IC_VER);
	p_ch376->usb_delay_ms(1000);
	xReadCH376Data(res);
	p_ch376->usb_cs_off();
	if (res != 0x41)
	{
//		return (ERR_USB_UNKNOWN);
		res = 0x41;
	}


	//����USB����ģʽ
	xWriteCH376Cmd(CMD11_SET_USB_MODE);
	usb_data = 0x06;
	xWriteCH376Data(usb_data);	//�����õ�������ʽ�����Զ�����SOF��
	p_ch376->usb_delay_ms(100);
	xReadCH376Data(res);
	p_ch376->usb_cs_off();
	if (res == CMD_RET_SUCCESS)
	{
		return USB_INT_SUCCESS;
	}
	else
	{
		return ERR_USB_UNKNOWN;
	}
}

// ���U���Ƿ�����,��֧��SD�� 
uint8_t Ch376DiskConnect(void)
{
	
	CH376GetIntStatus( );  // ����ж�
	return( CH376SendCmdWaitInt( CMD0H_DISK_CONNECT ) );
	
}

// ��ʼ�����̲����Դ����Ƿ����
uint8_t CH376DiskMount( void ) 
{
	return( CH376SendCmdWaitInt( CMD0H_DISK_MOUNT ) );
}


// �ӵ�ǰ�����˵�Ľ��ջ�������ȡ���ݿ�,���س��� 
uint8_t	CH376ReadBlock( uint8_t *buf, int buf_size  )
{
	uint8_t	s, l;

	xWriteCH376Cmd( CMD01_RD_USB_DATA0 );
	xReadCH376Data(l);  /* ���� */
//	xReadCH376Data(s); /* ���� */
	
	if ( l ) {
		if(l > buf_size)
			l = buf_size;
		s = xReadCH376Data_p(buf, l);
		
	}
	xEndCH376Cmd( );
	return( s );
}

// ���U���Ƿ�д����, ����USB_INT_SUCCESS˵������д,����0xFF˵��ֻ��/д����,��������ֵ˵���Ǵ������ 
uint8_t	IsDiskWriteProtect( void )
{
	uint8_t	s, SysBaseInfo;
	P_BULK_ONLY_CBW	pCbw;

	SysBaseInfo = CH376ReadVar8( VAR_SYS_BASE_INFO );  // ��ǰϵͳ�Ļ�����Ϣ 
	pCbw = (P_BULK_ONLY_CBW)DataBuff;
	for ( s = 0; s != sizeof( pCbw -> CBW_CB_Buf ); s ++ )
		pCbw -> CBW_CB_Buf[ s ] = 0;  // Ĭ����0 
	pCbw -> CBW_DataLen0 = 0x10;  // ���ݴ��䳤�� 
	pCbw -> CBW_Flag = 0x80;  // ���䷽��Ϊ���� 
	if ( SysBaseInfo & 0x40 )
	{  
		// SubClass-Code������6 
		pCbw -> CBW_CB_Len = 10;  // �����ĳ��� 
		pCbw -> CBW_CB_Buf[0] = 0x5A;  // ��������ֽ�, MODE SENSE(10) 
		pCbw -> CBW_CB_Buf[2] = 0x3F;
		pCbw -> CBW_CB_Buf[8] = 0x10;
	}
	else
	{ 
		// SubClass-Code�����Ϊ6 
		pCbw -> CBW_CB_Len = 6;  // �����ĳ��� 
		pCbw -> CBW_CB_Buf[0] = 0x1A;  // ��������ֽ�, MODE SENSE(6) 
		pCbw -> CBW_CB_Buf[2] = 0x3F;
		pCbw -> CBW_CB_Buf[4] = 0x10;
	}

	// ��USB�����˵�ķ��ͻ�����д�����ݿ�,ʣ�ಿ��CH376�Զ�� 
	CH376WriteHostBlock( (uint8_t *)pCbw, sizeof( BULK_ONLY_CBW ) );
	// ��U��ִ��BulkOnly����Э�� 
	s = CH376SendCmdWaitInt( CMD0H_DISK_BOC_CMD );  
	if ( s == USB_INT_SUCCESS )
	{
		// �ӵ�ǰ�����˵�Ľ��ջ�������ȡ���ݿ�,���س��� 
		s = CH376ReadBlock( DataBuff, CH376_DATABUF_SIZE );  
		if ( s > 3 )
		{
			// MODE SENSE��������ݵĳ�����Ч 
			if ( SysBaseInfo & 0x40 )
				s = DataBuff[3];  // MODE SENSE(10), device specific parameter 
			else
				s = DataBuff[2];  // MODE SENSE(6), device specific parameter 
			if ( s & 0x80 )
				return( 0xFF );  // U��д���� 
			else
				return( USB_INT_SUCCESS );  // U��û��д���� 
		}
		return( USB_INT_DISK_ERR );
	}

	CH376DiskReqSense( );  // ���USB�洢������ 
	return( s );
}


// ��ѯ����ʣ��ռ���Ϣ,������ 
uint8_t	CH376DiskQuery(uint32_t *DiskAll, uint32_t *DiskFre, uint8_t *diskFat  )  
{
	uint8_t	s,temp;
	uint8_t	c0, c1, c2, c3;

	s = CH376SendCmdWaitInt( CMD0H_DISK_QUERY );
	if ( s == USB_INT_SUCCESS )
	{
		// �ο�CH376INC.H�ļ���CH376_CMD_DATA�ṹ��DiskQuery 
		xWriteCH376Cmd( CMD01_RD_USB_DATA0 );
		xReadCH376Data(c0 );  // ��������sizeof(CH376_CMD_DATA.DiskQuery) 
		xReadCH376Data(c1 );  // CH376_CMD_DATA.DiskQuery.mTotalSector 
		xReadCH376Data(c2 );
		xReadCH376Data(c3 );
		*DiskAll = c0 | (uint16_t)c1 << 8 | (uint32_t)c2 << 16 | (uint32_t)c3 << 24;
		xReadCH376Data(c0 );  // CH376_CMD_DATA.DiskQuery.mFreeSector 
		xReadCH376Data(c1 );
		xReadCH376Data(c2 );
		xReadCH376Data(c3 );
		*DiskFre = c0 | (uint16_t)c1 << 8 | (uint32_t)c2 << 16 | (uint32_t)c3 << 24;

		xReadCH376Data(temp );  // CH376_CMD_DATA.DiskQuery.mDiskFat 
		*diskFat = temp;
		xEndCH376Cmd( );
		
	}
	else
		*DiskFre = 0;
	return( s );
}


uint8_t	CH376FileOpen(char* name )  /* �ڸ�Ŀ¼���ߵ�ǰĿ¼�´��ļ�����Ŀ¼(�ļ���) */
{
	CH376SetFileName( name );  /* ���ý�Ҫ�������ļ����ļ��� */
	if ( name[0] == DEF_SEPAR_CHAR1 || name[0] == DEF_SEPAR_CHAR2 ) CH376WriteVar32(VAR_CURRENT_CLUST, 0 );
	return( CH376SendCmdWaitInt( CMD0H_FILE_OPEN ) );
}

// �ڸ�Ŀ¼���ߵ�ǰĿ¼���½��ļ�,����ļ��Ѿ�������ô��ɾ�� 
uint8_t	CH376FileCreate(char *name )
{
	// ���ý�Ҫ�������ļ����ļ��� 
	if ( name )
		CH376SetFileName( name );  
//	p_ch376->usb_delay_ms(100);
	return(CH376SendCmdWaitInt(CMD0H_FILE_CREATE ) );
}

// �رյ�ǰ�Ѿ��򿪵��ļ�����Ŀ¼(�ļ���)
uint8_t	CH376FileClose( uint8_t is_update_size )
{
	return( CH376SendCmdDatWaitInt( CMD1H_FILE_CLOSE, is_update_size ) );
}

// ������Ϊ��λ�ڵ�ǰλ��д�����ݿ�,��֧��SD�� 
//���ݻ�������Ҫд�������������������д������ݸ���
uint8_t	CH376SecWrite( uint8_t *buf, uint8_t ReqCount, uint8_t *RealCount )  
{
	uint8_t	s;
	uint8_t	cnt,temp;
	uint32_t	StaSec;

	if ( RealCount )
		*RealCount = 0;

	do
	{
		p_ch376->usb_set_irq(0);
		xWriteCH376Cmd( CMD1H_SEC_WRITE );
		xWriteCH376Data( ReqCount );
		xEndCH376Cmd( );

		s = Wait376Interrupt( 1);
		if ( s != USB_INT_SUCCESS )
			return( s );
		xWriteCH376Cmd( CMD01_RD_USB_DATA0 );
		xReadCH376Data(temp );  // ��������sizeof(CH376_CMD_DATA.SectorWrite) 

		xReadCH376Data(cnt );  // CH376_CMD_DATA.SectorWrite.mSectorCount 
		xReadCH376Data(temp );
		xReadCH376Data(temp );
		xReadCH376Data(temp );

		// CH376_CMD_DATA.SectorWrite.mStartSector,��CH376оƬ��ȡ32λ�����ݲ��������� 
		StaSec = CH376Read32bitDat( ); 
		if ( cnt == 0 )
			break;
		s = CH376DiskWriteSec( buf, StaSec, cnt );  // ���������еĶ�����������ݿ�д��U�� 
		if ( s != USB_INT_SUCCESS )
		{
			p_ch376->usb_set_irq(1);
			return( s );
		}
		buf += cnt * DEF_SECTOR_SIZE;
		if ( RealCount ) *RealCount += cnt;
		ReqCount -= cnt;
	} while ( ReqCount );
	p_ch376->usb_set_irq(1);
	return( s );
}

/*******************************************************************************
* ��  ��  ��      : CH376ByteRead
* ��      ��      : ���ֽ�Ϊ��λ�ӵ�ǰλ�ö�ȡ���ݿ�
* ��      ��      : PUINT8 buf:
*					ָ�����ݻ�����.
*                   UINT16 ReqCount��
*                   �����ȡ���ֽ���.
*                   PUINT16 RealCount:
*                   ʵ�ʶ�ȡ���ֽ���.
* ��      ��      : �ж�״̬.
*******************************************************************************/
uint8_t	CH376ByteRead(uint8_t* buf, uint16_t ReqCount, uint16_t* RealCount )
{
	uint8_t	s;
	
	xWriteCH376Cmd( CMD2H_BYTE_READ );
	xWriteCH376Data_p((uint8_t *)&ReqCount, 2 );
	xEndCH376Cmd( );
	if ( RealCount ) 
	{
	    *RealCount = 0;
	}
	
	while ( 1 ) 
	{
		s = Wait376Interrupt( 1);
		if ( s == USB_INT_DISK_READ )                                                   /* �������ݶ��� */
		{
			s = CH376ReadBlock( buf, ReqCount );                                                  /* �ӵ�ǰ�����˵�Ľ��ջ�������ȡ���ݿ�,���س��� */
			xWriteCH376Cmd( CMD0H_BYTE_RD_GO );                                         /* ������ */
			xEndCH376Cmd( );
			buf += s;
			if ( RealCount ) 
			{
			    *RealCount += s;
			}
		}
		else 
		{
		    return( s );                                                                /* ���� */
		}
	}
}

//���ֽ�Ϊ��λ��ǰλ��д�����ݿ� 
uint8_t	CH376ByteWrite( uint8_t *buf, uint16_t ReqCount, uint16_t *RealCount )
{
	uint8_t	s;
	uint8_t	u16_part;
	
	
	p_ch376->usb_set_irq(0);
	xWriteCH376Cmd( CMD2H_BYTE_WRITE );
	u16_part = ReqCount;
	xWriteCH376Data(u16_part);
	u16_part = ReqCount>>8;
	xWriteCH376Data(u16_part);
	xEndCH376Cmd( );

	if ( RealCount )
		*RealCount = 0;

	while ( 1 )
	{
		
		s = Wait376Interrupt(0);
		
		if ( s == USB_INT_DISK_WRITE )
		{
			//���ڲ�ָ��������д����������ݿ�,���س��� 
			s = CH376WriteReqBlock( buf );
			xWriteCH376Cmd( CMD0H_BYTE_WR_GO );
			xEndCH376Cmd( );
			buf += s;
			if ( RealCount )
				*RealCount += s;
		}
		else
		{
			p_ch376->usb_set_irq(1);
			return( s );  // ����
			
		}
	}
	
//	p_ch376->usb_set_irq(1);
	
//	return 0;
}




// ���������еĶ�����������ݿ�д��U��,��֧��SD�� 
// baStart ��д�������ʼ��������, iSectorCount ��д��������� 
static uint8_t	CH376DiskWriteSec( uint8_t *buf, uint32_t iLbaStart, uint8_t iSectorCount )  
{
	uint8_t		s, err;
	uint16_t	mBlockCount;

	for ( err = 0; err != 3; ++ err )
	{
		// �������� 
		xWriteCH376Cmd( CMD5H_DISK_WRITE );  // ��USB�洢��д���� 
		xWriteCH376Data_u32(iLbaStart);
		
		xWriteCH376Data( iSectorCount );  // ������ 
		xEndCH376Cmd( );

		mBlockCount = iSectorCount * DEF_SECTOR_SIZE / CH376_DAT_BLOCK_LEN;
		
		for ( ; mBlockCount != 0; -- mBlockCount )
		{ 
			// ���ݿ���� 
			s = Wait376Interrupt( 1);  // �ȴ��жϲ���ȡ״̬ ��Ӧ����0x1E
			if ( s == USB_INT_DISK_WRITE )
			{
				// USB�洢��д���ݿ�,��������д�� 
				CH376WriteHostBlock( buf, CH376_DAT_BLOCK_LEN );  // ��USB�����˵�ķ��ͻ�����д�����ݿ� 
				xWriteCH376Cmd( CMD0H_DISK_WR_GO );  // ����ִ��USB�洢����д���� 
				xEndCH376Cmd( );
				buf += CH376_DAT_BLOCK_LEN;
			}
			else
				break;  // ���ش���״̬ 
		}
		if ( mBlockCount == 0 )
		{
			s = Wait376Interrupt(1 );  // �ȴ��жϲ���ȡ״̬ 
			if ( s == USB_INT_SUCCESS )
				return( USB_INT_SUCCESS );  // �����ɹ� 
		}
		if ( s == USB_INT_DISCONNECT )
			return( s );  // U�̱��Ƴ� 
		CH376DiskReqSense( );  // ���USB�洢������ 
	}
	return( s );  // ����ʧ�� 
}
uint8_t	CH376_Set_Data_Time(int dtm_type, uint16_t	dtm)
{
//	FAT_DIR_INFO	*fat_info = (FAT_DIR_INFO	*)DataBuff;
	uint8_t s;
	uint8_t offset = 0;
	//1 ��ȡ��ǰ�ļ���FAT_DIR_INFO�ṹ���ڴ滺����
	s = CH376SendCmdDatWaitInt( CMD1H_DIR_INFO_READ, 0xFF );
	if ( s != USB_INT_SUCCESS ) 
		return s;
	//2 c���ڴ滺������������
	CH376ReadBlock(DataBuff, CH376_DATABUF_SIZE);
	
	//3 �ٴζ�ȡ��ǰ�ļ���FAT_DIR_INFO�ṹ���ڴ滺����
	s = CH376SendCmdDatWaitInt( CMD1H_DIR_INFO_READ, 0xFF );
	if ( s != USB_INT_SUCCESS ) 
		return s;
	//4 ����Ҫ�޸ĵĲ�������д���ڴ滺��ƫ�Ƶ�ַ
	switch(dtm_type)
	{
		case DTM_CREATE_DATE:
			offset = (uint32_t)(&((FAT_DIR_INFO *)0)->DIR_CrtDate);
			break;
		case DTM_CREATE_TIME:
			offset = (uint32_t)(&((FAT_DIR_INFO *)0)->DIR_CrtTime);;
			break;		
		case DTM_CHANGE_DATE:
			offset = (uint32_t)(&((FAT_DIR_INFO *)0)->DIR_WrtDate);;
			break;
		case DTM_CHANGE_TIME:
			offset = (uint32_t)(&((FAT_DIR_INFO *)0)->DIR_WrtTime);;
			break;
		case DTM_LASTACC_DATE:
			offset = (uint32_t)(&((FAT_DIR_INFO *)0)->DIR_LstAccDate);;
			break;
		
		
		default:
			return USB_INT_BUF_OVER;		//���ݴ���
		
	}
	
	//5 д��Ҫ�޸ĵ�����
	CH376WriteOfsBlock((uint8_t *)&dtm, offset, 2);
	//6 �����ļ���Ϣ
	
	return( CH376SendCmdWaitInt( CMD0H_DIR_INFO_SAVE ) );
}
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{


static void xWriteCH376Cmd(uint8_t mCmd)
{
//	uint8_t i,res;

	//ch376��SPIƬѡ��Ч���һ���ֽ���Ϊ����
//	SET_CH376ENA_LOW;
//	ch376_dev->write(ch376_dev, &mCmd, 1);
		xStartCH376Cmd();
		xWriteCH376Data(mCmd);

	
	
//	for (i=0;i<10;i++)
//	{
//		//���״̬�˿ڵı�־λ��λ4��æ��־������Ч
//		res = xReadCH376Status();
//		if (( res & PARA_STATE_BUSY) == 0)
//		{
//			break;
//		}
//	}
}

// ��ȡ�ж�״̬��ȡ���ж����� 

static uint8_t CH376GetIntStatus(void)
{
	uint16_t	sc = 1000;
	uint8_t s;

	while(1)
	{
		xWriteCH376Cmd(CMD01_GET_STATUS);
		xReadCH376Data(s);
		xEndCH376Cmd();
//		if((s == USB_INT_DISCONNECT) || (s == ERR_DISK_DISCON))
//		{
//			//���������п��ܻ��U�̰γ�
//			//���Ҫ֪ͨ�ϲ�
//			
//			

//			
//			if(ch376_up_irq)
//				ch376_up_irq(s);
//			
//			//��ʱ������ж������Ѿ�������
//			//Ҫ�������һ���ж�
//			xWriteCH376Cmd(CMD01_GET_STATUS);
//			xReadCH376Data(s);
//			xEndCH376Cmd();
//			
//			break;
//		}
//		else 
			
		if(s != CMD01_GET_STATUS)
			break;
		
		if(sc)
			sc --;
		else
			break;
		p_ch376->usb_delay_ms(1);
	}
	return (s);
}


//�����������,�ȴ��ж� 
static uint8_t	CH376SendCmdWaitInt( uint8_t mCmd )  
{
	uint8_t	itr = 0;
	p_ch376->usb_set_irq(0);
	
	xWriteCH376Cmd( mCmd );
	p_ch376->usb_delay_ms(1);
	xEndCH376Cmd();
	itr = Wait376Interrupt(0);
	p_ch376->usb_set_irq(1);
	return itr;
}

//??CH376??(INT#???)
static int Query376Interrupt(void)
{
	char	pin_val = 0;
//	ch376_int->read(ch376_int, &pin_val, 1);
	pin_val = p_ch376->usb_read_intr_pin();
	if(pin_val)
	{
		return 0;
	}
	else
		return 1;
}
static uint8_t Wait376Interrupt(int	set_irq )  
{
	uint16_t	i = 2000;
	if(set_irq)
		p_ch376->usb_set_irq(0);
	while(1)
	{
		if (Query376Interrupt( ))
		{
			if(set_irq)
				p_ch376->usb_set_irq(1);
			return( CH376GetIntStatus( ) );	
		}
			
		p_ch376->usb_delay_ms(1);
		if(i)
			i --;
		else
			return( CH376GetIntStatus( ) );
//			break;
	}
	if(set_irq)
		p_ch376->usb_set_irq(1);
	return( ERR_USB_UNKNOWN );
}

static  void Ch376_intr(void)
{
	
	uint8_t	s = CH376GetIntStatus();// ���CH376�ж�
	if(ch376_up_irq)
		ch376_up_irq(s);
	
	
	  
}

// ��USB�����˵�ķ��ͻ�����д�����ݿ� 
static void	CH376WriteHostBlock( uint8_t *buf, uint8_t len )
{
	xWriteCH376Cmd( CMD10_WR_HOST_DATA );
	xWriteCH376Data( len );  // ���� 
	if ( len )
	{
		xWriteCH376Data_p(buf, len);
//		do
//		{
//			xWriteCH376Data( *buf );
//			//p_ch376->usb_delay_ms(2);
//			buf ++;
//		} while ( -- len );
	}
	xEndCH376Cmd( );
}

static void xWriteCH376Data_u32(uint32_t data_u32)
{
	uint8_t u32_part;
	u32_part = data_u32;
	xWriteCH376Data(u32_part);
	u32_part = data_u32 >> 8;
	xWriteCH376Data(u32_part);
	u32_part = data_u32 >> 16;
	xWriteCH376Data(u32_part);
	u32_part = data_u32 >> 24;
	xWriteCH376Data(u32_part);
}

static void	CH376WriteVar32(uint8_t var, uint32_t dat )  /* дCH376оƬ�ڲ���32λ���� */
{
	
	xWriteCH376Cmd( CMD20_WR_OFS_DATA );
	xWriteCH376Data( var );
	xWriteCH376Data_u32(dat);
	
	xEndCH376Cmd( );
}

/*******************************************************************************
* ��  ��  ��      : CH376WriteOfsBlock
* ��      ��      : ���ڲ�������ָ��ƫ�Ƶ�ַд�����ݿ�.
* ��      ��      : PUINT8 buf��
*					ָ�������ݻ�����.
*					UINT8 ofs:
*					ƫ�Ƶ�ַ.
*					UINT8 len:
*					���ݳ���.
* ��      ��      : ��.
*******************************************************************************/
static void	CH376WriteOfsBlock(uint8_t * buf, uint8_t ofs, uint8_t len )
{
	xWriteCH376Cmd( CMD20_WR_OFS_DATA );
	xWriteCH376Data( ofs );  															/* ƫ�Ƶ�ַ */
	xWriteCH376Data( len );  															/* ���ݳ��� */
	xWriteCH376Data_p(buf, len );
//	if ( len ) 
//	{
//		do 
//		{
//			xWriteCH376Data( *buf );
//			buf ++;
//		} while ( -- len );
//	}
	xEndCH376Cmd( );
}


//���usb��������
//��������ʱ��USBOCA�����ͣ���Ӧ��������USBENA���ɵ͵�ƽ
//void Test_USBOCA(void)
//{
//	if(!(IO0PIN & USB_OCA))
//	{
//		SET_CH376ENA_LOW;
//		p_ch376->usb_delay_ms(100);
//	}
//}






































static uint8_t	CH376ReadVar8( uint8_t var )  /* ��CH376оƬ�ڲ���8λ���� */
{
	uint8_t	c0;

	xWriteCH376Cmd( CMD11_READ_VAR8 );
	xWriteCH376Data( var );
	xReadCH376Data(c0);
	xEndCH376Cmd( );
	return( c0 );
}
// ���USB�洢������ 
static uint8_t	CH376DiskReqSense( void )  
{
	uint8_t	s;
	p_ch376->usb_delay_ms( 50 );
	s = CH376SendCmdWaitInt( CMD0H_DISK_R_SENSE );
	p_ch376->usb_delay_ms( 50 );
	return( s );
}

////��ȡ���̺��ļ�ϵͳ�Ĺ���״̬
//uint8_t	Ch376GetDiskStatus(void)
//{
//	return (CH376ReadVar8(VAR_DISK_STATUS));
//}
















// ���ý�Ҫ�������ļ����ļ��� 
static void	CH376SetFileName(char *name )
{
	uint8_t	c;
	
	xWriteCH376Cmd( CMD10_SET_FILE_NAME );

	//ֻ��0x43���ϵİ汾������ô�ɣ�Ŀǰ�õ���0x45�汾��оƬ
	c = *name;
	xWriteCH376Data( c );
	while ( c )
	{
		p_ch376->usb_delay_ms(5);
		name ++;
		c = *name;
		if ( c == DEF_SEPAR_CHAR1 || c == DEF_SEPAR_CHAR2 ) c = 0;  // ǿ�н��ļ�����ֹ 
		xWriteCH376Data( c );
	}
	xEndCH376Cmd( );
}





// ��CH376оƬ��ȡ32λ�����ݲ���������
static uint32_t	CH376Read32bitDat( void )  
{
	uint8_t	c0, c1, c2, c3;
	uint32_t	res = 0;

	xReadCH376Data(c0 );
	xReadCH376Data(c1 );
	xReadCH376Data(c2 );
	xReadCH376Data(c3 );
	xEndCH376Cmd( );

	res = c0 | (uint16_t)c1 << 8 | (uint32_t)c2 << 16 | (uint32_t)c3 << 24 ;
	return( res );
}



// ���ڲ�ָ��������д����������ݿ�,���س��� 
static  uint8_t	CH376WriteReqBlock( uint8_t *buf )
{
	uint8_t	s, l;

	xWriteCH376Cmd( CMD01_WR_REQ_DATA );
	xReadCH376Data(l);  // ����
	s = xWriteCH376Data_p(buf, l);
	
	xEndCH376Cmd( );
	return( s );
}









// �����������һ�ֽ����ݺ�,�ȴ��ж� 
static uint8_t	CH376SendCmdDatWaitInt( uint8_t mCmd, uint8_t mDat )  
{
	
	uint8_t	itr = 0;

	p_ch376->usb_set_irq(0);
	xWriteCH376Cmd( mCmd );
	xWriteCH376Data( mDat );
	xEndCH376Cmd( );
	itr = Wait376Interrupt(0);
	p_ch376->usb_set_irq(1);
	return itr;
}






