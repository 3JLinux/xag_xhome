#ifndef UNICVERLOG_H
#define UNICVERLOG_H

#include <stdio.h>
#include <stdint.h>

#define SENDER_ID_NUM 26

typedef enum{
	REGULAR = 0,
	ALARM
}BASICDEV_STA;

typedef enum{
	NORMAL = 0,
	ERROR
}ERR_STA;

typedef enum{
	CLOSE = 0,
	OPEN,
	OPENING,
	DOOR_ERROR 
}DOOR_STA;

typedef enum{
	READY_TAKE_OFF = 0,
	TAKING_OFF,
	READY_LAND,
	LANDING,
	WORKING,
	ALREADY_DOCK
}PLANE_STA;


typedef struct
{
	/************env status****************/
	int16_t SABInTemperature;					//舱内温度
	uint16_t SABInHumidity;						//舱内湿度
	int16_t SABCabinTemperature;				//上层舱温度
	int16_t SABEBoxTemperature;					//电器盒温度
	int16_t SABOutletTemperature;				//出风口温度
	uint16_t SABAirSpeed;						//环境风量
	uint32_t SABSolarIntensity;					//环境光照
	uint16_t SABRainfall;						//环境雨量
	/************basic status****************/
	int32_t Longitude; 							//经度
	int32_t Latitude;							//纬度
	int32_t Altitude;							//海拔
	int16_t SignalStrength; 					//信号强度
	uint8_t SABChargeCmuState; 					//充电器通信状态
	uint16_t SABCellVoltage;					//飞机电芯电压
	uint32_t SABRealCapacity;					//电池实际容量
	uint32_t SABDesignCapacity;					//电池设计容量
	uint16_t SABSOC;							//电池百分比
	uint8_t SABYDN_WDT;						//水浸状态
	uint8_t SABSmokeAlarmState;				//烟感状态
	uint8_t SABFanState;						//风扇反馈
	uint16_t SABFanSpeed;						//散热风扇转速
	/************work status****************/
	uint8_t LoadingTaskCompleted;				//加载任务完成
	uint8_t SecurityCheckConfirmation;			//安全检查确认
	uint8_t SABLefDoorState; 					//左舱门状态
	uint8_t SABRightDoorState; 					//右舱门状态
	uint8_t SABBattCmuState;					//电池通信状态
	uint8_t SABChargeState;						//充电状态
	uint16_t SABRequestVol;						//充电电压
	uint16_t SABRequestCur;						//充电电流
	uint8_t PlaneState;							//起飞状态
	uint8_t DockedState;						//停靠状态
	/************error status****************/
	uint8_t SABDoorOverCurrent;					//电机过流
	uint8_t SABLeftSeneorFault;					//左舱门位置传感器故障
	uint8_t SABRightSeneorFault;				//右舱门位置传感器故障
	uint8_t SABLeftOperationOvertime;			//左舱门操作超时
	uint8_t SABRightOperationOvertime;			//右舱门操作超时
	uint8_t SABOverTemperature;					//舱体过温
	uint8_t SABUnderTemperature;				//舱体低温
	uint8_t SABSmokeDetected;					//检测到烟雾
	uint8_t SABLeakDetected;					//检测到水浸
	uint8_t SABLoseCharger;						//充电器离线
	uint16_t SABLostSensor;						//传感器通信丢失
}__attribute__ ((packed)) UNICVERLOG_BODY;



typedef struct
{
        uint16_t prefix;
        uint16_t version;
        uint16_t sync_id;
        uint64_t timestamp;
        uint8_t content_type;
        uint8_t sender_length;
        uint8_t sender[SENDER_ID_NUM];
        uint16_t control_bits;
        uint32_t rule_type;
        uint16_t rule_version;
        uint32_t content_length;
        uint8_t content[0];
}__attribute__ ((packed)) UNICVERLOG_HEAD;

void unicverlog_thread_create(void);
void unicverlog_thread_wait(void);

#endif
