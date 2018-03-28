/*
 * hd_conf_def.h
 *
 *  Created on: 2017-10-10
 *      Author: Administrator
 */

#ifndef HD_CONF_DEF_H_
#define HD_CONF_DEF_H_
#include <stdint.h>
#include <stdbool.h>
typedef struct gpmc_config {
	uint32_t	base_address;		//���ʵ������ڴ�ĵ�ַ
	uint32_t	mask_address;		//GPMC���ڴ��ַ����
	uint32_t	mmap_size;			//ӳ����ڴ泤��
	// GPMC_CONFIG1_n
	uint8_t		chip_instance;
	char		device_size;
	char		device_type;
	char		mux_add_data;

	char		read_multiple;
	char		read_type;
	char		write_multiple;
	char		write_type;

	char 		waitpin;			///< wait����ѡ��
	bool		rd_monitoring;
	bool		wr_monitoring;
	char		rese;

	void		*cmmTim;
	void		*rdTim;
	void		*wrTim;

}gpmc_chip_cfg;


typedef struct gpio_config {

	//GPIOg_n
	uint8_t		pin_group;			//�������
	uint8_t		pin_number;			//�����������
	uint8_t		intr_type;
	uint8_t		intr_line;		//AM335x��GPIO�ж��������ж���
	uint8_t		intrPrio;			//�ж����ȼ�
	uint8_t		debou_time;

	uint16_t	pin_ctrl_off;
	uint8_t		clk_init;
	uint8_t		none[3];

}gpio_cfg;

#endif /* HD_CONF_DEF_H_ */
