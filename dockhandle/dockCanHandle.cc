#include <string.h>
#include <iostream>
#include "../communication/interface/interface_public.h"
#include "../communication/message/msg_public.h"
#include "dockCanHandle.h"


using namespace std;
using namespace xag_can;

DOCK_CAN_DATA_T dockCanData; //机场内需要上传到云端信息的全局变量
DOCK_TEMP_T dockTemp; //机场内环境信息全局变量

//舱门状态设置
enum DOOR_SET
{
    close = 1,
    open = 2
};

//任务状态
enum MISSION_STATE
{
    no_mission = 0,
    new_mission = 1
}missionstate_t;

enum MISSION_STATE missionState;

void setMissionStatus(enum MISSION_STATE missionState);



#define CHARGE_COMPLETE 3


/******************************************
 * 函数原型:	void mission_inform(void)
 * 功能:		给can发送任务通知  
 * 输入:		void
 * 返回:		void
 *****************************************/
void mission_inform(void)
{
    SuperX4_MissionInform_set(new_mission);
} 


/******************************************
 * 函数原型:	void openDoor(void)
 * 功能:		给can发送舱门开启   
 * 输入:		void
 * 返回:		void
 *****************************************/
void openDoor(void)
{
    enum DOOR_SET doorSet = open;
    SuperX4_Door_Set(doorSet);
    missionState = new_mission;
    setMissionStatus(missionState);
}


/******************************************
 * 函数原型:	void flyOutoffDoor(void)
 * 功能:		给can发送飞机离舱   
 * 输入:		void
 * 返回:		void
 *****************************************/
void flyOutoffDoor(void)
{
    SuperX4_LocationState_set();
}


/******************************************
 * 函数原型:	void dockGetTemp(void)
 * 功能:		通过can获取机场内环境信息   
 * 输入:		void
 * 返回:		void
 *****************************************/
void dockGetTemp(void)
{
    dockTemp.SABInTemperature = SAB_0X321.SAB_0X321.SAB_InTemperature;
    dockTemp.SABInHumidity = SAB_0X321.SAB_0X321.SAB_InHumidity;
    dockTemp.SABCabinTemperature = SAB_0X321.SAB_0X321.SAB_CabinTemperature;
    dockTemp.SABEBoxTemperature = SAB_0X321.SAB_0X321.SAB_EBoxTemperature;
    dockTemp.SABOutletTemperature = SAB_0X321.SAB_0X321.SAB_OutletTemperature;

    cout << "SABInTemperature:" << SAB_0X321.SAB_0X321.SAB_InTemperature << endl;
    cout << "SABInHumidity:" << SAB_0X321.SAB_0X321.SAB_InHumidity << endl;
    cout << "SABCabinTemperature:" << SAB_0X321.SAB_0X321.SAB_CabinTemperature << endl;
    cout << "SABEBoxTemperature" << SAB_0X321.SAB_0X321.SAB_EBoxTemperature << endl;
    cout << "SABOutletTemperature" << SAB_0X321.SAB_0X321.SAB_OutletTemperature << endl;
}


/******************************************
 * 函数原型:	void dockGetData(void)
 * 功能:		通过can获取需要上传的信息   
 * 输入:		void
 * 返回:		void
 *****************************************/
void dockGetData(void)
{
    dockCanData.SABChargeCmuState = SAB_0X320.SAB_0X320.SAB_ChargeCmuState;
    dockCanData.SABCellVoltage = SAB_0X29x.SAB_0X29x.SAB_0X292.SAB_80_CellVoltage1;
    dockCanData.SABRealCapacity = SAB_0X29x.SAB_0X29x.SAB_0X291.SAB_80_RealCapacity;
    dockCanData.SABDesignCapacity = SAB_0X28x.SAB_0X28x.SAB_0X280.DesignCapacity;
    dockCanData.SABSOC = SAB_0X29x.SAB_0X29x.SAB_0X296.SAB_80_SOC;
    dockCanData.SABYDN_WDT = SAB_0X320.SAB_0X320.SAB_YDN_WDT;
    dockCanData.SABSmokeAlarmState = SAB_0X400.SAB_0X400.SAB_LoseSmokeSensor;
    dockCanData.SABFanState = SAB_0X320.SAB_0X320.SAB_FanState;
    dockCanData.SABFanSpeed = SAB_0X320.SAB_0X320.SAB_FanSpeed0;
    dockCanData.LoadingTaskCompleted = 0;
    dockCanData.SecurityCheckConfirmation = 1;
    dockCanData.SABLefDoorState = SAB_0X320.SAB_0X320.SAB_LefDoorState;
    dockCanData.SABRightDoorState = SAB_0X320.SAB_0X320.SAB_RightDoorState;
    dockCanData.SABBattCmuState = SAB_0X320.SAB_0X320.SAB_BattCmuState;
    dockCanData.SABChargeState = SAB_0X320.SAB_0X320.SAB_ChargeState;
    dockCanData.SABRequestVol = SAB_0X31x.SAB_0X31x.SAB_0X310.SAB_83_RequestVol;
    dockCanData.SABRequestCur = SAB_0X31x.SAB_0X31x.SAB_0X310.SAB_83_RequestCur;
    dockCanData.PlaneState = 1;
    dockCanData.DockedState = SAB_0X320.SAB_0X320.SAB_LocationState;
    
    dockCanData.SABDoorOverCurrent = SAB_0X400.SAB_0X400.SAB_DoorOverCurrent;
    dockCanData.SABLeftSeneorFault = SAB_0X400.SAB_0X400.SAB_LeftSeneorFault;
    dockCanData.SABRightSeneorFault = SAB_0X400.SAB_0X400.SAB_RightSeneorFault;
    dockCanData.SABLeftOprationOvertime = SAB_0X400.SAB_0X400.SAB_LeftOperationOvertime;
    dockCanData.SABRightOperationOvertime = SAB_0X400.SAB_0X400.SAB_RightOperationOvertime;
    dockCanData.SABOverTemperature = SAB_0X400.SAB_0X400.SAB_OverTemperature;
    dockCanData.SABUnderTemperature = SAB_0X400.SAB_0X400.SAB_UnderTemperature;
    dockCanData.SABSmokeDetected = SAB_0X400.SAB_0X400.SAB_SmokeDetected;
    dockCanData.SABLeakDetected = SAB_0X400.SAB_0X400.SAB_LeakDetected;
    dockCanData.SABLoseCharger = SAB_0X400.SAB_0X400.SAB_LoseCharger;
    dockCanData.SABLostSenor = 0;

    cout << "SABChargeCmuState:" << SAB_0X320.SAB_0X320.SAB_ChargeCmuState << endl;
    cout << "SABCellVoltage:" << SAB_0X29x.SAB_0X29x.SAB_0X292.SAB_80_CellVoltage1 << endl;
    cout << "SABRealCapacity:" << SAB_0X29x.SAB_0X29x.SAB_0X291.SAB_80_RealCapacity << endl;
    cout << "SABDesignCapacity:" << SAB_0X28x.SAB_0X28x.SAB_0X280.DesignCapacity << endl;
    cout << "SABSOC:" << SAB_0X29x.SAB_0X29x.SAB_0X296.SAB_80_SOC << endl;
    cout << "SABYDN_WDT:" << SAB_0X320.SAB_0X320.SAB_YDN_WDT << endl;
    cout << "SABSmokeAlarmState:" << SAB_0X400.SAB_0X400.SAB_LoseSmokeSensor << endl;
    cout << "SABFanState:" << SAB_0X320.SAB_0X320.SAB_FanState << endl;
    cout << "SABFanSpeed:" << SAB_0X320.SAB_0X320.SAB_FanSpeed0 << endl;
    cout << "LoadingTaskCompleted:" << dockCanData.LoadingTaskCompleted << endl;
    cout << "SecurityCheckConfirmation:" << dockCanData.SecurityCheckConfirmation << endl;
    cout << "SABLefDoorState:" << SAB_0X320.SAB_0X320.SAB_LefDoorState << endl;
    cout << "SABRightDoorState:" << SAB_0X320.SAB_0X320.SAB_RightDoorState << endl;
    cout << "SABBattCmuState" << SAB_0X320.SAB_0X320.SAB_BattCmuState << endl;
    cout << "SABChargeState:" << SAB_0X320.SAB_0X320.SAB_ChargeState << endl;
    cout << "SABRequestVol:" << SAB_0X31x.SAB_0X31x.SAB_0X310.SAB_83_RequestVol << endl;
    cout << "SABRequestCur:" << SAB_0X31x.SAB_0X31x.SAB_0X310.SAB_83_RequestCur << endl;
    cout << "PlaneState:" << dockCanData.PlaneState << endl;
    cout << "DockedState:" << SAB_0X320.SAB_0X320.SAB_LocationState << endl;
    cout << "SABDoorOverCurrent:" << SAB_0X400.SAB_0X400.SAB_DoorOverCurrent << endl;
    cout << "SABLeftSeneorFault:" << SAB_0X400.SAB_0X400.SAB_LeftSeneorFault << endl;
    cout << "SABRightSeneorFault:" << SAB_0X400.SAB_0X400.SAB_RightSeneorFault << endl;
    cout << "SABLeftOprationOvertime:" << SAB_0X400.SAB_0X400.SAB_LeftOperationOvertime << endl;
    cout << "SABRightOperationOvertime:" << SAB_0X400.SAB_0X400.SAB_RightOperationOvertime << endl;
    cout << "SABOverTemperature:" << SAB_0X400.SAB_0X400.SAB_OverTemperature << endl;
    cout << "SABUnderTemperature:" << SAB_0X400.SAB_0X400.SAB_UnderTemperature << endl;
    cout << "SABSmokeDetected:" << SAB_0X400.SAB_0X400.SAB_SmokeDetected << endl;
    cout << "SABLeakDetected:" <<  SAB_0X400.SAB_0X400.SAB_LeakDetected << endl;
    cout << "SABLoseCharger:" << SAB_0X400.SAB_0X400.SAB_LoseCharger << endl;
    cout << "SAB_FlyCondition:" << SAB_0X320.SAB_0X320.SAB_FlyCondition << endl;
}


/******************************************
 * 函数原型:	uint8_t getFlyCondition(void)
 * 功能:		   
 * 输入:		   none
 * 返回:		   none
 *****************************************/
uint8_t getFlyCondition(void)
{
    return SAB_0X320.SAB_0X320.SAB_FlyCondition;
}


/******************************************
 * 函数原型:	uint8_t isChargeComplete(void)
 * 功能:		查询充电是否完成   
 * 输入:		void
 * 返回:		0：未完成 1：完成
 *****************************************/
uint8_t isChargeComplete(void)
{
    if(SAB_0X320.SAB_0X320.SAB_ChargeState == CHARGE_COMPLETE)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/******************************************
 * 函数原型:	uint8_t isDoorOpenComplete(void)
 * 功能:		查询舱门是否开启完成   
 * 输入:		void
 * 返回:		0：未完成 1：完成
 *****************************************/
uint8_t isDoorOpenComplete(void)
{
    if(SAB_0X320.SAB_0X320.SAB_LefDoorState ==1 && SAB_0X320.SAB_0X320.SAB_RightDoorState == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/******************************************
 * 函数原型:	uint8_t isDoorOpenComplete(void)
 * 功能:        
 * 输入:		void
 * 返回:		0：未完成 1：完成
 *****************************************/
void setMissionStatus(enum MISSION_STATE missionState)  
{
    SuperX4_0x101.SuperX4_0x101.SuperX4_TaskState = 1;
}