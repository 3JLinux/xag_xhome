#ifndef HDLS_H
#define HDLS_H

#include <stdint.h>

// typedef struct 
// {
//     int32_t version;
//     float temperture;
//     float humidity;
//     float pressure;
//     float illumination;
//     float rainfall;
//     float wind_speed;
//     float wind_direction;
//     int32_t battery;
// }SENSOR_DATA;

int hdls_socket_init(void);
void XHOME_subscription(void);
void get_sensor_thread_create(void);
void get_sensor_thread_wait(void);
void hdls_rcv_thread_create(void);
void hdls_rcv_thread_wait(void);
void hdls_send(char* hdlsSubject, char* equID, char* data, int32_t dataLen);


#endif