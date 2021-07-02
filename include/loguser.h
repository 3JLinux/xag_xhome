#ifndef LOGUSER_H
#define LOGUSER_H

#define USE_LOG 1

#ifdef USE_LOG
#include "WriteLog.h"
#define WRITELOGFILE(level, msg) WriteLogFile((uint8_t*)__FILE__, (uint8_t *)__FUNCTION__, __LINE__, level, msg)
extern void logFileInit(void);
#else
#define WRITELOGFILE(level, msg) 
#endif

#endif
