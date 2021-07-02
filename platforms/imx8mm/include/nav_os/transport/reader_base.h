#ifndef CYBER_NODE_READER_BASE_H_
#define CYBER_NODE_READER_BASE_H_

#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/proto/role_attributes.pb.h"
#include "transport/receiver/receiver.h"
#include "transport/transport.h"

namespace xag_nav {
namespace os {
using proto::RoleAttributes;
template<typename M>
using CallbackFunc = std::function<void(const std::shared_ptr<M>&)>;
/**
 * @class ReaderBase
 * @brief Base Class for Reader
 * Reader is identified by one xag_nav::os::proto:RoleAttribute,
 * it contains the channel_name, channel_id that we subscribe,
 * and process_id and node that we are located
 */
class ReaderBase {
 public:
  explicit ReaderBase(const RoleAttributes& role_attr)
      : role_attr_(role_attr), init_(false) {}
  virtual ~ReaderBase() {}

  /**
   * @brief Init the Reader object
   *
   * @return true if init successfully
   * @return false if init failed
   */
  virtual bool Init() = 0;

  /**
   * @brief Shutdown the Reader object
   */
  virtual void Shutdown() = 0;

  /**
   * @brief Query is there any writer that publish the subscribed channel
   * @return true if there is at least one Writer publish the channel
   * @return false if there is no Writer publish the channel
   */
  virtual bool HasWriter() { return false; }

  /**
   * @brief Get all writers pushlish the channel we subscribes
   *
   * @param writers result RoleAttributes vector
   */
  virtual void GetWriters(std::vector<RoleAttributes>* writers) {}

  /**
   * @brief Get Reader's Channel name
   *
   * @return const std::string& channel name
   */
  const std::string& ChannelName() const {
    return role_attr_.channel_name();
  }
  /**
   * @brief Get Reader's Channel id
   *
   * @return uint64_t channel id
   */
  uint64_t ChannelId() const { return role_attr_.channel_id(); }

  /**
   * @brief Query whether the Reader is initialized
   *
   * @return true if the Reader has been inited
   * @return false if the Reader has not been inited
   */
  bool IsInit() const { return init_.load(); }

 protected:
  // template <typename M>
  // void OnNewMessageCb(const std::shared_ptr<M> msg){
    
  // }

  RoleAttributes role_attr_;
  std::atomic<bool> init_;
};

/**
 * @brief One Channel is related to one Receiver.
 * ReceiverManager is in charge of attaching one Receiver to its responding
 * Channel. We pass a DataDispatche's callback func to this Receiver so when a
 * message is received, it will be push to the `ChannelBuffer`, and
 * `DataVisitor` will `Fetch` data and pass to `Reader`'s callback func
 *
 * @tparam MessageT Message Type.
 */
template <typename MessageT>
class ReceiverManager {
 public:
  // ~ReceiverManager() { receiver_map_.clear(); }
  ~ReceiverManager() = default;
  /**
   * @brief Get the Receiver object
   *
   * @param role_attr the attribute that the Receiver has
   * @return std::shared_ptr<transport::Receiver<MessageT>> result Receiver
   */
  auto GetReceiver(const RoleAttributes& role_attr,
                   const CallbackFunc<MessageT>& cb) 
      -> typename std::shared_ptr<transport::Receiver<MessageT>>;

 private:
  // key: channel_map
  // std::unordered_multimap<std::string,
  //                    typename std::shared_ptr<transport::Receiver<MessageT>>>
  //     receiver_map_;
  // std::mutex receiver_map_mutex_;
  DECLARE_SINGLETON(ReceiverManager<MessageT>)
};

/**
 * @brief Construct a new Receiver Manager< Message T>:: Receiver Manager object
 * @tparam MessageT param
 */
template <typename MessageT>
ReceiverManager<MessageT>::ReceiverManager() {}

template <typename MessageT>
auto ReceiverManager<MessageT>::GetReceiver(
    const RoleAttributes& role_attr,
    const CallbackFunc<MessageT>& cb) ->
    typename std::shared_ptr<transport::Receiver<MessageT>> {
  // std::lock_guard<std::mutex> lock(receiver_map_mutex_);
  // because multi reader for one channel will write datacache multi times,
  // so reader for datacache we use map to keep one instance for per channel
  // const std::string& channel_name = role_attr.channel_name();
  // if (receiver_map_.count(channel_name) == 0) {
    // receiver_map_[channel_name] =
        // transport::Transport::Instance()->CreateReceiver<MessageT>(
        //     role_attr, [=](const std::shared_ptr<MessageT>& msg,
        //                   const transport::MessageInfo& msg_info,
        //                   const RoleAttributes& reader_attr) {
        //       (void)msg_info;
        //       (void)reader_attr;
        //       if (cb != nullptr){
        //         cb(msg);
        //       }
        // });
  // }
  // return receiver_map_[channel_name];
  return transport::Transport::Instance()->CreateReceiver<MessageT>(
            role_attr, [=](const std::shared_ptr<MessageT>& msg,
                          const transport::MessageInfo& msg_info,
                          const RoleAttributes& reader_attr) {
              (void)msg_info;
              (void)reader_attr;
              if (cb != nullptr){
                cb(msg);
              }
        });
}




}  // namespace cyber
}  // namespace apollo

#endif  // CYBER_NODE_READER_BASE_H_
