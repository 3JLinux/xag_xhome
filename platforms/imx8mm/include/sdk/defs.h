//  Copyright 2020  wenweijie Sensing & Controling Dept.
#pragma once

#include <inttypes.h>
#include <netinet/in.h>

#include <iostream>

namespace nav_os {
namespace xnet {

// DLS通讯参数
#define PACKAGE_SIZE 3072  // IOT包最大不过3K，过大会被服务器过滤掉
#define XNET_TO_DLS_PORT 6767  // 该端口包含注册和通信
#define DEFAULT_SUBJECT "XNET"

// Node通讯端口
#define DLS_TO_XNET_PORT 5554
#define MODULE_TO_XNET_REGISTER_PORT 5555
#define XNET_TO_ROUTER_SYNC_PORT 5556  // 用于同步外部客户端注册
#define XNET_TO_MODULE_COM_PORT 5558

#define RESPONE_STATUS_OK 1
#define RESPONE_STATUS_ON 0

// Xnet
#define XET_IP_STR "192.168.234.1"

// CloudLog
#define CLOUD_LOG_SUBJECT "UNIVERLOG"
#define CLOUD_LOG_PORT 6767

// Xap
enum class FrameType {
  TYPE_NONE = 0,
  TYPE_REQUEST = 1,
  TYPE_RESPOND = 2,
  TYPE_MESSAGE = 3
};

#define XAP_PREFIX 0xc4c4c4c4
#define XAP_VERSION 0x01
#define XAP_META(ver, type) ((ver & 0x7) | ((type & 0x0F) << 3))
#define IS_TIMEOUT_META(meta) (meta & 0x80)
#define FRAME_TYPE_MESSAGE static_cast<uint16_t>(FrameType::TYPE_MESSAGE)
#define FRAME_TYPE_REQUEST static_cast<uint16_t>(FrameType::TYPE_REQUEST)
#define FRAME_TYPE_RESPOND static_cast<uint16_t>(FrameType::TYPE_RESPOND)
#define IS_FRAME_TYPE_MESSAGE(meta) (meta & (FRAME_TYPE_MESSAGE << 3))

enum class SdkErrCode {
  ERR_NONE,
  ERR_LONG_NAME = 400,
  ERR_CREATE_SOCKET = 401,
  ERR_EPOLL_CREATE = 402,
  ERR_EPOLL_ADD = 403,
  ERR_ON_REGISTER = 404,
  ERR_EMTPY_IOT_CB = 405,
  ERR_EMTPY_NODE_CB = 406,
  ERR_REG_SAME_NAME = 407,
  ERR_REG_SAME_PORT = 408,
  ERR_INOTIFY_INIT = 409,
  ERR_TIMERFD_INIT = 410,
  ERR_NODE_INEXISTENCE = 411,
  ERR_INVALID_DATA = 412,
};

typedef struct {
  struct sockaddr_in client_addr;
  int len;
} Node_UdpInfo_t;

typedef struct {
  uint32_t prefix;  // 起始符0xc4c4c4c4
  uint16_t meta;  // 元数据，bit0~2:协议版本 bit3~6:帧类型 1.请求 2.响应 3.消息
                  // bit7~15:保留
  uint16_t sequence;  // 序号
                      // 帧类型为请求响应时，响应方返回与请求方一致的值
                      // 帧类型为消息时，由发送方定义，采用自增方式
  uint16_t length;    // 帧内容长度
  uint16_t checksum;  // CRC16(content)
  uint8_t header_ext[0];
  uint8_t content[0];
} XAP_Header_t;

typedef struct {
  uint64_t timestamp;  // 帧发送时间，UTC毫秒级时间戳，单位：毫秒
                       // timestamp为0，响应端无需进行超时判断
  uint32_t timeout;  // 帧超时时间，单位：毫秒
                     // timeout为0，表示由响应端定义超时时间
} __attribute__((packed)) XAP_HeaderEx_t;

typedef struct {
  uint16_t head;  // 应用头(必要字段)，用于识别当前内容属于哪种应用
  uint16_t command;  // 应用命令
  uint8_t data[0];   // 应用命令数据
} XAP_ContentHeader_t;

enum class AppHeadType : uint16_t {
  // 0x0000 ~ 0x0FFF 系统应用节点
  HEAD_NONE,
  HEAD_XNET,
  HEAD_XPACK,
  HEAD_XSAFE,

  // 0x1000 ~ 0x3FFF 应用通信节点
  HEAD_ROUTER = 0x1001,  // 控制器网络
  HEAD_TPS = 0x1002,     // 任务处理
  HEAD_RC = 0x1003,      // 遥控
  HEAD_NAV = 0x1004,     // 导航
  HEAD_DLS = 0x1005,     // DLS

  HEAD_SPRAY = 0x2001,   // 喷洒
  HEAD_FLYMAP = 0x2002,  // 飞图
  HEAD_MS = 0x2003,      // MapService
  HEAD_FPV = 0x2004,     // FPV
  HEAD_BMS = 0x2005,     // 电池管理系统

  // 0x8000 ~ 0x8FFF
  // 测试或自定义通信节点
};

enum class DownloadState : uint16_t {
  DOWNLOAD_IDLE = 0,
  DOWNLOADING,
  DOWNLOAD_COMPLETED,
  DOWNLOAD_FAIL,
};

enum class XnetCMD : uint16_t {
  CMD_NONE,
  CMD_NODE_REGISTER,
  CMD_NODE_HEARTBEAT,
  CMD_GET_NODE_LIST,
  CMD_GET_SYS_STATUS,
  CMD_HARDWARE_REGISTER = 0x06,

  CMD_GET_VERSION = 0x0101,
  CMD_DOWNLOAD_XPK,
  CMD_DOWNLOAD_FEEDBACK,
  CMD_UPGRADE_XPK,
  CMD_UPGRADE_FEEDBACK,
};

// 0x0001 注册通信节点
typedef struct {
  // uint16_t head;    // 0x0001
  // uint16_t command; // 0x0001
  uint8_t guid[16];      // 节点唯一ID
  char name[16];         // 节点名称(IOT通信用)
  uint32_t ip;           // 网络地址
  uint16_t port;         // 网络端口(MESH通信用)
  uint16_t type;         // 端口协议类型
                         // 0x00: 内容走F8飞控协议
                         // 0x01: tcp 协议
                         // 0x02: udp 协议
                         // 0x03: tcp+udp 协议
                         // 0x04: kcp 协议
                         // 0x05: http 协议
  uint16_t router_type;  // 飞控网络模块类型
  uint16_t router_index;
  char pkg_name[32];  // 片外应用信息管理
  char app_name[32];
  uint16_t app_type;
  uint32_t version_code;
  char version_name[16];
} Xnet_NodeRegisterRequest_t;

enum class NodeProtocalType : uint16_t {
  PROTOCAL_F8 = 0,
  PROTOCAL_TCP,
  PROTOCAL_UDP,
  PROTOCAL_TCP_UDP,
  PROTOCAL_KCP,
  PROTOCAL_HTTP,
};

static char ProtocolType[][8]{"F8", "TCP", "UDP", "TCP+UDP", "KCP", "HTTP"};

typedef struct {
  // uint16_t head;      // 0x0001
  // uint16_t command;   // 0x0001
  uint16_t status;  // 状态: 0:失败 1:成功
  uint16_t reserved;
  uint8_t guid[16];  // 节点唯一ID
} Xnet_NodeRegisterRespond_t;

enum class RegisterState : uint16_t {
  SUCCESS = 1,
  FAIL_SAME_GUID = 401,
  FAIL_SAME_NAME = 402,
  FAIL_SAME_PORT = 403,
};

enum class RespondStatus {
  STATE_FAIL,
  STATE_SUCCESS,
};

// 0x0002 节点心跳  消息和请求类型都能识别
typedef struct {
  // uint16_t head;      // 0x0001
  // uint16_t command;   // 0x0002
  uint8_t guid[16];  // 节点唯一ID
} Xnet_NodeHeartBeat_t;

// 0x0003 获取通信节点列表
typedef struct {
  // uint16_t head;         // 0x0001
  // uint16_t command;      // 0x0003
} Xnet_NodeGetListRequest_t;

typedef struct {
  // uint16_t head;         // 0x0001
  // uint16_t command;      // 0x0003
  uint16_t node_count;  // 节点数量
  uint16_t reserved;
  uint8_t nodes[0];
} Xnet_NodeGetListRespond_t;

typedef struct {
  uint8_t guid[16];  // 节点唯一ID
  char name[16];     // 节点名称
  uint32_t ip;       // 网络地址
  uint16_t port;     // 网络端口
  uint16_t type;     // 端口协议类型
                     // 0x00: 内容走F8飞控协议
                     // 0x01: tcp 协议
                     // 0x02: udp 协议
                     // 0x03: tcp+udp 协议
                     // 0x04: kcp 协议
                     // 0x05: http 协议
  uint16_t router_type;
  uint16_t router_index;
  uint16_t status;  // 通信节点状态 0x00:离线 0x01:在线
  uint16_t reserved;
} Xnet_Node_t;

enum class NodeState : uint16_t {
  STATE_OFF_LINE,
  STATE_ON_LINE,
};

// 0x0004 系统状态消息  定时向通信节点发送系统状态消息
typedef struct {
  // uint16_t head;        // 0x0001
  // uint16_t command;     // 0x0004
  uint32_t version;     // XNET版本
  uint32_t sys_time;    // 系统时间
  uint16_t platform;    // 系统平台
  uint16_t cpu;         // CPU使用率
  uint16_t cpu_temp;    // CPU温度
  uint16_t memory;      // 内存使用率
  uint32_t sys_status;  // 系统状态
                        // 100：正常
                        // 3001：升级包下载中
                        // 3002：升级包下载完毕
                        // 3003：升级包下载失败
                        // 3004：升级中
                        // 3005：升级完毕
                        // 3006：升级失败
  uint32_t storage;     // 剩余存储空间，单位: MB
} Xnet_SystemStatus_t;

// 0x0005 系统事件
typedef struct {
  uint32_t event_id;       // 事件ID
  uint32_t event_time;     // 事件发生Unix时间戳, 单位: 秒
  uint32_t event_time_ms;  // 事件发生时间的毫秒, 单位: 毫秒
  uint16_t event_from;     // 事件来源（节点head）
  uint16_t event_to;  // 事件目标（节点head），当等于0时，表示所有节点
  uint32_t event_params_length;  // 事件附加内容长度（4字节对齐）
  uint8_t event_params[0];       // 事件附加内容
} Xnet_SystemEvent_t;

// 0x0102 下载固件
typedef struct {
  // uint16_t head;                   // 0x0001
  // uint16_t command;                // 0x0102
  uint16_t firmware_name_length;  // 固件文件名长度
  uint16_t reserved;
  uint8_t firmware_name[128];  // 固件文件名(ASCII编码)
  uint8_t firmware_hash[32];   // MD5哈希值（ASCII编码）
  uint32_t uri_size;           // uri字符串长度
  uint8_t uri[];               // 资源字符串(ASCII编码)
} Xnet_DownloadfirmwareRequest_t;
typedef struct {
  // uint16_t head;		// 0x0001
  // uint16_t command;	// 0x0102
  uint8_t guid[16];  // 节点唯一ID
  uint16_t fid;      // 本次请求操作符
  uint16_t status;   // 状态码，0:失败，1:成功
  uint16_t error;    // 内部错误码，失败时有效
  uint16_t reserved;
} Xnet_DownloadfirmwareRespond_t;

// 0x0103 下载进度反馈
typedef struct {
  // uint16_t head;		// 0x0001
  // uint16_t command;	// 0x0103
  uint8_t guid[16];  // 节点唯一ID
  uint16_t fid;      // 本次请求操作符
  uint16_t status;  // 状态，0:空闲，1:下载中，2：下载完成，3:下载出错
  uint16_t error;     // 内部错误码，失败时有效
  uint16_t progress;  // 下载进度, 单位：1%
} Xnet_DownloadProgressMessage_t;

// 0x0104 升级
typedef struct {
  // uint16_t head;     // 0x0001
  // uint16_t command;  // 0x0104
  uint16_t fid;
  uint16_t reserved;
} Xnet_UpgradeRequest_t;
typedef struct {
  //  uint16_t head;       // 0x0001
  //  uint16_t command;    // 0x0104
  uint8_t guid[16];  // 节点唯一ID
  uint16_t fid;      // 本次请求操作符
  uint16_t status;   // 状态码，0:失败，1:成功
  uint16_t error;    // 内部错误码，失败时有效
  uint16_t reserved;
} Xnet_UpgradeRepond_t;

// 0x0105 升级进度反馈
typedef struct {
  // uint16_t head;     // 0x0001
  // uint16_t command;  // 0x0105
  uint8_t guid[16];  // 节点唯一ID
  uint16_t fid;      // 本次请求操作符
  uint16_t status;  // 状态，0:空闲，1:升级中，2：升级完成，3:升级出错
  uint16_t error;     // 内部错误码，失败时有效
  uint16_t progress;  // 升级进度, 单位：0.1%
} Xnet_UpgradeMessage_t;

}  // namespace xnet
}  // namespace nav_os
