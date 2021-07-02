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

#ifndef CYBER_SERVICE_DISCOVERY_ROLE_ROLE_H_
#define CYBER_SERVICE_DISCOVERY_ROLE_ROLE_H_

#include <cstdint>
#include <memory>
#include <string>
#include "common/proto/role_attributes.pb.h"

namespace xag_nav {
namespace os {
namespace transport{

using proto::RoleAttributes;
class RoleBase;
using RolePtr = std::shared_ptr<RoleBase>;

class RoleWriter;
using RoleWriterPtr = std::shared_ptr<RoleWriter>;
using RoleReader = RoleWriter;
using RoleReaderPtr = std::shared_ptr<RoleReader>;

class RoleBase {
 public:

  RoleBase();
  explicit RoleBase(const proto::RoleAttributes& attr);
  virtual ~RoleBase() = default;
  /* 
  * @brief 主要是匹配process_id
  * @return target_attr的process_id不存在或者私有attributes_的process_id不一致返回false;
  */
  virtual bool Match(const proto::RoleAttributes& target_attr) const;
  const proto::RoleAttributes& attributes() const { return attributes_; }
  void set_attributes(const proto::RoleAttributes& attr) { attributes_ = attr; }

 protected:
  proto::RoleAttributes attributes_;
};

class RoleWriter : public RoleBase {
 public:
  RoleWriter() {}
  explicit RoleWriter(const proto::RoleAttributes& attr);
  virtual ~RoleWriter() = default;
  /* 
  * @brief 主要是匹配process_id
  * @return target_attr的process_id不存在或者私有attributes_的process_id不一致返回false;
  * 如果target_attr的id/channel_id存在,则需要比较私有属性attributes_相应的id/channel_id是否一致
  * 若不存在,则只比较process_ids
  */
  bool Match(const RoleAttributes& target_attr) const override;
};



}  // namespace service_discovery
}  // namespace cyber
}  // namespace apollo

#endif  // CYBER_SERVICE_DISCOVERY_ROLE_ROLE_H_
