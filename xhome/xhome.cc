#include <string.h>
#include <stdio.h>

#include "xhome.h"
//#include "dock.pb.h"
#include "xhome.pb.h"
#include "base64.h"
// #include "tps.h"
#include "xap.h"

using namespace std;
using namespace xhome;

XHOME_THRESHOLD xhomeThreshold;		//存储任务信息
SENSOR_DATA sensorData;				//存储气象站数据
XHOME_MISSION_T xhomeMission;		//

#define ID_MAX_LENGTH		64

// static uint8_t sourceID[ID_MAX_LENGTH] = "A26002D000951383532383232";
// static uint8_t sourceID[ID_MAX_LENGTH] = "A380043000F51303138393138";
// // static uint8_t targetID[ID_MAX_LENGTH] = "A450031000551303130373836";
// static uint8_t targetID[ID_MAX_LENGTH] = "A2F0033000E51303336373030";

static uint8_t missionLoadFlag = 0;

void setMissionLoadFlag(void);
uint8_t getMissionLoadStatus();
uint8_t getFlyingType();
uint32_t getLandingStatus();
// uint32_t getMissionTimeout();

/************************************************************
 * 函数原型：	void sensor_rcv_handle(char* rcvBuf)
 * 功能：		气象站数据接收解析处理函数(解析后数据存储到全局变量sensorData)
 * 输入：		char* rcvBuf 接收的数据
 * 输出：		void
 * *********************************************************/
void sensor_rcv_handle(char* rcvBuf)
{
	char* startBuf = rcvBuf;
	//startBuf = strchr(rcvBuf, ':');
	char* token;
	int i = 0;

	token = strtok(startBuf, ",");

	while(token != NULL)
	{
		switch(i)
		{
			case 0: sensorData.version = atoi(token);
			break;
			case 1: sensorData.temperture = atof(token);
			break;
			case 2: sensorData.humidity = atof(token);
			break;
			case 3: sensorData.pressure = atof(token);
			break;
			case 4: sensorData.illumination = atof(token);
			break;
			case 5: sensorData.rainfall = atof(token);
			break;
			case 6: sensorData.wind_speed = atof(token);
			break;
			case 7: sensorData.wind_direction = atof(token);
			break;
			default:break;
		}
		printf("%s\n", token);
		i++;
		token = strtok(NULL, ",");
	}

}


/************************************************************
 * 函数原型：	void conditions_parse(const char *revbuf)
 * 功能：		接收云端任务信息以及环境阈值解析处理函数
 * 输入：		const char* revbuf				
 * 输出：		void
 * *********************************************************/
void conditions_parse(const char *revbuf)
{
	uint32_t len;
	CBase64Coder coder;
	xhome::Mission example;
	string strData(revbuf, len);
	example.ParseFromString(strData);  
	xhomeMission.version = example.version();
	xhomeMission.mission_type = example.mission_type();
	xhomeMission.mission_source = example.mission_source();
	const char* p = coder.decode(example.mission_id().c_str(), example.mission_id().length());
	//string pData(p,len);
	memcpy(xhomeMission.mission_id,p, 16);

	// memcpy(xhomeMission.mission_id,example.mission_id().c_str(), 16);
	xhomeMission.mission_timeout = example.mission_timeout();
	xhomeMission.file_name_length = example.file_name_length();

	const char* q = coder.decode(example.file_name().c_str(), example.file_name().length());
	// strcpy((char*)xhomeMission.file_name, "/usr/bin/xag_tps/");
	// strcat((char*)xhomeMission.file_name, (char*)xhomeMission.mission_id);
	// // strcat((char*)xhomeMission.file_name, "/");
	// strcat((char*)xhomeMission.file_name, q);

	memcpy(xhomeMission.file_name, q, example.file_name_length());
	//memcpy(xhomeMission.file_name, example.file_name().c_str(), example.file_name_length());
	xhomeMission.field_name_length = example.field_name_length();
	p = coder.decode(example.field_name().c_str(), example.field_name().length());
	memcpy(xhomeMission.field_name, p, example.field_name_length());
	//memcpy(xhomeMission.field_name, example.field_name().c_str(), example.field_name_length());

	cout << "version:" << example.version() << endl;
	cout << "mission_type::" << example.mission_type() << endl;
	cout << "mission_source:" << example.mission_source() << endl;
	cout << "mission_id:" << xhomeMission.mission_id << endl;
	cout << "mission_timeout:" << example.mission_timeout() << endl;
	cout << "file_name_length:" << example.file_name_length() << endl;
	cout << "file_name:" << xhomeMission.file_name << endl;
	cout << "filed_name_length:" << example.field_name_length() << endl;
	cout << "filed_name:" << xhomeMission.field_name << endl;

	for(int i = 0; i < example.conditions_size(); i++)
	{
		Conditions* temp = example.mutable_conditions(i);
	// for(int i = 0; i < example().threshold_size();i++){
	// 	Threshold* temp = example.mutable_threshold(i);
		cout << "key: " << temp->key() <<endl;
		cout << "max: "  << temp->max() << endl;
		cout << "min: "  << temp->min() << endl;
		cout << "text: "  << temp->text() << endl;
		cout << "unit: "  << temp->unit() << endl;
        if(strcmp(temp->key().c_str(), "windSpeed") == 0)
        {
            xhomeThreshold.windSpeedMax = temp->max();
            xhomeThreshold.windSpeedMin = temp->min();
        }
        else if(strcmp(temp->key().c_str(), "temperature") == 0)
        {
            xhomeThreshold.temperatureMax = temp->max();
            xhomeThreshold.temperatureMin = temp->min();
        }
        else if(strcmp(temp->key().c_str(), "illumination") == 0)
        {
            xhomeThreshold.illuminationMax = temp->max();
            xhomeThreshold.illuminationMin = temp->min();
        }
	}
	setMissionLoadFlag();
	// printf("mission_id:");
	// for(int j =0; j<16; j++)
	// {
	// 	printf("%x ", xhomeMission.mission_id[j]);
	// }
	// printf("\n");
}

/************************************************************
 * 函数原型：	int32_t conditions_cmp(void)
 * 功能：		实际的气象信息与阈值判断函数
 * 输入：		void				
 * 输出：		int32_t 0:不满足条件 1：满足阈值条件
 * *********************************************************/
uint8_t conditions_cmp(void)
{
	if(sensorData.temperture < xhomeThreshold.temperatureMin || 
	sensorData.temperture > xhomeThreshold.temperatureMax)
	{
		return 0;
	}
	else if(sensorData.wind_speed < xhomeThreshold.windSpeedMin || 
	sensorData.wind_speed > xhomeThreshold.windSpeedMax)
	{
		return 0;
	}
	else if(sensorData.illumination < xhomeThreshold.illuminationMin ||
	sensorData.illumination > xhomeThreshold.illuminationMax)
	{
		return 0;
	}
	else if(sensorData.rainfall > 0)
	{
		return 0;
	}
	return 1;
}


/************************************************************
 * 函数原型：	int32_t xhome_sendData_fill(uint8_t* buf, uint16_t cmd, uint8_t* sourceID, uint8_t* targetID, uint8_t* data, uint32_t dataLen)
 * 功能：		填充xhome协议数据头部
 * 输入：		uint8_t* buf 输出数据的数组指针
 *				uint16_t cmd 命令
 *				uint8_t* sourceID 源设备ID
 *				uint8_t* targetID 目标设备ID
 *				uint8_t* data 数据指针
 *				uint32_t dataLen 数据长度
 * 输出：		int32_t 填充后数据长度
 * *********************************************************/
int32_t xhome_sendData_fill(uint8_t* buf, uint16_t cmd, uint8_t* sourceID, uint8_t* targetID, uint8_t* data, uint32_t dataLen)
{
	XHOME_PROTO_T* sendBuf = (XHOME_PROTO_T*)buf;
	sendBuf->head = XHOME_HEAD;
	sendBuf->cmd = cmd;
	memcpy(sendBuf->sourceID, sourceID, ID_MAX_LENGTH);
	memcpy(sendBuf->targetID, targetID, ID_MAX_LENGTH);
	memcpy(sendBuf->data, data, dataLen);
	return dataLen+ID_MAX_LENGTH*2+4;
}


/************************************************************
 * 函数原型：	int32_t xhome_xap_fill(uint8_t* buf, uint16_t meta, uint8_t* data, uint32_t dataLen)
 * 功能：		填充满足xap协议的头部信息
 * 输入：		uint8_t* buf 输出数据的数组指针
 *				uint16_t meta xap的meta参数
 *				uint8_t* data 输入的数据指针
 *				uint32_t 输入的数据长度
 * 输出：		int32_t 填充后的数据长度
 * *********************************************************/
int32_t xhome_xap_fill(uint8_t* buf, uint16_t meta, uint8_t* data, uint32_t dataLen)
{
	static uint16_t seq = 0;
	XAP_FRAME_T* sendBuf = (XAP_FRAME_T*)buf;
	sendBuf->prefix = XAP_PREFIX;
	sendBuf->meta = meta;
	sendBuf->sequence = seq++;
	sendBuf->length = dataLen;
	sendBuf->checksum = 0;
	memcpy((sendBuf->content), data, dataLen);
	return dataLen+12;
}


/************************************************************
 * 函数原型：	int32_t xhome_fill_handle(uint8_t* buf, uint16_t cmd, uint8_t* sourceID, uint8_t* targetID)
 * 功能：		填充XHOME_DOCK主题协议数据(发送前直接调用此函数进行数据填充)
 * 输入：		uint8_t* buf 输出数据的数组指针
 *				uint16_t cmd xhome命令
 *				uint8_t* sourceID 源设备ID
 *				uint8_t* targetID 目标设备ID
 * 输出：		int32_t 填充后数据长度
 * *********************************************************/
int32_t xhome_fill_handle(uint8_t* buf, uint16_t cmd, uint8_t* sourceID, uint8_t* targetID)
{
	static uint8_t data[1024] = {0};
	//static uint8_t buf[1024] = {0};
	uint32_t len = 0;
	uint32_t bufLen = 0;
	uint8_t ack = 1;
	uint8_t type = 0;
	switch(cmd)
	{
		case XHOME_CMD_MISSION_LOAD:
			len = xhome_sendData_fill(data, XHOME_CMD_MISSION_LOAD, sourceID, targetID, (uint8_t*)&xhomeMission, sizeof(xhomeMission));
		break;
		case XHOME_CMD_MISSION_LOAD_ACK:
			len = xhome_sendData_fill(data, XHOME_CMD_MISSION_LOAD_ACK, sourceID, targetID, &ack, 1);
		break;
		case XHOME_CMD_MISSION_LOAD_COMP:
			XHOME_MISSION_LOAD_STATUS_T xhomeMissionLoadStatus;
			memcpy(xhomeMissionLoadStatus.mission_id, xhomeMission.mission_id, 16);
			xhomeMissionLoadStatus.mission_load_status = getMissionLoadStatus();
			len = xhome_sendData_fill(data, XHOME_CMD_MISSION_LOAD_COMP, sourceID, targetID, (uint8_t*)&xhomeMissionLoadStatus, sizeof(xhomeMissionLoadStatus));
		break;
		case XHOME_CMD_MISSION_LOAD_COMP_ACK:
			len = xhome_sendData_fill(data, XHOME_CMD_MISSION_LOAD_COMP_ACK, sourceID, targetID, &ack, 1);
		break;
		case XHOME_CMD_MISSION_START:
			XHOME_MISSION_START_T xhomeMissionStart;
			memcpy(xhomeMissionStart.mission_id, xhomeMission.mission_id, 16);
			len = xhome_sendData_fill(data, XHOME_CMD_MISSION_START, sourceID, targetID, (uint8_t*)&xhomeMissionStart, sizeof(xhomeMissionStart));
		break;
		case XHOME_CMD_MISSION_START_ACK:
			len = xhome_sendData_fill(data, XHOME_CMD_MISSION_START_ACK, sourceID, targetID, (uint8_t*)&ack, 1);
		break;
		case XHOME_CMD_FLY:
			len = xhome_sendData_fill(data, XHOME_CMD_FLY, sourceID, targetID, (uint8_t*)&ack, 1);
		break;
		case XHOME_CMD_FLY_ACK:
			len = xhome_sendData_fill(data, XHOME_CMD_FLY_ACK, sourceID, targetID, (uint8_t*)&ack, 1);
		break;
		case XHOME_CMD_HOVER:
			type = getFlyingType();
			len = xhome_sendData_fill(data, XHOME_CMD_HOVER, sourceID, targetID, &type, 1);
		break;
		case XHOME_CMD_HOVER_ACK:
			len = xhome_sendData_fill(data, XHOME_CMD_HOVER_ACK, sourceID, targetID, (uint8_t*)&ack, 1);
		break;
		case XHOME_CMD_LANDING:
			XHOME_LANDING_T xhomeLanding;
			len = xhome_sendData_fill(data, XHOME_CMD_LANDING, sourceID, targetID, (uint8_t*) &xhomeLanding, sizeof(xhomeLanding));
		break;
		case XHOME_CMD_LANDING_ACK:
			uint32_t status = getLandingStatus();
			len = xhome_sendData_fill(data, XHOME_CMD_LANDING_ACK, sourceID, targetID, (uint8_t*) &status, 1);
	}
	bufLen = xhome_xap_fill(buf, XAP_META_MESSAGE, data, len);
	return bufLen;
}



/************************************************************
 * 函数原型：	uint8_t getMissionLoadStatus()
 * 功能：		获取任务加载状态
 * 输入：						
 * 输出：		
 * *********************************************************/
uint8_t getMissionLoadStatus()
{
	
}


/************************************************************
 * 函数原型：	uint8_t getFlyingType()
 * 功能：		获取飞机飞行状态（是否返航悬停）
 * 输入：						
 * 输出：		
 * *********************************************************/
uint8_t getFlyingType()
{

}


/************************************************************
 * 函数原型：	uint32_t getLandingStatus()
 * 功能：		获取飞机降落状态（TPS接收到的降落命令返回）
 * 输入：						
 * 输出：		
 * *********************************************************/
uint32_t getLandingStatus()
{

}


/************************************************************
 * 函数原型：	void setMissionLoadFlag(void)
 * 功能：		
 * 输入：						
 * 输出：		
 * *********************************************************/
void setMissionLoadFlag(void)
{
	missionLoadFlag = 1;
}


uint8_t getMissionLoadFlag(void)
{
	return missionLoadFlag;
}


void resetMissionLoadFlag(void)
{
	missionLoadFlag = 0;
}


uint32_t getMissionTimeout(void)
{
	return xhomeMission.mission_timeout;
}