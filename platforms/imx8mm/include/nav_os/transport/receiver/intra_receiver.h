#pragma once
#ifndef __TRANSPORT_RECEIVER_INTRA_RECEIVER_H_
#define __TRANSPORT_RECEIVER_INTRA_RECEIVER_H_
#include <unordered_map>
#include "transport/receiver/receiver.h"
#include "transport/dispatcher/dispatcher.h"
#include "transport/dispatcher/intra_dispatcher.h"
#include "spdlog/spdlog.h"

namespace xag_nav {
namespace os {
namespace transport{

template <typename MessageT>
class IntraReceiver : public Receiver<MessageT>{
public:
	/**
	 * Constructor a Reader object.
	 * @param _channel_name is the channel's unique name
	 * @param _reader_func is the callback function, when the message is received.
	 */
	explicit IntraReceiver(const RoleAttributes& role_attr,
									const typename Receiver<MessageT>::MessageListener& msg_listener)
					 :Receiver<MessageT>(role_attr, msg_listener){
		dispatcher_ = IntraDispatcher::Instance();
	}

	virtual ~IntraReceiver(){ Disable(); }

	void Enable() override;

	void Disable() override;

	void Enable(const proto::RoleAttributes& oppo_attrs) override;

	void Disable(const proto::RoleAttributes& oppo_attrs) override;

private:
  IntraDispatcherPtr dispatcher_;
	std::unordered_map<uint64_t, RoleAttributes> transmitters_;
};

template <typename MessageT>
void IntraReceiver<MessageT>::Enable() {
	if (this->is_enabled_) {
		return;
	}
	// SPDLOG_INFO("DEBUG IntraReceiver<MessageT>::Enable self_id = {}", this->id().ToString());
	// 不能进行绑定 
	// dispatcher_->AddListener<MessageT>(this->role_attr_,
	// 			std::bind(&IntraReceiver<MessageT>::OnNewMessage,this,
  //         				std::placeholders::_1, std::placeholders::_2));
	this->is_enabled_ = true;
}

template <typename MessageT>
void IntraReceiver<MessageT>::Disable() {
	if (!this->is_enabled_) {
		return;
	}
	dispatcher_->RemoveListener<MessageT>(this->role_attr_);
	
	for (auto& upper_reach : transmitters_) {
    Disable(upper_reach.second);
  }
	this->is_enabled_ = false;
}

template <typename MessageT>
void IntraReceiver<MessageT>::Enable(const proto::RoleAttributes& oppo_attrs) {
	// SPDLOG_INFO("DEBUG IntraReceiver<MessageT>::Enable self_id = {} oppo_id = {}",
			// this->id().ToString(), oppo_attrs.id();
	uint64_t id = oppo_attrs.id();
	if(transmitters_.count(id) == 0){
		transmitters_.insert(std::make_pair(id, oppo_attrs));
	} else {
		SPDLOG_WARN("more than one time Enable the same channel_name, so will be ignore");
		return ;
	}
	dispatcher_->AddListener<MessageT>(
      this->role_attr_, oppo_attrs,
      std::bind(&IntraReceiver<MessageT>::OnNewMessage, this, std::placeholders::_1,
                std::placeholders::_2));
}

template <typename MessageT>
void IntraReceiver<MessageT>::Disable(const proto::RoleAttributes& oppo_attrs) {
	dispatcher_->RemoveListener<MessageT>(this->role_attr_, oppo_attrs);
}

}
}
}

#endif //__TRANSPORT_RECEIVER_INTRA_RECEIVER_H_