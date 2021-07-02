//  Copyright 2021 wenweijie Sensing & Controling Dept.
#pragma once

#include <arpa/inet.h>
#include <string>

namespace nav_os {
namespace sdk {

class XnetSdk;

class NodeAddr {
 public:
  NodeAddr();
  NodeAddr(uint32_t netIp, uint16_t netPort);
  NodeAddr(const std::string &hostIp, uint16_t hostPort);
  NodeAddr(XnetSdk *xnet_sdk, const std::string &targetNode);
  ~NodeAddr();

  uint32_t ip() const;
  uint16_t port() const;
  const std::string &ip_str() const;
  const std::string &name() const;
  const sockaddr *sockAddr() const;
  int addrSize() const;

 private:
  void fillinSockAddr();

 private:
  XnetSdk *master_;
  uint32_t net_ip_; // 网络字节序
  uint16_t net_port_;
  std::string ip_str_;
  std::string name_;
  sockaddr_in sock_addr_;
};
}  // namespace sdk
}  // namespace nav_os
