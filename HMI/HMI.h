#ifndef _INC_HMI_H_
#define _INC_HMI_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <stdint.h>
#include <string.h>
#include "lw_oopc.h"

#include "sdhDef.h"
#ifdef NO_ASSERT
#include "basis/assert.h"
#else
#include "assert.h"
#endif

#include "sheet.h"
#include "focus.h"

#include "Component_Button.h"
#include "Component_curve.h"
#include "Component_progress_bar.h"
#include "Component_tips.h"

//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------




//��ʾ�õķ���
#define		HMI_DIR_UP					0
#define		HMI_DIR_DOWN				1
#define		HMI_DIR_LEFT				2
#define		HMI_DIR_RIGHT				3

#define		HMI_CMP_ALL					0xff		//


/********ͼ���ID	***********************************/
#define	ICO_ID_MENU					0x20	//���˵�
#define	ICO_ID_BAR					0x21	//��ͼ
#define	ICO_ID_DIGITAL				0x22	//����
#define	ICO_ID_PGUP					0x24	//���Ϸ�ҳ
#define	ICO_ID_PGDN					0x25	//���·�ҳ
#define	ICO_ID_ERASETOOL			0x26	//��������
#define	ICO_ID_SAVE					0x27	//����
#define	ICO_ID_COPY					0x28	//����
#define	ICO_ID_STOP					0x29	//ֹͣ
#define	ICO_ID_LOOP					0x2a	//ѭ����ʾ


#define	SHEET_BOXLIST				0x30
#define	SHEET_G_TEXT				0x31
#define	SHEET_PSD_TEXT			0x32

#define IS_CHECK(n)				((n&0xf0) == 0x30)
#define SHTID_CHECK(n)			(0x30 + n)

#define	GET_CHN_FROM_ID(id)		(id & 0x0f)


#define SHTID_RTL_MDIV				0x40

#define SHT_BTN_ID(n)			(0x30 | n)

/***************************************************************/


#define	HMI_FLAG_HIDE			1
#define	HMI_FLAG_SHOW			2
#define	HMI_FLAG_DEAL_HIT		3	
#define	HMI_FLAG_KEEP			4			//��Щ�����л�ʱ��Ҫ���治Ҫ���³�ʼ��״̬
#define	HMI_FLAG_ERR			0x10
#define HMI_FLAG_ONCE_USE_ARG_1	0x20		//Ҫ��ʹ�ý������1��ֻ��Чһ��

//���ڽ���������Ը�λ�Ĳ���
//��ʾ���������Ҫ�����ڽ��н���������ڴ����л��ؽ����ʱ�򣬻ᴦ��һЩ����Ľ�����Ϣ
//��ÿ����Ҫ�봰�ڽ����Ľ��棬����show�����У���Ӧ�����봰�ڽ��潻���Ĵ��� 
#define	HMIFLAG_WIN				0x20		
#define	HMIFLAG_KEYBOARD		0x40
//���浱ǰ�Ľ���λ�ڰ�ť����
#define	HMIFLAG_FOCUS_IN_BTN	0x80

#define IS_HMI_HIDE(flag)	((flag&1) == 0)
#define IS_HMI_KEYHANDLE(flag)	((flag&2))


#define SY_KEYTYPE_HIT				0
#define SY_KEYTYPE_DHIT				1
#define SY_KEYTYPE_LONGPUSH			2

//������

 //------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------

typedef int	(*keyboard_commit)(void *self, void *data, int len);


typedef struct {
	uint8_t		rowGrap;			//�м��
	uint8_t		colGrap;			//�м��
	uint8_t		bkc;
	uint8_t		numRow;
	uint8_t		numCol;
	uint8_t		none[3];
}hmiAtt_t;

typedef struct {
	uint8_t		f_row, f_col;
	uint8_t		start_byte, num_byte;
}strategy_focus_t;



typedef struct {
	uint8_t		key_type;
	uint8_t		none[3];
}strategy_keyval_t;

typedef int (*stategy_cmd)(void *p_rcv, int cmd, void *arg);

typedef enum {
	sycmd_reflush = 0,
	sycmd_reflush_position,  //ָ��λ�õ�ˢ��
	sycmd_win_tips,
	sycmd_win_time,
	sycmd_win_psd,
	sycmd_keyboard,
	wincmd_commit,
}e_sycmd_r;

typedef struct {
	
	
	//��pp_dataָ�����кź��кŵ���ʾ�ַ���ָ�룬�������ַ����ĳ���
	//����Ϊ0ʱ��ʾû������
	//���е���ʾӦ���Ƕ���ģ������ǳ��ֿն������
	int (*entry_txt)(int row, int col,void *pp_text);	
	int	(*init)(void	*arg);
	void (*build_component)(void *arg);
	int	(*key_hit_up)(void	*arg);
	int	(*key_hit_dn)(void	*arg);
	int	(*key_hit_lt)(void	*arg);
	int	(*key_hit_rt)(void	*arg);
	int	(*key_hit_er)(void	*arg);
	void (*reset_focus)(void);
	int	(*get_focus_data)(void *pp_data,  strategy_focus_t *p_in_syf);		//�ɹ����ش���0��ʧ�ܷ���-1
	int	(*commit)(void	*arg);
	void	(*sty_exit)(void);
	strategy_focus_t		sf;
	char								*p_stg_title;
	void								*p_cmd_rcv;
	stategy_cmd					cmd_hdl;
	uint8_t							stg_num_rows;			//�ܹ�������
	
	//����ʾ����Ļ�ϵ��еķ�Χ�����ڲ�������ִ�н����ƶ�ʱ���ж������ʱ�������Է�Χ
	uint8_t							row_in_lcd[2];		
	uint8_t							none;
}strategy_t;


ABS_CLASS(HMI)
{
//	HMI*			prev;
//	HMI*			next;
	
	focus_user_t	*p_fcuu;
	uint8_t			flag;
	uint8_t			arg[3];		//�л���Ļʱ��������Ļ�䴫��һЩ����
	
	
	//��ʾ��̬����
	int		(*init)( HMI *self, void *arg);
	void	(*show)( HMI *self);
	
	
	//�������
//	void	(*dataVisual)(HMI *self, void *arg);		//17-09-20 ���ݿ��ӻ�����
	
	// initSheet hide 
	void		(*hide)( HMI *self);
	void		(*initSheet)( HMI *self);
	void		(*switchHMI)( HMI *self, HMI *p_hmi);
	void		(*switchBack)( HMI *self);
	
	//��������
	void		(*hitHandle)( HMI *self, char kcd);
	void		(*dhitHandle)( HMI *self, char kcd);
	void		(*longpushHandle)( HMI *self,  char kcd);
	void		(*conposeKeyHandle)( HMI *self, char kcd_1, char kcd_2);
	//���㴦��
	void		(*init_focus)(HMI *self);
	void		(*clear_focus)(HMI *self, uint8_t fouse_row, uint8_t fouse_col);
	void		(*show_focus)( HMI *self);
	
	
		//��Ļ�ϵİ�ť
	void		(*build_component)(HMI *self);
	void		(*clean_cmp)(HMI *self);
	void		(*show_cmp)(HMI *self);
	int		(*btn_forward)(HMI *self);
	int		(*btn_backward)(HMI *self);
	void		(*btn_jumpout)(HMI *self);
	
	
	//180202 ����ִ�У�������������ˢ��Ļ�����������ڲ����ݵ�ˢ��
	void		(*hmi_run)(HMI *self);			
//	void		(*btn_hit)(HMI *self);
	
};

typedef   const char  ro_char;


//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------
extern keyboard_commit	kbr_cmt;

extern const Except_T Hmi_Failed;
//extern  ro_char str_endRow[];
//extern	ro_char str_endCol[];
extern HMI *g_p_curHmi, *g_p_lastHmi, *g_p_win_last;
//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------
extern void Set_flag_show(uint8_t	*p_flag, int val);
//extern void Set_flag_keyhandle(uint8_t	*p_flag, int val);
void STG_Duild_button(void *arg);

//�ܶ����Ĵ�����������İ�������һ�������ԾͰ�������Ĵ����ų���


int HMI_Init(void);
void HMI_choice(HMI *self, uint8_t choice_id);

extern void HMI_Ram_init(void);
extern void *HMI_Ram_alloc(int bytes);
extern uint16_t HMI_Ram_free_bytes(void);

#endif
