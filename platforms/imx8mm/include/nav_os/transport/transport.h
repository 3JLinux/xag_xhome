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

#ifndef CYBER_TRANSPORT_TRANSPORT_H_
#define CYBER_TRANSPORT_TRANSPORT_H_

#include <atomic>
#include <memory>
#include <string>
#include <iterator>
#include "transport/message/relation.h"
#include "common/proto/transport_conf.pb.h"
#include "transport/dispatcher/intra_dispatcher.h"
#include "transport/dispatcher/shm_dispatcher.h"
#include "transport/receiver/hybrid_receiver.h"
#include "transport/receiver/intra_receiver.h"
#include "transport/receiver/receiver.h"
#include "transport/receiver/shm_receiver.h"
#include "transport/shm/notifier/notifier_factory.h"
#include "transport/transmitter/hybrid_transmitter.h"
#include "transport/transmitter/intra_transmitter.h"
#include "transport/transmitter/shm_transmitter.h"
#include "transport/transmitter/transmitter.h"

namespace xag_nav {
namespace os {
namespace transport{

  
using proto::OptionalMode;
using proto::RoleAttributes;
using proto::OptionalMode;
// 提供工厂方法建立Transmitter, Receiver

class Transport {
 public:
  virtual ~Transport();

  void Shutdown();

  template <typename M>
  auto CreateTransmitter(const proto::RoleAttributes& attr,
                         const OptionalMode& mode = OptionalMode::HYBRID) ->
      typename std::shared_ptr<Transmitter<M>>;

  template <typename M>
  auto CreateReceiver(const proto::RoleAttributes& attr,
                      const typename Receiver<M>::MessageListener& msg_listener,
                      const OptionalMode& mode = OptionalMode::HYBRID) ->
      typename std::shared_ptr<Receiver<M>>;
 private:
  std::atomic<bool> is_shutdown_ = {false};
  NotifierPtr notifier_ = nullptr;
  IntraDispatcherPtr intra_dispatcher_ = nullptr;
  ShmDispatcherPtr shm_dispatcher_ = nullptr;

  DECLARE_SINGLETON(Transport)
};

/* 
* @brief 
* @param[in] 
* @param[in] 
* @return 
*/
template <typename M>
auto Transport::CreateTransmitter(const proto::RoleAttributes& attr,
                                  const OptionalMode& mode) ->
    typename std::shared_ptr<Transmitter<M>> {
  if (is_shutdown_.load()) {
    //SPDLOG_INFO("transport has been shutdown.");
    return nullptr;
  }
  std::shared_ptr<Transmitter<M>> transmitter = nullptr;
  proto::RoleAttributes modified_attr = attr;

  // TODO
  if (attr.has_channel_name()){
    const std::string& channel_name = attr.channel_name();
    if (channel_name.size() > 4 && channel_name[0] == 'S'
        && channel_name[1] == 'H' && channel_name[2] == 'M'
        && channel_name[3] == '.')
      {
      transmitter = std::make_shared<ShmTransmitter<M>>(modified_attr);
      } else {
      transmitter = std::make_shared<IntraTransmitter<M>>(modified_attr);
      }
  }
  RETURN_VAL_IF_NULL(transmitter, nullptr);
  transmitter->Enable();

  // switch (mode) {
  //   case OptionalMode::INTRA:
  //         transmitter = std::make_shared<IntraTransmitter<M>>(modified_attr);
  //     break;
  //   case OptionalMode::SHM:
  //     transmitter = std::make_shared<ShmTransmitter<M>>(modified_attr);
  //     break;
  //   default:
  //     transmitter =
  //         std::make_shared<HybridTransmitter<M>>(modified_attr);
  //     break;
  // }
  // RETURN_VAL_IF_NULL(transmitter, nullptr);
  // if (mode != OptionalMode::HYBRID) {
  //   transmitter->Enable();
  // }
  return transmitter;
}

template <typename M>
auto Transport::CreateReceiver(
  const proto::RoleAttributes& attr,
  const typename Receiver<M>::MessageListener& msg_listener,
        const OptionalMode& mode) -> typename std::shared_ptr<Receiver<M>> {
    if (is_shutdown_.load()) {
      //SPDLOG_INFO("transport has been shut down.");
      return nullptr;
    }
  std::shared_ptr<Receiver<M>> receiver = nullptr;
  proto::RoleAttributes modified_attr = attr;

  //TODO
  if (attr.has_channel_name()){
    const std::string& channel_name = attr.channel_name();
    if (channel_name.size() > 4 && channel_name[0] == 'S'
        && channel_name[1] == 'H' && channel_name[2] == 'M'
        && channel_name[3] == '.')
      {
        receiver = std::make_shared<ShmReceiver<M>>(modified_attr, msg_listener);
      } else {
        receiver = std::make_shared<IntraReceiver<M>>(modified_attr, msg_listener);
    }
  }  
  RETURN_VAL_IF_NULL(receiver, nullptr);
  receiver->Enable();
  
  // switch (mode) {
  //   case OptionalMode::INTRA:
  //     receiver = std::make_shared<IntraReceiver<M>>(modified_attr, msg_listener);
  //     break;
  //   case OptionalMode::SHM:
  //     receiver = std::make_shared<ShmReceiver<M>>(modified_attr, msg_listener);
  //     break;
  //   default:
  //     receiver = std::make_shared<HybridReceiver<M>>(modified_attr, msg_listener);
  //     break;
  // }
  // RETURN_VAL_IF_NULL(receiver, nullptr);
  // warn:
  // if (mode != OptionalMode::HYBRID && mode != OptionalMode::INTRA) {
  //   receiver->Enable();
  // }
  return receiver;
}

}  // namespace transport
}  // namespace os
}  // namespace xag_nav

#endif  // CYBER_TRANSPORT_TRANSPORT_H_
