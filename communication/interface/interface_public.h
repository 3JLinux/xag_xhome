#ifndef _INTERFACE_PUBLIC_H_
#define _INTERFACE_PUBLIC_H_
#include "interface_info.h"


extern void SuperX4_LocationState_check(void);
extern void SuperX4_LocationState_set(void);
extern void SuperX4_Door_Set(uint8_t value);
extern void SuperX4_Charge_Set(uint8_t value);
extern void get_SuperX4_GetBatteryConfig(uint8_t value);
extern void get_SuperX4_GetBatteryInfo(uint8_t value);
extern void get_SuperX4_GetLossCount(uint8_t value);
extern void get_SuperX4_GetChargeInfo(uint8_t value);
extern void get_SuperX4_CleanDoorOverCurrent(uint8_t value);
extern void set_SuperX4_TestMode(uint8_t value);
extern void set_SuperX4_LeftControlT(uint8_t value);
extern void set_SuperX4_RightControlT(uint8_t value);
extern void set_SuperX4_DoorControlT(uint8_t value);
extern void set_SuperX4_WakeUpT(uint8_t value);
extern void set_SuperX4_ShutDown(uint8_t value);
extern void set_SuperX4_HandShakeT(uint8_t value);
extern void set_SuperX4_GetBatteryConfigT(uint8_t value);
extern void set_SuperX4_GetBatteryInfoT(uint8_t value);
extern void set_SuperX4_GetLossCountT(uint8_t value);
extern void set_SuperX4_GetChargeInfoT(uint8_t value);
extern void set_SuperX4_CleanDoorOverCurrentT(uint8_t value);
extern void set_SuperX4_ChargeT(uint8_t value);
extern void set_SuperX4_CheckSocT(uint8_t value);
extern void SuperX4_MissionInform_set(uint8_t value);

#endif // !INTERFACE_PUBLIC_H_