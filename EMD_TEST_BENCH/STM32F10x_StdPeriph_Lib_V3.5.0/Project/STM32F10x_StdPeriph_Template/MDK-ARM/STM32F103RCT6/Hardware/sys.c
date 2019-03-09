#include "sys.h"
//#include "includes.h"					//ucos ʹ��	
#include "stm32f10x.h"
#include "stm32f10x_flash.h"
//#include "vent_task.h"
#include "config_module.h"
#include "vent_module.h"
extern CONFIG_TYPE cur_config;
////////////////////////////////////////////////////////////////////////////////// 	 
static u8  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������

extern u8 dither_enable;

//systick�жϷ�����,ʹ��ucosʱ�õ�
void SysTick_Handler(void)
{				   
	OSIntEnter();		//�����ж�
	OSTimeTick();       //����ucos��ʱ�ӷ������               
	OSIntExit();        //���������л����ж�
	
	#if 0
	if(HIGH_PRESS_MODE == cur_config.run_mode && cur_config.mode_status != PAUSE_STATUS)
	{
		u16 air_pv_ctl_temp;
		u16 oxy_pv_ctl_temp;
		
		if(OSTime%20==0)
		{
			dither_enable = !dither_enable;
			
			air_pv_ctl_temp = get_air_pv_dac();
			oxy_pv_ctl_temp = get_oxy_pv_dac();
			
			//air_pv_ctl_temp = 3000;
			//oxy_pv_ctl_temp = 3000;
			
			//oxy_pv_ctl_temp = 3500;
			
			//���ӷ���Ϊ6%��Ƶ��Ϊ25HZ�Ķ���		
			air_pv_ctl_temp = air_pv_ctl_temp + (air_pv_ctl_temp >> 4)*dither_enable;
			oxy_pv_ctl_temp = oxy_pv_ctl_temp + (oxy_pv_ctl_temp >> 4)*dither_enable;
			
			//���ñ��������
			//vent_module_set_pv_val(AIR_PV, air_pv_ctl_temp);
			//vent_module_set_pv_val(OXY_PV, oxy_pv_ctl_temp);
			
			if(oxy_pv_ctl_temp > PWM_FREQ-1)
			{
				//
				oxy_pv_ctl_temp = PWM_FREQ-1;
			}
			
			pv_pwm_ctl(FALSE, oxy_pv_ctl_temp);
		}
	}
	#endif
}

//��ʼ���ӳٺ���
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void SysTick_init(void)	 
{
	u32 reload;

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	fac_us=64000000/8000000;	//Ϊϵͳʱ�ӵ�1/8  
	 
	reload=64000000/8000000;		//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/OS_TICKS_PER_SEC;//����OS_TICKS_PER_SEC�趨���ʱ��
							//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
	fac_ms=1000/OS_TICKS_PER_SEC;//����ucos������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 	//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    
}								    

void RCC_HSI_Configuration(void)
{
	RCC_DeInit();
	
	RCC_HSICmd(ENABLE);
	
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);//�ȴ�HSI����
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_PCLK1Config(RCC_SYSCLK_Div1);
	FLASH_SetLatency(FLASH_Latency_2);
	
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	
	while(RCC_GetSYSCLKSource()!= 0x00);
}

//����ʱ��64MHz
void init_hsi(void)
{
	RCC->CR |= (uint32_t)0x00000001;
	
	RCC->CFGR |= (uint32_t)RCC_CFGR_PLLSRC_HSI_Div2;
	
	RCC->CFGR |= (uint32_t)RCC_CFGR_PLLMULL16;
	
	RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
	
	RCC->CR |= RCC_CR_PLLON;
	
	while((RCC->CR & RCC_CR_PLLRDY) == 0);
	
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
	
	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;
	
	while((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08);
}

void init_sys_clock(void)
{
	RCC_HSI_Configuration();
	init_hsi();
	SysTick_init();	     //��ʱ��ʼ��	
}

void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
}

/********************************************************************
*���±���������һ�㲻��
******************************************************************/


void delay_init(u8 SYSCLK)
{
#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	u32 reload;
#endif
 	SysTick->CTRL&=~(1<<2);	//SYSTICKʹ���ⲿʱ��Դ	
//SysTick->CTRL|=(1<<2);  
	fac_us=SYSCLK/8;		//�����Ƿ�ʹ��ucos,fac_us����Ҫʹ��
	    
#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	reload=SYSCLK/8;		//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/OS_TICKS_PER_SEC;//����OS_TICKS_PER_SEC�趨���ʱ��
							//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
	fac_ms=1000/OS_TICKS_PER_SEC;//����ucos������ʱ�����ٵ�λ	   
	SysTick->CTRL|=1<<1;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 	//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=1<<0;   	//����SYSTICK    
#else
	fac_ms=(u16)fac_us*1000;//��ucos��,����ÿ��ms��Ҫ��systickʱ����   
#endif
}

void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)	 
{ 	   	 
	SCB->VTOR = NVIC_VectTab|(Offset & (u32)0x1FFFFF80);//����NVIC��������ƫ�ƼĴ���
	//���ڱ�ʶ����������CODE��������RAM��
}

void MYRCC_DeInit(void)
{	
 	RCC->APB1RSTR = 0x00000000;//��λ����			 
	RCC->APB2RSTR = 0x00000000; 
	  
  	RCC->AHBENR = 0x00000014;  //˯��ģʽ�����SRAMʱ��ʹ��.�����ر�.	  
  	RCC->APB2ENR = 0x00000000; //����ʱ�ӹر�.			   
  	RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //ʹ���ڲ�����ʱ��HSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //��λSW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //��λHSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //��λHSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //��λPLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //�ر������ж�		 
	//����������				  
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(0x20000000, 0x0);
#else   
	MY_NVIC_SetVectorTable(0x08000000,0x0);
#endif
}

void Stm32_Clock_Init(u8 PLL)
{
	unsigned char temp=0;   
	MYRCC_DeInit();		  //��λ������������
//RCC->CR, clock control reg.
//  	RCC->CR|=0x00010000;  //�ⲿ����ʱ��ʹ��HSEON
// 	while(!(RCC->CR>>17));//�ȴ��ⲿʱ�Ӿ���  -HSERDY
  	RCC->CR|=0x00000001;  //HSI����ʱ��ʹ��-HSION
 	while(!(RCC->CR>>1));//�ȴ�ʱ�Ӿ���  -HSIRDY  
//RCC->CFGR, clock config reg.  
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1; --APB1 2 div
	PLL-=2;//����2����λ
	RCC->CFGR|=PLL<<18;   //����PLLֵ 2~16 -multiplication factor
//	RCC->CFGR|=1<<16;	  //PLLSRC ON 1:HSE, 0:HSI/2
  RCC->CFGR&=~(1<<16);
	FLASH->ACR|=0x32;	  //FLASH 2����ʱ����

	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//�ȴ�PLL����
	RCC->CFGR|=0x00000002;//PLL��Ϊϵͳʱ��	 
	while(temp!=0x02)     //�ȴ�PLL��Ϊϵͳʱ�����óɹ�
	{   
		temp=RCC->CFGR>>2;
		temp&=0x03;
	}    
}

void delay_us(u32 nus)  //nus<=(2^24)/fac_us, ���򽫵�����ʱʱ�䲻׼.
{		
	#if 1
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD��ֵ	    	 
	ticks=nus*fac_us; 			//��Ҫ�Ľ�����	  		 
	tcnt=0;
	OSSchedLock();				//��ֹucos���ȣ���ֹ���us��ʱ
	told=SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
	OSSchedUnlock();			//����ucos����	 
	#else
	u32 temp;
	SysTick->LOAD = 9*nus;
	SysTick->VAL=0X00;//��ռ�����
	SysTick->CTRL=0X01;//ʹ�ܣ����������޶����������ⲿʱ��Դ
	do
	{
	temp=SysTick->CTRL;//��ȡ��ǰ������ֵ
	}while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽��
		 SysTick->CTRL=0x00; //�رռ�����
		SysTick->VAL =0X00; //��ռ�����
	#endif
}

void delay_ms(u16 nms)
{	
	delay_us((u32)(nms*1000));	//��ͨ��ʽ��ʱ 
}