// ��ϳ���
// �ɽ���, 2001.10.10

#include "logfile.h"


#define		BUFSIZE		1024

//CString	g_sLogFile;
CCriticalSection	log_xLogCrit;
char		log_szTitle[BUFSIZE]	= "δ�������� V1.0";			//? ���ܿռ䲻��
char		log_szFilename[BUFSIZE]	= "Logfile";
time_t		log_tStartServerTime	= 0;

////////////////////////////////////////////////////////////////////////////
// ��ʽ������ʱ�䴮
void	DateTime(char * buf20, time_t tInput)	// ����buf20��
{
	if(!tInput && buf20)
	{
		sprintf(buf20, "0000-00-00 00:00:00");
		return;
	}

	tm * pTm = localtime(&tInput);
	if(buf20)
		sprintf(buf20, "%04d-%02d-%02d %02d:%02d:%02d", 
					pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
}

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

void log_SaveFile(const char * pFilename, const char * pBuf, char* szCountHeadLine /*= NULL*/)
{
	CSingleLock xLock(&log_xLogCrit, true); 
	ASSERT(xLock.IsLocked());

	// д�ļ�
	time_t tCurrTime;
	time( &tCurrTime );
	tm *	pTmFile = localtime(&tCurrTime);
	char	szLogFile[1024] = "";				//? ���ܿռ䲻��
	if(!szCountHeadLine)
		sprintf(szLogFile, "dblog\\%s %04d-%02d-%02d.LOG", pFilename, pTmFile->tm_year+1900, pTmFile->tm_mon+1, pTmFile->tm_mday);
	else
		sprintf(szLogFile, "gamelog\\%s %04d-%02d-%02d.TXT", pFilename, pTmFile->tm_year+1900, pTmFile->tm_mon+1, pTmFile->tm_mday);

	FILE	* pFile;
	pFile = fopen(szLogFile, "a");
	if(pFile)
	{
		fseek(pFile, 0, SEEK_END);
		{
			if(ftell(pFile) == 0)
			{
				if(!szCountHeadLine)
				{
					char buf[2048];			//? 
					char bufStart[20];
					char bufCurr[20];
				
					DateTime(bufStart, log_tStartServerTime);
					DateTime(bufCurr, tCurrTime);
					int	nSec = (int)difftime(tCurrTime, log_tStartServerTime);
					sprintf(buf,	"################################################################################\n"
									"### %s\n"
									"### Start server time is %s\n"
									"################################################################################\n"
									"### Current time is %s\n"
									"### Server takes %d days  %02d:%02d:%02d\n"
									"################################################################################\n\n\n"
									, log_szTitle
									, bufStart
									, bufCurr
									, nSec / 86400, (nSec/3600)%24, (nSec/60)%60, nSec%60 );
					fputs(buf, pFile);
				}
				else
				{
					fputs(szCountHeadLine, pFile);
				}
			}
		}

		// �����Ϣ
		fputs(pBuf, pFile);
		fclose(pFile);
	}
}
DWORD MyExceptionFilter( DWORD dwExCode )
{
	LogSave( "Exception code:0X%08x",dwExCode );
	return EXCEPTION_EXECUTE_HANDLER;
}
