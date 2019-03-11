/**
********************************************************************************
* 版權：
* 模块名称：hardware.c
* 模块功能：
* 创建日期：
* 创 建 者：
* 声    明：
********************************************************************************
**/
#ifndef __HARDWARE_
#define __HARDWARE_

/***********************************
* 头文件
***********************************/
#include <stdint.h>
#include "datatype.h"
#include "stm32f10x.h"
/**********************************
*宏定义
***********************************/

//#define INTERRUPT_3_KEYS

//LED PORT and PIN
#define LED_PIN										GPIO_Pin_0
#define LED_PORT									GPIOB

//BELL PORT and PIN
#define BELL_PIN									GPIO_Pin_1
#define BELL_PORT									GPIOB

//STOP PORT and PIN
#define STOP_PIN									GPIO_Pin_4
#define STOP_PORT									GPIOA

//START PORT and PIN
#define START_PIN									GPIO_Pin_5
#define START_PORT								GPIOA

//NEXT PORT and PIN
#define NEXT_PIN									GPIO_Pin_6
#define NEXT_PORT									GPIOA

//VALVE PORT and PIN
#define VALVE_PIN									GPIO_Pin_8
#define VALVE_PORT								GPIOA

//ADC1
#define ADC1_MOTOR_CURRENT1_PIN		GPIO_Pin_0
#define ADC1_MOTOR_CURRENT1_PORT	GPIOA
#define ADC1_MOTOR_CURRENT2_PIN		GPIO_Pin_1
#define ADC1_MOTOR_CURRENT2_PORT	GPIOA
#define ADC1_VDD_3_0_PIN					GPIO_Pin_7
#define ADC1_VDD_3_0_PORT					GPIOA

//V1,V2
#define V1_PIN										GPIO_Pin_11
#define V1_PORT										GPIOA
#define V2_PIN										GPIO_Pin_12
#define V2_PORT										GPIOA

//Error LED
#define ERROR_LED_PIN							GPIO_Pin_7
#define ERROR_LED_PORT						GPIOB

//LCD
#define LCD_BLEN_PIN							GPIO_Pin_10
#define LCD_BLEN_PORT							GPIOB
#define LCD_RD_PIN								GPIO_Pin_11
#define LCD_RD_PORT								GPIOB
#define LCD_WR_PIN								GPIO_Pin_12
#define LCD_WR_PORT								GPIOB
#define LCD_DC_PIN								GPIO_Pin_13
#define LCD_DC_PORT								GPIOB
#define LCD_CS_PIN								GPIO_Pin_14
#define LCD_CS_PORT								GPIOB
#define LCD_RESET_PIN							GPIO_Pin_15
#define LCD_RESET_PORT						GPIOB

#define LCD_D0_PIN								GPIO_Pin_0
#define LCD_D0_PORT								GPIOC
#define LCD_D1_PIN								GPIO_Pin_1
#define LCD_D1_PORT								GPIOC
#define LCD_D2_PIN								GPIO_Pin_2
#define LCD_D2_PORT								GPIOC
#define LCD_D3_PIN								GPIO_Pin_3
#define LCD_D3_PORT								GPIOC
#define LCD_D4_PIN								GPIO_Pin_4
#define LCD_D4_PORT								GPIOC
#define LCD_D5_PIN								GPIO_Pin_5
#define LCD_D5_PORT								GPIOC
#define LCD_D6_PIN								GPIO_Pin_6
#define LCD_D6_PORT								GPIOC
#define LCD_D7_PIN								GPIO_Pin_7
#define LCD_D7_PORT								GPIOC

//JDQ_1,JDQ_2,JDQ_3
#define JDQ1_PIN									GPIO_Pin_8
#define JDQ1_PORT									GPIOC
#define JDQ2_PIN									GPIO_Pin_9
#define JDQ2_PORT									GPIOC
#define JDQ3_PIN									GPIO_Pin_10
#define JDQ3_PORT									GPIOC

//START KEY
#define START_KEY_PIN							GPIO_Pin_11
#define START_KEY_PORT						GPIOC

//MODE KEY
#define MODE_KEY_PIN							GPIO_Pin_12
#define MODE_KEY_PORT							GPIOC

//PUMP
#define PUMP_PIN									GPIO_Pin_6
#define PUMP_PORT									GPIOB
#define PWM_PUMP									1

//I2C
#define I2C_SDA_PIN								GPIO_Pin_8
#define I2C_SDA_PORT							GPIOB
#define I2C_SCL_PIN								GPIO_Pin_9
#define I2C_SCL_PORT							GPIOB
#define RST_IO_PIN								GPIO_Pin_15
#define RST_IO_PORT								GPIOA


#define ADC1_DR_Address    ((uint32_t)0x4001244C)


typedef enum
{
	ENUM_PIN_LED,
	ENUM_PIN_BELL,
	ENUM_PIN_START,
	ENUM_PIN_STOP,
	ENUM_PIN_JDQ1,
	ENUM_PIN_JDQ2,
	ENUM_PIN_JDQ3,
	ENUM_PIN_START_KEY,
	ENUM_PIN_MODE_KEY,
	ENUM_PIN_V1,
	ENUM_PIN_V2,
	ENUM_PIN_VALVE
}PIN_NAME;

/***********************************
* 全局变量
***********************************/

/***********************************
* 变量定义
***********************************/
//typedef enum{
//	LED_CLOSE,
//	LED_GREEN,
//	LED_RED,
//	LED_BLUE
//}LED_COLOR;

////时间结构体
//typedef struct 
//{
//		//公历日月年周
//	uint16_t w_year;
//	uint8_t  w_month;
//	uint8_t  w_date;
//	
//	uint8_t hour;
//	uint8_t min;
//	uint8_t sec;			 
//}_calendar_obj;
/***********************************
* 外部函数
***********************************/
void init_hardware(void);
//void init_hardware_byWakeUpOrNot(BOOL bWakeUp);
void init_rtc(void);
//void init_tim(void);
//void convert_rtc(_calendar_obj* calendar, uint32_t rtc);
void set_rtc(uint32_t rtc);
//void set_led(LED_COLOR color);
uint32_t get_rtc(void);
BOOL get_exp_status(void);
BOOL get_key_status(void);
BOOL save_one_page_to_flash(uint32_t Address, uint8_t* buf, uint16_t len);
void read_one_page_from_flash(uint32_t Address, uint8_t* buf, uint16_t len);
BOOL save_half_word_to_flash(uint32_t Address, uint16_t data);
void read_half_word_from_flash(uint32_t Address, uint16_t* pdata);
BOOL save_half_word_buf_to_eeprom(uint32_t Address, uint16_t* buf, uint16_t len);
void read_half_word_buf_from_eeprom(uint32_t Address, uint16_t* buf, uint16_t len);
uint8_t get_bat_vol_per(void);


//void Init_LED(void);

//void Init_PWRSAVE(void);

////配置按键key_wakeup
//void Key_WakeUp_Init(void);

//////初始化flash中的参数
////void Init_parameter_in_Flash(void);

////ADC
//void ADC1_Init(void);
////uint16_t Adc_Switch(uint32_t ADC_Channel);

////I2C
//void ADS115_Init(void);


//void Calibrate_pressure_sensor(int16_t* p_zeroPoint);


//Init all hardware
void init_hardware(void);

//LED
void Init_LED(void);
void Set_LED(BOOL b_open);

//BELL
void Init_BELL(void);
void Set_BELL(BOOL b_open);

//STOP
void Init_STOP(void);
//START
void Init_START(void);
//NEXT
void Init_NEXT(void);

//TIM2, using for delay
void init_tim(void);

//ADC1
void Init_ADC1(void);

//V1,V2
void Init_V1_V2(void);

//ERROR_LED
void Init_Error_LED(void);

//LCD
void Init_LCD(void);

//JDQ
void Init_JDQ(void);

//START KEY
void Init_START_KEY(void);

//MODE KEY
void Init_MODE_KEY(void);

//USART2,communicate with PCBA
void Init_UART_Comm_with_PCBA(void);

void Init_I2C(void);
void Reset_I2C(void);

//void Set_V1(BOOL b_open);
//void Set_V2(BOOL b_open);
//void Set_JDQ1(BOOL b_open);
//void Set_JDQ2(BOOL b_open);
//void Set_JDQ3(BOOL b_open);
void Set_PIN(PIN_NAME name,BOOL b_open);
#endif
