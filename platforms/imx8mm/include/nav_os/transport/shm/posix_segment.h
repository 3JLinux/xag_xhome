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

#ifndef CYBER_TRANSPORT_SHM_POSIX_SEGMENT_H_
#define CYBER_TRANSPORT_SHM_POSIX_SEGMENT_H_

#include <string>
#include "transport/shm/segment.h"
// #include "common/proto/role_attribute.pb.h"

namespace xag_nav {
namespace os {
namespace transport{
/** 
  * @brief 基于poxis系统调用的共享内存区相关调用
  * @privite: shm_name_  将会在 /dev/shm目录下建立该名字文件
  */
class PosixSegment : public Segment {
 public:
  explicit PosixSegment(uint64_t channel_id);
  // explicit PosixSegment(const std::string&  channel_name);
  virtual ~PosixSegment();

 private:
  void Reset() override;
  bool Remove() override;
  bool OpenOnly() override;
  bool OpenOrCreate() override;
  std::string shm_name_;
};

}
}
}

#endif  // CYBER_TRANSPORT_SHM_POSIX_SEGMENT_H_
