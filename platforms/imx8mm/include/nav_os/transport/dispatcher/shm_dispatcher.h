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

#ifndef CYBER_TRANSPORT_DISPATCHER_SHM_DISPATCHER_H_
#define CYBER_TRANSPORT_DISPATCHER_SHM_DISPATCHER_H_

#include <cstring>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>

#include "cyber/base/atomic_rw_lock.h"
#include "common/global_data.h"
#include "spdlog/spdlog.h"
#include "common/macros.h"
#include "transport/dispatcher/dispatcher.h"
#include "transport/shm/notifier/notifier_factory.h"
#include "transport/shm/segment_factory.h"
#include "transport/shm/segment.h"
#include "transport/message/message_wrapper/message_traits.h"
namespace xag_nav {
namespace os {
namespace transport{

class ShmDispatcher;
using ShmDispatcherPtr = ShmDispatcher*; 
using apollo::cyber::base::AtomicRWLock;
using apollo::cyber::base::ReadLockGuard;
using apollo::cyber::base::WriteLockGuard;

class ShmDispatcher : public Dispatcher {
 public:
  // key: channel_id
  using SegmentContainer = std::unordered_map<uint64_t, SegmentPtr>;

  virtual ~ShmDispatcher();

  void Shutdown() override;

  template <typename MessageT>
  void AddListener(const RoleAttributes& self_attr,
                   const MessageListener<MessageT>& listener);

  template <typename MessageT>
  void AddListener(const RoleAttributes& self_attr,
                   const RoleAttributes& opposite_attr,
                   const MessageListener<MessageT>& listener);

 private:
  void AddSegment(const RoleAttributes& self_attr);
  void ReadMessage(uint64_t channel_id, uint32_t block_index);
  void OnMessage(uint64_t channel_id, const std::shared_ptr<ReadableBlock>& rb,
                 const MessageInfo& msg_info);
  void ThreadFunc();
  void Init();

  SegmentContainer segments_;
  // key: channel_id ; value: block_index
  std::unordered_map<uint64_t, uint32_t> previous_indexes_;
  AtomicRWLock segments_lock_;
  std::thread thread_;
  NotifierPtr notifier_;

  DECLARE_SINGLETON(ShmDispatcher)
};

template <typename MessageT>
void ShmDispatcher::AddListener(const RoleAttributes& self_attr,
                                const MessageListener<MessageT>& listener) {
  // FIXME: make it more clean
  auto listener_adapter = [listener](const std::shared_ptr<ReadableBlock>& rb,
                                     const MessageInfo& msg_info) {
    auto msg = std::make_shared<MessageT>();
    // SPDLOG_INFO("DEBUG SHM CB READY TO BE CALL");
    RETURN_IF(!message::ParseFromArray(
        rb->buf, static_cast<int>(rb->block->msg_size()), msg.get()));
    listener(msg, msg_info);
  };
  Dispatcher::AddListener<ReadableBlock>(self_attr, listener_adapter);
  AddSegment(self_attr);
}
// 没用到，没有注册机制
template <typename MessageT>
void ShmDispatcher::AddListener(const RoleAttributes& self_attr,
                                const RoleAttributes& opposite_attr,
                                const MessageListener<MessageT>& listener) {
  // FIXME: make it more clean
  auto listener_adapter = [listener](const std::shared_ptr<ReadableBlock>& rb,
                                     const MessageInfo& msg_info) {
    auto msg = std::make_shared<MessageT>();
    // 反序列化
    RETURN_IF(!message::ParseFromArray(
        rb->buf, static_cast<int>(rb->block->msg_size()), msg.get()));
    listener(msg, msg_info);
  };

  Dispatcher::AddListener<ReadableBlock>(self_attr, opposite_attr,
                                         listener_adapter);
  AddSegment(self_attr);
}

}
}
}

#endif  // CYBER_TRANSPORT_DISPATCHER_SHM_DISPATCHER_H_
