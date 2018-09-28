// 全局变量
// 仙剑修，2001.11.19


#include "AllHeads.h"

HANDLE	g_xDatabase;

/////////////////////////////////////////////////////////////////////////////
// 用于统计

long	s_nSocketCount = 0;
long	s_nLoginCount = 0;
long	s_nRealProcLogin = 0;
long	s_nMaxServerTime = 0;
long	s_nAvgServerTime = 0;
long	s_nPointCount = 0;
long	s_nLoginAccept = 0;
long	s_nPointFee = 0;
long	s_nPointSum = 0;
long	s_nAllTimeSum = 0;
long	s_nDatabaseTimeSum = 0;
//20070804修罗:-------------
long	s_nPointThreadPocessTime = 0;//计点线程process用时
long	s_nPointThreadPocMsgCount = 0;//计点线程处理消息累加值
//--------------------------
///softworms-2007-10-06
long	s_nPointThreadVASPocessTime = 0;//计点线程VASprocess用时
long	s_nPointThreadLockTime = 0;//计点线程VASprocess用时

CCriticalSection	s_xCtrl;
char	s_bufText[4096] = "";
char	s_bufState[MAXGAMESERVERS] = "";
char	s_bufStateBackup[MAXGAMESERVERS] = "";
void	PrintText(const char * szFormat, ...)
{
	CSingleLock xLock(&s_xCtrl, true);
	ASSERT(xLock.IsLocked());

	va_list argptr;
	va_start( argptr, szFormat);     /* Initialize variable arguments. */

	char	buf[4096] = "";					/* ?????空间可能不足 */
	int		ret = vsprintf(buf, szFormat, argptr);
	ASSERT(ret < 4096-2);						/* ?????空间可能不足 */

	va_end( argptr );              /* Reset variable arguments.      */

	char *	pNew = strchr(s_bufText, '\n');
	if(pNew)
	{
		static int	nCount = 0;
		if(nCount < 11)
			nCount++;
		else
			strcpy(s_bufText, pNew+1);
	}

	strcat(s_bufText, "【");
	char szCurrTime[20];
	DateTime(szCurrTime, time(NULL));
	if(strlen(szCurrTime) >= 8)
		strcat(s_bufText, szCurrTime);		//  + 8过滤掉年份月份
	strcat(s_bufText, ": ");
	strcat(s_bufText, buf);
	strcat(s_bufText, "】\r\n");
}

bool	LockedGetText(char * buf)
{
	CSingleLock xLock(&s_xCtrl, true);
	ASSERT(xLock.IsLocked());

	if(strlen(s_bufText))
	{
		strcpy(buf, s_bufText);
//		s_bufText[0] = 0;
		return true;
	}
	else
	{
		return false;
	}
}

void	SetServerState(int nIndex, int nState)
{
	CSingleLock xLock(&s_xCtrl, true);
	ASSERT(xLock.IsLocked());

	if(nIndex >= 0 && nIndex < MAXGAMESERVERS)
		s_bufState[nIndex] = nState;

	if(nIndex >= 0 && nIndex < MAXGAMESERVERS && nState <= c_flagBaseMax)
		s_bufStateBackup[nIndex] = nState;
}

void	GetServerState(char * buf)
{
	CSingleLock xLock(&s_xCtrl, true);
	ASSERT(xLock.IsLocked());

	//首次初始化
	if(s_bufState[0] == 0)
	{
		memset(s_bufState, c_flagNone, MAXGAMESERVERS);
		memset(s_bufStateBackup, c_flagNone, MAXGAMESERVERS);
	}

	memcpy(buf, s_bufState, MAXGAMESERVERS);
	memcpy(s_bufState, s_bufStateBackup, MAXGAMESERVERS);
}

/////////////////////////////////////////////////////////////////////////////

time_t		g_tStartServerTime = 0;

/////////////////////////////////////////////////////////////////////////////
// 全局对象

CPointThread *	g_pPointThread = NULL;		//?? 必须先定义(POINTLISTENPORT);
CLoginThread *	g_pLoginThread = NULL;		//?? 要调用上一个类(LOGINLISTENPORT);
COnlineTable *	g_pOnlineTable = NULL;		// 在线玩家表
CAccount		g_cDatabase;
bool			g_bEnableLogin = false;		//?? 可改为SHARE共享变量

CServerAccount	g_aServerAccount[MAXGAMESERVERS];	// 游戏服务器列表
int				g_nServerAccount;					// 游戏服务器数量
char			g_szCountHeadLine[4096];			// 游戏服务器列表的标题行
ISP_SET			g_setISPList;						// IPS的IP地址列表
char			g_szMainServer[64];					// 主游戏服务器


//////////////////////////////////////////////////////////////////
// 全局函数

DWORD	NewAuthenID(unsigned long nClientIP)
{
	// 生成认证ID号
	srand(clock() * nClientIP);
	return (OBJID)((rand()+1) * (rand()+1));
}

// return false: 非法。或源串超长(成功拷贝部分串)
bool	SafeCopy(char * pTarget, const char * pSource, int nBufLen /*= 0*/)
{
	try{
		ASSERT(pTarget);
		if(pTarget)
		{
			pTarget[0] = 0;
			ASSERT(pSource);
			if(pSource)
			{
				if(nBufLen && strlen(pSource) >= nBufLen)
				{
					strncpy(pTarget, pSource, nBufLen-1);
					pTarget[nBufLen-1] = 0;
					return false;
				}
				else
				{
					strcpy(pTarget, pSource);
					return true;
				}
			}
		}
	}catch(...){
		LOGCATCH("SafeCopy(). invalid access.");
	}

	return false;
}

// 添尾0，删尾空
bool	FormatStr(char* pszBuf, int nBufLen /*= 0*/)
{
	ASSERT(pszBuf);

	try{
		if(nBufLen)
			pszBuf[nBufLen-1] = 0;

		// 删除尾空格
		char* ptr = pszBuf + strlen(pszBuf) - 1;
		while(ptr >= pszBuf && *ptr == ' ')
			*(ptr--) = 0;
	}catch(...){
		LOGCATCH("FormatStr(). invalid access.");
	}
	return false;
}



