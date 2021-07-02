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

#ifndef CYBER_MESSAGE_RAW_MESSAGE_H_
#define CYBER_MESSAGE_RAW_MESSAGE_H_

#include <cassert>
#include <cstring>
#include <memory>
#include <string>

#include "transport/message/message_wrapper/protobuf_factory.h"


namespace xag_nav {
namespace os {
namespace transport {
namespace message{

struct RawMessage {
  RawMessage() : message_(""){}

  explicit RawMessage(const std::string &data) : message_(data){}

  RawMessage(const RawMessage &raw_msg)
      : message_(raw_msg.message_){}
  RawMessage &operator=(const RawMessage &raw_msg) {
    if (this != &raw_msg) {
      this->message_ = raw_msg.message_;
    }
    return *this;
  }

  ~RawMessage() {}

  class Descriptor {
   public:
    std::string full_name() const { return "message.RawMessage"; }
    std::string name() const { return "message.RawMessage"; }
  };

  static const Descriptor *descriptor() {
    static Descriptor desc;
    return &desc;
  }

  static void GetDescriptorString(const std::string &type,
                                  std::string *desc_str) {
    ProtobufFactory::Instance()->GetDescriptorString(type, desc_str);
  }

  bool SerializeToArray(void *data, int size) const {
    if (data == nullptr || size < ByteSize()) {
      return false;
    }
    memcpy(data, message_.data(), message_.size());
    return true;
  }

  bool SerializeToString(std::string *str) const {
    if (str == nullptr) {
      return false;
    }
    *str = message_;
    return true;
  }

  bool ParseFromArray(const void *data, int size) {
    if (data == nullptr || size <= 0) {
      return false;
    }

    message_.assign(reinterpret_cast<const char *>(data), size);
    return true;
  }

  bool ParseFromString(const std::string &str) {
    message_ = str;
    return true;
  }

  int ByteSize() const { return static_cast<int>(message_.size()); }

  static std::string TypeName() { return "xag_nav::os::transport::message::RawMessage"; }

  std::string message_;
};

}  // namespace message
}  // namespace cyber
}  // namespace apollo
}
#endif  // CYBER_MESSAGE_RAW_MESSAGE_H_
