/****************************************************
*��Ȩ���У�������ʤ���ҽ����е���޹�˾
*�ļ�����hardware.h
*����˵����Ӳ��������ʼ����ӿ�
*���ߣ�
*�������ڣ�
*****************************************************/
#ifndef  __LCD_H_
#define  __LCD_H_

/*****************************************************
*ͷ�ļ�����
******************************************************/
//#include "includes.h"
#include "stm32f10x.h"
//#include "sys.h"
#include "hardware.h"

/******************************************************
*�ڲ��궨��
******************************************************/
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 


#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //����


#define	BLEN 		 PBout(10) //LCD����
#define OLED_RD  PBout(11)
#define OLED_WR  PBout(12)
#define OLED_RS  PBout(13)
#define OLED_CS  PBout(14)
#define OLED_RST PBout(15)

//PE8~15,��Ϊ������
//#define DATAOUT(x) GPIOE->ODR=(GPIOE->ODR&0x00FF)|(((x)<<8)&0xFF00); //���  
#define DATAOUT(x) GPIOC->ODR=(GPIOC->ODR&0xFF00)|(((x))&0x00FF); //���  
  		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

/******************************************************
*�ڲ����Ͷ���
******************************************************/


/******************************************************
*�ⲿ��������
******************************************************/

void init_lcd(void);
void set_lcd_backlight(u8 sw);
void lcd_ctrl_write_color(unsigned int color);
void lcd_draw_point(u8 x,u16 y,unsigned int color);
void lcd_crtl_write_area(unsigned char SX,unsigned char EX,unsigned int SY,unsigned int EY);
void lcd_draw_rect_real(unsigned char Start_X, unsigned int Start_Y,unsigned char End_X, unsigned int End_Y,unsigned int color);
void lcd_draw_rect(unsigned char Start_X, unsigned int Start_Y,unsigned char End_X, unsigned int End_Y,unsigned int color, unsigned char LineW);
#endif
