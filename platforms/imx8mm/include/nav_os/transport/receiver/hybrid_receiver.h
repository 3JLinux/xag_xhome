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

#ifndef CYBER_TRANSPORT_RECEIVER_HYBRID_RECEIVER_H_
#define CYBER_TRANSPORT_RECEIVER_HYBRID_RECEIVER_H_

#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common/global_data.h"
#include "spdlog/spdlog.h"
#include "common/proto/role_attributes.pb.h"
#include "transport/container/role.h"
#include "cyber/time/time.h"
#include "transport/receiver/intra_receiver.h"
#include "transport/receiver/shm_receiver.h"
#include "common/proto/transport_conf.pb.h"
#include "common/proto/topology_change.pb.h"
#include "transport/message/relation.h"
namespace xag_nav {
namespace os {
namespace transport{

using proto::OptionalMode;

template <typename M>
class HybridReceiver : public Receiver<M> {
 public:
  using ReceiverPtr = std::shared_ptr<Receiver<M>>;
  using ReceiverContainer =
      std::unordered_map<OptionalMode, ReceiverPtr, std::hash<int>>;
  using TransmitterContainer =
      std::unordered_map<OptionalMode,
                         std::unordered_map<uint64_t, proto::RoleAttributes>,
                         std::hash<int>>;
  using CommunicationModePtr = std::shared_ptr<proto::CommunicationMode>;
  using MappingTable =
      std::unordered_map<Relation, OptionalMode, std::hash<int>>;
  HybridReceiver(const proto::RoleAttributes& attr,
                 const typename Receiver<M>::MessageListener& msg_listener);
  virtual ~HybridReceiver();

  void Enable() override;
  void Disable() override;

  void Enable(const proto::RoleAttributes& opposite_attr) override;
  void Disable(const proto::RoleAttributes& opposite_attr) override;

 private:
  void InitMode();
  void InitReceivers();
  void ClearReceivers();
  void InitTransmitters();
  void ClearTransmitters();
  void ThreadFunc(const RoleAttributes& opposite_attr);
  Relation GetRelation(const RoleAttributes& opposite_attr);

  ReceiverContainer receivers_;
  TransmitterContainer transmitters_;
  std::mutex mutex_;
  CommunicationModePtr mode_;
  MappingTable mapping_table_; 
  // ParticipantPtr participant_;
};

template <typename M>
HybridReceiver<M>::HybridReceiver(
    const proto::RoleAttributes& attr,
    const typename Receiver<M>::MessageListener& msg_listener)
    // const ParticipantPtr& participant)
    : Receiver<M>(attr, msg_listener) {
      // participant_(participant) {
  InitMode();
  InitReceivers();
  InitTransmitters();
}

template <typename M>
HybridReceiver<M>::~HybridReceiver() {
  ClearTransmitters();
  ClearReceivers();
}

template <typename M>
void HybridReceiver<M>::Enable() {
  std::lock_guard<std::mutex> lock(mutex_);
  // for (auto& item : receivers_) {
  //   item.second->Enable();
  // }
  receivers_[OptionalMode::SHM]->Enable();
}

template <typename M>
void HybridReceiver<M>::Disable() {
  std::lock_guard<std::mutex> lock(mutex_);
  for (auto& item : receivers_) {
    item.second->Disable();
  }
}

template <typename M>
void HybridReceiver<M>::Enable(const RoleAttributes& opposite_attr) {
  auto relation = GetRelation(opposite_attr);
  RETURN_IF(relation == NO_RELATION);

  uint64_t id = opposite_attr.id();
  std::lock_guard<std::mutex> lock(mutex_);
  if (transmitters_[mapping_table_[relation]].count(id) == 0) {
    transmitters_[mapping_table_[relation]].insert(
        std::make_pair(id, opposite_attr));
    receivers_[mapping_table_[relation]]->Enable(opposite_attr);
  }
}

template <typename M>
void HybridReceiver<M>::Disable(const RoleAttributes& opposite_attr) {
  auto relation = GetRelation(opposite_attr);
  RETURN_IF(relation == NO_RELATION);

  uint64_t id = opposite_attr.id();
  std::lock_guard<std::mutex> lock(mutex_);
  if (transmitters_[mapping_table_[relation]].count(id) > 0) {
    transmitters_[mapping_table_[relation]].erase(id);
    receivers_[mapping_table_[relation]]->Disable(opposite_attr);
  }
}

template <typename M>
void HybridReceiver<M>::InitMode() {
  mode_ = std::make_shared<proto::CommunicationMode>();
  mapping_table_[SAME_PROC] = mode_->same_proc();
  mapping_table_[DIFF_PROC] = mode_->diff_proc();
}

// template <typename M>
// void HybridReceiver<M>::ObtainConfig() {
//   auto& global_conf = common::GlobalData::Instance()->Config();
//   if (!global_conf.has_transport_conf()) {
//     return;
//   }
//   if (!global_conf.transport_conf().has_communication_mode()) {
//     return;
//   }
//   mode_->CopyFrom(global_conf.transport_conf().communication_mode());

//   mapping_table_[SAME_PROC] = mode_->same_proc();
//   mapping_table_[DIFF_PROC] = mode_->diff_proc();
// }

template <typename M>
void HybridReceiver<M>::InitReceivers() {
  std::set<OptionalMode> modes;
  modes.insert(mode_->same_proc());
  modes.insert(mode_->diff_proc());
  auto listener = std::bind(&HybridReceiver<M>::OnNewMessage, this,
                            std::placeholders::_1, std::placeholders::_2);
  for (auto& mode : modes) {
    switch (mode) {
      case OptionalMode::INTRA:
        receivers_[mode] =
            std::make_shared<IntraReceiver<M>>(this->role_attr_, listener);
        break;
      case OptionalMode::SHM:
        receivers_[mode] =
            std::make_shared<ShmReceiver<M>>(this->role_attr_, listener);
        break;
      default:
        break;
    }
  }
}

template <typename M>
void HybridReceiver<M>::ClearReceivers() {
  receivers_.clear();
}

template <typename M>
void HybridReceiver<M>::InitTransmitters() {
  std::unordered_map<uint64_t, RoleAttributes> empty;
  for (auto& item : receivers_) {
    transmitters_[item.first] = empty;
  }
}

template <typename M>
void HybridReceiver<M>::ClearTransmitters() {
  for (auto& item : transmitters_) {
    for (auto& upper_reach : item.second) {
      receivers_[item.first]->Disable(upper_reach.second);
    }
  }
  transmitters_.clear();
}

template <typename M>
void HybridReceiver<M>::ThreadFunc(const RoleAttributes& opposite_attr) {
  std::string channel_name =
      std::to_string(opposite_attr.id()) + std::to_string(this->role_attr_.id());
  uint64_t channel_id = common::GlobalData::RegisterChannel(channel_name);

  RoleAttributes attr(this->role_attr_);
  attr.set_channel_name(channel_name);
  attr.set_channel_id(channel_id);

  volatile bool is_msg_arrived = false;
  auto listener = [&](const std::shared_ptr<M>& msg,
                      const MessageInfo& msg_info, const RoleAttributes& attr) {
    is_msg_arrived = true;
    this->OnNewMessage(msg, msg_info);
  };
}

template <typename M>
Relation HybridReceiver<M>::GetRelation(const RoleAttributes& opposite_attr) {
  if (opposite_attr.channel_name() != this->role_attr_.channel_name()) {
    return NO_RELATION;
  }
  if (opposite_attr.process_id() != this->role_attr_.process_id()) {
    return DIFF_PROC;
  }

  return SAME_PROC;
}

}  // namespace transport
}  // namespace os
}  // namespace xag_nav

#endif  // CYBER_TRANSPORT_RECEIVER_HYBRID_RECEIVER_H_
