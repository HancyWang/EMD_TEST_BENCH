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


//#include "stm32f10x.h"
//#include "stm32f10x_tim.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_pwr.h"
////#include "stm32f0xx_rtc.h"
//#include "stm32f10x_dma.h"

#include "delay.h"
#include "os_cfg.h"
#include "datatype.h"
#include "hardware.h"
//#include "time.h"
////#include "i2c.h"
////#include "comm_task.h"
//#include "protocol_module.h"
////#include "key_led_task.h"
////#include "app.h"
////#include "rtc.h"
#include "stm32f10x.h"
#include "misc.h"
#include "fifo.h"
#include "serial_port.h"
#include "CMD_Receive.h"
#include "comm_task.h"
#include "Motor_pwm.h"

//ȫ�ֱ���
__IO uint16_t ADCConvertedValue[3];

 // ������տ��ƶ���
extern CMD_Receive g_CmdReceive; 

//�l�͔���FIFO
extern FIFO_TYPE send_fifo;
extern uint8_t send_buf[SEND_BUF_LEN];

/**************************************************************
* ��ʼ��OS�δ�ʱ��
//ʹ��TIM2
**************************************************************/
void init_tim()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;
	
//	  /* PCLK1 = HCLK/8 */
  RCC_PCLK1Config(RCC_HCLK_Div8);

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = SystemCoreClock/8/OS_TICKS_PER_SEC-1;  // �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) 
  TIM_TimeBaseStructure.TIM_Prescaler = 1;	//ʱ��Ԥ��Ƶ�� ,��Ӳ���й�
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_ClearFlag(TIM2, TIM_FLAG_Update);			        // �������жϱ�־ 
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM2, ENABLE);
}


/**************************************************************
* ��ʼ��LED
**************************************************************/
void Init_LED()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

/**************************************************************
* ����LED״̬
**************************************************************/
void Set_LED(BOOL b_open)
{
	if(b_open)
	{
		GPIO_ResetBits(LED_PORT,LED_PIN);
	}
	else
	{
		GPIO_SetBits(LED_PORT,LED_PIN);
	}
}

/**************************************************************
* ��ʼ��BELL
**************************************************************/
void Init_BELL(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = BELL_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(BELL_PORT, &GPIO_InitStructure);
}

/**************************************************************
* ����BELL
**************************************************************/
void Set_BELL(BOOL b_open)
{
	if(b_open)
	{
		GPIO_ResetBits(BELL_PORT,BELL_PIN);
	}
	else
	{
		GPIO_SetBits(BELL_PORT,BELL_PIN);
	}
}

/**************************************************************
* ��ʼ��STOP
**************************************************************/
void Init_STOP(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = STOP_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(STOP_PORT, &GPIO_InitStructure);
}

/**************************************************************
* ��ʼ��START
**************************************************************/
void Init_START(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = START_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(START_PORT, &GPIO_InitStructure);
}

/**************************************************************
* ��ʼ��NEXT
**************************************************************/
void Init_NEXT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = NEXT_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(NEXT_PORT, &GPIO_InitStructure);
}

/**************************************************************
* ��ʼ��ADC1 Motor_current1��Motor_current2,�Լ�VDD_3_0_ADC
**************************************************************/
void Init_ADC1()
{
	//1.RCC Configuration
  /* ADCCLK = PCLK2/4 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div4); 

  /* Enable peripheral clocks ------------------------------------------------*/
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	
	//2.GPIO Configuration
	GPIO_InitTypeDef GPIO_InitStructure;
  /* Configure PA0,PA1 as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = ADC1_MOTOR_CURRENT1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADC1_MOTOR_CURRENT1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADC1_MOTOR_CURRENT2_PIN;
  GPIO_Init(ADC1_MOTOR_CURRENT2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADC1_VDD_3_0_PIN;
  GPIO_Init(ADC1_VDD_3_0_PORT, &GPIO_InitStructure);
	
	
	//3.DMA Configuration
	/* DMA1 channel1 configuration ----------------------------------------------*/
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//  DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_BufferSize = 3;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 3;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**************************************************************
* ��ʼ��V1,V2
**************************************************************/
void Init_V1_V2(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = V1_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(V1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = V2_PIN;
	GPIO_Init(V2_PORT, &GPIO_InitStructure);
}

/**************************************************************
* ��ʼ��Error LED
**************************************************************/
void Init_Error_LED()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = ERROR_LED_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(ERROR_LED_PORT, &GPIO_InitStructure);
}

/**************************************************************
* ��ʼ��LCD
**************************************************************/
void Init_LCD()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = LCD_BLEN_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(LCD_BLEN_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_RD_PIN;
	GPIO_Init(LCD_RD_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_WR_PIN;
	GPIO_Init(LCD_WR_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_DC_PIN;
	GPIO_Init(LCD_DC_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;
	GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_RESET_PIN;
	GPIO_Init(LCD_RESET_PORT, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = LCD_D0_PIN;
	GPIO_Init(LCD_D0_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_D1_PIN;
	GPIO_Init(LCD_D1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_D2_PIN;
	GPIO_Init(LCD_D2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_D3_PIN;
	GPIO_Init(LCD_D3_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_D4_PIN;
	GPIO_Init(LCD_D4_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_D5_PIN;
	GPIO_Init(LCD_D5_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_D6_PIN;
	GPIO_Init(LCD_D6_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_D7_PIN;
	GPIO_Init(LCD_D7_PORT, &GPIO_InitStructure);
}

/**************************************************************
* ��ʼ��JDQ
**************************************************************/
void Init_JDQ(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = JDQ1_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(JDQ1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = JDQ2_PIN;
	GPIO_Init(JDQ2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = JDQ3_PIN;
	GPIO_Init(JDQ3_PORT, &GPIO_InitStructure);
}

/**************************************************************
* ��ʼ��START KEY
**************************************************************/
void Init_START_KEY(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = START_KEY_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(START_KEY_PORT, &GPIO_InitStructure);
}


/**************************************************************
* ��ʼ��MODE KEY
**************************************************************/
void Init_MODE_KEY(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = MODE_KEY_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(MODE_KEY_PORT, &GPIO_InitStructure);
}

/**************************************************************
* ��ʼ��USART2,�Ͷ�ʹPCBAͨ�ŵ�
**************************************************************/
void Init_UART_Comm_with_PCBA()
{
	fifoInit(&send_fifo,send_buf,SEND_BUF_LEN);
	UARTInit(g_CmdReceive.m_Buf1, BUF1_LENGTH);	
	Init_Receive(&g_CmdReceive);
}

/**************************************************************
* ��ʼ��Ӳ���ܽ�
**************************************************************/
void init_hardware()
{
	//��IO��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	
	//��TIM1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//��TIM2,TIM3,TIM4ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4,ENABLE);
	
  Init_LED();
	Init_BELL();

	Init_STOP();
	Init_START();
	Init_NEXT();
	
	Init_ADC1();
	
	Init_V1_V2();
	
	Init_Error_LED();
	
	Init_LCD();
	
	Init_JDQ();
	Init_START_KEY();
	Init_MODE_KEY();
	
	Init_UART_Comm_with_PCBA();
	Init_Motor_PWM();
}
