/****************************************************
*版权所有：深圳永胜宏基医疗器械有限公司
*文件名：display_module.c
*功能说明：界面显示模块
*作者：软件组
*生成日期：2017/02/27
*****************************************************/
#ifndef  __DISPLAY_MODULE_H_
#define  __DISPLAY_MODULE_H_

/*****************************************************
*头文件包含
******************************************************/
//#include "main.h"
//#include "includes.h"
#include <stm32f10x.h> 
#include "lang.h"
/******************************************************
*内部宏定义
******************************************************/
//#define DEBUG_FLOW_DISPLAY



#define RED       	   0xf800    //红色
#define GREEN     	   0x07e0    //绿色
#define BLUE      	   0x001f    //蓝色
#define WHITE     	   0xffff    //白色
#define BLACK     	   0x0000    //黑色
#define GRAY     	   	 0x7BEF    //灰色  0x630c 
#define YELLOW     	   0xffe0    //黄色
#define BRIGHT_BLUE    0x07ff    //青色
#define AIR_BLUE       0x81C0    //天蓝 xx
#define PURPLE    	   0x624F 	 //0x07E0    //紫色	 
#define BLACKRED  	   0xF210    //黑红色 xx

#define BACK_COLOR     PURPLE   //背景色
/******************************************************
*内部类型定义
******************************************************/
typedef struct{
	int a;
}DISPLAY_MODULE;


/******************************************************
*外部函数声明
******************************************************/
void LCD_CtrlWrite_Color(unsigned int color);

//块
void display_module_clear(void);
void display_module_draw_rect_real(unsigned char Start_X, unsigned int Start_Y,\
				unsigned char End_X, unsigned int End_Y,unsigned int color);  //画实心矩形
//字符、数字
//void display_module_show_str(u8 x, u16 y,u8 height_x,u16 width_y, u8 *p, u8 mode, u32 color); //显示字符串
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
//图标
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

//汉字
void display_module_show_chinese_16x16(u16 x, u16 y, u8* str,u32 color);
void display_module_show_chinese_24x24(u16 x, u16 y, u8* str,u32 color);
void display_module_show_chinese_20x20(u16 x, u16 y, u8* str,u32 color);
void display_module_show_chinese_24x24_back_color(u16 x, u16 y, u8* str, u32 color, u32 back_color);

//显示语言
void display_module_show_lang(u16 x, u16 y, u8 height, LANG_STR_INDEX lang_index, u32 color);
void display_module_show_lang_back_color(u16 x, u16 y, u8 height, LANG_STR_INDEX lang_index, u32 color, u32 back_color);
#endif
