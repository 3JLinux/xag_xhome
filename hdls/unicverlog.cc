#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "hdls.h"
#include "unicverlog.h"
#include "dockCanHandle.h"
#include "xhome.h"

#define PREFIX          0x4c4c
#define UNICVERLOG_VERSION 1
#define CONTENT_TYPE    1
#define SENDER_LENGTH   25
#define RULE_TYPE       312
#define RULE_VERSION    0

// static char id[] = "A26002D000951383532383232";
static char id[] = "A380043000F51303138393138";
pthread_t unicverlog_thread_flag;


/************************************************************
 * 函数原型：	int32_t unicverlog_body_fillin(void* msgBuf)
 * 功能：		云日志报文数据填充	
 * 输入：		void* msgbuf 缓冲区指针
 * 输出：		int32_t 填充后数据长度
 * *********************************************************/
int32_t unicverlog_body_fillin(void* msgBuf)
{
    static uint16_t sync_ID = 0;
    char buf[1024] = {0};
    UNICVERLOG_HEAD* message = (UNICVERLOG_HEAD*)buf;
    struct timeval tv;
    gettimeofday(&tv, NULL);

    message->prefix = PREFIX;
    message->version = UNICVERLOG_VERSION;
    message->sync_id = sync_ID++;
    message->timestamp = tv.tv_sec*1000+tv.tv_usec/1000;
    message->content_type = CONTENT_TYPE;
    message->sender_length = SENDER_LENGTH;
    memcpy(message->sender, id, SENDER_LENGTH);
    message->rule_type = RULE_TYPE;
    message->rule_version = RULE_VERSION;
    message->content_length = sizeof(UNICVERLOG_BODY);

    UNICVERLOG_BODY body;
    dockGetTemp();
    body.SABInTemperature = dockTemp.SABInTemperature;
    body.SABInHumidity = dockTemp.SABInHumidity;
    body.SABCabinTemperature = dockTemp.SABCabinTemperature;
    body.SABEBoxTemperature = dockTemp.SABEBoxTemperature;
    body.SABOutletTemperature = dockTemp.SABOutletTemperature;
    body.SABAirSpeed = sensorData.wind_speed;
    body.SABSolarIntensity = sensorData.illumination;
    body.SABRainfall = sensorData.rainfall;


    // body.SABInTemperature = 23;					
    // body.SABInHumidity = 50;					
    // body.SABCabinTemperature = 30;				
    // body.SABEBoxTemperature = 50;					
    // body.SABOutletTemperature = 41;				
    // body.SABAirSpeed = 1;						
    // body.SABSolarIntensity = 5000;				
    // body.SABRainfall = 1;						
    body.Longitude = 1134220740; 							
    body.Latitude = 231825820;						
    body.Altitude = 10000000;							
    body.SignalStrength = -60; 					

    // BASICDEV_STA basic_sta = REGULAR;
    dockGetData();
    body.SABChargeCmuState = dockCanData.SABChargeCmuState;
    body.SABCellVoltage = dockCanData.SABCellVoltage;
    body.SABRealCapacity = dockCanData.SABRealCapacity;
    body.SABDesignCapacity = dockCanData.SABDesignCapacity;
    body.SABSOC = dockCanData.SABSOC;
    body.SABYDN_WDT = dockCanData.SABYDN_WDT;
    body.SABSmokeAlarmState = dockCanData.SABSmokeAlarmState;
    body.SABFanSpeed = dockCanData.SABFanSpeed;
    body.SABFanSpeed = dockCanData.SABFanSpeed;
    body.LoadingTaskCompleted = dockCanData.LoadingTaskCompleted;
    body.SecurityCheckConfirmation = dockCanData.SecurityCheckConfirmation;
    body.SABLefDoorState = dockCanData.SABLefDoorState;
    body.SABRightDoorState = dockCanData.SABRightDoorState;
    body.SABBattCmuState = dockCanData.SABBattCmuState;
    body.SABChargeCmuState = dockCanData.SABChargeCmuState;
    body.SABRequestVol = dockCanData.SABRequestVol;
    body.SABRequestCur = dockCanData.SABRequestCur;
    // body.SABChargeCmuState = basic_sta; 					
    // body.SABCellVoltage = 5;					
    // body.SABRealCapacity = 750;					
    // body.SABDesignCapacity = 800;					
    // body.SABSOC = 909;							
    // body.SABYDN_WDT = basic_sta;					
    // body.SABSmokeAlarmState = basic_sta;				
    // body.SABFanState = basic_sta;					
    // body.SABFanSpeed = 500;					

    // body.LoadingTaskCompleted = 0;				
    // body.SecurityCheckConfirmation = 0;			

    // DOOR_STA door_sta = CLOSE;
    // body.SABLefDoorState = door_sta; 					
    // body.SABRightDoorState = door_sta; 					
    // body.SABBattCmuState = 1;					
    // body.SABChargeState = 1;						
    // body.SABRequestVol = 12;						
    // body.SABRequestCur = 3;						

    body.PlaneState = dockCanData.PlaneState;
    body.DockedState = dockCanData.DockedState;
    // PLANE_STA plane_sta = ALREADY_DOCK;
    // body.PlaneState = plane_sta;							
    // body.DockedState = 1;						

    body.SABDoorOverCurrent = dockCanData.SABDoorOverCurrent;
    body.SABLeftSeneorFault = dockCanData.SABLeftSeneorFault;
    body.SABRightSeneorFault = dockCanData.SABRightSeneorFault;
    body.SABOverTemperature = dockCanData.SABOverTemperature;
    body.SABUnderTemperature = dockCanData.SABUnderTemperature;
    body.SABSmokeDetected = dockCanData.SABSmokeDetected;
    body.SABLeakDetected = dockCanData.SABLeakDetected;
    body.SABLoseCharger = dockCanData.SABLoseCharger;
    // ERR_STA error_sta = NORMAL;
    // body.SABDoorOverCurrent = error_sta;					
    // body.SABLeftSeneorFault = error_sta;					
    // body.SABRightSeneorFault = error_sta;				
    // body.SABLeftOperationOvertime = error_sta;			
    // body.SABRightOperationOvertime = error_sta;			
    // body.SABOverTemperature = error_sta;				
    // body.SABUnderTemperature = error_sta;				
    // body.SABSmokeDetected = error_sta;					
    // body.SABLeakDetected = error_sta;					
    // body.SABLoseCharger = error_sta;						
    body.SABLostSensor = 0;
    memcpy(message->content, (char*)&body, sizeof(UNICVERLOG_BODY));
    memcpy(msgBuf, message, sizeof(UNICVERLOG_BODY) +sizeof(UNICVERLOG_HEAD));
    return sizeof(UNICVERLOG_BODY) +sizeof(UNICVERLOG_HEAD);
}


/************************************************************
 * 函数原型：	void unicverlog_sendto(void)
 * 功能：		云日志数据发送
 * 输入：		void	
 * 输出：		void
 * *********************************************************/
void unicverlog_sendto(void)
{
    char subject[128] = "UNIVERLOG";
    uint8_t buf[1024] = {0};
    uint32_t len = unicverlog_body_fillin(buf);
    hdls_send(subject, NULL, (char*)buf, len);
}


/************************************************************
 * 函数原型：	void* unicverlog_thread(void* ptr)
 * 功能：		云日志发送线程
 * 输入：		void* ptr				
 * 输出：		void*
 * *********************************************************/
void* unicverlog_thread(void* ptr)
{
    while(1)
    {
        unicverlog_sendto();
        sleep(10);
    }
}


/************************************************************
 * 函数原型：	void unicverlog_thread_create(void)
 * 功能：		云日志线程创建
 * 输入：		void				
 * 输出：		void
 * *********************************************************/
void unicverlog_thread_create(void)
{
    int temp;
    memset(&unicverlog_thread_flag, 0, sizeof(unicverlog_thread_flag));
    if((temp = pthread_create(&unicverlog_thread_flag, NULL, &unicverlog_thread, NULL)) != 0)
    {
        printf("unicverlog_thread_create ERROR!\n");
    }
    else
    {
        printf("unicverlog_thread_create SUCCESS!\n");
    }
}


/************************************************************
 * 函数原型：	void unicverlog_thread_wait(void)
 * 功能：		云日志线线程等待结束
 * 输入：		void
 * 输出：		void
 * *********************************************************/
void unicverlog_thread_wait(void)
{
    if(unicverlog_thread_flag != 0)
    {
        pthread_join(unicverlog_thread_flag, NULL);
        printf("unicverlog_thread END!\n");
    }
}
