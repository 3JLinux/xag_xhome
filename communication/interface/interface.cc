#include "interface.h"
#include "../can_driver/can_com.h"
#include "../message/msg_public.h"
#include <stdio.h>

using namespace xag_can;

/******************************************
 * 函数原型:	void SuperX4_LocationState_check(void)
 * 功能:		   check airplane location 不需要调用，程序1ms执行一次
 * 输入:		   none
 * 返回:		   none
 *****************************************/
void SuperX4_LocationState_check(void)
{
    if(SAB_0X320.SAB_0X320.SAB_LocationState == 0)
    {
        SuperX4_0x101.SuperX4_0x101.SuperX4_LocationState = 0;
    }
} 


/******************************************
 * 函数原型:	void SuperX4_LocationState_set(void)
 * 功能:		   
 * 输入:		   none
 * 返回:		   none
 *****************************************/
void SuperX4_MissionInform_set(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x101.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_101 = 1;
   SuperX4_0x101.SuperX4_0x101.SuperX4_TaskState = value;
}


/******************************************
 * 函数原型:	void SuperX4_LocationState_set(void)
 * 功能:		   设置飞机离开机舱，需要外部调用，视飞机真实离仓状态调用
 * 输入:		   none
 * 返回:		   none
 *****************************************/
void SuperX4_LocationState_set(void)
{
    SuperX4_0x101.SuperX4_0x101.SuperX4_LocationState = 1;
} 

/******************************************
 * 函数原型:	void SuperX4_Door_Set(uint8_t value)
 * 功能:	      舱门开关控制
 * 输入:		   value
 *              0：无操作
 *              1：Close
 *              2：Open
 * 返回:		   none
 *****************************************/
void SuperX4_Door_Set(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x100.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_100 = 1;
   SuperX4_0x100.SuperX4_0x100.SuperX4_DoorSet = value;
} 

/******************************************
 * 函数原型:	void SuperX4_Charge_Set(uint8_t value)
 * 功能:	      充电开关控制
 * 输入:		   value
 *              0：无操作
 *              1：Close
 *              2：Open
 * 返回:		   none
 *****************************************/
void SuperX4_Charge_Set(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x100.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_100 = 1;
   SuperX4_0x100.SuperX4_0x100.SuperX4_ChargeSet = value;
} 

/******************************************
 * 函数原型:	void get_SuperX4_GetBatteryConfig(void)
 * 功能:	      获取电池配置信息
 * 输入:		   value
 *              0：无操作
 *              1：enable
 * 返回:		   none
 *****************************************/
void get_SuperX4_GetBatteryConfig(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x100.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_100 = 1;
   SuperX4_0x100.SuperX4_0x100.SuperX4_GetBatterConfig = value;
} 

/******************************************
 * 函数原型:	void get_SuperX4_GetBatteryInfo(void)
 * 功能:	      获取电池充电参数
 * 输入:		   value
 *              0：无操作
 *              1：enable
 * 返回:		   none
 *****************************************/
void get_SuperX4_GetBatteryInfo(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x100.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_100 = 1;
   SuperX4_0x100.SuperX4_0x100.SuperX4_GetBatteryInfo = value;
} 

/******************************************
 * 函数原型:	void get_SuperX4_GetLossCount(void)
 * 功能:	      获取电池运行参数
 * 输入:		   value
 *              0：无操作
 *              1：enable
 * 返回:		   none
 *****************************************/
void get_SuperX4_GetLossCount(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x100.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_100 = 1;
   SuperX4_0x100.SuperX4_0x100.SuperX4_GetLossCount = value;
} 

/******************************************
 * 函数原型:	void get_SuperX4_GetChargeInfo(void)
 * 功能:	      获取充电器参数
 * 输入:		   value
 *              0：无操作
 *              1：enable
 * 返回:		   none
 *****************************************/
void get_SuperX4_GetChargeInfo(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x100.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_100 = 1;
   SuperX4_0x100.SuperX4_0x100.SuperX4_GetChargeInfo = value;
}

/******************************************
 * 函数原型:	void get_SuperX4_CleanDoorOverCurrent(void)
 * 功能:	      清除舱门过流故障
 * 输入:		   value
 *              0：无操作
 *              1：enable
 * 返回:		   none
 *****************************************/
void get_SuperX4_CleanDoorOverCurrent(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x100.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_100 = 1;
   SuperX4_0x100.SuperX4_0x100.SuperX4_CleanDoorOverCurrent = value;
} 

/******************************************
 * 函数原型:	  void set_SuperX4_TestMode(void)
 * 功能:	       清除舱门过流故障
 * 输入:	       value
 *              0：无操作
 *              1：测试模式使能
 *              2：测试模式失能
 * 返回:		    none
 *****************************************/
void set_SuperX4_TestMode(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_TestMode = value;
}

/******************************************
 * 函数原型:	  void set_SuperX4_TestMode(void)
 * 功能:	       左门控制
 * 输入:	       value
 *              0：无操作
 *              1：开启左门
 *              2：关闭左门
 * 返回:		    none
 *****************************************/
void set_SuperX4_LeftControlT(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_LeftControlT = value;
}

/******************************************
 * 函数原型:	  void set_SuperX4_RightControlT(void)
 * 功能:	       右门控制
 * 输入:	       value
 *              0：无操作
 *              1：开启右门
 *              2：关闭右门
 * 返回:		    none
 *****************************************/
void set_SuperX4_RightControlT(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_RightControlT = value;
}

/******************************************
 * 函数原型:	  void set_SuperX4_DoorControlT(void)
 * 功能:	       舱门控制
 * 输入:	       value
 *              0：无操作
 *              1：开启舱门
 *              2：关闭舱门
 * 返回:		    none
 *****************************************/
void set_SuperX4_DoorControlT(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_DoorControlT = value;
}

/******************************************
 * 函数原型:	  void set_SuperX4_WakeUpT(void)
 * 功能:	       唤醒测试
 * 输入:	       value
 *              0：无操作
 *              1：开启
 *              2：关闭
 * 返回:		    none
 *****************************************/
void set_SuperX4_WakeUpT(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_WakeUpT = value;
}

/******************************************
 * 函数原型:	  void set_SuperX4_ShutDown(void)
 * 功能:	       关机测试
 * 输入:	       value
 *              0：无操作
 *              1：开启
 *              2：关闭
 * 返回:		    none
 *****************************************/
void set_SuperX4_ShutDown(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_ShutDown = value;
}

/******************************************
 * 函数原型:	  void set_SuperX4_HandShakeT(void)
 * 功能:	       通信握手测试
 * 输入:	       value
 *              0：无操作
 *              1：开启
 *              2：关闭
 * 返回:		    none
 *****************************************/
void set_SuperX4_HandShakeT(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_HandShakeT = value;
}

/******************************************
 * 函数原型:	  void set_SuperX4_GetBatteryConfigT(void)
 * 功能:	       获取电池配置信息
 * 输入:	       value
 *              0：无操作
 *              1：开启
 *              2：关闭
 * 返回:		    none
 *****************************************/
void set_SuperX4_GetBatteryConfigT(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_GetBatteryConfigT = value;
}

/******************************************
 * 函数原型:	  void set_SuperX4_GetBatteryInfoT(void)
 * 功能:	       获取电池充电交互参数
 * 输入:	       value
 *              0：无操作
 *              1：开启
 *              2：关闭
 * 返回:		    none
 *****************************************/
void set_SuperX4_GetBatteryInfoT(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_GetBatteryInfoT = value;
}

/******************************************
 * 函数原型:	  void set_SuperX4_GetLossCountT(void)
 * 功能:	       获取电池运行参数
 * 输入:	       value
 *              0：无操作
 *              1：开启
 *              2：关闭
 * 返回:		    none
 *****************************************/
void set_SuperX4_GetLossCountT(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_GetLossCountT = value;
}

/******************************************
 * 函数原型:	  void set_SuperX4_GetChargeInfoT(void)
 * 功能:	       获取电池充电参数
 * 输入:	       value
 *              0：无操作
 *              1：开启
 *              2：关闭
 * 返回:		    none
 *****************************************/
void set_SuperX4_GetChargeInfoT(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_GetChargeInfoT = value;
}

/******************************************
 * 函数原型:	  void set_SuperX4_CleanDoorOverCurrentT(void)
 * 功能:	       清除舱门过流故障
 * 输入:	       value
 *              0：无操作
 *              1：开启
 *              2：关闭
 * 返回:		    none
 *****************************************/
void set_SuperX4_CleanDoorOverCurrentT(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_CleanDoorOverCurrentT = value;
}

/******************************************
 * 函数原型:	  void set_SuperX4_ChargeT(void)
 * 功能:	       充电测试
 * 输入:	       value
 *              0：无操作
 *              1：开启
 *              2：关闭
 * 返回:		    none
 *****************************************/
void set_SuperX4_ChargeT(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_ChargeT = value;
}

/******************************************
 * 函数原型:	  void set_SuperX4_CheckSocT(void)
 * 功能:	       检测电池电量
 * 输入:	       value
 *              0：无操作
 *              1：开启
 *              2：关闭
 * 返回:		    none
 *****************************************/
void set_SuperX4_CheckSocT(uint8_t value)
{
   uint64_t *prt = NULL;

   prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
   *prt = 0;

   Msg_info.SuperX4_CANSend_102 = 1;
   SuperX4_0x102.SuperX4_0x102.SuperX4_CheckSocT = value;
}