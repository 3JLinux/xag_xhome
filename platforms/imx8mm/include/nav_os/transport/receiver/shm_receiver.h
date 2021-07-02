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

#ifndef CYBER_TRANSPORT_RECEIVER_SHM_RECEIVER_H_
#define CYBER_TRANSPORT_RECEIVER_SHM_RECEIVER_H_

#include <functional>
#include <string>
#include "spdlog/spdlog.h"
#include "transport/dispatcher/shm_dispatcher.h"
#include "transport/receiver/receiver.h"
#include "common/proto/role_attributes.pb.h"

namespace xag_nav {
namespace os {
namespace transport{


template <typename M>
class ShmReceiver : public Receiver<M> {
 public:

  ShmReceiver(const RoleAttributes& attr,
              const typename Receiver<M>::MessageListener& _reader_func = nullptr);

  virtual ~ShmReceiver();

  void Enable() override;
  void Disable() override;

  void Enable(const RoleAttributes& opposite_attr) override;
  void Disable(const RoleAttributes& opposite_attr) override;

 private:
  
  ShmDispatcherPtr dispatcher_;
};

template <typename M>
ShmReceiver<M>::ShmReceiver(const RoleAttributes& attr,
    const typename  Receiver<M>::MessageListener& _reader_func)
    : Receiver<M>(attr, _reader_func) {
  dispatcher_ = ShmDispatcher::Instance();
  // Enable();
}



template <typename M>
ShmReceiver<M>::~ShmReceiver() {
  Disable();
}

template <typename M>
void ShmReceiver<M>::Enable() {
  if (this->is_enabled_) {
    return;
  }
	//SPDLOG_INFO("DEBUG ShmReceiver<MessageT>::Enable self_id = {}", this->id().ToString());

  dispatcher_->AddListener<M>(
    this->role_attr_, std::bind(&ShmReceiver<M>::OnNewMessage, this,
                            std::placeholders::_1, std::placeholders::_2));
  this->is_enabled_ = true;
}

template <typename M>
void ShmReceiver<M>::Disable() {
  if (!this->is_enabled_) {
    return;
  }
  dispatcher_->RemoveListener<M>(this->role_attr_);
  this->is_enabled_ = false;
}

template <typename M>
void ShmReceiver<M>::Enable(const RoleAttributes& opposite_attr) {
  //SPDLOG_INFO("DEBUG ShmReceiver<MessageT>::Enable self_id = {}, oppo_id = {}", 
    //    this->id().ToString(), opposite_attr.id());

  dispatcher_->AddListener<M>(
      this->role_attr_, opposite_attr,
      std::bind(&ShmReceiver<M>::OnNewMessage, this, std::placeholders::_1,
                std::placeholders::_2));
}

template <typename M>
void ShmReceiver<M>::Disable(const RoleAttributes& opposite_attr) {
  dispatcher_->RemoveListener<M>(this->role_attr_, opposite_attr);
}

}
}
}

#endif  // CYBER_TRANSPORT_RECEIVER_SHM_RECEIVER_H_
