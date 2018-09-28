// 全局变量
// 仙剑修，2001.11.19

#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
using namespace std;

extern HANDLE	g_xDatabase;

//////////////////////////////////////////////////////////////////
// 用于统计
extern long	s_nSocketCount;
extern long	s_nLoginCount;
extern long s_nRealProcLogin;//实际处理人数
extern long	s_nMaxServerTime;
extern long	s_nAvgServerTime;
extern long	s_nPointCount;
extern long s_nLoginAccept;
extern long s_nPointFee;
extern long s_nPointSum;
extern long s_nAllTimeSum;
extern long s_nDatabaseTimeSum;

extern long s_nPointThreadPocessTime;
extern long s_nPointThreadPocMsgCount;

///softworms-添加VAS处理时间-2007-10-06
extern long s_nPointThreadVASPocessTime;
extern long s_nPointThreadLockTime;

void	PrintText(const char * szFormat, ...);
bool	LockedGetText(char * buf);

enum	{ c_flagNone='.', c_flagOffline=',', c_flagSocket=':', c_flagAccount='-', c_flagStop='#', c_flagNormal='=', c_flagBaseMax='@', 
						c_flagBegin='S', c_flagPoint='Y', c_flagEnd='X', c_flagHeartbeat='?', 
						c_flagBegin2='s', c_flagPoint2='y', c_flagEnd2='x' };
void	SetServerState(int nServer, int nState);
void	GetServerState(char * buf);
//////////////////////////////////////////////////////////////////

extern	time_t	g_tStartServerTime;				// log need

//////////////////////////////////////////////////////////////////
// IPS列表常量
typedef	char	NAMESTR[_MAX_NAMESIZE];
struct	ISP_ST{
	NAMESTR		szName;
	NAMESTR		szFirstIP;
	NAMESTR		szLastIP;
	NAMESTR		szAccount;
	NAMESTR		szPassword;
};
typedef vector<ISP_ST>		ISP_SET;

//////////////////////////////////////////////////////////////////
// 全局对象

extern COnlineTable *	g_pOnlineTable;		// 在线玩家表
extern CPointThread *	g_pPointThread;		// (POINTLISTENPORT);
extern CLoginThread *	g_pLoginThread;		// (LOGINLISTENPORT);
extern CAccount			g_cDatabase;
extern bool				g_bEnableLogin;

extern CServerAccount	g_aServerAccount[];		// 游戏服务器列表
extern int				g_nServerAccount;		// 游戏服务器数量
extern char				g_szCountHeadLine[];	// 游戏服务器列表的标题行
extern ISP_SET			g_setISPList;			// IPS的IP地址列表


//////////////////////////////////////////////////////////////////
// 全局函数

DWORD	NewAuthenID(unsigned long nClientIP);
bool	SafeCopy(char * pTarget, const char * pSource, int nBufLen = 0);		// return false: 非法。或源串超长(成功拷贝部分串)
bool	FormatStr(char* pszBuf, int nBufLen = 0);






#endif // GLOBAL_H