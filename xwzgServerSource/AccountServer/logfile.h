// ��ϳ���
// �ɽ���, 2001.10.10

#ifndef	LOGFILE_H
#define LOGFILE_H

#include <afxmt.h>
#pragma warning(disable:4786)

////////////////////////////////////////////////////////////////////////////////////
// ʹ��˵��

// ʹ��ǰ��Ҫ���� InitLog()

////////////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG
	#define PROFILE		//? �ú궨��Ϊ����ϵͳ���أ�ͨ����RELEASE����Ҳ���ܻ������ϵͳ
#endif

void InitLog(const char * pTitle, const char * pFilename, time_t tStart);
void log_SaveFile(const char * pFilename, const char * pBuf, char* szCountHeadLine = NULL);




/*
#define SAVELOG(x)		log_SaveLog(x)
#define SAVELOG1(x,y)	log_SaveLog(x,(y))
#define SAVELOG2(x,y,z)	log_SaveLog(x,(y),(z))
//*/

//void	log_SaveLog(const char * sFormat, ...);
//void	InitProfile();

#define log_InlineLog(x,y)\
inline void	log_InlineLog##y(const char * sFormat, ...)\
{\
	va_list argptr;\
	va_start( argptr, sFormat);     /* Initialize variable arguments. */\
\
	time_t ltime;\
	time( &ltime );\
	tm *	pTm = localtime(&ltime);\
	char	buf[4096] = "";					/* ?????�ռ���ܲ��� */\
	sprintf(buf, "%02d:%02d:%02d " #y ": ", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);\
	int		ret = vsprintf(buf + strlen(buf), sFormat, argptr);\
	strcpy(buf + strlen(buf), "\n");\
	if(!(ret < 4096-2)) log_SaveFile(x, "ASSERT(ret < 4096-2)");	/* ?????�ռ���ܲ��� */\
\
	va_end( argptr );              /* Reset variable arguments.      */\
\
	log_SaveFile(x,buf);\
}

inline void	LOGISPPOINT(const char * x, const char * sFormat, ...)
{
	va_list argptr;
	va_start( argptr, sFormat);     /* Initialize variable arguments. */

	time_t ltime;
	time( &ltime );
	tm *	pTm = localtime(&ltime);
	char	buf[4096] = "";					/* ?????�ռ���ܲ��� */
	sprintf(buf, "# %02d:%02d:%02d "  ": ", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
	int		ret = vsprintf(buf + strlen(buf), sFormat, argptr);
	strcpy(buf + strlen(buf), "\n");
	if(!(ret < 4096-2)) log_SaveFile(x, "ASSERT(ret < 4096-2)");	/* ?????�ռ���ܲ��� */

	va_end( argptr );              /* Reset variable arguments.      */

	log_SaveFile(x,buf);
}


//#else	// PROFILE

//#define SAVELOG(x)
//#define SAVELOG1(x,y)
//#define SAVELOG2(x,y,z)

//#define log_InlineLog(x) inline void	log_InlineLog##x(const char * sFormat, ...) { }


/////////////////////////////////////////////////////////////////
extern char		log_szFilename[];		// �ڲ�ʹ��

log_InlineLog(log_szFilename, WARNING)
log_InlineLog(log_szFilename, Message)
log_InlineLog(log_szFilename, ERROR)
log_InlineLog("server", server)
log_InlineLog("account", account)
log_InlineLog(log_szFilename, Other)
log_InlineLog(log_szFilename, CATCH_ERROR)
log_InlineLog("point", Point)


#define LOGWARNING		log_InlineLogWARNING
#define LOGMSG			log_InlineLogMessage
#define LOGERROR		log_InlineLogERROR
#define LOGSERVER		log_InlineLogserver
#define LOGACCOUNT		log_InlineLogaccount
#define LogSave			log_InlineLogOther
#define LOGCATCH		log_InlineLogCATCH_ERROR
#define LOGPOINT		log_InlineLogPoint


/////////////////////////////////////////////////////////
void	DateTime(char * buf20, time_t tInput);

/////////////////////////////////////////////////////////
//#ifndef	PROFILE
//	#undef	log_InlineLog
//	#define log_InlineLog(x,y) inline void	log_InlineLog##y(const char * sFormat, ...) { }
//#endif	// PROFILE
//log_InlineLog(log_szFilename, Profile)
//#define LOGPROFILE			log_InlineLogProfile

/////////////////////////////////////////////////////////


//  �������-----begin----------[7/6/2007]----------------------
DWORD MyExceptionFilter( DWORD dwExCode );
#define NEW_TRY				_try{
#define NEW_EXCEPT			}_except( MyExceptionFilter( GetExceptionCode() ) )
//	---------------end--------------------------------------------


#endif	// LOGFILE_H
