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

#ifndef CYBER_TRANSPORT_TRANSMITTER_SHM_TRANSMITTER_H_
#define CYBER_TRANSPORT_TRANSMITTER_SHM_TRANSMITTER_H_

#include <cstring>
#include <iostream>
#include <memory>
#include <string>

#include "common/global_data.h"
#include "spdlog/spdlog.h"

#include "transport/shm/notifier/notifier_factory.h"
#include "transport/shm/notifier/readable_info.h"
#include "transport/shm/segment_factory.h"
#include "transport/shm/notifier/notifier_factory.h"
#include "transport/transmitter/transmitter.h"
#include "transport/shm/segment.h"
#include "common/macros.h"
#include "transport/message/message_wrapper/message_traits.h"
namespace xag_nav {
namespace os {
namespace transport{

template <typename MessageT>
class ShmTransmitter : public Transmitter<MessageT> {
 public:
  using MessagePtr = std::shared_ptr<MessageT>;

  explicit ShmTransmitter(const RoleAttributes& attr);
  virtual ~ShmTransmitter();

  void Enable() override;
  void Disable() override;

  bool Transmit(const MessagePtr& msg, const MessageInfo& msg_info) override;
 private:
  bool Transmit(const MessageT& msg, const MessageInfo& msg_info);

  SegmentPtr segment_;
  uint64_t channel_id_;
  NotifierPtr notifier_;
};

template <typename MessageT>
ShmTransmitter<MessageT>::ShmTransmitter(const RoleAttributes& attr)
    : Transmitter<MessageT>(attr),
      segment_(nullptr),
      channel_id_(attr.channel_id()),
      notifier_(nullptr) {}

template <typename MessageT>
ShmTransmitter<MessageT>::~ShmTransmitter() {
  Disable();
}

template <typename MessageT>
void ShmTransmitter<MessageT>::Enable() {
  // SPDLOG_INFO("ShmTransmitter<MessageT>::Enable, self_id = {}", channel_id_);
  if (this->is_enabled_) {
    //SPDLOG_INFO("ShmTransmitter is enabled_");
    return;
  }
  segment_ = SegmentFactory::CreateSegment(channel_id_);
  notifier_ = NotifierFactory::CreateNotifier();
  this->is_enabled_ = true;
}

template <typename MessageT>
void ShmTransmitter<MessageT>::Disable() {
  if (this->is_enabled_) {
    segment_ = nullptr;
    notifier_ = nullptr;
    this->is_enabled_ = false;
  }
}

template <typename MessageT>
bool ShmTransmitter<MessageT>::Transmit(const MessagePtr& msg,
                                 const MessageInfo& msg_info) {
  return Transmit(*msg, msg_info);
}

/** 
  * @brief 检查消息类型，
  * @param[in] msg: 确认消息类型为int, 能序列化，message_info能否序列化
  * @param[in] MessageInfo: 关于消息的基本信息
  * @return 
  *
  */

template <typename MessageT>
bool ShmTransmitter<MessageT>::Transmit(const MessageT& msg, const MessageInfo& msg_info) {
  // SPDLOG_INFO("ready to DEBUG ShmTransmitter<MessageT>::Transmit ");
  if (!this->is_enabled_) {
    SPDLOG_DEBUG("not enable.");
    return false;
  }
  // SPDLOG_INFO("DEBUG ShmTransmitter<MessageT>::Transmit msg_info.sender_id = {}", msg_info.sender_id().ToString());
  WritableBlock wb;
  std::size_t msg_size = message::ByteSize(msg);
  if ( msg_size == -1 ) { 
    SPDLOG_DEBUG("message::ByteSize(msg) return -1. also means it can't be transport in shm ");
    return false;
  }
  // SPDLOG_INFO("before AcquireBlockToWrite(msg_size, &wb)");

  if (!segment_->AcquireBlockToWrite(msg_size, &wb)) {
    SPDLOG_ERROR("acquire block failed.");
    return false;
  }
  // SPDLOG_INFO("block index:{} ",wb.index);
  if (!message::SerializeToArray(msg, wb.buf, static_cast<int>(msg_size))) {
    SPDLOG_ERROR("serialize to array failed.");
    segment_->ReleaseWrittenBlock(wb);
    return false;
  }
  wb.block->set_msg_size(msg_size);

  char* msg_info_addr = reinterpret_cast<char*>(wb.buf) + msg_size;
  if (!msg_info.SerializeTo(msg_info_addr, MessageInfo::kSize)) {
    SPDLOG_ERROR("serialize message info failed.");
    segment_->ReleaseWrittenBlock(wb);
    return false;
  }
  wb.block->set_msg_info_size(MessageInfo::kSize);
  segment_->ReleaseWrittenBlock(wb);

  ReadableInfo readable_info(wb.index, channel_id_);

  // SPDLOG_INFO("Writing sharedmem message: {} to block: {:d}", 
  //               common::GlobalData::GetChannelById(channel_id_), wb.index);
  return notifier_->Notify(readable_info);
}

}
}
}

#endif  // CYBER_TRANSPORT_TRANSMITTER_SHM_TRANSMITTER_H_
