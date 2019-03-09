#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "i2c.h"
#include "hardware.h"

void I2C_uConfiguration(void)
{
	GPIO_InitTypeDef   GPIO_uInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_uInitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_uInitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_uInitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
	GPIO_Init(I2C_SDA_PORT,&GPIO_uInitStructure);
 	
	GPIO_uInitStructure.GPIO_Pin = I2C_SCL_PIN;  
	GPIO_Init(I2C_SCL_PORT,&GPIO_uInitStructure);
}


void I2C_SDA_OUT(void)
{
	GPIO_InitTypeDef   GPIO_uInitStructure;
	
	GPIO_uInitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_uInitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_uInitStructure.GPIO_Speed = GPIO_Speed_10MHz;  // 10M
	GPIO_Init(I2C_SDA_PORT,&GPIO_uInitStructure);
}

void I2C_SDA_IN(void)
{
	GPIO_InitTypeDef   GPIO_uInitStructure;
	
	GPIO_uInitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_uInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_uInitStructure.GPIO_Speed = GPIO_Speed_10MHz;  // 10M
	GPIO_Init(I2C_SDA_PORT,&GPIO_uInitStructure);
}

void I2C_Start(void)
{
	I2C_SDA_OUT();
	
	SDA_H;
	SCL_H;
	Delay_us(5);
	SDA_L;
	Delay_us(5);
	SCL_L;
}

void I2C_Stop(void)
{
	//I2C_SDA_OUT();
	
  SDA_L;
  SCL_H;
  Delay_us(5);
  SDA_H;
  Delay_us(5);
}


void I2C_SendAck(void)
{
  I2C_SDA_OUT();
	
  SDA_L;
  SCL_H;
  Delay_us(5);
  SCL_L;
  Delay_us(5);
}


void I2C_SendNak(void)
{
  I2C_SDA_OUT();
	
  SDA_H;
  SCL_H;
  Delay_us(5);
  SCL_L;
  Delay_us(5);
}


INT8U I2C_RecAck(void)
{
  INT8U RecAck=0;
	
  //I2C_SDA_IN();
	SDA_H;
	Delay_us(5);
  SCL_H;
  Delay_us(5);
	I2C_SDA_IN();
	Delay_us(20);
  RecAck = (INT8U)SDA_READ;
	I2C_SDA_OUT();
	Delay_us(5);
  SCL_L;
  Delay_us(25);
	
  return RecAck;
}

void I2C_SendByte(INT8U dat)
{
  INT8U i;
  //I2C_SDA_OUT();
  for(i=0;i<8;i++)
  {
    if(dat&0x80)
		{
      SDA_H;
		}
    else
		{
      SDA_L;
		}
    dat <<=1;
		Delay_us(5);
    SCL_H;
    Delay_us(5);
    SCL_L;
    Delay_us(5);
  }
  //I2C_RecAck();
}


INT8U I2C_RecByte(void)
{
  INT8U i,dat=0;

  I2C_SDA_IN();
	
  for(i=0;i<8;i++)
  {
    SCL_H;
    Delay_us(5);
    dat <<=1;
    dat |= SDA_READ;
    SCL_L;
    Delay_us(5);
  }
  return dat;

}


/**************************************************************
* Honeywell初始化
**************************************************************/
void Init_honeywell_sensor(void)
{
	I2C_uConfiguration();
  I2C_Start();
  I2C_SendByte(0x30);
	I2C_RecAck();
  I2C_SendByte(0xAA);
	I2C_RecAck();
	I2C_SendByte(0x00);  		
	I2C_RecAck();
	I2C_SendByte(0x00);
	I2C_RecAck();
  I2C_Stop();
}

/**************************************************************
* Honeywell，判断Honeywell是否空闲
**************************************************************/
BOOLEAN Is_honeywell_free(void)
{
	INT8U data_status;
	I2C_Start();
	I2C_SendByte(0x31);
	I2C_RecAck();
	data_status=I2C_RecByte();
	I2C_SendNak();
	I2C_Stop();
	if(data_status==0x40)
		return 1;
	else
		return 0;
}

/**************************************************************
* Honeywell，读取数据
**************************************************************/
INT32U honeywell_readByte()
{
	INT8U data_staus,data23_16,data15_8,data7_0;
	I2C_Start();
	I2C_SendByte(0x31);
	I2C_RecAck();
	data_staus=I2C_RecByte();
	I2C_SendAck();
	data23_16=I2C_RecByte();
	I2C_SendAck();
	data15_8=I2C_RecByte();
	I2C_SendAck();
	data7_0=I2C_RecByte();
	I2C_SendNak();
	I2C_Stop();
	return (data23_16<<16)+(data15_8<<8)+data7_0;
//	return (data_staus<<24)+(data23_16<<16)+(data15_8<<8)+data7_0;
}
