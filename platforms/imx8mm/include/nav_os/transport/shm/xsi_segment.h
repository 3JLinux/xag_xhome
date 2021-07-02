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

#ifndef CYBER_TRANSPORT_SHM_XSI_SEGMENT_H_
#define CYBER_TRANSPORT_SHM_XSI_SEGMENT_H_

#include "transport/shm/segment.h"

namespace xag_nav {
namespace os {
namespace transport{

/** 
  * @brief apollo默认使用Xsi方式而非posix，打开关闭的方式不同而已。
  * Xsi通过key来定位共享内存，并且内存管理也有一些区别。
  * 了解的情况看， 性能 xsi >= posix 稳定性 posix >= xsi
  */
class XsiSegment : public Segment {
 public:
  explicit XsiSegment(uint64_t channel_id);
  virtual ~XsiSegment();

  static const char* Type() { return "xsi"; }

 private:
  void Reset() override;
  bool Remove() override;
  bool OpenOnly() override;
  bool OpenOrCreate() override;

  key_t key_;
};

}  // namespace transport
}  // namespace os
}  // namespace xag_nav

#endif  // CYBER_TRANSPORT_SHM_XSI_SEGMENT_H_
