#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>
#include <sys/stat.h>
#include "WriteLog.h"
#include "memb.h"
#include "list.h"

typedef unsigned char  BYTE;

//全局变量
uint32_t g_iLogLevel		= 0;//日志等级
uint32_t g_iLogPosition		= 0;//日志位置
uint8_t  g_szLogFile[100]	= {0};//带路径的日志文件名

static uint8_t *LogLevel(uint32_t iLogLevel);
static void GetTime(uint8_t *pszTimeStr);
void GetConfigValue(void);
static void GetStringContentValue(FILE *fp, uint8_t *pszSectionName, uint8_t *pszKeyName, uint8_t *pszOutput,  uint32_t iOutputLen);
static void GetConfigFileStringValue(uint8_t *pszSectionName, uint8_t *pszKeyName, uint8_t *iDefaultVal, uint8_t *pszOutput, uint32_t iOutputLen, uint8_t *pszConfigFileName);
static int32_t GetConfigFileIntValue(uint8_t *pszSectionName, uint8_t *pszKeyName, uint32_t iDefaultVal, uint8_t *pszConfigFileName);
//static void GetEmployeeInfo(void);
static void GetFileName(void);

static void logFileNumCheck(void);
static LOG_STAT_INFO* logStatListAdd(const LOG_STAT_INFO* pcnode);


/* Compare memory to memory */
int mem_cmp (const void* dst, const void* src, unsigned int cnt) 
{
	const BYTE *d = (const BYTE *)dst, *s = (const BYTE *)src;
	int r = 0;

	while (cnt-- && (r = *d++ - *s++) == 0) ;
	return r;
}

/******************************************************
 * 函数原型：	int32_t main(void)
 * 功能：		log测试用主程序
 * 输入：		void
 * 返回：
******************************************************/
/*
int32_t main(void)
{
	uint8_t szLogContent[1024] = {0};
	
	//获取配置文件中各个配置项的值
	GetConfigValue();

	//先打印版本相关信息
	snprintf(szLogContent, sizeof(szLogContent)-1, "Version[1.0], Bulid time[%s %s].", __DATE__, __TIME__);
	WRITELOGFILE(LOG_INFO, szLogContent);

	//打印第一条日志
	snprintf(szLogContent, sizeof(szLogContent)-1, "The Fatal log info!");
	WRITELOGFILE(LOG_FATAL, szLogContent);

	//打印第二条日志
	snprintf(szLogContent, sizeof(szLogContent)-1, "The Error log info!");

	WRITELOGFILE(LOG_ERROR, szLogContent);

	//打印第三条日志
	snprintf(szLogContent, sizeof(szLogContent)-1, "The Warn log info!");
	WRITELOGFILE(LOG_WARN, szLogContent);

	//打印第四条日志
	snprintf(szLogContent, sizeof(szLogContent)-1, "The Info log info!");
	WRITELOGFILE(LOG_INFO, szLogContent);

	//打印第五条日志
	snprintf(szLogContent, sizeof(szLogContent)-1, "The Trace log info!");
	WRITELOGFILE(LOG_TRACE, szLogContent);

	//打印第六条日志
	snprintf(szLogContent, sizeof(szLogContent)-1, "The Debug log info!");
	WRITELOGFILE(LOG_DEBUG, szLogContent);

	//打印第七条日志
	snprintf(szLogContent, sizeof(szLogContent)-1, "The All log info!");
	WRITELOGFILE(LOG_ALL, szLogContent);

	return 0;
}
*/

/*******************************************************
 * 函数原型：	void WriteLogFile(uint8_t *pszFileName, uint8_t *pszFunctionName, uint32_t iCodeLine, uint32_t iLogLevel, uint8_t *pszContent)
 * 功能：		将内容写到日志文件中
 * 输入：		uint8_t *pszFileName		代码文件名
				uint8_t *pszFunctionName	代码所在函数
				uint32_t iCodeLine			代码行
				uint32_t iLogLevel			日志等级
				uint8_t *pszContent			日志内容
 * 返回:		void
*******************************************************/
void WriteLogFile(uint8_t *pszFileName, uint8_t *pszFunctionName, uint32_t iCodeLine, uint32_t iLogLevel, uint8_t *pszContent)
{
	FILE *fp					= NULL;
	uint8_t szLogContent[2048]	= {0};
	uint8_t szTimeStr[128]		= {0};

	if(pszFileName == NULL || pszContent == NULL)
	{
		return;
	}

	//过滤日志等级
	if(iLogLevel > g_iLogLevel)
	{
		return;
	}

	GetFileName();
	fp = fopen((const char*)g_szLogFile, "at+");//打开文件，每次写入的时候在后面追加
	if(fp == NULL)
	{
		printf("%s open failed!\r\n", g_szLogFile);
		return;
	}

	//写入日志时间
	GetTime(szTimeStr);
	fputs((const char*)szTimeStr, fp);

	if(g_iLogPosition == 1)//在日志信息中显示“文件名/函数名/代码行数”信息
	{
		snprintf((char*)szLogContent, sizeof(szLogContent)-1, "[%s][%s][%04d][%s]%s\n", pszFileName, pszFunctionName, (int32_t)iCodeLine, LogLevel(iLogLevel), pszContent);
	}
	else//不用在日志信息中显示“文件名/代码行数”信息
	{
		snprintf((char*)szLogContent, sizeof(szLogContent)-1, "[%s]%s\n", LogLevel(iLogLevel), pszContent);
	}
	fputs((const char*)szLogContent, fp);

	fflush(fp);//刷新文件
	fclose(fp);//关闭文件
	fp = NULL;//将文件指针置空

	return;
}


/******************************************************
 * 函数原型：	static uint8_t *LogLeval(uint32_t iLogLevel)
 * 功能：		获取对应的日志等级
 * 输入：		uint32_t iLogLevel	日志等级
 * 返回：		日志等级信息字符串
******************************************************/
static uint8_t *LogLevel(uint32_t iLogLevel)
{
	switch(iLogLevel)
	{
		case LOG_FATAL:
		{
			return (uint8_t*)"FATAL";
		}
		case LOG_ERROR:
		{
			return (uint8_t*)"ERROR";
		}
		case LOG_WARN:
		{
			return (uint8_t*)"WARN";
		}
		case LOG_INFO:
		{
			return (uint8_t*)"INFO";
		}
		case LOG_TRACE:
		{
			return (uint8_t*)"TRACE";
		}
		case LOG_DEBUG:
		{
			return (uint8_t*)"DEBUG";
		}
		case LOG_ALL:
		{
			return (uint8_t*)"ALL";
		}
		default:
		{
			return (uint8_t*)"OTHER";
		}
	}
}


/******************************************************
 * 函数原型：	static void GetTime(uint8_t *pszTimeStr)
 * 功能：		获取时间
 * 输入：		uint8_t *pszTimeStr	时间串
 * 返回：		void
******************************************************/
static void GetTime(uint8_t *pszTimeStr)
{
	struct tm		tSysTime		= {0};
	struct timeval	tTimeVal		= {0};
	time_t			tCurrentTime	= {0};

	uint8_t szUsec[20] = {0};//微秒
	uint8_t szMsec[20] = {0};//毫秒

	if(pszTimeStr == NULL)
	{
		return;
	}

	tCurrentTime = time(NULL);
	localtime_r(&tCurrentTime, &tSysTime);

	gettimeofday(&tTimeVal, NULL);
	sprintf((char*)szUsec, "%06ld", tTimeVal.tv_usec);//获取微秒
	strncpy((char*)szMsec, (const char*)szUsec, 3);//微秒的前三位为毫秒（1毫秒=1000微秒）
	sprintf((char*)pszTimeStr, "[%04d.%02d.%02d %02d:%02d:%02d.%3.3s]",
			tSysTime.tm_year+1900, tSysTime.tm_mon+1, tSysTime.tm_mday,
			tSysTime.tm_hour, tSysTime.tm_min, tSysTime.tm_sec, szMsec);
}


/******************************************************
 * 函数原型：	void GetCompletePath(uint8_t *pszConfigFileName, uint8_t *pszWholePath)
 * 功能：		获取配置文件完整路径
 * 输入：		uint8_t *pszConfigFileName 配置文件名
				uint8_t *pszWholePath 配置文件完整路径
 * 返回：		void
******************************************************/
static void GetCompletePath(uint8_t *pszConfigFileName, uint8_t *pszWholePath)
{
	uint8_t *pszHomePath = NULL;
	uint8_t szWholePath[256] = {0};

	//先对输入参数进行异常判断
	if(pszConfigFileName == NULL || pszWholePath == NULL)
	{
		return;
	}
/*
	pszHomePath = (uint8_t *)(getenv("HOME"));//获取当前用户所在的路径
	if(pszHomePath == NULL)
	{
		return;
	}
*/
	//拼装装置文件路径
	snprintf((char*)szWholePath, sizeof(szWholePath)-1, "%s",  pszConfigFileName);

	strncpy((char*)pszWholePath, (const char*)szWholePath, strlen((const char*)szWholePath));
}


/******************************************************
 * 函数原型：	void GetConfigValue(void)
 * 功能：		获取日志配置项的值
 * 输入：		void
 * 返回：		void
******************************************************/
void GetConfigValue(void)
{
	//uint8_t szLogDir[256] = {0};

	//日志等级
	g_iLogLevel = GetConfigFileIntValue((uint8_t*)"LOG", (uint8_t*)"LogLevel", 5, configAddr);//"Config.ini");

	//日志位置
	g_iLogPosition = GetConfigFileIntValue((uint8_t*)"LOG", (uint8_t*)"LogPosition", 0, configAddr);//"Config.ini");

	//日志文件存放目录
	//GetConfigFileStringValue("LOG", "LogDir", "", szLogDir,sizeof(szLogDir), "Config.ini");
	//snprintf(g_szLogFile, sizeof(g_szLogFile)-1, "%s/WriteLog.log",szLogDir);
}


/*****************************************************
 * 函数原型：	void GetStringContentValue(FILE *fp, uint8_t *pszSectionName, uint8_t *pszKeyName, uint8_t *pszOutput, uint32_t iOutputLen)
 * 功能：		获取具体的字符串值
 * 输入：		FILE *fp	配置文件指针
				pszSectionName	段名
				pszKeyName	配置项名
				iOutputLen	输出缓存长度
 * 返回：		void
*****************************************************/
static void GetStringContentValue(FILE *fp, uint8_t *pszSectionName, uint8_t *pszKeyName, uint8_t *pszOutput, uint32_t iOutputLen)
{
	uint8_t szSectionName[100]		= {0};
	uint8_t szKeyName[100]			= {0};
	uint8_t szContentLine[256]		= {0};
	uint8_t szContentLineBak[256]	= {0};
	uint32_t iContentLineLen		= 0;
	uint32_t iPositionFlag			= 0;

	//先对输入参数进行异常判断
	if(fp == NULL || pszSectionName == NULL || pszKeyName == NULL || pszOutput == NULL)
	{
		printf("GetStringContentValue: input parametter(s) is NULL\n");
		return;
	}

	sprintf((char*)szSectionName, "[%s]", pszSectionName);
	strcpy((char*)szKeyName, (const char*)pszKeyName);

	while(feof(fp) == 0)
	{
		memset(szContentLine, 0x00, sizeof(szContentLine));
		fgets((char*)szContentLine, sizeof(szContentLine), fp);//获取段名

		//判断是否注释行（以；开头的行就是注释行）或以其它特殊字符开头的行
		if(szContentLine[0] == ';' || szContentLine[0] == '\r' || szContentLine[0] == '\n' || szContentLine[0] == '\0')
		{
			continue;
		}

		//匹配段名
		if(strncasecmp((const char*)szSectionName, (const char*)szContentLine, strlen((const char*)szSectionName)) == 0)
		{
			while(feof(fp) == 0)
			{
				memset(szContentLine, 0x00, sizeof(szContentLine));
				memset(szContentLineBak, 0x00, sizeof(szContentLine));
				fgets((char*)szContentLine, sizeof(szContentLine), fp);

				//判断是否注释行
				if(szContentLine[0] == ';')
				{
					continue;
				}

				memcpy(szContentLineBak, szContentLine, strlen((const char*)szContentLine));

				//匹配配置项名
				if(strncasecmp((const char*)szKeyName, (const char*)szContentLineBak, strlen((const char*)szKeyName)) == 0)
				{
					iContentLineLen = strlen((const char*)szContentLine);
					for(iPositionFlag = strlen((const char*)szKeyName); iPositionFlag <= iContentLineLen; iPositionFlag++)
					{
						if(szContentLine[iPositionFlag] == ' ')
						{
							continue;
						}
						if(szContentLine[iPositionFlag] == '=')
						{
							break;
						}

						iPositionFlag = iContentLineLen + 1;
						break;
					}

					iPositionFlag = iPositionFlag + 1;//跳过=的位置

					if(iPositionFlag > iContentLineLen)
					{
						continue;
					}

					memset(szContentLine, 0x00, sizeof((const char*)szContentLine));
					strcpy((char*)szContentLine, (const char*)(szContentLineBak + iPositionFlag));

					//去掉内容中的无关字节
					for(iPositionFlag = 0; iPositionFlag < strlen((const char*)szContentLine); iPositionFlag++)
					{
						if(szContentLine[iPositionFlag] == '\r' || szContentLine[iPositionFlag] == '\n' || szContentLine[iPositionFlag] == '\0')
						{
							szContentLine[iPositionFlag] = '\0';
							break;
						}
					}

					//将配置项内容拷贝到输出缓存中
					strncpy((char*)pszOutput, (const char*)szContentLine, iOutputLen-1);
					break;
				}
				else if(szContentLine[0] == '[')
				{
					break;
				}
			}
		}
		break;
	}
}


/******************************************************
 * 函数原型：	void GetConfigFileStringValue(uint8_t *pszSectionName, uint8_t * pszKeyName, uint8_t *pDefaultVal, uint8_t *pszOutput, uint32_t iOutputLen, uint8_t *pszConfigFileName)
 * 功能：		从配置文件中获取字符串
 * 输入：		uint8_t *pszSectionName	段名
				uint8_t *pszKeyName		配置项名
				uint8_t *pDefaultVal	默认值
				uint8_t *pszOutput		输出缓存
				uint32_t iOutputLen		输出缓存长度
				uint8_t *pszConfigFileName	配置文件名
 * 返回			void
 *****************************************************/
static void GetConfigFileStringValue(uint8_t *pszSectionName, uint8_t *pszKeyName, uint8_t *pDefaultVal, uint8_t *pszOutput, uint32_t iOutputLen, uint8_t *pszConfigFileName)
{
	FILE *fp					= NULL;
	uint8_t szWholePath[256]	= {0};

	//先对输入参数进行异常判断
	if(pszSectionName == NULL || pszKeyName == NULL || pszOutput == NULL || pszConfigFileName == NULL)
	{
		printf("GetConfigFileStringValue: input parameter(s) is NULL!\n");
		return;
	}

	//获取默认值
	if(pDefaultVal == NULL)
	{
		strcpy((char*)pszOutput, (const char*)"");
	}
	else
	{
		strcpy((char*)pszOutput, (const char*)pDefaultVal);
	}

	//打开配置文件
	GetCompletePath(pszConfigFileName, szWholePath);
	fp = fopen((const char*)szWholePath, (const char*)"r");
	if(fp == NULL)
	{
		printf("GetConfigFileStringValue: open %s failed!\n", szWholePath);
		return;
	}

	//调用函数用于获取具体配置项的值
	GetStringContentValue(fp, pszSectionName, pszKeyName, pszOutput, iOutputLen);

	//关闭文件
	fclose(fp);
	fp = NULL;
}


/******************************************************
 * 函数原型：	int32_t GetConfigFileIntValue(uint8_t *pszSectionName, uint8_t *pszKeyName, uint32_t iDefaultVal, uint8_t *pszConfigFileName)
 * 功能：		从配置文件中获取整型变量
 * 输入：		uint8_t *pszSectionName	段名
				uint8_t *pszKeyName		配置项名
				uint32_t iDefaultVal	默认值
				uint8_t *pszConfigFileName	配置文件名
 * 返回：		获取到的整型值
******************************************************/
static int32_t GetConfigFileIntValue(uint8_t *pszSectionName, uint8_t *pszKeyName, uint32_t iDefaultVal, uint8_t *pszConfigFileName)
{
	uint8_t szGetValue[512] = {0};
	int32_t iGetValue		= 0;

	//先对输入参数进行异常判断
	if(pszSectionName == NULL || pszKeyName == NULL || pszConfigFileName == NULL)
	{
		printf("GetConfigFileIntValue: input parameter(s) is NULL!\n");
		return -1;
	}

	GetConfigFileStringValue(pszSectionName, pszKeyName, NULL, szGetValue, 512-1, pszConfigFileName);//先将获取的值存放在字符型缓存中

	if(szGetValue[0] == '\0' || szGetValue[0] == ';')//如果是结束符或分号，则使用默认值
	{
		iGetValue = atoi((const char*)szGetValue);
	}
	else
	{
		iGetValue = atoi((const char*)szGetValue);
	}
	return iGetValue;
}


/******************************************************
 * 函数原型：	 static void GetFileName(void)
 * 功能：		获取时间
 * 输入：		uint8_t *pszTimeStr	时间串
 * 返回：		void
******************************************************/
static void GetFileName(void)
{
	struct tm 	tSysTime 		= {0};
	time_t 		tCurrentTime 	= {0};
	uint8_t pszFileNameStr[20] 	= {0};
	static uint8_t pre_day;
	uint8_t szLogDir[256] 		= {0};
	LOG_STAT_INFO info;
	tCurrentTime = time(NULL);
	localtime_r(&tCurrentTime, &tSysTime);

	//if(pre_day != tSysTime.tm_mday)
	if(pre_day != tSysTime.tm_mday)
	{
		GetConfigFileStringValue((uint8_t*)"LOG", (uint8_t*)"LogDir", (uint8_t*)"", szLogDir, sizeof(szLogDir), (uint8_t*)configAddr);//"Config.ini");
		sprintf((char*)pszFileNameStr, "%04d.%02d.%02d", tSysTime.tm_year+1900, tSysTime.tm_mon+1, tSysTime.tm_mday);
		snprintf((char*)g_szLogFile, sizeof(g_szLogFile)-1, "%s/%s.log", szLogDir, pszFileNameStr);
		memcpy(info.LogFileName, pszFileNameStr, strlen((const char*)pszFileNameStr));
		logFileNumCheck();
		logStatListAdd(&info);
	}
}

/*******************************************LOG文件链表********************************************************/
MEMB(logstatmem,LOG_STAT_INFO, LOG_FILE_MAX_NUM);
LIST(logstatlist);

static uint32_t file_num = 0; //链表文件个数
/******************************************************
 * 函数原型：	static void logListInit(void)
 * 功能：		链表初始化
 * 输入：		void
 * 返回：		void
******************************************************/
static void logStatListInit(void)
{
	memb_init(&logstatmem);
	list_init(logstatlist);
	file_num = 0;
}


/******************************************************
 * 函数原型：	static LOG_STAT_INFO* logStatListHead(void)
 * 功能：		返回链表头指针
 * 输入：		void
 * 返回：		LOG_STAT_INFO* 链表头指针
******************************************************/
static LOG_STAT_INFO* logStatListHead(void)
{
	return (LOG_STAT_INFO*)list_head(logstatlist);
}


/******************************************************
 * 函数原型：	static LOG_STAT_INFO* logStatListNext(LOG_STAT_INFO* pode)
 * 功能：		返回链表下一个结构体指针
 * 输入：		LOG_STAT_INFO* pode 链表当前结构体指针
 * 返回：		LOG_STAT_INFO* 链表下一个结构体指针
******************************************************/
static LOG_STAT_INFO* logStatListNext(LOG_STAT_INFO* pode)
{
	if(pode != NULL)
	{
		LOG_STAT_INFO* n = (LOG_STAT_INFO*)list_item_next(pode);
		return n;
	}
	return NULL;
}


/******************************************************
 * 函数原型：	static int32_t logStatListNums(void)
 * 功能：		链表长度
 * 输入：		void
 * 返回：		int32_t 链表长度
******************************************************/
static int32_t logStatListNums(void)
{
	return file_num;
}


/******************************************************
 * 函数原型：	static LOG_STAT_INFO* logStatListByName(const uint8_t* fileName)
 * 功能：		遍历链表查找fileName是否在链表中
 * 输入：		const uint8_t* fileName 文件名
 * 返回：		LOG_STAT_INFO* 文件名对应的链表指针
******************************************************/
static LOG_STAT_INFO* logStatListByName(const uint8_t* fileName)
{
	LOG_STAT_INFO* pnode = NULL;

	for(pnode = logStatListHead(); pnode != NULL; pnode = logStatListNext(pnode))
	{
		if(mem_cmp(fileName, pnode->LogFileName, strlen((const char*)fileName)) == 0)
		{
			return pnode;
		}
	}

	return NULL;
}


/******************************************************
 * 函数原型：	static LOG_STAT_INFO* logStatListAdd(const LOG_STAT_INFO* pcnode)
 * 功能：		将新的结构体插入到链表
 * 输入：		const LOG_STAT_INFO* pcnode 需要插入的结构体
 * 返回：		LOG_STAT_INFO* 新的链表指针
******************************************************/
static LOG_STAT_INFO* logStatListAdd(const LOG_STAT_INFO* pcnode)
{
	struct stat statbuf;
	uint8_t szLogDir[256] 		= {0};
	uint8_t logFile[LOG_FILE_MAX_NAME];
	LOG_STAT_INFO* pnode;

	pnode = logStatListByName(pcnode->LogFileName);
	if(pnode != NULL)
	{
		GetConfigFileStringValue((uint8_t*)"LOG", (uint8_t*)"LogDir", (uint8_t*)"", szLogDir, sizeof(szLogDir), (uint8_t*)configAddr);//"Config.ini");
		snprintf((char*)logFile, sizeof(logFile)-1, "%s/%s.log", szLogDir, pcnode->LogFileName);
		lstat((const char*)logFile, &statbuf);
		pnode->LogFileSize = statbuf.st_size;
		pnode->LogFileMtime = statbuf.st_atime;
		// printf("log fileName is in list\n");
		return pnode;
	}
	/*Allocate a routing entry and populate it.*/
	pnode = (LOG_STAT_INFO*)memb_alloc(&logstatmem);
	if(pnode == NULL)
	{
		printf("no space for new file");
		return NULL;
	}

	//copy log info to pnode
	*pnode = *pcnode;
	list_add(logstatlist, pnode);
	pnode->LogFileSize = statbuf.st_size;
	pnode->LogFileMtime = statbuf.st_atime;
	file_num++;
	return pnode;
}


/******************************************************
 * 函数原型：	void logStatListRemove(LOG_STAT_INFO* pnode)
 * 功能：		删除链表中的结构体
 * 输入：		LOG_STAT_INFO* pnode 要删除的结构体指针
 * 返回：		void
******************************************************/
static void logStatListRemove(LOG_STAT_INFO* pnode)
{
	list_remove(logstatlist, pnode);
	memb_free(&logstatmem, pnode);
	file_num--;
}


/******************************************************
 * 函数原型：	void dirLogFileAdd(void)
 * 功能：		遍历目录添加所有LOG文件到链表
 * 输入：		void
 * 返回：		void
******************************************************/
static void dirLogFileAdd(void)
{
	LOG_STAT_INFO infoBuf;
	struct stat statbuf;
	uint8_t szLogDir[256] 		= {0};
	DIR* dp = NULL;
	struct dirent *entry;
	GetConfigFileStringValue((uint8_t*)"LOG", (uint8_t*)"LogDir", (uint8_t*)"", szLogDir, sizeof(szLogDir), (uint8_t*)configAddr);//"Config.ini");
	dp = opendir((char*)szLogDir);
	//chdir((char*)szLogDir);
	while((entry=readdir(dp))!=NULL)
	{
		lstat(entry->d_name, &statbuf);
		if(S_ISREG(statbuf.st_mode))
		{
			memcpy(infoBuf.LogFileName, entry->d_name, strlen(entry->d_name));
			infoBuf.LogFileSize = statbuf.st_size;
			//memcpy(infoBuf.LogFileMtime, statbuf.st_mtime, strlen(statbuf.st_mtime));
			infoBuf.LogFileMtime = statbuf.st_atime;
			//infoBuf.next = NULL;
			logStatListAdd(&infoBuf);	
		}
	}
}


/******************************************************
 * 函数原型：	static void logFileNumCheck(void)
 * 功能：		当超出最大文件数量时候删除日期最早文件
 * 输入：		void
 * 返回：		void
******************************************************/
static void logFileNumCheck(void)
{
	struct tm *info;
	uint32_t fileYear = 0;
	uint32_t fileMon = 0;
	uint32_t fileDay = 0;
	uint8_t fileName[LOG_FILE_MAX_NAME] = {0};
	uint8_t logFile[LOG_FILE_MAX_NAME] = {0};
	uint8_t szLogDir[256] = {0};
	uint32_t ret;
	LOG_STAT_INFO* pnode = NULL;
	if(logStatListNums() >= LOG_FILE_MAX_NUM)
	{
		dirLogFileAdd();
		pnode = logStatListHead();
		if(pnode != NULL)
		{
			info = localtime(&(pnode->LogFileMtime));
			fileYear = info->tm_year;
			fileMon = info->tm_mon;
			fileDay = info->tm_mday;
			memset(fileName, 0, LOG_FILE_MAX_NAME);
			memcpy(fileName, pnode->LogFileName, strlen((const char*)pnode->LogFileName));
		}
		for(pnode = logStatListHead(); pnode != NULL; pnode = logStatListNext(pnode))
		{
			info = localtime(&(pnode->LogFileMtime));
			if(info->tm_year < fileYear)
			{
				fileYear = info->tm_year;
				fileMon = info->tm_mon;
				fileDay = info->tm_mday;
				memcpy(fileName, (const char*)pnode->LogFileName, strlen((const char*)pnode->LogFileName));
			}
			else if(info->tm_year == fileYear && info->tm_mon < fileMon)
			{
				fileYear = info->tm_year;
				fileMon = info->tm_mon;
				fileDay = info->tm_mday;
				memcpy(fileName, (const char*)pnode->LogFileName, strlen((const char*)pnode->LogFileName));
			}
			else if(info->tm_year == fileYear && info->tm_mon == fileMon && info->tm_mday < fileDay)
			{
				fileYear = info->tm_year;
				fileMon = info->tm_mon;
				fileDay = info->tm_mday;
				memcpy(fileName, (const char*)pnode->LogFileName, strlen((const char*)pnode->LogFileName));
			}
			// printf("fileName:%s\n", pnode->LogFileName);
			// printf("fileTime:%s\n", ctime(&(pnode->LogFileMtime)));
		}
		logStatListRemove(logStatListByName(fileName));
		GetConfigFileStringValue((uint8_t*)"LOG", (uint8_t*)"LogDir", (uint8_t*)"", szLogDir, sizeof(szLogDir), (uint8_t*)configAddr);//"Config.ini");
		snprintf((char*)logFile, sizeof(logFile)-1, "%s/%s.log", szLogDir, fileName);
		// printf("logFile:%s\n", logFile);
		ret = remove((const char*)logFile);
		if(ret == 0)
		{
			// printf("remove %s\n", logFile);
		}
		else
		{
			// printf("remove %d", ret);
		}
		
	}
}


/******************************************************
 * 函数原型：	void logFileInit(void)
 * 功能：		LOG文件管理初始化
 * 输入：		void
 * 返回：		void
******************************************************/
void logFileInit(void)
{
	GetConfigValue();
	logStatListInit();
	dirLogFileAdd();
}
