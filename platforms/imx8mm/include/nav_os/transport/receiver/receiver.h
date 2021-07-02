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

#ifndef CYBER_TRANSPORT_RECEIVER_RECEIVER_H_
#define CYBER_TRANSPORT_RECEIVER_RECEIVER_H_

#include <functional>
#include <memory>
#include "transport/message/message_info.h"
#include "transport/message/identity.h"
#include "common/proto/role_attributes.pb.h"
#include "common/global_data.h"
namespace xag_nav {
namespace os {
namespace transport{
/* 
* @class  Receiver<M>
* @brief[in] Receiver作用在于将回调函数进行绑定(connenct)
*
*/
using proto::RoleAttributes;
using common::GlobalData;

template <typename M>
class Receiver {
 public:
  using MessagePtr = std::shared_ptr<M>;
  using MessageListener = std::function<void(
      const MessagePtr&, const MessageInfo&, const RoleAttributes&)>;

  Receiver(const RoleAttributes& attr, const MessageListener& msg_listener);
  virtual ~Receiver();

  virtual void Enable() = 0;
  virtual void Disable() = 0;
  virtual void Enable(const RoleAttributes& opposite_attr) = 0;
  virtual void Disable(const RoleAttributes& opposite_attr) = 0;
  const Identity& id() const { return id_; }
  const RoleAttributes role_attributes() const { return role_attr_; }
 protected:
  void OnNewMessage(const MessagePtr& msg, const MessageInfo& msg_info);
  RoleAttributes  role_attr_;
  MessageListener msg_listener_;
  Identity id_;
  bool is_enabled_;
};

//template <typename M>
// Receiver<M>::Receiver(const RoleAttributes& attr,
//                       const MessageListener& msg_listener)
//     : role_attr_(attr), 
//       msg_listener_(msg_listener),
//       id_() {
//   if (!role_attr_.has_process_id()) {
//     role_attr_.set_process_id(GlobalData::Instance()->ProcessId());
//   }
//   if (!role_attr_.has_id()) {
//     role_attr_.set_id(id_.HashValue());
//   }
// }
////// | ////新增
////// V ////
template <typename M>
Receiver<M>::Receiver(const RoleAttributes& attr,
                      const MessageListener& msg_listener)
    : role_attr_(attr), 
      msg_listener_(msg_listener),
      id_(),
      is_enabled_(false) {
  if (role_attr_.has_id()) {
    id_.set_data(std::to_string(role_attr_.id()).c_str());
  }
  role_attr_.set_id(id_.HashValue());
  ////// | ////新增
  ////// V ////
  // if (!role_attr_.has_id()) {
  //   role_attr_.set_id(id_.HashValue());
  // }

  if (!role_attr_.has_process_id()) {
    role_attr_.set_process_id(GlobalData::Instance()->ProcessId());
  }
}


template <typename M>
Receiver<M>::~Receiver() {}

template <typename M>
void Receiver<M>::OnNewMessage(const MessagePtr& msg,
                               const MessageInfo& msg_info) {
  if (msg_listener_ != nullptr && role_attr_.has_channel_name()) {
    //SPDLOG_INFO("DEBUG cb(msg) msg_listener_(msg, msg_info, role_attr_)");
    msg_listener_(msg, msg_info, role_attr_);
  }else{
    SPDLOG_WARN("DEBUG msg_listener_ == nullptr");
  }
} 

}  // namespace transport
}  // namespace os
}  // namespace xag_nav

#endif  // CYBER_TRANSPORT_RECEIVER_RECEIVER_H_
