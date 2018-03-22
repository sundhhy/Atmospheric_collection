//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef __INC_system_H_
#define __INC_system_H_
#include <stdint.h>
#include "utils/time.h"
#include "utils/PHN_string.h"

//#include "HMI/HMI.h"
//------------------------------------------------------------------------------
// check for correct compilation options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

#define HRD_VER						10		//1Œª–° ˝

//œµÕ≥≈‰÷√¿‡∫Í∂®“Â




#define OP_ADD				0
#define OP_SUB				1
#define OP_MUX				2
#define OP_DIV				3

#define FSH_FM25_NUM			0
#define FSH_W25Q_NUM			1
#define FSH_OPT_SECTOR			0
#define FSH_OPT_BLOCK			1
#define FSH_OPT_CHIP			2
#define NUM_FSH					2
#define FSH_FLAG_READBACK_CHECK		2			//

#define FS_ALARM_LOWSPACE		1

#define	CHG_SYSTEM_CONF				1	
#define	CHG_MODCHN_CONF(n)			(1 << (n + 1))

#define SYSFLAG_SETTING				1
#define SYSFLAG_EFS_NOTREADY		2		//Œƒº˛œµÕ≥Œ¥æÕ–˜£¨ø…ƒ‹’˝‘⁄≤¡≥˝Œƒº˛
#define SYSFLAG_POWEON				4
#define SYSFLAG_POWEROFF			8

#define SYSFLAG_ERR					0x10
#define SYSFLAG_URGENCY				0x20
#define SYSFLAG_READ_CET			0x40		//“™«Û∂¡»°¿‰∂ÀŒ¬∂»

#define SYS_TIME				aci_sys.sys_time
//#define	STSFLAG_TIME_CHANGE		8
//#define	STSFLAG_PARAM_SYSTEM_CHANGE		0x10
//#define	STSFLAG_PARAM_CHANNEL_CHANGE		0x20
//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------
typedef enum {
	es_psd = 0,
	es_rcd_t_s,
	es_brk_cpl,
	es_brk_rss,
	es_cmn_md,
	es_baud,
	es_id,
	es_mdfy_prm,
	es_cold_end_way,
	es_CJC,
	es_vcs,
	es_beep,
}e_system_t;

typedef struct {
	uint8_t		num_chn;
	uint8_t		password[3];
	
	//record_gap_s:sec 0:1 1:2 2:4 3:8 4:12 5:24 6:36 7:60 8:120 9:180 10:240 
	uint8_t		record_gap_s;		
	uint8_t		break_couple;		//∂œ≈º¥¶¿Ì∑Ω Ω:  ºµ„£¨±£≥÷£¨÷’µ„
	uint8_t		break_resistor;		//∂œ◊Ë¥¶¿Ì
	uint8_t		communication_mode;			//“«±Ì”Îpc¡¨Ω”£∫ Õ®—∂£ª “«±Ì”Î¥Ú”°ª˙¡¨Ω”: ¥Ú”°
	
	uint8_t		id;											// 1 - 63
	uint8_t		baud_idx;
	uint8_t		sys_flag;
	uint8_t			cold_end_way;				//0 Õ‚≤ø£¨ 1 …Ë∂®
	uint8_t		CJC;								//¿‰∂À≤π≥• 0-99 Œ™…Ë∂®ƒ£ Ω
	
	
	
	
	
	uint8_t		disable_modify_adjust_paramter;		//Ω˚÷π–ﬁ∏ƒµ˜Ω⁄≤Œ ˝
	uint8_t		disable_view_chn_status;					//Ω˚÷πÕ®µ¿◊¥Ã¨œ‘ æ
	uint8_t		enable_beep;											//∞¥º¸…˘“Ù‘ –Ì
	int 		baud_rate;

}system_conf_t;

//-----------HMI -----------------------------------------------
typedef struct {
	int			hmi_sem;
}hmi_mgr_t;
//---------- flash«˝∂Øµƒ∂®“Â --------------------------------------

typedef struct {
	uint16_t		num_sct;
	uint16_t		num_blk;
	uint32_t		total_pagenum;					///’˚∏ˆ¥Ê¥¢∆˜µƒ“≥ ˝¡ø
	
	uint16_t		page_size;						///“ª“≥µƒ≥§∂»
	
	
	uint8_t			fnf_flag;
	uint8_t			none;
}fsh_info_t;

typedef struct {
	
	fsh_info_t	fnf;
	
//	int (*fsh_init)(void);
	void (*fsh_wp)(int p);
	void (*fsh_info)(fsh_info_t *nf);


	int	(*fsh_ersse)(int opt, uint32_t	num);
	void	(*fsh_ersse_addr)(uint32_t	start_addr, uint32_t size);
//	int	(*fsh_wr_sector)(uint8_t *wr_buf, uint16_t num_sector);
//	int	(*fsh_rd_sector)(uint8_t *rd_buf, uint16_t num_sector);
	int (*fsh_write)(uint8_t *wr_buf, uint32_t wr_addr, uint32_t num_bytes);
	int (*fsh_raw_write)(uint8_t *wr_buf, uint32_t wr_addr, uint32_t num_bytes);
	int (*fsh_read)(uint8_t *wr_buf, uint32_t rd_addr, uint32_t num_bytes);
	int (*fsh_raw_read)(uint8_t *wr_buf, uint32_t rd_addr, uint32_t num_bytes);
	
	void (*fsh_flush)(void);
}flash_t;

//----------------Œƒº˛œµÕ≥µƒ∂®“Â --------------------------------

typedef enum {
	WR_SEEK_SET = 0,
	WR_SEEK_CUR = 1,
	WR_SEEK_END = 2,
	RD_SEEK_SET = 3,
	RD_SEEK_CUR = 4,
	RD_SEEK_END = 5,
	GET_WR_END = 6,
	GET_RD_END = 7,
}lseek_whence_t;
typedef struct {
	uint8_t			fsh_No;				//∂‘”¶µƒ¥Ê¥¢∆˜±‡∫≈
	uint8_t			opt_mode;			//0  ÷ª∂¡  1 ∂¡–¥
	uint8_t			file_flag;
	uint8_t			low_pg;
	
	int				file_sem;
	
	uint16_t		start_page;
	uint16_t		num_page;
	
	uint32_t		file_size;
	uint32_t		read_position;
	uint32_t		write_position;

	char			*p_name;
}file_info_t;

typedef struct {
	uint8_t		num_partitions;
	
	//ø…øø–‘µ»º∂, 0 “ª∞„£¨‘⁄–¥Œƒº˛ ±≤ªªÿ∂¡≈–∂œ 1 ∏ﬂ£¨–¥Œƒº˛ ±£¨“™ªÿ∂¡≈–∂œ
	uint8_t		reliable_level;		
	uint16_t	err_code;
	
	//file_size‘⁄Œƒº˛≤ª¥Ê‘⁄ ±£¨–Ë“™¥¥Ω® ± π”√
	int		(*fs_open)(uint8_t		prt, char *path, char *mode, int	file_size);	
	int		(*fs_close)(int fd);
	int		(*fs_delete)(int fd, char *path);	//fd < 0µƒ ±∫Ú£¨Õ®π˝path¿¥≤È’“Œƒº˛
	int		(*fs_write)(int fd, uint8_t *p, int len);
	int		(*fs_raw_write)(int fd, uint8_t *p, int len);		//÷±Ω”–¥»ÎµΩ”≤º˛£¨≤ª π”√ª∫¥Ê
	int		(*fs_read)(int fd, uint8_t *p, int len);
	int		(*fs_raw_read)(int fd, uint8_t *p, int len);
	int		(*fs_resize)(int fd, char *name, int new_size);
	int 	(*fs_lseek)(int fd, int whence, int32_t offset);
	void 	(*fs_erase_file)(int fd, uint32_t start, uint32_t size);
	void 	(*fs_shutdown)(void);
	file_info_t*		(*fs_file_info)(int fd);
			
	
}fs_t;

//--------------------------------------------------------------------------

typedef struct {
	uint8_t				major_ver;
	uint8_t				minor_ver;
	uint8_t				save_chg_flga;		//ø…¥Ê¥¢µƒ≈‰÷√–≈œ¢µƒ±‰ªØ±Í÷æ
	uint8_t				usb_device;		//0 Œﬁusb…Ë±∏ 1 ”–usb…Ë±
	uint8_t				sys_flag;
	uint8_t				pwr_rcd_index;
	
	
	//œ‘ æœ‡πÿ
	uint16_t			lcd_cmd_bytes;
	uint32_t			lcd_sem_wait_ms;
	
	struct  tm		sys_time;
	
	//Õ®µ¿∞Â◊”…œµƒ–≈œ¢
	uint8_t				DO_val;				//DOµƒ µ ±÷µ
	uint8_t				DO_err;
	uint16_t			code_end_temperature;
		

	
	
	
	//∞¥º¸
	uint16_t				key_weight;
	uint16_t				hit_count;
	
	hmi_mgr_t				hmi_mgr;
	system_conf_t		sys_conf;
	flash_t					arr_fsh[NUM_FSH];
	fs_t						fs;
}system_t;
	

//------------------------------------------------------------------------------
// global variable declarations
//------------------------------------------------------------------------------
extern	char 				*arr_p_vram[16];
extern	uint16_t		time_smp;			//œ¬¥Œº«¬ºµƒ ±º‰
extern 	char				g_setting_chn;
extern	char				flush_flag;

extern system_t			aci_sys;
//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------
int	Operate_in_tange(int	arg1, int op, int arg2, int rangel, int rangeh);

void System_init(void);
void System_power_off(void);

void 			System_time(struct  tm *stime);
uint32_t  SYS_time_sec(void);
int  			Str_time_2_tm(char *s_time, struct  tm	*time);
uint32_t  Str_time_2_u32(char *s_time);
uint32_t  Time_2_u32(struct  tm	*tm_2_sec);
int 			Sec_2_tm(uint32_t seconds, struct  tm *time);
int 			System_set_time(struct  tm *stime);


extern void System_default(void);





int SYS_Commit(void);
#endif
