//  Copyright 2021 wenweijie. Sensing & Controling Dept.
#pragma once

#include <atomic>

#include "defs.h"
#include "node_addr.h"  // NodeAddr

namespace nav_os {
namespace sdk {

class XnetSdk;

typedef struct {
  int id;             // Unique ID
  int src;            // Source Address
  int dest;           // Destination Address
  char subject[128];  // Message Subject
  int size;           // Message Body Size
  char body[0];       // Message Body Buffer
} XIot_Message_t;

typedef struct {
  uint16_t prefix;        // 起始符，字符串“LL”
  uint16_t version;       // 协议版本
  uint16_t sync_id;       // 同步ID
  uint64_t timestamp;     // UTC时间戳（毫秒)
  uint8_t content_type;   // 日志内容格式
                          // 1: binary
                          // 2: protobuf
                          // 3: json
                          // 4: plain text
  uint8_t sender_length;  // 发送者字节长度
  uint8_t sender[0];      // 发送者（如设备ID、其它GUID等）

  uint16_t control_bits;    // 控制位（自定义）
  uint32_t rule_type;       // 规则类型
  uint16_t rule_version;    // 规则版本
  uint32_t content_length;  // content的长度
  uint8_t content[0];       // 日志内容
} __attribute__((packed)) XIot_LogHeader_t;

enum class ContentType : uint16_t {
  TYPE_NONE = 0,
  TYPE_BINARY = 1,
  TYPE_PROTOBUF = 2,
  TYPE_JSON = 3,
  TYPE_PLAIN_TEXT = 4,
};

// class CloudLogHeader {
//  public:
//   CloudLogHeader(const uint8_t *guid, int len, uint32_t ruleType,
//                  uint16_t ruleVersion,
//                  ContentType ctype = ContentType::TYPE_BINARY,
//                  uint16_t version = 1);
//   ~CloudLogHeader();

//  private:
//   uint8_t *class_header_;
//   XIot_LogHeader_t struct_header_;
// };

class CloudLog {
 public:
  CloudLog();
  CloudLog(XnetSdk *node, const uint8_t *guid, int glen, uint32_t ruleType,
           uint16_t ruleVersion, ContentType ctype = ContentType::TYPE_BINARY,
           uint16_t version = 1);
  ~CloudLog();

  // 必须先设置相关信息，guid是设备ID或注册节点guid
  void setInfo(XnetSdk *node, const uint8_t *guid, int glen, uint32_t ruleType,
               uint16_t ruleVersion,
               ContentType ctype = ContentType::TYPE_BINARY,
               uint16_t version = 1);
  int log(const uint8_t *content, int len);

 private:
  XnetSdk *master_;
  uint8_t *guid_;
  XIot_LogHeader_t header_;

  NodeAddr cloud_addr_;

  static std::atomic<uint16_t> Sync_id;
};
}  // namespace sdk
}  // namespace nav_os
