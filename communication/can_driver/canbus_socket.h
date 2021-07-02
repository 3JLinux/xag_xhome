#ifndef _CANBUS_SOCKET_H
#define _CANBUS_SOCKET_H
//#pragma once

#include "can_com.h"
#include <sys/socket.h>
#include <cstring>
#include <net/if.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <ctime>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <iostream>
#include <cstdio>
#include <tr1/memory>
#include <sys/time.h>

using namespace std;

namespace xag_can{
namespace canbus{

class SocketCan{
public:
	typedef std::shared_ptr<SocketCan> Ptr;
	SocketCan(){}
	~SocketCan()
	{
		if (ifcanstart_)
		{
			stop();
		}
	}
	
ErrorCode start();
ErrorCode stop();
int write(uint8_t *buffer,uint32_t id);
int read(uint8_t *buffer);
//int read_fd(){return can_socket_fd_;};

private:

int can_socket_fd_ = 0;
bool ifcanstart_ = false;
SAB_CAN_ID sab_can_id_;

};

}
}
#endif