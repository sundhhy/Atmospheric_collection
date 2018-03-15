//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//
#include "Component_curve.h"
#include "ExpFactory.h"
#include "HMI.h"
#include "HMI_comm.h"
#include "sdhDef.h"
#include "arithmetic/bit.h"
#include "mem/CiiMem.h"
//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------


#define CRV_FLAG_HIDE				1
#define CRV_FLAG_FULL_CLEAN			2		//�����������������ʱ�����ȫ���������ݱ�־


#define	CRV_BEEK					1		//����ƽ����
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

static const char CRV_code_line[] = { "<line></>" };
static const char CRV_code_bk_pic[] =  {"<bpic vx0=0 vy0=50 m=1 >15</>" };
//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------

		
//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static Curve 	*p_CRV_self = NULL;
static sheet	*p_CRV_line;
static sheet	*p_CRV_bk_pic;

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static void 	CRV_Init(Curve *self, uint8_t	num_curve);
static int 		CRV_Alloc(curve_att_t  *c);
static void		CRV_Free(uint8_t  crv_fd);
static void		CRV_Reset(uint8_t  crv_fd);
static void		CRV_Add_point(uint8_t  crv_fd, uint8_t prc);
static void		CRV_Ctl(uint8_t  crv_fd, uint8_t	ctl, uint16_t val);
static void		CRV_Show_bkg(void);
static void		CRV_Show_curve(uint8_t  crv_fd, uint8_t show_ctl);
static void		CRV_Data_flex(uint8_t	crv_fd, char flex, uint8_t	scale, uint16_t new_max_num);


static void CRV_Alloc_bkg_id(uint8_t crv_fd);
static void CRV_Set_dirty(uint8_t bkg_id, uint8_t val);
static void CRV_Copy_att(curve_att_t *dst, curve_att_t *src);
static void CRV_Deal_full(uint8_t crv_fd);
static void CRV_Exc_display(uint8_t  crv_fd, uint8_t show_ctl);
static void CRV_Clean_bkg(uint8_t	crv_fd);
static void CRV_Draw_left_to_right(uint8_t  crv_fd, uint8_t show_ctl);
static void CRV_Draw_right_to_left(uint8_t  crv_fd, uint8_t show_ctl);
static __inline uint8_t *CRV_Get_val_y(uint8_t crv_fd, short index);
//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//
Curve	*CRV_Get_Sington(void)
{
	
	if(p_CRV_self == NULL)
	{
		p_CRV_self = Curve_new();
		if(p_CRV_self  == NULL) while(1);
	}
	
	return p_CRV_self;
	
}




CTOR(Curve)
FUNCTION_SETTING(init, CRV_Init);
FUNCTION_SETTING(alloc, CRV_Alloc);
FUNCTION_SETTING(free, CRV_Free);
FUNCTION_SETTING(reset, CRV_Reset);
FUNCTION_SETTING(add_point, CRV_Add_point);
FUNCTION_SETTING(crv_ctl, CRV_Ctl);
FUNCTION_SETTING(crv_show_bkg, CRV_Show_bkg);
FUNCTION_SETTING(crv_show_curve, CRV_Show_curve);
FUNCTION_SETTING(crv_data_flex, CRV_Data_flex);

END_CTOR
//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//
/// \name Private Functions
/// \{
static void 	CRV_Init(Curve *self, uint8_t	num_curve)
{
	Expr 			*p_exp ;
	shtctl 		*p_shtctl = NULL;
	
	p_shtctl = GetShtctl();
	p_exp = ExpCreate( "line");
	p_CRV_line = Sheet_alloc( p_shtctl);
	p_exp->inptSht( p_exp, (void *)CRV_code_line, p_CRV_line) ;
	
	p_exp = ExpCreate( "pic");
	p_CRV_bk_pic = Sheet_alloc( p_shtctl);
	p_exp->inptSht( p_exp, (void *)CRV_code_bk_pic, p_CRV_bk_pic);
	
	
	
	self->num_curve = num_curve;
	self->set_free_curve = 0xff;
	self->set_vaild_curve = 0;
	self->p_crv_att = CALLOC(num_curve, sizeof(curve_att_t));
	self->p_run_info = CALLOC(num_curve, sizeof(crv_run_info_t));
	self->p_bkg_info = CALLOC(num_curve, sizeof(crv_back_ground_info_t));
}

//ʧ�ܷ���-1
static int CRV_Alloc(curve_att_t  *c)
{
	uint16_t		i = 0;

	
	for(i = 0; i < p_CRV_self->num_curve; i ++)
	{
		if(Check_bit(&p_CRV_self->set_free_curve, i))
		{
			Clear_bit(&p_CRV_self->set_free_curve, i);
			Set_bit(&p_CRV_self->set_vaild_curve, i);
			CRV_Copy_att(p_CRV_self->p_crv_att + i, c);
			p_CRV_self->p_run_info[i].p_vals_y = HMI_Ram_alloc(p_CRV_self->p_crv_att[i].crv_buf_size);
			p_CRV_self->p_run_info[i].next_index = 0;
			p_CRV_self->p_run_info[i].crv_start_index = 0;
			p_CRV_self->p_crv_att[i].crv_flag |= CRV_FLAG_FULL_CLEAN;		//Ĭ������£��������˾����
			CRV_Alloc_bkg_id(i);
			return i;
			
		}
	}	
	
	return -1;
	
}

static void		CRV_Free(uint8_t  crv_fd)
{
	int	i = 0;
	
	for(i = 0; i < p_CRV_self->num_curve; i++)
	{
		if((i == crv_fd) || (crv_fd == HMI_CMP_ALL))
		{
			Clear_bit(&p_CRV_self->set_vaild_curve, i);
			Set_bit(&p_CRV_self->set_free_curve, i);
			p_CRV_self->p_crv_att[i].crv_flag = 0;
			p_CRV_self->p_run_info[i].crv_num_points = 0;
			p_CRV_self->p_run_info[i].crv_start_index = 0;
			p_CRV_self->p_run_info[i].next_index = 0;
		}	
	}
}
static void		CRV_Reset(uint8_t  crv_fd)
{
	if(crv_fd >= p_CRV_self->num_curve)
			return;
	
	p_CRV_self->p_run_info[crv_fd].next_index = 0;
	p_CRV_self->p_run_info[crv_fd].crv_num_points = 0;
	
}


static void		CRV_Add_point(uint8_t  crv_fd, uint8_t	prc)
{
	uint16_t		val_y = 0;
	uint16_t		range;
	uint16_t		height;
	
	
	if(crv_fd >= p_CRV_self->num_curve)
			return;
	

	
	
	CRV_Deal_full(crv_fd);
		
//	if(cv->val <= cv->lower_limit)
//		cv->prc = 0;
//	else if(cv->val >= cv->up_limit)
//		cv->prc = 100;
//	else
//	{
//		cv->prc = ((cv->val - cv->lower_limit) * 100) / (cv->up_limit - cv->lower_limit);
//		
//	}
	if(prc > 100)
		prc = 100;
	
	
	range = p_CRV_self->p_crv_att[crv_fd].crv_y1 - p_CRV_self->p_crv_att[crv_fd].crv_y0;
	height = prc * range / 100;
	val_y = p_CRV_self->p_crv_att[crv_fd].crv_y1 - height;
	
	if(val_y == *CRV_Get_val_y(crv_fd, p_CRV_self->p_run_info[crv_fd].next_index - 1))
	{
		val_y = *CRV_Get_val_y(crv_fd, p_CRV_self->p_run_info[crv_fd].next_index - 1);
		
	}
	
	*CRV_Get_val_y(crv_fd, p_CRV_self->p_run_info[crv_fd].next_index) = val_y;
//	p_CRV_self->p_run_info[crv_fd].p_vals_y[CRV_abs_idx(p_CRV_self->p_run_info[crv_fd].next_index)] = val_y;
	
	
	p_CRV_self->p_run_info[crv_fd].next_index += 1;
	p_CRV_self->p_run_info[crv_fd].next_index %= p_CRV_self->p_crv_att[crv_fd].crv_max_num_data;
	
	p_CRV_self->p_run_info[crv_fd].crv_num_points ++;
	
	
	
	if(p_CRV_self->p_crv_att[crv_fd].crv_direction == HMI_DIR_LEFT)
		CRV_Set_dirty(p_CRV_self->p_bkg_info[crv_fd].crv_bkg_id, 1);

}
static void		CRV_Ctl(uint8_t  crv_fd, uint8_t	ctl, uint16_t val)
{
//	crv_run_info_t					*p_run;
	curve_att_t							*p_att = p_CRV_self->p_crv_att + crv_fd;
//	crv_back_ground_info_t				*p_bkg =  p_CRV_self->p_bkg_info + crv_fd;
	int								i = 0;
	switch(ctl)
	{
		case CRV_CTL_HIDE:
			if(val)
				p_att->crv_flag |= CRV_FLAG_HIDE;
			else
				p_att->crv_flag &= ~CRV_FLAG_HIDE;
			CRV_Set_dirty(p_CRV_self->p_bkg_info[crv_fd].crv_bkg_id, 1);
			
			break;
		case CRV_CTL_STEP_PIX:
			if(val == 0)
				val = 1;
			
				
				
			for(i = 0; i < p_CRV_self->num_curve; i++)
			{
				if((i != crv_fd) && (crv_fd != HMI_CMP_ALL))
					continue;
				
				if(Check_bit(&p_CRV_self->set_vaild_curve, i) == 0)
					continue;
				
				p_att = p_CRV_self->p_crv_att + i;
//					p_run = p_CRV_self->p_run_info + i;
				
				p_att->crv_step_pix = val;
				
//					p_run->crv_start_index = 0;
//					p_run->next_index = 0;
				
//					p_run->crv_size = (p_att->crv_x1 - p_att->crv_x0) / p_att->crv_step_pix;
//					p_run->p_vals_y = HMI_Ram_alloc(p_CRV_self->p_run_info[i].crv_size);
//					p_run->next_index = 0;
//					p_run->crv_start_index = 0;
//					p_run->crv_num_points = 0;
				
					
			}
				
			
			
			if(crv_fd == HMI_CMP_ALL)
			{
				
				CRV_Set_dirty(HMI_CMP_ALL, 1);
				
			}
			else
			{
				
				CRV_Set_dirty(p_CRV_self->p_bkg_info[crv_fd].crv_bkg_id, 1);
				
			}
			
			break;
		case CRV_CTL_MAX_NUM:
			if(val > p_att->crv_buf_size)
				val = p_att->crv_buf_size;
			
			p_att->crv_max_num_data = val;
			break;
		default:
			break;
		
		
	}
	
}

//��start_idx��ƽ��ȥ��
static void CRV_Move_start_idx(uint8_t *dst, uint16_t src_star_idx, uint16_t len)
{
	short i, j;
	uint16_t	end;
	uint8_t tmp = 0;

	
	if(src_star_idx == 0)
		return;
	
	
	if(src_star_idx < len / 2)
	{
		
		//��ǰ��
		end = len - 1;
		
		for(i = 0;i < src_star_idx; i++)
		{
			tmp = dst[0];	//�ѿ�ͷ��λ�ÿճ���
			
			//���е�������ǰ�ƶ�һ��
			for(j = 0; j < end; j++)
			{
				dst[j] = dst[j + 1];
				
			}
			dst[len -1] = tmp;	//�ѿ�ͷ���ݷŵ�β��
			
		}
	}
	else
	{
		//������
		end = len - src_star_idx;
		
		for(i = 0;i < end ; i++)
		{
			tmp = dst[len - 1];	//������λ�ÿճ���
			
//			//���е����������ƶ�һ��
			for(j = len - 1; j >= 0; j--)
			{
				dst[j] = dst[j - 1];
//				
			}
			dst[0] = tmp;		//���������ݷŵ���ͷ
			
		}
	}
	
	
}

//��src�а��ռ������ȡ�����ݵ�dst
//�� 1 2 3 4���ռ��2�������dst = 1 3
static void CRV_smp(uint8_t *dst, uint8_t *src, uint16_t dst_len, uint16_t src_len, uint8_t src_star_idx, uint8_t gap)
{
	uint16_t i = 0, j = 0;
	uint16_t	l = dst_len;
	if(l > src_len)
		l = src_len;
	
	CRV_Move_start_idx(src, src_star_idx, src_len);
	
	for(i = 0; i < l; i+= gap)
	{
		
		dst[j++] = src[i];
		
	}
	
	
}

static void CRV_Cut_expand_tail(uint8_t *dst, uint8_t *src, uint16_t tali_len, uint16_t src_len, uint8_t mul)
{
	short i = 0, j = 0, k =0;
	uint16_t	l = tali_len;
	
	if(l > src_len)
		l = src_len;

	
	j = src_len - tali_len;
	
	
	//�Ӻ���ǰ��ֵ ��Ϊ�˱��⸲�ǵ�����
	for(i = l - 1; i >= 0; i -- )
	{
		
		dst[i * mul] = src[i + j];
		
	}
	
	for(i = 0; i < l; i+= 1)
	{
		
		for(k = 1; k < mul; k ++)
		{
			dst[i * mul + k] = src[i * mul];
		}
		
	}
	
	
	
}



//�Ŵ�
//���ߵ�ʱ����С��ʱ����Ļ�ϵĵ�������ʾ���µģ���ȡ�����ܷ��µĲ��ֽ���������ʾ
static void CRV_Zoom_in(uint8_t	crv_fd, uint8_t	scale, uint16_t new_max_num)
{
//	curve_att_t						*p_att = p_CRV_self->p_crv_att + crv_fd;
	crv_run_info_t				*p_run = p_CRV_self->p_run_info + crv_fd;
	uint8_t								*p1, *p2;
	uint8_t								cut_len = 0;
	
	
	p1 = p_run->p_vals_y;
	p2 = p_run->p_vals_y;
	
	
	CRV_Move_start_idx(p2, p_run->crv_start_index, p_run->crv_num_points);
	switch(scale)
	{
		case 1:	
			cut_len = p_run->crv_num_points % new_max_num;
		
			CRV_Cut_expand_tail(p1, p2, cut_len, p_run->crv_num_points, 1);
			p_run->crv_num_points = cut_len;
			
			break;
		case 2:
			cut_len = p_run->next_index % (new_max_num - 1);
		
			CRV_Cut_expand_tail(p1, p2, cut_len, p_run->crv_num_points, 2);	
			p_run->crv_num_points = cut_len * 2;
			break;
		case 4:
			cut_len = p_run->next_index % (new_max_num - 1);
			CRV_Cut_expand_tail(p1, p2, cut_len, p_run->crv_num_points, 4);	
			p_run->crv_num_points = cut_len * 4;
			break;
		
	}
	
	p_run->next_index = p_run->crv_num_points;
	p_run->crv_start_index = 0;
	
}



//��С
//ʱ���С�Ĵ��ʱ����ʾ����Ҫ�ȱ�����С���������Ҫ����ɸѡ����������
static void CRV_Zoom_out(uint8_t	crv_fd, uint8_t	scale, uint16_t new_max_num)
{
//	curve_att_t							*p_att = p_CRV_self->p_crv_att + crv_fd;
	crv_run_info_t					*p_run = p_CRV_self->p_run_info + crv_fd;
	uint8_t									*p1, *p2;
//	uint8_t								new_idx = 0;
	
	
	p1 = p_run->p_vals_y;
	p2 = p_run->p_vals_y;
	
	switch(scale)
	{
		case 1:
			//ɶҲ������
//			if(p_run->crv_start_index)		//����ʼλ�ù�0
//				CRV_smp(p1, p2, new_max_num, p_run->crv_num_points, p_run->crv_start_index, 1);
			CRV_Move_start_idx(p2, p_run->crv_start_index, p_run->crv_num_points);
			
			break;
		case 2:
			CRV_smp(p1, p2, new_max_num, p_run->crv_num_points, p_run->crv_start_index, 2);
			
			p_run->crv_num_points = p_run->crv_num_points >> 1;
			break;
		case 4:
			CRV_smp(p1, p2, new_max_num, p_run->crv_num_points, p_run->crv_start_index, 4);
			p_run->crv_num_points = p_run->crv_num_points >> 2;
			break;
	}
	
	p_run->next_index = p_run->crv_num_points;
	p_run->crv_start_index = 0;
	
	
}

static void		CRV_Data_flex(uint8_t	crv_fd, char flex, uint8_t	scale, uint16_t new_max_num)
{
	
	curve_att_t							*p_att;
	crv_run_info_t					*p_run;
	int										i;
	
	if(scale == 0)
		scale = 1;
	
	for(i = 0; i < p_CRV_self->num_curve; i ++)
	{
		if(Check_bit(&p_CRV_self->set_vaild_curve, i) == 0)
			continue;
		if((i != crv_fd) && crv_fd != HMI_CMP_ALL)
			continue;
		p_att = p_CRV_self->p_crv_att + i;
		p_run = p_CRV_self->p_run_info + i ;
		if(new_max_num > p_att->crv_buf_size)
			new_max_num = p_att->crv_buf_size;
		
		if(flex == FLEX_ZOOM_IN)
		{
			CRV_Zoom_in(i, scale, new_max_num);
			
			
		} 
		else if(flex == FLEX_ZOOM_OUT)
		{
			
			CRV_Zoom_out(i, scale, new_max_num);
			
		}
		else
		{
			p_run->crv_num_points = 0;
			p_run->crv_start_index = 0;
			p_run->next_index = 0;
		}
		p_att->crv_max_num_data = new_max_num;
	}
	
}



static void		CRV_Show_bkg(void)
{
	if(p_CRV_self->set_vaild_curve)
		p_CRV_bk_pic->p_gp->vdraw(p_CRV_bk_pic->p_gp, &p_CRV_bk_pic->cnt, &p_CRV_bk_pic->area);
}


static void CRV_Alloc_bkg_id(uint8_t crv_fd)
{
//	crv_run_info_t					*p_run = p_CRV_self->p_run_info + crv_fd;
//	curve_att_t							*p_att = p_CRV_self->p_crv_att + crv_fd;
//	crv_back_ground_info_t	*p_bkg;
//	short	i;
//	short	used_id = 0;
//	for(i = 0; i < p_CRV_self->num_curve; i++)
//	{
//		if(Check_bit(&p_CRV_self->set_vaild_curve, i) == 0)
//			continue;
//		
//		
//		
//	}
	
	//todo: Ŀǰ�����ߵı����϶���һ���ģ���˾Ͳ�������������ˣ�����Ϊ0,�Ժ�����Ҫ��������ʾ�ڲ�ͬ������ʱ����ʵ���㷨
	
	p_CRV_self->p_bkg_info[crv_fd].crv_bkg_id = 0;
	
}

static void CRV_Copy_att(curve_att_t *dst, curve_att_t *src)
{
	
	dst->crv_col = src->crv_col;
	dst->crv_direction = src->crv_direction;
	if(src->crv_step_pix)
		dst->crv_step_pix = src->crv_step_pix;
	else
		dst->crv_step_pix = 1;
	dst->crv_flag = src->crv_flag;
	dst->crv_x0 = src->crv_x0;
	dst->crv_x1 = src->crv_x1;
	dst->crv_y0 = src->crv_y0;
	dst->crv_y1 = src->crv_y1;
	dst->crv_buf_size = src->crv_buf_size;
	dst->crv_max_num_data = src->crv_max_num_data;
	
}

static void CRV_Deal_full(uint8_t crv_fd)
{
	
	if(p_CRV_self->p_run_info[crv_fd].crv_num_points <  p_CRV_self->p_crv_att[crv_fd].crv_max_num_data)
		return;
	
	//���ߵ������Ժ������ȫ���ĵ����»��ƻ���������ǵ���
	if(p_CRV_self->p_crv_att[crv_fd].crv_flag & CRV_FLAG_FULL_CLEAN)
	{
		CRV_Set_dirty(p_CRV_self->p_bkg_info[crv_fd].crv_bkg_id, 1);
		//���������
		p_CRV_self->p_run_info[crv_fd].next_index = 0;
		p_CRV_self->p_run_info[crv_fd].crv_num_points = 0;
	}
	else
	{
		//ֻ�Ƴ�������Ǹ���,��������
		p_CRV_self->p_run_info[crv_fd].next_index += 1;
		p_CRV_self->p_run_info[crv_fd].next_index %= p_CRV_self->p_crv_att[crv_fd].crv_max_num_data;
		//�ƶ������ߵ���ʼ����
		p_CRV_self->p_run_info[crv_fd].crv_start_index ++;
		p_CRV_self->p_run_info[crv_fd].crv_start_index %= p_CRV_self->p_crv_att[crv_fd].crv_max_num_data;
	}
	
	
}

static void		CRV_Show_curve(uint8_t  crv_fd, uint8_t show_ctl)
{
	
	short	i = 0, has_show = 0;
	
	if(show_ctl == CRV_SHOW_WHOLE)		
		CRV_Set_dirty(HMI_CMP_ALL, 1);
	
	
	
	
	for(i = 0; i < p_CRV_self->num_curve; i++)
	{
		if((i != crv_fd) && (crv_fd != HMI_CMP_ALL))
		{
			continue;
			
		}
		if(Check_bit(&p_CRV_self->set_vaild_curve, i) == 0)
			continue;
		if(p_CRV_self->p_crv_att[i].crv_flag & CRV_FLAG_HIDE)
			continue;
		
		has_show = 1;
		//��ʾ������
		CRV_Exc_display(i, show_ctl);
		
		
	}
	
	//һ������Ҳû����ʾ��Ҫ���һ�±������������
	if(has_show == 0)
	{
		for(i = 0; i < p_CRV_self->num_curve; i++)
		{
			if((p_CRV_self->p_bkg_info[i].crv_bkg_dirty))
			{
				CRV_Clean_bkg(i);
				
			}
			
		}
		
		
	}
	
		
	
	
}
static void CRV_Exc_display(uint8_t  crv_fd, uint8_t show_ctl)
{
	if((p_CRV_self->p_bkg_info[crv_fd].crv_bkg_dirty))
	{
		CRV_Clean_bkg(crv_fd);
		
	}
	
	p_CRV_line->cnt.colour = p_CRV_self->p_crv_att[crv_fd].crv_col;
	
	if(p_CRV_self->p_run_info[crv_fd].crv_num_points == 0)
		return;
	
	//���ֻ��һ����ͻ�һ����
	if(p_CRV_self->p_run_info[crv_fd].crv_num_points == 1) {
		p_CRV_line->area.x0 = p_CRV_self->p_crv_att[crv_fd].crv_x0;
		p_CRV_line->area.x1 = p_CRV_self->p_crv_att[crv_fd].crv_x0;
//		p_CRV_line->area.y0 = p_CRV_self->p_run_info[crv_fd].p_vals_y[p_CRV_self->p_run_info[crv_fd].crv_start_index];
		p_CRV_line->area.y0 = *CRV_Get_val_y(crv_fd, p_CRV_self->p_run_info[crv_fd].crv_start_index);
		p_CRV_line->area.y1 = p_CRV_line->area.y0;
		
		p_CRV_line->p_gp->vdraw(p_CRV_line->p_gp, &p_CRV_line->cnt, &p_CRV_line->area);
		
		return;
	}
	
	if(p_CRV_self->p_crv_att[crv_fd].crv_direction == HMI_DIR_LEFT)
	{
		CRV_Draw_right_to_left(crv_fd, show_ctl);
	}
	else if(p_CRV_self->p_crv_att[crv_fd].crv_direction == HMI_DIR_RIGHT)
	{
		
		CRV_Draw_left_to_right(crv_fd, show_ctl);
		
	}
	
	
}

static void CRV_Set_dirty(uint8_t bkg_id, uint8_t val)
{
	int		i;
	for(i = 0; i < p_CRV_self->num_curve; i++)
	{
		if(Check_bit(&p_CRV_self->set_vaild_curve, i) == 0)
			continue;
		
		if((p_CRV_self->p_bkg_info[i].crv_bkg_id != bkg_id) && (bkg_id != HMI_CMP_ALL))
			continue;
		p_CRV_self->p_bkg_info[i].crv_bkg_dirty = val;
	}
	
	
}


static void CRV_Clean_bkg(uint8_t	crv_fd)
{
	p_CRV_bk_pic->p_gp->vdraw(p_CRV_bk_pic->p_gp, &p_CRV_bk_pic->cnt, &p_CRV_bk_pic->area);
	
	//��������ͬid�ı��������־����
	CRV_Set_dirty(p_CRV_self->p_bkg_info[crv_fd].crv_bkg_id, 0);
}

//������ָ��������ͬ�ĵ���
//��������������
static int CRV_Rle(uint8_t  crv_fd, int cur_idx)
{
	crv_run_info_t	*p_run = p_CRV_self->p_run_info + crv_fd;
//	curve_att_t			*p_att = p_CRV_self->p_crv_att + crv_fd;
	int 	len = 0;
//	short 	i = 0;
	short	points = 0;
	short	idx = 0;
	
	points = p_run->crv_num_points - (p_run->crv_start_index + cur_idx) - 1;
	idx = cur_idx + 1;
	
	//��cur_idx֮�����еĵ���м���
	while(points-- > 0) {
		
//		idx = cur_idx + i;
//		idx %= p_att->crv_max_num_data;
		
		
		if(abs(*CRV_Get_val_y(crv_fd, cur_idx) - \
			*CRV_Get_val_y(crv_fd, idx)) < CRV_BEEK)
			len ++;
		else
			break;
		idx ++;
		
		
	}
//	if(len < 1)
//		len = 1;
	return len ;
}


//����������ʾ�����ߣ���Ҫ�����ػ��
//todo:180205 ��ʱ����Ҫ
static void CRV_Draw_right_to_left(uint8_t  crv_fd, uint8_t show_ctl)
{
//	short 	i = 0;
//	short		points = 0;
//	short		idx = 0;
//	short		len = 0;
//	crv_run_info_t	*p_run = p_CRV_self->p_run_info + crv_fd;
//	curve_att_t			*p_att = p_CRV_self->p_crv_att + crv_fd;
//	
//	points = p_run->crv_num_points;
//	
//	for(i = 0; i < (points - 1); i+= len) {
//		//����x0,y0
//		p_CRV_line->area.x0 = p_att->crv_x1 - (points - 1 - i) * p_att->crv_step_pix;
//		idx = p_run->crv_start_index + i;
//		idx %= p_run->crv_size;
//		p_CRV_line->area.y0 = p_run->p_vals_y[idx];
//		
//		//�������x1,y1������

//		len = CRV_Rle(crv_fd, idx);
//		//����x1, y1
//		p_CRV_line->area.x1 = p_CRV_line->area.x0 + p_att->crv_step_pix * (len + 0);
//		idx += len;
//		idx %= p_run->crv_size;
//		p_CRV_line->area.y1 = p_run->p_vals_y[idx];
//		
//		p_CRV_line->p_gp->vdraw(p_CRV_line->p_gp, &p_CRV_line->cnt, &p_CRV_line->area);
//	}
}


//�����������1��ʱ���ܵ��ã���������
//���������������Ƿ����1

static void CRV_Draw_left_to_right(uint8_t  crv_fd, uint8_t show_ctl)
{
	crv_run_info_t	*p_run = p_CRV_self->p_run_info + crv_fd;
	curve_att_t			*p_att = p_CRV_self->p_crv_att + crv_fd;
	short 	i = 0;
	short		points = 0;
	short		idx = 0;
	short		len = 0;
	
	
	if(show_ctl == CRV_SHOW_LATEST)
	{
		
	
		p_CRV_line->cnt.colour = p_att->crv_col;
		
		p_CRV_line->area.x0 = p_att->crv_x0 +  p_att->crv_step_pix * \
		(((p_run->next_index + p_att->crv_max_num_data)- 2) % p_att->crv_max_num_data);
		
		p_CRV_line->area.x1 = p_CRV_line->area.x0 + p_att->crv_step_pix;
		
		p_CRV_line->area.y0 =  *CRV_Get_val_y(crv_fd, p_run->next_index - 2);
		p_CRV_line->area.y1 =  *CRV_Get_val_y(crv_fd, p_run->next_index - 1);
		p_CRV_line->p_gp->vdraw(p_CRV_line->p_gp, &p_CRV_line->cnt, &p_CRV_line->area);		
		return;
	}
	
	
	points = p_run->crv_num_points - 1;
	
	//������ƽ�Ƶ���0��ʼ
	CRV_Move_start_idx(p_run->p_vals_y, p_run->crv_start_index, p_run->crv_num_points);
	
	for(i = 0; i < points; i+= len) {
		//����x0,y0
		p_CRV_line->area.x0 = p_att->crv_x0 +  i * p_att->crv_step_pix;
		idx = i;
		p_CRV_line->area.y0 = *CRV_Get_val_y(crv_fd, idx);
		
		//�������x1,y1������

		len = CRV_Rle(crv_fd, idx);
		
		if(len == 0)
		{
			//˵�����ڵĵ��ǲ�һ����
			len = 1;
			
		}
		//����x1, y1
		p_CRV_line->area.x1 = p_CRV_line->area.x0 + p_att->crv_step_pix * (len);
		idx += len;
		p_CRV_line->area.y1 = *CRV_Get_val_y(crv_fd, idx);
		
		if(p_CRV_line->area.y0 < 50 || p_CRV_line->area.y1 < 50)
			break;
		p_CRV_line->p_gp->vdraw(p_CRV_line->p_gp, &p_CRV_line->cnt, &p_CRV_line->area);
		
		
			
	}
	
	
}

//������������ƫ�ƣ���ʼ��������ȡ���õ����ݵ�λ��
static __inline uint8_t *CRV_Get_val_y(uint8_t crv_fd, short index)
{
	uint16_t		abs_idx;
	
	if(index < 0)
	{
		index += p_CRV_self->p_crv_att[crv_fd].crv_max_num_data;
		
	}
	abs_idx = index + p_CRV_self->p_run_info[crv_fd].crv_start_index;
	abs_idx %= p_CRV_self->p_crv_att[crv_fd].crv_max_num_data;
	
	return p_CRV_self->p_run_info[crv_fd].p_vals_y + abs_idx;
	
}




