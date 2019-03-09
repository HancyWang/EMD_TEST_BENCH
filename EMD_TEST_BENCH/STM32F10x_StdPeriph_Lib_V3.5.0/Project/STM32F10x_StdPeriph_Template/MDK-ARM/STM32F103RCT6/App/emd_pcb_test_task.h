#ifndef __EMD_PCB_TEST_TASK_H_
#define __EMD_PCB_TEST_TASK_H_
/***********************************
* 头文件
***********************************/

/**********************************
*宏定义
***********************************/
#define EMD_PCB_TEST_PERIOD  10

/***********************************
* 全局变量
***********************************/
typedef enum
{
	EMD_CHECK_NONE,
	EMD_CHECK_START,                     						//start checking 
	EMD_CHECK_SLEEP_CURRENT,												//check EMD sleep current
	EMD_CHECK_SW_VERSION,     											//check software version,the current ver. is 1.1.0       
	EMD_CHECK_PARA,																	//check parameters
	EMD_CHECK_DC_DC,																//check DC/DC
	EMD_CHECK_MOTORS,																//check motors
	EMD_CHECK_GET_PRESSURE_ZERO_POINT,              //get zero point from EMD PCBA，this is very important
	EMD_CHECK_08mmHg,																//check 0.8mmHg
	EMD_CHECK_10mmHg,																//check 10mmHg
	EMD_CHECK_KEY_PRESSED_AFTER_10mmHg,             //After reach 10mmHg,check key pressed
	EMD_CHECK_BAT_GREATER_THAN_2_6V,     						//check battery voltage >2.6V, UI suppose to be "LED green solid and vibration 0.5s"
	EMD_CHECK_BAT_BETWEEN_2_45_AND_2_6V,						//check battery voltage between 2.45V and 2.6V,UI suppose to be "LED fast flash x5 times,
																									//and then turn green solid and vibration 0.5s"
	EMD_CHECK_BAT_LESS_THAN_2_45V,									//check battery voltage <2.45V 
	EMD_CHECK_POWER_ON_KEY,
	EMD_CHECK_MODE_KEY,
	EMD_CHECK_FINISHED,
	EMD_CHECK_FAIL
}EMD_PCB_TEST_STATUS;


/***********************************
* Function
***********************************/
void EMD_PCB_test_task(void);
#endif
