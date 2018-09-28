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
#include "AFXDISP.H"//2007114����:COleDateTime
#include "StrRes.h"

#define	MUTEX_SYSLOG
#ifdef	MUTEX_SYSLOG
CCriticalSection	log_xSysLogCrit;
#endif

//20070627����:��־���ر�־λ
BOOL g_bLogSave = true;
char g_szLogMemFile[128] = "";

//  [2007-10-8] ���	��������д��־λ
BOOL g_bItemLifeTimeSaveDB = TRUE;

//  [1/15/2008 ���]
BOOL g_bEnableEquipCreate = TRUE;	//	װ������
BOOL g_bEnableTopListRefresh = TRUE;	//	�������а�
BOOL g_bEnableSynModificate = TRUE;		//	���ξ�����Ƹ�뿪������
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
// ��ʽ������ʱ�䴮
void	DateTime(char * buf20, time_t tInput /*= 0*/)	// ����buf20��
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

void DateTimeStamp(char * buf15, time_t tInput /*= 0*/)	// ����buf15��
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
// return: ʱ���ǩnDate����ָ������nDays
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
	//20070627����:д��־��־λ---------
	if(!g_bLogSave)
		return;
	//----------------------------------

#ifdef	MUTEX_SYSLOG
	CSingleLock(&log_xSysLogCrit, true);
#endif


	//�����ڴ��ļ���ʽ
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
	
	
	//�����ڴ��ļ���ʽ
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
	
	
	//�����ڴ��ļ���ʽ
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
	
	
	//�����ڴ��ļ���ʽ
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
		//20070402����:ʱ���.����
	case TIME_EPOCH_DAY:
		{
			//2007115����:------------------------
			//ԭ�㷨�����
//			dwTime = ::time(NULL);//��λ:��
//			dwTime = dwTime/60;//��
//			dwTime = dwTime/60;//Сʱ
//			dwTime = dwTime/24;//��

			//����1970 1�� 1�� 0:00:00 ������
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
//20061020����.���������д.������Ĭ��RAND_MAX 0x7fff��ߵ�10000W.
//static _int64 RandSeed = 3721 ;
int RandGet(int nMax, BOOL bReset)
{
	if( nMax == 0 )
		return 0;
	if( nMax >1000000000 )
	{
		LOGWARNING("RandGet()nMaxֵΪ%d,��������,������������",nMax);
	}
	if ( bReset )
		::srand( (unsigned)TimeGet() );

	int newRand;
	newRand=::rand()%1000*10000 + ::rand()%10000;
	// pengfeng ��������������λ [12/18/2007]//0~9999999,�����߽�
	double num = 0;
	num = ::rand()%100*10000000;
	newRand += num;//0~999999999,�����߽�

	return newRand%nMax;

	/*
	if( nMax == 0 )
		return 0;
	if ( bReset )
		::srand( (unsigned)TimeGet() );
	return ::rand() % nMax;
	*/
//06.10.18����ע�����²��޸�,ԭ��������������ĳЩϵͳ�³���
	/*
	//20061019����.������-----------------
	if (nMax <=0)
	{
		LOGWARNING("RandGet()nMaxֵΪ%d",nMax);
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
	//20061019����.������-----------------
	if (final <=0)
	{
		LOGWARNING("RandGet()finalֵΪ%d,nMaxֵΪ%d,RandSeed��ֵΪ%d,iֵΪ%f",final,nMax,RandSeed,i);
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
		// x ����
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
		// y ����
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
//06.10.18����ע������,���޸�
//��Ϊ������Ϸֻ�ڼ���ʺź�����ʱ���ô˺���,���Ը�Ϊֻ��Ϊ��������ĸ,����Υ��
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
			//20070615����--------------
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

	if (strcmp(pszString,g_objStrRes.GetStr(11024) ) == 0)//"��"
		return false;

	if (strstr(pszString, "��"))
		return false;

	if (strstr(pszString, "��"))
	{
		return false;
	}

	if (strstr(pszString, "��"))
		return false;

	if (strstr(pszString, "��"))
		return false;

	if (strstr(pszString, "��"))
		return false;

	if (strstr(pszString, "��"))
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

	if (strstr(pszString, "���"))
		return false;

	if (strstr(pszString, "�У�"))
		return false;

	if (strstr(pszString, "���"))
		return false;

	if (strstr(pszString, "�ǣ�"))
		return false;

	if (strstr(pszString, "NPC"))
		return false;

	if (strstr(pszString, "npc"))
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17029)))//"ʢ��"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17030)))//"����"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17031)))//"ϵͳ"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17032)))//"�߻�"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17033)))//"����"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17034)))//"����"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17035)))//"�ͷ�"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17036)))//"����"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17037)))//"�콱"
		return false;

	// ��Ϸ������
	if (strstr(pszString, g_objStrRes.GetStr(17038)))//"����"
		return false;
	if (strstr(pszString, g_objStrRes.GetStr(17039)))//"����"
		return false;
	if (strstr(pszString, g_objStrRes.GetStr(17040)))//"����"
		return false;
	if (strstr(pszString, g_objStrRes.GetStr(17041)))//"����"
		return false;
	if (strstr(pszString, g_objStrRes.GetStr(17042)))//"����"
		return false;
	if (strstr(pszString, g_objStrRes.GetStr(17043)))//"���"
		return false;
	if (strstr(pszString, g_objStrRes.GetStr(17044)))//"������"
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
		//case '\''://20090511haocq��˫���Ų�����
		//case '"':
			return false;
		}
	}

	return ::DisableWordCheck(pszString, bNewLineEnable);
}

//////////////////////////////////////////////////////////////////////
BOOL DisableWordCheck(char* pszString, BOOL bNewLineEnable)
{
	if (strstr(pszString, g_objStrRes.GetStr(17045)))//"����"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17046)))//"̨��"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17047)))//"����"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17048)))//"���"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17049)))//"��־"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17050)))//"����"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17051)))//"����"
		return false;	

	if (strstr(pszString, g_objStrRes.GetStr(17052)))//"���"
		return false;

	if (strstr(pszString, g_objStrRes.GetStr(17053)))//"����"
		return false;	

	if (strstr(pszString, g_objStrRes.GetStr(17029)))//"ʢ��"
		return false;

	if (strstr(pszString, "fuck"))
		return false;

	if (strstr(pszString, "FUCK"))
		return false;
	return ::StringCheck(pszString, bNewLineEnable);
}

//////////////////////////////////////////////////////////////////////////
//	20070709 ��� ������Ʒ��¼
void	MyLogSaveCostlyItem(const char* pszServerName/*��������,����*/,
							int	nAccountID/*�˺�ID*/, int nCharID/*��ɫID*/, const char* pszCharName/*��ɫ����*/,
							int nOtherAccountID/*��һ�˺�ID*/, int nOtherCharID/*��һ��ɫID*/, const char* pszOtherCharName/*��һ��ɫ����*/,
							int nType/*��������*/, int nItemTypeID/*��Ʒ����*/, int	nItemID/*��ƷID*/, const char* pszItemName/*��Ʒ����*/, int nItemCount/*����*/)
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
//[��;���� 2009.06.10]�ʽ�������Ʒ�����¼
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
		LOGWARNING("NumConversion()nConvNumΪ%d,������Χ!����0",nConvNum);
		return 0;
	}

	const int _conversion_num = nConvNum;//n����ת��
	
	int nResult = 0;
	for(int i=0;//��λ��
	nTrainPoint&&i<9;//��߲�����9λ
	i++)
	{
		nResult += pow((double)10,(int)i)*(nTrainPoint%_conversion_num);
		nTrainPoint = nTrainPoint/_conversion_num;
	}
	return nResult;
}