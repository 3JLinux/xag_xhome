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

#ifndef CYBER_COMMON_GLOBAL_DATA_H_
#define CYBER_COMMON_GLOBAL_DATA_H_
#include <string>

// #include <uuid/uuid.h>
#include "common/uuid/uuid.h"
#include "spdlog/spdlog.h"
#include "common/scope_guard.h"
#include "common/macros.h"
#include "cyber/base/atomic_hash_map.h"
namespace xag_nav {
namespace os {
namespace common {
using apollo::cyber::base::AtomicHashMap;

class GlobalData {
 public:
  
  ~GlobalData(){}

  const int ProcessId() const;

  template<typename Type>
  inline  static std::size_t HashValue(const Type& key) { return std::hash<Type>{}(key);}
  static uint64_t GenerateHashId(const std::string channel_name){
    return std::hash<std::string>{}(channel_name);
  }
  static uint64_t RegisterChannel(const std::string& channel_name);
  // std::string GetChannelById(std::size_t id);
  static const std::string GetChannelById (uint64_t id) ;
  // 临时使用
  static void RegisterShmChannel(const std::string& channel_name){
    uint64_t channel_id = HashValue<std::string>(channel_name);
    if (count(ShmChannel.begin(), ShmChannel.end(), channel_id) == 0){
      ShmChannel.emplace_back(channel_id);
    }
  }
  static std::vector<uint64_t>& GetShmChannel() { return ShmChannel; }
 private:
  static std::vector<uint64_t> ShmChannel;
  static AtomicHashMap<uint64_t, std::string, 256> channel_id_map_;
  // process info
  int process_id_;
  DECLARE_SINGLETON(GlobalData)
};


}  // namespace common
}  // namespace cyber
}  // namespace apollo

#endif  // CYBER_COMMON_GLOBAL_DATA_H_
