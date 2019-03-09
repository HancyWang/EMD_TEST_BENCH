/**
********************************************************************************
* ��ࣺ
* ģ�����ƣ�hardware.c
* ģ�鹦�ܣ�
* �������ڣ�
* �� �� �ߣ�
* ��    ����
********************************************************************************
**/

/***********************************
* ͷ�ļ�
***********************************/
#include "Motor_pwm.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rtc.h"

#include "delay.h"
#include "os_cfg.h"
#include "datatype.h"
#include "hardware.h"

#include "time.h"

/**********************************
*�궨��
***********************************/

/***********************************
* ȫ�ֱ���
***********************************/
uint16_t PrescalerValue = 0;

/**************************************************************
* ��ʼ��ʱ�ӣ�����PUMP��GPIO
**************************************************************/
static void Motor_PWM_GPIO_Config(void) 
{
	//1.RCC Configuration
	/* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  /*  GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	//2.GPIO Configuration
	GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = PUMP_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(PUMP_PORT, &GPIO_InitStructure);
}

/**************************************************************
* ����PWM
**************************************************************/
static void Motor_PWM_Config(void)  //��Ƶ
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//18M����
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 18000-1;
  TIM_TimeBaseStructure.TIM_Prescaler = 1000-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);   //TIM4 CH1

  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM4, ENABLE);

  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);
}

/**************************************************************
* PUMP��ʼ������
**************************************************************/
void Init_Motor_PWM(void)
{
	Motor_PWM_GPIO_Config();
	Motor_PWM_Config();
}

/**************************************************************
* ����PWM���
**************************************************************/
void Motor_PWM_Freq_Dudy_Set(UINT8 PWM_NUMBER, UINT16 Freq,UINT16 Duty)			//PWM1-2-3,FREQ,DUFY
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	UINT32 i;	
	
	if((Freq >=1) && (Freq <=50000)// Frequency  1 - 255Hz
		&& (Duty <= 100))//Duty cycle 10 - 90
	{
		TIM_TimeBaseStructure.TIM_Period = 18000/Freq - 1;       //
		TIM_TimeBaseStructure.TIM_Prescaler = 1000-1;	    //1000Ԥ��Ƶ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
		
		i = TIM_TimeBaseStructure.TIM_Period + 1;
		i *= Duty;
		i /= 100;
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = i;
		
		if(PWM_NUMBER == PWM_PUMP)
		{
			TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	
			TIM_OC1Init(TIM4, &TIM_OCInitStructure);	 //ʹ��TIM3CH1
			TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);	//		
		}
		else
		{
			//do nothing
		}
	}
}


  



