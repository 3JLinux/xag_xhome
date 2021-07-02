#ifndef WRITELOG_H
#define WRITELOG_H

#include <stdint.h>
#include <sys/stat.h>

// #define WRITELOGFILE(level, msg) WriteLogFile(__FILE__, (uint8_t *)__FUNCTION__, __LINE__, level, msg)

//日志级别定义
#define LOG_FATAL		    0//严重错误
#define LOG_ERROR		    1//一般错误
#define LOG_WARN		    2//警告
#define LOG_INFO		    3//一般信息
#define LOG_TRACE		    4//跟踪信息

#define LOG_DEBUG		    11//调试信息
#define LOG_ALL			    12//所有信息

#define LOG_FILE_MAX_NAME   256

#define LOG_FILE_MAX_NUM    3//
//#define LOG_FILE_MAX_SIZE   

static uint8_t configAddr[] = "/root/log/xag-xhome/Config.ini";

typedef struct _log_stat_info
{
    struct _log_stat_info *next;
    uint8_t LogFileName[LOG_FILE_MAX_NAME];
    uint32_t LogFileSize;
    time_t LogFileMtime;
}LOG_STAT_INFO;


void GetConfigValue();
void WriteLogFile(uint8_t *pszFileName, uint8_t *pszFunctionName, uint32_t iCodeLine, uint32_t iLogLevel, uint8_t *pszContent);
void logFileInit(void);

#endif


