#ifndef XHOME_H
#define XHOME_H

#include <stdint.h>

// #define XHOME_CMD_STATUS    0x01
// #define XHOME_CMD_TITUDE    0x02   

#define XHOME_CMD_MISSION_LOAD              0x0001
#define XHOME_CMD_MISSION_LOAD_ACK          0x1001
#define XHOME_CMD_MISSION_LOAD_COMP         0x0002
#define XHOME_CMD_MISSION_LOAD_COMP_ACK     0x1002
#define XHOME_CMD_MISSION_START             0x0003
#define XHOME_CMD_MISSION_START_ACK         0x1003
// #define XHOME_CMD_MISSION_START_COMP        0x0004
// #define XHOME_CMD_MISSION_START_COMP_ACK    0x1004
#define XHOME_CMD_FLY                       0x0004
#define XHOME_CMD_FLY_ACK                   0x1004
#define XHOME_CMD_HOVER                     0x0005
#define XHOME_CMD_HOVER_ACK                 0x1005
#define XHOME_CMD_LANDING                   0x0006
#define XHOME_CMD_LANDING_ACK               0X1006

#define XHOME_HEAD          0x8001

// typedef enum XHOME_EVENT 
// {
//     FLY_READY = 0x01,
//     LANDING_READY = 0x02,

//     FLY_READY_ACK = 0x11,
//     LANDING_READY_ACK = 0x12
// };


typedef struct
{
    uint16_t head;
    uint16_t cmd;
    uint8_t sourceID[64];
    uint8_t targetID[64];
    uint8_t data[];
}XHOME_PROTO_T;


typedef struct 
{
    uint32_t version;
    uint16_t mission_type;
    uint16_t mission_source;
    uint8_t mission_id[16];
    uint32_t mission_timeout;

    uint32_t file_name_length;
    uint8_t file_name[128];

    uint32_t field_name_length;
    uint8_t field_name[128];
}XHOME_MISSION_T;


typedef struct 
{
    float windSpeedMax;
    float windSpeedMin;
    float temperatureMax;
    float temperatureMin;
    float illuminationMax;
    float illuminationMin;
}XHOME_THRESHOLD;


typedef struct 
{
    int32_t version;
    float temperture;
    float humidity;
    float pressure;
    float illumination;
    float rainfall;
    float wind_speed;
    float wind_direction;
    int32_t battery;
}SENSOR_DATA;


typedef struct 
{
    XHOME_MISSION_T mission;
}XHOME_MISSION_LOAD_T;


typedef struct 
{
    uint8_t mission_id[16];
    uint8_t mission_load_status;
}XHOME_MISSION_LOAD_STATUS_T;



typedef struct
{
    uint8_t mission_id[16];
    uint32_t mission_seq;
    uint16_t mission_type;
    uint16_t mission_source;
}XHOME_MISSION_START_T;


typedef struct
{
    uint32_t latitude;
    uint32_t longitude;
    uint32_t altitude;
}XHOME_LANDING_T;


void conditions_parse(const char *revbuf);
void sensor_rcv_handle(char* rcvBuf);
uint8_t conditions_cmp(void);

uint8_t getMissionLoadFlag(void);
void resetMissionLoadFlag(void);
int32_t xhome_fill_handle(uint8_t* buf, uint16_t cmd, uint8_t* sourceID, uint8_t* targetID);
uint32_t getMissionTimeout(void);

extern SENSOR_DATA sensorData;	

#endif
