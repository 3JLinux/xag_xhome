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

#ifndef CYBER_NODE_NODE_H_
#define CYBER_NODE_NODE_H_

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <mutex>
#include "transport/reader.h"
#include "transport/writer.h"
#include "cyber/node/node_channel_impl.h"
#include "common/proto/role_attributes.pb.h"
namespace xag_nav {
namespace os {

using proto::RoleAttributes;
/**
 * @class Node
 * @brief Node is the fundamental building part.
 * every module contains and communicates through the node.
 * A module can have different types of communication by defining
 * read/write in a node.
 * @warning Duplicate name is not allowed in objects
 */
class Node {
 public:

  friend class ComponentBase;
  friend std::unique_ptr<Node> CreateNode(const std::string&);
  ~Node(){}
  /**
   * @brief Get node's name.
   * @warning duplicate node name is not allowed in the topo.
   */
  const std::string& Name() const;

  /**
   * @brief Create a Writer with specific message type.
   * @tparam MessageT Message Type
   * @param role_attr is a protobuf message RoleAttributes, which includes the
   * channel name and other info.
   * @return std::shared_ptr<Writer<MessageT>> result Writer Object
   */
  template <typename MessageT>
  auto CreateWriter(const RoleAttributes& role_attr)
      -> std::shared_ptr<Writer<MessageT>>;

  /**
   * @brief Create a Writer with specific message type.
   * @tparam MessageT Message Type
   * @param channel_name the channel name to be published.
   * @return std::shared_ptr<Writer<MessageT>> result Writer Object
   */
  template <typename MessageT>
  auto CreateWriter(const std::string& channel_name)
      -> std::shared_ptr<Writer<MessageT>>;

  /**
   * @brief Create a Reader with specific message type with channel name
   * qos and other configs used will be default
   *
   * @tparam MessageT Message Type
   * @param channel_name the channel of the reader subscribed.
   * @param reader_func invoked when message receive
   * invoked when the message is received.
   * @return std::shared_ptr<cyber::Reader<MessageT>> result Reader Object
   */
  template <typename MessageT>
  auto CreateReader(const std::string& channel_name,
                    const CallbackFunc<MessageT>& reader_func = nullptr)
      -> std::shared_ptr<Reader<MessageT>>;
  /**
   * @brief Create a Reader object with `RoleAttributes`
   *
   * @tparam MessageT Message Type
   * @param role_attr instance of `RoleAttributes`,
   * includes channel name, qos, etc.
   * @param reader_func invoked when message receive
   * @return std::shared_ptr<cyber::Reader<MessageT>> result Reader Object
   */
  template <typename MessageT>
  auto CreateReader(const RoleAttributes& role_attr,
                    const CallbackFunc<MessageT>& reader_func = nullptr)
      -> std::shared_ptr<Reader<MessageT>>;

  // /**
  //  * @brief Get the Reader object that subscribe `channel_name`
  //  *
  //  * @tparam MessageT Message Type
  //  * @param channel_name channel name
  //  * @return std::shared_ptr<Reader<MessageT>> result reader
  //  */
  // template <typename MessageT>
  // auto GetReader(const std::string& channel_name)
  //     -> std::shared_ptr<Reader<MessageT>>;

 private:
  explicit Node(const std::string& node_name);

  std::string node_name_;
  // std::mutex readers_mutex_;
  // std::map<std::string, std::shared_ptr<ReaderBase>> readers_;
  std::unique_ptr<NodeChannelImpl> node_channel_impl_ = nullptr;
};

template <typename MessageT>
auto Node::CreateWriter(const RoleAttributes& role_attr)
    -> std::shared_ptr<Writer<MessageT>> {
  return node_channel_impl_->CreateWriter<MessageT>(role_attr);
}

template <typename MessageT>
auto Node::CreateWriter(const std::string& channel_name)
    -> std::shared_ptr<Writer<MessageT>> {
  return node_channel_impl_->template CreateWriter<MessageT>(channel_name);
}

template <typename MessageT>
auto Node::CreateReader(const RoleAttributes& role_attr,
                        const CallbackFunc<MessageT>& reader_func)
    -> std::shared_ptr<Reader<MessageT>> {
  // std::lock_guard<std::mutex> lg(readers_mutex_);
  // if (readers_.find(role_attr.channel_name()) != readers_.end()) {
  //   SPDLOG_WARN("Failed to create reader: reader with the same channel already exists.");
  //   return nullptr;
  // }
  auto reader = node_channel_impl_->template CreateReader<MessageT>(
      role_attr, reader_func);
  // if (reader != nullptr) {
  //   readers_.emplace(std::make_pair(role_attr.channel_name(), reader));
  // }
  return reader;
}

template <typename MessageT>
auto Node::CreateReader(const std::string& channel_name,
                        const CallbackFunc<MessageT>& reader_func)
    -> std::shared_ptr<Reader<MessageT>> {
  // std::lock_guard<std::mutex> lg(readers_mutex_);
  // if (readers_.find(channel_name) != readers_.end()) {
  //   SPDLOG_WARN("Failed to create reader: reader with the same channel already exists.");
  //   return nullptr;

  auto reader = node_channel_impl_->template CreateReader<MessageT>(
      channel_name, reader_func);
  // if (reader != nullptr) {
  //   readers_.emplace(std::make_pair(channel_name, reader));
  // }
  return reader;
}
// template <typename MessageT>
// auto Node::GetReader(const std::string& name)
//     -> std::shared_ptr<Reader<MessageT>> {
//   std::lock_guard<std::mutex> lg(readers_mutex_);
//   auto it = readers_.find(name);
//   if (it != readers_.end()) {
//     return std::dynamic_pointer_cast<Reader<MessageT>>(it->second);
//   }
//   return nullptr;
// }

}  
} 

#endif  // CYBER_NODE_NODE_H_
