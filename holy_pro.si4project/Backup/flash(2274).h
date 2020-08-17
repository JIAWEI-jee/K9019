#ifndef __FLASH_H__
#define __FLASH_H__
#include "HC89F303.h"

#define FLASH_CHECK_ADDRESS   0X3C00 //��flashĩβ1K(��16K)��ַ��Ϊ�洢����ַ 1024*14 = 0X3800   
#define FLASH_DATA_ADDRESS	FLASH_CHECK_ADDRESS + 4

#define Correct_Value  1
typedef enum TIMER_TYPE
{
	TIMER_ON	= 0X00,
	TIMER_0_5H 	= 0X01,
	TIMER_1H	= 0X02,
	TIMER_2H	= 0X03,
	TIMER_4H	= 0X04,
	TIMER_8H	= 0X05
};

typedef enum GAP_TYPE
{
	GAP_WARM	= 0X00,
	GAP_1 	= 0X01,
	GAP_2	= 0X02,
	GAP_3	= 0X03,
	GAP_4	= 0X04,
	GAP_5	= 0X05,
	GAP_6	= 0X06,
	GAP_7	= 0X07,
	GAP_8	= 0X08,
	GAP_9	= 0X09,

};

typedef enum GAP_TEMP
{
	GAP_WARM_temp	= 40,
	GAP_1_temp 	= 50,
	GAP_2_temp	= 55,
	GAP_3_temp	= 60,
	GAP_4_temp	= 63,
	GAP_5_temp	= 66,
	GAP_6_temp	= 70,
	GAP_7_temp	= 75,
	GAP_8_temp	= 77,
	GAP_9_temp	= 78,

};
	typedef enum corrected_value
	{
		corrected_value_warm_temp	= 2,
		corrected_value_GAP_1_temp	= 2,
		corrected_value_GAP_2_temp	= 2,
		corrected_value_GAP_3_temp	= 2,
		corrected_value_GAP_4_temp	= 2,
		corrected_value_GAP_5_temp	= 2,
		corrected_value_GAP_6_temp	= 2,
		corrected_value_GAP_7_temp	= 2,
		corrected_value_GAP_8_temp	= 2,
		corrected_value_GAP_9_temp	= 6,
	
	};

typedef enum DEVICE_STATE
{
	ON	= 0X00,
	OFF = 0X01
};


typedef struct _flash_info_t
{
	u8 gap;
	u8 timer;
} flash_info_t;

extern flash_info_t flash_info;
//extern  u8 device_state;

void flash_init ( void );
void flah_save_data ( void );

u8 get_device_state ( void );
void set_device_state ( u8 state );


#endif
