/****************************************************
*版权所有：深圳永胜宏基医疗器械有限公司
*文件名：
*功能说明：
*作者：
*生成日期：
*修改记录：//修改了则写
*修改者/日期：//修改了则写
*****************************************************/
#ifndef  __LANG_H_
#define  __LANG_H_

/*****************************************************
*头文件包含
******************************************************/
#include <stm32f10x.h> 
//#include "config_module.h"
/******************************************************
*内部宏定义
******************************************************/

/******************************************************
*类型定义
******************************************************/
typedef enum{
	SET_STR_INDEX = 0, //设置
	FLOW_STR_INDEX, //流量
	TIME_SET_INDEX, //时间设置
	TIME_INDEX, //时间
	DATE_INDEX, //日期
	BACK_INDEX, //返回
	ENGLISH_INDEX,//英文
	CHINESE_INDEX,//中文
	MODE_INDEX,//模式
	MORE_INDEX,//更多
	LANG_INDEX, //语言
	INFO_INDEX, //信息
	GAS_INDEX,//气体
	BLEND_INDEX,//混合
	O2_INDEX,//氧气
	TWO_INDEX,//2
	ONLY_INDEX,//仅有
	ELECTRONIC_GAS_BLENDER_INDEX,//lectronic Gas Blender
	SN_INDEX,//sn
	SOFT_VER_INDEX,//软件版本
	HARD_VER_INDEX,//硬件版本
	MODEL_INDEX,//型号
	ALARM_REVIEW_INDEX, //报警回顾
	ALARM_INDEX,//报警
	LOSS_GAS_INDEX,// "无气体",//
	CHECK_GAS_SUPPLY_INDEX,// "请检查气源",
	SYS_FAILURE_INDEX,// "系统错误",
	LOW_FIO2_INDEX,// "低氧浓度",
	HIGH_FIO2_INDEX,// "高氧浓度",
	BAT_LOW_INDEX,// "电池电量低",
	MORE_FLOW_INDEX,// "更多流量,请更改氧浓度设置",
	POWER_OFF_INDEX,// "关机之前请停止运行"
	SHUTDOWN_INDEX,//系统关机
	HAVE_NO_ALARM_INDEX,//无报警
	RESET_TIME_INDEX,//请重新设置时间
	BAT_LOW_START_INDEX,//电池电量低!
	WARNING_INDEX,//警告
	LOW_PRESS_MESSAGE_INDEX,//请打开环境空气盖子！
	LOW_PRESS_MESSAGE_2_INDEX,//请打开环境空气盖子！
	YES_INDEX,//是
	NO_INDEX,//否
	AIR_SENSOR_ERR_INDEX,//空气传感器错误
	OXY_SENSOR_ERR_INDEX,//氧气传感器错误
	OPT_SENSOR_ERR_INDEX,//可选传感器错误
	SENSOR_ERR_INDEX,//传感器错误
	EEPROM_INDEX,//EEPROM
	FAULT_INDEX,//错误
	EEPROM_WARNING_INDEX,//eeprom错误提示 上
	EEPROM_WARNING_2_INDEX,//eeprom错误提示 下
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
	s16 offset; //显示偏移
	u8* str;//字符串
}LANG_STR_TYPE;
/******************************************************
*外部函数声明
******************************************************/
extern LANG_STR_TYPE* lang_array;

void init_lang(void);
void modify_lang(u32 lang);
#endif
