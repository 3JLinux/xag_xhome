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

#ifndef CYBER_TRANSPORT_SHM_SEGMENT_H_
#define CYBER_TRANSPORT_SHM_SEGMENT_H_

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "transport/shm/block.h"
#include "transport/shm/shm_conf.h"
#include "transport/shm/state.h"

namespace xag_nav {
namespace os {
namespace transport{

class Segment;

using SegmentPtr = std::shared_ptr<Segment>;

/** 
 * @brief 可写区域(或者叫共享内存货架)信息。
 * @member: index 货架号
 *          block 用来记录当前货架的状态信息(包括该货架状态和可读写货架大小)
 *          buf 货架上放置货物首地址
 */
struct WritableBlock {
  uint32_t index = 0;
  Block* block = nullptr;
  uint8_t* buf = nullptr;
};

using ReadableBlock = WritableBlock;

/** 
  * @brief 这是共享内存区的基类。可由不同的系统调用实现 
  * 一个segment至多对应一个通道名，相同通道名使用同一个共享内存segment
  * @privite: block_buf_addrs_ 看作是uint 
*/

class Segment {
 public:
  explicit Segment(uint64_t channel_id);
  virtual ~Segment() {}
  /** 
    * @brief 获取一块Block进行写操作
    * @param[in] msg_size 需要的大小
    * @param[in] writable_block 当成功申请到时，调用方将通过该结构体获取可用资源的信息
    * @return 申请成功则返回True，writable_block为nullptr/未初始化/为成功OpenOrCreate
    * 未能分配足够size将返回false
   */
  bool AcquireBlockToWrite(std::size_t msg_size, WritableBlock* writable_block);
  
  void ReleaseWrittenBlock(const WritableBlock& writable_block);

  bool AcquireBlockToRead(ReadableBlock* readable_block);
  void ReleaseReadBlock(const ReadableBlock& readable_block);

 protected:
  virtual bool Destroy();
  virtual void Reset() = 0;
  virtual bool Remove() = 0;
  virtual bool OpenOnly() = 0;
  virtual bool OpenOrCreate() = 0;

  bool init_;
  ShmConf conf_;
  uint64_t channel_id_;
  State* state_;
  Block* blocks_;
  void* managed_shm_;
  std::mutex block_buf_lock_;
  // key: index,value=buf_addr
  std::unordered_map<uint32_t, uint8_t*> block_buf_addrs_;

 private:
  bool Remap();
  bool Recreate(const uint64_t& msg_size);
  uint32_t GetNextWritableBlockIndex();
};

}
}
}

#endif  // CYBER_TRANSPORT_SHM_SEGMENT_H_
