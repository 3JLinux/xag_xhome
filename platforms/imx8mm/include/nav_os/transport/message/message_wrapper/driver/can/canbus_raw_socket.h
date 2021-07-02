#pragma once
#include "spdlog/spdlog.h"
#include "radar/can_com.h"
#include "radar/can_configure.h"
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <cstring>
#include <linux/net.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <unistd.h> 
#include <fcntl.h>
#include <memory>
#include <ctime>

namespace xag_nav{
namespace canbus{

class SocketCan {
 public:  
  typedef std::shared_ptr<SocketCan> ptr;
  SocketCan(const RADAR_CAN_ID& radar_can_id) : radar_can_id_(radar_can_id) {
  }
  ~SocketCan(){ 
    if(is_started_){
      stop();
    }
  }

  ErrorCode start();
  ErrorCode stop();
  int write(uint8_t *buffer, const int nbytes);
  int read( uint8_t *buffer, const int nbytes);
  const RADAR_CAN_ID get_radar_can_id() const { 
    return radar_can_id_;
  }
 private:

  bool is_started_ = false;
  int can_socket_fd_ = 0;
  const RADAR_CAN_ID radar_can_id_;
};

} // namespace canbus
} // namespace xag_nav
