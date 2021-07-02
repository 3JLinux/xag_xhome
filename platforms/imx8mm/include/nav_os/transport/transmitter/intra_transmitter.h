#ifndef CYBER_TRANSPORT_TRANSMITTER_INTRA_TRANSMITTER_H_
#define CYBER_TRANSPORT_TRANSMITTER_INTRA_TRANSMITTER_H_
#include <memory>
#include <string>
#include <atomic>
#include "spdlog/spdlog.h"

#include "transport/writer_base.h"
#include "transport/dispatcher/dispatcher.h"
#include "transport/dispatcher/intra_dispatcher.h"
#include "transport/transmitter/transmitter.h"
#include "common/proto/role_attributes.pb.h"
#include "transport/topology_manager/topology_manager.h"
namespace xag_nav {
namespace os {
namespace transport{

template <typename M>
class IntraTransmitter : public Transmitter<M>{
 public:
  using MessageTypePtr = std::shared_ptr<M>;

  explicit IntraTransmitter(const RoleAttributes& role_attr)
        : Transmitter<M>(role_attr),
          channel_id_(role_attr.channel_id()),
          dispatcher_(nullptr){}

  virtual ~IntraTransmitter() { Disable(); }

  void Enable() override; 

  void Disable() override;

  bool Transmit(const MessageTypePtr& msg, const MessageInfo& msg_info) override;

 private:
  IntraDispatcherPtr dispatcher_;
  uint64_t channel_id_;  
};

template <typename M>
void IntraTransmitter<M>::Enable(){
  if(this->is_enabled_){
    return;
  }
  //SPDLOG_INFO("DEBUG IntraTransmitter<MessageT>::Enable self_id = {}", this->id().ToString());

  dispatcher_ = IntraDispatcher::Instance();
  this->is_enabled_ = true;
}

template <typename M>
void IntraTransmitter<M>::Disable(){
  if (this->is_enabled_) {
    dispatcher_ = nullptr;
    this->is_enabled_ = false;
  }
}

template <typename M>
bool IntraTransmitter<M>::Transmit(const MessageTypePtr& msg, const MessageInfo& msg_info){
  if(!this->is_enabled_){
    SPDLOG_WARN("IntraTransmitter Channel \"{}\" has not been enabled!", this->role_attr_.channel_name());
    return false;
  }
  //SPDLOG_INFO("DEBUG IntraTransmitter<MessageT>::Transmit msg_info.sender_id = {}",
   //    msg_info.sender_id().ToString());
  // 7
  // getchar();
  RETURN_VAL_IF(!dispatcher_, false);
  dispatcher_->OnMessage<M>(channel_id_, msg, msg_info);
  return true;
}


}
}
}


#endif