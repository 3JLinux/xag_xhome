#ifndef __SUPERX_II_H__
#define __SUPERX_II_H__
#include <stdint.h>
#include "transport/message/message_wrapper/driver/uart/uart.h"
#define SUPERX_II_API_VERSION 3.5

/*
 * version note:
 * v2.0 uses superx ii v2.x protocol, but firmware upgarde uses v1.x protocol
 * v2.0 uses crc16 checksum
*/
#ifdef __cplusplus
extern "C" {
#endif

extern int get_data_size_;
extern	int superx_fd_uart;
extern uint32_t fccom_bautrate;
#define get_module_no(module_type, module_index) (module_type << 0x016 | module_index)
#define VERSION(ver, sub,area, biuld) (((uint32_t)ver << 24)|((uint32_t)sub << 16)|((uint32_t)area << 8)|(biuld & 0xFF))
#define MODULE_STATUS_OFFLINE 0
#define MODULE_STATUS_ONLINE  1
#define MODULE_MAXNUM  20		//���ģ����Ŀ

typedef enum
{
    MODULE_TYPE_FC = 0x01,
    MODULE_TYPE_AHRS = 0x02,
    MODULE_TYPE_GPS = 0x03,
    MODULE_TYPE_OSD = 0x04,
    MODULE_TYPE_XLINK = 0x05,
    MODULE_TYPE_OA = 0x06,
    MODULE_TYPE_BLACKBOX = 0x07,
    MODULE_TYPE_DATALINK = 0x08,
    MODULE_TYPE_RC = 0x09,
    MODULE_TYPE_SPRAY = 0x0A,
    MODULE_TYPE_SONAR = 0x0B,
    MODULE_TYPE_BATTERY = 0x0C,
    MODULE_TYPE_TOF = 0x0D,
    MODULE_TYPE_CAMERA = 0x0E,
    MODULE_TYPE_P20LED = 0x10,
    MODULE_TYPE_NFC = 0x11,
    MODULE_TYPE_HUB = 0x12,
    MODULE_TYPE_DEBUGGER = 0x0F,
    MODULE_TYPE_AVOIDANCE = 0x15
} API_ModuleType_t;

typedef struct {
	uint32_t From;       //������Ϣ��ʱ��FromΪģ���Լ������Բ���д
	uint32_t To;         //������Ϣ��ʱ��ToΪģ���Լ�
	uint16_t Length;
	uint8_t * Data;
    uint8_t version;
} API_Message_t;

typedef struct {
    uint32_t From_Node;
	uint32_t To_Node;
    uint8_t Request_Id;
    uint8_t Respond_Id;
	uint32_t From;       //������Ϣ��ʱ��FromΪģ���Լ������Բ���д
	uint32_t To;         //������Ϣ��ʱ��ToΪģ���Լ�
	uint16_t Length;
	uint8_t * Data;
    uint8_t version;
} API_Net_Message_t;

typedef struct {
	uint32_t ModuleID[3];
	uint16_t ModuleType;		
	uint16_t ModuleIndex;		    //��API����
	uint32_t ModuleNo;			    //ģ���Զ�ע��ɹ�ʱ����
	uint8_t Status;                 //MODULE_STATUS_ONLINE, MODULE_STATUS_OFFLINE
	uint32_t HardwareVersion;       //Ӳ���汾��Ϣ
    uint32_t SoftwareVersion;       //�����汾��Ϣ
	uint16_t DataSize;		        //��������С
	uint16_t ModuleTimeout;	        //����ģ�鳬ʱʱ��
    uint8_t Baudrate;               //0,115200;1,19200;2,38400;3,57600;4,115200;5,256000;6,921600
    uint8_t RequireIndex;           //0Ϊ�Զ����䣨Ĭ�ϣ���1~7Ϊָ��
	uint16_t UseId;
	uint8_t Database;				//是否与CAN数据仓库共享，对电调舵机有效，0x88为共享，其他值不共享
} API_Config_t;

typedef struct {
	uint8_t Type;
	uint8_t Index;
	uint8_t Port;
	uint8_t Status;
	uint32_t HardwareVersion;
	uint32_t SoftwareVersion;
	uint32_t id[3];
} __attribute__((packed)) API_OldModuleInfo_t;	//ģ����Ϣ

typedef struct {
	uint8_t num;	//ģ����Ŀ
	API_OldModuleInfo_t moduleinfo[MODULE_MAXNUM];
} __attribute__((packed)) API_OldModuleLite_t;	//ģ���б�

typedef struct {
	uint16_t Type;
	uint16_t Index;
	uint8_t Port;
	uint8_t Status;
	uint32_t HardwareVersion;
	uint32_t SoftwareVersion;
	uint32_t id[3];
	uint8_t ProtocolVersion;
	uint16_t use_id;
	uint32_t reserve;
} __attribute__((packed)) API_ModuleInfo_t;		//ģ����Ϣ

typedef struct {
	uint8_t num;	//ģ����Ŀ
	API_ModuleInfo_t moduleinfo[MODULE_MAXNUM];
} __attribute__((packed)) API_ModuleLite_t;	//ģ���б�


void api_init(API_Config_t * config);

void api_loop(uint32_t time);

uint8_t api_get_fc_protocol_version(void); // 1,2,3
void api_get_data(int32_t offset, int32_t len);
void api_set_data(int32_t offset, int32_t len, void * data, uint8_t no_ack);
void api_send_message(API_Message_t *msg);
uint8_t api_send_net_message(API_Net_Message_t *msg);
void api_get_modulelist(uint8_t list_version);	//list_version, 0 - old_list, 1 - new_list
void api_auto_load_data(int32_t offset, int32_t len, uint16_t enabled, uint16_t interval);
void api_port_received(int32_t len, uint8_t * data);

void api_port_send(int32_t len, uint8_t * data); //weak
void api_on_data_set(uint8_t result); //weak, 0 - successful, 1 - failed
void api_on_data_returned(int32_t offset, int32_t len, uint8_t * data); //weak
void api_on_message_received(API_Message_t message); //weak
void api_on_net_message_received(API_Net_Message_t message); //weak
void api_on_upgrade(void);
void api_on_modulelist_received(API_ModuleLite_t *modublelist);		//list_version - 1
void api_on_oldmodulelist_received(API_OldModuleLite_t *modublelist);	//list_version - 0
void api_on_configurate_baudrate(int fd, int baudrate);


//Utils
#define CRC16_INIT_VALUE 0xFFFF
typedef struct{
    uint16_t current; //termporary value during crc16 calculation,original value is 0xffff
    uint16_t crc16; //the crc16 value
} crc16_t;

void crc16(uint8_t input, crc16_t * output);
uint16_t crc_ccitt(uint8_t * input, int len);

#ifdef __cplusplus
}
#endif

#endif  /* endif __SUPERX_II_H__ */

