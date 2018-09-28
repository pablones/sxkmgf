// PointThread.cpp: 计点线程类
// 仙剑修：2001.11.20
//////////////////////////////////////////////////////////////////////

#include "AllHeads.h"
#include "PointThread.h"
#include "MsgConnect.h"
#include "MsgAccount.h"
#include "MsgLogin.h"
#include "MsgFee.h"
#include "MsgQueryFee.h"
#include "MsgVAS.h"
#include "MsgStopAccount.h"
#include "MsgNewPlayerExam.h"
#include "MsgAward.h"
#include "MsgManager.h"
#include "RoleManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define		MAX_SERVER_USER_NUM		1200		//游戏服务器最大玩家限制
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointThread::CPointThread(u_short nPort, int nSndBuf /*= 0*/)
			: CThreadBase(), m_cListenSocket(nPort, nSndBuf)
{
	for(int i = 0; i < MAXGAMESERVERS; i++)
	{
		Clear(i, false);
	}

	// 读入帐号表
	GetServerAccount();		// 预读
	memset(m_aServerUserNum,0,sizeof(m_aServerUserNum));
}

CPointThread::~CPointThread()
{

}

/////////////////////////////////////////////////////////////////////////////
// 共享函数，注意互斥

//#define	LOCK	{LOCKTHREAD;
//#define	UNLOCK	}

// ★使用共享成员变量，必须先锁定。调用外部函数，必须先解锁。
void	CPointThread::OnInit()
{
	LOCKTHREAD;
	try{

		m_cListenSocket.Open();
		LOGMSG("计点线程正常启动");
	}catch(...) { LOGCATCH("计点线程初始化时异常退出");}
}

//20070726修罗:将try分块
bool	CPointThread::OnProcess()
{
//	time_t	tLockStart = clock();
	LOCKTHREAD;
//		long	nLockUsed = clock() - tLockStart;
//		InterlockedExchangeAdd(&s_nPointThreadLockTime,nLockUsed);//20070804修罗:计点线程process总用时

		time_t	tStart = clock();
		// select
		fd_set	readmask;
		FD_ZERO(&readmask);
		if(!m_cListenSocket.IsOpen())
			m_cListenSocket.Rebuild();
		if(m_cListenSocket.IsOpen())
			FD_SET(m_cListenSocket.Socket(), &readmask);
		for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
		{
			if(m_aServerSocket[i].IsOpen())
				FD_SET(m_aServerSocket[i].Socket(), &readmask);
		}

		struct timeval	timeout = {0,0};
		int ret = select(FD_SETSIZE, &readmask, (fd_set *) 0, (fd_set *) 0, &timeout);

	try
	{
		// 检查是否接收到新连接
		if(ret == -1)	// error
		{
			m_cListenSocket.Close();
			LOGERROR("计点线程 select 错误[%d]", WSAGetLastError());
			PrintText("计点线程出错，SOCKET被关闭。%d 秒后将会自动重建", REBUILDLISTENDELAYSEC);
		}
		else if(ret > 0 && FD_ISSET(m_cListenSocket.Socket(), &readmask))
		{
			FD_CLR(m_cListenSocket.Socket(), &readmask);
			ret--;
			u_long	nClientIP;
			SOCKET	sockNew = m_cListenSocket.Accept(nClientIP);
			if(sockNew != INVALID_SOCKET)
			{
				// 找空位置
				bool	bSuccess = false;
				for(int i =1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
				{
					if(!m_aServerSocket[i].IsOpen())
					{
						bSuccess = true;
						break;	// 成功
					}
				}

				// 添加连接
				struct in_addr	in;
				in.S_un.S_addr = nClientIP;
				char * pAddr = inet_ntoa(in);
				if(bSuccess)
				{
					m_aServerSocket[i].Open(sockNew, nClientIP);
					m_aHeartbeat[i] = clock();
					LOGMSG("一个新游戏服务器连接进来，服务器ID号分配为[%d], IP地址为[%s]", i, pAddr);
					::SetServerState(i, c_flagSocket);
				}
				else
				{
					LOGWARNING("已接收到一个新连接[%s]，但连接表已满。请修改MAXGAMESERVERS，并重新编译程序。"
								, pAddr);
					//	修改SOCKET属性成立即关闭型
					struct linger ling;
					ling.l_onoff = 1;
					ling.l_linger = 0;
					setsockopt(sockNew, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(ling));
					closesocket(sockNew);
					InterlockedDecrement(&s_nSocketCount);
				}
			}
		}
	}
	catch(...)
	{
		LogSave("CPointThread::OnProcess() catch");
	}

	try
	{
		// 接收到登录消息包。需要优化??
		for( i = 1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
		{
			if(ret <= 0)
				break;	// 没连接有数据了

			if(m_aServerSocket[i].IsOpen())
			{
				if(FD_ISSET(m_aServerSocket[i].Socket(), &readmask))
				{
					FD_CLR(m_aServerSocket[i].Socket(), &readmask);
					ret--;
					char	bufMsg[_MAX_MSGSIZE];
					int		nMsgLen = 0;
					if(m_aServerSocket[i].Recv(bufMsg, nMsgLen))
					{
						if(nMsgLen)
						{
							m_aHeartbeat[i] = clock();				//收到消息就认为收到心跳.....
							m_aHeartbeatLast[i] = 0;
							ProcessMsg(i, bufMsg, nMsgLen);
							//20070804修罗
							InterlockedIncrement(&s_nPointThreadPocMsgCount);
						}
					}
					else	// 错误或关闭
					{
						LOGWARNING("Recv错，服务器[%s]关闭。", m_aServerName[i]);
						LOGSERVER("掉线，服务器[%s]关闭。", m_aServerName[i]);
						if (stricmp(g_szMainServer, m_aServerName[i])==0)
						{
							strcpy(g_szMainServer, "");
							PrintText("主服务器“%s”关闭，重新任命主服务器...", m_aServerName[i]);
							MsgManager::Instance().AppointMainServer(i);//任命主服务器
						}
						else
							PrintText("服务器“%s”关闭。", m_aServerName[i]);

						MsgManager::Instance().ProcessMsgServerDisconnect(i);///处理断线消息
						Clear(i);
					}
				
				}
			}
		}

		UNLOCKHEAD;

		long	nUsed = clock() - tStart;
		InterlockedExchangeAdd(&s_nPointThreadPocessTime,nUsed);//20070804修罗:计点线程process总用时
		InterlockedExchangeAdd(&s_nAllTimeSum, nUsed);
		InterlockedIncrement(&s_nPointCount);

		long	nRemain = POINTLOOPDELAY - nUsed;
		if(nRemain < 0 || nRemain > POINTLOOPDELAY)
			nRemain = 0;
		//20070803修罗:打开切换

		///检查是否还有数据在套接字中,如果有就不许休息
		for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
		{
			if(m_aServerSocket[i].IsOpen())
				FD_SET(m_aServerSocket[i].Socket(), &readmask);
		}

		struct timeval	timeout = {0,0};
		int ret = select(FD_SETSIZE, &readmask, (fd_set *) 0, (fd_set *) 0, &timeout);
		///检查是否还有数据在套接字中,如果有就不许休息
		
//		ret=0;
		if(nRemain && ret<=0)		//? 不切换到登录线程。计费线程优先//06.10.21王玉波注释,保证登陆线程优先
		{
			Sleep(nRemain);
			InterlockedExchangeAdd(&s_nPointThreadLockTime, nRemain);
		}
		else
		{
			nRemain=0;
		}
		MsgManager::Instance().OnBreathe();///处理定时任务
//		Sleep(30);
 		return true;

	}catch(...) { 
		LOGCATCH("计点线程主循环异常，程序坚持运行中"); 
		PrintText("计点线程主循环出错，程序坚持运行中...");
		return true; 
	}
}

void	CPointThread::OnDestroy()
{
	LOCKTHREAD;

//	LOCK	// VVVVVVVVV
	try{
		m_cListenSocket.Close();
		for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
		{
			if(m_aServerSocket[i].IsOpen())
				m_aServerSocket[i].Close();
		}
		LOGMSG("计点线程正常退出");

	}catch(...) { LOGCATCH("计点线程结束时异常退出"); }

//	UNLOCK	// AAAAAAAAA
}

// ★共享函数，需要互斥。由LOGIN线程调用，最好改为消息传递模式??
// return ERR_NONE: OK
int	CPointThread::NewLogin(OBJID idAccount, DWORD nAuthenID, const char * szClientIP, const char * pGameServer)
{
	//---------------
	clock_t clkNewLoginBegin = clock();
	//---------------
	LOCKTHREAD;
	//----------------
	clock_t clkNewLoginUsed = clock() - clkNewLoginBegin;
	if(clkNewLoginUsed>20)//proc超时
	{
		char szLog[512];
		sprintf(szLog,"NewLogin超时(%d):idAccount:%d,pGameServer:%s",
			clkNewLoginUsed,idAccount,pGameServer);
		::LogSave(szLog);
	}
	//----------------

	CMsgConnect	cMsg;
	ASSERT(IPSTRSIZE == _MAX_NAMESIZE);
	cMsg.Create(idAccount, nAuthenID, (char *)szClientIP);
	OBJID		nIndex = GetServerIndex_0(pGameServer);		// 错误返回NULL
	if(nIndex && m_aServerSocket[nIndex].IsOpen())
	{
		if(m_aServerUserNum[nIndex]>=MAX_SERVER_USER_NUM)
			return ERR_FULL;
		if(m_aState[nIndex] == STATE_NORMAL || idAccount <= MAX_GM_ID)
		{
			m_aServerSocket[nIndex].Send(cMsg.GetBuf(), cMsg.GetSize());	// 连接失败时忽略错误，下次会删除游戏服务器名
//			LOGACCOUNT("发送给[%s]玩家[%d]的登录消息。认证ID[%08X]，客户端IP[%s]", 
//						m_aServerName[nIndex], idAccount, nAuthenID, szClientIP);

			m_aServerUserNum[nIndex]++;

			return ERR_NONE;
		}
		else if(m_aState[nIndex] == STATE_BUSY)
			return ERR_BUSY;
		else if(m_aState[nIndex] == STATE_FULL)
			return ERR_FULL;
	}

	return ERR_NO_SERVER;	// 错误的服务器名或服务器未启动。
}

bool	CPointThread::GetServerIP(char * bufIP, const char * pServerName)
{
	LOCKTHREAD;

	for(int i = 1; i < MAXGAMESERVERS; i++)	// 保留0，0为错误
	{
		if(strcmp(m_aServerName[i], pServerName) == 0)
			break;
	}

	if(bufIP && i < MAXGAMESERVERS)
	{
		if(m_aServerSocket[i].IsOpen())
		{
			strcpy(bufIP, m_aServerSocket[i].GetPeerIP());
			return true;
		}
		else
		{
			if(m_aServerName[i][0])
			{
				LOGWARNING("取IP时，游戏服务器[%s]已经关闭", m_aServerName[i]);
				LOGSERVER("服务器[%s]已经关闭了", m_aServerName[i]);
				PrintText("服务器“%s”已经关闭了", m_aServerName[i]);
			}
			Clear(i);
		}
	}

	bufIP[0] = 0;
	return false;
}

bool	CPointThread::GetServerURL(char * bufURL, const char * pServerName)
{
	LOCKTHREAD;
	
	for(int i = 1; i < MAXGAMESERVERS; i++)	// 保留0，0为错误
	{
		if(strcmp(m_aServerName[i], pServerName) == 0)
			break;
	}
	
	if(bufURL && i < MAXGAMESERVERS)
	{
		if(m_aServerSocket[i].IsOpen())
		{
			strcpy(bufURL, m_aServerURL[i]);
			return true;
		}
		else
		{
			if(m_aServerName[i][0])
			{
				LOGWARNING("取IP时，游戏服务器[%s]已经关闭", m_aServerName[i]);
				LOGSERVER("服务器[%s]已经关闭了", m_aServerName[i]);
				PrintText("服务器“%s”已经关闭了", m_aServerName[i]);
			}
			Clear(i);
		}
	}
	
	bufURL[0] = 0;
	return false;
}

int	CPointThread::GetServerLoginMode(const char * pServerName)
{
	LOCKTHREAD;
	
	for(int i = 1; i < MAXGAMESERVERS; i++)	// 保留0，0为错误
	{
		if(strcmp(m_aServerName[i], pServerName) == 0)
			break;
	}
	
	if(i < MAXGAMESERVERS)
	{
		if(m_aServerSocket[i].IsOpen())
		{
			return m_nServerLoginMode[i];
		}
	}
	return -1;
}

int	CPointThread::GetServerState(LPCTSTR szServerName)
{
	LOCKTHREAD;

	int	nIndex = GetServerIndex_0(szServerName);
	if(nIndex)
		return m_aState[nIndex];
	else
		return STATE_OFFLINE;
}

// int	CPointThread::GetServerLoginMode(const char * pServerName)
// {
// 	LOCKTHREAD;
// 	
// 	for(int i = 1; i < MAXGAMESERVERS; i++)	// 保留0，0为错误
// 	{
// 		if(strcmp(m_aServerName[i], pServerName) == 0)
// 			break;
// 	}
// 	
// 	if(i < MAXGAMESERVERS)
// 	{
// 		if(m_aServerSocket[i].IsOpen())
// 		{
// 			return m_nServerLoginMode[i];
// 		}
// 	}
// 	return -1;
// }

int	CPointThread::GetServerCount()
{
	LOCKTHREAD;

	bool	nCount = 0;
	for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
	{
		if(m_aState[i] != STATE_OFFLINE)
			nCount++;
	}

	return nCount;
}

bool CPointThread::CheckHeartbeatAll()
{
	LOCKTHREAD;

	bool	ret = false;
	for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
	{
		if(m_aServerSocket[i].IsOpen())
		{
			CheckHeartbeat(i);
			ret = true;
		}
	}
	return ret;
}

/*bool	CPointThread::GetServerName(OBJID idServer, char bufName[SERVERNAMESIZE])
{
	LOCKTHREAD;

	ASSERT(bufName);

	bufName[0] = 0;
	if(!idServer)
		return false;

	if(m_aServerSocket[idServer].IsOpen())
	{
		SafeCopy(bufName, m_aServerName[idServer], _MAX_NAMESIZE);
		return true;
	}
	else
	{
		if(m_aServerName[idServer][0])
		{
			LOGWARNING("取名时，游戏服务器[%s]已经关闭", m_aServerName[idServer]);
			LOGSERVER("服务器[%s]已经关闭。", m_aServerName[idServer]);
			PrintText("服务器“%s”已经关闭。", m_aServerName[idServer]);
		}
		Clear(idServer);		// SOCKET关闭后，删除游戏服务器名。被动同步
		return false;
	}
}*/

void	CPointThread::LogCount()
{
	LOCKTHREAD;

	char	bufTime[20];
	DateTime(bufTime, time(NULL));
	char	bufLine[4096]="";			//?
	int		nAmount = g_pOnlineTable->GetPlayerCount();
	for(int i = 1; i < g_nServerAccount; i++)	// 1: 从1开始。
	{
		int	nCount = g_pOnlineTable->GetPlayerCount(g_aServerAccount[i].m_szServerName);
		int	nState = g_pPointThread->GetServerState(g_aServerAccount[i].m_szServerName);
		if(nState == CPointThread::STATE_OFFLINE)
		{
			nAmount	-= nCount;
			nCount	= 0;
		}
		char	buf[256];
		sprintf(buf, " | %12d", nCount);
		strcat(bufLine, buf);
	}
	strcat(bufLine, "\n");

	char	buf[4096];
	sprintf(buf, "%s | %12d", bufTime, nAmount);
	strcat(buf, bufLine);
	log_SaveFile("count", buf, g_szCountHeadLine);		//? LOGFILE中有自己的互斥量，双重互斥效率较低
}

void	CPointThread::LogSerialCount()
{
	LOCKTHREAD;

	char	bufTime[20];
	DateTime(bufTime, time(NULL));
	char	bufLine[4096]="";			//?
	int		nAmount = 0;
	int		nCount = 0;
	for(COnlineTable::SERIAL_INDEX::iterator iter = g_pOnlineTable->GetSerialSet()->Begin();
			iter != g_pOnlineTable->GetSerialSet()->End();
			iter++, nCount++)
	{
		char	buf[256];
		sprintf(buf, " | %4d:%4d", iter->first, iter->second);
		strcat(bufLine, buf);
		nAmount	+= iter->second;
		if(strlen(bufLine) > 4000)
			break;
		if(nCount > 50000)
		{
			LOGERROR("★LogSerialCount()卡死★");
			break;
		}
	}
	strcat(bufLine, "\n");

	char	buf[4196];
	sprintf(buf, "%s | %8d", bufTime, nAmount);
	strcat(buf, bufLine);
	log_SaveFile("serial", buf, "时间                |   合计   | 序列号:人数\n");		//? LOGFILE中有自己的互斥量，双重互斥效率较低
}

void	CPointThread::LogServer()
{
	LOCKTHREAD;

	char	bufTime[20];
	DateTime(bufTime, time(NULL));
	char	bufLine[4096];			//?
	sprintf(bufLine, "%s | %12d", bufTime, GetServerCount());
	for(int i = 1; i < g_nServerAccount; i++)	// 1: 从1开始。
	{
		BOOL	bState = GetServerState(g_aServerAccount[i].m_szServerName);
		char	buf[256];
		if(bState != STATE_OFFLINE)
			sprintf(buf, " | %12s", "√");
		else
			sprintf(buf, " | %12s", "╳");
		strcat(bufLine, buf);
	}
	strcat(bufLine, "\n");

	log_SaveFile("server", bufLine, g_szCountHeadLine);		//? LOGFILE中有自己的互斥量，双重互斥效率较低
}

bool	CPointThread::Kickout(OBJID idAccount)
{
	LOCKTHREAD;

	char	bufServer[256];
	if(g_pOnlineTable->GetServerName(idAccount, bufServer))
	{
		return Kickout_0(idAccount, bufServer);
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////
// ★非共享函数，由子线程独占调用。不能调用外部函数。

bool	CPointThread::Kickout_0(OBJID idAccount, LPCTSTR szServerName)
{
	int nIndex = GetServerIndex_0(szServerName);

	if(nIndex != 0 )
	{
		CMsgFee	cMsg;
		cMsg.Create(idAccount, CMsgFee::FEE_KICKOUT);
		m_aServerSocket[nIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
		return true;
	}


	return false;
}

bool	CPointThread::ProcessMsg(int nServerIndex,  char * pBuf, int nLen)
{
	unsigned short unMsgType = CNetMsg::GetType(pBuf, nLen);
	MsgManager::Instance().ProcessMsg(nServerIndex,pBuf, nLen);///接管消息处理
	switch(unMsgType)
	{
		case _MSG_RELAY:
		case _MSG_TALK:
		case _MSG_FRIEND:
		case _MSG_SYNDICATE:
		case _MSG_ITEMINFO:
		case _MSG_EUDEMONINFO:
		case _MSG_SYNINFO:
		case _MSG_SYNMEMBERINFO:
		case _MSG_ITEM:
		case _MSG_WEAPONSOUL:
		case _MSG_MAINSERVER:
		case _MSG_REGISTER:
		case _MSG_SCHOOLMEMBER:
		case _MSG_VIPLIST:
		case _MSG_ACCLOCK:
		case _MSG_WARGAME:
		{
			break;
		}
	case _MSG_ACCOUNT:
		{
			try
			{
				CMsgAccount	cMsg;
				cMsg.Create(pBuf, nLen);
				LOGMSG("接收到[%s]游戏服务器ACCOUNT消息。帐号[%s],口令[***]。"
					, cMsg.m_pInfo->szServer
					, cMsg.m_pInfo->szAccount
					//, cMsg.m_pInfo->szPassword
					);
				
				// 消息检查
				if(strlen(cMsg.m_pInfo->szAccount) == 0 
					|| strlen(cMsg.m_pInfo->szPassword) == 0 
					|| strlen(cMsg.m_pInfo->szServer) == 0)
				{
					LOGERROR("错误的登录消息[%s][%s][%s]", 
						cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, cMsg.m_pInfo->szServer);
					return false;
				}
				
				GetServerAccount();		// 即时读入
				
				// 检查帐号
				int		nAccount = 0;		// 在帐号表中的序号，非游戏服务器ID号。
				for(int i = 1; i < g_nServerAccount; i++)	// 1: 从1开始。
				{
					if(strcmp(g_aServerAccount[i].m_szServerName, cMsg.m_pInfo->szServer) == 0)		// 以服务器名匹配为准
					{
						nAccount = i;
						break;
					}
				}
				
				m_aServerUserNum[nAccount] = 0;

				CMsgLogin	cRetMsg;
				//		AfxMessageBox(g_aServerAccount[nAccount].m_szLoginName);
				//		AfxMessageBox(g_aServerAccount[nAccount].m_szPassword);
				if(nAccount					// 0：无此帐号
					&& strcmp(g_aServerAccount[nAccount].m_szLoginName, cMsg.m_pInfo->szAccount) == 0
					&& strcmp(g_aServerAccount[nAccount].m_szPassword, cMsg.m_pInfo->szPassword) == 0)	// 找到
				{
					for(int i = 1; i < MAXGAMESERVERS; i++)
					{
						if(strcmp(g_aServerAccount[nAccount].m_szServerName, m_aServerName[i]) == 0)
						{
							// 重复登录
							if(m_aServerSocket[i].IsOpen())
							{
								LOGMSG("游戏服务器[%s]重复ACCOUNT登录，前一个连接[%d]已断开。", m_aServerName[i], i);
								m_aServerSocket[i].Close();		// 关闭前一个登录的连接
								Clear(i);
							}
							m_aServerName[i][0] = 0;
							//表小，可全搜一遍	break;
						}
					}
					
					// 保存游戏服务器域名/IP
					strcpy(m_aServerName[nServerIndex], g_aServerAccount[nAccount].m_szServerName);	// cMsg.m_pInfo->szServer);
					// 保存游戏服务器域名/IP
					strcpy(m_aServerName[nServerIndex], g_aServerAccount[nAccount].m_szServerName);	// cMsg.m_pInfo->szServer);
					strcpy(m_aServerURL[nServerIndex], g_aServerAccount[nAccount].m_szServerURL);
					m_nServerLoginMode[nServerIndex] = g_aServerAccount[nAccount].m_nServerLoginMode;

					//?? BUG：提前允许登录了，应该接收到_MSG_LOGIN后开始允许登录
					PrintText("第%d号游戏服务器“%s”已登录", nServerIndex, m_aServerName[nServerIndex]);
					LOGSERVER("第%d号游戏服务器“%s”已登录", nServerIndex, m_aServerName[nServerIndex]);
					//?? BUG：提前允许登录了，应该接收到_MSG_LOGIN后开始允许登录
					if(cRetMsg.Create(nServerIndex, 0, "", ACCOUNTVERSION))
					{
						if(m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize())==false)
							LOGMSG("游戏服务器登录回应包,发送失败!%s", m_aServerName[nServerIndex]);
					}
					else
					{
						LOGMSG("游戏服务器登录回应包,组包失败!,%s", m_aServerName[nServerIndex]);
					}
					LOGMSG("新的游戏服务器[%s]ACCOUNT帐号认可。返回服务器ID[%d],游戏版本[%04X]", 
						m_aServerName[nServerIndex], nServerIndex, ACCOUNTVERSION);
					LOGMSG("新的游戏服务器[%s]ACCOUNT登录中......", m_aServerName[nServerIndex]);
					::SetServerState(nServerIndex, c_flagAccount);
					MsgManager::Instance().AppointMainServer(-1);//任命主服务器
					// 添加索引
					//				m_idxID.Add(nServerIndex, nAccount);
					//				g_aServerAccount[nAccount].m_nIndex = nServerIndex;
				}
				else
				{
					cRetMsg.Create(0, 0, "", ACCOUNTVERSION);
					m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
					LOGMSG("新的游戏服务器[%s]登录失败", cMsg.m_pInfo->szServer);
				}
				///!!!!!!!!!!!!!!!softworms
				//			CMsgVAS msgVAS;
				//			msgVAS.m_pInfo=new CMsgVAS::MSG_Info;
				//			CMsgVAS::MsgTraffic mt={0};
				//			mt.itemAmount=1;
				//			mt.itemID=131090;
				//			mt.unMsgType=_MSG_GENERAL + 1046;
				//			mt.unMsgSize=sizeof(mt);
				//			msgVAS.Create(mt,4900,240,14003832);
				//			ProcessMsg(nServerIndex,(char*)msgVAS.m_pInfo,msgVAS.m_pInfo->unMsgSize);
				//			delete msgVAS.m_pInfo;
				///!!!!!!!!!!!!!!!
				MsgManager::Instance().BroadcastGameServerListMsg(nServerIndex);
			}
			catch(...)
			{
				LogSave("CPointThread::ProcessMsg...case _MSG_ACCOUNT");
			}
		}
		break;
	case _MSG_LOGIN:
		{
			try
			{
				// 消息检查(不需要)
				
				//			CMsgConnect	cMsg;
				//			cMsg.Create(nServerIndex, 0, "登录成功");
				
				// 仅简单回传
				//			m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				m_aState[nServerIndex] = STATE_NORMAL;		// 登录完成
				
				LOGMSG("......[%s]游戏服务器LOGIN登录成功", m_aServerName[nServerIndex]);
				LOGMSG("接收到登录消息。新的游戏服务器[%s]LOGIN登录成功", m_aServerName[nServerIndex]);
				::SetServerState(nServerIndex, c_flagNormal);
			}
			catch(...)
			{
				LogSave("CPointThread::ProcessMsg...case _MSG_LOGIN");
			}
		}
		break;
	case _MSG_GAINAWARDS:
		{
			try
			{
				CMsgAward cMsg;
				cMsg.Create(pBuf, nLen);

				switch(cMsg.m_pInfo->nAction)
				{
				case _GAINAWARD_QUERY:
					{
						VECAWARD vecAwards;
						if(m_objInspectManage.ReadAwardEx(cMsg.m_pInfo->accountID, cMsg.m_pInfo->gaInfo[0].nType, cMsg.m_pInfo->szServerName,  vecAwards))
						{
							if(!vecAwards.empty())
							{
								cMsg.m_pInfo->nAction = _GAINAWARD_INIT;
								int nLimitAmount = (_MAX_MSGSIZE - sizeof(CMsgAward::MSG_Info))/sizeof(GainAwardInfo);
								int nLeaveAmount = vecAwards.size();
								int i = 0;

								while(nLeaveAmount > nLimitAmount)
								{
									cMsg.m_pInfo->nAmount = nLimitAmount;
									cMsg.m_pInfo->unMsgSize = sizeof(CMsgAward::MSG_Info) + sizeof(GainAwardInfo)*(nLimitAmount-1);
									memcpy(cMsg.m_pInfo->gaInfo, &vecAwards[i], sizeof(GainAwardInfo)*nLimitAmount);
									m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());

									i += nLimitAmount;
									nLeaveAmount -= nLimitAmount;
								}

								cMsg.m_pInfo->nAmount = nLeaveAmount;
								cMsg.m_pInfo->unMsgSize = sizeof(CMsgAward::MSG_Info) + sizeof(GainAwardInfo)*(nLeaveAmount-1);
								memcpy(cMsg.m_pInfo->gaInfo, &vecAwards[i], sizeof(GainAwardInfo)*nLeaveAmount);
								m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
							}
						}
					}
					break;
					
				case _GAINAWARD_UPDATE:
					{
						m_objInspectManage.WriteAwardeEx(cMsg.m_pInfo->accountID, cMsg.m_pInfo->gaInfo[0].nType, cMsg.m_pInfo->szServerName, cMsg.m_pInfo->gaInfo[0].nValue);
					}
					break;
				}
			}
			catch(...)
			{
				LogSave("CPointThread::ProcessMsg...case _MSG_GAINAWARDS");
			}
		}
		break;
	case _MSG_EXAM:
		{
			CMsgNewPlayerExam cMsg;
			cMsg.Create(pBuf, nLen);
			const int saveSize = 512;
			m_objInspectManage.GetNewPlayerExamState(cMsg.m_pInfo->nAccountID,cMsg.m_pInfo->szBuf,saveSize,cMsg.m_pInfo->usSzSize);
			cMsg.Create( 0,cMsg.m_pInfo->szBuf,cMsg.m_pInfo->usSzSize );
			m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
		}
		break;
	case _MSG_VAS:
		{
			time_t	tStart = clock();
			try
			{
				BOOL bResult=0;
				CMsgVAS		cMsg;
				cMsg.Create(pBuf, nLen);
				OBJID	idAccount = cMsg.m_pInfo->AccountID;///得到帐号
				::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
				//	usAction值说明：1表示购买成功，2表示购买失败，3表示余额查询，100表示消息的来源是世界线程
				//	  10表示地推号状态查询，      11表示领1级新手装备，      12表示领20级装备，      13表示领40级装备。
				//	1000表示地推号状态查询返回
				//[2010-07-26 goto]修改usAction,1001表示世界线程发来的查询余额
				//[2010-07-26 goto]修改CMsgVAS消息结构，注释以前的消息处理
				if( cMsg.m_pInfo->usAction >= 10 && cMsg.m_pInfo->usAction < 100 )
				{
					switch( cMsg.m_pInfo->usAction )
					{
					case 10:
						{
// 							cMsg.m_pInfo->mtExtraInfo.usAction = 999 + cMsg.m_pInfo->usVASPoint;
// 							cMsg.m_pInfo->usVASPoint = m_objInspectManage.ManageGeneralize( cMsg.m_pInfo->AccountID, 0, cMsg.m_pInfo->usVASPoint );
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
						}
						break;
					case 11:
						{
// 							if( 0 > m_objInspectManage.ManageGeneralize( cMsg.m_pInfo->AccountID, cMsg.m_pInfo->mtExtraInfo.itemAmount, cMsg.m_pInfo->usVASPoint ) )
// 							{
// 								//领取一级装备失败写日志
// 								::LogSave("ERROR: case _MSG_VAS: NO Present_1 AccountID:%d",cMsg.m_pInfo->AccountID );///日志记录			
// 							}
						}
						break;
					case 12:
						{
//							m_objInspectManage.ManageGeneralize( cMsg.m_pInfo->AccountID,1 , cMsg.m_pInfo->usVASPoint,cMsg.m_pInfo->mtExtraInfo.itemID ) ;							
						}
						break;
					}
				}
				//////////////////////////////////////////////////////////////////////////
				//  [2007-9-15] 朱斌
				else if(cMsg.m_pInfo->usAction == _MSGVAS_REQUEST_ITEM_LIFE_TIME_CONTINUE)
				{
// 					int nResult = m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 					if(!nResult)
// 					{
// 						cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_REQUEST_ITEM_LIFE_TIME_CONTINUE_FAL;	
// 					}
// 					else
// 					{
// 						UINT uType = _VAS_CONSUMELOG_TYPE_CONTINUE_FULL;
// 						if(cMsg.m_pInfo->mtExtraInfo.itemAmount == _ITEM_LIFE_TIME_CONTINUE_TYPE_HALF)
// 							uType = _VAS_CONSUMELOG_TYPE_CONTINUE_HALF;
// 					//	nResult = m_objManager.ExecItemLifeContinue(cMsg.m_pInfo->usVASPoint, cMsg.m_pInfo->PlayerID, cMsg.m_pInfo->mtExtraInfo.itemID,1, m_aServerName[nServerIndex], uType);
// 					//  [2/16/2008 朱斌]	写入ITEMTYPEID
// 						nResult = m_objManager.ExecItemLifeContinue(cMsg.m_pInfo->usVASPoint, cMsg.m_pInfo->PlayerID, cMsg.m_pInfo->mtExtraInfo.dwData, 1, m_aServerName[nServerIndex], uType);
// 						if(nResult >= 0)	//  [3/14/2008 朱斌]	续费花费等于账户的金钱时扣钱又续费失败问题
// 						{
// 							//[游途道标 2009.01.09]返回余额
// 							cMsg.m_pInfo->mtExtraInfo.usAction = 100;
// 							cMsg.m_pInfo->usVASPoint=nResult;
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_REQUEST_ITEM_LIFE_TIME_CONTINUE_SUC;
// 							//cMsg.m_pInfo->usVASPoint = nResult;		//	带回余额或交易结果，如果交易失败则返回-1
// 						}
// 						else
// 						{
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_REQUEST_ITEM_LIFE_TIME_CONTINUE_FAL;
// 							cMsg.m_pInfo->usVASPoint = nResult;		//	带回余额或交易结果，如果交易失败则返回-1
// 						}
// 					}
// 					m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				}
				else if(cMsg.m_pInfo->usAction == 101)
				{
// 					OBJID	idAccount = cMsg.m_pInfo->AccountID;///得到帐号
// 					int nResult = m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 					if(!nResult)
// 					{
// 						cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_FAL;	
// 					}
// 					else
// 					{
// 						nResult=m_objManager.ExecChg(cMsg.m_pInfo->usVASPoint,m_aServerName[nServerIndex],1);
// 						if(nResult == -1)
// 						{
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_FAL;	
// 						}
// 						else
// 						{
// 							//[游途道标 2009.01.09]返回余额
// 							cMsg.m_pInfo->mtExtraInfo.usAction = 100;
// 							int usVASPoint = cMsg.m_pInfo->usVASPoint;
// 							cMsg.m_pInfo->usVASPoint=nResult;
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 							cMsg.m_pInfo->usVASPoint = usVASPoint;
// 							//-------------------------------------------
// 							idAccount = cMsg.m_pInfo->mtExtraInfo.itemAmount;//cMsg.m_pInfo->PlayerID;
// 							int nResult = m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 							if(!nResult)
// 							{
// 								cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_FAL;	
// 							}
// 							else
// 							{
// 								nResult=m_objManager.ExecChg(cMsg.m_pInfo->usVASPoint,m_aServerName[nServerIndex],2);
// 								//[游途道标 2009.01.09]返回余额
// 								cMsg.m_pInfo->mtExtraInfo.usAction = 100;
// 								cMsg.m_pInfo->usVASPoint=nResult;
// 								idAccount = cMsg.m_pInfo->AccountID;
// 								OBJID idPlayer = cMsg.m_pInfo->PlayerID;
// 								cMsg.m_pInfo->AccountID = cMsg.m_pInfo->mtExtraInfo.itemAmount;
// 								cMsg.m_pInfo->PlayerID = cMsg.m_pInfo->mtExtraInfo.id;
// 								m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 								cMsg.m_pInfo->AccountID = idAccount;
// 								cMsg.m_pInfo->PlayerID = idPlayer;
// 
// 								cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_SUC;
// 							}
// 						}
// 					}
// 					m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				}
				else if(cMsg.m_pInfo->usAction == 102)
				{
// 					OBJID	idAccount = cMsg.m_pInfo->AccountID;///得到帐号
// 					int nResult = 0;
// 					if(m_objManager.InitServer(idAccount))
// 					{
// 						nResult=m_objManager.ExecServerChk(cMsg.m_pInfo->nJuan,cMsg.m_pInfo->nRealID,cMsg.m_pInfo->usVASPoint);
// 						if(nResult >= 0)
// 						{							
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_SERVER_SUC;
// 							cMsg.m_pInfo->usVASPoint=nResult;	
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 						}
// // 						else
// // 						{
// // 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_SERVER_FAL;
// // 							cMsg.m_pInfo->usVASPoint=nResult;							
// // 						}
// 					}
				}
				else if(cMsg.m_pInfo->usAction == _MSGVAS_CHG_TRADE)//[游途道标 2008/12/3]改变增值币数量
				{
// 					OBJID idAccount = cMsg.m_pInfo->mtExtraInfo.itemAmount;
// 					int nResult = m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 					if (!nResult)
// 					{
// 						cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_FAL;	
// 					}
// 					else
// 					{
// 						nResult=m_objManager.ExecChg(m_aServerName[nServerIndex], -cMsg.m_pInfo->usVASPoint);
// 						if (nResult < 0)
// 						{
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_FAL;
// 						}
// 						else
// 						{
// 							idAccount = cMsg.m_pInfo->AccountID;///得到帐号
// 
// 							int nResult2 = nResult;
// 							nResult = m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 							if (!nResult)
// 							{
// 								idAccount = cMsg.m_pInfo->mtExtraInfo.itemAmount;
// 								m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 								m_objManager.ExecChg(m_aServerName[nServerIndex], cMsg.m_pInfo->usVASPoint);
// 								cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_FAL;	
// 							}
// 							else
// 							{
// 								nResult=m_objManager.ExecChg(m_aServerName[nServerIndex], cMsg.m_pInfo->usVASPoint);
// 								cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_SUC;
// 								//[游途道标 2009.01.09]返回余额
// 								cMsg.m_pInfo->mtExtraInfo.usAction = 100;
// 								cMsg.m_pInfo->usVASPoint=nResult;
// 								m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 								cMsg.m_pInfo->usVASPoint=nResult2;
// 								cMsg.m_pInfo->AccountID = cMsg.m_pInfo->mtExtraInfo.itemAmount;
// 								cMsg.m_pInfo->PlayerID = cMsg.m_pInfo->mtExtraInfo.id;
// 								m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 							}
// 						}
// 					}
// 					m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				}
				else if(cMsg.m_pInfo->usAction == _MSGVAS_CHG)//[游途道标 2009.01.08]修改增值币数量
				{
// 					OBJID idAccount = cMsg.m_pInfo->AccountID;
// 					int nResult = m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 					if (!nResult)
// 					{
// 						cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_FAL;
// 						m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 					}
// 					else
// 					{
// 						nResult=m_objManager.ExecChg(m_aServerName[nServerIndex], cMsg.m_pInfo->usVASPoint);
// 						if (nResult < 0)
// 						{
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_FAL;
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 						}
// 						else
// 						{
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_SUC;
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 							cMsg.m_pInfo->mtExtraInfo.usAction = 100;
// 							cMsg.m_pInfo->usVASPoint=nResult;
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 						}
// 					}
				}
				else if(cMsg.m_pInfo->usAction == _MSGVAS_DAYMASK_ACC_GET)
				{
// 					OBJID idAccount = cMsg.m_pInfo->AccountID;
// 					int nResult = m_objManager.InitServer(idAccount);
// 					if (nResult)
// 					{
// 						nResult = m_objManager.ExecMaskDo(cMsg.m_pInfo->usVASPoint,1,cMsg.m_pInfo->mtExtraInfo.itemAmount);
// 					}
// 					
// 					cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_DAYMASK_ACC_RETURN;
// 					cMsg.m_pInfo->usVASPoint=nResult;
// 					m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				}
				else if(cMsg.m_pInfo->usAction == _MSGVAS_DAYMASK_ACC_SET)
				{
// 					OBJID idAccount = cMsg.m_pInfo->AccountID;
// 					int nResult = m_objManager.InitServer(idAccount);
// 					if (nResult)
// 					{
// 						nResult = m_objManager.ExecMaskDo(cMsg.m_pInfo->usVASPoint,2,cMsg.m_pInfo->mtExtraInfo.itemAmount);
// 					}
// 					
// 					cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_DAYMASK_ACC_RETURN;
// 					cMsg.m_pInfo->usVASPoint=nResult;
// 					m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				}
				//////////////////////////////////////////////////////////////////////////
				else
				{
					int nResult = -1;
					bResult=m_objManager.InitMananger(idAccount,m_aServerName[nServerIndex]);
					if( bResult )
					{
						switch( cMsg.m_pInfo->usAction )
						{
						case 0:
						case 3:
							{
								//nResult=m_objManager.ExecTrade(cMsg.m_pInfo->usVASPoint,cMsg.m_pInfo->PlayerID,cMsg.m_pInfo->mtExtraInfo.itemID,cMsg.m_pInfo->mtExtraInfo.itemAmount,m_aServerName[nServerIndex],cMsg.m_pInfo->nJuan);///交易失败返回-1,成功返回余额
							}
							break;
						case 4:
							{
// 								nResult=m_objManager.ExecSupply(cMsg.m_pInfo->usVASPoint,m_aServerName[nServerIndex],0,4,cMsg.m_pInfo->mtExtraInfo.itemAmount);///交易失败返回-1,成功返回余额
// 								//如果是兑换就给物品ID：4444
// 								//用于游戏服务器接受判断
// 								cMsg.m_pInfo->mtExtraInfo.itemID = 4444;// 彭峰 [6/26/2007]
							}
							break;
						case 5:
							{
// 								cMsg.m_pInfo->nJuan = 5;
// 								nResult=m_objManager.ExecTrade(cMsg.m_pInfo->usVASPoint,cMsg.m_pInfo->PlayerID,0,0,m_aServerName[nServerIndex],cMsg.m_pInfo->nJuan);
// 								//如果是兑换就给物品ID：4444
// 								//用于游戏服务器接受判断
// 								cMsg.m_pInfo->nJuan = 1;
// 								cMsg.m_pInfo->mtExtraInfo.itemID = 0;// 彭峰 [6/26/2007]
							}
							break;
						case 6:
							{
// 								cMsg.m_pInfo->nJuan = 6;
// 								nResult=m_objManager.ExecTrade(cMsg.m_pInfo->usVASPoint,cMsg.m_pInfo->PlayerID,0,0,m_aServerName[nServerIndex],cMsg.m_pInfo->nJuan);
// 								//如果是兑换就给物品ID：4444
// 								//用于游戏服务器接受判断
// 								cMsg.m_pInfo->nJuan = 1;
// 								cMsg.m_pInfo->mtExtraInfo.itemID = 0;// 彭峰 [6/26/2007]
							}
							break;
						//[2010-07-26 goto]添加以下部分
						case _MSGVAS_QUERY_BALANCE://查询余额
							{
								nResult=m_objManager.GetBalance();
								cMsg.m_pInfo->usVASPoint = nResult;
								cMsg.m_pInfo->usAction = _MSGVAS_RETURN_BALANCE;
							}
							break;
						case _MSGVAS_ADD_VAS:
							{
								nResult=m_objManager.ExecChg(cMsg.m_pInfo->usVASPoint,m_aServerName[nServerIndex], 2);
								if (nResult < 0)
								{
									cMsg.m_pInfo->usAction = _MSGVAS_ADD_VAS_FAIL;
								}
								else
								{
									cMsg.m_pInfo->usAction = _MSGVAS_ADD_VAS_SUC;
								}
								
								cMsg.m_pInfo->usVASPoint=nResult;
							}
							break;
						case _MSGVAS_SPEND_VAS:
							{
								nResult=m_objManager.ExecChg(cMsg.m_pInfo->usVASPoint,m_aServerName[nServerIndex], 1);
								if (nResult < 0)
								{
									cMsg.m_pInfo->usAction = _MSGVAS_SPEND_VAS_FAIL;
								}
								else
								{
									cMsg.m_pInfo->usAction = _MSGVAS_SPEND_VAS_SUC;
								}
								
								cMsg.m_pInfo->usVASPoint=nResult;
							}
							break;
						case _MSGVAS_BUY_ITEM:
							{
								nResult=m_objManager.ExecTrade(cMsg.m_pInfo->usVASPoint, cMsg.m_pInfo->PlayerID, m_aServerName[nServerIndex], cMsg.m_pInfo->nGoods, cMsg.m_pInfo->usGoodSize);
								if (nResult < 0)
								{
									cMsg.m_pInfo->usAction = _MSGVAS_BUY_FAIL;
								}
								else
								{
									cMsg.m_pInfo->usAction = _MSGVAS_BUY_SUC;
								}
								
								cMsg.m_pInfo->usVASPoint=nResult;
							}
							break;
						}
					}
// 					if(3 == cMsg.m_pInfo->mtExtraInfo.usAction && -1 == nResult)
// 						cMsg.m_pInfo->mtExtraInfo.itemID = 3;
// 					cMsg.m_pInfo->mtExtraInfo.usAction = 100;
// 					cMsg.m_pInfo->usVASPoint=nResult;///带回余额或交易结果，如果交易失败则返回-1
					//cMsg.m_pInfo->nJuan = m_pInfo->nJuan;
					m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				}
				::ReleaseMutex(g_xDatabase);	//------------------------------------------
			}
			catch(...)
			{
				LogSave("CPointThread::ProcessMsg...case _MSG_VAS");
			}
			long nUsed = clock() - tStart;
			InterlockedExchangeAdd(&s_nPointThreadVASPocessTime,nUsed);//20071006-softworms:计点线程VASprocess总用时
		}
		break;
	case _MSG_GMSTOPACCOUNT:
		{
			try
			{
				CMsgStopAccount	cMsg;
				cMsg.Create(pBuf, nLen);

				if (1 == cMsg.m_pInfo->unAction)
				{
					OBJID	idAccount = cMsg.m_pInfo->ucStopIDAccount;///得到帐号
					USHORT  accountStatus = m_objInspectManage.GetAccountStatus(idAccount);
					if (cMsg.Create(1,accountStatus,cMsg.m_pInfo->playerID))
					{
						m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
					}
					break;
				}
				else if (2 == cMsg.m_pInfo->unAction)
				{
					OBJID	idAccount = cMsg.m_pInfo->ucStopIDAccount;///得到帐号
					m_objInspectManage.ActiveAccountByGMtool(idAccount);					
					break;
				}
				else
				{		
					OBJID	idAccount = cMsg.m_pInfo->ucStopIDAccount;///得到帐号
					if( !m_objInspectManage.StopAccountByGMtool( idAccount ) )
					{
						//如果失败写日志
						LogSave("CPointThread::ProcessMsg...StopAccountByGMtool() fail..");		
					}
				}
			}
			catch (...)
			{
				LogSave("CPointThread::ProcessMsg...case _MSG_VAS");

			}
		}
	break;
	case _MSG_FEE:
		{
			CMsgFee		cMsg;
			cMsg.Create(pBuf, nLen);
			OBJID	idAccount = cMsg.m_pInfo->idAccount;
			
			//ashulanTest
#ifdef _DEBUG
//			if(idAccount>1125)
//			{
//				int ashulanTest;
//				ashulanTest = 1;
//			}
#endif

			switch(cMsg.m_pInfo->ucType)
			{
			case CMsgFee::FEE_BEGIN:
				{
					try
					{
						// 消息检查
						if(idAccount == ID_NONE)
						{
							LOGERROR("错误的FEE_BEGIN消息[%d]★", idAccount);
							return false;
						}
						
						// 添加在线表。仅用于重新同步
						if(!g_pOnlineTable->IsOnline(idAccount))		//? 可能引起APPENDNEW()的非原子性操作
						{
							DWORD	nAuthenID = 0xCCCCCCCC;		// 代替认证号
							::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
							if(g_cDatabase.Create(idAccount))	// VVVVVVVVVVVVVVV
							{
								char* pName = g_cDatabase.GetName();
								g_pOnlineTable->AppendNew(idAccount, nAuthenID, "StartFee", pName, "", 
									//											g_cDatabase.GetType(), m_aServerName[nServerIndex], idAccount, pName);
									c_typeNone, m_aServerName[nServerIndex], idAccount, pName);	// return 0: error。c_typeNone : 未知的计费类型。
								//?							if(g_bEnableLogin)
								LOGERROR("补登FEE_BEGIN, 服务器[%s]帐号[%s][%d]。如为ISP模式或计点模式将不再扣点★", 
									m_aServerName[nServerIndex], pName, idAccount);
								g_cDatabase.Destroy();			// AAAAAAAAAAAAAAA
							}
							else
							{
								LOGERROR("游戏服务器[%s]上传了一个未知帐号[%d]的FEE_BEGIN消息，未补登", 
									m_aServerName[nServerIndex], idAccount);
							}
							::ReleaseMutex(g_xDatabase);	//------------------------------------------
						}
						
						//#ifdef	SERVER_X
						//					LOGPOINT("[%s]上传[%d]的START_FEE", m_aServerName[nServerIndex], idAccount);
						//#endif	// SERVER_X
						int nRet = g_pOnlineTable->StartFee(idAccount, m_aServerName[nServerIndex]);		// return -n: 服务器不匹配
						// 双重登录，踢人
						if(nRet < 0)		// <0 : error
						{
							LOGERROR("...双重登录，多余的START_FEE。帐号[%d]被踢下线。", idAccount);
							Kickout_0(idAccount, m_aServerName[nServerIndex]);
						}
						
						if(m_aState[nServerIndex] == STATE_NORMAL)
							::SetServerState(nServerIndex, c_flagBegin);
						else
							::SetServerState(nServerIndex, c_flagBegin2);
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::FEE_BEGIN");
					}
				}
				break;
			case CMsgFee::FEE_END:
			case CMsgFee::FEE_OFFLINE:
				{
					try
					{
						// 消息检查
						if(idAccount == ID_NONE)
						{
							LOGERROR("错误的FEE_END消息[%d]★", idAccount);
							return false;
						}
						
						g_pOnlineTable->EndFee(idAccount, m_aServerName[nServerIndex], 
							cMsg.m_pInfo->ucType == CMsgFee::FEE_OFFLINE);
						
						// 删除在线表
						//由ENDFEE删除					g_pOnlineTable->Remove(idAccount);				// 不能删除空帐号
						if(m_aState[nServerIndex] == STATE_NORMAL)
							::SetServerState(nServerIndex, c_flagEnd);
						else
							::SetServerState(nServerIndex, c_flagEnd2);

						///softworms 屏蔽此处
						//m_objInspectManage.WatchToolInfo(idAccount,m_aServerName[nServerIndex],2);///[2009.3.18--张冬]删除监控工具数据信息
						m_objInspectManage.UpdateAccoutOnlineTime(idAccount,m_aServerName[nServerIndex]);
						m_aServerUserNum[nServerIndex]--;
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::FEE_OFFLINE");
					}
				}
				break;
			case CMsgFee::FEE_POINT:
			case CMsgFee::FEE_TICK:
				{
					try
					{
						// 消息检查
						if(idAccount == ID_NONE)
						{
							LOGERROR("错误的FEE_POINT消息[%d]★", idAccount);
							return false;
						}
						
						// 添加在线表。仅用于重新同步
						if(!g_pOnlineTable->IsOnline(idAccount))		//? 可能引起APPENDNEW()的非原子性操作
						{
							DWORD	nAuthenID = 0xCCCCCCCC;		// 代替认证号
							::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
							if(g_cDatabase.Create(idAccount))	// VVVVVVVVVVVVVVV
							{
								char* pName = g_cDatabase.GetName();
								g_pOnlineTable->AppendNew(idAccount, nAuthenID, "PointFee", pName, "", 
									//											g_cDatabase.GetType(), m_aServerName[nServerIndex], idAccount, pName);			// return 0: error
									c_typeNone, m_aServerName[nServerIndex], idAccount, pName);	// return 0: error。c_typeNone : 未知的计费类型。
								//?							if(g_bEnableLogin)
								LOGERROR("补登FEE_POINT，服务器[%s]帐号[%s][%d]。如为ISP模式或计点模式将不再扣点★", 
									m_aServerName[nServerIndex], pName, idAccount);
								g_cDatabase.Destroy();			// AAAAAAAAAAAAAAA
							}
							else
							{
								LOGERROR("游戏服务器[%s]上传了一个未知帐号[%d]的FEE_POINT消息，未补登", 
									m_aServerName[nServerIndex], idAccount);
							}
							::ReleaseMutex(g_xDatabase);	//------------------------------------------
						}
						
						int		nFeeType		= c_typeNone;
						char	szServerName[_MAX_NAMESIZE] = "(未知)";
						OBJID	idFeeAccount	= ID_NONE;
						char	szFeeAccount[_MAX_ACCOUNT_NAMESIZE] = "(未知)";
						int		nPoint			= 0;
						if(g_pOnlineTable->GetAttr2(idAccount, nFeeType, szServerName, idFeeAccount, szFeeAccount))	//? 可能引起APPENDNEW()的非原子性操作
						{
							if((nFeeType == c_typePoint || nFeeType == c_typeNetBarPoint 
								|| nFeeType == c_typeISP) && cMsg.m_pInfo->ucType != CMsgFee::FEE_TICK)		//? 非计点类型不减点
							{
								if(strcmp(szServerName, m_aServerName[nServerIndex]) == 0)
								{
									//  [5/26/2007]	王玉波注释----------------begin----------------------------
									//	说明：接收到服务器的计点信息，减点功能去除，因现在为免费运营模式
									//	服务器端不再发TICK消息，正常发FEE_POINT消息，但频率以原来TICK的频率（每3分钟）,这样做是因为要使用以下这部分功能，因为有cMsg.m_pInfo->ucType != CMsgFee::FEE_TICK
									//								bool	bOk		= false;
									//								::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
									//								if(g_cDatabase.Create(idFeeAccount))	// VVVVVVVVVVVVVVV
									//								{
									//									bOk		= true;
									//									nPoint	= g_cDatabase.GetPoint();
									//									g_cDatabase.Destroy();			// AAAAAAAAAAAAAAA
									//								}
									//								::ReleaseMutex(g_xDatabase);	//------------------------------------------
									//								
									//								if(bOk)
									//								{
									//									if(nPoint > 0)
									//									{
									//										if(g_cDatabase.DecPoint(idFeeAccount))		//?? 不需要OPEN，直接使用CDATABASE的查询语句。
									//										{
									//											InterlockedIncrement(&s_nPointFee);
									//											InterlockedIncrement(&s_nPointSum);
									//										}
									//										else
									//										{
									//											LOGERROR("数据库错误，帐号ID[%d]无法扣点。★", idFeeAccount);
									//											LOGPOINT("数据库错误，帐号ID[%d]无法扣点。", idFeeAccount);
									//										}
									//									}
									//									else if(KICKOUT_USER_WHEN_NO_POINT)
									//									{
									//										LOGPOINT("服务器[%s]的玩家[%d]因帐号[%s][%d]点数耗尽被踢。", 
									//											m_aServerName[nServerIndex], idAccount, szFeeAccount, idFeeAccount);
									//										Kickout_0(idAccount, m_aServerName[nServerIndex]);
									//									}
									//								}
									//								else
									//								{
									//									LOGERROR("游戏服务器[%s]上传了一个未知帐号[%d]的计点消息。点数未登记★", 
									//										m_aServerName[nServerIndex], idFeeAccount);
									//									LOGPOINT("游戏服务器[%s]上传了一个未知帐号[%d]的计点消息。点数未登记", 
									//										m_aServerName[nServerIndex], idFeeAccount);
									//								}
									//----------------------------------------------end-------------------------------------------------
								}
								else
								{
									LOGERROR("双重登录。玩家ID[%d]使用帐号[%s][%d]登录了[%s]，但收到[%s]上传了的的计点消息。玩家被踢下线★", 
										idAccount, szFeeAccount, idFeeAccount, szServerName, m_aServerName[nServerIndex]);
									Kickout_0(idAccount, m_aServerName[nServerIndex]);
								}
							}
							
							int	nPointCount = g_pOnlineTable->PointFee(idAccount, m_aServerName[nServerIndex]);		// return -n: 服务器不匹配
							//							LOGPOINT("[%s]上传[%d]的POINT_FEE。用了[%d]，剩余[%d]", 
							//								m_aServerName[nServerIndex], idAccount, nPointCount, nPoint-1);
							// 双重登录，踢人
							//						if(nPointCount < 0)		// <0 : error
							{
								//							LOGERROR("...双重登录，多余的POINT_FEE。帐号[%d]被踢下线。", idAccount);
								//							Kickout_0(idAccount, m_aServerName[nServerIndex]);
							}
							
							if(m_aState[nServerIndex] == STATE_NORMAL)
								::SetServerState(nServerIndex, c_flagPoint);
							else
								::SetServerState(nServerIndex, c_flagPoint2);
						} // GetAttr()
						else
						{
							LOGERROR("共享冲突，FEE_POINT补登录帐号不成功");
						}
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::FEE_TICK");
					}
				}
				break;
			case CMsgFee::HEARTBEAT:		//接收到游戏服务器上传的心跳消息
				{
					try
					{
					/*// 消息检查
					if(idAccount == ID_NONE)
					{
					LOGERROR("错误的HEARTBEAT消息[%d]★", idAccount);
					return false;
					}*/
						
						LOGMSG("接收到[%s]游戏服务器上传的心跳消息.", m_aServerName[nServerIndex]);
						// 无操作，ONPROCESS中会自动刷新心跳时间。
						::SetServerState(nServerIndex, c_flagHeartbeat);
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::HEARTBEAT");
					}
				}
				break;
			case CMsgFee::SERVER_BUSY:					//游戏服务器不应该不会上传此消息，因为有了排队机制
				{
					try
					{
						SetServerBusy(nServerIndex);
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::SERVER_BUSY");
					}
				}
				break;
			case CMsgFee::SERVER_FULL:					//游戏服务器不应该不会上传此消息，因为有了排队机制
				{
					try
					{
						SetServerFull(nServerIndex);
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::SERVER_FULL");
					}
				}
				break;
			case CMsgFee::SERVER_CLOSE:					//疑问，未做任何处理
				{
					try
					{
						PrintText("接收到服务器[%s]的关闭消息。", m_aServerName[nServerIndex]);
						//??? SetServerClose(nServerIndex);
						//??? ::SetServerState(nIndex, c_flagOffline);
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::SERVER_CLOSE");
					}
				}
				break;
			default:
				LOGERROR("游戏服务器[%s]计费消息子类型[%d]错误", m_aServerName[nServerIndex], cMsg.m_pInfo->ucType);
			}
		}
		break;
	case _MSG_QUERYFEE:
		{
			CMsgQueryFee	cMsg;
			cMsg.Create(pBuf, nLen);

			// 消息检查
			if(cMsg.m_pInfo->idAccount == ID_NONE)
			{
				LOGERROR("错误的_MSG_QUERYFEE消息[%d]★", cMsg.m_pInfo->idAccount);
				return false;
			}

			int		nFeeType	= c_typeNone;
			DWORD	dwData		= 0;
			int		nTime		= 0;
			char	szServerName[_MAX_NAMESIZE];
			OBJID	idFeeAccount;
			char	szFeeAccount[_MAX_ACCOUNT_NAMESIZE];
			if(g_pOnlineTable->GetAttr2(cMsg.m_pInfo->idAccount, nFeeType, szServerName, idFeeAccount, szFeeAccount))
			{
				switch(nFeeType)
				{
				case	c_typePoint:
					::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
					if(g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
					{
						dwData	= g_cDatabase.GetPoint();
						g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
					}
					else
					{
						LOGERROR("游戏服务器[%s]请求查询一个未知帐号[%d]计费数据", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
					}
					::ReleaseMutex(g_xDatabase);	//------------------------------------------
					break;
				case	c_typeTime:
					::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
					if(g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
					{
						nTime	= g_cDatabase.GetPointTime();
						g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
					}
					else
					{
						LOGERROR("游戏服务器[%s]请求查询一个未知帐号[%d]计费数据", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
					}
					::ReleaseMutex(g_xDatabase);	//------------------------------------------
					break;
				case	c_typeNetBarTime:
					/* 暂不开放
					::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
					if(g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
					{
						dwData	= g_cDatabase.GetLicence();
						g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
					}
					else
					{
						LOGERROR("游戏服务器[%s]请求查询一个未知帐号[%d]计费数据", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
					}
					::ReleaseMutex(g_xDatabase);	//------------------------------------------
					break;
					//*/
				case	c_typeNetBarPoint:
				case	c_typeISP:
				case	c_typeFree:
					break;
				}
			}
/*
			::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
			if(g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
			{
				nType	= g_cDatabase.GetType();
				dwData	= g_cDatabase.GetPoint();
				nTime	= g_cDatabase.GetPointTime();
				g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
			}
			else
			{
				LOGERROR("游戏服务器[%s]请求查询一个未知玩家[%d]计费数据", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
			}
			::ReleaseMutex(g_xDatabase);	//------------------------------------------
//*/
			// 返回消息
			cMsg.m_pInfo->ucType	= nFeeType;
			cMsg.m_pInfo->dwData	= dwData;
			cMsg.m_pInfo->nTime		= nTime;
			m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
//			LOGPOINT("发送给[%s]游戏服务器[%d]帐号的计费查询消息。类型[%d]，点数[%d]，时限[%04d-%02d-%02d]", 
//						m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount, 
//						nFeeType, dwData, nTime/10000, (nTime/100)%100, nTime%100);
		}
		break;
	default:
		LOGERROR("游戏服务器上传了未知的消息类型[%d]", unMsgType);
		LOGPOINT("接收到[%s]游戏服务器上传的未知消息.", m_aServerName[nServerIndex], unMsgType);
	}

	return true;
}

// 返回0：错误
OBJID	CPointThread::GetServerIndex_0(const char * pServerName)
{
	for(int i = 1; i < MAXGAMESERVERS; i++)	// 保留0，0为错误
	{
		if(strcmp(m_aServerName[i], pServerName) == 0)
			break;
	}

	if(i < MAXGAMESERVERS)
	{
		if(m_aServerSocket[i].IsOpen())
		{
			return i;
		}
		else
		{
			if(m_aServerName[i][0])
			{
				LOGWARNING("取服务器ID时，游戏服务器[%s]已经关闭", m_aServerName[i]);
				LOGSERVER("服务器[%s]已经关闭了。", m_aServerName[i]);
				PrintText("服务器“%s”已经关闭了。", m_aServerName[i]);
			}
			Clear(i);		// SOCKET关闭后，删除游戏服务器名。被动同步
		}
	}

	return 0;
}

// 检查服务器是否在线，不在线时转换显示状态，并不让玩家登录。
bool	CPointThread::CheckHeartbeat(int nIndex)
{
	// 恢复BUSY、FULL
	if((m_aState[nIndex] == STATE_BUSY || m_aState[nIndex] == STATE_FULL) && clock() > m_aServerDelay[nIndex])
	{
		m_aState[nIndex] = STATE_NORMAL;
		::SetServerState(nIndex, c_flagNormal);
	}

	bool	ret = false;
	// 检查心跳
	if(m_aHeartbeatLast[nIndex] && (clock() - m_aHeartbeat[nIndex]) / CLOCKS_PER_SEC > HEARTBEATKICKSECS)
	{
		LOGWARNING("游戏服务器[%s]因为响应超时[%d]秒，连接被强行断开", 
						m_aServerName[nIndex], (clock() - m_aHeartbeat[nIndex])/CLOCKS_PER_SEC);
		LOGSERVER("响应超时，服务器[%s]已断开连接!", m_aServerName[nIndex]);
		PrintText("响应超时，服务器“%s”已断开连接!", m_aServerName[nIndex]);
		m_aServerSocket[nIndex].Close();
		Clear(nIndex);
		ret = true;
	}
	else if((clock() - m_aHeartbeat[nIndex]) / CLOCKS_PER_SEC > HEARTBEATINTERVALSECS
				&& (clock() - m_aHeartbeatLast[nIndex]) / CLOCKS_PER_SEC > HEARTBEATINTERVALSECS)
	{
		CMsgFee	cMsg;
		cMsg.Create(666666, CMsgFee::HEARTBEAT);		// 666666: 玩家帐号ID，无意义
		if(m_aServerSocket[nIndex].Send(cMsg.GetBuf(), cMsg.GetSize()))
		{
			LOGERROR("游戏服务器[%s]响应超时[%d]秒，已发送一heartbeat消息", 
						m_aServerName[nIndex], (clock() - m_aHeartbeat[nIndex])/CLOCKS_PER_SEC);
		}
		else
		{
			LOGWARNING("游戏服务器[%s]连接已关闭，无法SEND心跳消息", m_aServerName[nIndex]);
			LOGSERVER("发送心跳失败，服务器[%s]已断开连接!", m_aServerName[nIndex]);
			PrintText("发送心跳失败，服务器“%s”已断开连接!", m_aServerName[nIndex]);
			Clear(nIndex);
		}
		m_aHeartbeatLast[nIndex] = clock();
	}

	return ret;
}

void CPointThread::Clear(int nIndex, bool flag /*= true*/) 
{ 
	m_aState[nIndex] = STATE_OFFLINE;
	
	//  [5/30/2007]王玉波添加-----------begin---------------------------
	if( flag )
	{
		g_pOnlineTable->RemoveByServerName( m_aServerName[ nIndex] );

	}
	//----------------------------------end-----------------------------

	m_aServerName[nIndex][0] = m_aHeartbeat[nIndex] = m_aHeartbeatLast[nIndex] = m_aServerDelay[nIndex] = 0; 
	if(flag)
		::SetServerState(nIndex, c_flagOffline);
}

void CPointThread::SetServerBusy(int nServerIndex) { 
	m_aServerDelay[nServerIndex] = clock() + SERVER_BUSY_DELAY_SECS*CLOCKS_PER_SEC;
	m_aState[nServerIndex] = STATE_BUSY;
	::SetServerState(nServerIndex, c_flagStop);
}

void CPointThread::SetServerFull(int nServerIndex) {
	m_aServerDelay[nServerIndex] = clock() + SERVER_FULL_DELAY_SECS*CLOCKS_PER_SEC;
	m_aState[nServerIndex] = STATE_FULL;
	::SetServerState(nServerIndex, c_flagStop);
}

bool CPointThread::AddTooLoginInfo(OBJID idAccount , const char * pGameServer)
{
	bool hr=false;
	hr = m_objInspectManage.WatchToolInfo(idAccount,pGameServer,1);
	return hr;
}






