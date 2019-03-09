
/**
********************************************************************************
* ��ࣺ
* ģ�����ƣ�app.c
* ģ�鹦�ܣ�
* �������ڣ�
* �� �� �ߣ�
* ��    ����
********************************************************************************
**/

/***********************************
* ͷ�ļ�
***********************************/
#include "app.h"
#include "datatype.h"
#include "comm_task.h"
//#include "CMD_Receive.h"
//#include "serial_port.h"
#include "hardware.h"
//#include "Motor_pwm.h"
//#include "fifo.h"
//#include "protocol_module.h"
//#include "stm32f0xx_rtc.h"
//#include "key_led_task.h"
////#include "exp_task.h" 
//#include "stm32f0xx_usart.h"
////#include "store_fifo.h"
//#include "delay.h"
#include "honeywell_sampling_data.h"
#include "emd_pcb_test_task.h"
#include "key_check_task.h"
/**********************************
*�궨��
***********************************/

/***********************************
* ȫ�ֱ���
***********************************/

/***********************************
* �ֲ�����
***********************************/

/***********************************
* ��������
***********************************/

/***********************************
* ��������
***********************************/



//��ʼ������
void init_task(void)
{
	//��ʼ��Ӳ��
	init_hardware();	
	
	os_create_task(TaskDataSend, OS_TRUE, SEND_TASK_ID);
	os_create_task(CMD_ProcessTask, OS_TRUE, RECEIVE_TASK_ID);

	os_create_task(key_check_task, OS_TRUE, KEY_CHECK_TASK_ID);
	os_create_task(EMD_PCB_test_task, OS_TRUE, EMD_PCB_TEST_TASK_ID);
	os_create_task(honeywell_sampling_data,OS_TRUE,HONEYWELL_SAMPLING_DATA_TASK_ID);

	os_pend_task(INIT_TASK_ID);
}


