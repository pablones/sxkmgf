// ��ϳ���
// �ɽ���, 2001.10.10

#include <afxmt.h>
#include "common.h"
#include "basefunc.h"
#include "logfile.h"

#define		BUFSIZE		1024

//CString	g_sLogFile;
#define	MUTEX_LOGFILE
#ifdef	MUTEX_LOGFILE
CCriticalSection	log_xLogCrit;
#endif
char		log_szTitle[BUFSIZE]	= "δ�������� V1.0";			//? ���ܿռ䲻��
char		log_szFilename[BUFSIZE]	= "Logfile";
time_t		log_tStartServerTime	= 0;

////////////////////////////////////////////////////////////////////////////
// ��ʼ��LOG�ļ�
void InitLog(const char * pTitle, const char * pFilename, time_t tStart)
{
	if(pTitle)
	{
		int nLen = strlen(pTitle);
		if(nLen >= BUFSIZE-1)
			nLen = BUFSIZE-1;
		strncpy(log_szTitle, pTitle, nLen+1);
		log_szTitle[BUFSIZE-1]	= 0;
	}

	if(pFilename)
	{
		int nLen = strlen(pFilename);
		if(nLen >= BUFSIZE-1)
			nLen = BUFSIZE-1;
		strncpy(log_szFilename, pFilename, nLen+1);
		log_szFilename[BUFSIZE-1]	= 0;
	}

	log_tStartServerTime = tStart;
}

void LOSEDATA(const char * sFormat, ...)
{
	va_list argptr;
	va_start( argptr, sFormat);     /* Initialize variable arguments. */

	char	buf[4096] = "";					/* ?????�ռ���ܲ��� */
	int		ret = vsprintf(buf, sFormat, argptr);
	strcat(buf, "\n");
//	if(!(ret < 4096-2)) log_Save(x, "ASSERT(ret < 4096-2)");	/* ?????�ռ���ܲ��� */

	va_end( argptr );              /* Reset variable arguments.      */

	log_Save("LOSE_DATA", buf);
}

// bool log_SaveByMemfile(const char * pFilename, const char * pBuf)
// {
// 	if(strlen(g_szLogMemFile) == 0)
// 		return false;
// 	// TODO: Add your control notification handler code here
// 	HANDLE m_hMapFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS,
// 		FALSE,g_szLogMemFile);
// 	
// 	if(m_hMapFile)
// 	{
// 		stLogData* pLogData = (stLogData*)MapViewOfFile(m_hMapFile,FILE_MAP_ALL_ACCESS,
// 			0,0,0);
// 		
// 		if(pLogData->unLast > pLogData->unFirst)
// 		{
// 			if(pLogData->unLast == 99
// 				&& pLogData->unFirst == 0)
// 				return false;
// 		}
// 		else
// 		{
// 			if(pLogData->unFirst == pLogData->unLast + 1)
// 				return false;
// 		}
// 		
// 		strcpy(pLogData->arrayFile[pLogData->unLast],pFilename);
// 		sprintf(pLogData->arrayLog[pLogData->unLast],pBuf);
// 		pLogData->unLast ++;
// 		if(pLogData->unLast >= 100)
// 			pLogData->unLast = 0;
// 		
// 		FlushViewOfFile(pLogData,sizeof(stLogData));
// 		UnmapViewOfFile(pLogData);
// 		CloseHandle(m_hMapFile);
// 		return true;
// 	}
// 	
// 	return false;
// }

void log_Save(const char * pFilename, const char * pBuf)
{
#ifdef _DEBUG
	::OutputDebugString(pBuf);
#endif
	//20070627����:д��־��־λ---------
	if(!g_bLogSave)
		return;
	//----------------------------------
	
#ifdef	MUTEX_LOGFILE
	CSingleLock(&log_xLogCrit, true);
#endif

	//�����ڴ��ļ���ʽ
	log_SaveByMemfile(pFilename,pBuf);
	return;

	// д�ļ�
	time_t tCurrTime;
	time( &tCurrTime );
	tm *	pTmFile = localtime(&tCurrTime);
	char	szLogFile[1024] = "";				//? ���ܿռ䲻��
	sprintf(szLogFile, "%s %04d-%02d-%02d.LOG", pFilename, pTmFile->tm_year+1900, pTmFile->tm_mon+1, pTmFile->tm_mday);

	FILE	* pFile;
	pFile = fopen(szLogFile, "a");
	if(pFile)
	{
		fseek(pFile, 0, SEEK_END);
		{
			if(ftell(pFile) == 0)
			{
				char buf[2048];			//? 
				char bufStart[20];
				char bufCurr[20];
			
				DateTime(bufStart, log_tStartServerTime);
				DateTime(bufCurr, tCurrTime);
				int	nSec = (int)difftime(tCurrTime, log_tStartServerTime);
				sprintf(buf,	"================================================================================\n"
								"=== %s\n"
								"=== Start server time is %s\n"
								"================================================================================\n"
								"=== Current time is %s\n"
								"=== Server takes %d days  %02d:%02d:%02d\n"
								"================================================================================\n\n\n"
								, log_szTitle
								, bufStart
								, bufCurr
								, nSec / 86400, (nSec/3600)%24, (nSec/60)%60, nSec%60 );
				fputs(buf, pFile);
			}
		}

		// �����Ϣ
		if(ftell(pFile) < 500*1000*1000)
			fputs(pBuf, pFile);
		fclose(pFile);
	}
}

//////////////////////////////////////////////////////////////////////
CLogFile::CLogFile(LPCTSTR szFile, LPCTSTR szTitle/*=NULL*/, int nMaxSize/*=500*1000*1000*/)
{
}

//////////////////////////////////////////////////////////////////////
void CLogFile::SaveLog(LPCTSTR szHead, LPCTSTR szFormat, ...)
{
}

//////////////////////////////////////////////////////////////////////
void CLogFile::SaveRaw(LPCTSTR szLog)
{
}