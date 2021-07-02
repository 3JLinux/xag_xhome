#ifndef DOCK_CAN_HANDLE_H
#define DOCK_CAN_HANDLE_H

#include <stdint.h>

typedef struct 
{
    int16_t SABInTemperature;
    uint16_t SABInHumidity;
    int16_t SABCabinTemperature;
    int16_t SABEBoxTemperature;
    int16_t SABOutletTemperature;
}DOCK_TEMP_T;

extern DOCK_TEMP_T dockTemp;

typedef struct 
{
    uint8_t SABChargeCmuState;
    uint16_t SABCellVoltage;
    uint32_t SABRealCapacity;
    uint32_t SABDesignCapacity;
    uint16_t SABSOC;
    uint8_t SABYDN_WDT;
    uint8_t SABSmokeAlarmState;
    uint8_t SABFanState;
    uint16_t SABFanSpeed;

    uint8_t LoadingTaskCompleted;
    uint8_t SecurityCheckConfirmation;
    uint8_t SABLefDoorState;
    uint8_t SABRightDoorState;
    uint8_t SABBattCmuState;
    uint8_t SABChargeState;
    uint16_t SABRequestVol;
    uint16_t SABRequestCur;
    uint8_t PlaneState;
    uint8_t DockedState;

    uint8_t SABDoorOverCurrent;
    uint8_t SABLeftSeneorFault;
    uint8_t SABRightSeneorFault;
    uint8_t SABLeftOprationOvertime;
    uint8_t SABRightOperationOvertime;
    uint8_t SABOverTemperature;
    uint8_t SABUnderTemperature;
    uint8_t SABSmokeDetected;
    uint8_t SABLeakDetected;
    uint8_t SABLoseCharger;
    uint16_t SABLostSenor;
}DOCK_CAN_DATA_T;

extern DOCK_CAN_DATA_T dockCanData;


void mission_inform(void);
void openDoor(void);
void flyOutoffDoor(void);
void dockGetTemp(void);
void dockGetData(void);
uint8_t getFlyCondition(void);
uint8_t isChargeComplete(void);
uint8_t isDoorOpenComplete(void);

#endif
