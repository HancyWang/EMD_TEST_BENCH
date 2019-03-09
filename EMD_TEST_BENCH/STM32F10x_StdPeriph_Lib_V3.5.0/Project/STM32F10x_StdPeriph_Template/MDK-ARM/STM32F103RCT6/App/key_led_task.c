

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
#include "display_module.h"
#include "lcd.h"

#include "i2c.h"
#include "Motor_pwm.h"
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
#include "honeywell_sampling_data.h"
/**********************************
*宏定义
***********************************/
#define BATTERY_NO_POWER_THRESHOLD 		3276  //3345对应2.45V  3303对应2.2V  3276 2.4v
#define BATTERY_LOW_POWER_THRESHOLD		3549  //3549对应2.6V
//#define BATTERY_DIFF_VOLTAGE_NO_POWE 	819
//#define BATTERY_DIFF_VOLTAGE_LOW_POWE 682

#define BACK_ROW_COORD         47 
#define BACK_COLUMN_COORD				20

#define EEPROM_TITLE_ROW_COORD      190
#define EEPROM_TITLE_COLUMN_COORD   100

#define EEPROM_WARNING_ROW_COORD      150
#define EEPROM_WARNING_COLUMN_COORD   20

#define EEPROM_YES_ROW_COORD      BACK_ROW_COORD
#define EEPROM_YES_COLUMN_COORD   BACK_COLUMN_COORD

#define EEPROM_NO_ROW_COORD      EEPROM_YES_ROW_COORD
#define EEPROM_NO_COLUMN_COORD   265
/******************************************************
*内部类型定义
******************************************************/
typedef enum{
	EEPROM_START_ITEM = 0,
	EEPROM_YES_ITEM,
	EEPROM_NO_ITEM,
	EEPROM_END_ITEM
}EEPROM_ITEM_TYPE;
/***********************************
* 全局变量
***********************************/

/***********************************
* 局部变量
***********************************/
extern __IO uint16_t ADCConvertedValue[3];
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
//LED_STATE led_state=LED_ON;
int test=0;
BOOL b_flag=0;

void key_led_task(void)
{
	//功能验证
//	Set_V1(0);
//	Set_V2(0);
//	
//	Set_JDQ1(1);
//	Set_JDQ2(0);
//	
//	Motor_PWM_Freq_Dudy_Set(1,100,50);
	
	
	
	set_lcd_backlight(1);
////	display_module_show_char(220,255+8,16,0%100/10+'0',0,BLUE);
////	LCD_CtrlWrite_Color(RED);
////	lcd_draw_rect_real(0,0,63,239,BLUE);
	lcd_draw_rect_real(0,0,239,319,BACK_COLOR);
//	lcd_draw_rect_real(0,0,239,319,WHITE);
//	display_module_show_char(0,0,24,'A',0,WHITE);
//	display_module_show_char(0+16,0,24,'B',0,WHITE);
	display_module_show_string(15, 20,24,"Sensor:",0,WHITE);
	display_module_show_string(150, 20,24,"TESTING",0,YELLOW);
	for(int i=0;i<10;i++)
	{
		delay_ms(1000);
	}
	display_module_show_string(150, 20,24,"PASS   ",0,GREEN);

//	display_module_clear();
//	display_module_show_lang(181,50,24,0,WHITE);
//	
//	for(int i=0;i<10;i++)
//	{
//		Set_BELL(1);
//	delay_us(1000000);
//	Set_BELL(0);
//	delay_us(1000000);
//	}
//	
	
//	b_flag=!b_flag;
//	if(b_flag)
//	{
//		Set_LED(0);
//		Set_BELL(0);
////		display_module_show_lang(EEPROM_NO_ROW_COORD, EEPROM_NO_COLUMN_COORD,24, NO_INDEX, WHITE);
////		display_module_show_lang(150 - 27 + 3,40+12*3,24, HAVE_NO_ALARM_INDEX, WHITE);
////		display_module_show_lang(186, 81,24, TIME_SET_INDEX, WHITE);
//		Motor_PWM_Freq_Dudy_Set(1,100,50);
//		set_lcd_backlight(1);
//		Init_honeywell_sensor();
//	}
//	else
//	{
//		Set_LED(1);
//		Set_BELL(1);
////		set_lcd_backlight(0);
//		set_lcd_backlight(0);
//		Motor_PWM_Freq_Dudy_Set(1,100,0);
//	}
	

	//IWDG_Feed();   //喂狗
	os_delay_ms(KEY_LED_TASK_ID, KEY_LED_PERIOD);
}
