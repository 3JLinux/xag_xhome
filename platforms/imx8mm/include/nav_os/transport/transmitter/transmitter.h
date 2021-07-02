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

#ifndef CYBER_TRANSPORT_TRANSMITTER_TRANSMITTER_H_
#define CYBER_TRANSPORT_TRANSMITTER_TRANSMITTER_H_

#include <cstdint>
#include <memory>
#include <string>
#include "transport/message/message_info.h"
#include "common/proto/role_attributes.pb.h"
#include "transport/message/identity.h"
namespace xag_nav {
namespace os {
namespace transport{

using proto::RoleAttributes;

template <typename M>
class Transmitter {
 public:
  using MessageTypePtr = std::shared_ptr<M>;

  explicit Transmitter(const RoleAttributes& attr);
  virtual ~Transmitter();

  virtual void Enable() = 0;
  virtual void Disable() = 0;

  virtual void Enable(const RoleAttributes& opposite_attr);
  virtual void Disable(const RoleAttributes& opposite_attr);

  virtual bool Transmit(const MessageTypePtr& msg);
  virtual bool Transmit(const MessageTypePtr& msg, const MessageInfo& msg_info) = 0;
  uint64_t NextSeqNum() { return ++seq_num_; }
  uint64_t seq_num() const { return seq_num_; }
  const Identity id() const { return id_; }
  const RoleAttributes role_attributes() const { return role_attr_; }
 protected:

  uint64_t seq_num_;
  Identity id_;
  MessageInfo msg_info_;
  RoleAttributes role_attr_;
  bool is_enabled_;
};

template <typename M>
Transmitter<M>::Transmitter(const RoleAttributes& attr)
    : role_attr_(attr), seq_num_(0) ,id_(),is_enabled_(false){
  msg_info_.set_sender_id(this->id_);
  msg_info_.set_seq_num(this->seq_num_);
  if (!role_attr_.has_process_id()){
    role_attr_.set_process_id(common::GlobalData::Instance()->ProcessId());
  }
  if(!role_attr_.has_id()){
    role_attr_.set_id(id_.HashValue());
  }
}
////// | //// 换种方式处理id
////// V ////
// template <typename M>
// Transmitter<M>::Transmitter(const RoleAttributes& attr)
//     : role_attr_(attr), seq_num_(0) ,id_(), is_enabled_(false){
//   if(role_attr_.has_id()){
//     id_.set_data(std::to_string(role_attr_.id()).c_str());
//   }
//   role_attr_.set_id(id_.HashValue());
//   msg_info_.set_sender_id(this->id_);
//   msg_info_.set_seq_num(this->seq_num_);
//   if (!role_attr_.has_process_id()){
//     role_attr_.set_process_id(common::GlobalData::Instance()->ProcessId());
//   }
  
// }


template <typename M>
Transmitter<M>::~Transmitter() {}

template <typename M>
bool Transmitter<M>::Transmit(const MessageTypePtr& msg) {
  msg_info_.set_seq_num(NextSeqNum());
  return Transmit(msg, msg_info_);
}

template <typename M>
void Transmitter<M>::Enable(const RoleAttributes& opposite_attr) {
  (void)opposite_attr;
  Enable();
}

template <typename M>
void Transmitter<M>::Disable(const RoleAttributes& opposite_attr) {
  (void)opposite_attr;
  Disable();
}

}  // namespace transport
}  // namespace os
}  // namespace xag_nav

#endif  // CYBER_TRANSPORT_TRANSMITTER_TRANSMITTER_H_
