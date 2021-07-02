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

#ifndef CYBER_TRANSPORT_SHM_BLOCK_H_
#define CYBER_TRANSPORT_SHM_BLOCK_H_

#include <atomic>
#include <cstdint>
namespace xag_nav {
namespace os {
namespace transport{

/** 
  * @brief: 作为segment的一个个区域。可以把segment当作仓库，Block就是货架区
  * @privite: lock_num_原子操作实现读写锁（极大提高访问速率)
  * bool compare_exchange_weak( T& expected, T desired,
                            std::memory_order success,
                            std::memory_order failure );
  */

class Block {
  friend class Segment;

 public:
  Block();
  virtual ~Block();

  uint64_t msg_size() const { return msg_size_; }
  void set_msg_size(uint64_t msg_size) { msg_size_ = msg_size; }

  uint64_t msg_info_size() const { return msg_info_size_; }
  void set_msg_info_size(uint64_t msg_info_size) {
    msg_info_size_ = msg_info_size;
  }
  // 读写锁状态信息
  static const int32_t kRWLockFree;
  static const int32_t kWriteExclusive;
  static const int32_t kMaxTryLockTimes;

 private:
 /** 
  * @brief 尝试写操作
  * @return 成功返回ture, 达到最大尝试次数返回false
  *
  */
  bool TryLockForWrite();
  /** 
  * @brief 尝试读操作
  * @return 成功返回ture, 达到最大尝试次数返回false
  *
  */
  bool TryLockForRead();
  /** 
    * @brief 释放写锁
    * @return 
    */
  void ReleaseWriteLock();
  /** 
    * @brief 释放读锁
   */ 
  void ReleaseReadLock();

  volatile std::atomic<int32_t> lock_num_ = {0};

  uint64_t msg_size_;
  uint64_t msg_info_size_;
};

}
}
}
#endif  // CYBER_TRANSPORT_SHM_BLOCK_H_
