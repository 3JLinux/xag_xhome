#ifndef TRANSPORT_MESSAGE_MESSAGE_WRAPPER_H_
#define TRANSPORT_MESSAGE_MESSAGE_WRAPPER_H_

#include <cstring>
#include <memory>
#include "transport/message/message_wrapper/protobuf_factory.h"
#include "transport/message/message_wrapper/superx_ii_api/superx_ii_api.h"
#include "transport/message/message_wrapper/superx_ii_api/router_message.h"
#include "common/proto/dls_message.pb.h"

namespace xag_nav {
namespace os {
namespace transport {
namespace message{

using ::xag_nav::os::proto::bundle;

enum class Command
{
    CMD_NOMODULE = 0x00,
    CMD_REGISTER = 0x01,
    CMD_HEARTBEAT = 0x02,
    CMD_GET_MODULE = 0x03,
    CMD_GET_DATA = 0x04,
    CMD_SET_DATA = 0x05,
    CMD_SEND_MSG = 0x06,
    CMD_AUTO_LOAD_DATA = 0x07,
    CMD_UPGRADE = 0xf0,
    CMD_UPGRADE_REQUEST = 0xf1,
    CMD_UPGRADE_PK = 0xf2,
    CMD_UPGRADE_FINISH = 0xf3,
    CMD_FC_ONLINE = 0x8001
};

enum class MessageFrom
{
    CLEAR = 0,
    DLS_COM = 1,
    NAV_COM = 2,
    TPS_COM = 3,
    FC_COM = 4,
    UDP_COM = 5 ,
    F8_ROUTER = 6,
    OLD_ROUTER = 7,
    NAV_OS_REGISTER_COM = 8,
    RC_APP_COM = 9,
    SPARY_APP_COM = 10,
};

enum class MessageType
{
    NONE = 0,
    NET_MESSAGE = 1,
    API_MESSAGE = 2,
    API_NET_MESSAGE = 4,
    MODULE_LIST = 8,
};

union message_t{
  Net_Message_t net_message_;
  API_Message_t api_message_;
  API_Net_Message_t api_net_message_;
} ;
class MessageWrapper
{
  public:
    explicit MessageWrapper();
    explicit MessageWrapper(const MessageWrapper& other);
    MessageWrapper& operator=(const MessageWrapper& other);
    explicit MessageWrapper(const Net_Message_t& _message);
    explicit MessageWrapper(const API_Message_t& _message);
    explicit MessageWrapper(const API_Net_Message_t& _message);
    explicit MessageWrapper(const bundle& _message);
    explicit MessageWrapper(const uint8_t* _modules_list);
    explicit MessageWrapper(const uint8_t* buffer,uint16_t length);
    explicit MessageWrapper(const Command _cmd);
    ~MessageWrapper();
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

    int ByteSize() const;
    bool SerializeToString(std::string *str) const;
    bool SerializeToArray(void *data, int size) const;
    bool ParseFromString(const std::string &_str);
    bool ParseFromArray(const void *data, int size);

    int SerializeToProtobufArray(uint8_t* _array, int _size);

    bool message_empty() const {return type_ == MessageType::NONE || type_ == MessageType::MODULE_LIST;}
    enum MessageType message_type() const {return type_;}
    //先获取Message类型再作转换，非必要时不推荐使用
    const void* message() const {return &message_;}
    uint32_t message_From() const
     {
       return (static_cast<int>(type_) & 0x01) ? message_.net_message_.from_node : 
                      (static_cast<int>(type_) & 0x02) ? message_.api_message_.From :
                      (static_cast<int>(type_) & 0x04) ? message_.api_net_message_.From :
                      0;
       }
    uint32_t message_From_Node() const
    {
      return (static_cast<int>(type_) & 0x01) ? message_.net_message_.from_node : 
                  (static_cast<int>(type_) & 0x02) ? 0 :
                  (static_cast<int>(type_) & 0x04) ? message_.api_net_message_.From_Node :
                  0;
    }
    uint32_t message_To() const 
    {
      return (static_cast<int>(type_) & 0x01) ? message_.net_message_.to_node : 
                      (static_cast<int>(type_) & 0x02) ? message_.api_message_.To :
                      (static_cast<int>(type_) & 0x04) ? message_.api_net_message_.To :
                      0;
    }
    uint32_t message_To_Node() const
    {
      return (static_cast<int>(type_) & 0x01) ? message_.net_message_.to_node : 
                      (static_cast<int>(type_) & 0x02) ? 0 :
                      (static_cast<int>(type_) & 0x04) ? message_.api_net_message_.To_Node :
                      0;
    }
    uint16_t message_Length() const
    {
      return (static_cast<int>(type_) & 0x01) ? message_.net_message_.length : 
                      (static_cast<int>(type_) & 0x02) ? message_.api_message_.Length :
                      (static_cast<int>(type_) & 0x04) ? message_.api_net_message_.Length :
                      0;
    }
    uint8_t message_version() const
    {
      return (static_cast<int>(type_) & 0x01) ? DRAW_VER(message_.net_message_.ver_cmd) : 
                      (static_cast<int>(type_) & 0x02) ? message_.api_message_.version :
                      (static_cast<int>(type_) & 0x04) ? message_.api_net_message_.version :
                      0;
    }
    uint8_t* message_Data() const
    {
      return (static_cast<int>(type_) & 0x01) ? message_.net_message_.payload : 
                      (static_cast<int>(type_) & 0x02) ? message_.api_message_.Data :
                      (static_cast<int>(type_) & 0x04) ? message_.api_net_message_.Data :
                      (uint8_t*)0;
    }
    uint8_t message_Data_at(std::size_t i) const 
    {
      if (message_Data() == nullptr || i > message_Length()) return 0;
      else  return message_Data()[i];
    }
    void set_message_Data_ptr(uint8_t* _data)
    {
      (static_cast<int>(type_) & 0x01) ? (message_.net_message_.payload = _data) : 
      (static_cast<int>(type_) & 0x02) ? (message_.api_message_.Data  = _data) :
      (static_cast<int>(type_) & 0x04) ? (message_.api_net_message_.Data  = _data) :
      0;
    }
    void show_message_data()
    {
      printf("message data: ");
      for (size_t i = 0; i < message_Length(); i++)
      {
        printf("%02x ",message_Data_at(i));
      }
      printf("\n");
    }
    uint8_t net_message_cmd() const {return  
                          (static_cast<int>(type_) & 0x01) ? DRAW_CMD(message_.net_message_.ver_cmd) : 
                          0;
                          }
    uint8_t message_request_id() const
    {
      return (static_cast<int>(type_) & 0x01) ? message_.net_message_.request_id : 
                      (static_cast<int>(type_) & 0x02) ? 0 :
                      (static_cast<int>(type_) & 0x04) ? message_.api_net_message_.Request_Id :
                      0;
    }
    uint8_t message_respond_id() const
    {
      return (static_cast<int>(type_) & 0x01) ? message_.net_message_.respond_id : 
                      (static_cast<int>(type_) & 0x02) ? 0 :
                      (static_cast<int>(type_) & 0x04) ? message_.api_net_message_.Respond_Id :
                      0;
    }
    bool convert_to_api_message();
    bool convert_to_net_message();
    bool convert_to_api_net_message();
    bool convert_to_net_message_by_old_api_message(uint32_t from_node,uint32_t to_node,uint8_t cmd,uint16_t length,uint32_t to_from);
    bool convert_to_old_api_message_by_net_message(uint32_t To, uint32_t From);
    bool convert_to_old_api_message_by_net_message(uint8_t To, uint8_t From);
    void set_sender(const MessageFrom& _who) {who_ = _who;}
    MessageFrom message_sender() const {return who_;}
    bool module_list_empty() const {return modules_list_ == nullptr;}
    uint8_t* modules_list() const {return modules_list_;}
    bool cmd_enpty() const {return cmd_ == 0;}
    uint32_t cmd() const { return cmd_; }
    static std::string TypeName() {return "xag_nav::os::transport::message::MessageWrapper";}
    void set_timestamp(uint64_t seconds){timestamp_s = seconds;}
    uint64_t get_timestamp(){ return timestamp_s;}
    enum MessageType type_;
    enum MessageFrom who_; 
    message_t message_;
    uint8_t* modules_list_;
    uint32_t cmd_;
    uint64_t timestamp_s;
  private:
    // enum MessageType type_;
    // enum MessageFrom who_; 

    // union {
    //   Net_Message_t net_message_;
    //   API_Message_t api_message_;
    //   API_Net_Message_t api_net_message_;
    // } message_;
    // uint8_t* modules_list_;
    // uint32_t cmd_;
};

}   //namespace message
}   //namespace transport
}   //namespace os
}   //namespace xag_nav
#endif
