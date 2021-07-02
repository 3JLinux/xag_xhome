#ifndef _MSG_INFO_H_
#define _MSG_INFO_H_


#include <stdint.h>

#define NORMALID  (0x320)
#define GETINDEX(id)	((id&0x0f)*8)
#define LOSSTTIME	(2*1000)	// 2s

namespace xag_can
{
    
	typedef struct message_info
	{
		uint32_t SuperX4_CANSend_100	:1;
        uint32_t SuperX4_CANSend_101	:1;
        uint32_t SuperX4_CANSend_102	:1;
		uint32_t Lost_SAB			    :1;
		uint32_t SuperX4_SendFail	    :1;
		uint32_t reserverd	            :22;		
		/* data */
	}Message_info_st;

	typedef enum
	{
		SuperX4_0x100_ = 0,
		SuperX4_0x101_ = 1,
		SuperX4_0x102_ = 2,
		CanSendTypeNum
	}CanSendType_en;


	typedef enum
	{
		SAB_0x28x_=0,
		SAB_0x29x_,
		SAB_0x30x_,
		SAB_0x31x_,
		SAB_0x320_,
		SAB_0x321_,
		SAB_0x322_,
		CanRcvTypeNum
	}CanRcvType_en;

	typedef struct RcvMsg
	{
		const uint32_t index;
		uint8_t *const address;
		RcvMsg(uint32_t num,uint8_t*ptr):index(num),address(ptr){}
		/* data */
	}RcvMsg;

	typedef struct
	{
		uint32_t	SuperX4_DoorSet					:2;
		uint32_t	SuperX4_ChargeSet				:2;
		uint32_t	SuperX4_GetBatterConfig			:1;
		uint32_t	SuperX4_GetBatteryInfo			:1;
		uint32_t	SuperX4_GetLossCount			:1;
		uint32_t	SuperX4_GetChargeInfo			:1;
		uint32_t	SuperX4_CleanDoorOverCurrent	:1;
		uint32_t	SuperX4_Reserved1				:7;
		uint32_t	SuperX4_Reserved2				:8;
		uint32_t	SuperX4_Reserved3				:8;
		uint32_t	SuperX4_Reserved4				:8;
		uint32_t	SuperX4_Reserved5				:8;
		uint32_t	SuperX4_Reserved6				:8;
		uint32_t	SuperX4_Reserved7				:8;
	}SuperX4_0x100_st;


	typedef struct
	{
		uint32_t	SuperX4_LocationState			:2;
		uint32_t	SuperX4_TaskState				:2;
		uint32_t	SuperX4_Reserved1				:4;
		uint32_t	SuperX4_Reserved2				:8;
		uint32_t	SuperX4_Reserved3				:8;
		uint32_t	SuperX4_Reserved4				:8;
		uint32_t	SuperX4_Reserved5				:8;
		uint32_t	SuperX4_Reserved6				:8;
		uint32_t	SuperX4_Reserved7				:8;
		uint32_t	SuperX4_Reserved8				:8;
	}SuperX4_0x101_st;

	typedef struct
	{
		uint32_t	SuperX4_TestMode				:2;
		uint32_t	SuperX4_LeftControlT			:2;
		uint32_t	SuperX4_RightControlT			:2;
		uint32_t	SuperX4_DoorControlT			:2;
		uint32_t	SuperX4_WakeUpT					:2;
		uint32_t	SuperX4_ShutDown				:2;
		uint32_t	SuperX4_HandShakeT				:2;
		uint32_t	SuperX4_GetBatteryConfigT		:2;
		uint32_t	SuperX4_GetBatteryInfoT			:2;
		uint32_t	SuperX4_GetLossCountT			:2;
		uint32_t	SuperX4_GetChargeInfoT			:2;
		uint32_t	SuperX4_CleanDoorOverCurrentT	:2;
		uint32_t	SuperX4_ChargeT					:2;
		uint32_t	SuperX4_CheckSocT				:2;
		uint32_t	SuperX4_Reserved1				:4;
		uint32_t	SuperX4_Reserved2				:8;
		uint32_t	SuperX4_Reserved3				:8;
		uint32_t	SuperX4_Reserved4				:8;
		uint32_t	SuperX4_Reserved5				:8;

	}SuperX4_0x102_st;

	typedef struct
	{
		uint32_t DesignCapacity;					//mAh，设计容量
		uint16_t NormalVoltage;						//mV，电芯标称电压
		uint16_t FullVoltage;						//mV，电芯满电电压
	}SAB_0X280_st;

	typedef struct
	{
		uint16_t CutOffVoltage;						//mV，电芯截止电压
		uint16_t Series;							//S，电芯数量
		uint16_t DischargeRate;						//0.1C，放电倍率
		uint16_t ChargeRate;						//0.1C，充电倍率
	}SAB_0X281_st;

	typedef struct
	{
		int32_t ChargeCurrent;						//mA，充电阈值
		int32_t DischargeCurrent;					//mA，放电阈值
	}SAB_0X282_st;
	typedef struct
	{
		int32_t ChargeOverCurrent;					//mA，充电过流阈值
		int32_t ChargeOverCurrentRecovery;			//mA，充电过流恢复
	}SAB_0X283_st;

	typedef struct
	{
		int32_t DischargeOverCurrent;				//mA，放电过流阈值
		int32_t DischargeOverCurrentRecovery;		//mA，放电过流恢复
	}SAB_0X284_st;
	typedef struct
	{
		uint16_t OverVoltage;						//mV，过压阈值
		uint16_t OverVoltageRecovery;				//mV，过压恢复
		uint16_t UnderVoltage;						//mV，欠压阈值
		uint16_t UnderVoltageRecovery;				//mV，欠压恢复
	}SAB_0X285_st;
	typedef struct
	{
		uint16_t BalanceThreshold;					//mV，平衡阈值
		uint16_t BalanceError;						//mV，平衡压差
		uint16_t PowerOffVoltage;					//mV，断电电压
		uint16_t WarningSOC;						//0.1%，警告电量
	}SAB_0X286_st;
	typedef struct
	{
		int16_t OverTemperature;					//0.1℃，过温阈值
		int16_t OverTemperatureRecovery;			//0.1℃，过温恢复
		int16_t UnderTemperature;					//0.1℃，低温阈值
		int16_t UnderTemperatureRecovery;			//0.1℃，低温恢复
	}SAB_0X287_st;

	typedef struct
	{
		SAB_0X280_st SAB_0X280;
		SAB_0X281_st SAB_0X281;
		SAB_0X282_st SAB_0X282;
		SAB_0X283_st SAB_0X283;
		SAB_0X284_st SAB_0X284;
		SAB_0X285_st SAB_0X285;
		SAB_0X286_st SAB_0X286;
		SAB_0X287_st SAB_0X287;
	}SAB_0X28x_st;


	typedef struct
	{
		uint16_t SAB_80_Version;							//信息版本，其中高 8 位为电芯数，低 8 位为版本号。
		uint16_t SAB_80_RemainTime;						//秒，预测剩余续航时间，超过 65535 秒时，此值为 65535。
		uint32_t SAB_80_DesignCapacity;					//mAh，设计容量
	}SAB_0X290_st;
	typedef struct
	{
		uint32_t SAB_80_RealCapacity;						//mAh，实际容量
		uint32_t SAB_80_Status;							//电池各种状态
	}SAB_0X291_st;
	typedef struct
	{
		int32_t Current;							//mA，正值为放电，负值为充电
		uint16_t SAB_80_CellVoltage1;				//mV，1节电芯电压
		uint16_t SAB_80_CellVoltage2;				//mV，1节电芯电压

	}SAB_0X292_st;
	typedef struct
	{
		uint16_t SAB_80_CellVoltage3;				//mV，1节电芯电压
		uint16_t SAB_80_CellVoltage4;				//mV，1节电芯电压
		uint16_t SAB_80_CellVoltage5;				//mV，1节电芯电压
		uint16_t SAB_80_CellVoltage6;				//mV，1节电芯电压
	}SAB_0X293_st;

	typedef struct
	{
		uint16_t SAB_80_CellVoltage7;				//mV，1节电芯电压
		uint16_t SAB_80_CellVoltage8;				//mV，1节电芯电压
		uint16_t SAB_80_CellVoltage9;				//mV，1节电芯电压
		uint16_t SAB_80_CellVoltage10;				//mV，1节电芯电压
	}SAB_0X294_st;

	typedef struct
	{
		uint16_t SAB_80_CellVoltage11;				//mV，1节电芯电压
		uint16_t SAB_80_CellVoltage12;				//mV，1节电芯电压
		uint16_t SAB_80_CellTemp1;					//mV，1节电芯电压
		uint16_t SAB_80_CellTemp2;					//mV，1节电芯电压
	}SAB_0X295_st;


	typedef struct
	{
		int16_t  SAB_80_PcbTemp;					//0.1℃，PCB 温度
		uint16_t SAB_80_SOC;						//0.1%，电量百分比
		uint16_t SAB_80_CycleTimes;					//次，循环次数
		int16_t  SAB_80_MosTemp;					//0.1℃，MOS 管温度
	}SAB_0X296_st;

	typedef struct
	{
		uint16_t SAB_80_OverDsgCount;				//次，过放次数
		uint16_t SAB_80_Reserved;					//预留
		uint32_t SAB_80_utc;						//s，utc 时间
	}SAB_0X297_st;


	typedef struct
	{
		SAB_0X290_st SAB_0X290;
		SAB_0X291_st SAB_0X291;
		SAB_0X292_st SAB_0X292;
		SAB_0X293_st SAB_0X293;
		SAB_0X294_st SAB_0X294;
		SAB_0X295_st SAB_0X295;
		SAB_0X296_st SAB_0X296;
		SAB_0X297_st SAB_0X297;
	}SAB_0X29x_st;


	typedef struct
	{
		uint16_t SAB_82_Version; 							//信息版本，其中高 8 位为结构体大小，低 8 位为版本号
		uint16_t SAB_82_BatVersion; 						//电池信息版本，参考电池信息结构体
		uint32_t SAB_82_BatStatus; 						//电池信息状态，参考电池信息结构体
	}SAB_0X300_st;

	typedef struct
	{
		uint32_t SAB_82_Reserved; 							//预留
		uint16_t SAB_82_Reboot; 							//次，开机次数
		uint16_t SAB_82_Cycles; 							//次，循环次数
	}SAB_0X301_st;

	typedef struct
	{
		uint16_t SAB_82_Soh; 								//次，SOH
		uint16_t SAB_82_DynamicSOH; 						//次，动态 SOH
		uint16_t SAB_82_DsgOverCurrent; 					//次，放电过流次数
		uint16_t SAB_82_ChgOverCurrent; 					//次，充电过流次数
	}SAB_0X302_st;
	typedef struct
	{
		uint16_t SAB_82_OverTempChg; 						//次，过温充电次数
		uint16_t SAB_82_OverTempDsg; 						//次，过温放电次数
		uint16_t SAB_82_UnderTempChg; 						//次，低温充电次数
		uint16_t SAB_82_UnderTempDsg; 						//次，低温放电次数
	}SAB_0X303_st;
	typedef struct
	{
		uint16_t SAB_82_CellOverVoltage1; 					//次，电芯过压次数
		//Byte[0]：高于过压阈值+50mV
		uint16_t SAB_82_CellOverVoltage2; 					//次，电芯过压次数
		//Byte[1]：高于过压阈值+100mV；
		uint16_t SAB_82_CellUnderVoltage1; 				//电芯欠压次数
		//Byte[0]：低于截至电压+200mV
		uint16_t SAB_82_CellUnderVoltage2; 				//电芯欠压次数
		//Byte[1]：低于截至电压+100mV
	}SAB_0X304_st;
	typedef struct
	{
		uint16_t SAB_82_CellError1;						//电芯压差次数
		//Byte[0]：100-200mV
		uint16_t SAB_82_CellError2;						//电芯压差次数
		//Byte[1]：> 200mV
		uint16_t SAB_82_Reserved1;
		uint16_t SAB_82_Reserved2;
	}SAB_0X305_st;
	typedef struct
	{
		uint16_t SAB_82_Reserved3;
		uint16_t SAB_82_Reserved4;
		uint16_t SAB_82_Reserved5;
		uint16_t SAB_82_Reserved6;
	}SAB_0X306_st;
	typedef struct
	{
		uint16_t SAB_82_Reserved7;
		uint16_t SAB_82_Reserved8;
		uint16_t SAB_82_Reserved9;
		uint16_t SAB_82_Reserved10;
	}SAB_0X307_st;
	typedef struct
	{
		uint16_t SAB_82_Reserved11;
		uint16_t SAB_82_Reserved12;
		uint16_t SAB_82_Reserved13;
		uint16_t SAB_82_Reserved14;
	}SAB_0X308_st;
	typedef struct
	{
		uint16_t SAB_82_Reserved15;
		uint16_t SAB_82_Reserved16;
		uint16_t SAB_82_Reserved17;
		uint16_t SAB_82_Crc;							//2 字节 CRC
	}SAB_0X309_st;

	typedef struct
	{
		SAB_0X300_st SAB_0X300;
		SAB_0X301_st SAB_0X301;
		SAB_0X302_st SAB_0X302;
		SAB_0X303_st SAB_0X303;
		SAB_0X304_st SAB_0X304;
		SAB_0X305_st SAB_0X305;
		SAB_0X306_st SAB_0X306;
		SAB_0X307_st SAB_0X307;
		SAB_0X308_st SAB_0X308;
		SAB_0X309_st SAB_0X309;
	}SAB_0X30x_st;


	typedef struct
	{
		uint16_t SAB_83_RequestVol;						// 电压需求 数据分辨率：0.1V/位，0V 偏移量
		uint16_t SAB_83_RequestCur;						// 电流需求 数据分辨率：0.1A/位，-400A 偏移量
		uint8_t SAB_83_ChargMode;						// 充电模式(0x01：恒压充电；0x02：恒流充电)
		uint8_t SAB_83_Reserved1;
		uint8_t SAB_83_Reserved2;
		uint8_t SAB_83_Reserved3;
	}SAB_0X310_st;

	typedef struct
	{
		SAB_0X310_st SAB_0X310;
	}SAB_0X31x_st;

	typedef struct
	{
		uint32_t SAB_ChargeState				:3;
		uint32_t SAB_ChargeCmuState				:2;
		uint32_t SAB_BattCmuState				:2;
		uint32_t SAB_FlyCondition				:1;

		uint32_t SAB_LocationState				:2;
		uint32_t SAB_BattState					:2;
		uint32_t SAB_LefDoorState				:2;
		uint32_t SAB_RightDoorState				:2;

		uint32_t SAB_FanSpeed0					:4;
		uint32_t SAB_FanState					:2;
		uint32_t SAB_SmokeAlarmState			:1;
		uint32_t SAB_YDN_WDT					:1;

		uint32_t SAB_FanSpeed1					:8;

		uint32_t SAB_Lost_Sensor				:4;
		uint32_t SAB_Reserverd					:4;

		uint32_t SAB_Reserverd1					:8;
		uint32_t SAB_Reserverd2					:8;
		uint32_t SAB_Reserverd3					:8;

	}SAB_0X320_st;


	typedef struct
	{
		uint32_t SAB_InTemperature				:8;
		uint32_t SAB_InHumidity					:8;
		uint32_t SAB_CabinTemperature			:8;
		uint32_t SAB_EBoxTemperature			:8;
		uint32_t SAB_OutletTemperature			:8;

		uint32_t SAB_Reserverd					:8;
		uint32_t SAB_Reserverd1					:8;
		uint32_t SAB_Reserverd2					:8;

	}SAB_0X321_st;

	typedef struct
	{
		uint32_t SAB_DoorOverCurrent			:1;
		uint32_t SAB_LeftSeneorFault			:1;
		uint32_t SAB_RightSeneorFault			:1;
		uint32_t SAB_LeftOperationOvertime		:1;
		uint32_t SAB_RightOperationOvertime		:1;
		uint32_t SAB_OverTemperature			:1;
		uint32_t SAB_UnderTemperature			:1;
		uint32_t SAB_LoseCharger				:1;
		uint32_t SAB_LeakDetected				:1;
		uint32_t SAB_SmokeDetected				:1;
		uint32_t SAB_LoseYDN_WDT				:1;
		uint32_t SAB_LoseSmokeSensor			:1;
		uint32_t SAB_LoseTemperatureSensor		:1;
		uint32_t SAB_LoseSuperX4				:1;
		uint32_t SAB_Reserverd1					:2;
		uint32_t SAB_Reserverd2					:8;
		uint32_t SAB_Reserverd3					:8;
		uint32_t SAB_Reserverd4					:8;
		uint32_t SAB_Reserverd5					:8;
		uint32_t SAB_Reserverd6					:8;
		uint32_t SAB_Reserverd7					:8;

	}SAB_0X400_st;

	typedef union
	{
		uint8_t buff[8];
		SuperX4_0x100_st SuperX4_0x100;
	}SuperX4_0x100_un;

	typedef union
	{
		uint8_t buff[8];
		SuperX4_0x101_st SuperX4_0x101;
	}SuperX4_0x101_un;

	typedef union
	{
		uint8_t buff[8];
		SuperX4_0x102_st SuperX4_0x102;
	}SuperX4_0x102_un;

	typedef union
	{
		uint8_t buff[64];
		SAB_0X28x_st SAB_0X28x;
	}SAB_0X28x_un;

	typedef union
	{
		uint8_t buff[64];
		SAB_0X29x_st SAB_0X29x;
	}SAB_0X29x_un;

	typedef union
	{
		uint8_t buff[80];
		SAB_0X30x_st SAB_0X30x;
	}SAB_0X30x_un;

	typedef union
	{
		uint8_t buff[8];
		SAB_0X31x_st SAB_0X31x;
	}SAB_0X31x_un;

	typedef union
	{
		uint8_t buff[8];
		SAB_0X320_st SAB_0X320;
	}SAB_0X320_un;

	typedef union
	{
		uint8_t buff[8];
		SAB_0X321_st SAB_0X321;
	}SAB_0X321_un;

	typedef union
	{
		uint8_t buff[8];
		SAB_0X400_st SAB_0X400;
	}SAB_0X400_un;
	/* Exported constants --------------------------------------------------------*/

	/* Exported variables --------------------------------------------------------*/

	/* Exported functions prototypes ---------------------------------------------*/



}


#endif // !_MSG_INFO_H_