/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "os_core.h"
#include "hardware.h"
#include "key_check_task.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
		os_isr_ticks();

    TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
  }
}

#ifdef INTERRUPT_3_KEYS
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)  
	{ 
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4))
		{
			b_stopKey_falling=TRUE;
			p_b_KeyFalling=&b_stopKey_falling;
			p_b_KeyFalled=&b_stopKey_falled;	
			p_KeyFallingCnt=&stopKey_falling_cnt;
			p_b_KeyRaised=&b_stopKey_pressed;
		}
		else
		{
			b_stopKey_raising=TRUE;
			p_b_KeyRaising=&b_stopKey_raising;
			p_b_KeyRaised=&b_stopKey_raised;
			p_KeyRaisingCnt=&stopKey_raising_cnt;
		}
	}  
	EXTI_ClearITPendingBit(EXTI_Line4);
//	EXTI_ClearFlag(EXTI_Line4);
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)  
	{ 
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5))
		{
			b_startKey_falling=TRUE;
			p_b_KeyFalling=&b_startKey_falling;
			p_b_KeyFalled=&b_startKey_falled;	
			p_KeyFallingCnt=&startKey_falling_cnt;
			p_b_KeyRaised=&b_startKey_pressed;
		}
		else
		{
			b_startKey_raising=TRUE;
			p_b_KeyRaising=&b_startKey_raising;
			p_b_KeyRaised=&b_startKey_raised;
			p_KeyRaisingCnt=&startKey_raising_cnt;
		}
	}  
	else if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
	{
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))
		{
			b_nextKey_falling=TRUE;
			p_b_KeyFalling=&b_nextKey_falling;
			p_b_KeyFalled=&b_nextKey_falled;	
			p_KeyFallingCnt=&nextKey_falling_cnt;
			p_b_KeyRaised=&b_nextKey_pressed;
		}
		else
		{
			b_nextKey_raising=TRUE;
			p_b_KeyRaising=&b_nextKey_raising;
			p_b_KeyRaised=&b_nextKey_raised;
			p_KeyRaisingCnt=&nextKey_raising_cnt;
		}
	}
	
	EXTI_ClearITPendingBit(EXTI_Line5);
	EXTI_ClearITPendingBit(EXTI_Line6);
//	EXTI_ClearFlag(EXTI_Line5);
//	EXTI_ClearFlag(EXTI_Line6);
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
