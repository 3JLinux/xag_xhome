#ifndef __TRANSPORT_WRITER_H__
#define __TRANSPORT_WRITER_H__
#include <memory>
#include <string>
#include <atomic>

#include "spdlog/spdlog.h"
#include "transport/writer_base.h"
#include "transport/transport.h"
#include "transport/dispatcher/dispatcher.h"
#include "transport/transmitter/transmitter.h"
#include "common/proto/role_attributes.pb.h"
#include "common/proto/topology_change.pb.h"
#include "transport/topology_manager/topology_manager.h"

namespace xag_nav {
namespace os {

/**
 * @class Writer<M>
 * @brief The Channel Writer has only one function: publish message through the
 * channel pointed in its RoleAttributes
 * @tparam M Message Type of the Writer handles
 */
template <typename M>
class Writer : public WriterBase{
  using MessageTypePtr = std::shared_ptr<M>;
  using TransmitterPtr = std::shared_ptr<transport::Transmitter<M>>;
  using ChangeConnection =
      typename transport::Manager::ChangeConnection;
 public:
  Writer() = delete;
  /**
   * @brief Construct a new Writer object
   *
   * @param role_attr we use RoleAttributes to identify a Writer
   */
  explicit Writer(const RoleAttributes& role_attr)
        : WriterBase(role_attr), channel_manager_(nullptr){}

  Writer(const Writer& other)
  {
    this->role_attr_ = other.role_attr_;
    this->is_enable_.store(other.is_enable_.load());
  }

  // Writer& operator=(const Writer& other)
  // {
  //   role_attr_ = other.role_attr_;
  //   is_enable_.store(other.is_enable_.load());
  // }
  virtual ~Writer() { Disable(); }

   /**
   * @brief Enable the Writer
   *
   * @return true if init successfully
   * @return false if init failed
   */
  virtual bool Enable() override; 
  /**
   * @brief Disable the Writer
   */
  virtual void Disable() override; 
  virtual bool Transmit(const MessageTypePtr& msg) ;
  virtual bool Transmit(const M& msg) ;
  virtual bool HasReader() override;
  virtual void GetReaders(std::vector<RoleAttributes>* readers) override;
 
 private:
  
  void JoinTheTopology();
  void LeaveTheTopology();
  void OnChannelChange(const proto::ChangeMsg& change_msg);

  ChangeConnection change_conn_;
  transport::ChannelManagerPtr channel_manager_;
  TransmitterPtr transmitter_;
};


template <typename MessageT>
bool Writer<MessageT>::Enable(){
  if(this->is_enable_.load()){
    return true;
  }
  transmitter_ = transport::Transport::Instance()->CreateTransmitter<MessageT>(this->role_attr_);
  if (transmitter_ == nullptr) {
    return false;
  }
  
  this->role_attr_.set_id(transmitter_->id().HashValue());
  channel_manager_ = transport::TopologyManager::Instance()->channel_manager();
  JoinTheTopology();
  this->is_enable_.store(true);
  return true;
}


template <typename MessageT>
void Writer<MessageT>::Disable(){
	if(!this->is_enable_.exchange(false)){
		return;
	}
  LeaveTheTopology();
	transmitter_ = nullptr;
}

template <typename MessageT>
bool Writer<MessageT>::Transmit(const MessageT& msg) {
  RETURN_VAL_IF(!WriterBase::IsEnable(), false);
  auto msg_ptr = std::make_shared<MessageT>(msg);
  return Transmit(msg_ptr);
}

template <typename MessageT>
bool Writer<MessageT>::Transmit(const std::shared_ptr<MessageT>& msg_ptr) {
  RETURN_VAL_IF(!WriterBase::IsEnable(), false);
  return transmitter_->Transmit(msg_ptr);
}

template <typename MessageT>
void Writer<MessageT>::OnChannelChange(const proto::ChangeMsg& change_msg) {
  if (change_msg.role_type() != proto::RoleType::ROLE_READER) {
    return;
  }

  auto& reader_attr = change_msg.role_attr();
  if (reader_attr.channel_name() != this->role_attr_.channel_name()) {
    return;
  }

  auto operate_type = change_msg.operate_type();
  if (operate_type == proto::OperateType::OPT_JOIN) {
    transmitter_->Enable(reader_attr);
  } else {
    transmitter_->Disable(reader_attr);
  }
}


template <typename MessageT>
void Writer<MessageT>::JoinTheTopology(){
 // add listener
  change_conn_ = channel_manager_->AddChangeListener(std::bind(
      &Writer<MessageT>::OnChannelChange, this, std::placeholders::_1));

  // get peer readers
  const std::string& channel_name = this->role_attr_.channel_name();
  std::vector<proto::RoleAttributes> readers;
  channel_manager_->GetReadersOfChannel(channel_name, &readers);
  for (auto& reader : readers) {
    transmitter_->Enable(reader);
  }
  channel_manager_->Join(this->role_attr_, proto::RoleType::ROLE_WRITER);
}

template <typename MessageT>
void Writer<MessageT>::LeaveTheTopology() {
  channel_manager_->RemoveChangeListener(change_conn_);
  channel_manager_->Leave(this->role_attr_, proto::RoleType::ROLE_WRITER);
}



template <typename MessageT>
bool Writer<MessageT>::HasReader() {
  RETURN_VAL_IF(!WriterBase::IsEnable(), false);
  return channel_manager_->HasReader(role_attr_.channel_name());
}


template <typename MessageT>
void Writer<MessageT>::GetReaders(std::vector<proto::RoleAttributes>* readers) {
  if (readers == nullptr) {
    return;
  }

  if (!WriterBase::IsEnable()) {
    return;
  }

  channel_manager_->GetReadersOfChannel(role_attr_.channel_name(), readers);
}



}
}

#endif