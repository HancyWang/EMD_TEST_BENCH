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
	//��������
	u8 tyep_password;
	//����ģʽ
	u8 run_mode;
	
	//���� ��λ10ml
	u16 flow_set;
	
	//����Ũ��
	u8 fio2_high_limit_set;
	u8 fio2_low_limit_set;
	u8 fio2_set;
	
	//����
	u8 speaker_volume;
	
	//��ص����ٷֱ�-��ѹ
	u16 bat_q_low_bit;
	u16 bat_q_junior_low_bit;
	u16 bat_q_junior_high_bit;
	u16 bat_q_high_bit;
	
	//��������
	u32 lang_type;
}CONFIG_SAVE_TYPE;


typedef struct{
	//����Ũ��
	u8 fio2_high_limit_set;
	u8 fio2_low_limit_set;
	u8 fio2_set;
	
	//���� ��λ10ml
	u16 flow_set;

	//��������
	u8 lang_type;
	
	//����ģʽ
	u8 run_mode;
	//��ѹģʽ��ʼ���б�־
	u8 high_mode_start;
	u8 mode_status;
	
	u8 have_option_flow_sensor;

	//����
	u8 speaker_volume;
		
	//ʱ��
	u32 cur_time;
	
	//��ص����ٷֱ�-��ѹ
	u16 bat_q_low_bit;
	u16 bat_q_junior_low_bit;
	u16 bat_q_junior_high_bit;
	u16 bat_q_high_bit;
	
	//24CXX��־
	u8 have_eeprom;
}CONFIG_TYPE;
/***********************************
* �ⲿ����
***********************************/
void init_task(void);
//void init_system(void);
void init_system(BOOL bWakeUp);
#endif
