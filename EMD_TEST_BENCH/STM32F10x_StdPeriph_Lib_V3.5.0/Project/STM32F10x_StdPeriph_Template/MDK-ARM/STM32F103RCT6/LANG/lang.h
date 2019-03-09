/****************************************************
*��Ȩ���У�������ʤ���ҽ����е���޹�˾
*�ļ�����
*����˵����
*���ߣ�
*�������ڣ�
*�޸ļ�¼��//�޸�����д
*�޸���/���ڣ�//�޸�����д
*****************************************************/
#ifndef  __LANG_H_
#define  __LANG_H_

/*****************************************************
*ͷ�ļ�����
******************************************************/
#include <stm32f10x.h> 
//#include "config_module.h"
/******************************************************
*�ڲ��궨��
******************************************************/

/******************************************************
*���Ͷ���
******************************************************/
typedef enum{
	SET_STR_INDEX = 0, //����
	FLOW_STR_INDEX, //����
	TIME_SET_INDEX, //ʱ������
	TIME_INDEX, //ʱ��
	DATE_INDEX, //����
	BACK_INDEX, //����
	ENGLISH_INDEX,//Ӣ��
	CHINESE_INDEX,//����
	MODE_INDEX,//ģʽ
	MORE_INDEX,//����
	LANG_INDEX, //����
	INFO_INDEX, //��Ϣ
	GAS_INDEX,//����
	BLEND_INDEX,//���
	O2_INDEX,//����
	TWO_INDEX,//2
	ONLY_INDEX,//����
	ELECTRONIC_GAS_BLENDER_INDEX,//lectronic Gas Blender
	SN_INDEX,//sn
	SOFT_VER_INDEX,//����汾
	HARD_VER_INDEX,//Ӳ���汾
	MODEL_INDEX,//�ͺ�
	ALARM_REVIEW_INDEX, //�����ع�
	ALARM_INDEX,//����
	LOSS_GAS_INDEX,// "������",//
	CHECK_GAS_SUPPLY_INDEX,// "������Դ",
	SYS_FAILURE_INDEX,// "ϵͳ����",
	LOW_FIO2_INDEX,// "����Ũ��",
	HIGH_FIO2_INDEX,// "����Ũ��",
	BAT_LOW_INDEX,// "��ص�����",
	MORE_FLOW_INDEX,// "��������,�������Ũ������",
	POWER_OFF_INDEX,// "�ػ�֮ǰ��ֹͣ����"
	SHUTDOWN_INDEX,//ϵͳ�ػ�
	HAVE_NO_ALARM_INDEX,//�ޱ���
	RESET_TIME_INDEX,//����������ʱ��
	BAT_LOW_START_INDEX,//��ص�����!
	WARNING_INDEX,//����
	LOW_PRESS_MESSAGE_INDEX,//��򿪻����������ӣ�
	LOW_PRESS_MESSAGE_2_INDEX,//��򿪻����������ӣ�
	YES_INDEX,//��
	NO_INDEX,//��
	AIR_SENSOR_ERR_INDEX,//��������������
	OXY_SENSOR_ERR_INDEX,//��������������
	OPT_SENSOR_ERR_INDEX,//��ѡ����������
	SENSOR_ERR_INDEX,//����������
	EEPROM_INDEX,//EEPROM
	FAULT_INDEX,//����
	EEPROM_WARNING_INDEX,//eeprom������ʾ ��
	EEPROM_WARNING_2_INDEX,//eeprom������ʾ ��
	EXIT_RUN_STATUS_TIP_INDEX,//Press 3 seconds to exit!
	EXIT_RUN_STATUS_TIP_2_INDEX,
	MAX_STR_CNT 
}LANG_STR_INDEX;

typedef enum {
	ENGLISH,
	CHINESE
}LANG_TYPE;

typedef struct{
	LANG_TYPE lang; //  ENGLISH
	s16 offset; //��ʾƫ��
	u8* str;//�ַ���
}LANG_STR_TYPE;
/******************************************************
*�ⲿ��������
******************************************************/
extern LANG_STR_TYPE* lang_array;

void init_lang(void);
void modify_lang(u32 lang);
#endif
