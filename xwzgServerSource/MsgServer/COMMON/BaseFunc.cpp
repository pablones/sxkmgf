#include <afxmt.h>
#include <windows.h>
#include <mmsystem.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include "common.h"
#include "BaseFunc.h"
#include "AFXDISP.H"//2007114修罗:COleDateTime
#include "StrRes.h"

#define	MUTEX_SYSLOG
#ifdef	MUTEX_SYSLOG
CCriticalSection	log_xSysLogCrit;
#endif

//20070627修罗:日志开关标志位
BOOL g_bLogSave = true;
char g_szLogMemFile[128] = "";

//  [2007-10-8] 朱斌	生命周期写标志位
BOOL g_bItemLifeTimeSaveDB = TRUE;

//  [1/15/2008 朱斌]
BOOL g_bEnableEquipCreate = TRUE;	//	装备打造
BOOL g_bEnableTopListRefresh = TRUE;	//	屏蔽排行榜
BOOL g_bEnableSynModificate = TRUE;		//	屏蔽军团招聘与开除功能
BOOL g_bEnableDatabaseReconnect = FALSE;
BOOL g_bEnableFatigue = TRUE;

BYTE g_cbEncryptKey=0;
/////////////////////////////////////////////////////////////////////////////
bool log_SaveByMemfile(const char * pFilename, const char * pBuf)
{
	if(strlen(g_szLogMemFile) == 0)
		return false;
	// TODO: Add your control notification handler code here
	HANDLE m_hMapFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS,
		FALSE,g_szLogMemFile);
	
	if(m_hMapFile)
	{
		stLogData* pLogData = (stLogData*)MapViewOfFile(m_hMapFile,FILE_MAP_ALL_ACCESS,
			0,0,0);
		
		if(pLogData->unLast > pLogData->unFirst)
		{
			if(pLogData->unLast == 99
				&& pLogData->unFirst == 0)
				return false;
		}
		else
		{
			if(pLogData->unFirst == pLogData->unLast + 1)
				return false;
		}
		
		strcpy(pLogData->arrayFile[pLogData->unLast],pFilename);
		sprintf(pLogData->arrayLog[pLogData->unLast],pBuf);
		pLogData->unLast ++;
		if(pLogData->unLast >= 100)
			pLogData->unLast = 0;
		
		FlushViewOfFile(pLogData,sizeof(stLogData));
		UnmapViewOfFile(pLogData);
		CloseHandle(m_hMapFile);
		return true;
	}
	
	return false;
}

String	DumpBuffer(const char* buf, int nSize)
{
	IF_NOT(buf && nSize > 0 && nSize <= 256)
		return String();

	String str;
	for(int i = 0; i < nSize; i++)
	{
		char bufChar[255];
		sprintf(bufChar, "%02x", (*(buf++) & 0xFF));
		str	+= bufChar;
		if((i % 16) == 15 && i != nSize-1)
			str += ' ';
		else if((i % 4) == 3 && i != nSize-1)
			str	+= '-';
	}
	return str;
}

/////////////////////////////////////////////////////////////////////////////
// szFormat: "%04d-%02d-%02d %02d:%02d:%02d"
// 格式化日期时间串
void	DateTime(char * buf20, time_t tInput /*= 0*/)	// 填入buf20中
{
	if(!buf20)
		return;

	if(tInput == 0)
		tInput = time(NULL);

	tm * pTm = localtime(&tInput);
	if(buf20)
		sprintf(buf20, "%04d-%02d-%02d %02d:%02d:%02d", 
					pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
}

////////////////////////////////////////////////////////////////////////////
bool FormatDateTime(char* buf, const char* szFormat, time_t tInput /*= 0*/)
{
	if(!buf || !szFormat)
		return false;

	if(tInput == 0)
		tInput = time(NULL);

	tm * pTm = localtime(&tInput);
	sprintf(buf, szFormat, 
			pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);

	return true;
}

void DateTimeStamp(char * buf15, time_t tInput /*= 0*/)	// 填入buf15中
{
	if(!buf15)
		return;

	if(tInput == 0)
		tInput = time(NULL);

	tm * pTm = localtime(&tInput);
	if(buf15)
		sprintf(buf15, "%04d%02d%02d%02d%02d%02d", 
					pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
}

////////////////////////////////////////////////////////////////////////////
int DateStamp(time_t tTime/*=0*/)
{
	time_t	tInput;
	if(tTime)
		tInput	= tTime;
	else
		tInput = time(NULL);

	tm * pTm = localtime(&tInput);
	return (pTm->tm_year+1900)	* 10000
			+ (pTm->tm_mon+1) * 100
			+ pTm->tm_mday;
		/*	+ pTm->tm_hour0;*/
// 	time_t timep;
// 	time(&timep);
// 	return timep;
}

////////////////////////////////////////////////////////////////////////////
// return: 时间标签nDate加上指定天数nDays
// return -1: error
int DateStampPass(int nDate, int nDays)
{
	time_t	tCurr = time(NULL);
	tm	tmComp;
	memset(&tmComp, 0, sizeof(tm));
	tmComp.tm_year	= nDate/10000 - 1900;
	tmComp.tm_mon	= (nDate/100)%100 - 1;
	tmComp.tm_mday	= nDate%100 + nDays;

	time_t	tComp	= mktime(&tmComp);
	if(tComp == -1)
		return -1;

	return DateStamp(tComp);
}

//////////////////////////////////////////////////////////////////////
bool IsActiveTime(time_t tCurr, unsigned long nFormat)		// DDWWHHMMSS
{
	tm* pTm = localtime(&tCurr);
	CHECKF(pTm);

	if(nFormat / 100000000)		// day per month
	{
		if(pTm->tm_mday == int(nFormat/100000000)
			&& pTm->tm_hour == int(nFormat/10000) % 100
			&& pTm->tm_min	== int(nFormat/100) % 100
			&& pTm->tm_sec	== int(nFormat % 100)
			)
			return true;
	}
	else if(nFormat / 1000000)		// day per weak
	{
		if(((pTm->tm_wday+6)%7)+1 == int(nFormat/1000000)
			&& pTm->tm_hour == int(nFormat/10000) % 100
			&& pTm->tm_min	== int(nFormat/100) % 100
			&& pTm->tm_sec	== int(nFormat % 100)
			)
			return true;
	}
	else			// time per day
	{
		if(pTm->tm_hour == int(nFormat/10000) % 100
			&& pTm->tm_min	== int(nFormat/100) % 100
			&& pTm->tm_sec	== int(nFormat % 100)
			)
			return true;
	}
	return false;
}

bool IsBetween(int nCurr, int nBegin, int nEnd)
{
	if(nBegin <= nEnd)
	{
		if(nBegin <= nCurr && nCurr < nEnd)
			return true;
	}
	else // if(nEnd < nBegin)
	{
		if(nCurr < nEnd || nBegin <= nCurr)
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool IsTimeRange(unsigned long nBegin, unsigned long nEnd, time_t tCurr/*=0*/)		// DDWWHHMMSS
{
	if(tCurr == 0)
		tCurr = time(NULL);

	tm* pTm = localtime(&tCurr);
	CHECKF(pTm);

	if(nBegin / 100000000)		// per month
	{
		if(IsBetween(pTm->tm_mday*100*1000000 + pTm->tm_hour*10000 + pTm->tm_min*100 + pTm->tm_sec, nBegin, nEnd))
			return true;
	}
	else		// per weak or day ...
	{
		if(IsBetween(pTm->tm_wday*1000000 + pTm->tm_hour*10000 + pTm->tm_min*100 + pTm->tm_sec, nBegin, nEnd))
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
char*	StrStrCh(const char* string, const char* strCharSet)
{
	CHECKF(string && strCharSet);
	unsigned int	nLen	= strlen(strCharSet);
	if(nLen == 0 || strlen(string) < nLen)
		return NULL;

	char	head	= *strCharSet;
	const char* ptr = string;
	while(*ptr)
	{
		while(*ptr && *ptr != head)
		{
			unsigned char uch = static_cast<unsigned char>(*ptr);
			if(uch >= 0x81 && uch <= 0xFE && *(ptr+1))
				ptr++;
			ptr++;
		}
		if(*ptr && strncmp(ptr, strCharSet, nLen) == 0)
			return const_cast<char*>(ptr);
		ptr++;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
DWORD SysTimeGet(void)
{
	time_t long_time;
	time( &long_time );                /* Get time as long integer. */

	struct tm *pTime;
	pTime = localtime( &long_time ); /* Convert to local time. */

	DWORD dwTime	=pTime->tm_hour*100 + pTime->tm_min;
	return dwTime;
}

//////////////////////////////////////////////////////////////////////
DWORD SysTimeGetEx(void)
{
	time_t long_time;
	time( &long_time );                /* Get time as long integer. */

	struct tm *pTime;
	pTime = localtime( &long_time ); /* Convert to local time. */

	DWORD dwTime	=	pTime->tm_mon *100000000 +
						pTime->tm_mday*1000000 +
						pTime->tm_hour*10000 + 
						pTime->tm_min *100 +
						pTime->tm_sec;
	return dwTime;
}

/////////////////////////////////////////////////////////////////////////////
void LogSave (const char* fmt, ...)
{	
	//20070627修罗:写日志标志位---------
	if(!g_bLogSave)
		return;
	//----------------------------------

#ifdef	MUTEX_SYSLOG
	CSingleLock(&log_xSysLogCrit, true);
#endif


	//尝试内存文件方式
	char buffer[4096];
    vsprintf( buffer, fmt, (char*) ((&fmt)+1) );

	time_t ltime;
	time( &ltime );
	sprintf(buffer, "%s -- %s", buffer, ctime(&ltime));
	log_SaveByMemfile("syslog/sys",buffer);
	return;

// 	const DWORD _MAX_LOGLINE	=1000000;
// 	static DWORD dwLineCount	=0;
// 
// 	time_t ltime;
// 	time( &ltime );
// 
// 	struct tm *pTime;
// 	pTime = localtime( &ltime ); /* Convert to local time. */
// 
//     char buffer[4096];
//     vsprintf( buffer, fmt, (char*) ((&fmt)+1) );
// 
// 	char szLogName[256];
// 	sprintf(szLogName, "syslog/sys %d-%d %u.log", (pTime->tm_mon+1)%12, pTime->tm_mday, ++dwLineCount/_MAX_LOGLINE);
// 
// 	FILE* fp	=fopen(szLogName, "a+");
// 	if(!fp)
// 		return;
// 
// 	fprintf(fp, "%s -- %s", buffer, ctime(&ltime));
// 	fclose(fp);	
// 
// #ifdef _DEBUG
// 	afxDump<<buffer<<"\n";
// #endif

}

// #include "../msgserver/worldkernel/WorldKernel.h"


void MyLogSave2 (const char* pszName, const char* fmt, ...)
{	
#ifdef	MUTEX_SYSLOG
	CSingleLock(&log_xSysLogCrit, true);
#endif

	if (!pszName || !fmt)
		return;

	time_t ltime;
	time( &ltime );

	struct tm *pTime;
	pTime = localtime( &ltime ); /* Convert to local time. */

    char buffer[1024];
    vsprintf( buffer, fmt, (char*) ((&fmt)+1) );

	char szLogName[256];
	sprintf(szLogName, "%s %d-%d.log", pszName, (pTime->tm_mon+1)%12, pTime->tm_mday);

	CreateDirectory("EventIssue",NULL);
	FILE* fp	=fopen(szLogName, "a+");
	if(!fp)
		return;

	fprintf(fp, "%s\n", buffer);
	fclose(fp);	
}

void	FinanceLog(	 LPCSTR strServerName,
						 LPCSTR strEventSource,
						 LPCSTR strEventName,
						 LPCSTR strCurrency,
						 LPCSTR strActiveObj,
						 LPCSTR strPassiveObj,
						 LPCSTR strValue1,
						 LPCSTR strValue2,
						 LPCSTR strValue3,
						 LPCSTR strValue4,
						 LPCSTR strValue5,
						 LPCSTR strValue6
					)
{
	MyLogSave2("EventIssue/EventIssueLog",
			  "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",
			  COleDateTime::GetCurrentTime().Format(),
			  strServerName,
			  strEventSource,
			  strEventName,
			  strCurrency,
			  strActiveObj,
			  strPassiveObj,
			  strValue1,
			  strValue2,
			  strValue3,
			  strValue4,
			  strValue5,
			  strValue6);
}

/////////////////////////////////////////////////////////////////////////////
void MyLogSave (const char* pszName, const char* fmt, ...)
{
	if(!g_bLogSave)
		return;
		
#ifdef	MUTEX_SYSLOG
	CSingleLock(&log_xSysLogCrit, true);
#endif
	
	
	//尝试内存文件方式
	char buffer[4096];
    vsprintf( buffer, fmt, (char*) ((&fmt)+1) );

	time_t ltime;
	time( &ltime );
	sprintf(buffer, "%s -- %s", buffer, ctime(&ltime));
	log_SaveByMemfile(pszName,buffer);
	return;

// 	if (!pszName || !fmt)
// 		return;
// 
// 	const DWORD _MAX_LOGLINE	=1000000;
// 	static DWORD dwLineCount	=0;
// 
// 	time_t ltime;
// 	time( &ltime );
// 
// 	struct tm *pTime;
// 	pTime = localtime( &ltime ); /* Convert to local time. */
// 
//     char buffer[1024];
//     vsprintf( buffer, fmt, (char*) ((&fmt)+1) );
// 
// 	char szLogName[256];
// 	sprintf(szLogName, "%s %d-%d %u.log", pszName, (pTime->tm_mon+1)%12, pTime->tm_mday, ++dwLineCount/_MAX_LOGLINE);
// 
// 	FILE* fp	=fopen(szLogName, "a+");
// 	if(!fp)
// 		return;
// 
// 	fprintf(fp, "%s -- %s", buffer, ctime(&ltime));
// 	fclose(fp);	
}

/////////////////////////////////////////////////////////////////////////////
void DebugSave (const char* fmt, ...)
{	
    char szMsg[1024];
    vsprintf( szMsg, fmt, (char*) ((&fmt)+1) );
	strcat(szMsg, "\n");

	::OutputDebugString(szMsg);
}

//////////////////////////////////////////////////////////////////////
void MsgLogSave	(const char* pszFileName, const char* fmt, ...)
{
	if(!g_bLogSave)
		return;
	
#ifdef	MUTEX_SYSLOG
	CSingleLock(&log_xSysLogCrit, true);
#endif
	
	
	//尝试内存文件方式
	char buffer[4096];
    vsprintf( buffer, fmt, (char*) ((&fmt)+1) );

	time_t ltime;
	time( &ltime );
	sprintf(buffer, "%s -- %s", buffer, ctime(&ltime));
	log_SaveByMemfile(pszFileName,buffer);
	return;

// 	time_t ltime;
// 	time( &ltime );
// 
// 	struct tm *pTime;
// 	pTime = localtime( &ltime ); /* Convert to local time. */
// 
//     char buffer[2048];
//     vsprintf( buffer, fmt, (char*) ((&fmt)+1) );
// 
// 	char szLogName[256];
// 	sprintf(szLogName, "%s %d-%d.log", pszFileName, (pTime->tm_mon+1)%12, pTime->tm_mday);
// 
// 	FILE* fp	=fopen(szLogName, "a+");
// 	if(!fp)
// 		return;
// 
// 	fprintf(fp, "%s###%d-%d-%d %d:%d:%d\n", buffer,pTime->tm_year+1900,(pTime->tm_mon+1)%12,pTime->tm_mday,pTime->tm_hour,pTime->tm_min,pTime->tm_sec);
// 	fclose(fp);	
}

//////////////////////////////////////////////////////////////////////
void GmLogSave	(const char* fmt, ...)
{
	if(!g_bLogSave)
		return;
	
#ifdef	MUTEX_SYSLOG
	CSingleLock(&log_xSysLogCrit, true);
#endif
	
	
	//尝试内存文件方式
	char buffer[4096];
    vsprintf( buffer, fmt, (char*) ((&fmt)+1) );

	time_t ltime;
	time( &ltime );
	sprintf(buffer, "%s -- %s", buffer, ctime(&ltime));
	log_SaveByMemfile("gmlog/gm",buffer);
	return;

// 	time_t ltime;
// 	time( &ltime );
// 
// 	struct tm *pTime;
// 	pTime = localtime( &ltime ); /* Convert to local time. */
// 
//     char buffer[1024];
//     vsprintf( buffer, fmt, (char*) ((&fmt)+1) );
// 
// 	char szLogName[256];
// 	sprintf(szLogName, "gmlog/gm %d-%d.log", (pTime->tm_mon+1)%12, pTime->tm_mday);
// 
// 	FILE* fp	=fopen(szLogName, "a+");
// 	if(!fp)
// 		return;
// 
// 
// 	fprintf(fp, "%s -- %s", buffer, ctime(&ltime));
// 	fclose(fp);	
}

//////////////////////////////////////////////////////////////////////
int	Double2Int(double dValue)
{
	if((int)(dValue+0.5) >(int)dValue)
		return int(dValue)+1;
	else
		return int(dValue);
}


//////////////////////////////////////////////////////////////////////
//DWORD TimeGet(void)
//{
//	return timeGetTime();
//}
DWORD TimeGet(TIME_TYPE type/*=TIME_MILLISECOND*/)
{
	DWORD dwTime = 0;
	switch(type)
	{
	case TIME_SECOND:
		dwTime = ::time(NULL);
		break;

	case TIME_MINUTE:
		{
			time_t long_time;
			time( &long_time );                /* Get time as long integer. */

			struct tm *pTime;
			pTime = localtime( &long_time ); /* Convert to local time. */

			dwTime	=	pTime->tm_year%100*100000000 +
						(pTime->tm_mon+1)*1000000 +
						pTime->tm_mday*10000 +
						pTime->tm_hour*100 + 
						pTime->tm_min;
		}
		break;

	case TIME_DAY:
		{
			time_t long_time;
			time( &long_time );                /* Get time as long integer. */

			struct tm *pTime;
			pTime = localtime( &long_time ); /* Convert to local time. */

			dwTime	=	pTime->tm_year*10000 +
						(pTime->tm_mon+1)*100 +
						pTime->tm_mday;
		}
		break;

	case TIME_DAYTIME: 
		{
			time_t long_time;
			time( &long_time );                /* Get time as long integer. */

			struct tm *pTime;
			pTime = localtime( &long_time ); /* Convert to local time. */

			dwTime	=	pTime->tm_hour*10000 + 
						pTime->tm_min *100 +
						pTime->tm_sec;
		}
		break;

	case TIME_STAMP: 
		{
			time_t long_time;
			time( &long_time );                /* Get time as long integer. */

			struct tm *pTime;
			pTime = localtime( &long_time ); /* Convert to local time. */

			dwTime	=	(pTime->tm_mon+1)*100000000 +
						pTime->tm_mday*1000000 +
						pTime->tm_hour*10000 + 
						pTime->tm_min *100 +
						pTime->tm_sec;
		}
		break;
		//20070402修罗:时间点.天数
	case TIME_EPOCH_DAY:
		{
			//2007115修罗:------------------------
			//原算法有误差
//			dwTime = ::time(NULL);//单位:秒
//			dwTime = dwTime/60;//分
//			dwTime = dwTime/60;//小时
//			dwTime = dwTime/24;//天

			//距离1970 1月 1日 0:00:00 的天数
			COleDateTime dt(1970,1,1,0,0,0);
			COleDateTimeSpan dts=COleDateTime::GetCurrentTime()-dt;
			dwTime = dts.GetDays();
			//-------------------------------------
		}
		break;
	default:
		dwTime = ::timeGetTime();
		break;
	}

	return dwTime;
}


//////////////////////////////////////////////////////////////////////
//20061020修罗.随机函数重写.精度由默认RAND_MAX 0x7fff提高到10000W.
//static _int64 RandSeed = 3721 ;
int RandGet(int nMax, BOOL bReset)
{
	if( nMax == 0 )
		return 0;
	if( nMax >1000000000 )
	{
		LOGWARNING("RandGet()nMax值为%d,超出精度,但仍正常返回",nMax);
	}
	if ( bReset )
		::srand( (unsigned)TimeGet() );

	int newRand;
	newRand=::rand()%1000*10000 + ::rand()%10000;
	// pengfeng 将精度增加至亿位 [12/18/2007]//0~9999999,包括边界
	double num = 0;
	num = ::rand()%100*10000000;
	newRand += num;//0~999999999,包括边界

	return newRand%nMax;

	/*
	if( nMax == 0 )
		return 0;
	if ( bReset )
		::srand( (unsigned)TimeGet() );
	return ::rand() % nMax;
	*/
//06.10.18王玉波注释以下并修改,原来的随机数获得在某些系统下出错
	/*
	//20061019修罗.测试用-----------------
	if (nMax <=0)
	{
		LOGWARNING("RandGet()nMax值为%d",nMax);
		return 0;
	}
	//-----------------------------------
	if (bReset)
		RandSeed = ::TimeGet();

	_int64 x = 0xffffffff;
	double i =0;
	unsigned long final ;
	
	RandSeed *= ((_int64)134775813);
	RandSeed += 1 ;
	RandSeed = RandSeed % x ;
	i = ((double)RandSeed) / (double)0xffffffff ;
	final = (long) (nMax * i) ;
	//20061019修罗.测试用-----------------
	if (final <=0)
	{
		LOGWARNING("RandGet()final值为%d,nMax值为%d,RandSeed的值为%d,i值为%f",final,nMax,RandSeed,i);
		return 0;
	}
	//-----------------------------------

	return (int)final;
	*/
}

//////////////////////////////////////////////////////////////////////
double RandomRateGet(double dRange)
{
	double pi=3.1415926;

	int nRandom	=RandGet(999, false)+1;
	double a	=sin(nRandom*pi/1000);
	double b;
	if (nRandom >= 90)
		b	=(1.0+dRange)-sqrt(sqrt(a))*dRange;
	else
		b	=(1.0-dRange)+sqrt(sqrt(a))*dRange;

	return b;
}

//////////////////////////////////////////////////////////////////////
void DDALineEx(int x0, int y0, int x1, int y1, vector<POINT>& vctPoint)
{
	vctPoint.clear();
	if (x0 == x1 && y0 == y1)
		return;

	int dx = x1 - x0;
	int dy = y1 - y0;
	int abs_dx = abs(dx);
	int abs_dy = abs(dy);
	if(abs_dx > abs_dy)
	{
		int _0_5		= abs_dx * (dy>0 ? 1:-1);
		int numerator	= dy * 2;
		int denominator	= abs_dx * 2;
		// x 增益
		if(dx > 0)
		{
			// x0 ++
			for(int i = 1; i <= abs_dx; i ++)
			{
				POINT point;
				point.x = x0 + i;
				point.y = y0 + ((numerator*i + _0_5) / denominator);
				vctPoint.push_back(point);
			}
		}
		else if(dx < 0)
		{
			// x0 --
			for(int i = 1; i <= abs_dx; i ++)
			{
				POINT point;
				point.x = x0 - i;
				point.y = y0 + ((numerator*i + _0_5) / denominator);
				vctPoint.push_back(point);
			}
		}
	}
	else
	{
		int _0_5		= abs_dy * (dx>0 ? 1:-1);
		int numerator	= dx * 2;
		int denominator	= abs_dy * 2;
		// y 增益
		if(dy > 0)
		{
			// y0 ++
			for(int i = 1; i <= abs_dy; i ++)
			{
				POINT point;
				point.y = y0 + i;
				point.x = x0 + ((numerator*i + _0_5) / denominator);
				int nAmount = vctPoint.size();
				vctPoint.push_back(point);
			}
		}
		else if(dy < 0)
		{
			// y0 -- 
			for(int i = 1; i <= abs_dy; i ++)
			{
				POINT point;
				point.y = y0 - i;
				point.x = x0 + ((numerator*i + _0_5) / denominator);
				vctPoint.push_back(point);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
BOOL StringCheck(char* pszString, BOOL bNewLineEnable)
{
	if (!pszString)
		return false;

	int nLen	=strlen(pszString);
	for (int i=0; i < nLen; i++)

	{
/*
//06.10.18王玉波注释以下,并修改
//因为现在游戏只在检查帐号和密码时调用此函数,所以改为只可为数字与字母,其它违法
		if( !(( pszString[ i ] >= '0' && pszString[ i ] <= '9' )
			|| ( pszString[ i ] >= 'a' && pszString[ i ] <= 'z' ) 
			|| ( pszString[ i ] >= 'A' && pszString[ i ] <= 'Z' ))
			)
			return false;*/

		unsigned char c	=(unsigned char)pszString[i];
		if (c >= 0x81 && c <= 0xfe)	
		{
			if (i+1 >= nLen)
				return false;

			unsigned char c2	=(unsigned char)pszString[i+1];
			if (c2 < 0x40 && c2 > 0x7e && c2 < 0x80 && c2 > 0xfe)
				return false;
			else
				i++;
		}
		else
		{
			if (c==0x80)
				return false;
			else
			{
				if(bNewLineEnable)
				{
					if (c < ' ' && c != 0x0A && c != 0x0D)
						return false;
				}
				else
				{
					if (c < ' ')
						return false;
				}
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL NameStrCheck(char* pszString)
{
 	if (!pszString)
		return false;

	int nLen	=strlen(pszString);
	for (int i=0; i < nLen; i++)
	{
		unsigned char c	=pszString[i];
		if(c < ' ')
			return false;
		switch(c)
		{
			//20070615修罗--------------
		case '	'://tab
			//--------------------------
		case ' ':
		case ';':
		case ',':
		case '/':
		case '\\':
		case '=':
		case '%':
		case '@':
		case '\'':
		case '"':
		case '[':
		case ']':
		case '#':
		case '?':
		case '*':
		case '!':
			return false;
		}
	}

	if (strcmp(pszString,g_objStrRes.GetStr(11024) ) == 0)//"无"
		return false;

	if (strstr(pszString, "※"))
		return false;

	if (strstr(pszString, "？"))
	{
		return false;
	}

	if (strstr(pszString, ""))
		return false;

	if (strstr(pszString, "　"))
		return false;

	if (strstr(pszString, "【"))
		return false;

	if (strstr(pszString, "】"))
		return false;

	if (strstr(pszString, "GM"))
		return false;

	if (strstr(pszString, "gm"))
		return false;

	if (strstr(pszString, "PM"))
		return false;

	if (strstr(pszString, "pm"))
		return false;

	if (strstr(pszString, "SYSTEM"))
		return false;

	if (strstr(pszString, "system"))
		return false;

	if (strstr(pszString, "ｐｍ"))
		return false;

	if (strstr(pszString, "ＰＭ"))
		return false;

	if (strstr(pszString, "ｇｍ"))
		return false;

	if (strstr(pszString, "ＧＭ"))
		return false;

	if (strstr(pszString, "NPC"))
		return false;

	if (strstr(pszString, "npc"))
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17029)))//"盛科"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17030)))//"程序"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17031)))//"系统"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17032)))//"策划"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17033)))//"服务"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17034)))//"管理"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17035)))//"客服"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17036)))//"公告"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17037)))//"领奖"
		return false;

	// 游戏中特用
	if (strstr(pszString, g_objStrRes.GetStr(17038)))//"军团"
		return false;
	if (strstr(pszString, g_objStrRes.GetStr(17039)))//"卫兵"
		return false;
	if (strstr(pszString, g_objStrRes.GetStr(17040)))//"守卫"
		return false;
	if (strstr(pszString, g_objStrRes.GetStr(17041)))//"队伍"
		return false;
	if (strstr(pszString, g_objStrRes.GetStr(17042)))//"好友"
		return false;
	if (strstr(pszString, g_objStrRes.GetStr(17043)))//"大家"
		return false;
	if (strstr(pszString, g_objStrRes.GetStr(17044)))//"张三丰"
		return false;

	return ::DisableWordCheck(pszString);
}

//////////////////////////////////////////////////////////////////////
BOOL TalkStrCheck(char* pszString, BOOL bNewLineEnable)
{
	return true;
	if (!pszString)
		return false;

	int nLen	=strlen(pszString);
	for (int i=0; i < nLen; i++)
	{
		unsigned char c	=pszString[i];
		if(c < ' ' && c != 0x0d && c != 0x0a)
			return false;
		switch(c)
		{
		case '\\':
		//case '\''://20090511haocq单双引号不过虑
		//case '"':
			return false;
		}
	}

	return ::DisableWordCheck(pszString, bNewLineEnable);
}

//////////////////////////////////////////////////////////////////////
BOOL DisableWordCheck(char* pszString, BOOL bNewLineEnable)
{
	if (strstr(pszString, g_objStrRes.GetStr(17045)))//"法轮"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17046)))//"台独"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17047)))//"独立"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17048)))//"民进"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17049)))//"宏志"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17050)))//"泽民"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17051)))//"锦涛"
		return false;	

	if (strstr(pszString, g_objStrRes.GetStr(17052)))//"妈的"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17053)))//"鸡巴"
		return false;	

	if (strstr(pszString, g_objStrRes.GetStr(17029)))//"盛科"
		return false;

	if (strstr(pszString, "fuck"))
		return false;

	if (strstr(pszString, "FUCK"))
		return false;
	return ::StringCheck(pszString, bNewLineEnable);
}

//////////////////////////////////////////////////////////////////////////
//	20070709 朱斌 贵重物品记录
void	MyLogSaveCostlyItem(const char* pszServerName/*服务器名,无用*/,
							int	nAccountID/*账号ID*/, int nCharID/*角色ID*/, const char* pszCharName/*角色名称*/,
							int nOtherAccountID/*另一账号ID*/, int nOtherCharID/*另一角色ID*/, const char* pszOtherCharName/*另一角色名称*/,
							int nType/*操作类型*/, int nItemTypeID/*物品类型*/, int	nItemID/*物品ID*/, const char* pszItemName/*物品名称*/, int nItemCount/*数量*/)
{

	if(!g_bLogSave)
		return;

	char	str[2048];
	sprintf(str, "%d\t%d\t%s\t%d\t%d\t%s\t%d\t%d\t%d\t%s\t%d", nAccountID, nCharID,
			pszCharName, nOtherAccountID, nOtherCharID, pszOtherCharName,
			nType, nItemTypeID, nItemID, pszItemName, nItemCount);

	if(MY_LOG_SAVE_COSTLY_ITEM_TO_DISK == 1)
	{
#ifdef	MUTEX_SYSLOG
		CSingleLock(&log_xSysLogCrit, true);
#endif
		time_t ltime;
		time( &ltime );
		
		struct tm *pTime;
		pTime = localtime( &ltime ); /* Convert to local time. */
		
		char szLogName[256];
		sprintf(szLogName, "%s %d-%d-%d.log", "gmlog/CostlyItem", (pTime->tm_year + 1900),
			(pTime->tm_mon+1)%12, pTime->tm_mday);
		
		FILE* fp	=fopen(szLogName, "a+");
		if(!fp)
			return;
		
		fprintf(fp, "%d-%d-%d %d:%d:%d\t%s\n", pTime->tm_year+1900,(pTime->tm_mon+1)%12,pTime->tm_mday,pTime->tm_hour,pTime->tm_min,pTime->tm_sec,str);
		fclose(fp);
	}	
}

//////////////////////////////////////////////////////////////////////////
//[游途道标 2009.06.10]资金流向及物品流向记录
void PropertyStreamLog(STPROPERTYSTREAMLOG &stProStream)
{
	
	if(!g_bLogSave)
		return;
	
	char str[2048];
	sprintf(str, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
		stProStream.idSAccount,
		stProStream.idSource,	//1
		stProStream.dwSVas,		//2
		stProStream.dwSMoney,	//3
		stProStream.idTAccount,
		stProStream.idTarget,	//4
		stProStream.dwTVas,		//5
		stProStream.dwTMoney,	//6
		stProStream.dwVas,		//7
		stProStream.dwMoney,	//8
		stProStream.idItemType,	//9
		stProStream.idSItem,	//10
		stProStream.dwSNumber,	//11
		stProStream.idTItem,	//12
		stProStream.dwTNumber,	//13
		stProStream.dwSysVas,
		stProStream.dwSysMoney,
		stProStream.dwType);	//14
	
	time_t ltime;
	time(&ltime);
		
	struct tm *pTime;
	pTime = localtime(&ltime);
		
	char szLogName[256];
	sprintf(szLogName,"%s %d-%d-%d.log",
			"3017/ZiChan",
			(pTime->tm_year + 1900),
			(pTime->tm_mon+1)%12,
			pTime->tm_mday);
		
	FILE* fp =fopen(szLogName, "a+");
	if(!fp)
	{
		return;
	}
	fprintf(fp, "%d-%d-%d %d:%d:%d\t%s\n",
		    pTime->tm_year+1900,
			(pTime->tm_mon+1)%12,
			pTime->tm_mday,
			pTime->tm_hour,
			pTime->tm_min,
			pTime->tm_sec,
			str);
	fclose(fp);
}
//////////////////////////////////////////////////////////////////////////

//-------------------------------
int NumConversion(int nTrainPoint,int nConvNum)
{
	if(nConvNum<=1 || nConvNum >= 10)
	{
		LOGWARNING("NumConversion()nConvNum为%d,超出范围!返回0",nConvNum);
		return 0;
	}

	const int _conversion_num = nConvNum;//n进制转换
	
	int nResult = 0;
	for(int i=0;//计位数
	nTrainPoint&&i<9;//最高不超过9位
	i++)
	{
		nResult += pow((double)10,(int)i)*(nTrainPoint%_conversion_num);
		nTrainPoint = nTrainPoint/_conversion_num;
	}
	return nResult;
}