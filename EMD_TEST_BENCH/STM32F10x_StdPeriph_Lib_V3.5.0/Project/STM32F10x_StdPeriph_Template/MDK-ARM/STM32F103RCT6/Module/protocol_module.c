/**
********************************************************************************
* 版權：
* 模块名称：protocol.c
* 模块功能：跟上位機進行通信
* 创建日期：
* 创 建 者：
* 声    明：
********************************************************************************
**/

/***********************************
* 头文件
***********************************/
#include "stm32f10x_usart.h"
#include "stm32f10x.h"
#include "datatype.h"
#include "serial_port.h"
#include "hardware.h"
#include "fifo.h"
#include "protocol_module.h"
#include "comm_task.h"
#include "os_core.h"
#include "app.h"
#include "string.h"
#include "stdlib.h"
#include "stm32f10x_flash.h"
#include "key_led_task.h"

#include "common.h"
//#include "rtc.h"
/**********************************
*宏定义
***********************************/

/***********************************
* 全局变量
***********************************/
extern const uint8_t default_parameter_buf[PARAMETER_BUF_LEN];
extern BOOL b_get_pressure_zero_point;
extern uint32_t EMD_PRESSURE_ZERO_POINT;

uint32_t pageBuff[512];
//BOOL rcvParameters_from_PC=FALSE;
uint8_t rcvParaSuccess=0x00;
//發送數據FIFO
extern FIFO_TYPE send_fifo;
extern uint8_t send_buf[SEND_BUF_LEN];

extern uint16_t RegularConvData_Tab[2];

extern BOOL b_get_sw_version_success;
extern BOOL b_get_para_success;
/***********************************
* 局部变量
***********************************/

/***********************************
* 局部函数
***********************************/


//發送数据校验和
void protocol_module_send_train_data_check_sum(uint32_t check_sum)
{
	uint8_t buffer[CMD_BUFFER_LENGTH];
	
	buffer[0] = PACK_HEAD_BYTE;
	buffer[1] = 0x08;
	buffer[2] = MODULE_CMD_TYPE;
	buffer[3] = SEND_EXP_TRAIN_DATA_CHECK_SUM_ID;
	buffer[4] = check_sum & 0xff;
	buffer[5] = (check_sum >> 8) & 0xff;
	buffer[6] = (check_sum >> 16) & 0xff;
	buffer[7] = (check_sum >> 24) & 0xff;
	
	CalcCheckSum(buffer);
	
	fifoWriteData(&send_fifo, buffer, buffer[1]+2);
}

BOOL Is_the_sw_version_right(uint8_t* pdata,char frameId)
{
	BOOL rt;
	if(0x73==frameId)
	{
		uint8_t ver_buffer[3];
		ver_buffer[0]=pdata[4];
		ver_buffer[1]=pdata[5];
		ver_buffer[2]=pdata[6];
		if(ver_buffer[0]==1&&ver_buffer[1]==1&&ver_buffer[2]==0)
		{
			rt=1;
		}
	}
	else
	{
		rt=0;
	}
	return rt;
}

BOOL Is_the_para_frame1_right(uint8_t* pdata,char frameId)
{
	uint8_t len=pdata[1];  
//	for(int i=0;i<len-4;i++)
//	{
//		if(pdata[4+i]!=default_parameter_buf[i])
//		{
//			return FALSE;
//		}
//	}
	for(int i=4;i<len;i++)
	{
		if(pdata[i]!=default_parameter_buf[i-4])
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL Is_the_para_frame2_right(uint8_t* pdata,char frameId)
{
	uint8_t len=pdata[1];  
//	for(int i=0;i<len-4;i++)
//	{
//		if(pdata[4+i]!=default_parameter_buf[i+242])
//		{
//			return FALSE;
//		}
//	}
	for(int i=4;i<len;i++)
	{
		if(pdata[i]!=default_parameter_buf[i-4+242])
		{
			return FALSE;
		}
	}
	return TRUE;
}


void Get_pressure_zero_point_from_EMD_PCB(uint8_t* pdata,char frameId)
{
	EMD_PRESSURE_ZERO_POINT=(pdata[4]<<24)+(pdata[5]<<16)+(pdata[6]<<8)+pdata[7];
	b_get_pressure_zero_point=TRUE;
}

void send_para_rcv_result()
{
	uint8_t buffer[7];
	buffer[0] = 0xFF;       //0xFF，头
	buffer[1] = 0x04;            			//长度
	buffer[2] = 0x00;      //0x00，下位机像上位机发送命令的标志
	buffer[3] = 0x05; //0x08，FrameID
	CalcCheckSum(buffer);
	fifoWriteData(&send_fifo, buffer, buffer[1]+2);
	rcvParaSuccess=0x00;   //复位，为下次接收
}

//send getting sw version requestion
void send_get_sw_version()
{
	uint8_t buffer[6];
	
	buffer[0] = 0xFF;      						//0xFF
	buffer[1] = 0x04;            
	buffer[2] = 0x01;      
	buffer[3] = GET_SW_VERSION; 			//0x72
	
	CalcCheckSum(buffer);
	fifoWriteData(&send_fifo, buffer, buffer[1]+2);
}

void send_get_pressure_zero_point()
{
	uint8_t buffer[6];
	
	buffer[0] = 0xFF;      						//0xFF
	buffer[1] = 0x04;            
	buffer[2] = 0x01;      
	buffer[3] = GET_PRESSURE_ZERO_POINT; 			//0x74
	
	CalcCheckSum(buffer);
	fifoWriteData(&send_fifo, buffer, buffer[1]+2);
}

void SendQuery1ForParameters()
{
		uint8_t buffer[7];
		buffer[0] = 0xFF;
		buffer[1] = 0x05;  //len
		buffer[2] = 0x01;
		buffer[3] = 0x05;   //framID
		buffer[4] = 0x01; 

		CalcCheckSum(buffer);
		fifoWriteData(&send_fifo, buffer, buffer[1]+2);
}

void SendQuery2ForParameters()
{
		uint8_t buffer[7];
		buffer[0] = 0xFF;
		buffer[1] = 0x05;   //len
		buffer[2] = 0x01;
		buffer[3] = 0x06;   //framID
		buffer[4] = 0x01; 

		CalcCheckSum(buffer);
		fifoWriteData(&send_fifo, buffer, buffer[1]+2);
}

//解析上位机命令
void protocol_module_process(uint8_t* pdata)
{
	uint8_t *pCmdPacketData = (uint8_t *)pdata;
	uint8_t byFrameID = pCmdPacketData[3];

//	uint8_t bat_per;//电池电量
	
//	//如果没有上电，直接返回
//	if(mcu_state!=POWER_ON)
//	{
//		return;
//	}
	
	//pCmdPacketData = pdata;
	//byFrameID = pCmdPacketData[3];
	//byFrameID = *(pdata+3);

	//byFrameID = GET_BAT_PER_ID;
	switch(byFrameID)
	{
		case SEND_SW_VERSION:       //0x73, sent from EAR PCBA
			if(Is_the_sw_version_right(pdata,SEND_SW_VERSION))
			{
				b_get_sw_version_success=TRUE;
			}
			else
			{
				b_get_sw_version_success=FALSE;
			}
			
			break;
//		case 0x05:            //only for debug
//			send_para_rcv_result();
//			break;
		case SEND_FLASH_DATA_1_ID:
			if(Is_the_para_frame1_right(pdata,SEND_FLASH_DATA_1_ID))
			{
				SendQuery2ForParameters();
			}
			break;
		case SEND_FLASH_DATA_2_ID:
			if(Is_the_para_frame2_right(pdata,SEND_FLASH_DATA_2_ID))
			{
				b_get_para_success=TRUE;
			}
			break;
		case SEND_PRESSURE_ZERO_POINT:
			Get_pressure_zero_point_from_EMD_PCB(pdata,SEND_PRESSURE_ZERO_POINT);
			break;
	default:
		break;
	}
}
