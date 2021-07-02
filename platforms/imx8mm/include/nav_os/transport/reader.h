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

#ifndef CYBER_NODE_READER_H_
#define CYBER_NODE_READER_H_



#include <algorithm>
#include <list>
#include <mutex>
#include <utility>

#include "transport/reader_base.h"
#include "common/global_data.h"
#include "common/proto/topology_change.pb.h"
#include "transport/topology_manager/topology_manager.h"
#include "cyber/time/time.h"
#include "transport/transport.h"

namespace xag_nav {
namespace os {

using proto::RoleType;
/**
 * @class Reader
 * @brief Reader subscribes a channel, it has a main functions:
 * * You can pass a `CallbackFunc` to handle the message then it arrived.
 * Reader will Join the topology when init and Leave the topology when shutdown
 */
template <typename MessageT>
class Reader : public ReaderBase {
 public:
  using ReceiverPtr = std::shared_ptr<transport::Receiver<MessageT>>;
  using ChangeConnection = typename transport::Manager::ChangeConnection;

  /**
   * Constructor a Reader object.
   * @param role_attr is a protobuf message RoleAttributes, which includes the
   * channel name and other info.
   * @param reader_func is the callback function, when the message is received.
   */
  explicit Reader(const proto::RoleAttributes& role_attr,
                  const CallbackFunc<MessageT>& reader_func = nullptr);
  virtual ~Reader();

  /**
   * @brief Init Reader
   *
   * @return true if init successfully
   * @return false if init failed
   */
  bool Init() override;

  /**
   * @brief Shutdown Reader
   */
  void Shutdown() override;

  /**
   * @brief Is there is at least one writer publish the channel that we
   * subscribes?
   *
   * @return true if the channel has writer
   * @return false if the channel has no writer
   */
  bool HasWriter() override;

  /**
   * @brief Get all writers pushlish the channel we subscribes
   *
   * @param writers result vector of RoleAttributes
   */
  void GetWriters(std::vector<proto::RoleAttributes>* writers) override;

 private:
  
  void JoinTheTopology();
  void LeaveTheTopology();
  void OnChannelChange(const proto::ChangeMsg& change_msg);

  CallbackFunc<MessageT> reader_func_;
  ReceiverPtr receiver_ = nullptr;

  // BlockerPtr blocker_ = nullptr;
  ChangeConnection change_conn_;
  transport::ChannelManagerPtr channel_manager_ = nullptr;
};

template <typename MessageT>
Reader<MessageT>::Reader(const proto::RoleAttributes& role_attr,
                         const CallbackFunc<MessageT>& reader_func)
    : ReaderBase(role_attr),
      reader_func_(reader_func) {
}

template <typename MessageT>
Reader<MessageT>::~Reader() {
  Shutdown();
}

template <typename MessageT>
bool Reader<MessageT>::Init() {
  if (init_.exchange(true)) {
    return true;
  }
  std::function<void(const std::shared_ptr<MessageT>&)> func;
  if (reader_func_ != nullptr) {
    func = [this](const std::shared_ptr<MessageT>& msg) {
      this->reader_func_(msg);
    };
  }
  receiver_ = ReceiverManager<MessageT>::Instance()->GetReceiver(role_attr_, func);
  if (!(receiver_)){
    init_.store(false);
    return false;
  }
  
  this->role_attr_.set_id(receiver_->id().HashValue()); 
  channel_manager_ =
      transport::TopologyManager::Instance()->channel_manager();
  JoinTheTopology();
  return true;
}

template <typename MessageT>
void Reader<MessageT>::Shutdown() {
  if (!init_.exchange(false)) {
    return;
  }
  LeaveTheTopology();
  receiver_ = nullptr;
  channel_manager_ = nullptr;
}

template <typename MessageT>
void Reader<MessageT>::JoinTheTopology() {
  // add listener
  change_conn_ = channel_manager_->AddChangeListener(std::bind(
      &Reader<MessageT>::OnChannelChange, this, std::placeholders::_1));

  // get peer writers
  const std::string& channel_name = this->role_attr_.channel_name();
  std::vector<RoleAttributes> writers;
  channel_manager_->GetWritersOfChannel(channel_name, &writers);
  for (auto& writer : writers) {
    receiver_->Enable(writer);
  }
  channel_manager_->Join(this->role_attr_, proto::RoleType::ROLE_READER);
}

template <typename MessageT>
void Reader<MessageT>::LeaveTheTopology() {
  channel_manager_->RemoveChangeListener(change_conn_);
  channel_manager_->Leave(this->role_attr_, proto::RoleType::ROLE_READER);
}

template <typename MessageT>
void Reader<MessageT>::OnChannelChange(const proto::ChangeMsg& change_msg) {
  if (change_msg.role_type() != proto::RoleType::ROLE_WRITER) {
    return;
  }

  auto& writer_attr = change_msg.role_attr();
  if (writer_attr.channel_name() != this->role_attr_.channel_name()) {
    return;
  }

  auto operate_type = change_msg.operate_type();
  if (operate_type == proto::OperateType::OPT_JOIN) {
    receiver_->Enable(writer_attr);
  } else {
    receiver_->Disable(writer_attr);
  }
}

template <typename MessageT>
bool Reader<MessageT>::HasWriter() {
  if (!init_.load()) {
    return false;
  }
  return channel_manager_->HasWriter(role_attr_.channel_name());
}

template <typename MessageT>
void Reader<MessageT>::GetWriters(std::vector<RoleAttributes>* writers) {
  if (writers == nullptr) {
    return;
  }
  if (!init_.load()) {
    return;
  }
  channel_manager_->GetWritersOfChannel(role_attr_.channel_name(), writers);
}

}  // namespace cyber
}  // namespace apollo

#endif  // CYBER_NODE_READER_H_
