#ifndef __APP_H
#define __APP_H

#include "os_cfg.h"

/***********************************
* 头文件
***********************************/

/**********************************
*宏定义
***********************************/
//#define _DEBUG
//#define _DEBUG_BATTERY
/***********************************
* 全局变量
***********************************/

/***********************************
* 類型定義
***********************************/
#define BOOL unsigned char

typedef enum{
	INIT_TASK_ID = 0,
//	TASK_ADC_VALUE_SAMPLE,
//	HONEYWELL_SAMPLING_DATA_TASK_ID,
//	KEY_LED_TASK_ID,
	EMD_PCB_TEST_TASK_ID,
	KEY_CHECK_TASK_ID,
	
	HONEYWELL_SAMPLING_DATA_TASK_ID,
//	TASK_LED_SHOW,
//	TASK_GET_SWITCH_MODE,
//	//KEY_LED_TASK_ID,
//	TASK_OUTPUT_PWM,
////	TASK_THERMAL_CHECK,
	SEND_TASK_ID,
//	//TASK_OUTPUT_PWM,
	RECEIVE_TASK_ID,
//	//KEY_LED_TASK_ID,
//	//TASK_OUTPUT_PWM,
//	EXP_DETECT_SAVE_TASK_ID,
//	EXP_READ_SEND_TASK_ID,
//	TEST_TASK_ID,
//	TASK_MAX_ID
}TASK_ID;


//typedef struct{
//	uint8_t run_status;//
//	
//}CONFIG_TYPE;

typedef struct{
	//类型密码
	u8 tyep_password;
	//运行模式
	u8 run_mode;
	
	//流量 单位10ml
	u16 flow_set;
	
	//氧气浓度
	u8 fio2_high_limit_set;
	u8 fio2_low_limit_set;
	u8 fio2_set;
	
	//音量
	u8 speaker_volume;
	
	//电池电量百分比-电压
	u16 bat_q_low_bit;
	u16 bat_q_junior_low_bit;
	u16 bat_q_junior_high_bit;
	u16 bat_q_high_bit;
	
	//语言类型
	u32 lang_type;
}CONFIG_SAVE_TYPE;


typedef struct{
	//氧气浓度
	u8 fio2_high_limit_set;
	u8 fio2_low_limit_set;
	u8 fio2_set;
	
	//流量 单位10ml
	u16 flow_set;

	//语言类型
	u8 lang_type;
	
	//运行模式
	u8 run_mode;
	//高压模式起始运行标志
	u8 high_mode_start;
	u8 mode_status;
	
	u8 have_option_flow_sensor;

	//音量
	u8 speaker_volume;
		
	//时间
	u32 cur_time;
	
	//电池电量百分比-电压
	u16 bat_q_low_bit;
	u16 bat_q_junior_low_bit;
	u16 bat_q_junior_high_bit;
	u16 bat_q_high_bit;
	
	//24CXX标志
	u8 have_eeprom;
}CONFIG_TYPE;
/***********************************
* 外部函数
***********************************/
void init_task(void);
//void init_system(void);
void init_system(BOOL bWakeUp);
#endif
