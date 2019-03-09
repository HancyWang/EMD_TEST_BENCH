/****************************************************
*版权所有：深圳永胜宏基医疗器械有限公司
*文件名：lcd.c
*功能说明：初始化系统时钟、创建任务等
*作者：软件组
*生成日期：2017/02/27
*****************************************************/

/*****************************************************
*头文件包含
******************************************************/
//#include "main.h"
//#include "includes.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
#include "delay.h"

/******************************************************
*局部变量定义
******************************************************/

/******************************************************
*全局变量定义
******************************************************/

/******************************************************
*内部函数声明
******************************************************/

/******************************************************
*其他定义
******************************************************/

/******************************************************
*内部函数定义
******************************************************/





//-------------------------LCD--------------------------//
//???????
//dat:??????/??
//cmd:??/???? 0,????;1,????;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	//OSSchedLock();
	OLED_RS=cmd;
  DATAOUT(dat);	    
	OLED_CS=0;	   
	OLED_WR=0;	 
	OLED_WR=1;
	OLED_CS=1;	  
	OLED_RS=1;	 
	//OSSchedUnlock();	
}

void lcd_ctrl_write_color(unsigned int color)
{
	OLED_CS=0;
	OLED_RS=1;
  DATAOUT(color >> 8);  

//??
	OLED_WR=0;	 
	OLED_WR=1;
  DATAOUT(color & 0xff); 
	OLED_WR=0;	 
	OLED_WR=1;	  
	OLED_RS=1;
	OLED_CS=1; 
}
//x,y:??
//POINT_COLOR:?????
void lcd_draw_point(u8 x,u16 y,unsigned int color)
{
		 OLED_WR_Byte(0x2A,OLED_CMD);	 //column address set
	   OLED_WR_Byte(0,OLED_DATA);	  //XS
     OLED_WR_Byte(x,OLED_DATA);
     OLED_WR_Byte(0,OLED_DATA);   //XE
     OLED_WR_Byte(x,OLED_DATA);
	   
     OLED_WR_Byte(0x2B,OLED_CMD); //row address set
	   OLED_WR_Byte(y>>8,OLED_DATA);	  //YS 
     OLED_WR_Byte(y&0xff,OLED_DATA);
     OLED_WR_Byte(y>>8,OLED_DATA);   //YE
     OLED_WR_Byte(y&0xff,OLED_DATA);

     OLED_WR_Byte(0x2C,OLED_CMD);	//memory write
	   lcd_ctrl_write_color(color); //????GRAM
}

//m^n??
//???:m^n??.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	

//control write area. wait for write.
void  lcd_crtl_write_area(unsigned char SX,unsigned char EX,unsigned int SY,unsigned int EY)
{
	//OSSchedLock();

	   OLED_WR_Byte(0x2A,OLED_CMD);
	   OLED_WR_Byte(0,OLED_DATA);
     OLED_WR_Byte(SX,OLED_DATA);
     OLED_WR_Byte(0,OLED_DATA);
     OLED_WR_Byte(EX,OLED_DATA);  

     OLED_WR_Byte(0x2B,OLED_CMD);
	   OLED_WR_Byte(SY>>8,OLED_DATA);
     OLED_WR_Byte(SY & 0xff,OLED_DATA);
	   OLED_WR_Byte(EY>>8,OLED_DATA);
     OLED_WR_Byte(EY & 0xff,OLED_DATA);
	
     OLED_WR_Byte(0x2C,OLED_CMD);	
	//OSSchedUnlock();	
} 

//??
void Draw_Bias_Line(unsigned char start_x,unsigned int start_y,unsigned char end_x,unsigned int end_y,unsigned int color, unsigned char linew,unsigned char angle)
{
	unsigned int i,j,w;
	w=end_x-start_x;
//	h=end_y-start_y;
	for(j = 0; j<=linew; j++)
	{
		for(i = 0; i<=w-5;i++)
		{
			lcd_draw_point((start_x+j-1+i),(start_y+i+2),color);
			
		}
	}
	for(j = 0; j<=linew; j++)
	{
		for(i = 0; i<=w-5;i++)
		{
			lcd_draw_point((end_x-j-2-i),(start_y+i+2),color);
		}
	}
} 

//?????
void lcd_draw_rect_real(unsigned char Start_X, unsigned int Start_Y,unsigned char End_X, unsigned int End_Y,unsigned int color)
{
 	   unsigned int i,j,w,h;
//			OSSchedLock();
 	   w=End_X-Start_X;
 	   h=End_Y-Start_Y;
	   lcd_crtl_write_area(Start_X,End_X,Start_Y,End_Y);
     for (j=0; j<=h;j++)
     {
 	     for(i=0;i<=w;i++)
 	     {
 	   	    lcd_ctrl_write_color(color);
 	     }
 	   }
//		 OSSchedUnlock();
}

void lcd_draw_rect(unsigned char Start_X, unsigned int Start_Y,unsigned char End_X, unsigned int End_Y,unsigned int color, unsigned char LineW)
{
 	   unsigned int i,j,w,h;
	
 	   w=End_X-Start_X;
 	   h=End_Y-Start_Y;
 	   //?????
	   lcd_crtl_write_area(Start_X,End_X,Start_Y,Start_Y+LineW-1);
     for (j=0; j<LineW;j++)
     {
 	     for(i=0;i<=w;i++)
 	     {
 	   	    lcd_ctrl_write_color(color);
 	     }
 	   }
	   lcd_crtl_write_area(Start_X,End_X,End_Y-LineW+1,End_Y);
     for (j=0; j<LineW;j++)
     {
 	     for(i=0;i<=w;i++)
 	     {
 	   	      lcd_ctrl_write_color(color);
 	     }
 	   }
	   lcd_crtl_write_area(Start_X,Start_X+LineW-1,Start_Y,End_Y);
     for (j=0; j<=h;j++)
     {
 	     for(i=0;i<LineW;i++)
 	     {
 	   	     lcd_ctrl_write_color(color);
 	     }
 	   }
	   lcd_crtl_write_area(End_X-LineW+1,End_X,Start_Y,End_Y);
     for (j=0; j<=h;j++)
     {
 	     for(i=0;i<LineW;i++)
 	     {
 	   	    lcd_ctrl_write_color(color);
 	     }
 	   }
 }

 //????
void SdCmd(u8 cmd)
{
	OLED_WR_Byte(cmd,OLED_CMD);
}

//????
void SdData(u8 data)
{
	OLED_WR_Byte(data,OLED_DATA);
}
 
 
void LCD_Initial(void)
{
 
	OLED_RST = 0;
	//OSTimeDlyHMSM(0, 0, 0, 20); // Delay 1ms
	delay_ms(20);
	OLED_RST = 1;
	//OSTimeDlyHMSM(0, 0, 0, 800); // Delay 10ms // This delay time is necessary
	delay_ms(800);
	
	SdCmd(0x11);//Wake up
//	OSTimeDlyHMSM(0, 0, 0,120); //Delay 120ms
	delay_ms(120);
	SdCmd(0x35);SdData(0x00);//TE ON   60Hz
//	SdCmd(0x36);SdData(0x60);//Seting RAM data access
			
	//LCD_ILI9341V_CMD(0x36);
	//LCD_ILI9341V_Parameter(0x00); //RTN
	//LCD_ILI9341V_CMD(0x21); //????	 
	SdCmd(0x36);
	SdData(0x00);
	//SdCmd(0x21);

  SdCmd(0xb2);  //Porch setting
	SdData(0x0c);
	SdData(0x0c);
	SdData(0x00);
	SdData(0x33);
	SdData(0x33);

	SdCmd(0xb7);  //Gate Control
	SdData(0x35); //VGH=13.26V, VGL=-10.43V

	SdCmd(0xbb); //vcom setting
	SdData(0x19);

	SdCmd(0xc0); //LCM Control
	SdData(0x2c);

	SdCmd(0xc2);  //VDV and VRH Command Enable	
	SdData(0x01);

	SdCmd(0xc3); //VRH Set, VAP(GVDD) & VAN(GVCL)
	SdData(0x1d); 

	SdCmd(0xc4);  //VDV Set, 
	SdData(0x20); //VDV=0V

	SdCmd(0xc6);//Frame rate control in normal mode
	SdData(0x0f);//dot inversion & 60Hz

	SdCmd(0xd0);   //Power control 1
	SdData(0xa4);
	SdData(0xa1); //AVDD=6.8V; AVCL=-4.8V; VDDS=2.3V

	SdCmd(0xe8);
	SdData(0x83);
//--------------------------------ST7789VI gamma setting---------------------------------------//
    SdCmd(0xe0);
	SdData(0xd2);
	SdData(0x11);
	SdData(0x16);
	SdData(0x0d);
	SdData(0x0d);
	SdData(0x38);
	SdData(0x3d);
	SdData(0x44);
	SdData(0x4c);
	SdData(0x1c);
	SdData(0x15);
	SdData(0x15);
	SdData(0x1c);
	SdData(0x20);


	SdCmd(0xe1);
	SdData(0xd2);
	SdData(0x11);
	SdData(0x16);
	SdData(0x0d);
	SdData(0x0d);
	SdData(0x38);
	SdData(0x3d);
	SdData(0x44);
	SdData(0x4c);
	SdData(0x1c);
	SdData(0x15);
	SdData(0x15);
	SdData(0x1c);
	SdData(0x20);

	SdCmd(0x3a);SdData(0x55); // 8/16bit,65k colors
	SdCmd(0x29);             //Display on

	//OSTimeDlyHMSM(0, 0, 0,10);
	delay_ms(10);

	SdCmd(0x2a);
	SdData(0x00);
	SdData(0x00);
	SdData(0x00);
	SdData(0xEF);

	SdCmd(0x2b);
	SdData(0x00);
	SdData(0x00);
	SdData(0x01);
	SdData(0x3F);

//	OSTimeDlyHMSM(0, 0, 0,120);
	delay_ms(120);
	SdCmd(0x2c);
}

void init_lcd(void)
{ 	 
#if 0	
// 	GPIO_InitTypeDef  GPIO_InitStructure;
// 	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	

//	//reset
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOB,&GPIO_InitStructure);	  
//	GPIO_SetBits(GPIOB,GPIO_Pin_15);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	 //PD3,PD6????  
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??50MHz
// 	GPIO_Init(GPIOE, &GPIO_InitStructure);	  //???GPIOD3,6
//	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;	 //PD3,PD6????  
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??50MHz
// 	GPIO_Init(GPIOD,&GPIO_InitStructure);	  //???GPIOD3,6
// 	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);	//PD3,PD6 ???	
#endif
  BLEN=0;  
	#if 0
	OLED_RST=0;			  		//??
	delay_ms(20);
	OLED_RST=1;
  delay_ms(800);  

  OLED_WR_Byte(0x11,OLED_CMD);//??  - // Start Initial Sequence /Driving ability Setting
  OLED_WR_Byte(0x3A,OLED_CMD);  OLED_WR_Byte(0x55,OLED_DATA); //65k?,16bit -GDOFF
//OLED_WR_Byte(0x36,OLED_CMD);  OLED_WR_Byte(0x60,OLED_DATA); //??: MY=1, MX=1, MV=1(RowColEx), ML=0(normal), RGB=0(RGB)
  OLED_WR_Byte(0x36,OLED_CMD);  OLED_WR_Byte(0x00,OLED_DATA);   //RTN
  //OLED_WR_Byte(0x21,OLED_CMD); //???? 
  OLED_WR_Byte(0x29,OLED_CMD);  //???
	BLEN=0;
	#endif
	
	LCD_Initial();
  
  //Draw_Rectangle_Real(0,0,239,319,BLACK18);//clear
	lcd_draw_rect_real(0,0,239,319,0);//clear
  BLEN=0;
}  

void set_lcd_backlight(u8 sw)
{
	if(sw)
		BLEN=1;
	else
		BLEN=0;
}
