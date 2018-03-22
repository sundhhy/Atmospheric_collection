#ifndef __INC_hmi_conf_H__
#define __INC_hmi_conf_H__



#define HMI_NUM_P_SHT							8		//sheet指针池
#define HMI_NUM_P_CHOICE_SHT			8		//可座位选项被选中的sheet池
#define BARHMI_NUM_BARS						2
#define CURVE_POINT								240			//曲线点数最多240点,但是数据的长度要加上1个起始点

#define SETTING_ROW_MAX						3				//设置画面的最多列数
#define SETTING_COL_MAX						2	

#define SHT_ID_PRIVATE		0x40			//各界面可以分配的图层ID的下限
#endif
