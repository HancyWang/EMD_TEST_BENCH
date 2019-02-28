/*******************************************************************************
* ��Ŀ��� :  
* �汾��   :  1.0
* �ļ���   :  serial_port.c
* �������� :  
* ����     :  
* ����˵�� :  ���������
*******************************************************************************/
/*******************************************************************************
*                                 ͷ�ļ�����
*******************************************************************************/
#include "serial_port.h"
#include "hardware.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "datatype.h"

/*******************************************************************************
*                                 �ֲ�׃��
*******************************************************************************/

/*******************************************************************************
*                                 ȫ��׃��
*******************************************************************************/

/*******************************************************************************
*                                 �������x
*******************************************************************************/
/*******************************************************************************
* �������� : UARTInit
* �������� : ��ʼ������
* ������� : ��
* ������� : ��
* �������� : ��
*******************************************************************************/
void UARTInit(uint8_t* p_rec_buf, uint32_t rec_num)
{
	//1.RCC Configuration
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	
	//2.GPIO Configuration
	GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = UART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(UART_IO_PORT, &GPIO_InitStructure);
 
  /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = UART_TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(UART_IO_PORT, &GPIO_InitStructure);
	
	
	//3.DMA Configuration
	//3.1 DMA TX
	DMA_InitTypeDef DMA_InitStructure;
  /* USARTy_Tx_DMA_Channel (triggered by USARTy Tx event) Config */
  DMA_DeInit(UART_DMA_TX_CHANNEL);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART->DR);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = (uint32_t)0;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(UART_DMA_TX_CHANNEL, &DMA_InitStructure);
	//3.2 DMA RX
	DMA_DeInit(UART_DMA_RX_CHANNEL);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART->DR);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)p_rec_buf;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = rec_num;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(UART_DMA_RX_CHANNEL, &DMA_InitStructure);
	
	USART_DMACmd(UART, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
	
  DMA_Cmd(UART_DMA_RX_CHANNEL, ENABLE);
  DMA_Cmd(UART_DMA_TX_CHANNEL, ENABLE);
	
	//4.USART ����
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = UART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
  USART_Init(UART, &USART_InitStructure);
  USART_Cmd(UART, ENABLE);
}

/*******************************************************************************
* �������� : DMA_ReWorkCfg
* �������� : ����һ���Ѿ���ʼ����DMAͨ��, ���������乤��
* ������� : dma_Channel_x  ָ��DMAͨ����ָ��
*            mem_address    �������õ��ڴ��ַ
*            num            ���´������Ŀ
*******************************************************************************/
static void DMA_ReWorkCfg (DMA_Channel_TypeDef* dma_Channel_x, uint32_t mem_address, uint32_t num)
{
	dma_Channel_x->CCR &= 0xFFFFFFFE;                /* �Ƚ�ֹDMAxͨ�� */

	dma_Channel_x->CMAR = mem_address;               /* ���������ڴ��ַ */

	dma_Channel_x->CNDTR = num;                      /* ���´�����Ŀ */

	dma_Channel_x->CCR |= 0x00000001;                /* ʹ��DMAxͨ�� */
}

/*******************************************************************************
* �������� : UartSendNBytes
* �������� : ͨ��DMA���ƴӴ��ڷ�������
* ������� : ��
* ������� : ��
* �������� : ��
*******************************************************************************/
void UartSendNBytes (uint8_t *p_buf, uint32_t num)
{
	DMA_ReWorkCfg(UART_DMA_TX_CHANNEL, (uint32_t)p_buf, num);
}

/*******************************************************************************
* �������� : GetUartReceiverResidualCnt
* �������� : ȡ����pdc������ʣ�����
* ������� : ��
* ������� : ��
* �������� :����pdc������ʣ����� 
*******************************************************************************/
int32_t GetUartReceiverResidualCnt(void)
{
	return(DMA_GetCurrDataCounter(UART_DMA_RX_CHANNEL));
}



