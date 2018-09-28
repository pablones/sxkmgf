// UserList.h: interface for the CUserList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERLIST_H__419FFB53_E120_437C_9BF8_B8839D75C9E1__INCLUDED_)
#define AFX_USERLIST_H__419FFB53_E120_437C_9BF8_B8839D75C9E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "AllMsg.h"
#include "../share/define.h"
#include "windows.h"
#include "NetworkDef.h"
#include "TimeOut.h"
#include "ClientSocket.h"
#include "AutoPtr.h"
#include "GameObjSet.h"
#include <time.h>
#include <vector>
#include <map>
using namespace std;
using namespace world_kernel;

struct CPlayer
{
	OBJID		idUser;
	NAMESTR		szName;
	PROCESS_ID	idProcess;

	NAMESTR		szAccount;				// 仅用于无帐号服务器登录
	SOCKET_ID	idSocket;			// idSocket == SOCKET_NONE : is agent
	ULONG		m_nIP;

	CPlayer() { idUser = ID_NONE; m_tPoint = 0; idSocket = SOCKET_NONE; fetchID = 0; fetchType = 0;fetchSynID = 0;m_nIP = 0; }
	OBJID	GetID()					{ return idUser; }
	void	ReleaseByOwner()		{ delete this; }
	OBJID	GetProcessID()			{ return idProcess; }
	bool	SendMsg(CNetMsg* pMsg);
	bool	IsGM()					{ return (strstr(szName, "[GM]") || strstr(szName, "[PM]")); }
	bool	IsPM()					{ return (strstr(szName, "[PM]") != NULL); }
	bool    fetchTest(OBJID id,OBJID synid,int type = 1);
	void	fetchSet(OBJID id,OBJID synid,int type = 1);
	OBJID	fetchGet(int typeGet,int type = 1);

public: // point fee
	void	OnTimer(time_t tCurr);
public:
	int		m_tPoint;
	CTimeOut	m_tTick;
	OBJID	m_idAccount;
	int 	m_nLevel;

	NAMESTR		szNotify;

	OBJID fetchID;
	OBJID fetchSynID;
	int fetchType;
};

enum { LOGIN_OK, LOGIN_BAN };
const int RECOVER_FISHTIRED = 500;    //钓鱼疲劳恢复数

class IDatabase;
class CUserList  
{
public:
	CUserList();
	virtual ~CUserList();
	static CUserList*	CreateNew()		{ return new CUserList; }
	bool	Create(ISocket* pPort, IDatabase* pDb);
	ULONG	Release()			{ delete this; return 0; }

public:
	bool	LoginUser(LPCTSTR szAccount, LPCTSTR szPassword, SOCKET_ID idSocket, ULONG nIP);	// 仅用于非帐号服务器启动
	int		LoginUser(OBJID idAccount, SOCKET_ID idSocket, LPCTSTR szInfo, ULONG nIP);			// enum { LOGIN_OK, LOGIN_NEW, LOGIN_BAN };
	bool	LogoutUser(SOCKET_ID idSocket);		// return false: no this user
	bool	KickOutAccount(OBJID idAccount, LPCTSTR szText);		// return false: 不在线
	bool	KickOutSocket(SOCKET_ID idSocket);		// return false: 不在线
	void	OnTimer(time_t tCurr);
	void	LogoutAllUser();
	bool	CreateUser(PROCESS_ID idProcess, OBJID idUser, LPCTSTR szName, OBJID idAccount, SOCKET_ID idSocket, int nLevel, LPCTSTR szNotify, ULONG nIP);
	CPlayer*	GetPlayer(LPCTSTR szName);
	CPlayer*	GetPlayer(OBJID idUser);
	CPlayer*	GetPlayerBySocket(SOCKET_ID idSocket)	{ if(idSocket>=0 && idSocket<m_setUser.size()) return m_setUser[idSocket]; return NULL; }
	CPlayer*	GetPlayerByAccount(LPCTSTR szAccount);
	CPlayer*	GetPlayerByAccountID(OBJID idAccount);
	CPlayer*	GetPlayerByIndex(int index)
	{
		return m_setUser[index];
	}
	CPlayer*	GetGM()		{ if (m_setGm.size() <= 0) return NULL; else return this->GetPlayer(m_setGm.front()); }
	CPlayer*	GetRandomGM();///		{ if (m_setGm.size() <= 0) return NULL; else return this->GetPlayer(m_setGm.front()); }
	bool	CreateNewPlayer(SOCKET_ID idSocket/*, LPCTSTR szAccount*/, LPCTSTR szName, LPCTSTR szPassword, 
							int unLook, int unData, OBJID idAccount/*, char cLength, char cFat, int nSex*/);
	void	ChangeProcessID(SOCKET_ID idSocket, PROCESS_ID idProcess);
	bool	IsLoginMapGroup(SOCKET_ID idSocket)			{ return (idSocket>=0 && idSocket<m_setUser.size() && m_setUser[idSocket] && m_setUser[idSocket]->idProcess >= MSGPORT_MAPGROUP_FIRST); }
	void	LoginToMapGroup(CPlayer* pPlayer);
	int		GetUserAmount()								{ return m_nPlayerAmount; }
	void	SendTimeToSocket(SOCKET_ID idSocket);
	void	BroadcastMsg(CNetMsg* pMsg, CPlayer* pExcludeSender=NULL);			// 全世界广播
	void	BroadcastMsg(LPCTSTR szMsg, CPlayer* pExcludeSender=NULL, LPCTSTR pszEmotion=NULL, DWORD dwColor=0xffffff, int nAttribute=_TXTATR_NORMAL);			// 全世界广播
protected:
	int m_nHour;
	CTimeOut m_tHour;
	typedef vector<CPlayer*>	USERLIST_SET;
	USERLIST_SET	m_setUser;
	typedef	CGameObjSet<CPlayer>	AGENT_SET;
	vector<OBJID>	m_setGm;
	typedef	map<string,OBJID> USERNAME_SET;
	USERNAME_SET	m_setUserName;					// 以UserName为索引。reference point set

	ISocket*		m_pSocket;
	int				m_nPlayerAmount;
	int				m_nMaxPlayerAmount;
public: // ctrl
	int LoginUserSnap(OBJID idAccount, SOCKET_ID idSocket,bool bCreate = false );
	MYHEAP_DECLARATION(s_heap)
};



#endif // !defined(AFX_USERLIST_H__419FFB53_E120_437C_9BF8_B8839D75C9E1__INCLUDED_)
