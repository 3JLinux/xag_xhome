//  Copyright 2020  wenweijie. Sensing & Controling Dept.
/*  XAP(X Application Protocol)消息帧类
 *  帧头 + 帧内容 = 完整一帧
 *  应用头 + 应用命令 + 应用数据(变长) = 帧内容
 *  详情查看XAP协议 */
#pragma once

#include <atomic>

#include "defs.h"

namespace nav_os {
namespace xnet {

constexpr int XAP_HEADER_SIZE = sizeof(XAP_Header_t);
constexpr int XAP_EX_HEADER_SIZE =
    sizeof(XAP_Header_t) + sizeof(XAP_HeaderEx_t);
constexpr int XAP_MINI_SIZE =
    sizeof(XAP_Header_t) + sizeof(XAP_ContentHeader_t);
constexpr int XAP_EX_MINI_SIZE =
    sizeof(XAP_Header_t) + sizeof(XAP_HeaderEx_t) + sizeof(XAP_ContentHeader_t);



class XapFrame {
 public:
  XapFrame();
  XapFrame(const uint8_t *data, int len);
  ~XapFrame();
  XapFrame(const XapFrame &other);
  XapFrame(XapFrame &&other);
  XapFrame &operator=(const XapFrame &other);
  XapFrame &operator=(XapFrame &&other);

  bool valid();
  uint32_t prefix();
  uint16_t protocolVersion();
  FrameType frameType();
  uint16_t createRespondMeta();
  uint16_t sequence();
  uint16_t lenght();
  uint16_t checksum();
  AppHeadType headType();
  uint64_t timestamp();
  uint32_t timeout();
  ;
  uint16_t command();
  int contentDataLen();
  const uint8_t *contentData();
  int data_size();
  const uint8_t *data();

  bool fillIn(const uint8_t *content, uint16_t contentLength,
              FrameType type = FrameType::TYPE_MESSAGE,
              uint16_t sequence = ++Sequence, uint8_t version = XAP_VERSION);

 private:
  void copyConstruct(const XapFrame &other);
  void moveConstruct(XapFrame &&other);

 private:
  bool valid_;
  uint8_t *data_;
  int data_len_;
  XAP_Header_t *header_;
  XAP_HeaderEx_t *header_ext_;
  XAP_ContentHeader_t *content_header_;
  uint8_t *content_data_;
  int content_data_len_;

  static std::atomic<uint16_t> Sequence;
};

// template <typename CmdType>
// inline CmdType XapFrame::commandType() {}

// // 使用本模板函数可以检查帧内容数据长度是否正确
// template <typename ContentDataType>
// inline ContentDataType *XapFrame::contentData() {}

}  // namespace xnet
}  // namespace nav_os
