/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
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
#include "stm32f10x.h"
//#include "stm32_eval.h"
//#include <stdio.h>
#include "hardware.h"
#include "delay.h"
#include "os_core.h"
#include "app.h"

#if 0
//#ifdef USE_STM32100B_EVAL
// #include "stm32100b_eval_lcd.h"
//#elif defined USE_STM3210B_EVAL
// #include "stm3210b_eval_lcd.h"
//#elif defined USE_STM3210E_EVAL
// #include "stm3210e_eval_lcd.h" 
//#elif defined USE_STM3210C_EVAL
// #include "stm3210c_eval_lcd.h"
//#elif defined USE_STM32100E_EVAL
// #include "stm32100e_eval_lcd.h"
//#endif

///** @addtogroup STM32F10x_StdPeriph_Template
//  * @{
//  */

///* Private typedef -----------------------------------------------------------*/
///* Private define ------------------------------------------------------------*/
//#ifdef USE_STM32100B_EVAL
//  #define MESSAGE1   "STM32 MD Value Line " 
//  #define MESSAGE2   " Device running on  " 
//  #define MESSAGE3   "  STM32100B-EVAL    " 
//#elif defined (USE_STM3210B_EVAL)
//  #define MESSAGE1   "STM32 Medium Density" 
//  #define MESSAGE2   " Device running on  " 
//  #define MESSAGE3   "   STM3210B-EVAL    " 
//#elif defined (STM32F10X_XL) && defined (USE_STM3210E_EVAL)
//  #define MESSAGE1   "  STM32 XL Density  " 
//  #define MESSAGE2   " Device running on  " 
//  #define MESSAGE3   "   STM3210E-EVAL    "
//#elif defined (USE_STM3210E_EVAL)
//  #define MESSAGE1   " STM32 High Density " 
//  #define MESSAGE2   " Device running on  " 
//  #define MESSAGE3   "   STM3210E-EVAL    " 
//#elif defined (USE_STM3210C_EVAL)
//  #define MESSAGE1   " STM32 Connectivity " 
//  #define MESSAGE2   " Line Device running" 
//  #define MESSAGE3   " on STM3210C-EVAL   "
//#elif defined (USE_STM32100E_EVAL)
//  #define MESSAGE1   "STM32 HD Value Line " 
//  #define MESSAGE2   " Device running on  " 
//  #define MESSAGE3   "  STM32100E-EVAL    "   
//#endif

///* Private macro -------------------------------------------------------------*/
///* Private variables ---------------------------------------------------------*/
// USART_InitTypeDef USART_InitStructure;

///* Private function prototypes -----------------------------------------------*/
//#ifdef __GNUC__
///* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//   set to 'Yes') calls __io_putchar() */
//#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */
#endif

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

	delay_init();
	os_init();
	
	os_create_task(init_task, OS_TRUE, INIT_TASK_ID);
	os_start();

	return 0;
}

#if 0
///**
//  * @brief  Retargets the C library printf function to the USART.
//  * @param  None
//  * @retval None
//  */
//PUTCHAR_PROTOTYPE
//{
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the USART */
//  USART_SendData(EVAL_COM1, (uint8_t) ch);

//  /* Loop until the end of transmission */
//  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
//  {}

//  return ch;
//}

//#ifdef  USE_FULL_ASSERT

///**
//  * @brief  Reports the name of the source file and the source line number
//  *         where the assert_param error has occurred.
//  * @param  file: pointer to the source file name
//  * @param  line: assert_param error line source number
//  * @retval None
//  */
//void assert_failed(uint8_t* file, uint32_t line)
//{ 
//  /* User can add his own implementation to report the file name and line number,
//     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

//  /* Infinite loop */
//  while (1)
//  {
//  }
//}
//#endif

///**
//  * @}
//  */
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
