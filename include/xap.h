#ifndef XAP_H
#define XAP_H

#include <stdint.h>

#define XAP_PREFIX	0xC4C4c4c4

#define HEAD_XENT       0x0001
#define HEAD_XPACK      0x0002
#define HEAD_XSAFE      0x0003

#define HEAD_ROUTER     0x1001
#define HEAD_TPS        0x1002
#define HEAD_RC         0x1003
#define HEAD_NAV        0x1004
#define HEAD_DLS        0x1005

#define HEAD_SPRAY      0x2001
#define HEAD_FLYWAP     0x2002
#define HEAD_MS         0x2003
#define HEAD_FPV        0x2004
#define HEAD_BMS        0x2005
#define HEAD_WIFI       0x2006
#define HEAD_CANERA     0x2007

#define XAP_META_MESSAGE    0x0019

typedef struct 
{
    uint16_t head;
    uint16_t command;
    uint8_t data[];
}Content;


typedef struct 
{
    uint32_t prefix;
    uint16_t meta;
    uint16_t sequence;
    uint16_t length;
    uint16_t checksum;
    //Content content;
    uint8_t content[];
}XAP_FRAME_T;


typedef struct 
{
    uint32_t prefix;
    uint16_t meta;
    uint16_t sequence;
    uint16_t length;
    uint16_t checksum;
    uint64_t timestamp;
    uint32_t timeout;
    Content content;
}XAP_FRAME_EXTENDSION_T;


typedef struct 
{
    uint32_t prefix;
    uint16_t meta;
    uint16_t sequence;
    uint16_t length;
    uint16_t checksum;
    uint8_t type;
    uint8_t encoding;
    uint16_t protocol;
    Content content;
}XAP_FRAME_SOCKET_T;


/***********************/
typedef struct 
{
    uint8_t net_type;
    uint8_t rev[3];
    uint32_t ip;
    uint16_t port;
    uint16_t rev1;
}TPS_CHANGE_NETWORK_T;


#endif
