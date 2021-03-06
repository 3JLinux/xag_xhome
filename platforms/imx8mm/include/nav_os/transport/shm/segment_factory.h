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

#ifndef CYBER_TRANSPORT_SHM_SEGMENT_FACTORY_H_
#define CYBER_TRANSPORT_SHM_SEGMENT_FACTORY_H_
#include <memory>
#include <string>
#include "spdlog/spdlog.h"
#include "transport/shm/segment.h"
#include "transport/shm/posix_segment.h"
#include "transport/shm/xsi_segment.h"
namespace xag_nav {
namespace os {
namespace transport{

class SegmentFactory {
 public:
  static SegmentPtr CreateSegment(uint64_t channel_id);
};

}
}
}

#endif  // CYBER_TRANSPORT_SHM_SEGMENT_FACTORY_H_
