

/**
********************************************************************************
* ��ࣺ
* ģ�����ƣ�emd_pcb_test_task.c
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
#include "hardware.h"
#include "fifo.h"
#include "emd_pcb_test_task.h"
#include "protocol_module.h"
#include "display_module.h"
#include "lcd.h"

#include "i2c.h"
#include "Motor_pwm.h"

#include "delay.h"
#include "comm_task.h"
//#include "iwtdg.h"
#include "honeywell_sampling_data.h"
#include "key_check_task.h"
/**********************************
*�궨��
***********************************/
#define TIMEOUT_SW_VERSION    1
#define TIMEOUT_PARA					2
#define TIMEOUT_ZERO_POINT		3
#define TIMEOUT_08mmHg				4
#define TIMEOUT_10mmHg				5

#define ITEM_SLEEP_CURRENT					0
#define ITEM_DC_DC									1
#define ITEM_MOTORS									2
#define ITEM_SW_VERSION							3
#define ITEM_PARA										4
#define ITEM_08mmHg									5
#define ITEM_10mmHg									6
#define ITEM_Vb_2_6V								7
#define ITEM_Vb_2_45_AND_2_6V				8
#define ITEM_Vb_2_45V								9
#define ITEM_POWER_ON_KEY						10
#define ITEM_MODE_KEY								11


/******************************************************
*�ڲ����Ͷ���
******************************************************/
typedef enum
{
	PROMT_CHECK_FAIL,
	PROMT_CHECK_SLEEP_CURRENT,
	PROMT_CHECK_SLEEP_CURRENT_Y,
	PROMT_CHECK_SLEEP_CURRENT_N,
	PROMT_CHECK_MOTORS,
	PROMT_CHECK_MOTORS_Y,
	PROMT_CHECK_MOTORS_N,
	PROMT_CHECK_SW_VERSION,
	PROMT_CHECK_SW_VERSION_Y,
	PROMT_CHECK_SW_VERSION_N,
	PROMT_CHECK_PARA,
	PROMT_CHECK_PARA_Y,
	PROMT_CHECK_PARA_N,
	PROMT_CHECK_DC_DC,
	PROMT_CHECK_DC_DC_Y,
	PROMT_CHECK_DC_DC_N,
	PROMT_CHECK_08mmHg,
	PROMT_CHECK_08mmHg_Y,
	PROMT_CHECK_08mmHg_N,
	PROMT_CHECK_10mmHg,
	PROMT_CHECK_10mmHg_Y,
	PROMT_CHECK_10mmHg_N,
	PROMT_CHECK_GREATER_THAN_2_6V,
	PROMT_CHECK_GREATER_THAN_2_6V_Y,
	PROMT_CHECK_GREATER_THAN_2_6V_N,
	PROMT_CHECK_BETWEEN_2_45_AND_2_6V,
	PROMT_CHECK_BETWEEN_2_45_AND_2_6V_Y,
	PROMT_CHECK_BETWEEN_2_45_AND_2_6V_N,
	PROMT_CHECK_LESS_THAN_2_45V,
	PROMT_CHECK_LESS_THAN_2_45V_Y,
	PROMT_CHECK_LESS_THAN_2_45V_N,
	PROMT_CHECK_POWER_ON_KEY,
	PROMT_CHECK_POWER_ON_KEY_Y,
	PROMT_CHECK_POWER_ON_KEY_N,
	PROMT_CHECK_MODE_KEY,
	PROMT_CHECK_MODE_KEY_Y,
	PROMT_CHECK_MODE_KEY_N,
	PROMT_CHECK_FINISH
}PROMT_INFO;

typedef enum
{
	KEY_START,
	KEY_NEXT,
	KEY_STOP
}KEY_NUM;

typedef enum
{
	EMD_OP_START_TEST_SLEEP_CURRENT,  		   //open sleep current test
	EMD_OP_STOP_TEST_SLEEP_CURRENT,					//close sleep current test
	EMD_OP_SUPPLY_MOTORS_POWER,   					//supply motors power
	EMD_OP_CUTOFF_MOTORS_POWER,   					//cut off motors power supply
	EMD_OP_SUPPLY_3_0_V,   									//supply 3.0V
	EMD_OP_SUPPLY_2_5_V,										//supply 2.5V
	EMD_OP_SUPPLY_2_2_V,										//supply 2.2V
	EMD_OP_CUTOFF_POWER_SUPPLY,    					//cut off power supply
	EMD_OP_PRESS_POWER_BUTTON,
	EMD_OP_PRESS_MODE_BUTTON,
	EMD_OP_OPEN_VALVE,
	EMD_OP_CLOSE_VALVE
}EMD_PCB_OPERATE;

/***********************************
* ȫ�ֱ���
***********************************/
BOOL SW_VERSION_timing_flag=TRUE;
uint32_t prev_SW_VERSION_os_tick;
uint8_t  SW_VERSION_fail_cnt;

BOOL PARA_timing_flag=TRUE;
uint32_t prev_PARA_os_tick;
uint8_t  PARA_fail_cnt;

BOOL zero_point_timing_flag=TRUE;
uint32_t prev_zero_point_os_tick;
uint8_t  zero_point_fail_cnt;

BOOL _08mmHg_timing_flag=TRUE;
uint32_t prev_08mmHg_os_tick;

BOOL _10mmHg_timing_flag=TRUE;
uint32_t prev_10mmHg_os_tick;

EMD_PCB_TEST_STATUS EMD_check_status=EMD_CHECK_START;
//EMD_PCB_TEST_STATUS EMD_check_status=EMD_CHECK_NONE;

BOOL b_get_sw_version_success=FALSE;
BOOL b_get_para_success=FALSE;
BOOL b_get_pressure_zero_point=FALSE;

BOOL* b_timing_flag;
uint32_t* p_prev_os_tick;

BOOL b_reach_10mmHg=FALSE;
BOOL b_show_10mmHg=FALSE;

uint32_t EMD_PRESSURE_ZERO_POINT;
extern uint32_t HONEYWELL_ZERO_POINT;
int32_t diff_in_EMD_and_CtrlBoard;



extern __IO uint16_t ADCConvertedValue[3];
extern uint32_t os_ticks;
extern uint32_t adc_pressure_value;
extern const uint8_t default_parameter_buf[PARAMETER_BUF_LEN];
extern BOOL b_getHoneywellZeroPoint;

//�����ַ���
unsigned char str_Y[]="Y";
unsigned char str_N[]="N";

/***********************************
* �ֲ�����
***********************************/

/***********************************
* �ֲ�����
***********************************/
void EMD_PCB_operate_on(EMD_PCB_OPERATE op)
{
	int emd_op=(int)op;
	switch(emd_op)
	{
		case EMD_OP_START_TEST_SLEEP_CURRENT:    //test sleep curren in 3.0V power supplying
			Set_PIN(ENUM_PIN_JDQ1,TRUE);
			Set_PIN(ENUM_PIN_JDQ2,TRUE);
			break;
		case EMD_OP_STOP_TEST_SLEEP_CURRENT:
			Set_PIN(ENUM_PIN_JDQ1,TRUE);
			Set_PIN(ENUM_PIN_JDQ2,FALSE);
			break;
		case EMD_OP_SUPPLY_MOTORS_POWER:
			Set_PIN(ENUM_PIN_JDQ3,TRUE);
			break;
		case EMD_OP_CUTOFF_MOTORS_POWER:
			Set_PIN(ENUM_PIN_JDQ3,FALSE);
			break;
		case EMD_OP_SUPPLY_3_0_V:
			Set_PIN(ENUM_PIN_V1,TRUE);
			Set_PIN(ENUM_PIN_V2,TRUE);
			break;
		case EMD_OP_SUPPLY_2_5_V:
			Set_PIN(ENUM_PIN_V1,TRUE);
			Set_PIN(ENUM_PIN_V2,FALSE);
			break;
		case EMD_OP_SUPPLY_2_2_V:
			Set_PIN(ENUM_PIN_V1,FALSE);
			Set_PIN(ENUM_PIN_V2,FALSE);
			break;
		case EMD_OP_CUTOFF_POWER_SUPPLY:
			break;
		case EMD_OP_PRESS_POWER_BUTTON:
			Set_PIN(ENUM_PIN_START_KEY,TRUE);
			delay_ms(260);
			Set_PIN(ENUM_PIN_START_KEY,FALSE);
			break;
		case EMD_OP_PRESS_MODE_BUTTON:
			Set_PIN(ENUM_PIN_MODE_KEY,TRUE);
			delay_ms(260);
			Set_PIN(ENUM_PIN_MODE_KEY,FALSE);
			break;
		case EMD_OP_OPEN_VALVE:
			Set_PIN(ENUM_PIN_VALVE,FALSE);
			break;
		case EMD_OP_CLOSE_VALVE:
			Set_PIN(ENUM_PIN_VALVE,TRUE);
			break;
		default:
			break;
	}
}

void re_power_on_EMD_PCBA()
{
	//how to cut off EMD PCBA power supply?
	//open JDQ2 then close JDQ2
	Set_PIN(ENUM_PIN_JDQ2,TRUE);
	delay_ms(500);
	Set_PIN(ENUM_PIN_JDQ2,FALSE);
	delay_ms(1000);
	EMD_PCB_operate_on(EMD_OP_PRESS_POWER_BUTTON);
}

void shut_down_EMD_PCBA()
{
	Set_PIN(ENUM_PIN_JDQ2,TRUE);
	delay_ms(500);
}

void beep()
{
	Set_PIN(ENUM_PIN_BELL,TRUE);
	delay_ms(60);
	Set_PIN(ENUM_PIN_BELL,FALSE);
}

void LCD_show_promt_info(PROMT_INFO info)
{
	int promt_info=(int)info;
	switch(promt_info)
	{
		case PROMT_CHECK_SLEEP_CURRENT:
			display_module_show_string(0, 5+(16+3)*ITEM_SLEEP_CURRENT,16,"1.Sleep current <40uA?",0,WHITE);
			display_module_show_string(0, 260,16,"The current meter value<40uA?",0,YELLOW);
	
			beep();
			display_module_show_string(100, 290,24,"Yes",0,YELLOW);
			display_module_show_string(200, 290,24,"No",0,YELLOW);
			break;
		case PROMT_CHECK_DC_DC:
			display_module_show_string(0, 5+(16+3)*ITEM_DC_DC,16,"2.DC/DC works ok?",0,WHITE);
			break;
		case PROMT_CHECK_MOTORS:
			display_module_show_string(0, 5+(16+3)*ITEM_MOTORS,16,"3.Motors works ok?",0,WHITE);
			break;
		case PROMT_CHECK_SW_VERSION:
			display_module_show_string(0, 5+(16+3)*ITEM_SW_VERSION,16,"4.SW version is 1.1.0?",0,WHITE);
			break;
		case PROMT_CHECK_PARA:
			display_module_show_string(0, 5+(16+3)*ITEM_PARA,16,"5.Parameters are right?",0,WHITE);
			break;
		case PROMT_CHECK_08mmHg:
			display_module_show_string(0, 5+(16+3)*ITEM_08mmHg,16,"6.0.8mmHg trigger ok?",0,WHITE);
			break;
		case PROMT_CHECK_10mmHg:
			display_module_show_string(0, 5+(16+3)*ITEM_10mmHg,16,"7.10mmHg trigger ok?",0,WHITE);
		
			display_module_show_string(0, 260,16,"Solid yellow and vibration?",0,YELLOW);
			delay_ms(1500);
			display_module_show_string(100, 290,24,"Yes",0,YELLOW);
			display_module_show_string(200, 290,24,"No",0,YELLOW);
			beep();
			break;
		case PROMT_CHECK_GREATER_THAN_2_6V:
			display_module_show_string(0, 5+(16+3)*ITEM_Vb_2_6V,16,"8.Work ok on Vb>2.6V?",0,WHITE);
		
			display_module_show_string(0, 260,16,"LED solid green?",0,YELLOW);
			display_module_show_string(100, 290,24,"Yes",0,YELLOW);
			display_module_show_string(200, 290,24,"No",0,YELLOW);
			beep();
			break;
		case PROMT_CHECK_BETWEEN_2_45_AND_2_6V:
			display_module_show_string(0, 5+(16+3)*ITEM_Vb_2_45_AND_2_6V,16,"9.Work ok on 2.45V<Vb<2.6V?",0,WHITE);
		
			display_module_show_string(0, 260,16,"Flash yellow then turn green?",0,YELLOW);
			display_module_show_string(100, 290,24,"Yes",0,YELLOW);
			display_module_show_string(200, 290,24,"No",0,YELLOW);
			beep();
			break;
		case PROMT_CHECK_LESS_THAN_2_45V:
			display_module_show_string(0, 5+(16+3)*ITEM_Vb_2_45V,16,"10.Work ok on Vb<2.45V?",0,WHITE);
		
			display_module_show_string(0, 260,16,"Solid yellow and vibration?",0,YELLOW);
			display_module_show_string(100, 290,24,"Yes",0,YELLOW);
			display_module_show_string(200, 290,24,"No",0,YELLOW);
			beep();
			break;
		case PROMT_CHECK_POWER_ON_KEY:
			display_module_show_string(0, 5+(16+3)*ITEM_POWER_ON_KEY,16,"11.Manual power on/off ok?",0,WHITE);
		
			display_module_show_string(0, 260,16,"Manual power on/off ok?",0,YELLOW);
			display_module_show_string(100, 290,24,"Yes",0,YELLOW);
			display_module_show_string(200, 290,24,"No",0,YELLOW);
			beep();
			break;
		case PROMT_CHECK_MODE_KEY:
			display_module_show_string(0, 5+(16+3)*ITEM_MODE_KEY,16,"12.Manual choose mode ok?",0,WHITE);
			
			display_module_show_string(0, 260,16,"Manual choose mode ok?",0,YELLOW);
			display_module_show_string(100, 290,24,"Yes",0,YELLOW);
			display_module_show_string(200, 290,24,"No",0,YELLOW);
			beep();
			break;
		case PROMT_CHECK_SLEEP_CURRENT_Y:
			display_module_show_string(220, 5+(16+3)*ITEM_SLEEP_CURRENT,16,str_Y,0,GREEN);
			break;
		case PROMT_CHECK_SLEEP_CURRENT_N:
			display_module_show_string(220, 5+(16+3)*ITEM_SLEEP_CURRENT,16,str_N,0,RED);
			break;
		case PROMT_CHECK_SW_VERSION_Y:
			display_module_show_string(220, 5+(16+3)*ITEM_SW_VERSION,16,str_Y,0,GREEN);
			break;
		case PROMT_CHECK_SW_VERSION_N:
			display_module_show_string(220, 5+(16+3)*ITEM_SW_VERSION,16,str_N,0,RED);
			break;
		case PROMT_CHECK_PARA_Y:
			display_module_show_string(220, 5+(16+3)*ITEM_PARA,16,str_Y,0,GREEN);
			break;
		case PROMT_CHECK_PARA_N:
			display_module_show_string(220, 5+(16+3)*ITEM_PARA,16,str_N,0,RED);
			break;
		case PROMT_CHECK_DC_DC_Y:
			display_module_show_string(220, 5+(16+3)*ITEM_DC_DC,16,str_Y,0,GREEN);
			break;
		case PROMT_CHECK_DC_DC_N:
			display_module_show_string(220, 5+(16+3)*ITEM_DC_DC,16,str_N,0,RED);
			break;
		case PROMT_CHECK_MOTORS_Y:
			display_module_show_string(220, 5+(16+3)*ITEM_MOTORS,16,str_Y,0,GREEN);
			break;
		case PROMT_CHECK_MOTORS_N:
			display_module_show_string(220, 5+(16+3)*ITEM_MOTORS,16,str_N,0,RED);
			break;
		case PROMT_CHECK_08mmHg_Y:
			display_module_show_string(220, 5+(16+3)*ITEM_08mmHg,16,str_Y,0,GREEN);
			break;
		case PROMT_CHECK_08mmHg_N:
			display_module_show_string(220, 5+(16+3)*ITEM_08mmHg,16,str_N,0,RED);
			break;
		case PROMT_CHECK_10mmHg_Y:
			display_module_show_string(220, 5+(16+3)*ITEM_10mmHg,16,str_Y,0,GREEN);
			break;
		case PROMT_CHECK_10mmHg_N:
			display_module_show_string(220, 5+(16+3)*ITEM_10mmHg,16,str_N,0,RED);
			break;
		case PROMT_CHECK_GREATER_THAN_2_6V_Y:
			display_module_show_string(220, 5+(16+3)*ITEM_Vb_2_6V,16,str_Y,0,GREEN);
			break;
		case PROMT_CHECK_GREATER_THAN_2_6V_N:
			display_module_show_string(220, 5+(16+3)*ITEM_Vb_2_6V,16,str_N,0,RED);
			break;
		case PROMT_CHECK_BETWEEN_2_45_AND_2_6V_Y:
			display_module_show_string(220, 5+(16+3)*ITEM_Vb_2_45_AND_2_6V,16,str_Y,0,GREEN);
			break;
		case PROMT_CHECK_BETWEEN_2_45_AND_2_6V_N:
			display_module_show_string(220, 5+(16+3)*ITEM_Vb_2_45_AND_2_6V,16,str_N,0,RED);
			break;
		case PROMT_CHECK_LESS_THAN_2_45V_Y:
			display_module_show_string(220, 5+(16+3)*ITEM_Vb_2_45V,16,str_Y,0,GREEN);
			break;
		case PROMT_CHECK_LESS_THAN_2_45V_N:
			display_module_show_string(220, 5+(16+3)*ITEM_Vb_2_45V,16,str_N,0,RED);
			break;
		case PROMT_CHECK_POWER_ON_KEY_Y:
			display_module_show_string(220, 5+(16+3)*ITEM_POWER_ON_KEY,16,str_Y,0,GREEN);
			break;
		case PROMT_CHECK_POWER_ON_KEY_N:
			display_module_show_string(220, 5+(16+3)*ITEM_POWER_ON_KEY,16,str_N,0,RED);
			break;
		case PROMT_CHECK_MODE_KEY_Y:
			display_module_show_string(220, 5+(16+3)*ITEM_MODE_KEY,16,str_Y,0,GREEN);
			break;
		case PROMT_CHECK_MODE_KEY_N:
			display_module_show_string(220, 5+(16+3)*ITEM_MODE_KEY,16,str_N,0,RED);
			break;
		case PROMT_CHECK_FAIL:
			display_module_show_string(80,248,40,"FAIL",0,RED);
			display_module_show_string(0, 290,24,"Start",0,YELLOW);
		  EMD_PCB_operate_on(EMD_OP_STOP_TEST_SLEEP_CURRENT);
			beep();
			break;
		case PROMT_CHECK_FINISH:
			display_module_show_string(80,248,40,"PASS",0,GREEN);
			display_module_show_string(0, 290,24,"Start",0,YELLOW);
		  EMD_PCB_operate_on(EMD_OP_STOP_TEST_SLEEP_CURRENT);
			beep();
			break;
		default:
			break;
	}
}



#if 0
//BOOL Is_the_key_pressed(KEY_NUM num)
//{
//	BOOL res=FALSE;
//	GPIO_TypeDef* GPIO_X=0;
//	uint16_t      PIN_X=0;
//	
//	switch((int)num)
//	{
//		case KEY_START:
//			GPIO_X=START_PORT;
//			PIN_X=START_PIN;
//			break;
//		case KEY_STOP:
//			GPIO_X=STOP_PORT;
//			PIN_X=STOP_PIN;
//			break;
//		case KEY_NEXT:
//			GPIO_X=NEXT_PORT;
//			PIN_X=NEXT_PIN;
//			break;
//		default:
//			break;
//	}
//	

////	if(!GPIO_ReadInputDataBit(GPIO_X,PIN_X))
////	{
////		delay_ms(5);
////		if(!GPIO_ReadInputDataBit(GPIO_X,PIN_X))
////		{
////			res=TRUE;
////		}
////	}
//	
//	BOOL b_pressed=FALSE;
//	if(!GPIO_ReadInputDataBit(GPIO_X,PIN_X))
//	{
//		delay_ms(5);
//		if(!GPIO_ReadInputDataBit(GPIO_X,PIN_X))
//		{
//			b_pressed=TRUE;
//		}
//	}
//	
//	if(b_pressed)
//	{
//		while(1)
//		{
//			if(GPIO_ReadInputDataBit(GPIO_X,PIN_X))
//			{
//				delay_ms(5);
//				if(GPIO_ReadInputDataBit(GPIO_X,PIN_X))
//				{
//					res=TRUE;
//					break;
//				}
//			}
//		}
//	}
//	
//	return res;
//}
#endif

BOOL Is_timing_Xmillisec(uint32_t n_ms,uint8_t num)
{
	switch(num)
	{
		case TIMEOUT_SW_VERSION:      
			b_timing_flag=&SW_VERSION_timing_flag;
			p_prev_os_tick=&prev_SW_VERSION_os_tick;
			break;
		case TIMEOUT_PARA:
			b_timing_flag=&PARA_timing_flag;
			p_prev_os_tick=&prev_PARA_os_tick;
			break;
		case TIMEOUT_ZERO_POINT:
			b_timing_flag=&zero_point_timing_flag;
			p_prev_os_tick=&prev_zero_point_os_tick;
			break;
		case TIMEOUT_08mmHg:
			b_timing_flag=&_08mmHg_timing_flag;
			p_prev_os_tick=&prev_08mmHg_os_tick;
			break;
		case TIMEOUT_10mmHg:
			b_timing_flag=&_10mmHg_timing_flag;
			p_prev_os_tick=&prev_10mmHg_os_tick;
			break;
		default:
			break;
	}
	
	if(*b_timing_flag==TRUE)
	{
		*p_prev_os_tick=os_ticks;
		*b_timing_flag=FALSE;
	}
	if(os_ticks+n_ms<os_ticks) //���os_ticks+n_ms����ˣ���ôos_ticks+n_ms��ȻС��os_ticks
	{
		//*p_prev_os_tick=os_ticks;
		if(os_ticks==os_ticks+n_ms)
		{
			*b_timing_flag=TRUE;
			*p_prev_os_tick=0;
			return TRUE;
		}
	}
	else
	{
		if(os_ticks-*p_prev_os_tick>=n_ms)
		{
			*b_timing_flag=TRUE;
			*p_prev_os_tick=0;
			return TRUE;
		}
	}
	
	return FALSE;
}

void Init_gloable_var()
{
	SW_VERSION_timing_flag=TRUE;
	prev_SW_VERSION_os_tick=0;
	SW_VERSION_fail_cnt=0;

	PARA_timing_flag=TRUE;
	prev_PARA_os_tick=0;
	PARA_fail_cnt=0;
	
	zero_point_timing_flag=TRUE;
	prev_zero_point_os_tick=0;
  zero_point_fail_cnt=0;
	
	_08mmHg_timing_flag=TRUE;
	prev_08mmHg_os_tick=0;
	
	_10mmHg_timing_flag=TRUE;
	prev_10mmHg_os_tick=0;
	
	diff_in_EMD_and_CtrlBoard=0;
	HONEYWELL_ZERO_POINT=0;
	EMD_PRESSURE_ZERO_POINT=0;
	
	b_reach_10mmHg=FALSE;
	b_show_10mmHg=FALSE;
	
	b_timing_flag=0;
	p_prev_os_tick=0;

	b_get_sw_version_success=FALSE;
	b_get_para_success=FALSE;
	b_get_pressure_zero_point=FALSE;	
}

/*******************************************************************************
** ��������: EMD_PCB_test_task
** ��������: ���EMD PCBA����
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
*******************************************************************************/
#if 0
//LED_STATE led_state=LED_ON;
int test=0;
BOOL b_flag=0;
#endif
extern BOOL b_stopKey_pressed;
extern BOOL b_nextKey_pressed;
extern BOOL b_startKey_pressed;

void EMD_PCB_test_task(void)
{
	#if 0
//	EMD_PCB_operate_on(EMD_OP_SUPPLY_MOTORS_POWER);
//	EMD_PCB_operate_on(EMD_OP_START_TEST_SLEEP_CURRENT);
//	
////	if(EMD_check_status==EMD_CHECK_NONE)
////	{
////		Motor_PWM_Freq_Dudy_Set(PWM_PUMP,3000,55);
////		for(int i=0;i<5;i++)
////		{
////			delay_ms(1000);
////		}
////		Motor_PWM_Freq_Dudy_Set(PWM_PUMP,3000,0);
////		
////		EMD_PCB_operate_on(EMD_OP_OPEN_VALVE);
////		
////		delay_ms(1000);
////		delay_ms(1000);
////		delay_ms(1000);
////		EMD_PCB_operate_on(EMD_OP_CLOSE_VALVE);
////		delay_ms(1000);
////		delay_ms(1000);
////		delay_ms(1000);
////		delay_ms(1000);
////	}
#endif
	
	//0.Start checking...
	if(EMD_check_status==EMD_CHECK_START)
	{		 
//		b_getHoneywellZeroPoint=FALSE;
		b_reach_10mmHg=FALSE;
		HONEYWELL_ZERO_POINT=0;
		
		EMD_PCB_operate_on(EMD_OP_CLOSE_VALVE);

		set_lcd_backlight(1);
		lcd_draw_rect_real(0,0,239,319,BACK_COLOR);
		
		LCD_show_promt_info(PROMT_CHECK_SLEEP_CURRENT);
		
		EMD_PCB_operate_on(EMD_OP_SUPPLY_3_0_V); 								//power on EMD PCBA
		EMD_PCB_operate_on(EMD_OP_START_TEST_SLEEP_CURRENT);		//start test sleep current when Vbat=3.0V
			
		EMD_check_status=EMD_CHECK_SLEEP_CURRENT;
		
		Init_gloable_var();
	}
	
	//1.Check EMD PCB sleep Current
	if(EMD_check_status==EMD_CHECK_SLEEP_CURRENT)
	{
		if(b_nextKey_pressed)
		{
			Init_all_keys();
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);     // clear "Yes" "No"
			LCD_show_promt_info(PROMT_CHECK_SLEEP_CURRENT_Y); //show sleep current test "Y"
			
			EMD_PCB_operate_on(EMD_OP_STOP_TEST_SLEEP_CURRENT);  
			delay_ms(1000);
			EMD_PCB_operate_on(EMD_OP_SUPPLY_MOTORS_POWER);			 

			EMD_check_status=EMD_CHECK_DC_DC;
			LCD_show_promt_info(PROMT_CHECK_DC_DC); 
		}
		else if(b_stopKey_pressed)
		{
			Init_all_keys();
			
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);
			LCD_show_promt_info(PROMT_CHECK_SLEEP_CURRENT_N);
			
			EMD_check_status=EMD_CHECK_FAIL;
			LCD_show_promt_info(PROMT_CHECK_FAIL);
		}
		else
		{
			//do nothing
		}
	}
	
	//2.check DC/DC
	if(EMD_check_status==EMD_CHECK_DC_DC)
	{
		if(ADCConvertedValue[2]>=1737&&ADCConvertedValue[2]<=2000)   //3.3-4096,1.5+-0.1V->1.4v-1.61v,1737-2000
		{
			LCD_show_promt_info(PROMT_CHECK_DC_DC_Y);
			
			EMD_PCB_operate_on(EMD_OP_PRESS_POWER_BUTTON);       //Start_Key 1 delay 100ms then Start_Key 0
			
			EMD_check_status=EMD_CHECK_MOTORS;
			LCD_show_promt_info(PROMT_CHECK_MOTORS); 
		}
		else if(ADCConvertedValue[2]<1737)
		{
			LCD_show_promt_info(PROMT_CHECK_DC_DC_N);
			EMD_check_status=EMD_CHECK_FAIL;
			LCD_show_promt_info(PROMT_CHECK_FAIL); 
		}
	}
	
	//3.Check Motors
	if(EMD_check_status==EMD_CHECK_MOTORS)
	{
		if(ADCConvertedValue[0]>=620&&ADCConvertedValue[0]<=1861&&ADCConvertedValue[1]>=620&&ADCConvertedValue[1]<=1861) //0.5V-1.5V
		{
			LCD_show_promt_info(PROMT_CHECK_MOTORS_Y);
			
			EMD_check_status=EMD_CHECK_SW_VERSION;
			LCD_show_promt_info(PROMT_CHECK_SW_VERSION);
			send_get_sw_version();
		}
		else if((ADCConvertedValue[0]<620||ADCConvertedValue[0]>1861)||(ADCConvertedValue[1]<620||ADCConvertedValue[1]>1861))
		{
			LCD_show_promt_info(PROMT_CHECK_MOTORS_N);
			EMD_check_status=EMD_CHECK_FAIL;
			LCD_show_promt_info(PROMT_CHECK_FAIL); 
		}
	}
	
	//4.Check sw version, suppose to be 1.1.0
	if(EMD_check_status==EMD_CHECK_SW_VERSION)
	{
		//there are 3 chance to get the SW version
		//in each chance, if do not get the sw version, then go to next chance
		//when total up to 3 chances, system thought fail to get the sw version
		if(SW_VERSION_fail_cnt==3)
		{
			SW_VERSION_fail_cnt=0;
			EMD_check_status=EMD_CHECK_FAIL;
			LCD_show_promt_info(PROMT_CHECK_FAIL);
			LCD_show_promt_info(PROMT_CHECK_SW_VERSION_N);
		}
		else
		{
			//if do not get the sw version, resent "send_get_sw_version()"
			//wait 3s then resent
			if(Is_timing_Xmillisec(3000,TIMEOUT_SW_VERSION))   
			{
				send_get_sw_version();
				SW_VERSION_fail_cnt++;
			}
			else
			{
				if(b_get_sw_version_success)
				{
					b_get_sw_version_success=FALSE;
				
					LCD_show_promt_info(PROMT_CHECK_SW_VERSION_Y);
					EMD_check_status=EMD_CHECK_PARA;
					
					//there are 2 request for parameters
					//send query1
					SendQuery1ForParameters();
					LCD_show_promt_info(PROMT_CHECK_PARA);
				}
			}
		}
	}
	
	//5.check parameters
	if(EMD_check_status==EMD_CHECK_PARA)
	{
		if(PARA_fail_cnt==3)
		{
			PARA_fail_cnt=0;
			EMD_check_status=EMD_CHECK_FAIL;
			LCD_show_promt_info(PROMT_CHECK_PARA_N);
			LCD_show_promt_info(PROMT_CHECK_FAIL);
		}
		else
		{
			if(Is_timing_Xmillisec(4000,TIMEOUT_PARA))   
			{
				SendQuery1ForParameters();
				PARA_fail_cnt++;
			}
			else
			{
				if(b_get_para_success)
				{
					b_get_para_success=FALSE;
					
					LCD_show_promt_info(PROMT_CHECK_PARA_Y);
					
//					send_get_pressure_zero_point();
//					EMD_check_status=EMD_CHECK_GET_PRESSURE_ZERO_POINT;
					
					EMD_check_status=EMD_CHECK_08mmHg;

					LCD_show_promt_info(PROMT_CHECK_08mmHg);
				
					Motor_PWM_Freq_Dudy_Set(PWM_PUMP,3000,50);
					HONEYWELL_ZERO_POINT=adc_pressure_value;    //get zero point
				}
			}
		}
	}
	
	#if 0
	//6. 1> get pressure zero point from EMD PCBA
	if(EMD_check_status==EMD_CHECK_GET_PRESSURE_ZERO_POINT)
	{
		if(zero_point_fail_cnt==3)
		{
			zero_point_fail_cnt=0;
			EMD_check_status=EMD_CHECK_FAIL;
			LCD_show_promt_info(PROMT_CHECK_FAIL);
//			LCD_show_promt_info(PROMT_CHECK_08mmHg_N);
			display_module_show_string(210, 5+(16+3)*ITEM_08mmHg,16,"N",0,RED);
		}
		else
		{
			if(Is_timing_Xmillisec(1000,TIMEOUT_ZERO_POINT))   
			{
				send_get_pressure_zero_point();
				zero_point_fail_cnt++;
			}
			else
			{
				if(b_get_pressure_zero_point)
				{
					b_get_pressure_zero_point=FALSE;
				
					EMD_check_status=EMD_CHECK_08mmHg;
					display_module_show_string(200, 5+(16+3)*ITEM_08mmHg,16,"Y",0,GREEN);
					diff_in_EMD_and_CtrlBoard=EMD_PRESSURE_ZERO_POINT-adc_pressure_value;
				}
			}
		}
	}
	#endif
	
	//6. 2>judge wheter the pressure reach 0.8mmHg 
	if(EMD_check_status==EMD_CHECK_08mmHg)
	{
		if(Is_timing_Xmillisec(5000,TIMEOUT_08mmHg))   
		{
			Motor_PWM_Freq_Dudy_Set(PWM_PUMP,3000,0);
			
			EMD_check_status=EMD_CHECK_FAIL;
			LCD_show_promt_info(PROMT_CHECK_FAIL);
			LCD_show_promt_info(PROMT_CHECK_08mmHg_N);
		}
		else
		{
//			static uint32_t _08mmHg;
//			static uint32_t _10mmHg;
//			_08mmHg=trans_xmmHg_2_adc_value(default_parameter_buf[0]);
//			_10mmHg=trans_xmmHg_2_adc_value(PRESSURE_SAFETY_THRESHOLD);
			//if(b_getHoneywellZeroPoint)
			{
				if(adc_pressure_value>=trans_xmmHg_2_adc_value(default_parameter_buf[0])
				&&adc_pressure_value<=trans_xmmHg_2_adc_value(PRESSURE_SAFETY_THRESHOLD))
				{
					LCD_show_promt_info(PROMT_CHECK_08mmHg_Y);
					Motor_PWM_Freq_Dudy_Set(PWM_PUMP,3000,0);
					
					EMD_check_status=EMD_CHECK_10mmHg;
//					LCD_show_promt_info(PROMT_CHECK_10mmHg);
					
					Motor_PWM_Freq_Dudy_Set(PWM_PUMP,3000,50);
				}
			}
		}
	}
	//7. check 10mmHg
	if(EMD_check_status==EMD_CHECK_10mmHg)
	{
		if(Is_timing_Xmillisec(5000,TIMEOUT_10mmHg))   
		{
			Motor_PWM_Freq_Dudy_Set(PWM_PUMP,3000,0);
			
			EMD_check_status=EMD_CHECK_FAIL;
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);     // clear "Yes" "No"
			LCD_show_promt_info(PROMT_CHECK_FAIL);
			LCD_show_promt_info(PROMT_CHECK_10mmHg_N);
		}
		else
		{
			if(!b_show_10mmHg)
			{
				if(adc_pressure_value>=trans_xmmHg_2_adc_value(PRESSURE_SAFETY_THRESHOLD_LOW_LIMIT))
				{
					b_show_10mmHg=TRUE;
					LCD_show_promt_info(PROMT_CHECK_10mmHg);
//					Motor_PWM_Freq_Dudy_Set(PWM_PUMP,3000,0);
//					delay_ms(500);
					Motor_PWM_Freq_Dudy_Set(PWM_PUMP,3000,50);
				}
			}
			
			if(adc_pressure_value>=trans_xmmHg_2_adc_value(PRESSURE_SAFETY_THRESHOLD_HIGH_LIMIT))
			{
				EMD_PCB_operate_on(EMD_OP_OPEN_VALVE);
				
				Motor_PWM_Freq_Dudy_Set(PWM_PUMP,3000,0);
				b_reach_10mmHg=TRUE;
				EMD_check_status=EMD_CHECK_KEY_PRESSED_AFTER_10mmHg;
			}
		}
	}
	
	if(EMD_check_status==EMD_CHECK_KEY_PRESSED_AFTER_10mmHg)
	{
		if(b_nextKey_pressed)
		{
			Init_all_keys();

			LCD_show_promt_info(PROMT_CHECK_10mmHg_Y);
			
			LCD_show_promt_info(PROMT_CHECK_GREATER_THAN_2_6V);		
			EMD_check_status=EMD_CHECK_BAT_GREATER_THAN_2_6V;
			
			EMD_PCB_operate_on(EMD_OP_OPEN_VALVE);
			EMD_PCB_operate_on(EMD_OP_SUPPLY_3_0_V);         //supply 3.0V to EMD PCBA
			
			re_power_on_EMD_PCBA();

		}
		else if(b_stopKey_pressed)
		{
			Init_all_keys();
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);
			LCD_show_promt_info(PROMT_CHECK_10mmHg_N);
			
			EMD_check_status=EMD_CHECK_FAIL;
			LCD_show_promt_info(PROMT_CHECK_FAIL);
		}
	}
	
	if(EMD_check_status==EMD_CHECK_BAT_GREATER_THAN_2_6V)
	{
		if(b_nextKey_pressed)
		{
			Init_all_keys();
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);     // clear "Yes" "No"
			LCD_show_promt_info(PROMT_CHECK_GREATER_THAN_2_6V_Y); 
			#if 0
//			EMD_PCB_operate_on(EMD_OP_PRESS_POWER_BUTTON);   //power off the EMD PCBA
//			shut_down_EMD_PCBA();
//			
//			EMD_PCB_operate_on(EMD_OP_SUPPLY_2_5_V);         //supply 2.5V to EMD PCBA
//			delay_ms(200);
//			
//			EMD_PCB_operate_on(EMD_OP_PRESS_POWER_BUTTON);	//power on the EMD PCBA
			#endif
			EMD_PCB_operate_on(EMD_OP_SUPPLY_2_5_V);
			re_power_on_EMD_PCBA();
			
			EMD_check_status=EMD_CHECK_BAT_BETWEEN_2_45_AND_2_6V;
			LCD_show_promt_info(PROMT_CHECK_BETWEEN_2_45_AND_2_6V); 
		}
		else if(b_stopKey_pressed)
		{
			Init_all_keys();
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);
			LCD_show_promt_info(PROMT_CHECK_GREATER_THAN_2_6V_N);
			
			EMD_check_status=EMD_CHECK_FAIL;
			LCD_show_promt_info(PROMT_CHECK_FAIL);
		}
		else
		{
			//do nothing
		}
	}
	
	if(EMD_check_status==EMD_CHECK_BAT_BETWEEN_2_45_AND_2_6V)
	{
		if(b_nextKey_pressed)
		{
			Init_all_keys();
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);     // clear "Yes" "No"
			LCD_show_promt_info(PROMT_CHECK_BETWEEN_2_45_AND_2_6V_Y); 

			EMD_PCB_operate_on(EMD_OP_SUPPLY_2_2_V); 
			re_power_on_EMD_PCBA();
			
			EMD_check_status=EMD_CHECK_BAT_LESS_THAN_2_45V;
			LCD_show_promt_info(PROMT_CHECK_LESS_THAN_2_45V); 
		}
		else if(b_stopKey_pressed)
		{
			Init_all_keys();
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);
			LCD_show_promt_info(PROMT_CHECK_BETWEEN_2_45_AND_2_6V_N);
			
			EMD_check_status=EMD_CHECK_FAIL;
			LCD_show_promt_info(PROMT_CHECK_FAIL);
		}
		else
		{
			//do nothing
		}
	}
	
	if(EMD_check_status==EMD_CHECK_BAT_LESS_THAN_2_45V)
	{
		if(b_nextKey_pressed)
		{
			Init_all_keys();
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);     // clear "Yes" "No"
			LCD_show_promt_info(PROMT_CHECK_LESS_THAN_2_45V_Y); 
			
			EMD_PCB_operate_on(EMD_OP_SUPPLY_3_0_V);
			shut_down_EMD_PCBA();
			Set_PIN(ENUM_PIN_JDQ2,FALSE);
			delay_ms(1000);
			
			EMD_check_status=EMD_CHECK_POWER_ON_KEY;
			LCD_show_promt_info(PROMT_CHECK_POWER_ON_KEY); 
		}
		else if(b_stopKey_pressed)
		{
			Init_all_keys();
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);
			LCD_show_promt_info(PROMT_CHECK_LESS_THAN_2_45V_N);
			
			EMD_check_status=EMD_CHECK_FAIL;
			LCD_show_promt_info(PROMT_CHECK_FAIL);
		}
		else
		{
			//do nothing
		}
	}
	
	if(EMD_check_status==EMD_CHECK_POWER_ON_KEY)
	{
		if(b_nextKey_pressed)
		{
			Init_all_keys();
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);     // clear "Yes" "No"
			LCD_show_promt_info(PROMT_CHECK_POWER_ON_KEY_Y); 
			
			re_power_on_EMD_PCBA();
			
			EMD_check_status=EMD_CHECK_MODE_KEY;
			LCD_show_promt_info(PROMT_CHECK_MODE_KEY); 
		}
		else if(b_stopKey_pressed)
		{
			Init_all_keys();
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);
			LCD_show_promt_info(PROMT_CHECK_POWER_ON_KEY_N);
			
			EMD_check_status=EMD_CHECK_FAIL;
			LCD_show_promt_info(PROMT_CHECK_FAIL);
		}
		else
		{
			//do nothing
		}
	}
	
	if(EMD_check_status==EMD_CHECK_MODE_KEY)
	{
		if(b_nextKey_pressed)
		{
			Init_all_keys();
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);     // clear "Yes" "No"
			LCD_show_promt_info(PROMT_CHECK_MODE_KEY_Y); 
			
			EMD_check_status=EMD_CHECK_FINISHED;
			LCD_show_promt_info(PROMT_CHECK_FINISH); 
		}
		else if(b_stopKey_pressed)
		{
			Init_all_keys();
			lcd_draw_rect_real(0,260,239,319,BACK_COLOR);
			LCD_show_promt_info(PROMT_CHECK_MODE_KEY_N);
			
			EMD_check_status=EMD_CHECK_FAIL;
			LCD_show_promt_info(PROMT_CHECK_FAIL);
		}
		else
		{
			//do nothing
		}
	}
	
	if(EMD_check_status==EMD_CHECK_FAIL||EMD_check_status==EMD_CHECK_FINISHED)
	{
		if(b_startKey_pressed)
		{
			EMD_PCB_operate_on(EMD_OP_OPEN_VALVE); 
			shut_down_EMD_PCBA();
			
			Init_all_keys();
			EMD_check_status=EMD_CHECK_START;
		}
	}
	
	//IWDG_Feed();   //ι��
	os_delay_ms(EMD_PCB_TEST_TASK_ID, EMD_PCB_TEST_PERIOD);
}