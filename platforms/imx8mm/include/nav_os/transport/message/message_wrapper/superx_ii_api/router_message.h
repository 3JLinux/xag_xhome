#pragma once
#include <stdio.h>
#include <stdint.h>
#define F8_HEADER_V1 16
#define VER_CMD(ver, cmd) (((uint8_t)ver << 6)|((uint8_t)cmd & 0x3f ))
#define DRAW_CMD(ver_cmd) (ver_cmd & 0x3f)  
#define DRAW_VER(ver_cmd) (ver_cmd >> 6) 
typedef struct {
  	uint8_t prefix;
    uint8_t ver_cmd;
    uint8_t request_id;
    uint8_t respond_id;  
	uint32_t from_node;
	uint32_t to_node;
    uint16_t length;
    uint16_t reserve;
    uint8_t * payload;           
} Net_Message_t;

