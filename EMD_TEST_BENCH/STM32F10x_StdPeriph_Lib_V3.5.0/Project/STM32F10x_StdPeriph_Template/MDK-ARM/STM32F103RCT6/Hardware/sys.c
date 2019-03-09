#include "sys.h"
//#include "includes.h"					//ucos 使用	
#include "stm32f10x.h"
#include "stm32f10x_flash.h"
//#include "vent_task.h"
#include "config_module.h"
#include "vent_module.h"
extern CONFIG_TYPE cur_config;
////////////////////////////////////////////////////////////////////////////////// 	 
static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数

extern u8 dither_enable;

//systick中断服务函数,使用ucos时用到
void SysTick_Handler(void)
{				   
	OSIntEnter();		//进入中断
	OSTimeTick();       //调用ucos的时钟服务程序               
	OSIntExit();        //触发任务切换软中断
	
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
			
			//叠加幅度为6%，频率为25HZ的抖动		
			air_pv_ctl_temp = air_pv_ctl_temp + (air_pv_ctl_temp >> 4)*dither_enable;
			oxy_pv_ctl_temp = oxy_pv_ctl_temp + (oxy_pv_ctl_temp >> 4)*dither_enable;
			
			//设置比例阀输出
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

//初始化延迟函数
//当使用ucos的时候,此函数会初始化ucos的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void SysTick_init(void)	 
{
	u32 reload;

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	fac_us=64000000/8000000;	//为系统时钟的1/8  
	 
	reload=64000000/8000000;		//每秒钟的计数次数 单位为K	   
	reload*=1000000/OS_TICKS_PER_SEC;//根据OS_TICKS_PER_SEC设定溢出时间
							//reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右	
	fac_ms=1000/OS_TICKS_PER_SEC;//代表ucos可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 	//每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK    
}								    

void RCC_HSI_Configuration(void)
{
	RCC_DeInit();
	
	RCC_HSICmd(ENABLE);
	
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);//等待HSI就绪
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_PCLK1Config(RCC_SYSCLK_Div1);
	FLASH_SetLatency(FLASH_Latency_2);
	
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	
	while(RCC_GetSYSCLKSource()!= 0x00);
}

//配置时钟64MHz
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
	SysTick_init();	     //延时初始化	
}

void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
}

/********************************************************************
*以下保留函数，一般不用
******************************************************************/


void delay_init(u8 SYSCLK)
{
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	u32 reload;
#endif
 	SysTick->CTRL&=~(1<<2);	//SYSTICK使用外部时钟源	
//SysTick->CTRL|=(1<<2);  
	fac_us=SYSCLK/8;		//不论是否使用ucos,fac_us都需要使用
	    
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	reload=SYSCLK/8;		//每秒钟的计数次数 单位为K	   
	reload*=1000000/OS_TICKS_PER_SEC;//根据OS_TICKS_PER_SEC设定溢出时间
							//reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右	
	fac_ms=1000/OS_TICKS_PER_SEC;//代表ucos可以延时的最少单位	   
	SysTick->CTRL|=1<<1;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 	//每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL|=1<<0;   	//开启SYSTICK    
#else
	fac_ms=(u16)fac_us*1000;//非ucos下,代表每个ms需要的systick时钟数   
#endif
}

void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)	 
{ 	   	 
	SCB->VTOR = NVIC_VectTab|(Offset & (u32)0x1FFFFF80);//设置NVIC的向量表偏移寄存器
	//用于标识向量表是在CODE区还是在RAM区
}

void MYRCC_DeInit(void)
{	
 	RCC->APB1RSTR = 0x00000000;//复位结束			 
	RCC->APB2RSTR = 0x00000000; 
	  
  	RCC->AHBENR = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.	  
  	RCC->APB2ENR = 0x00000000; //外设时钟关闭.			   
  	RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //使能内部高速时钟HSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //关闭所有中断		 
	//配置向量表				  
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(0x20000000, 0x0);
#else   
	MY_NVIC_SetVectorTable(0x08000000,0x0);
#endif
}

void Stm32_Clock_Init(u8 PLL)
{
	unsigned char temp=0;   
	MYRCC_DeInit();		  //复位并配置向量表
//RCC->CR, clock control reg.
//  	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON
// 	while(!(RCC->CR>>17));//等待外部时钟就绪  -HSERDY
  	RCC->CR|=0x00000001;  //HSI高速时钟使能-HSION
 	while(!(RCC->CR>>1));//等待时钟就绪  -HSIRDY  
//RCC->CFGR, clock config reg.  
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1; --APB1 2 div
	PLL-=2;//抵消2个单位
	RCC->CFGR|=PLL<<18;   //设置PLL值 2~16 -multiplication factor
//	RCC->CFGR|=1<<16;	  //PLLSRC ON 1:HSE, 0:HSI/2
  RCC->CFGR&=~(1<<16);
	FLASH->ACR|=0x32;	  //FLASH 2个延时周期

	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//等待PLL锁定
	RCC->CFGR|=0x00000002;//PLL作为系统时钟	 
	while(temp!=0x02)     //等待PLL作为系统时钟设置成功
	{   
		temp=RCC->CFGR>>2;
		temp&=0x03;
	}    
}

void delay_us(u32 nus)  //nus<=(2^24)/fac_us, 否则将导致延时时间不准.
{		
	#if 1
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD的值	    	 
	ticks=nus*fac_us; 			//需要的节拍数	  		 
	tcnt=0;
	OSSchedLock();				//阻止ucos调度，防止打断us延时
	told=SysTick->VAL;        	//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;//时间超过/等于要延迟的时间,则退出.
		}  
	};
	OSSchedUnlock();			//开启ucos调度	 
	#else
	u32 temp;
	SysTick->LOAD = 9*nus;
	SysTick->VAL=0X00;//清空计数器
	SysTick->CTRL=0X01;//使能，减到零是无动作，采用外部时钟源
	do
	{
	temp=SysTick->CTRL;//读取当前倒计数值
	}while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达
		 SysTick->CTRL=0x00; //关闭计数器
		SysTick->VAL =0X00; //清空计数器
	#endif
}

void delay_ms(u16 nms)
{	
	delay_us((u32)(nms*1000));	//普通方式延时 
}