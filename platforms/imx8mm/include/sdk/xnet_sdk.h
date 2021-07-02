//  Copyright 2021 wenweijie. Sensing & Controling Dept.
#pragma once

#include <functional>
#include <iostream>
#include <thread>
#include <list>

#include "node_addr.h"
#include "spdlog/spdlog.h"
#include "xap_frame.h"
#include "timer.h"
#include "xinotify.h"
#include "timerfd.h"
#include "cloud_log.h"

// #define SDK_DEBUG

namespace nav_os {
namespace sdk {

static NodeAddr IotAddr;

using nav_os::xnet::SdkErrCode;
using nav_os::xnet::XapFrame;
using nav_os::xnet::Xnet_NodeRegisterRequest_t;
using nav_os::xnet::Xnet_Node_t;

typedef struct {
  NodeAddr source;
  XapFrame data;
} XapPack_t;

class XnetSdk {
  friend class Xinotify;
  friend class TimerFd;
  friend class NodeAddr;
  friend class CloudLog;

 public:
  XnetSdk(const char *moduleName, uint16_t udpPort, std::string xnetIp = "");
  ~XnetSdk();
  XnetSdk() = delete;
  XnetSdk(XnetSdk &other) = delete;
  XnetSdk &operator=(XnetSdk &other) = delete;

  void setRouterNode(uint16_t type, uint16_t index = 0);
  // 片外应用设置版本注册信息
  void setVeisionInfo(std::string pkgName, std::string appName, int appType,
                      int versionCode, std::string versionName);
  // IOT消息专用回调
  void setIotCallBack(std::function<void(const uint8_t *, int)> &&func);
  // Xnet网络节点消息回调
  void setNodeCallBack(
      std::function<void(const uint8_t *, int, const NodeAddr &)> &&func);
  void setXnetRestartCallBack(std::function<void()> &&func);

  bool init();
  int transmit(const uint8_t *data, int len, const NodeAddr &dest = IotAddr);
  int transmit(XapFrame &, const NodeAddr &dest = IotAddr);
  int registerHardware(const uint8_t *data, int len);

  bool isRegistered() { return is_registered_.load(); }
  const Xnet_NodeRegisterRequest_t *self_info() { return &node_info_; }
  // 保存上一次的错误信息
  SdkErrCode ecode() const { return ecode_; }

 private:
  bool createSocket();
  int registerToXnet();
  int sendHeartbreat();
  int requestNodes();
  bool isLocalCommunication();
  void timerLoop();

  void listen();
  int epollCreate();
  bool epollAdd(int ep_fd, int fd);
  void epollLoop(int ep_fd);

  bool inotifyEpollAdd(int ep_fd);
  bool timerEpollAdd(int ep_fd);

  void iotMessageHandler(const uint8_t *, int);
  void nodeMessageHandler(const uint8_t *, int, in_addr_t, in_port_t);
  void xnetMessageHandler(const uint8_t *, int, in_addr_t, in_port_t);

  void xnetRegisterReplyHandler(XapFrame &);
  void xnetSysReplyHandler(XapFrame &);
  void xnetNodeReplyHandler(XapFrame &);
  void xnetCommandHandler(const uint8_t *, int, const NodeAddr &);

  void onRegistered();
  void onXnetRestart();

 private:
  std::atomic<bool> is_registered_;

  std::string xnet_ip_;
  Xnet_NodeRegisterRequest_t node_info_;

  int socket_fd_;
  struct sockaddr_in xnet_addr_;
  uint8_t *buffer_;

  std::atomic<bool> is_termiated_;
  std::thread listen_thd_;
  std::function<void(const uint8_t *, int)> iot_callback_;
  std::function<void(const uint8_t *, int, const NodeAddr &)> node_calback_;
  std::function<void(void)> restart_callback_;

  SdkErrCode ecode_;

  // 计时器
  Timer link_timer_;
  // 文件监测
  int inotify_fd_;
  Xinotify xinotify_;
  // 定时器
  int timerfd_fd_;
  TimerFd timerfd_;

  std::list<Xnet_Node_t> nodes_;
};
}  // namespace sdk
}  // namespace nav_os
