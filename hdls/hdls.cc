#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#include "xiot.h"
#include "hdls.h"
//#include "dock.pb.h"
#include "base64.h"
#include "xhome.h"
#include "tps.h"
#include "dockhandle.h"
#include "loguser.h"

using namespace std;

#define PACKAGE_SIZE 3000
#define GATEWAY_UDP_PORT 6767

int udp_fd;

pthread_t get_sensor_thread_flag;
pthread_t hdls_rcv_thread_flag;

const char sensorID[] = "W866323036653169";

void test_tps_send();


/************************************************************
 * 函数原型：	int message_sendto(xiot_message_t *m)
 * 功能：		发送message报文
 * 输入：		xiot_mssage_t *m 发送message指针				
 * 输出：		-1：发送失败 其他：发送字节数
 * *********************************************************/
int message_sendto(xiot_message_t *m)
{
	int ret;
	int offset;
	char sdata[PACKAGE_SIZE+200] = {0};
	struct sockaddr_in des_addr;
	des_addr.sin_family = AF_INET;
	des_addr.sin_port = htons(GATEWAY_UDP_PORT);
	des_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	memset(sdata, 0, PACKAGE_SIZE+200);
	memcpy(sdata, "message", strlen("message"));
	offset = strlen("message");
	memcpy(sdata+offset, m, sizeof(xiot_message_t) + m->size);
	offset += sizeof(xiot_message_t) + m->size;

	ret = sendto(udp_fd, sdata, offset, 0, (struct sockaddr*)&des_addr, sizeof(des_addr));
	if(ret>0)
	{
		//printf("sendto:%s\n", sdata);
		int i=0;
		for(i=0;i<ret;i++)
			printf("%02x ", sdata[i]);
		printf("\n");
	}
	else
	{
		printf("sendto failed\n");
		WRITELOGFILE(LOG_ERROR, (uint8_t*)"hdls message sendto failed!");
	}
	return ret;
}


/************************************************************
 * 函数原型：	int post_datapoint(xiot_datapoint_clp_t *m)
 * 功能：		发送datapoint报文
 * 输入：		xiot_mssage_t *m 发送datapoint指针				
 * 输出：		-1：发送失败 其他：发送字节数
 * *********************************************************/
int post_datapoint(xiot_datapoint_clp_t *m)
{
	int ret;
	int offset;
	char sdata[PACKAGE_SIZE+200] = {0};
	struct sockaddr_in des_addr;
	des_addr.sin_family = AF_INET;
	des_addr.sin_port = htons(GATEWAY_UDP_PORT);
	des_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	memcpy(sdata, "datapoint", strlen("datapoint"));
	offset = strlen("datapoint");
	//memcpy(sdata+offset, m->location, 28);//sizeof(xiot_location_t));
	//offset += 28;//sizeof(xiot_location_t);
	memcpy(sdata+offset, m, sizeof(xiot_datapoint_clp_t) + m->size);
	offset += sizeof(xiot_datapoint_clp_t) + m->size;

	ret = sendto(udp_fd, sdata, offset, 0, (struct sockaddr*)&des_addr, sizeof(des_addr));
	// int i=0;
	// for(i=0;i<ret;i++)
	// 	printf("%02x ", sdata[i]);
	// printf("\n");
	return ret;
}


/************************************************************
 * 函数原型：	int devmessage_sendto(xiot_message_dev_t *m)
 * 功能：		发送devmessage报文
 * 输入：		xiot_mssage_t *m 发送datapoint指针				
 * 输出：		-1：发送失败 其他：发送字节数
 * *********************************************************/
int devmessage_sendto(xiot_message_dev_t *m)
{
	int ret;
	int offset;
	char sdata[PACKAGE_SIZE+200] = {0};
	struct sockaddr_in des_addr;
	des_addr.sin_family = AF_INET;
	des_addr.sin_port = htons(GATEWAY_UDP_PORT);
	des_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	memcpy(sdata, "devmessage", strlen("devmessage"));
	offset = strlen("devmessage");
	memcpy(sdata+offset, m, sizeof(xiot_message_dev_t) + m->size);
	offset += sizeof(xiot_message_dev_t) + m->size;

	ret = sendto(udp_fd, sdata, offset, 0, (struct sockaddr*)&des_addr, sizeof(des_addr));
	if(ret <= 0)
	{
		printf("send failed!\n");
		WRITELOGFILE(LOG_ERROR, (uint8_t*)"hdls devmessage send failed!");
	}
	char data[PACKAGE_SIZE] = {0};
	WRITELOGFILE(LOG_INFO, (uint8_t*)m->subject);
	sprintf(data, "%s", "devmessage:");
	for(int i=0;i< m->size; i++)
	{
		sprintf((data + strlen("devmessage:"))+i*2, "%02x", m->body[i]);
	}
	WRITELOGFILE(LOG_DEBUG, (uint8_t*)data);
	// printf("hdls_send:");
    // for(int i=0; i<ret; i++)
    // {
    //     printf("0x%02x ", sdata[i]);
    // }
	// printf("\n");
	return ret;
}


/************************************************************
 * 函数原型：	int socket_init(void)
 * 功能：		套接字初始化
 * 输入：		void
 * 输出：		套接字
 * *********************************************************/
int hdls_socket_init(void)
{
	udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
	int ret;
	udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(21219);
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ret = bind(udp_fd, (struct sockaddr*)&local_addr, sizeof(local_addr));
	if(ret < 0)
	{
		printf("bind fail\n");
		WRITELOGFILE(LOG_ERROR, (uint8_t*)"bind fail!");
		return -1;
	}
	
	return udp_fd;
}


/************************************************************
 * 函数原型：	int post_datapoint(xiot_datapoint_clp_t *m)
 * 功能：		发送datapoint报文
 * 输入：		xiot_mssage_t *m 发送datapoint指针				
 * 输出：		-1：发送失败 其他：发送字节数
 * *********************************************************/
void sensor_date_fill(SENSOR_DATA* sensor_data)
{
	sensor_data->version = 1;
	sensor_data->temperture = 31.29;
	sensor_data->humidity = 41.91;
	sensor_data->pressure = 1006.17;
	sensor_data->illumination = 0.27;
	sensor_data->rainfall = 83.09;
	sensor_data->wind_speed = 1.98;
	sensor_data->wind_direction = 135;
	sensor_data->battery = -1;
}


// int hdls_get_sensor()
// {
// 	int len;
// 	int ret;
// 	struct timeval timeout;
// 	char subject[128] = "DEMO";
// 	char data_buf[4000] = {0};
// 	xiot_message_t *msg = (xiot_message_t*)data_buf;
// 	xiot_message_dev_t *msg_dev = (xiot_message_dev_t*)data_buf;

// 	// struct sockaddr_in local_addr;
// 	// local_addr.sin_family = AF_INET;
// 	// local_addr.sin_port = htons(21219);
// 	// local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
// 	// ret = bind(udp_fd, (struct sockaddr*)&local_addr, sizeof(local_addr));
// 	// if(ret < 0)
// 	// {
// 	// 	printf("bind fail\n");
// 	// 	return -1;
// 	// }
// 	memset(data_buf, 0, sizeof(data_buf));
// 	msg_dev->id = 0;
// 	memcpy(msg_dev->to, "380043000F51303138393138", strlen("380043000F51303138393138"));
// 	strcpy(msg_dev->subject, subject);
	
// 	SENSOR_DATA sensor_data;
// 	sensor_date_fill(&sensor_data);

// 	char body_buf[256] = {0};
// 	memset(body_buf, 0, 256);

// 	for(int i=0; ;i++)
// 	{
// 		switch(i)
// 		{
// 			case 0:
// 			sprintf(body_buf,"%d",sensor_data.version);
// 			break;
// 			case 1:
// 			sprintf(body_buf,"%.02f",sensor_data.temperture);
// 			break;
// 			case 2:
// 			sprintf(body_buf, "%.02f", sensor_data.humidity);
// 			break;
// 			case 3:
// 			sprintf(body_buf,"%.02f", sensor_data.pressure);
// 			break;
// 			case 4:
// 			sprintf(body_buf,"%.02f", sensor_data.illumination);
// 			break;
// 			case 5:
// 			sprintf(body_buf, "%.02f", sensor_data.rainfall);
// 			break;
// 			case 6:
// 			sprintf(body_buf, "%.02f", sensor_data.wind_speed);
// 			break;
// 			case 7:
// 			sprintf(body_buf, "%.02f", sensor_data.wind_direction);
// 			break;
// 			case 8:
// 			sprintf(body_buf, "%d", sensor_data.battery);
// 			goto END;
// 			default:
// 			goto END;
// 			break;
// 		}
// 		strcat(msg_dev->body,body_buf);
// 		strcat(msg_dev->body,",");
// 		memset(body_buf, 0 , 256);
// 	}
// 	END:
// 		strcat(msg_dev->body,body_buf);
	


// 	msg_dev->size = (int)strlen(msg_dev->body);
// 	printf("send:%s\n", msg_dev->body);
// 	devmessage_sendto(msg_dev);
// }


/************************************************************
 * 函数原型：	int hdls_get_sensor_send(char* sensorID)
 * 功能：		请求气象站数据发送订阅函数	
 * 输入：		char* sensorID 气象站设备ID				
 * 输出：		int
 * *********************************************************/
int hdls_get_sensor_send(char* sensorID)
{
	char subject[128] = "GET_SENSOR";
	char data_buf[4000] = {0};
	static int i=0;
	xiot_message_dev_t *msg_dev = (xiot_message_dev_t*)data_buf;

	memset(data_buf, 0, sizeof(data_buf));
	msg_dev->id = i++;
	memcpy(msg_dev->to, sensorID, strlen(sensorID));
	strcpy(msg_dev->subject, subject);
	//strcpy(msg_dev->body, NULL);
	msg_dev->size = 0;
	devmessage_sendto(msg_dev);
}


/************************************************************
 * 函数原型：	
 * 功能：		
 * 输入：						
 * 输出：		
 * *********************************************************/
// int main()
// {
// 	int len;
// 	int ret;
// 	struct timeval timeout;
// 	char subject[128] = "XHOME";
// 	char data_buf[4000] = {0};
// 	xiot_message_t *msg = (xiot_message_t*)data_buf;
// 	xiot_message_dev_t *msg_dev = (xiot_message_dev_t*)data_buf;

// 	udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
// 	if(udp_fd == -1)
// 	{
// 		return -1;
// 	}
// 	//int opt = 1;
// 	//setsockopt(udp_fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&opt, sizeof(opt));

// 	struct sockaddr_in local_addr;
// 	local_addr.sin_family = AF_INET;
// 	local_addr.sin_port = htons(21219);
// 	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
// 	ret = bind(udp_fd, (struct sockaddr*)&local_addr, sizeof(local_addr));
// 	if(ret < 0)
// 	{
// 		printf("bind fail\n");
// 		return -1;
// 	}
// 	// hdls_get_sensor();

// 	memset(data_buf, 0, sizeof(data_buf));
// 	msg_dev->id = 0;
// 	memcpy(msg_dev->to, "A450031000551303130373836", strlen("A450031000551303130373836"));
// 	strcpy(msg_dev->subject, subject);
// 	strcpy(msg_dev->body, "abcde");
// 	msg_dev->size = (int)strlen(msg_dev->body);
// 	devmessage_sendto(msg_dev);

// 	struct sockaddr_in rcv_addr;
// 	socklen_t rcv_len = sizeof(rcv_addr);
// 	xiot_message_t *m;
// 	xiot_message_dev_t *mdev;
// 	while(1)
// 	{
// 		memset(data_buf, 0, sizeof(data_buf));
// 		len = recvfrom(udp_fd, data_buf, sizeof(data_buf), 0, (struct sockaddr *)&rcv_addr, &rcv_len);
// 		if(len == -1)
// 			return -1;
		
// 		if(memcmp(data_buf, "message", strlen("message")) == 0)
// 		{
// 			m = (xiot_message_t*)(data_buf+strlen("message"));
// 			printf("message recv body:%s\n", m->body);
// 		}
// 		else if(memcmp(data_buf, "devmessage", strlen("devmessage")) == 0)
// 		{
// 			mdev = (xiot_message_dev_t*)(data_buf+strlen("devmessage"));
// 			printf("devmessage recv from:%s, body:%s\n", mdev->from, mdev->body);
// 		}
// 	}
// 	return 0;
// }

/************************************************************
 * 函数原型：	void* get_sensor_thread(void* ptr)
 * 功能：		请求气象站数据线程
 * 输入：		void* ptr				
 * 输出：		void*
 * *********************************************************/
void* get_sensor_thread(void* ptr)
{
	while(1)
	{
		hdls_get_sensor_send((char*)sensorID);
		sleep(10);
		//test_tps_send();	
	}
}


/************************************************************
 * 函数原型：	void get_sensor_thread_create(void)
 * 功能：		请求气象站数据线程创建函数
 * 输入：		void				
 * 输出：		void
 * *********************************************************/
void get_sensor_thread_create(void)
{
	int temp;
	memset(&get_sensor_thread_flag, 0, sizeof(get_sensor_thread_flag));
	if((temp = pthread_create(&get_sensor_thread_flag, NULL, &get_sensor_thread, NULL)) != 0)
	{
		printf("get_sensor_thread_create ERROR!\n");
		WRITELOGFILE(LOG_ERROR, (uint8_t*)"get_sensor_thread_create ERROR!");
	}
	else
	{
		printf("get_sensor_thread_create SUCCESS!\n");
		WRITELOGFILE(LOG_TRACE, (uint8_t*)"get_sensor_thread_create SUCCESS!");
	}
}


/************************************************************
 * 函数原型：	void get_sensor_thread_wait(void)
 * 功能：		请求气象站数据线程等待结束
 * 输入：		void				
 * 输出：		void
 * *********************************************************/
void get_sensor_thread_wait(void)
{
	if(get_sensor_thread_flag != 0)
	{
		pthread_join(get_sensor_thread_flag, NULL);
		printf("get_sensor_thread END!\n");
		WRITELOGFILE(LOG_TRACE, (uint8_t*)"get_sensor_thread END!");
	}
}


/************************************************************
 * 函数原型：	void* hdls_rcv_thread(void* ptr)
 * 功能：		hdls接收数据处理线程
 * 输入：		void* ptr				
 * 输出：		void*
 * *********************************************************/
void* hdls_rcv_thread(void* ptr)
{
	int ret;
	int len;
	char data_buf[4000] = {0};
	char data[4000] = {0};
	xiot_message_t *msg = (xiot_message_t*)data_buf;
	xiot_message_dev_t *msg_dev = (xiot_message_dev_t*)data_buf;
	// struct sockaddr_in local_addr;
	// local_addr.sin_family = AF_INET;
	// local_addr.sin_port = htons(21219);
	// local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// ret = bind(udp_fd, (struct sockaddr*)&local_addr, sizeof(local_addr));
	// if(ret < 0)
	// {
	// 	printf("bind fail\n");
	// }

	struct sockaddr_in rcv_addr;
	socklen_t rcv_len = sizeof(rcv_addr);
	xiot_message_t *m;
	xiot_message_dev_t *mdev;
	CBase64Coder coder;
	
	while(1)
	{
		memset(data_buf, 0, sizeof(data_buf));
		memset(data, 0, 4000);
		len = recvfrom(udp_fd, data_buf, sizeof(data_buf), 0, (struct sockaddr *)&rcv_addr, &rcv_len);
		if(len > 0)
		{
			if(memcmp(data_buf, "message", strlen("message")) == 0)
			{
				m = (xiot_message_t*)(data_buf+strlen("message"));
				printf("message recv body:%s\n", m->body);

				sprintf(data, "message recv from %s:", m->subject);
				for(int i=0;i<m->size;i++)
				{
					sprintf(data+strlen("message recv from :")+strlen(m->subject)+i*2, "%02x", m->body[i]);
				}
				WRITELOGFILE(LOG_DEBUG, (uint8_t*)data);
				if(strcmp(m->subject,"GET_SENSOR") == 0)	//接收气象站数据
				{
					//SENSOR_DATA sensorDataBuf;
					sensor_rcv_handle(m->body);
				}
				else if(strcmp(m->subject, "XHOME") == 0)	//接收云端主动下发数据，需要对protobuf进行解析
				{
					if(m->size/4 != 0)
					{
						m->size += 4 - m->size%4;
					}
					const char* p = coder.decode(m->body, m->size);
					conditions_parse(p);
					// setDockFlyState(STATE_MISSION_LOAD);
					//docking_mission_load();
				}
				else if(strcmp(m->subject, "XHOME_DOCK") == 0)	//接收飞机xhome发送的交互信息
				{
					
				}

			}
			else if(memcmp(data_buf, "devmessage", strlen("devmessage")) == 0)
			{
				mdev = (xiot_message_dev_t*)(data_buf+strlen("devmessage"));
				printf("devmessage recv from %s:, body:%s\n", mdev->from, mdev->body);
				// sprintf(data, "devmessage recv from:%s", mdev->from);
				// WRITELOGFILE(LOG_INFO, (uint8_t*)data);
				sprintf(data, "devmessage recv from %s:", mdev->subject);
				for(int i=0;i<mdev->size;i++)
				{
					sprintf(data+strlen("devmessage recv from :")+strlen(mdev->subject)+i*2, "%02x", mdev->body[i]);
				}
				WRITELOGFILE(LOG_DEBUG, (uint8_t*)data);
				// printf("m->subject:%s\n", m->subject);
				// for(int i=0; i<mdev->size; i++)
				// {
				// 	printf("0x%02x ", mdev->body[i]);
				// }
				//SENSOR_DATA sensorDataBuf;
				// sensor_rcv_handle(mdev->body);
				if(strcmp(mdev->subject, "XHOME_DOCK") == 0)	//接收飞机xhome设备间通讯交互信息
				{
					docking_rcv_handle((uint8_t*)mdev->body, mdev->size);
					// for(int i=0; i<mdev->size; i++)
					// {
					// 	printf("0x%02x ", mdev->body[i]);
					// }
				}

			}
		}
		//sleep(1);
	}
}


/************************************************************
 * 函数原型：	void hdls_rcv_thread_create(void)
 * 功能：		hdls接收线程创建
 * 输入：		void
 * 输出：		void
 * *********************************************************/
void hdls_rcv_thread_create(void)
{
	int temp;
	memset(&hdls_rcv_thread_flag, 0, sizeof(hdls_rcv_thread_flag));
	if((temp = pthread_create(&hdls_rcv_thread_flag, NULL, &hdls_rcv_thread, NULL)) != 0)
	{
		printf("hdls_rcv_thread_create ERROR!\n");
		WRITELOGFILE(LOG_ERROR, (uint8_t*)"hdls_rcv_thread_create ERROR!");
	}
	else
	{
		printf("hdls_rcv_thread_create SUCCESS!\n");
		WRITELOGFILE(LOG_TRACE, (uint8_t*)"hdls_rcv_thread_create SUCCESS!");
	}
}


/************************************************************
 * 函数原型：	void hdls_rcv_thread_wait(void)
 * 功能：		
 * 输入：						
 * 输出：		
 * *********************************************************/
void hdls_rcv_thread_wait(void)
{
	if(hdls_rcv_thread_flag != 0)
	{
		pthread_join(hdls_rcv_thread_flag, NULL);
		printf("hdls_rcv_thread END!\n");
		WRITELOGFILE(LOG_TRACE, (uint8_t*)"hdls_rcv_thread END!");
	}
}


/************************************************************
 * 函数原型：	void XHOME_subscription(void)
 * 功能：		xhome主题订阅（接收数据前要先进行主题订阅）
 * 输入：		void
 * 输出：		void
 * *********************************************************/
void XHOME_subscription(void)
{
	char subject[128] = "XHOME";
	char data_buf[4000] = {0};
	static int i=0;
	xiot_message_t *msg = (xiot_message_t*) data_buf;

	memset(data_buf, 0, sizeof(data_buf));
	msg->id = i++;
	strcpy(msg->subject, subject);
	//strcpy(msg_dev->body, NULL);
	msg->size = 0;
	message_sendto(msg);
}


/************************************************************
 * 函数原型：	void hdls_send(char* hdlsSubject, char* equID, char* data, int32_t dataLen)
 * 功能：		hdls数据发送函数
 * 输入：		char* hdlsSubject hdls主题
 *				char* equID 设备ID NULL时使用message_sendto
 *				char* data 需要发送的数据指针
 *				int32_t dataLen 需要发送的数据长度
 * 输出：		void
 * *********************************************************/
void hdls_send(char* hdlsSubject, char* equID, char* data, int32_t dataLen)
{
	char subject[128] = {0};
	char data_buf[1024] = {0};
	char msg_buf[1024] = {0};
	static int i=0;
	xiot_message_dev_t *msg_dev = (xiot_message_dev_t*)data_buf;
    xiot_message_t *msg = (xiot_message_t*)msg_buf;

	if(equID != NULL)
	{
		memset(data_buf, 0, sizeof(data_buf));
		msg_dev->id = i++;
		memcpy(msg_dev->to, equID, strlen(equID));
		strcpy(msg_dev->subject, hdlsSubject);
		memcpy(msg_dev->body, data, dataLen);
		msg_dev->size = dataLen;
		devmessage_sendto(msg_dev);
	}
	else
	{
		memset(msg_buf, 0, sizeof(msg_buf));
		msg->id = i++;
		msg->src = 0;
		msg->dest = 0;
		strcpy(msg->subject, hdlsSubject);

		msg->size = dataLen;
		memcpy(msg->body, data, msg->size);
		message_sendto(msg);
	}
}




// int main()
// {
// 	hdls_socket_init();
// 	XHOME_subscription();
// 	get_sensor_thread_create();
// 	hdls_rcv_thread_create();
// 	get_sensor_thread_wait();
// 	hdls_rcv_thread_wait();
// }
