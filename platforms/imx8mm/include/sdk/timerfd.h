//  Copyright 2021 wenweijie. Sensing & Controling Dept.
#pragma once

#include <sys/timerfd.h>

namespace nav_os {
namespace sdk {

class XnetSdk;

class TimerFd {
 public:
  TimerFd(int counter, XnetSdk *xnet_sdk);
  TimerFd() = delete;
  ~TimerFd();

  int init();
  void timeoutTick();

 private:
  int timer_fd_;
  int ms_;
  XnetSdk *master_;
};
}  // namespace sdk
}  // namespace nav_os
