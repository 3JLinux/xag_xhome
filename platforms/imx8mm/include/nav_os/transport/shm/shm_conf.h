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

#ifndef CYBER_TRANSPORT_SHM_SHM_CONF_H_
#define CYBER_TRANSPORT_SHM_SHM_CONF_H_

#include <cstdint>
#include <string>

namespace xag_nav {
namespace os {
namespace transport{
/** 
  * @brief 这是segment配置类
  * @param[in] real_msg_size 共享内存区的总大小
  * @pritive: max_msg_size 表某共享内存区的最大容量。 
  *           block_buf_size_ 所有货架总大小
  *           block_num_ 货架数量
  *           managed_shm_size_ 当前管理的仓库大小。
  * 内存模型
  * |-------------------> managed_shm_size_<------------------------------ ---|                  
  * |State|Block1|Block2|…|Blockn|block_buf1+MF|block_buf2+MF|…| block_bufn+MF|...|
  *                              |--->  <------|
  *                           block_buf_size_
  * |------------------------->   max_msg_size      <-------------------------- --|
  * block_num_ == n
  */
class ShmConf {
 public:
  ShmConf();
  explicit ShmConf(const uint64_t& real_msg_size);
  virtual ~ShmConf();
  /** 
    * @brief 用来初始化/更新空间大小。
    * @param[in] real_msg_size 需要申请的空间大小
    */
  void Update(const uint64_t& real_msg_size);

  const uint64_t& max_msg_size() { return max_msg_size_; }
  const uint64_t& block_buf_size() { return block_buf_size_; }
  const uint32_t& block_num() { return block_num_; }
  const uint64_t& managed_shm_size() { return managed_shm_size_; }

 private:
  uint64_t GetMaxMessageSize(const uint64_t& real_msg_size);
  uint64_t GetBlockBufSize(const uint64_t& max_msg_size);
  uint32_t GetBlockNum(const uint64_t& max_msg_size);

  uint64_t max_msg_size_;
  uint64_t block_buf_size_;
  uint32_t block_num_;
  uint64_t managed_shm_size_;

  // Extra size, Byte
  static const uint64_t EXTRA_SIZE;
  // State size, Byte
  static const uint64_t STATE_SIZE;
  // Block size, Byte
  static const uint64_t BLOCK_SIZE;
  // Message info size, Byte
  static const uint64_t MESSAGE_INFO_SIZE;
  // For message 0-2K
  static const uint32_t BLOCK_NUM_2K;
  static const uint64_t MESSAGE_SIZE_2K;
  // For message 2-16K
  static const uint32_t BLOCK_NUM_16K;
  static const uint64_t MESSAGE_SIZE_16K;
  // For message 10K-100K
  static const uint32_t BLOCK_NUM_128K;
  static const uint64_t MESSAGE_SIZE_128K;
  // For message 100K-1M
  static const uint32_t BLOCK_NUM_1M;
  static const uint64_t MESSAGE_SIZE_1M;
  // For message 1M-6M
  static const uint32_t BLOCK_NUM_8M;
  static const uint64_t MESSAGE_SIZE_8M;
  // For message 6M-10M
  static const uint32_t BLOCK_NUM_16M;
  static const uint64_t MESSAGE_SIZE_16M;
  // For message 10M+
  static const uint32_t BLOCK_NUM_MORE;
  static const uint64_t MESSAGE_SIZE_MORE;
};

}
}
}

#endif  // CYBER_TRANSPORT_SHM_SHM_CONF_H_
