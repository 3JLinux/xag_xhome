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

#ifndef CYBER_TRANSPORT_SHM_NOTIFIER_BASE_H_
#define CYBER_TRANSPORT_SHM_NOTIFIER_BASE_H_

#include <memory>

#include "transport/shm/notifier/readable_info.h"

namespace xag_nav {
namespace os {
namespace transport{

class NotifierBase;
using NotifierPtr = NotifierBase*;
/** 
  * @brief 通报器/唤醒器 
  *
  */
class NotifierBase {
 public:
  virtual ~NotifierBase() = default;
  // 关停
  virtual void Shutdown() = 0;
  // 写完告知。
  virtual bool Notify(const ReadableInfo& info) = 0;
  // 尝试读取。
  virtual bool Listen(int timeout_ms, ReadableInfo* info) = 0;
};

}
}
}

#endif  // CYBER_TRANSPORT_SHM_NOTIFIER_BASE_H_
