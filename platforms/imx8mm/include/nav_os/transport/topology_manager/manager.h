/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef CYBER_SERVICE_DISCOVERY_SPECIFIC_MANAGER_MANAGER_H_
#define CYBER_SERVICE_DISCOVERY_SPECIFIC_MANAGER_MANAGER_H_

#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <memory>

#include "cyber/base/signal.h"
#include "common/proto/topology_change.pb.h"
#include "common/proto/role_attributes.pb.h"

namespace xag_nav {
namespace os {
namespace transport{
using apollo::cyber::base::Signal;
using apollo::cyber::base::Connection;
using proto::ChangeMsg;
using proto::OperateType;
using proto::RoleAttributes;
using proto::RoleType;
// using ChangeConnection = Connection<const ChangeMsg&>;

/**
 * @class Manager
 * @brief Base class for management of Topology elements.
 * Manager can Join/Leave the Topology, and Listen the topology change
 */
class Manager {
 public:
  // ChangeMsg 响应拓扑变化的回调函数的参数
  using ChangeSignal = Signal<const ChangeMsg&>;
  // 响应拓扑变化的回调
  using ChangeFunc = std::function<void(const ChangeMsg&)>;
  // 信号槽连接器
  using ChangeConnection = Connection<const ChangeMsg&>;

  /**
   * @brief Construct a new Manager object
   */
  Manager();

  /**
   * @brief Destroy the Manager object
   */
  virtual ~Manager();

  /**
   * @brief Shutdown module
   */
  virtual void Shutdown();

  /**
   * @brief Join the topology
   *
   * @param attr is the attributes that will be sent to other Manager(include
   * ourselves)
   * @param role is one of RoleType enum. writer/reader
   * @return true if Join topology successfully
   * @return false if Join topology failed
   */
  bool Join(const RoleAttributes& attr, RoleType role);
  /**
   * @brief Leave the topology
   *
   * @param attr is the attributes that will be sent to other Manager(include
   * ourselves)
   * @param role if one of RoleType enum . writer/reader
   * @return true if Leave topology successfully
   * @return false if Leave topology failed
   */
  bool Leave(const RoleAttributes& attr, RoleType role);

  /**
   * @brief Add topology change listener, when topology changed, func will be
   * called.
   *
   * @param func the callback function
   * @return ChangeConnection Store it to use when you want to stop listening.
   */
  ChangeConnection AddChangeListener(const ChangeFunc& func);

  /**
   * @brief Remove our listener for topology change.
   *
   * @param conn is the return value of `AddChangeListener`
   */
  void RemoveChangeListener(const ChangeConnection& conn);

  /**
   * @brief Called when a process' topology manager instance leave
   * @param process_id is the process' id
   */
  virtual void OnTopoModuleLeave(int process_id) = 0;

 protected:
  virtual bool Check(const RoleAttributes& attr) = 0;
  virtual void Dispose(const ChangeMsg& msg) = 0;
  // 将ChangeMsg* msg 信息填充好
  void Convert(const RoleAttributes& attr, RoleType role, OperateType opt,
                ChangeMsg* msg);
  // 调用signal()
  void Notify(const ChangeMsg& msg);
  // TODO
  void OnRemoteChange(const std::string& msg_str);
  bool IsFromSameProcess(const ChangeMsg& msg);
  std::atomic<bool> is_shutdown_;//
  int process_id_; //
  std::mutex lock_;//
  ChangeSignal signal_;
};

}  // namespace service_discovery
}  // namespace cyber
}  // namespace apollo

#endif  // CYBER_SERVICE_DISCOVERY_SPECIFIC_MANAGER_MANAGER_H_
