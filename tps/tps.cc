#include <string.h>
#include <stdio.h>
#include "tps.h"

#define XAP_META(ver, type) ((ver & 0x7) | ((type & 0x0F) << 3)) 

static tps_mission_rcv_t tps_mission_mes;	//任务数据信息


/************************************************************
 * 函数原型：	void tps_mission_rcv_handle(char* rcvBuf)
 * 功能：		tps任务接收处理，将接收的任务通知数据存到全局变量
 * 输入：		char* rcvBuf 接收数据指针
 * 输出：		void
 * *********************************************************/
void tps_mission_rcv_handle(char* rcvBuf)
{
    tps_mission_rcv_t* buf = (tps_mission_rcv_t*) rcvBuf;
    tps_mission_mes.mission_type = buf->mission_type;
    tps_mission_mes.mission_source = buf->mission_source;
    memcpy(tps_mission_mes.mission_id, buf->mission_id, 16);

    tps_mission_mes.file_name_length = buf->file_name_length;
    memcpy(tps_mission_mes.file_name, buf->file_name, 128);

    tps_mission_mes.field_name_length = buf->field_name_length;
    memcpy(tps_mission_mes.field_name, buf->field_name, 128);
}


/************************************************************
 * 函数原型：	static int32_t tps_startMission_fill(char* buf, tps_mission_rcv_t* tps_rcv_buf)
 * 功能：		tps开启任务命令数据填充
 * 输入：		char* buf 缓冲区指针
 *				tps_mission_rcv_t* tps_rcv_buf 任务信息指针
 * 输出：		int32_t 返回填充后数据长度
 * *********************************************************/
static int32_t tps_startMission_fill(char* buf, tps_mission_rcv_t* tps_rcv_buf)
{
    tps_mission_start_t* sendBuf = (tps_mission_start_t*)buf;
    sendBuf->head = TPS_HEAD;
    sendBuf->command = TPS_CMD_M_START;
    memcpy(sendBuf->mission_id,tps_rcv_buf->mission_id, 16);
    sendBuf->mission_seq = 0;
    sendBuf->mission_type = tps_rcv_buf->mission_type;
    return sizeof(tps_mission_start_t);
} 


/************************************************************
 * 函数原型：	static int32_t tps_pauseMission_fill(char* buf)
 * 功能：		tps任务暂停数据填充
 * 输入：		char* buf 缓冲区指针
 * 输出：		int32_t 返回填充后数据长度
 * *********************************************************/
static int32_t tps_pauseMission_fill(char* buf)
{
    tps_mission_pause_t* sendBuf = (tps_mission_pause_t*)buf;
    sendBuf->head = TPS_HEAD;
    sendBuf->command = TPS_CMD_M_PAUSE;
    return sizeof(tps_mission_pause_t);
}


/************************************************************
 * 函数原型：	static int32_t tps_resumeMission_fill(char* buf, tps_mission_rcv_t* tps_rcv_buf, uint16_t mode)
 * 功能：		tps任务恢复数据填充
 * 输入：		char* buf 缓冲区指针
 *				tps_mission_rcv_t* tps_rcv_buf 任务信息指针
 *				uint16_t mode 恢复方式 0：自动 1：恢复Paused的任务 2：恢复Suspend的任务
 * 输出：		int32_t 返回填充后数据长度
 * *********************************************************/
static int32_t tps_resumeMission_fill(char* buf, tps_mission_rcv_t* tps_rcv_buf, uint16_t mode)
{
    tps_mission_resume_t* sendBuf =(tps_mission_resume_t*)buf;
    sendBuf->head = TPS_HEAD;
    sendBuf->command = TPS_CMD_M_RESUME;
    sendBuf->mode = mode;
    if(mode == 2)
    {
        memcpy(sendBuf->mission_id, tps_rcv_buf->mission_id, 16);
        sendBuf->mission_seq = 0;
    }
    return sizeof(tps_mission_resume_t);
}


/************************************************************
 * 函数原型：	static int32_t tps_getMissionStatus_fill(char* buf)
 * 功能：		tps获取任务状态数据填充
 * 输入：		char* buf 缓冲区指针
 * 输出：		int32_t 返回填充后数据长度
 * *********************************************************/
static int32_t tps_getMissionStatus_fill(char* buf)
{
    tps_mission_get_status_t* sendBuf = (tps_mission_get_status_t*)buf;
    sendBuf->head = TPS_HEAD;
    sendBuf->command = TPS_CMD_M_GET_STATUS;
    return sizeof(tps_mission_get_status_t);
}


/************************************************************
 * 函数原型：	static int32_t tps_landing_fill(char* buf)
 * 功能：		tps降落命令数据填充
 * 输入：		char* buf 缓冲区指针
 * 输出：		int32_t 返回填充后数据长度
 * *********************************************************/
static int32_t tps_landing_fill(char* buf)
{
    tps_landing_t* sendBuf = (tps_landing_t*)buf;
    sendBuf->head = TPS_HEAD;
    sendBuf->command = TPS_CMD_LANDING;
    return sizeof(tps_landing_t);
}


/************************************************************
 * 函数原型：	static int32_t tps_loadMission_fill(char*buf, tps_mission_rcv_t* tps_rcv_buf)
 * 功能：		tps任务加载命令数据填充
 * 输入：		char* buf 缓冲区指针
 *				tps_mission_rcv_t* tps_rcv_buf 任务信息指针
 * 输出：		int32_t 返回填充后数据长度
 * *********************************************************/
static int32_t tps_loadMission_fill(char*buf, tps_mission_rcv_t* tps_rcv_buf)
{
    tps_mission_load_t* sendBuf = (tps_mission_load_t*)buf;
    sendBuf->head = TPS_HEAD;
    sendBuf->command = TPS_CMD_M_LOAD;
    sendBuf->mission_type = tps_rcv_buf->mission_type;
    sendBuf->mission_source = tps_rcv_buf->mission_source;
    memcpy(sendBuf->mission_id, tps_rcv_buf->mission_id, 16);
    sendBuf->file_name_length = tps_rcv_buf->file_name_length;
    memcpy(sendBuf->file_name, tps_rcv_buf->file_name, 128);
    sendBuf->field_name_length = tps_rcv_buf->field_name_length;
    memcpy(sendBuf->field_name, tps_rcv_buf->field_name, 128);
    return sizeof(tps_mission_load_t);
}


/************************************************************
 * 函数原型：	static int32_t tps_getRouteMissionStatus_fill(char* buf)
 * 功能：		获取航线任务命令数据填充
 * 输入：		char* buf 缓冲区指针
 * 输出：		int32_t 返回填充后数据长度
 * *********************************************************/
static int32_t tps_getRouteMissionStatus_fill(char* buf)
{
    tps_mission_route_get_t* sendBuf = (tps_mission_route_get_t*)buf;
    sendBuf->head = TPS_HEAD;
    sendBuf->command = TPS_CMD_M_GET_ROUTE_STATUS;
    return sizeof(tps_mission_route_get_t);
}


/************************************************************
 * 函数原型：	static int32_t tps_changeNetwork_fill(char* buf)
 * 功能：		tps切换网络命令数据填充
 * 输入：		char* buf 缓冲区指针				
 * 输出：		int32_t 返回填充后数据长度
 * *********************************************************/
static int32_t tps_changeNetwork_fill(char* buf)
{
    tps_change_network_t* sendBuf = (tps_change_network_t*)buf;
    sendBuf->head = TPS_HEAD;
    sendBuf->command = TPS_CMD_NETWORK_CHANGE;
    sendBuf->port = TPS_SEND_PORT;
    return sizeof(tps_change_network_t);
}


/************************************************************
 * 函数原型：	static int32_t xapFrame_fill(char* buf)
 * 功能：		xap协议头数据填充
 * 输入：		char* buf 缓冲区指针	
 * 输出：		int32_t 返回填充后数据长度
 * *********************************************************/
static int32_t xapFrame_fill(char* data, int32_t dataLen, char* xap_sendBuf, uint16_t meta, uint16_t sequence)
{
    XAP_FRAME* xap_frame = (XAP_FRAME*)xap_sendBuf;
    xap_frame->prefix = XAP_PREFIX;
    xap_frame->meta = meta;
    xap_frame->sequence = sequence;
    xap_frame->length = dataLen;
    xap_frame->checksum = 0;
    memcpy(xap_frame->data, data, dataLen);
    return dataLen+12;
}

/************************************************************
 * 函数原型：	int32_t TPS_sendData(void (*tps_send)(char* data, int32_t dataLen), uint16_t cmd)
 * 功能：		tps数据发送
 * 输入：		void (*tps_send)(char* data, int32_t dataLen) tps底层发送回调函数
 *				uint16_t cmd 命令
 * 输出：		int32_t 返回发送的数据长度
 * *********************************************************/
int32_t TPS_sendData(void (*tps_send)(char* data, int32_t dataLen), 
        uint16_t cmd)
{
    int32_t len;
    char sendBuf[1024] = {0};
    char xap_sendBuf[1024] = {0};
    static uint16_t seq = 0;
    //XAP_FRAME* xap_frame = (XAP_FRAME*)xap_sendBuf;
    memset(sendBuf, 0, 1024);
    switch(cmd)
    {
        case TPS_CMD_M_START:
        len = tps_startMission_fill(sendBuf,&tps_mission_mes);
        break;
        case TPS_CMD_M_PAUSE:
        len = tps_pauseMission_fill(sendBuf);
        break;
        case TPS_CMD_M_GET_STATUS:
        len = tps_getMissionStatus_fill(sendBuf);
        break;
        case TPS_CMD_LANDING:
        len = tps_landing_fill(sendBuf);
        break;
        case TPS_CMD_M_LOAD:
        len = tps_loadMission_fill(sendBuf, &tps_mission_mes);
        break;
        case TPS_CMD_M_GET_ROUTE_STATUS:
        len = tps_getRouteMissionStatus_fill(sendBuf);
        break;
        case TPS_CMD_NETWORK_CHANGE:
        len = tps_changeNetwork_fill(sendBuf);
        break;
        default:
        break;
    }
    FrameType type=TYPE_REQUEST;
    xapFrame_fill(sendBuf, len, xap_sendBuf, XAP_META(1, type), seq++);

    tps_send(xap_sendBuf, len+12);
    printf("tps_send:");
    for(int i=0; i<len+12; i++)
    {
        printf("0x%02x ", xap_sendBuf[i]);
    }
    printf("\n");
    return len;
}
