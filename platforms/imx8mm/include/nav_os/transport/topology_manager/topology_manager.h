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

#ifndef __TOPOLOGY_MANAGER_TOPOLOGY_MANAGER_H_
#define __TOPOLOGY_MANAGER_TOPOLOGY_MANAGER_H_
#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>

#include "cyber/base/signal.h"
#include "common/macros.h"
#include "transport/topology_manager/channel_manager/channel_manager.h"
// #include "transport/topology_manager/node_manager.h"
namespace xag_nav {
namespace os {
namespace transport{

class ChannelManager;
using ChannelManagerPtr = std::shared_ptr<ChannelManager>;

/**
 * @class TopologyManager
 * @brief elements in Cyber -- Node, Channel, Writer, Reader's
 * relationship is presented by Topology. You can Imagine that a
 * directed graph -- Node is the container of Writer/Reader, and
 * they are the vertice of the graph and Channel is the Edge from Writer flow to
 * the Reader. Thus we call Writer `Upstream`, Reader  `Downstream` To generate this graph,
 * we use TopologyManager, it has two sub managers -- NodeManager: You can
 * find Nodes in this topology ChannelManager: You can find Channels in this
 * topology, and their Writers and Readers ServiceManager: You can find Services
 * in this topology. It can broadcast Join or Leave
 * messages of those elements. Also, you can register you own `ChangeFunc` to
 * monitor topology change
 */
class TopologyManager {
 public:
  using ChangeSignal = apollo::cyber::base::Signal<const ChangeMsg&>;
  using ChangeFunc = std::function<void(const ChangeMsg&)>;
  using ChangeConnection = apollo::cyber::base::Connection<const ChangeMsg&>;
  using ParticipateNameContainer = std::unordered_map<uint64_t, std::string>;
  virtual ~TopologyManager();

  /**
   * @brief Shutdown the TopologyManager
   */
  void Shutdown();

  /**
   * @brief To observe the topology change, you can register a `ChangeFunc`
   *
   * @param func is the observe function
   * @return ChangeConnection is the connection that connected to
   * `change_signal_`. Used to Remove your observe function
   */
  ChangeConnection AddChangeListener(const ChangeFunc& func);

  /**
   * @brief Remove the observe function connect to `change_signal_` by `conn`
   */
  void RemoveChangeListener(const ChangeConnection& conn);

  /**
   * @brief Get shared_ptr for ChannelManager
   */
  ChannelManagerPtr& channel_manager() { return channel_manager_; }
  // NodeManagerPtr& node_manager() { return node_manager_; }

 private:
  bool Init();
  bool InitChannelManager() {return true;}
  bool InitNodeManager() { return true; }
  bool CreateParticipant();
  bool ParseParticipantName(int* process_id);

  std::atomic<bool> init_;             /// Is TopologyManager inited
  ChannelManagerPtr channel_manager_;  /// shared ptr of ChannelManager
  // NodeManagerPtr node_manager_;        /// shared ptr of NodeManagerPtr

  // ParticipantPtr participant_;
  // ParticipantListener* participant_listener_;
  ChangeSignal change_signal_;           /// topology changing signal,
                                         ///< connect to `ChangeFunc`s
  ParticipateNameContainer participant_names_; // other participate in topology
  DECLARE_SINGLETON(TopologyManager)
};

}  // namespace service_discovery
}  // namespace cyber
}  // namespace apollo
#endif