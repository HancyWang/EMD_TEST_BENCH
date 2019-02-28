

/**
********************************************************************************
* 版權：
* 模块名称：key_led_task.c
* 模块功能：
* 创建日期：
* 创 建 者：
* 声    明：
********************************************************************************
**/

/***********************************
* 头文件
***********************************/

#include "app.h"
#include "datatype.h"
#include "hardware.h"
#include "fifo.h"
#include "key_led_task.h"
#include "protocol_module.h"

//#include "i2c.h"
//#include "Motor_pwm.h"
//#include "stm32f0xx_pwr.h"
//#include "stm32f0xx_exti.h"
//#include "stm32f0xx_syscfg.h"
//#include "stm32f0xx_dma.h"
//#include "serial_port.h"
//#include "CMD_receive.h"
//#include "app.h"
#include "delay.h"
//#include "comm_task.h"
//#include "iwtdg.h"
//#include "honeywell_sampling_data.h"
/**********************************
*宏定义
***********************************/
#define BATTERY_NO_POWER_THRESHOLD 		3276  //3345对应2.45V  3303对应2.2V  3276 2.4v
#define BATTERY_LOW_POWER_THRESHOLD		3549  //3549对应2.6V
//#define BATTERY_DIFF_VOLTAGE_NO_POWE 	819
//#define BATTERY_DIFF_VOLTAGE_LOW_POWE 682
/***********************************
* 全局变量
***********************************/

/***********************************
* 局部变量
***********************************/
//extern HONEYWELL_STATE honeywell_state;

//extern uint32_t os_ticks;
//extern CMD_Receive g_CmdReceive;  // 命令接收控制对象

//extern FIFO_TYPE send_fifo;
//extern uint8_t send_buf[SEND_BUF_LEN];
//extern CHCKMODE_OUTPUT_PWM state;

//extern PWM_STATE pwm1_state;
//extern PWM_STATE pwm2_state;
//extern PWM_STATE pwm3_state;

//extern uint8_t led_high_cnt;
//extern uint8_t led_low_cnt;
//extern uint8_t vabirate_cnt;
//extern BOOL b_vabirate;
//extern BOOL b_check_BAT_ok;
////KEY值，这里点按为确认蓝牙连接
//typedef enum {
//	NO_KEY,
//	BLUE_CHECK
//}KEY_VAL;

//MCU_STATE mcu_state=POWER_OFF;
////mcu_state=POWER_OFF;

////extern uint8_t OUTPUT_FINISH;
//BOOL b_Is_PCB_PowerOn=FALSE;
////BOOL b_check_bat=FALSE;
//volatile KEY_STATE key_state=KEY_UPING;

//extern uint16_t RegularConvData_Tab[2];
//extern uint8_t adc_state;
////extern THERMAL_STATE thermal_state;

//static uint8_t wakeup_Cnt;

//LED_STATE led_state=LED_NONE;

//extern uint8_t prev_mode;

//extern BOOL b_getHoneywellZeroPoint;
//extern uint32_t adc_pressure_value;
//extern uint32_t HONEYWELL_ZERO_POINT;

//extern uint16_t checkPressAgain_cnt;
/***********************************
* 局部函数
***********************************/

/*******************************************************************************
** 函数名称: EnterStopMode
** 功能描述: 进入低功耗模式
** 输　  入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
*******************************************************************************/
void CfgPA0ASWFI()
{
//	//时钟使能
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOF,ENABLE);  
//	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); 
//	
//	
//	//外部按键GPIOA初始化,PA0  
//	GPIO_InitTypeDef GPIO_InitStructure;  
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;  
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;  
//	//GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;  
//	GPIO_Init(GPIOA,&GPIO_InitStructure);  

//	//将EXTI0指向PA0  
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);  
//	//EXTI0中断线配置
//	EXTI_InitTypeDef EXTI_InitStructure;  
//	EXTI_InitStructure.EXTI_Line=EXTI_Line0;  
//	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;  
//	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;  
//	EXTI_InitStructure.EXTI_LineCmd=ENABLE;  
//	EXTI_Init(&EXTI_InitStructure);  

//	//EXTI0中断向量配置  
//	NVIC_InitTypeDef NVIC_InitStructure;  
//	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_1_IRQn;  
//	NVIC_InitStructure.NVIC_IRQChannelPriority=0x01;  
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  
//	NVIC_Init(&NVIC_InitStructure);  
}


//LED_STATE led_state=LED_ON;
int test=0;
BOOL b_flag=0;

void key_led_task(void)
{
//	if(led_state==LED_OFF)
//	{
//		if(test==50)
//		{
//			test=0;
//			led_state=LED_ON;
//		}
//		else
//		{
//			test++;
//			Set_LED(0);
//		}
//	}
	
	b_flag=!b_flag;
	if(b_flag)
	{
		Set_LED(0);
	}
	else
	{
		Set_LED(1);
	}
	

	//IWDG_Feed();   //喂狗
	os_delay_ms(KEY_LED_TASK_ID, KEY_LED_PERIOD);
}
