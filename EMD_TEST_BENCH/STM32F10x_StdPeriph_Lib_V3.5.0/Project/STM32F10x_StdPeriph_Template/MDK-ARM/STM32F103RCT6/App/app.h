#ifndef __APP_H
#define __APP_H

#include "os_cfg.h"

/***********************************
* ͷ�ļ�
***********************************/

/**********************************
*�궨��
***********************************/
//#define _DEBUG
//#define _DEBUG_BATTERY
/***********************************
* ȫ�ֱ���
***********************************/

/***********************************
* ��Ͷ��x
***********************************/
#define BOOL unsigned char

typedef enum{
	INIT_TASK_ID = 0,
//	TASK_ADC_VALUE_SAMPLE,
//	HONEYWELL_SAMPLING_DATA_TASK_ID,
	KEY_LED_TASK_ID,
	HONEYWELL_SAMPLING_DATA_TASK_ID,
	TASK_LED_SHOW,
	TASK_GET_SWITCH_MODE,
	//KEY_LED_TASK_ID,
	TASK_OUTPUT_PWM,
//	TASK_THERMAL_CHECK,
	SEND_TASK_ID,
	//TASK_OUTPUT_PWM,
	RECEIVE_TASK_ID,
	//KEY_LED_TASK_ID,
	//TASK_OUTPUT_PWM,
	EXP_DETECT_SAVE_TASK_ID,
	EXP_READ_SEND_TASK_ID,
	TEST_TASK_ID,
	TASK_MAX_ID
}TASK_ID;


typedef struct{
	uint8_t run_status;//
	
}CONFIG_TYPE;
/***********************************
* �ⲿ����
***********************************/
void init_task(void);
//void init_system(void);
void init_system(BOOL bWakeUp);
#endif
