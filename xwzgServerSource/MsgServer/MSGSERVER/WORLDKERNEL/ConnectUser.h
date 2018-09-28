// ConnectUser.h: interface for the CConnectUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONNECTUSER_H__9394419B_3694_4679_A978_091F18D965C6__INCLUDED_)
#define AFX_CONNECTUSER_H__9394419B_3694_4679_A978_091F18D965C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Timeout.h"

const int	DELETECONNECTUSER_SECS		= 600;		// 20秒超时，删除连接表
const int  LOGINOK = 1;
const int  CLIENTOK  = 2;
const int  JOIN = LOGINOK|CLIENTOK;

class CConnectUser  
{
protected:
	CConnectUser();
	virtual ~CConnectUser();
public:
	static CConnectUser* CreateNew()	{ return new CConnectUser; }
	ULONG	ReleaseByOwner()			{ delete this; return 0; }

	OBJID	GetID()						{ return m_idAccount; }
	bool	Create(OBJID id, DWORD dw, LPCTSTR szInfo, SOCKET_ID idSocket = SOCKET_NONE,int state = 0,int nYouke = 0,ULONG nIP=0);

public:
	bool	CheckData(DWORD dw)			{ return m_dwCheckData == dw; }
	SOCKET_ID	GetSocketID()			{ return m_idSocket; }
	LPCTSTR	GetInfo()					{ return m_szInfo; }
	bool	IsValid()					{ return !m_tDelete.IsTimeOut(); }
	int		GetLoginState()				{ return m_LoginState;}
	void	SetLoginState(int state)	{ m_LoginState=state;}
	void	SetSocketID(SOCKET_ID idSocket){ m_idSocket = idSocket; }
	
	ULONG	GetULongIP()				{ return m_nIP; }
	void	SetULongIP(ULONG nIP)		{ m_nIP = nIP; }
	bool	CheckHeartBeat( OBJID& idAccount );			//07.5.26王玉波添加
	int		m_Youke;

protected:
	OBJID		m_idAccount;
	DWORD		m_dwCheckData;
	NAMESTR		m_szInfo;
	SOCKET_ID	m_idSocket;

	CTimeOut	m_tDelete;

	int			m_LoginState;		//李京哲0307 客户端登陆的状态，3种	
//-------------07.5.26王玉波添加--------------------
	CTimeOut	m_tTick;
//-----------end------------------------------------
	ULONG		m_nIP;//ip地址
protected: // ctrl
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_CONNECTUSER_H__9394419B_3694_4679_A978_091F18D965C6__INCLUDED_)
