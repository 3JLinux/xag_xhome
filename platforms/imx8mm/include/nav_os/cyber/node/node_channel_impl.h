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

#ifndef CYBER_NODE_NODE_CHANNEL_IMPL_H_
#define CYBER_NODE_NODE_CHANNEL_IMPL_H_

#include <memory>
#include <string>

#include "common/global_data.h"
#include "transport/reader.h"
#include "transport/writer.h"
#include "util/utils.h"
#include "common/proto/role_attributes.pb.h"
#include "transport/message/message_wrapper/message_traits.h"
namespace xag_nav {
namespace os {
using proto::RoleAttributes;
using common::GlobalData;
class Node;
/**
 * @class NodeChannelImpl
 * @brief The implementation for Node to create Objects connected by Channels.
 * e.g. Channel Reader and Writer
 */
class NodeChannelImpl {
  friend class Node;
 public:
  /**
   * @brief Construct a new Node Channel Impl object
   *
   * @param node_name node name
   */
  explicit NodeChannelImpl(const std::string& node_name)
      : node_name_(node_name) {
    node_attr_.set_process_id(common::GlobalData::Instance()->ProcessId());
    node_attr_.set_node_name(node_name);
  }

  /**
   * @brief Destroy the Node Channel Impl object
   */
  virtual ~NodeChannelImpl() {}

  /**
   * @brief get name of this node
   *
   * @return const std::string& actual node name
   */
  const std::string& NodeName() const { return node_name_; }

 private:
  template <typename MessageT>
  auto CreateWriter(const RoleAttributes& role_attr)
      -> std::shared_ptr<Writer<MessageT>>;

  template <typename MessageT>
  auto CreateWriter(const std::string& channel_name)
      -> std::shared_ptr<Writer<MessageT>>;

  template <typename MessageT>
  auto CreateReader(const std::string& channel_name,
                    const CallbackFunc<MessageT>& reader_func)
      -> std::shared_ptr<Reader<MessageT>>;
  template <typename MessageT>
  auto CreateReader(const RoleAttributes& role_attr,
                                    const CallbackFunc<MessageT>& reader_func)
      -> std::shared_ptr<Reader<MessageT>>;
  template <typename MessageT>
  auto CreateReader(const RoleAttributes& role_attr)
      -> std::shared_ptr<Reader<MessageT>>;

  template <typename MessageT>
  void FillInAttr(RoleAttributes* attr);

  std::string node_name_;
  RoleAttributes node_attr_;
};

template <typename MessageT>
auto NodeChannelImpl::CreateWriter(const RoleAttributes& role_attr)
    -> std::shared_ptr<Writer<MessageT>> {
  if (!role_attr.has_channel_name() || role_attr.channel_name().empty()) {
    SPDLOG_ERROR("Can't create a writer with empty channel name!");
    return nullptr;
  }
  RoleAttributes new_attr(role_attr);
  FillInAttr<MessageT>(&new_attr);

  std::shared_ptr<Writer<MessageT>> writer_ptr = nullptr;
  writer_ptr = std::make_shared<Writer<MessageT>>(new_attr);
  RETURN_VAL_IF_NULL(writer_ptr, nullptr);
  RETURN_VAL_IF(!writer_ptr->Enable(), nullptr);
  return writer_ptr;
}

template <typename MessageT>
auto NodeChannelImpl::CreateWriter(const std::string& channel_name)
    -> std::shared_ptr<Writer<MessageT>> {
  RoleAttributes role_attr;
  role_attr.set_channel_name(channel_name);
  return this->CreateWriter<MessageT>(role_attr);
}

template <typename MessageT>
auto NodeChannelImpl::CreateReader(const std::string& channel_name,
                                   const CallbackFunc<MessageT>& reader_func)
    -> std::shared_ptr<Reader<MessageT>> {
  RoleAttributes role_attr;
  role_attr.set_channel_name(channel_name);
  return this->template CreateReader<MessageT>(role_attr, reader_func);
}


template <typename MessageT>
auto NodeChannelImpl::CreateReader(const RoleAttributes& role_attr,
                                   const CallbackFunc<MessageT>& reader_func)
    -> std::shared_ptr<Reader<MessageT>> {
  if (!role_attr.has_channel_name() || role_attr.channel_name().empty()) {
    SPDLOG_ERROR("Can't create a reader with empty channel name!");
    return nullptr;
  }
  RoleAttributes new_attr(role_attr);
  FillInAttr<MessageT>(&new_attr);
  std::shared_ptr<Reader<MessageT>> reader_ptr = nullptr;
  reader_ptr =
        std::make_shared<Reader<MessageT>>(new_attr, reader_func);
  RETURN_VAL_IF_NULL(reader_ptr, nullptr);
  RETURN_VAL_IF(!reader_ptr->Init(), nullptr);
  return reader_ptr;
}

template <typename MessageT>
auto NodeChannelImpl::CreateReader(const RoleAttributes& role_attr)
    -> std::shared_ptr<Reader<MessageT>> {
  return this->template CreateReader<MessageT>(role_attr, nullptr);
}

template <typename MessageT>
void NodeChannelImpl::FillInAttr(RoleAttributes* attr) {
  attr->set_process_id(node_attr_.process_id());
  attr->set_node_name(node_attr_.node_name());
  auto channel_id = GlobalData::RegisterChannel(attr->channel_name());
  attr->set_channel_id(channel_id);
  if (!attr->has_message_type()) {
    attr->set_message_type(util::TypeToName<MessageT>());
  }
  // TODO 
  if (!attr->has_proto_desc()) {
    std::string proto_desc("");
  //   transport::message::GetDescriptorString<MessageT>(attr->message_type(), &proto_desc);
    attr->set_proto_desc(proto_desc);
  }
}


}
}
#endif  // CYBER_NODE_NODE_CHANNEL_IMPL_H_
