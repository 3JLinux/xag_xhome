#include "../can_driver/can_com.h"
#include "../message/msg_public.h"
#include "../can_driver/canbus_socket.h"
#include "../interface/interface_public.h"
#include "superx4_SAB.h"
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <memory.h>
#include <mutex>
#include <iomanip>
// #include <vector>
// #include <chrono> 
using namespace xag_can;
using namespace xag_can::canbus;
using namespace std;

#define RUN 1
#define STOP 0
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int status = RUN;
pthread_t can_locationStateCheck_thread_flag;

std::mutex mtx;
static Can_un CanBuffer;
static struct timeval last_sig;
static timer_t timer;
RcvMsg CanBufAddr[CanRcvTypeNum] ={
                                    {0x28F,SAB_0X28x.buff},
                                    {0x29F,SAB_0X29x.buff},
                                    {0x30F,SAB_0X30x.buff},
                                    {0x31F,SAB_0X31x.buff},
                                    {0x320,SAB_0X320.buff},
                                    {0x321,SAB_0X321.buff},
                                    {0x400,SAB_0X400.buff},
};
bool test_stop = false;
SocketCan::Ptr socket_can_ptr = std::make_shared<SocketCan>();

/******************************************
 * 函数原型:	void time_out_xx(void)
 * 功能:		1ms执行一次,接收功能
 * 输入:		none
 * 返回:		none
 *****************************************/
void time_out_xx(void)
{
    std::lock_guard<std::mutex> lock(mtx);
    static uint16_t losscount = 0;
    static uint32_t sendcount = 0;

    losscount++;
    sendcount++;
    if (sendcount % 100 == 0 )
    {
        sendcount = 0;
        Msg_info.SuperX4_CANSend_101 = 1;
    }
    if (sendcount % 10 == 0)
    {
        superx4_Send();
    } 

    while(socket_can_ptr->read(CanBuffer.id_data))
    {
        losscount = 0;
        for(uint8_t i=0; i<CanRcvTypeNum; i++)
        {
            if (CanBuffer.messages.id  <  NORMALID)
            {
                if(CanBufAddr[i].index > CanBuffer.messages.id)
                {
                    memcpy(&CanBufAddr[i].address[GETINDEX(CanBuffer.messages.id)],CanBuffer.messages.data,8);
                    break;
                }
            }
            else if(CanBufAddr[i].index == CanBuffer.messages.id)
            {
                memcpy(CanBufAddr[i].address,CanBuffer.messages.data,8);
                break;
            }
        }
    }
    // SuperX4_LocationState_check();
    if(losscount > LOSSTTIME)
    {
        Msg_info.Lost_SAB = true;
    }
    else
    {
        Msg_info.Lost_SAB = false;
    }
}

/******************************************
 * 函数原型:	void timer_notify(sigval_t val)
 * 功能:		1ms执行一次，功能函数
 * 输入:		none
 * 返回:		none
 *****************************************/
void timer_notify(sigval_t val)
{
    if (gettimeofday(&last_sig,NULL))
    {
        perror("gettimeofday()");
    }
    time_out_xx();
}

/******************************************
 * 函数原型:	void timerinit(void)
 * 功能:		初始化一个定时器
 * 输入:		none
 * 返回:		none
 *****************************************/
void timerinit(void)
{
    struct sigevent sigev;

    std::cout<< "test timerinit" << std::endl;
    if (gettimeofday(&last_sig,NULL))
    {
        perror("gettimeofday()");
    }
    memset(&sigev,0,sizeof(struct sigevent));
    sigev.sigev_value.sival_int = 0;
    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_notify_attributes = NULL;
    sigev.sigev_notify_function = timer_notify;

    if(timer_create(CLOCK_REALTIME,&sigev,&timer)){
        perror("timer_create()");
    }
}

/******************************************
 * 函数原型:	void setTimer(void)
 * 功能:		设置定时周期
 * 输入:		none
 * 返回:		none
 *****************************************/
void setTimer(void)
{
//	printf("setTimer(TIMEVAL value=%d)\n", value);
	// TIMEVAL is us whereas setitimer wants ns...
	struct itimerspec timerValues;
	timerValues.it_value.tv_sec = 0;
	timerValues.it_value.tv_nsec = 1000000;
	timerValues.it_interval.tv_sec = 0;
	timerValues.it_interval.tv_nsec = 1000000;
    std::cout<< "test setTimer" << std::endl;
 	if(timer_settime (timer, 0, &timerValues, NULL) == -1)
     {
         perror("fail to timer_settime");
     }
}

/******************************************
 * 函数原型:	void superx4_Send(void)
 * 功能:		发送函数 10ms 运行一次
 * 输入:		none
 * 返回:		none
 *****************************************/
void superx4_Send(void)
{
    if (Msg_info.SuperX4_CANSend_100 == 1)
    {
        if (socket_can_ptr->write(SuperX4_0x100.buff,0x100))
        {
            Msg_info.SuperX4_SendFail = 1;
        }
        else
        {
            Msg_info.SuperX4_SendFail = 0;
        }
        Msg_info.SuperX4_CANSend_100 = 0;
    }
    else if(Msg_info.SuperX4_CANSend_101 == 1)
    {
        Msg_info.SuperX4_CANSend_101 = 0;
        if (socket_can_ptr->write(SuperX4_0x101.buff,0x101))
        {
            Msg_info.SuperX4_SendFail = 1;
        }
        else
        {
            Msg_info.SuperX4_SendFail = 0;
        }
    }
    else if(Msg_info.SuperX4_CANSend_102 == 3)
    {
        Msg_info.SuperX4_CANSend_102 = 0;
        if (socket_can_ptr->write(SuperX4_0x102.buff,0x102))
        {
            Msg_info.SuperX4_SendFail = 1;
        }
        else
        {
            Msg_info.SuperX4_SendFail = 0;
        }    
    } 
}

//int main() {

//     uint64_t *prt = NULL;
//     SuperX4_0x102.SuperX4_0x102.SuperX4_TestMode = 1;
//     SuperX4_0x102.SuperX4_0x102.SuperX4_WakeUpT = 1;
//     SuperX4_0x102.SuperX4_0x102.SuperX4_GetBatteryInfoT = 1;
//     SuperX4_0x102.SuperX4_0x102.SuperX4_ChargeT = 1;
//     std::cout<< "SuperX4_TestMode = " << SuperX4_0x102.SuperX4_0x102.SuperX4_TestMode <<std::endl;
//     prt = reinterpret_cast<uint64_t *>(SuperX4_0x102.buff);
//    *prt = 0;
//     SuperX4_0x102.buff[0] = 50;
//     cout<< hex << SuperX4_0x102.buff[0] <<endl;
//     cout<< setbase(10) << SuperX4_0x102.buff[1] <<endl;
//     cout<< setbase(10) << SuperX4_0x102.buff[2] <<endl;
//     cout<< setbase(10) << SuperX4_0x102.buff[3] <<endl;
//     cout<< setbase(10) << SuperX4_0x102.buff[4] <<endl;
//     cout<< setbase(10) << SuperX4_0x102.buff[5] <<endl;
//     cout<< setbase(10) << SuperX4_0x102.buff[6] <<endl;
//     cout<< setbase(10) << SuperX4_0x102.buff[7] <<endl;

//    timerinit();
//    setTimer();
//    socket_can_ptr->start();
//    std::cout<< "test priont" << std::endl;

//    while(1);
//    return 0;
//}




/******************************************
 * 函数原型:	void canStartInit(void)
 * 功能:		
 * 输入:		none
 * 返回:		none
 *****************************************/
void canStartInit(void)
{
    timerinit();
    setTimer();
    socket_can_ptr->start();
}


/******************************************
 * 函数原型:	
 * 功能:		
 * 输入:		none
 * 返回:		none
 *****************************************/
void* can_locationStateCheck_thread(void* ptr)
{

    while(1)
    {
        pthread_mutex_lock(&mut);
        while(!status)
        {
            pthread_cond_wait(&cond,&mut);
        }
        pthread_mutex_unlock(&mut);
        SuperX4_LocationState_check();
        // printf("SuperX4_LocationState_check\n");
        usleep(1000*500); //500ms
    }
}


void can_locationStateCheck_thread_resume(void)
{
    if(status == STOP)
    {
        pthread_mutex_lock(&mut);
        status = RUN;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mut);
    }
}


void can_locationStateCheck_thread_pause(void)
{
    if(status == RUN)
    {
        pthread_mutex_lock(&mut);
        status = STOP;
        pthread_mutex_unlock(&mut);
    }
}

void can_locationStateCheck_thread_create(void)
{
    int temp;
    memset(&can_locationStateCheck_thread_flag, 0, sizeof(can_locationStateCheck_thread_flag));
    if((temp = pthread_create(&can_locationStateCheck_thread_flag, NULL, &can_locationStateCheck_thread, NULL)) != 0)
    {
        printf("can_locationStateCheck_thread_create ERROR!\n");
    }
    else
    {
        printf("can_locationStateCheck_thread_create SUCCESS\n");
    }
}


void can_locationStateCheck_thread_wait(void)
{
    if(can_locationStateCheck_thread_flag != 0)
    {
        pthread_join(can_locationStateCheck_thread_flag, NULL);
        printf("can_locationStateCheck_thread END!\n");
    }
}





