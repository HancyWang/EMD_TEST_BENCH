#include "key_check_task.h"
#include "app.h"
#include "datatype.h"
#include "hardware.h"
#include "emd_pcb_test_task.h"

typedef enum
{
	KEY_STOP,
	KEY_START,
	KEY_NEXT
}KEY_NAME;

#ifdef INTERRUPT_3_KEYS
#if 1
BOOL b_startKey_pressed=FALSE;
BOOL b_nextKey_pressed=FALSE;
BOOL b_stopKey_pressed=FALSE;

BOOL b_startKey_falling=FALSE;
BOOL b_nextKey_falling=FALSE;
BOOL b_stopKey_falling=FALSE;
BOOL b_startKey_falled=FALSE;
BOOL b_nextKey_falled=FALSE;
BOOL b_stopKey_falled=FALSE;

uint8_t startKey_falling_cnt=0;
uint8_t nextKey_falling_cnt=0;
uint8_t stopKey_falling_cnt=0;
uint8_t startKey_raising_cnt=0;
uint8_t nextKey_raising_cnt=0;
uint8_t stopKey_raising_cnt=0;

BOOL b_startKey_raising=FALSE;
BOOL b_nextKey_raising=FALSE;
BOOL b_stopKey_raising=FALSE;
BOOL b_startKey_raised=FALSE;
BOOL b_nextKey_raised=FALSE;
BOOL b_stopKey_raised=FALSE;

//通用指针，用来保存以上变量的地址
BOOL* p_b_Key_pressed=0;
BOOL* p_b_KeyFalling=0;
BOOL* p_b_KeyFalled=0;
BOOL* p_b_KeyRaising=0;
BOOL* p_b_KeyRaised=0;
uint8_t* p_KeyFallingCnt=0;
uint8_t* p_KeyRaisingCnt=0;


typedef enum
{
	KEY_NONE,
	KEY_FALLING,
	KEY_FALLED,
	KEY_RAISING,
	KEY_RAISED,
	KEY_PRESSED
}KEY_STATE;

KEY_STATE key_state=KEY_NONE;

void Init_Key_var()
{
//	b_startKey_pressed=FALSE;
//	b_nextKey_pressed=FALSE;
//	b_stopKey_pressed=FALSE;

	b_startKey_falling=FALSE;
	b_nextKey_falling=FALSE;
	b_stopKey_falling=FALSE;
	b_startKey_falled=FALSE;
	b_nextKey_falled=FALSE;
	b_stopKey_falled=FALSE;

	startKey_falling_cnt=0;
	nextKey_falling_cnt=0;
	stopKey_falling_cnt=0;
	startKey_raising_cnt=0;
	nextKey_raising_cnt=0;
	stopKey_raising_cnt=0;

	b_startKey_raising=FALSE;
	b_nextKey_raising=FALSE;
	b_stopKey_raising=FALSE;
	b_startKey_raised=FALSE;
	b_nextKey_raised=FALSE;
	b_stopKey_raised=FALSE;

	p_b_Key_pressed=0;
	p_b_KeyFalling=0;
	p_b_KeyFalled=0;
	p_b_KeyRaising=0;
	p_b_KeyRaised=0;
	p_KeyFallingCnt=0;
	p_KeyRaisingCnt=0;
}



void key_check_task(void)
{
	
	if(p_b_KeyFalling==0||p_b_KeyRaising==0)
	{
		//do nothing
	}
	else
	{
		if(*p_b_KeyFalling==TRUE)
		{
			if(*p_KeyFallingCnt==3)   //如果连按30ms
			{
				*p_KeyFallingCnt=0;
				*p_b_KeyFalled=TRUE;  //认为按键falled
			}
			else
			{
				++(*p_KeyFallingCnt);
			}
		}
		else
		{
			*p_KeyFallingCnt=0;
			*p_b_KeyFalled=FALSE;
		}
		
		if(*p_b_KeyRaising==TRUE)
		{
			if(*p_b_KeyFalled==TRUE)   //必须要falled才能去判断raise
			{
				if(*p_KeyRaisingCnt==3)   
				{
					*p_KeyRaisingCnt=0;
					*p_b_KeyRaised=TRUE;    //认为按键raised
					*p_b_Key_pressed=TRUE;
					
					Init_Key_var();					//清除变量
				}
				else
				{
					++(*p_KeyRaisingCnt);
				}
			}
		}
		else
		{
			*p_KeyRaisingCnt=0;
			*p_b_KeyRaised=FALSE;
		}
	}
	
	os_delay_ms(KEY_CHECK_TASK_ID, KEY_CHECK_PERIOD);
}
#endif
#else
//采用查询的方式来check 按键是否按下
extern EMD_PCB_TEST_STATUS EMD_check_status;
typedef enum
{
	KEY_NONE,
	KEY_FALLED
}KEY_STATE;

KEY_STATE KeySTOP_state=KEY_NONE;
uint8_t KeySTOP_cnt=0;
BOOL b_stopKey_pressed=FALSE;

KEY_STATE KeySTART_state=KEY_NONE;
uint8_t KeySTART_cnt=0;
BOOL b_startKey_pressed=FALSE;

KEY_STATE KeyNEXT_state=KEY_NONE;
uint8_t KeyNEXT_cnt=0;
BOOL b_nextKey_pressed=FALSE;


//按键初始化
void Init_key_stop()
{
	KeySTOP_state=KEY_NONE;
	KeySTOP_cnt=0;
	b_stopKey_pressed=FALSE;
}

void Init_key_start()
{
	KeySTART_state=KEY_NONE;
	KeySTART_cnt=0;
	b_startKey_pressed=FALSE;
}

void Init_key_next()
{
	KeyNEXT_state=KEY_NONE;
	KeyNEXT_cnt=0;
	b_nextKey_pressed=FALSE;
}

void Init_all_keys()
{
	Init_key_stop();
	Init_key_start();
	Init_key_next();
}

//按键检测任务
void key_check_task(void)
{
	
	if(EMD_check_status==EMD_CHECK_SLEEP_CURRENT||
		EMD_check_status==EMD_CHECK_BAT_GREATER_THAN_2_6V||
		EMD_check_status==EMD_CHECK_BAT_BETWEEN_2_45_AND_2_6V||
		EMD_check_status==EMD_CHECK_BAT_LESS_THAN_2_45V||
		EMD_check_status==EMD_CHECK_POWER_ON_KEY||
		EMD_check_status==EMD_CHECK_MODE_KEY||
		EMD_check_status==EMD_CHECK_KEY_PRESSED_AFTER_10mmHg)
	{
		//PA4 stop
		#if 1
		if(KeySTOP_state==KEY_NONE)
		{
			if(KeySTOP_cnt==3)
			{
				KeySTOP_cnt=0;
				KeySTOP_state=KEY_FALLED;
			}
			else
			{
				if(!GPIO_ReadInputDataBit(STOP_PORT,STOP_PIN))
				{
					KeySTOP_cnt++;
				}
			}
		}
		else if(KeySTOP_state==KEY_FALLED)
		{
			if(KeySTOP_cnt==3)
			{
				KeySTOP_cnt=0;
				KeySTOP_state=KEY_NONE;
				b_stopKey_pressed=TRUE;
				Init_key_start();   //清除另外两个按键
				Init_key_next();
			}
			else
			{
				if(GPIO_ReadInputDataBit(STOP_PORT,STOP_PIN))
				{
					KeySTOP_cnt++;
				}
			}
		}
		#endif
		
		//PA6 next
		#if 1
		if(KeyNEXT_state==KEY_NONE)
		{
			if(KeyNEXT_cnt==3)
			{
				KeyNEXT_cnt=0;
				KeyNEXT_state=KEY_FALLED;
			}
			else
			{
				if(!GPIO_ReadInputDataBit(NEXT_PORT,NEXT_PIN))
				{
					KeyNEXT_cnt++;
				}
			}
		}
		else if(KeyNEXT_state==KEY_FALLED)
		{
			if(KeyNEXT_cnt==3)
			{
				KeyNEXT_cnt=0;
				KeyNEXT_state=KEY_NONE;
				b_nextKey_pressed=TRUE;
				Init_key_stop();   //清除另外两个按键
				Init_key_start();
			}
			else
			{
				if(GPIO_ReadInputDataBit(NEXT_PORT,NEXT_PIN))
				{
					KeyNEXT_cnt++;
				}
			}
		}
		#endif
	}
	
	
	if(EMD_check_status==EMD_CHECK_FAIL||
			EMD_check_status==EMD_CHECK_FINISHED||
			EMD_check_status==EMD_CHECK_PREV_START)
	{
		//PA5 start
		if(KeySTART_state==KEY_NONE)
		{
			if(KeySTART_cnt==3)
			{
				KeySTART_cnt=0;
				KeySTART_state=KEY_FALLED;
			}
			else
			{
				if(!GPIO_ReadInputDataBit(START_PORT,START_PIN))
				{
					KeySTART_cnt++;
				}
			}
		}
		else if(KeySTART_state==KEY_FALLED)
		{
			if(KeySTART_cnt==3)
			{
				KeySTART_cnt=0;
				KeySTART_state=KEY_NONE;
				b_startKey_pressed=TRUE;
				Init_key_stop();   //清除另外两个按键
				Init_key_next();
			}
			else
			{
				if(GPIO_ReadInputDataBit(START_PORT,START_PIN))
				{
					KeySTART_cnt++;
				}
			}
		}
	}
	
	



	os_delay_ms(KEY_CHECK_TASK_ID, KEY_CHECK_PERIOD);
}

#endif
