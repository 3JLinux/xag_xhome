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

#ifndef CYBER_NODE_WRITER_BASE_H_
#define CYBER_NODE_WRITER_BASE_H_

#include <atomic>
#include <mutex>
#include <string>
#include <vector>
#include <memory>

#include "common/proto/role_attributes.pb.h"

namespace xag_nav {
namespace os {

/**
 * @class WriterBase
 * @brief Base class for a Writer. A Writer is an object to send messages
 * through a 'Channel'
 * @warning One Writer can only write one channel.
 * But different writers can write through the same channel
 */
using proto::RoleAttributes;
class WriterBase {
 public:
  /**
   * @brief Construct a new Writer Base object
   *
   * @param role_attr role attributes for this Writer
   */
  explicit WriterBase(const RoleAttributes& role_attr)
      : role_attr_(role_attr), is_enable_(false) {}

  virtual ~WriterBase() {}

  /**
   * @brief Enable the Writer
   *
   * @return true if init success
   * @return false if init failed
   */
  virtual bool Enable() = 0;

  /**
   * @brief Disable the Writer
   */
  virtual void Disable() = 0;

  /**
   * @brief Get Writer's Channel name
   *
   * @return const std::string& const reference to the channel name
   */
  const std::string& ChannelName() const {
    return role_attr_.channel_name();
  }
  /**
   * @brief Is there any Reader that subscribes our Channel?
   * You can publish message when this return true
   *
   * @return true if the channel has reader
   * @return false if the channel has no reader
   */
  virtual bool HasReader() { return false; }

  /**
   * @brief Get all Readers that subscriber our writing channel
   *
   * @param readers result vector of RoleAttributes
   */
  virtual void GetReaders(std::vector<RoleAttributes>* readers) {}

  virtual bool IsEnable() const { return is_enable_.load(); }


 protected:
  proto::RoleAttributes role_attr_;
  std::atomic<bool> is_enable_;
  // mutable std::mutex lock_;
  // bool init_;
};

}  
}
#endif  // CYBER_NODE_WRITER_BASE_H_
