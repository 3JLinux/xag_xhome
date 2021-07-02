#ifndef DOCKHANDLE_H
#define DOCKHANDLE_H

#include <stdint.h>

void docking_mission_load(void);
void docking_rcv_handle(uint8_t* data, uint32_t dataLen);
void dockhandle_thread_create(void);
void dockhandle_thread_wait(void);

#endif
