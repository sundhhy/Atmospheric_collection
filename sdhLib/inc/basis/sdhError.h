#ifndef __MYERROR_H__
#define __MYERROR_H__

typedef int err_t;

#define RET_OK						0

#define RET_FAILED						-1

#define ERR_BEEN_INSTALL				-0x100		//�Ѿ�����ʼ�����ˣ�����ͨ��ж����ȥ���������
#define ERR_NOT_EXIST					-0x101
#define ERR_ILLEGALACCESS				-0x102
#define ERR_ALREADY_EXIST				-0x103
#define ERR_NOT_SUPPORT					-0x104		
#define ERR_OUT_OF_RANGE					-0x105	//��ַ����Χ
#define ERR_OUT_OF_LIMIT					-0x106	//���ݳ���

//�豸�����
#define ERR_DEV_UNAVAILABLE				-0x200		//
#define ERR_DEV_FAILED					-0x201

//��Դ�����
#define ERR_RSU_UNAVAILABLE				-0x300
#define ERR_RSU_BUSY					-0x301
//������
#define ERR_OPT_FAILED					-0x400
#define ERR_OPT_REPEAT					-0x401				//�ظ��Ĳ���
#define ERR_OPT_ILLEGAL					-0x402				//�ظ��Ĳ���

//���������
#define ERR_PARAM_BAD						-0x500

//�ڴ���
#define ERR_MEM_LACK						-0x600

//�ļ���
#define ERR_FS_NOT_READY				-0x700  //�ļ��������ã����ܻ��ڱ�����

//ͨѶ��
#define ERR_CMM_ILLEGAL					-0x800
#define ERR_CMM_CMDERR					-0x801
#define ERR_CMM_CKECKERR					-0x802
#define ERR_CMM_ADDR_ERR					-0x803

//�ļ�ϵͳ��
#define ERR_FS_OPEN_FAIL				-0x900
//���µĴ�����벻Ҫ��ʹ���� 170714
#define ERR_MEM_UNAVAILABLE			-6					/* ??????			*/

#define ERR_RES_UNAVAILABLE			-7					/* ??????			*/
#define ERR_DEV_TIMEOUT					-8					/* ??????			*/
#define ERR_FAIL								-9					/* ����ʧ��		*/ 
#define ERR_ADDR_ERROR						-10
#define ERR_DRI_OPTFAIL							-11				/* ����ִ��ĳ���ʧ��			*/
#define ERR_DEV_BUSY							-12				/* ����ִ��ĳ���ʧ��			*/
#define ERR_BADSTATUS							-12				/*��ǰ״̬��֧��		*/

#define ERR_OK 									0 					/* No error, everything OK. */
#define ERR_MEM        -1    /* Out of memory error.     */
#define ERR_BUF        -2    /* Buffer error.            */
#define ERR_TIMEOUT    -3    /* Timeout.                 */
#define ERR_RTE        -4    /* Routing problem.         */
#define ERR_INPROGRESS -5    /* Operation in progress    */
#define ERR_VAL        -6    /* Illegal value.           */
#define ERR_WOULDBLOCK -7    /* Operation would block.   */
#define ERR_USE        -8    /* Address in use.          */
#define ERR_ISCONN     -9    /* Already connected.       */

#define ERR_IS_FATAL(e) ((e) < ERR_ISCONN)

#define ERR_ABRT       -10   /* Connection aborted.      */
#define ERR_RST        -11   /* Connection reset.        */
#define ERR_CLSD       -12   /* Connection closed.       */
#define ERR_CONN       -13   /* Not connected.           */

#define ERR_ARG        -14   /* Illegal argument.        */

#define ERR_IF         -15   /* Low-level netif error    */



#define ERR_UNKOWN     			-16    /* δ֪����      */
#define ERR_BAD_PARAMETER    	-17    /* �������      */
#define ERR_ERROR_INDEX     	-18    /* ��������      */
#define ERR_UNINITIALIZED     	-19    /* δ��ʼ���ı�������ϵͳ      */
#define ERR_CATASTROPHIC_ERR	-20 /* �����Դ���			*/
#define ERR_UNAVAILABLE			-21	  /* ���ɻ�ȡ����Դ			*/
#define ERR_OSRSU_UNAVAILABLE	-24	  /* ���ɻ�ȡ��ϵͳ��Դ			*/
#define ERR_BUSY     			-23    /*       */
/*
err_t :
MSB  ----------|----------- LSB
1  	|	ģ���ʶ  |	�����ʶ
1bit| 15b				| 16b


*/

#define MODULE_BITS	15
#define ERROR_BITS	16

#define ERROR_BIT		(1<<(MODULE_BITS + ERROR_BITS))
#define ERROR_BEGIN(_module_id)	((_module_id) << ERROR_BITS)
#define	ERROR_T(_module_error)	(ERROR_BIT | _module_error)

//��ȡ�����ʶ
#define MODULE_ERROR(_error_t) ((_error_t) & ((1 << ERROR_BITS) - 1))
//��ȡģ���ʶ
#define MODULE_ID(_error_t)		(((_error_t) & ~(ERROR_BIT)) >> ERROR_BITS)

#define INVERSE_ERROR(_error_t) ((_error_t) & ( ~(ERROR_BIT)))

//----------------------------------------------------------------------
//ϵͳ�е����������ṹ
// ��4λ��ʾ�㼶 ����λ��ʾ�㼶�е����   �㼶|���
//----------------------------------------------------------------------
#define	APP_MAIN	0x00
#define HAL_ENC624	0x80
#define DRIVE_GPMC	0x90
#define DRIVE_GPIO	0x91
#define CHECK_ERROR( return_val, msg) { \
	if( return_val != 0) { \
		fprintf( stderr, "%s:%s \n", msg, strerror(return_val)); \
		exit(-1);	\
		}\
	}


#endif
