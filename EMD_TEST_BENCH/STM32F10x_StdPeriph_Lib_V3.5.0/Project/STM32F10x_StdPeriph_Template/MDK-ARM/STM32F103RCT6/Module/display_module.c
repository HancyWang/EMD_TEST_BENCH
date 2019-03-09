/****************************************************
*��Ȩ���У�������ʤ���ҽ����е���޹�˾
*�ļ�����display_module.c
*����˵����������ʾģ��
*���ߣ������
*�������ڣ�2017/02/27
*****************************************************/

/*****************************************************
*ͷ�ļ�����
******************************************************/
//#include "main.h"
//#include "includes.h"
#include "hardware.h"
#include "fifo.h"
//#include "system_behavior.h"
#include "lcd.h"
#include "font.h"
#include "display_module.h"
#include "lang.h"
#include "lang_chinese.h"
#include "lang_english.h"

/******************************************************
*�ֲ���������
******************************************************/

/******************************************************
*ȫ�ֱ�������
******************************************************/

/******************************************************
*�ڲ���������
******************************************************/
static void LCD_DrawPoint(u8 x,u16 y,unsigned int color);
static void LCD_ShowChar(u8 x,u16 y,u8 size,u8 num,u8 mode,u32 color);
static void DISP_FNT8X16(u8  X, u16 Y,u8 NUM,u8 mode,u32 color);
static void LCD_CrtlWrite_Area(unsigned char SX,unsigned char EX,unsigned int SY,unsigned int EY);
static void LCD_CtrlWrite_Color(unsigned int color);
static void DISP_NUM_10X16(unsigned char  X, unsigned int Y,unsigned char NUM,unsigned int color);
static void DISP_NUM_30X56(unsigned char  X, unsigned int Y,unsigned char NUM,unsigned int color);
void DISP_FNT12X24_back_color(u8 X, u16 Y,u8 NUM,u8 mode,u32 color, u32 back_color);
/******************************************************
*��������
******************************************************/


/******************************************************
*�ڲ���������
******************************************************/
static void LCD_CtrlWrite_Color(unsigned int color)
{
	lcd_ctrl_write_color(color);
}

void  LCD_CrtlWrite_Area(unsigned char SX,unsigned char EX,unsigned int SY,unsigned int EY)
{
	lcd_crtl_write_area(SX,EX,SY,EY);
}

//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u8 x,u16 y,unsigned int color)
{
	lcd_draw_point(x, y, color);
}

// ����ķ�ʽ��ʾ16*8 ASC�ַ�  
//X���� 0-239
//Y���� 0-319
//NUM �ַ�
//mode:���ӷ�ʽ(1)��д����ɫ  ���Ƿǵ��ӷ�ʽ(0)д����ɫ
//color�ַ�����ɫ	
void DISP_FNT8X16(u8  X, u16 Y,u8 NUM,u8 mode,u32 color)
{
   u8  w, j;
   u16 F_y;
   u8  F_x;

   F_x = X;
   F_y = Y;	 

//  LCD_CrtlWrite_Area(X,X+15,Y,Y+7);


//   NUM=NUM-' ';//�õ�ƫ�ƺ��ֵ		   
//   for(w=0;w<8;w++) //fit the font library.
//   {
//     for(j=0;j<8;j++)
//     {
//       if((asc2_1608[NUM][w*2+1]&(0x01<<j))!=0)
//       {
//        if(mode == 0)
//          LCD_CtrlWrite_Color(color); //����д
//        else
//          LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM         
//       }
//			 else
//			 {
//				 if(mode == 0)
//						LCD_CtrlWrite_Color(BACK_COLOR);
//			 }
//			 F_x++;				
//     } 
//     for(j=0;j<8;j++)
//     {
//       if((asc2_1608[NUM][w*2]&(0x01<<j))!=0)
//        {
//          if(mode == 0)
//            LCD_CtrlWrite_Color(color); //����д
//          else
//            LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM				
//        }
//				else
//        {
//          if(mode == 0)
//           LCD_CtrlWrite_Color(BACK_COLOR);
//        }
//        F_x++;				
//     }
//     F_y++; 
//     F_x = X; 
//  } 
  LCD_CrtlWrite_Area(X,X+7,Y,Y+15);

   NUM=NUM-' ';//�õ�ƫ�ƺ��ֵ		   
   for(w=0;w<16;w++) //fit the font library.
   {
//     for(j=0;j<8;j++)
//     {
//       if((asc2_1608[NUM][w*2+1]&(0x01<<j))!=0)
//       {
//        if(mode == 0)
//          LCD_CtrlWrite_Color(color); //����д
//        else
//          LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM         
//       }
//			 else
//			 {
//				 if(mode == 0)
//						LCD_CtrlWrite_Color(BACK_COLOR);
//			 }
//			 F_x++;				
//     } 
     for(j=0;j<8;j++)
     {
       if((asc2_1608[NUM][w]&(0x01<<j))!=0)
        {
          if(mode == 0)
            LCD_CtrlWrite_Color(color); //����д
          else
            LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM				
        }
				else
        {
          if(mode == 0)
           LCD_CtrlWrite_Color(BACK_COLOR);
        }
        F_x++;				
     }
     F_y++; 
     F_x = X; 
  } 
}

//����ķ�ʽ ������ʾ���ӷ�ʽ ��ʾ24*12 ASC�ַ�  
//X���� 0-239
//Y���� 0-319
//NUM�ַ�
//mode:���ӷ�ʽ(1)��д����ɫ  ���Ƿǵ��ӷ�ʽ(0)д����ɫ �ٶȿ죬����ʽ
//color�ַ�����ɫ 
 void DISP_FNT12X24(u8 X, u16 Y,u8 NUM,u8 mode,u32 color)
{
	   u8  F_x,w,j;
	   unsigned int F_y; 	   

	   F_x = X;
	   F_y = Y;

//	   LCD_CrtlWrite_Area(X,X+23,Y,Y+11);

//	   NUM=NUM-' ';//�õ�ƫ�ƺ��ֵ		   
//	   for(w=0;w<12;w++)
//	   {
//	   	 for(j=0;j<8;j++)
//	 	   {
//	 	  	 if((asc2_2412[NUM][w*3+2]&(0x01<<j))!=0)
//         {
//          if(mode == 0)
//            LCD_CtrlWrite_Color(color); //����д
//          else
//	 	        	LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM				
//         }
//	 	     else
//	 	   	  {
//            if(mode == 0)
//	 	   	    	LCD_CtrlWrite_Color(BACK_COLOR);
//			    }
//				F_x++;					
//	 	   } 
//	 	   for(j=0;j<8;j++)
//	 	   {
//	 	  	 if((asc2_2412[NUM][w*3+1]&(0x01<<j))!=0)
//	 	     {
//          if(mode == 0)
//            LCD_CtrlWrite_Color(color); //����д
//          else
//	 	        LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM				
//         }
//	 	     else
//	 	   	  {
//            if(mode == 0)
//	 	   	    	LCD_CtrlWrite_Color(BACK_COLOR);
//			    }
//				F_x++;				
//	 	   } 
//		   for(j=0;j<8;j++)
//	 	   {
//	 	  	 if((asc2_2412[NUM][w*3]&(0x01<<j))!=0)
//	 	     {
//            if(mode == 0)
//              LCD_CtrlWrite_Color(color); //����д
//            else
//	 	        	LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM				
//         }
//	 	     else
//	 	   	  {
//            if(mode == 0)
//	 	   	    	LCD_CtrlWrite_Color(BACK_COLOR);
//			    }
//			   F_x++;
//	 	   }  
//		   F_y++; 
//		   F_x = X; 
//	 	} 

	   LCD_CrtlWrite_Area(X,X+15,Y,Y+23);

	   NUM=NUM-' ';//�õ�ƫ�ƺ��ֵ		   
	   for(w=0;w<24;w++)
	   {
	   	 for(j=0;j<8;j++)
	 	   {
	 	  	 if((asc2_2412[NUM][2*w]&(0x01<<j))!=0)
         {
          if(mode == 0)
            LCD_CtrlWrite_Color(color); //����д
          else
	 	        	LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM				
         }
	 	     else
	 	   	  {
            if(mode == 0)
	 	   	    	LCD_CtrlWrite_Color(BACK_COLOR);
			    }
				F_x++;					
	 	   }  
			 for(j=0;j<8;j++)
	 	   {
	 	  	 if((asc2_2412[NUM][2*w+1]&(0x01<<j))!=0)
         {
          if(mode == 0)
            LCD_CtrlWrite_Color(color); //����д
          else
	 	        	LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM				
         }
	 	     else
	 	   	  {
            if(mode == 0)
	 	   	    	LCD_CtrlWrite_Color(BACK_COLOR);
			    }
				F_x++;					
	 	   } 
		   F_y++; 
		   F_x = X; 
	 	} 
}

void DISP_FNT20X40(u8 X, u16 Y,u8 NUM,u8 mode,u32 color)
{
	u8  F_x,w,j;
	unsigned int F_y; 	   

	F_x = X;
	F_y = Y;
	
	LCD_CrtlWrite_Area(X,X+23,Y,Y+39);

	NUM=NUM-' ';//�õ�ƫ�ƺ��ֵ		   
	for(w=0;w<40;w++)
	{
		for(j=0;j<8;j++)
		{
		 if((asc2_4020[NUM][3*w]&(0x01<<j))!=0)
		 {
			if(mode == 0)
				LCD_CtrlWrite_Color(color); //����д
			else
					LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM				
		 }
		 else
			{
				if(mode == 0)
					LCD_CtrlWrite_Color(BACK_COLOR);
			}
		F_x++;					
		}  
		for(j=0;j<8;j++)
		{
		 if((asc2_4020[NUM][3*w+1]&(0x01<<j))!=0)
		 {
			if(mode == 0)
				LCD_CtrlWrite_Color(color); //����д
			else
					LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM				
		 }
		 else
			{
				if(mode == 0)
					LCD_CtrlWrite_Color(BACK_COLOR);
			}
		F_x++;					
		} 
		for(j=0;j<8;j++)
		{
		 if((asc2_4020[NUM][3*w+2]&(0x01<<j))!=0)
		 {
			if(mode == 0)
				LCD_CtrlWrite_Color(color); //����д
			else
					LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM				
		 }
		 else
			{
				if(mode == 0)
					LCD_CtrlWrite_Color(BACK_COLOR);
			}
		F_x++;					
		} 

	   
	 F_y++; 
	 F_x = X; 
	} 
}

//���ӱ���ɫ
 void DISP_FNT12X24_back_color(u8 X, u16 Y,u8 NUM,u8 mode,u32 color, u32 back_color)
{
	   u8  F_x,w,j;
	   unsigned int F_y; 	   

	   F_x = X;
	   F_y = Y;

	   LCD_CrtlWrite_Area(X,X+23,Y,Y+11);

	   NUM=NUM-' ';//�õ�ƫ�ƺ��ֵ		   
	   for(w=0;w<12;w++)
	   {
	   	 for(j=0;j<8;j++)
	 	   {
	 	  	 if((asc2_2412[NUM][w*3+2]&(0x01<<j))!=0)
         {
          if(mode == 0)
            LCD_CtrlWrite_Color(color); //����д
          else
	 	        	LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM				
         }
	 	     else
	 	   	  {
            if(mode == 0)
	 	   	    	LCD_CtrlWrite_Color(back_color);
			    }
				F_x++;					
	 	   } 
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((asc2_2412[NUM][w*3+1]&(0x01<<j))!=0)
	 	     {
          if(mode == 0)
            LCD_CtrlWrite_Color(color); //����д
          else
	 	        LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM				
         }
	 	     else
	 	   	  {
            if(mode == 0)
	 	   	    	LCD_CtrlWrite_Color(back_color);
			    }
				F_x++;				
	 	   } 
		   for(j=0;j<8;j++)
	 	   {
	 	  	 if((asc2_2412[NUM][w*3]&(0x01<<j))!=0)
	 	     {
            if(mode == 0)
              LCD_CtrlWrite_Color(color); //����д
            else
	 	        	LCD_DrawPoint(F_x,F_y,color);//��ʼд��GRAM				
         }
	 	     else
	 	   	  {
            if(mode == 0)
	 	   	    	LCD_CtrlWrite_Color(back_color);
			    }
			   F_x++;
	 	   }  
		   F_y++; 
		   F_x = X; 
	 	} 
}

//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//size:�����С 16/24
//num:Ҫ��ʾ���ַ�:" "--->"~" 
//mode:���ӷ�ʽ(1)��д����ɫ  ���Ƿǵ��ӷ�ʽ(0)д����ɫ
//color:
void LCD_ShowChar(u8 x,u16 y,u8 size,u8 num,u8 mode,u32 color)
{  							     
	//���ô���		   
	 //LCD_CrtlWrite_Area(0,239,0,319);
	if(size == 16)
     DISP_FNT8X16(x,y,num,mode,color);
	else if(size == 24)
     DISP_FNT12X24(x,y,num,mode,color); 
	else if(size==40)
		DISP_FNT20X40(x,y,num,mode,color); 
} 

void display_module_show_char(u8 x,u16 y,u8 size,u8 num,u8 mode,u32 color)
{
	LCD_ShowChar(x,y,size,num,mode,color);
}

/***************************************************
*�������ƣ�display_module_show_str
*������������ʾ��ʾ16*8 ��24*12 ASC�ַ�
*���������
* x���ַ�������ʼ������
* y: �ַ�������ʼ������
* height_x:�ַ����߶ȣ�Ҳ���ֺ�,��16��ʾ16*8��24��ʾ24*12
* width_y:�ַ�����ȣ����ֵ���󳤶ȣ����ⳬ����Χ
* *p:�ַ�����
* mode:��ʾģʽ�����ӷ�ʽ(1)��д����ɫ  ���Ƿǵ��ӷ�ʽ(0)д����ɫ
* color����ʾ��ɫ
*�����������
*����ֵ����
****************************************************/
void display_module_show_str(u16 x, u16 y,u8 height_x,u16 width_y, u8 *p, u8 mode, u32 color)
{
//		u8 size;	 //�����С 16/24       
//	u16 y0;
//	y0=y;

//	size = height_x;  //�ֺŻ�߶�
//	height_x+=x; // <240.
//	width_y+=y;//�ܿ��
//  while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
//  {       
//    if(y>=width_y){y=y0;y+=size;} //the end. overwrite if out of range.
//    //if(x>=height_x)break;//�˳�
//    LCD_ShowChar(x,y,size,*p,mode,color);
//    y+=size/2;	//next char
//    p++;
//  }
	
	u8 size;	 //�����С 16/24       
//	u16 y0;
//	y0=y;
	u16 x0;
	x0=x;

	size = height_x;  //�ֺŻ�߶�
	height_x+=x; // <240.
	width_y+=y;//�ܿ��
	while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
  {       
    if(x>=240){x=x0;x+=size;} //the end. overwrite if out of range.
    //if(x>=height_x)break;//�˳�
    LCD_ShowChar(x,y,size,*p,mode,color);
		if(size==24)
		{
			x+=size-12;	//next char
		}
		else if(size==16)
		{
			x+=size-8;
		}
		else if(size==40)
		{
			x+=size-20;
		}
    p++;
  }
}

void display_module_show_str_back_color(u16 x, u16 y,u8 height_x,u16 width_y, u8 *p, u8 mode, u32 color, u32 back_color)
{
			u8 size;	 //�����С 16/24       
	u16 y0;
	y0=y;

	size = height_x;  //�ֺŻ�߶�
	height_x+=x; // <240.
	width_y+=y;//�ܿ��
  while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
  {       
    if(y>=width_y){y=y0;y+=size;} //the end. overwrite if out of range.
    //if(x>=height_x)break;//�˳�
    //LCD_ShowChar(x,y,size,*p,mode,color);
		DISP_FNT12X24_back_color(x,y,*p,mode,color, back_color);
    y+=size/2;	//next char
    p++;
  }
}

void display_module_show_string(u16 x, u16 y,u8 height, u8 *p, u8 mode, u32 color)
{
	u16 cnt = 0, width = 0;
	
	//����ַ�������
	while(*(p++) != 0)
	{
		cnt ++;
	}
	p -= (cnt+1);
	
	width = height*cnt;
	width >>= 1;
	
	display_module_show_str(x, y,height, width, p, mode, color);
}

//���ӱ���ɫ
void display_module_show_string_back_color(u16 x, u16 y,u8 height, u8 *p, u8 mode, u32 color, u32 back_color)
{
	u16 cnt = 0, width = 0;
	
	//����ַ�������
	while(*(p++) != 0)
	{
		cnt ++;
	}
	p -= (cnt+1);
	
	width = height*cnt;
	width >>= 1;
	
	display_module_show_str_back_color(x, y,height, width, p, mode, color, back_color);
}
/***************************************************
*�������ƣ�display_module_show_str_underline_adjust
*������������ʾ��ʾ16*8 ��24*12 ASC�ַ�
*���������
* x���ַ�������ʼ������
* y: �ַ�������ʼ������
* height_x:�ַ����߶ȣ�Ҳ���ֺ�,��16��ʾ16*8��24��ʾ24*12
* width_y:�ַ�����ȣ����ֵ���󳤶ȣ����ⳬ����Χ
* *p:�ַ�����
* mode:��ʾģʽ�����ӷ�ʽ(1)��д����ɫ  ���Ƿǵ��ӷ�ʽ(0)д����ɫ
* color����ʾ��ɫ
*�����������
*����ֵ����
****************************************************/
void display_module_show_str_underline_adjust(u16 x, u16 y,u8 height_x,u16 width_y, u8 *p, u8 mode, u32 color, u8 underline, u8 adjust)
{
	u32 color_temp; 
	u8 size;	 //�����С 16/24       
	u16 y0;
	u16 y_coord, x_coord, width;
	
	x_coord = x;
	y_coord = y;
	width = width_y;
	
	y0=y;

	size = height_x;  //�ֺŻ�߶�
	height_x+=x; // <240.
	width_y+=y;//�ܿ��
  while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
  {       
    if(y>=width_y){y=y0;y+=size;} //the end. overwrite if out of range.
    //if(x>=height_x)break;//�˳�
    LCD_ShowChar(x,y,size,*p,mode,color);
    y+=size/2;	//next char
    p++;
  }
	
	if(adjust)
		color_temp = RED;
	else
		color_temp = color;
	
	if(underline)
	{
		display_module_draw_rect_real(x_coord-3,y_coord,x_coord-2,y_coord+width, color_temp);
	}
	else
	{
		display_module_draw_rect_real(x_coord-3,y_coord,x_coord-2,y_coord+width, BACK_COLOR);
	}
}

//
void display_module_show_string_underline_adjust(u16 x, u16 y,u8 height, u8 *p, u8 mode, u32 color, u8 underline, u8 adjust)
{
	u16 cnt = 0, width = 0;
	
	//����ַ�������
	while(*(p++) != 0)
	{
		cnt ++;
	}
	p -= (cnt+1);
	
	width = height*cnt;
	width >>= 1;
	
	display_module_show_str_underline_adjust(x, y,height, width, p, mode, color, underline, adjust);
}


void display_module_draw_rect_real(unsigned char Start_X, unsigned int Start_Y,\
				unsigned char End_X, unsigned int End_Y,unsigned int color)
{
	lcd_draw_rect_real(Start_X, Start_Y, End_X, End_Y, color);
}

//display 30x56 number 0~9.
void display_module_show_num_30x56(unsigned char  X, unsigned int Y,unsigned char NUM,unsigned int color)
{
	 DISP_NUM_30X56(X, Y, NUM, color);  
}

//10x16 number, 0~9.
void DISP_NUM_10X16(unsigned char  X, unsigned int Y,unsigned char NUM,unsigned int color)
{
	   unsigned int  w,j;
	
     LCD_CrtlWrite_Area(X,X+15,Y,Y+9);
	   for(w=0;w<20;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((NUM10X16[NUM][w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
}

//��ʾ1λ
void display_module_show_num_1bit_10X16(unsigned char  X, unsigned int Y,unsigned char NUM,unsigned int color)
{
	DISP_NUM_10X16(X,Y,NUM,color);
}

//��ʾ����16*16
void display_module_show_chinese_16x16(u16 x, u16 y, u8* str, u32 color)
{
	u32  w,j,cnt = 0, code; // cnt:�ۼ��ַ��ĸ���,code:�ֿ�ļ���
	u8  temp;
	u8 pIndex[2];
	typFNT_GB16 font_temp;
	
	//OSSchedLock();
	
	while(*(str+cnt) != 0)
	{
		pIndex[0] = str[cnt]; 
		pIndex[1] = str[cnt+1];
		
			//ȷ������
		LCD_CrtlWrite_Area(x,x+15,y+(cnt<<3),y+(cnt<<3)+15);
			//Ѱ����Ļ
		for(code = 0; code < CODE_16_CNT; code++)
		{
			if((code_GB_16[code].Index[0] == pIndex[0])&&(code_GB_16[code].Index[1] == pIndex[1]))
			{
				break;
			}
		}
		
		//����Ҳ�����ֱ�ӷ���
		if(code >= CODE_16_CNT)
			return;
		
		//������Ļ����
		font_temp = code_GB_16[code];
		
		for(w = 0;w<31;w+=2)
		{
			temp = font_temp.Msk[w];
			font_temp.Msk[w] = font_temp.Msk[w+1];
			font_temp.Msk[w+1] = temp;
		}
		//���
		for(w = 0; w < 31; w ++)
		{
			for(j = 0; j < 8; j++)
			{
				if((font_temp.Msk[w]&(0x80 >> j)))
				{
					LCD_CtrlWrite_Color(color);
				}
				else
				{
					LCD_CtrlWrite_Color(BACK_COLOR);
				}
			}
		}	
	
		cnt += 2;//ÿ������ռ�����ַ�
	}
	//OSSchedUnlock();
}

//��ʾ����20*20
void display_module_show_chinese_20x20(u16 x, u16 y, u8* str, u32 color)
{
	u32  w,j,cnt = 0, code; // cnt:�ۼ��ַ��ĸ���,code:�ֿ�ļ���
	u8  temp;
	u8 pIndex[2];
	typFNT_GB20 font_temp;
	
	//OSSchedLock();
	
	while(*(str+cnt) != 0)
	{
		pIndex[0] = str[cnt]; 
		pIndex[1] = str[cnt+1];
		
			//ȷ������
		LCD_CrtlWrite_Area(x,x+23,y+(cnt*10),y+(cnt*10)+19); //16*cnt/2  20*cnt/2
			//Ѱ����Ļ
		for(code = 0; code < CODE_20_CNT; code++)
		{
			if((code_GB_20[code].Index[0] == pIndex[0])&&(code_GB_20[code].Index[1] == pIndex[1]))
			{
				break;
			}
		}
		
		//����Ҳ�����ֱ�ӷ���
		if(code >= CODE_20_CNT)
			return;
		
		//������Ļ����
		font_temp = code_GB_20[code];
		for(w = 0;w<59;w+=3)
		{
			temp = font_temp.Msk[w];
			font_temp.Msk[w] = font_temp.Msk[w+2];
			font_temp.Msk[w+2] = temp;
		}

		//���
		for(w = 0; w < 59; w ++)
		{
			for(j = 0; j < 8; j++)
			{
				if((font_temp.Msk[w]&(0x80 >> j)))
				{
					LCD_CtrlWrite_Color(color);
				}
				else
				{
					LCD_CtrlWrite_Color(BACK_COLOR);
				}
			}
		}	
	
		cnt += 2;//ÿ������ռ�����ַ�
	}
	//OSSchedUnlock();
}
//��ʾ����24*24
void display_module_show_chinese_24x24(u16 x, u16 y, u8* str, u32 color)
{
	u32  w,j,cnt = 0, code; // cnt:�ۼ��ַ��ĸ���,code:�ֿ�ļ���
	u8  temp;
	u8 pIndex[2];
	typFNT_GB24 font_temp;
	
	//OSSchedLock();
	
	while(*(str+cnt) != 0)
	{
		pIndex[0] = str[cnt]; 
		pIndex[1] = str[cnt+1];
		
			//ȷ������
		LCD_CrtlWrite_Area(x,x+23,y+(cnt*12),y+(cnt*12)+23);//(24*cnt/2)
		
			//Ѱ����Ļ
		for(code = 0; code < CODE_24_CNT; code++)
		{
			if((code_GB_24[code].Index[0] == pIndex[0])&&(code_GB_24[code].Index[1] == pIndex[1]))
			{
					break;
			}
		}
		
		//����Ҳ�����ֱ�ӷ���
		if(code >= CODE_24_CNT)
			return;
		
		//������Ļ����

		font_temp = code_GB_24[code];
		
		for(w = 0;w<71;w+=3)
		{
			temp = font_temp.Msk[w];
			font_temp.Msk[w] = font_temp.Msk[w+2];
			font_temp.Msk[w+2] = temp;
		}

		//���
		for(w = 0; w < 71; w ++)
		{
			for(j = 0; j < 8; j++)
			{
				if((font_temp.Msk[w]&(0x80 >> j)))
				{
					LCD_CtrlWrite_Color(color);
				}
				else
				{
					LCD_CtrlWrite_Color(BACK_COLOR);
				}
			}
		}	
	
		cnt += 2;//ÿ������ռ�����ַ�
	}
	//OSSchedUnlock();
}

//��ʾ����24*24 ���ӱ���ɫ
void display_module_show_chinese_24x24_back_color(u16 x, u16 y, u8* str, u32 color, u32 back_color)
{
	u32  w,j,cnt = 0, code; // cnt:�ۼ��ַ��ĸ���,code:�ֿ�ļ���
	u8  temp;
	u8 pIndex[2];
	typFNT_GB24 font_temp;
	
	//OSSchedLock();
	
	while(*(str+cnt) != 0)
	{
		pIndex[0] = str[cnt]; 
		pIndex[1] = str[cnt+1];
		
			//ȷ������
		LCD_CrtlWrite_Area(x,x+23,y+(cnt*12),y+(cnt*12)+23);//(24*cnt/2)
		
			//Ѱ����Ļ
		for(code = 0; code < CODE_24_CNT; code++)
		{
			if((code_GB_24[code].Index[0] == pIndex[0])&&(code_GB_24[code].Index[1] == pIndex[1]))
			{
					break;
			}
		}
		
		//����Ҳ�����ֱ�ӷ���
		if(code >= CODE_24_CNT)
			return;
		
		//������Ļ����

		font_temp = code_GB_24[code];
		
		for(w = 0;w<71;w+=3)
		{
			temp = font_temp.Msk[w];
			font_temp.Msk[w] = font_temp.Msk[w+2];
			font_temp.Msk[w+2] = temp;
		}

		//���
		for(w = 0; w < 71; w ++)
		{
			for(j = 0; j < 8; j++)
			{
				if((font_temp.Msk[w]&(0x80 >> j)))
				{
					LCD_CtrlWrite_Color(color);
				}
				else
				{
					LCD_CtrlWrite_Color(back_color);
				}
			}
		}	
	
		cnt += 2;//ÿ������ռ�����ַ�
	}
	//OSSchedUnlock();
}
//display 30x56 number 0~9.
void DISP_NUM_30X56(unsigned char  X, unsigned int Y,unsigned char NUM,unsigned int color)
{
	   unsigned int w,j;
	//OSSchedLock();
	   LCD_CrtlWrite_Area(X,X+55,Y,Y+29);
	   for(w=0;w<210;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((NUM30X56[NUM][w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
		 //OSSchedUnlock();
}

//17x40 number, 0~9.
void DISP_NUM_17X40(unsigned char  X, unsigned int Y,unsigned char NUM,unsigned int color)
{
	   unsigned int  w,j;
	//OSSchedLock();
	   LCD_CrtlWrite_Area(X,X+39,Y,Y+16);
	   for(w=0;w<85;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((NUM17X40[NUM][w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	  }
		 //OSSchedUnlock();
}

/***************************************************
*�������ƣ�display_module_show_num_3bit
*������������ʾ2λ���� number 0~99
*���������
* x���ַ�������ʼ������
* y: �ַ�������ʼ������
* width:�ַ�����ȣ�Ҳ���ֺ�,��10��ʾ10*16��30��ʾ30*56
* NUM:���� number 0~99
* color����ʾ��ɫ
*�����������
*����ֵ����
****************************************************/
void display_module_show_num_3bit(u8 x, u16 y, u8 width, u8 NUM, u32 color, u8 underline, u8 adjust)
{ 
	u32 temp_color;
	
	if(NUM < 0 || NUM > 100)
		return;
	
	if(adjust)
		temp_color = RED;
	else
		temp_color = color;
	
  if(width==10) //10x16
  {
    DISP_NUM_10X16(x,y,NUM/10,color);
    DISP_NUM_10X16(x,y+width,NUM%10,color);
		
		if(underline)
			display_module_draw_rect_real(x-3,y,x-2,y+20,temp_color);
		else
			display_module_draw_rect_real(x-3,y,x-2,y+20,BACK_COLOR);
  }
  else if(width==30) //30x56
  {
    DISP_NUM_30X56(x,y,NUM/10,color);
    DISP_NUM_30X56(x,y+width,NUM%10,color);
		
		if(underline)
			display_module_draw_rect_real(x-5,y,x-2,y+60,temp_color);
		else
			display_module_draw_rect_real(x-5,y,x-2,y+60,BACK_COLOR);
  }
}
//0~199
void display_module_show_num_3bit_10X16(u8 x, u16 y, u8 NUM, u32 color, u8 underline, u8 adjust)
{ 
	u32 temp_color;
	
	if(adjust)
		temp_color = RED;
	else
		temp_color = color;
	
	if(NUM < 100)
  {
    DISP_NUM_10X16(x,y+5,NUM/10,color);
    DISP_NUM_10X16(x,y+15,NUM%10,color);
		display_module_draw_rect_real(x-3,y+25,x+16,y+28,BACK_COLOR);
		display_module_draw_rect_real(x-3,y,x+16,y+4,BACK_COLOR);
		
		if(underline)
			display_module_draw_rect_real(x-3,y,x-2,y+25,temp_color);
		else
			display_module_draw_rect_real(x-3,y,x-2,y+28,BACK_COLOR);
  }
	else if(NUM < 200)
	{
		DISP_NUM_10X16(x,y,1,color);
		DISP_NUM_10X16(x,y+8,NUM/10%10,color);
    DISP_NUM_10X16(x,y+8+10,NUM%10,color);
		
		if(underline)
			display_module_draw_rect_real(x-3,y,x-2,y+28,temp_color);
		else
			display_module_draw_rect_real(x-3,y,x-2,y+28,BACK_COLOR);
	}
}

/***************************************************
*�������ƣ�display_module_show_num_3bit_30x56
*������������ʾ3λ���� number 0-100,�ֺ�,��10��ʾ10*16��30��ʾ30*56
*���������
* x���ַ�������ʼ������
* y: �ַ�������ʼ������
* NUM:���� number 0~999
* color����ʾ��ɫ
* underline:�»���
*adjust������
*�����������
*����ֵ����
****************************************************/
void display_module_show_num_3bit_30x56(u8 x, u16 y, u16 NUM, u32 color, u8 underline, u8 adjust)
{ 
	u32 temp_color;
	
	if(NUM < 0 || NUM > 100)
		return;
	
	if(adjust)
		temp_color = RED;
	else
		temp_color = color;
	
	if(NUM < 100)
	{
    DISP_NUM_30X56(x,y,NUM/10,color);
    DISP_NUM_30X56(x,y+30,NUM%10,color);
		display_module_draw_rect_real(x-5,y-23,x+56,y,BACK_COLOR);
		
		if(underline)
			display_module_draw_rect_real(x-5,y,x-2,y+59,temp_color);
		else
			display_module_draw_rect_real(x-5,y-10,x-2,y+59,BACK_COLOR);
	}
	else
	{
		DISP_NUM_30X56(x,y-23,1,color);
		DISP_NUM_30X56(x,y,0,color);
    DISP_NUM_30X56(x,y+30,0,color);
		if(underline)
			display_module_draw_rect_real(x-5,y-10,x-2,y+59,temp_color);
		else
			display_module_draw_rect_real(x-5,y-10,x-2,y+59,BACK_COLOR);
	}
}

//display 2λ x10������+1λС�� number 0.0~99.9, 100. 
void display_module_show_num10_2bit_17x40(u8 x, u16 y, u16 NUM, u32 color)
{ 
		if(NUM > 99)
		{
			DISP_NUM_17X40(x,y,NUM/100,color);
			DISP_NUM_17X40(x,y+17,NUM/10%10,color);
		}
		else if(NUM >= 0 && NUM < 100)
		{
			DISP_NUM_17X40(x,y,NUM/10,color);
			display_module_show_str(x,y+16,24,12,".",0,color);
			DISP_NUM_17X40(x,y+25,NUM%10,color);
		}	
}

//������ʾ0.0~9.9~10~99 L 
//����num��λΪ10ml,����ʾ����С100��
void display_module_show_rt_flow(u8 x, u16 y, u16 NUM, u32 color)
{
//#define DISP_POIINT_FLG 1
	static int flow_disp_clear = 0;
	
	//if(NUM >9900)
		//NUM = 9900;
	
	if(NUM >19999)
		NUM = 19999;
	
	//��������
	#if DISP_POIINT_FLG
	if(NUM%10 >= 5)
	NUM += 10;
	#else
	if(NUM%100 >= 50)
	NUM += 100;
	#endif

	#if 1
	if(NUM >= 10000) //���ڵ���100L
	{
		if(flow_disp_clear != 0)
		{
			flow_disp_clear = 0;
			lcd_draw_rect_real(x,y,x+40,y+75,BACK_COLOR);
		}
		
		DISP_NUM_17X40(x,y-10,1,color);
		DISP_NUM_17X40(x,y+3,NUM%10000/1000,color);
		DISP_NUM_17X40(x,y+20,NUM%1000/100,color);
		#if DISP_POIINT_FLG
		display_module_show_str(x,y+17+16+3,24,12,".",0,color);
		DISP_NUM_10X16(x,y+17+16+12,NUM%100/10,color);
		#endif
	}
	else if(NUM >= 1000) //���ڵ���10L
	{	
		if(flow_disp_clear != 1)
		{
			flow_disp_clear = 1;
			lcd_draw_rect_real(x,y-10,x+40,y,BACK_COLOR);
			lcd_draw_rect_real(x,y,x+40,y+75,BACK_COLOR);
		}
		
		DISP_NUM_17X40(x,y,NUM/1000,color);
		DISP_NUM_17X40(x,y+17,NUM%1000/100,color);
		#if DISP_POIINT_FLG
		display_module_show_str(x,y+17+16,24,12,".",0,color);
		DISP_NUM_10X16(x,y+17+16+12,NUM%100/10,color);
		#endif
	}
	else if(NUM >= 0)//С�ڵ���10L����һλС����  999
	{
		if(flow_disp_clear != 2)
		{
			flow_disp_clear = 2;
			lcd_draw_rect_real(x,y-10,x+40,y,BACK_COLOR);
			lcd_draw_rect_real(x,y,x+40,y+75,BACK_COLOR);
		}
		
		DISP_NUM_17X40(x,y,0,color);
		DISP_NUM_17X40(x,y+17,NUM/100,color);
		#if DISP_POIINT_FLG
		display_module_show_str(x,y+16,24,12,".",0,color);
		DISP_NUM_17X40(x,y+25,NUM%100/10,color);
		#endif
	}
	#endif
}

//��ʾ����0~100   
//NUM �Ŵ���10��
//alarm_flag: RESET��������SET������
void display_module_show_rt_fio2(u8 x, u16 y, u16 NUM, u32 color, u8 alarm_flag)
{
	static int fio2_disp_clear = RESET;
	u32 color_temp;
	#if 1
	if(alarm_flag == SET)
	{
		color_temp = RED;
	}
	else
	{
		color_temp = color;
	}
	
	//��������
	if(NUM%10 >= 5)
	NUM += 10;
	
	if(NUM >= 1000)
	{
		if(fio2_disp_clear)
		{
			fio2_disp_clear = RESET;
			lcd_draw_rect_real(x,y,x+40,y+52,BACK_COLOR);
		}
		//��ʾ��100
		DISP_NUM_17X40(x,y,1,color_temp);
		DISP_NUM_17X40(x,y+17,0,color_temp);
		DISP_NUM_17X40(x,y+34,0,color_temp);
	}
	else
	{
		if(!fio2_disp_clear)
		{
			fio2_disp_clear = SET;
			lcd_draw_rect_real(x,y,x+40,y+52,BACK_COLOR);
		}
			
		DISP_NUM_17X40(x,y,NUM/100,color_temp);
		DISP_NUM_17X40(x,y+17,NUM%100/10,color_temp);
	}
	#endif
}

//
void display_module_show_num10_2bit_30x56(u8 x, u16 y, u16 NUM, u32 color, u8 underline, u8 adjust)
{ 
	u32 temp_color;
	
	//OSSchedLock();
	
	if(adjust)
		temp_color = RED;
	else
		temp_color = color;
	
	NUM /= 10;
	
	if(NUM > 999)
	{
		//
		DISP_NUM_30X56(x,y-10,1,color);
		DISP_NUM_30X56(x,y+15,NUM%1000/100,color);
		DISP_NUM_30X56(x,y+45,NUM%100/10,color);
		
		if(underline)
			display_module_draw_rect_real(x-5,y-5,x-2, y+65,temp_color);
		else
			display_module_draw_rect_real(x-5,y-5,x-2, y+65,BACK_COLOR);
	}
	else if(NUM > 99)
		{
			DISP_NUM_30X56(x,y,NUM/100,color);
			DISP_NUM_30X56(x,y+30,NUM/10%10,color);
			display_module_draw_rect_real(x,y+60,x+56,y+75,BACK_COLOR);
			display_module_draw_rect_real(x,y-10,x+56,y,BACK_COLOR);
			
			if(underline)
				display_module_draw_rect_real(x-5,y-5,x-2, y+65,temp_color);
			else
				display_module_draw_rect_real(x-5,y-5,x-2, y+65,BACK_COLOR);
		}
		else if(NUM >= 0 && NUM < 100)
		{
			#ifdef DEBUG_FLOW_DISPLAY 
			DISP_NUM_30X56(x,y,NUM/10,color);
			display_module_draw_rect_real(x,y+30,x+56,y+39,BACK_COLOR);
			display_module_draw_rect_real(x+2,y+33,x+6,y+37,color);
			DISP_NUM_30X56(x,y+40,NUM%10,color);
			
			if(underline)
				display_module_draw_rect_real(x-5,y-5,x-2, y+65,temp_color);
			else
				display_module_draw_rect_real(x-5,y-5,x-2, y+65,BACK_COLOR);
			#else //����ʾС����
			DISP_NUM_30X56(x,y,0,color);
			DISP_NUM_30X56(x,y+30,NUM/10,color);
			display_module_draw_rect_real(x,y+60,x+56,y+70,BACK_COLOR);
			
			if(underline)
				display_module_draw_rect_real(x-5,y-5,x-2, y+65,temp_color);
			else
				display_module_draw_rect_real(x-5,y-5,x-2, y+65,BACK_COLOR);
			#endif
		}	
		
		//OSSchedUnlock();
}

//show bat bmp--active.
/***************************************************
*�������ƣ�display_module_show_img_bat
*������������ʾ���ͼ�ꡪ�������
*���������
* x����ʼ������
* y: ��ʼ������
* active_flag:�����־
* color����ʾ��ɫ
*�����������
*����ֵ����
****************************************************/
void display_module_show_img_bat(unsigned char  X, unsigned int Y, unsigned char active_flag, unsigned int color)
{
	   unsigned int w,j;
	if(active_flag)
	{
		LCD_CrtlWrite_Area(X,X+79,Y,Y+44);
	   for(w=0;w<450;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_bat45x80[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
	}
	else
	{
		LCD_CrtlWrite_Area(X,X+31,Y,Y+17);
	   for(w=0;w<72;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_bat18x32[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
	}
	   
}

//show setting bmp--inactive.
/***************************************************
*�������ƣ�display_module_show_str
*������������ʾ����ͼ��BMP
*���������
* x����ʼ������
* y: ��ʼ������
* active_flag:�����־
* color����ʾ��ɫ
*�����������
*����ֵ����
****************************************************/
void  display_module_show_img_seting(unsigned char  X, unsigned int Y, unsigned char active_flag, unsigned int color)
{
	   unsigned int w,j;
	if(active_flag)
	{
		LCD_CrtlWrite_Area(X,X+55,Y,Y+62);
	   for(w=0;w<441;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_set63x56[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
	}
	else
	{
		LCD_CrtlWrite_Area(X,X+23,Y,Y+26);
	   for(w=0;w<81;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_set27x24[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
	}
}

//show bell bmp--inactive.
/***************************************************
*�������ƣ�display_module_show_str
*������������ʾ����ͼ��BMP�����Ǽ���
*���������
* x����ʼ������
* y: ��ʼ������
* active_flag:�����־
* color����ʾ��ɫ
*�����������
*����ֵ����
****************************************************/
void display_module_show_img_bell(unsigned char  X, unsigned int Y,unsigned char active_flag, unsigned int color)
{
	   unsigned int w,j;
	if(active_flag)
	{
		LCD_CrtlWrite_Area(X,X+55,Y,Y+52);
	   for(w=0;w<371;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_bell53x56[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
	}
	else
	{
		LCD_CrtlWrite_Area(X,X+31,Y,Y+29);
	   for(w=0;w<120;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_bell30x32[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
	}   
}

//show circle ico.
/***************************************************
*�������ƣ�display_module_img_circle48x48
*������������ʾ48*48Բͼ��BMP
*���������
* x����ʼ������
* y: ��ʼ������
* color����ʾ��ɫ
*�����������
*����ֵ����
****************************************************/
void display_module_show_img_circle48x48(unsigned char  X, unsigned int Y,unsigned int color)
{
	   unsigned int w,j;
	   LCD_CrtlWrite_Area(X,X+47,Y,Y+47);
	   for(w=0;w<288;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_circle48x48[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
}
/***************************************************
*�������ƣ�display_module_img_pause
*������������ʾ��ͣͼ��BMP
*���������
* x����ʼ������
* y: ��ʼ������
* color����ʾ��ɫ
*�����������
*����ֵ����
****************************************************/
void display_module_show_img_pause(unsigned char  X, unsigned int Y,unsigned int color)
{
	   unsigned int w,j;
	   LCD_CrtlWrite_Area(X,X+23,Y,Y+12);
	   for(w=0;w<39;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_pause13x24[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
}
/***************************************************
*�������ƣ�display_module_img_start
*������������ʾ����ͼ��BMP
*���������
* x����ʼ������
* y: ��ʼ������
* color����ʾ��ɫ
*�����������
*����ֵ����
****************************************************/
void display_module_show_img_start(unsigned char  X, unsigned int Y,unsigned int color)
{
	   unsigned int w,j;
	   LCD_CrtlWrite_Area(X,X+23,Y,Y+20);
	   for(w=0;w<63;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_start21x24[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
}
/***************************************************
*�������ƣ�display_module_img_lock
*������������ʾ����ͼ��BMP
*���������
* x����ʼ������
* y: ��ʼ������
* color����ʾ��ɫ
*�����������
*����ֵ����
****************************************************/
void display_module_show_img_lock(unsigned char  X, unsigned int Y,unsigned int color)
{
	   unsigned int w,j;
	   LCD_CrtlWrite_Area(X,X+15,Y,Y+14);
	   for(w=0;w<30;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_lock15x16[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
}

//show speaker base bmp, volume image behind it.
void display_module_show_img_speaker(unsigned char  X, unsigned int Y,unsigned int color)
{
	   unsigned int w,j;
	   LCD_CrtlWrite_Area(X,X+63,Y,Y+44);
	   for(w=0;w<360;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_speaker45x64[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
}

//show speaker volume 1/2/3 bmp, voice volume adjust.
//volume: 0,1,2,means the volume of voice. 0:samllest. 1:medium. 2:biggest.
void display_module_show_img_volume(unsigned char  X, unsigned int Y, u8 volume, unsigned int color)
{
  unsigned int w,j;
  LCD_CrtlWrite_Area(X,X+63,Y,Y+31);
  for(w=0;w<256;w++)
  {
   for(j=0;j<8;j++)
   {
     switch(volume)
     {
       case 0:
         if((gImage_volume1_32x64[w]&(0x01<<j))!=0)
            LCD_CtrlWrite_Color(color);
         else
            LCD_CtrlWrite_Color(BACK_COLOR);
         break;
         
         case 1:
         if((gImage_volume2_32x64[w]&(0x01<<j))!=0)
            LCD_CtrlWrite_Color(color);
         else
            LCD_CtrlWrite_Color(BACK_COLOR);
         break;
         
         case 2:
         if((gImage_volume3_32x64[w]&(0x01<<j))!=0) //biggest volume.
            LCD_CtrlWrite_Color(color);
         else
            LCD_CtrlWrite_Color(BACK_COLOR);
         break;
         
         default:
           break;
      }
   }
  }
}

//show valve air pressure meter ico.
void display_module_show_img_valve(unsigned char  X, unsigned int Y,unsigned int color)
{
	   unsigned int w,j;
	   LCD_CrtlWrite_Area(X,X+39,Y,Y+38);
	   for(w=0;w<195;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_valve39x40[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
}

//show Inspired Medical bmp logo
void display_module_show_img_logo(unsigned char  X, unsigned int Y,unsigned int color)
{
	   unsigned int w,j;
	   LCD_CrtlWrite_Area(X,X+103,Y,Y+62);
	   for(w=0;w<819;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_IMlogo63x104[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BACK_COLOR);
	 	    }
	 	 }
}

//����
void display_module_clear(void)
{
	display_module_draw_rect_real(0,0,239,319,BACK_COLOR);  //��ʵ�ľ���
}

//
//* height: �����С 16/24
void display_module_show_lang(u16 x, u16 y, u8 height, LANG_STR_INDEX lang_index, u32 color)
{
	LANG_STR_TYPE lang_str; 
	
	lang_str = lang_array[lang_index];
	
	if(lang_str.lang == ENGLISH)
	{
		display_module_show_string(x, y+lang_str.offset, height, lang_str.str, 0, color);
	}
	else if(lang_str.lang == CHINESE)
	{
		if(height == 16)
			display_module_show_chinese_16x16(x, y+lang_str.offset, lang_str.str, color);
		else if(height == 24)
			display_module_show_chinese_24x24(x, y+lang_str.offset, lang_str.str, color);
	}
}
//* height: �����С 16/24 ������back_color
void display_module_show_lang_back_color(u16 x, u16 y, u8 height, LANG_STR_INDEX lang_index, u32 color, u32 back_color)
{
		LANG_STR_TYPE lang_str; 
	
	lang_str = lang_array[lang_index];
	
	if(lang_str.lang == ENGLISH)
	{
		display_module_show_string_back_color(x, y+lang_str.offset, height, lang_str.str, 0, color, back_color);
	}
	else if(lang_str.lang == CHINESE)
	{
		if(height == 16)
			display_module_show_chinese_16x16(x, y+lang_str.offset, lang_str.str, color);
		else if(height == 24)
			display_module_show_chinese_24x24_back_color(x, y+lang_str.offset, lang_str.str, color, back_color);
	}
}

void display_module_show_line(unsigned char  X0, unsigned int Y0,unsigned char  X1, unsigned int Y1,unsigned int color)
{
	int x, y;
	int cnt;
	float x0,x1,y0,y1;
	float k;
	
	x0 = X0;
	x1 = X1;
	y0 = Y0;
	y1 = Y1;
	
	k = (x1-x0)/(y1-y0);
	
	for(y = y0; y <= y1; y ++)
	{
		x = k*(y-y0) + x0;
		
		LCD_DrawPoint(x,y,color);
		LCD_DrawPoint(x-1,y,color);
		LCD_DrawPoint(x-2,y,color);
	}
}

void display_module_show_img_bat_x(unsigned char  X, unsigned int Y,unsigned int color)
{
	display_module_show_line(X,Y,X+79-19,Y+44-8, color);
	display_module_show_line(X+79-19,Y,X,Y+44-8, color);
}

void display_module_show_img_batcharge14x40(unsigned char  X, unsigned int Y, unsigned int color)
{
	   unsigned int w,j;	
	   LCD_CrtlWrite_Area(X,X+39,Y,Y+13);
	   for(w=0;w<70;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_batcharge14x40[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(BLUE);           
	 	    }
	 	 }
}

//��ʾ����־
void display_module_show_img_batcharge14x40_back_color(unsigned char  X, unsigned int Y, unsigned int color, unsigned int back_color)
{
	   unsigned int w,j;	
	   LCD_CrtlWrite_Area(X,X+39,Y,Y+13);
	   for(w=0;w<70;w++)
	   {
	 	   for(j=0;j<8;j++)
	 	   {
	 	  	 if((gImage_batcharge14x40[w]&(0x01<<j))!=0)
	 	        LCD_CtrlWrite_Color(color);
	 	     else
	 	   	    LCD_CtrlWrite_Color(back_color);           
	 	    }
	 	 }
}

