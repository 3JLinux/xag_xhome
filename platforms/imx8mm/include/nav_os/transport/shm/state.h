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

#ifndef CYBER_TRANSPORT_SHM_STATE_H_
#define CYBER_TRANSPORT_SHM_STATE_H_

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
namespace xag_nav {
namespace os {
namespace transport{

/** 
  * @brief segment的状态类
  * @param[in] need_remap_ 是否需要重新remap
  *            seq_ 有多少个block
  *            reference_count_ 引用次数
  *            max_msg_size_  最大空间大小,单位byte
  */
class State {
 public:
  explicit State(const uint64_t& max_msg_size);
  virtual ~State(){}
  //todo
  void DecreaseReferenceCounts() {
    uint32_t current_reference_count = reference_count_.load();
    do {
      if (current_reference_count == 0) {
        return;
      }
    } while (!reference_count_.compare_exchange_strong(
        current_reference_count, current_reference_count - 1));
  }
  void IncreaseReferenceCounts() { reference_count_.fetch_add(1); }

  uint32_t FetchAddSeq(uint32_t diff) { return seq_.fetch_add(diff); }
  uint32_t get_seq() { return seq_.load(); }

  void set_need_remap(bool need) { need_remap_.store(need); }
  bool need_remap() { return need_remap_; }

  uint64_t max_msg_size() { return max_msg_size_.load(); }
  uint32_t reference_counts() { return reference_count_.load(); }

 private:
  
  std::atomic<bool> need_remap_ = {false};
  std::atomic<uint32_t> seq_ = {0};
  std::atomic<uint32_t> reference_count_ = {0};
  std::atomic<uint64_t> max_msg_size_;
};


}
}
}

#endif  // CYBER_TRANSPORT_SHM_STATE_H_
