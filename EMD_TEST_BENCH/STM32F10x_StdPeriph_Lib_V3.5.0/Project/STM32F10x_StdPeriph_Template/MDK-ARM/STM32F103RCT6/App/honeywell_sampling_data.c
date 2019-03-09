#include "honeywell_sampling_data.h"
#include "os_core.h"
#include "app.h"
#include "i2c.h"
#include "comm_task.h"
#include "protocol_module.h"

#ifdef _DEBUG
#include "Motor_pwm.h"

//extern uint16_t RegularConvData_Tab[2]; 
#endif

//#define HONEYWELL_RATE			11185   //б��,���ݹ�ʽ�������
#define HONEYWELL_RATE			9000




//extern uint32_t adc_value[2];
uint32_t adc_pressure_value;

BOOL b_getHoneywellZeroPoint=0;
HONEYWELL_STATE honeywell_state=HONEYWELL_START;
uint32_t HONEYWELL_ZERO_POINT=0;

uint32_t trans_xmmHg_2_adc_value(uint8_t xmmHg)
{
//	uint32_t tmp;
//	FlashRead(FLASH_WRITE_START_ADDR,&tmp,1);
//	uint8_t data=(tmp&0xFF)>>8;
	
	uint8_t data_high=xmmHg/16;
	uint8_t data_low=xmmHg%16;
	//y=ax+b
	return HONEYWELL_RATE*(data_high*10+data_low)/10+HONEYWELL_ZERO_POINT;
}


void honeywell_sampling_data()
{
	if(honeywell_state==HONEYWELL_START)
	{
		Init_honeywell_sensor();
		honeywell_state=HONEYWELL_READ_DATA;
	}
	
	if(honeywell_state==HONEYWELL_READ_DATA)
	{	
		if(Is_honeywell_free()==1)
		{
			if(b_getHoneywellZeroPoint==0) //���ǵ�������Ʈ,�ڵ�һ�ζ����ݵ�ʱ���ȡ���ֵ
			{
				HONEYWELL_ZERO_POINT=honeywell_readByte();
				b_getHoneywellZeroPoint=1;
			}
			else
			{
				//debug
//				static uint32_t test001;
//				test001=trans_xmmHg_2_adc_value(160);
				
				adc_pressure_value=honeywell_readByte();
				honeywell_state=HONEYWELL_START;
			}
		}
		#ifdef _DEBUG
		static uint16_t before_voltage=0;
		static uint16_t after_voltage=0;
		static uint16_t diff=0;
		before_voltage=RegularConvData_Tab[0];  //��ʼ��ѹ
		Delay_us(50);
		
		Motor_PWM_Freq_Dudy_Set(1,20,100);        //������𶯵��и���
		Motor_PWM_Freq_Dudy_Set(2,20,100);
		Delay_us(600);
		
		after_voltage=RegularConvData_Tab[0];  //������ѹ
		
		diff=before_voltage-after_voltage;
		
		Motor_PWM_Freq_Dudy_Set(1,20,0);        
		Motor_PWM_Freq_Dudy_Set(2,20,0);
		#endif
	}
	
	os_delay_ms(HONEYWELL_SAMPLING_DATA_TASK_ID,HONEYWELL_SAMPLING_DATA_PERIOD);  //10msѭ��һ������
}


