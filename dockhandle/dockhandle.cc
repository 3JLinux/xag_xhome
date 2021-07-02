#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "xhome.h"
#include "hdls.h"
#include "xap.h"
#include "dockCanHandle.h"
#include "../communication/usr/superx4_SAB.h"
#include "loguser.h"

//#define XHOME_DEBUG     1		//调试打印开启

#define ID_MAX_LENGTH		64	//id号最大长度

#define SAB_FLY_MAX_DELAY_TIME 30*5*1000//200ms
#define CHARGE_TIMEOUT 60*30*5*1000 //200ms

#define BUF_MAX_LEN				1024

#define STATE_FREE              0
#define STATE_MISSION_LOAD      1
#define STATE_MISSION_START     2
#define STATE_FLY               3
#define STATE_HOVER             4
#define STATE_LAND              5
#define STATE_ERROR             6

// static uint8_t dockID[ID_MAX_LENGTH] = "A26002D000951383532383232"; //停靠站ID

static uint8_t dockID[ID_MAX_LENGTH] = "A380043000F51303138393138"; //停靠站ID
// static uint8_t flyID[ID_MAX_LENGTH] = "A450031000551303130373836";  //飞机ID
static uint8_t flyID[ID_MAX_LENGTH] = "A2F0033000E51303336373030";

char xhome_subject[] = "XHOME_DOCK";	//停靠站与飞机通讯主题

static uint8_t missionLoadAckFlag = 0;
static uint8_t missionLoadCompFlag = 0;
static uint8_t missionStartAckFlag = 0;
static uint8_t landAckFlag = 0;
static uint8_t dockFlyState = 0;

pthread_t dockhandle_thread_flag;


/************************************************************
 * 函数原型：	void xhomePrintf(char* data)
 * 功能：		调试打印函数
 * 输入：		char* data 需要打印的数据				
 * 输出：		void
 * *********************************************************/
void xhomePrintf(char* data)
{
#ifdef XHOME_DEBUG
    printf(data);
#endif
}


/************************************************************
 * 函数原型：	static uint8_t missionLoadAckSend(void)
 * 功能：		任务加载回复函数
 * 输入：		void				
 * 输出：		uint8_t 发送的字节数
 * *********************************************************/
static uint8_t missionLoadAckSend(void)
{
    uint8_t buf[BUF_MAX_LEN] = {0};
    uint32_t len = 0;
    len = xhome_fill_handle(buf, XHOME_CMD_MISSION_LOAD_ACK, flyID, dockID);
    hdls_send(xhome_subject, (char*)dockID, (char*)buf, len);
    xhomePrintf((char*)"XHOME_CMD_MISSION_LOAD_ACK");
    
	return len;
}


/************************************************************
 * 函数原型：	static uint8_t missionLoadAckSend(void)
 * 功能：		任务加载完成回复发送函数
 * 输入：		void				
 * 输出：		uint8_t 发送的字节数
 * *********************************************************/
static uint8_t missionLoadCompAckSend(void)
{
    uint8_t buf[BUF_MAX_LEN] = {0};
    uint32_t len = 0;
    len = xhome_fill_handle(buf, XHOME_CMD_MISSION_LOAD_COMP_ACK, dockID, flyID);
    hdls_send(xhome_subject, (char*)flyID, (char*)buf, len);
    xhomePrintf((char*)"XHOME_CMD_MISSION_LOAD_COMP_ACK");
	return len;
}


/************************************************************
 * 函数原型：	static uint8_t hoverAckSend(void)
 * 功能：		悬停状态回复函数
 * 输入：		void
 * 输出：		uint8_t 发送的字节数
 * *********************************************************/
static uint8_t hoverAckSend(void)
{
    uint8_t buf[BUF_MAX_LEN] = {0};
    uint32_t len = 0;
    len = xhome_fill_handle(buf, XHOME_CMD_HOVER_ACK, dockID, flyID);
    hdls_send(xhome_subject, (char*)flyID, (char*)buf, len);
    xhomePrintf((char*)"XHOME_CMD_HOVER_ACK");
	return len;
}


/************************************************************
 * 函数原型：	static uint8_t landingAckSend(void)
 * 功能：		降落命令回复函数
 * 输入：		void
 * 输出：		uint8_t 发送的字节数
 * *********************************************************/
static uint8_t landingAckSend(void)
{
    uint8_t buf[BUF_MAX_LEN] = {0};
    uint32_t len = 0;
    len = xhome_fill_handle(buf, XHOME_CMD_LANDING_ACK, flyID, dockID);
    hdls_send(xhome_subject, (char*)dockID, (char*)buf, len);
    xhomePrintf((char*)"XHOME_CMD_LANDING_ACK");
	return len;
}


/************************************************************
 * 函数原型：	static int8_t getAckFlag(uint16_t cmd)
 * 功能：		获取回复标志函数(停靠站与飞机交互的回复函数)
 * 输入：		uint16_t cmd	通过命令区分查询哪个函数的回复标志			
 * 输出：		int8_t -1:获取失败
 * *********************************************************/
static int8_t getAckFlag(uint16_t cmd)
{
    switch (cmd)
    {
        case XHOME_CMD_MISSION_LOAD_ACK:
            return missionLoadAckFlag;
        case XHOME_CMD_MISSION_START_ACK:
            return missionStartAckFlag;
        case XHOME_CMD_LANDING_ACK:
            return landAckFlag;
        default:
            return -1;
    }
}


/************************************************************
 * 函数原型：	static void setAckFlag(uint16_t cmd)
 * 功能：		置位回复标志函数
 * 输入：		uint16_t cmd	置位哪个函数的回复标志				
 * 输出：		void
 * *********************************************************/
static void setAckFlag(uint16_t cmd)
{
    switch(cmd)
    {
        case XHOME_CMD_MISSION_LOAD_ACK:
            missionLoadAckFlag = 1;
        break;
        case XHOME_CMD_MISSION_START_ACK:
            missionStartAckFlag = 1;
        case XHOME_CMD_LANDING_ACK:
            landAckFlag = 1;
        default:
        break;
    }
}


/************************************************************
 * 函数原型：	static void resetAckFlag(uint16_t cmd)
 * 功能：		复位回复标志函数
 * 输入：		uint8_t cmd				
 * 输出：		void
 * *********************************************************/
static void resetAckFlag(uint16_t cmd)
{
    switch(cmd)
    {
        case XHOME_CMD_MISSION_LOAD_ACK:
            missionLoadAckFlag = 0;
        break;
        case XHOME_CMD_MISSION_START_ACK: 
            missionStartAckFlag = 0;
        case XHOME_CMD_LANDING_ACK:
            landAckFlag = 0;
        default:
        break;
    }
}


/************************************************************
 * 函数原型：	static void seMissionLoadComptFlag(uint16_t cmd)
 * 功能：		接收到命令接收完成指令后的标志置位函数
 * 输入：		uint16_t cmd				
 * 输出：		void
 * *********************************************************/
static void setMissionLoadCompFlag(void)
{
    missionLoadCompFlag = 1;
}


/************************************************************
 * 函数原型：	uint8_t getMissionLoadCompFlag(void)
 * 功能：		查询任务接收完成标志函数
 * 输入：		void
 * 输出：		uint8_t 0:未完成 1：已完成
 * *********************************************************/
uint8_t getMissionLoadCompFlag(void)
{
    return missionLoadCompFlag;
}


/************************************************************
 * 函数原型：	void resetMissionLoadCompFlag(void)
 * 功能：		复位任务接收完成标志
 * 输入：		void				
 * 输出：		void
 * *********************************************************/
void resetMissionLoadCompFlag(void)
{
    missionLoadCompFlag = 0;
}


/************************************************************
 * 函数原型：	void setDockFlyState(uint8_t state)
 * 功能：		设置飞机运行状态用于与xhome交互信息调整	
 * 输入：		uint8_t state 设置的状态				
 * 输出：		void
 * *********************************************************/
void setDockFlyState(uint8_t state)
{
    dockFlyState = state;
}


/************************************************************
 * 函数原型：	void resetDockFlyState(void)
 * 功能：		复位飞机状态标志
 * 输入：		void				
 * 输出：		void
 * *********************************************************/
void resetDockFlyState(void)
{
    dockFlyState = STATE_FREE;
}


/************************************************************
 * 函数原型：	uint8_t getDockFlyState(void)
 * 功能：		获取飞机运行状态
 * 输入：		void				
 * 输出：		uint8_t
 * *********************************************************/
uint8_t getDockFlyState(void)
{
    return dockFlyState;
}



/************************************************************
 * 函数原型：	int8_t docking_fly_handle(void)
 * 功能：		停靠站处理飞机起飞执行函数
 * 输入：		void 			
 * 输出：		int8_t 0:
 * *********************************************************/
int8_t docking_fly_handle(void)
{
    static uint32_t timeCout = 0;  //s
    static uint8_t openFlag = 0;
    // if(conditions_cmp() && isChargeComplete() && !openFlag)
    if(!openFlag)
    {
        timeCout = 0;
        openDoor();
        openFlag = 1;
        printf("open door\n");
        WRITELOGFILE(LOG_INFO, (uint8_t*)"open door");
        //return 1;
    }
    else
    {
        timeCout++;
        if(isChargeComplete()) //开门后进行充电判断
        {
            timeCout = 0;
            if(getFlyCondition()) //舱门完全打开判断
            {
                timeCout = 0;
                openFlag = 0;
                return 1;
            }
            else
            {
                timeCout++;
                if(timeCout >= SAB_FLY_MAX_DELAY_TIME) //舱门打开超时
                {
                    openFlag = 0;
                    return -2;
                }
            }
        }
        else
        {
            if(timeCout >= CHARGE_TIMEOUT) //30分钟充电超时
            {
                printf("CHARGE_TIMEOUT\n");
                WRITELOGFILE(LOG_ERROR, (uint8_t*)"CHARGE TIMEOUT");
                timeCout = 0;
                openFlag = 0;
                return -3;
            }
        }
        // if(getMissionTimeout() != 0) //任务存活时间
        // {
        //     if(timeCout >= getMissionTimeout())
        //     {
        //         openFlag = 0;
        //         timeCout = 0;
        //         return -1;
        //     }
        // }
    }
    // if(openFlag)
    // {
    //     if(getFlyCondition())
    //     {
    //         timeCout = 0;
    //         // openFlag = 0;
    //         return 1;
    //     }
    //     else
    //     {
    //         timeCout++;
    //         if(timeCout >= SAB_FLY_MAX_DELAY_TIME)
    //         {
    //             // openFlag = 0;
    //             return -2;
    //         }
    //     }
    // }
    return 0;
}


/************************************************************
 * 函数原型：	void docking_mission_load(void)
 * 功能：		停靠站加载任务函数（收到云端下发的任务加载通知后通知飞机加载任务）
 * 输入：		void				
 * 输出：		void
 * *********************************************************/
void docking_mission_load(void)
{
    uint8_t buf[BUF_MAX_LEN] = {0};
    uint32_t len = 0;
    if(getMissionLoadFlag()) //获取到云端任务通知
    {
        mission_inform();	//给can发送任务通知
        WRITELOGFILE(LOG_TRACE, (uint8_t*)"DOCK STATE:MISSION LOADING");
        len = xhome_fill_handle(buf, XHOME_CMD_MISSION_LOAD, dockID, flyID);
        hdls_send((char*)"XHOME_DOCK", (char*)flyID, (char*)buf, len);
        usleep(1000*500);   //500ms
        if(getAckFlag(XHOME_CMD_MISSION_LOAD_ACK) == 1) //任务加载回复
        {
            resetAckFlag(XHOME_CMD_MISSION_LOAD_ACK);
            resetMissionLoadFlag();
        }
    }
    if(getMissionLoadCompFlag()) //任务加载完成
    {
        WRITELOGFILE(LOG_TRACE, (uint8_t*)"DOCK STATE:MISSION LOAD COMPLETE");
        len = xhome_fill_handle(buf, XHOME_CMD_MISSION_LOAD_COMP_ACK, dockID, flyID);
        hdls_send((char*)"XHOME_DOCK", (char*)flyID, (char*)buf, len);
        //docking_fly_handle();
        //setFlyHandleFlag();
        setDockFlyState(STATE_FLY);
        resetMissionLoadCompFlag();
    }
}


/************************************************************
 * 函数原型：	void docking_rcv_handle(uint8_t* data, uint32_t dataLen)
 * 功能：		XHOME_DOCK主题数据接收解析处理函数
 * 输入：		uint8_t* data		接收到的数据指针
 *				uint32_t dataLen	接收到的数据长度
 * 输出：		void
 * *********************************************************/
void docking_rcv_handle(uint8_t* data, uint32_t dataLen)
{
    uint8_t buf[BUF_MAX_LEN] = {0};
    Content* content = NULL;
    XAP_FRAME_T* rec_data = (XAP_FRAME_T*)buf;
    memcpy(buf, data, dataLen);
    content = (Content*)(rec_data->content);
    if(content->head == XHOME_HEAD) //协议头判断
    {
        char cmdBuf[4];
        sprintf(cmdBuf, "-cmd:%04x", content->command);
        WRITELOGFILE(LOG_INFO, (uint8_t*)cmdBuf);
        switch(content->command)
        {
            case XHOME_CMD_MISSION_LOAD:
                mission_inform();	//给can发送任务通知
                missionLoadAckSend();
            break;
            case XHOME_CMD_MISSION_LOAD_ACK:
                setAckFlag(XHOME_CMD_MISSION_LOAD_ACK);
                // setDockFlyState(STATE_MISSION_LOAD);
            break;
            case XHOME_CMD_MISSION_LOAD_COMP:
                setMissionLoadCompFlag();
                // missionLoadCompAckSend();
                //setDockFlyState(STATE_FLY);
            break;
            case XHOME_CMD_MISSION_LOAD_COMP_ACK:

            break;
            case XHOME_CMD_MISSION_START:
            break;
            case XHOME_CMD_MISSION_START_ACK:
                setAckFlag(XHOME_CMD_MISSION_START_ACK);
            break;
            case XHOME_CMD_FLY:

                flyOutoffDoor();	//舱门关闭
                printf("close door\n");
                WRITELOGFILE(LOG_TRACE, (uint8_t*)"CLOSE DOOR!");
            break;
            case XHOME_CMD_FLY_ACK:
            break;
            case XHOME_CMD_HOVER:
                hoverAckSend();
                if(getDockFlyState() != STATE_LAND) 
                {
                    setDockFlyState(STATE_HOVER);
                }
            break;
            case XHOME_CMD_HOVER_ACK:

            break;
            case XHOME_CMD_LANDING:
                landingAckSend();
            break;
            case XHOME_CMD_LANDING_ACK:
                setAckFlag(XHOME_CMD_LANDING_ACK);
            break;
            default:
            break;
        }
    }
}


/************************************************************
 * 函数原型：	void* dockhandle_thread(void* ptr)
 * 功能：		飞机起降交互处理线程
 * 输入：		void				
 * 输出：		void
 * *********************************************************/
void* dockhandle_thread(void* ptr)
{
    uint8_t buf[BUF_MAX_LEN] = {0};
    uint32_t len = 0;
    uint8_t flyHandleState = 0;
    while(1)
    {
        if(getMissionLoadFlag())	//获取任务通知
        {
            setDockFlyState(STATE_MISSION_LOAD);
            resetMissionLoadCompFlag();
        }
        if(getDockFlyState() == STATE_MISSION_LOAD) //任务加载
        {
            docking_mission_load();
            printf("load mission!\n");
        }
        if(getDockFlyState() == STATE_FLY)	//飞机起飞准备
        {
            WRITELOGFILE(LOG_TRACE, (uint8_t*)"DOCK STATE:FLY WAITING");
            flyHandleState = docking_fly_handle();
            //  flyHandleState = 1;
            if(flyHandleState == -1)
            {
                resetDockFlyState();
                printf("mission timeout\n");
                WRITELOGFILE(LOG_WARN, (uint8_t *)"mission timeout!");
            }
            else if(flyHandleState == -2)
            {
                resetDockFlyState();
                printf("door open timeout\n");
                WRITELOGFILE(LOG_WARN, (uint8_t*)"door open timeout!");
            }
            else if(flyHandleState == -3)
            {
                resetDockFlyState();
                printf("door open timeout\n");
                WRITELOGFILE(LOG_WARN, (uint8_t*)"door open timeout!");
            }
            else if(flyHandleState == 1)
            {
                setDockFlyState(STATE_MISSION_START); 
                printf("door open success\n");
                WRITELOGFILE(LOG_TRACE, (uint8_t*)"door open success!");
            }
        }
        if(getDockFlyState() == STATE_MISSION_START) //任务开启（飞机起飞）
        {
            can_locationStateCheck_thread_pause(); //挂起给can发送location线程
            printf("STATE_MISSION_START\n");
            WRITELOGFILE(LOG_TRACE, (uint8_t*)"DOCK STATE:MISSION START");
            memset(buf, 0, BUF_MAX_LEN);
            len = xhome_fill_handle(buf, XHOME_CMD_MISSION_START, dockID, flyID);
            hdls_send((char*)"XHOME_DOCK", (char*)flyID, (char*)buf, len);
            usleep(1000*500); 
            if(getAckFlag(XHOME_CMD_MISSION_START_ACK) == 1)
            {
                resetAckFlag(XHOME_CMD_MISSION_START_ACK);
                resetDockFlyState();
            }
        }
        if(getDockFlyState() == STATE_HOVER)	//飞机返航后悬停
        {
            // flyOutoffDoor();
            printf("STATE_HOVER\n");
            WRITELOGFILE(LOG_INFO, (uint8_t*)"STATE_HOVER");
            openDoor();
            setDockFlyState(STATE_LAND);
        }
        if(getDockFlyState() == STATE_LAND)		//飞机降落
        {
            if(isDoorOpenComplete())
            {
                printf("STATE_LAND\n");
                WRITELOGFILE(LOG_INFO, (uint8_t*)"STATE_LAND");
                memset(buf, 0, BUF_MAX_LEN);
                len = xhome_fill_handle(buf, XHOME_CMD_LANDING, dockID, flyID);
                hdls_send((char*)"XHOME_DOCK", (char*)flyID, (char*)buf, len);
                usleep(1000*500);   //500ms
                if(getAckFlag(XHOME_CMD_LANDING_ACK) == 1)
                {
                    resetAckFlag(XHOME_CMD_LANDING_ACK);
                    resetDockFlyState();
                    can_locationStateCheck_thread_resume();
                }
            }
        }
        usleep(200);
    }
}


/************************************************************
 * 函数原型：	void dockhandle_thread_create(void)
 * 功能：		飞机起降处理线程创建
 * 输入：		void				
 * 输出：		void
 * *********************************************************/
void dockhandle_thread_create(void)
{
    int temp;
    canStartInit();	//can初始化
    memset(&dockhandle_thread_flag, 0, sizeof(dockhandle_thread_flag));
    if((temp = pthread_create(&dockhandle_thread_flag, NULL, &dockhandle_thread, NULL)) != 0)
    {
        printf("dockhandle_thread_create ERROR!\n");
        WRITELOGFILE(LOG_ERROR, (uint8_t*)"dockhandle_thread_create ERROR!");
    }
    else
    {
        printf("dockhandle_thread_create SUCCESS!\n");
        WRITELOGFILE(LOG_TRACE, (uint8_t*)"dockhandle_thread_create SUCCESS!");
    }
}


/************************************************************
 * 函数原型：	void dockhandle_thread_wait(void)
 * 功能：		飞机起降处理线程结束等待
 * 输入：		void				
 * 输出：		void
 * *********************************************************/
void dockhandle_thread_wait(void)
{
    if(dockhandle_thread_flag != 0)
    {
        pthread_join(dockhandle_thread_flag, NULL);
        printf("dockhandle_thread END!\n");
        WRITELOGFILE(LOG_TRACE, (uint8_t*)"dockhandle_thread END!");
    }
}
