/****************************************************
*��Ȩ���У�������ʤ���ҽ����е���޹�˾
*�ļ�����display_module.c
*����˵����������ʾģ��
*���ߣ������
*�������ڣ�2017/02/27
*****************************************************/
#ifndef  __DISPLAY_MODULE_H_
#define  __DISPLAY_MODULE_H_

/*****************************************************
*ͷ�ļ�����
******************************************************/
//#include "main.h"
//#include "includes.h"
#include <stm32f10x.h> 
#include "lang.h"
/******************************************************
*�ڲ��궨��
******************************************************/
//#define DEBUG_FLOW_DISPLAY



#define RED       	   0xf800    //��ɫ
#define GREEN     	   0x07e0    //��ɫ
#define BLUE      	   0x001f    //��ɫ
#define WHITE     	   0xffff    //��ɫ
#define BLACK     	   0x0000    //��ɫ
#define GRAY     	   	 0x7BEF    //��ɫ  0x630c 
#define YELLOW     	   0xffe0    //��ɫ
#define BRIGHT_BLUE    0x07ff    //��ɫ
#define AIR_BLUE       0x81C0    //���� xx
#define PURPLE    	   0x624F 	 //0x07E0    //��ɫ	 
#define BLACKRED  	   0xF210    //�ں�ɫ xx

#define BACK_COLOR     PURPLE   //����ɫ
/******************************************************
*�ڲ����Ͷ���
******************************************************/
typedef struct{
	int a;
}DISPLAY_MODULE;


/******************************************************
*�ⲿ��������
******************************************************/
void LCD_CtrlWrite_Color(unsigned int color);

//��
void display_module_clear(void);
void display_module_draw_rect_real(unsigned char Start_X, unsigned int Start_Y,\
				unsigned char End_X, unsigned int End_Y,unsigned int color);  //��ʵ�ľ���
//�ַ�������
//void display_module_show_str(u8 x, u16 y,u8 height_x,u16 width_y, u8 *p, u8 mode, u32 color); //��ʾ�ַ���
void display_module_show_char(u8 x,u16 y,u8 size,u8 num,u8 mode,u32 color);
void display_module_show_string(u16 x, u16 y,u8 height, u8 *p, u8 mode, u32 color);
void display_module_show_string_underline_adjust(u16 x, u16 y,u8 height, u8 *p, u8 mode, u32 color, u8 underline, u8 adjust);
void display_module_show_num_30x56(unsigned char  X, unsigned int Y,unsigned char NUM,unsigned int color);
void display_module_show_num_2bit(u8 x, u16 y, u8 width, u8 NUM, u32 color, u8 underline, u8 adjust);
void display_module_show_num10_2bit_17x40(u8 x, u16 y, u16 NUM, u32 color);
void display_module_show_num10_2bit_30x56(u8 x, u16 y, u16 NUM, u32 color, u8 underline, u8 adjust);
void display_module_show_num_3bit_30x56(u8 x, u16 y, u16 NUM, u32 color, u8 underline, u8 adjust);
void display_module_show_num_3bit_10X16(u8 x, u16 y, u8 NUM, u32 color, u8 underline, u8 adjust);
void display_module_show_num_1bit_10X16(unsigned char  X, unsigned int Y,unsigned char NUM,unsigned int color);
void display_module_show_rt_fio2(u8 x, u16 y, u16 NUM, u32 color, u8 alarm_flag);
void display_module_show_rt_flow(u8 x, u16 y, u16 NUM, u32 color);
//ͼ��
void display_module_show_img_bat(unsigned char  X, unsigned int Y,unsigned char active_flag, unsigned int color);
void display_module_show_img_bat_x(unsigned char  X, unsigned int Y,unsigned int color);
void display_module_show_img_seting(unsigned char  X, unsigned int Y,unsigned char active_flag, unsigned int color);
void display_module_show_img_bell(unsigned char  X, unsigned int Y,unsigned char active_flag, unsigned int color);
void display_module_show_img_circle48x48(unsigned char  X, unsigned int Y,unsigned int color);
void display_module_show_img_pause(unsigned char  X, unsigned int Y,unsigned int color);
void display_module_show_img_start(unsigned char  X, unsigned int Y,unsigned int color);
void display_module_show_img_lock(unsigned char  X, unsigned int Y,unsigned int color);
void display_module_show_img_speaker(unsigned char  X, unsigned int Y,unsigned int color);
void display_module_show_img_volume(unsigned char  X, unsigned int Y, u8 volume, unsigned int color);
void display_module_show_img_valve(unsigned char  X, unsigned int Y,unsigned int color);
void display_module_show_img_logo(unsigned char  X, unsigned int Y,unsigned int color);
void display_module_show_img_bat_x(unsigned char  X, unsigned int Y,unsigned int color);
void display_module_show_img_batcharge14x40(unsigned char  X, unsigned int Y, unsigned int color);
void display_module_show_img_batcharge14x40_back_color(unsigned char  X, unsigned int Y, unsigned int color, unsigned int back_color);

//����
void display_module_show_chinese_16x16(u16 x, u16 y, u8* str,u32 color);
void display_module_show_chinese_24x24(u16 x, u16 y, u8* str,u32 color);
void display_module_show_chinese_20x20(u16 x, u16 y, u8* str,u32 color);
void display_module_show_chinese_24x24_back_color(u16 x, u16 y, u8* str, u32 color, u32 back_color);

//��ʾ����
void display_module_show_lang(u16 x, u16 y, u8 height, LANG_STR_INDEX lang_index, u32 color);
void display_module_show_lang_back_color(u16 x, u16 y, u8 height, LANG_STR_INDEX lang_index, u32 color, u32 back_color);
#endif
