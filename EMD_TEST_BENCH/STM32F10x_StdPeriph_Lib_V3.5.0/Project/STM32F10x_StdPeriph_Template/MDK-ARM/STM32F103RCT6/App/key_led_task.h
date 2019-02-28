#ifndef __KEY_TASK_H_
#define __KEY_TASK_H_
/***********************************
* ͷ�ļ�
***********************************/

/**********************************
*�궨��
***********************************/
#define KEY_LED_PERIOD  1000
#define CHECK_MODE_OUTPUT_PWM 10
#define ONE_SEC_KEY_TIME   1000/KEY_LED_PERIOD
/***********************************
* ȫ�ֱ���
***********************************/
typedef enum
{
	POWER_ON,
	POWER_OFF
}MCU_STATE;

typedef enum
{
	KEY_STOP_MODE,
	KEY_UPING,
	KEY_DOWNING,
//	KEY_DOWN_UP,
	KEY_WAKE_UP
	//KEY_UP_DOWN
}KEY_STATE;

typedef enum
{
	LED_ON,
	LED_OFF,
	LED_NONE,
	LED_RED_SOLID,
	LED_RED_FLASH,
	LED_GREEN_SOLID
}LED_STATE;

/***********************************
* ��Ͷ��x
***********************************/

/***********************************
* �ⲿ����
***********************************/
void key_led_task(void);
//void EnterStopMode(void);
//void CfgPA0ASWFI(void);
//void CfgALLPins4StopMode(void);
//void init_system_afterWakeUp(void);
////extern INT8U I2C_RecByte(void);
////LED_STATE Check_Bat(unsigned short int diff_voltage);
//LED_STATE Check_Bat(void);


#endif
