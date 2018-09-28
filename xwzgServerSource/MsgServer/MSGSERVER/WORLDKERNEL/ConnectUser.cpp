// ConnectUser.cpp: implementation of the CConnectUser class.
//
//////////////////////////////////////////////////////////////////////
#include "../share/define.h"
#include "windows.h"
#include "ConnectUser.h"


//using namespace world_kernel;


MYHEAP_IMPLEMENTATION(CConnectUser,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CConnectUser::CConnectUser()
{
	m_LoginState	=	0;
	m_Youke = 0;
}

//////////////////////////////////////////////////////////////////////
CConnectUser::~CConnectUser()
{

}

//////////////////////////////////////////////////////////////////////
bool CConnectUser::Create(OBJID id, DWORD dw, LPCTSTR szInfo, SOCKET_ID idSocket /*= SOCKET_NONE*/,int state,int nYouke/* = 0*/,ULONG nIP/*=0*/)
{
	m_idAccount		= id;
	m_dwCheckData	= dw;
	SafeCopy(m_szInfo, szInfo, _MAX_NAMESIZE);
	m_idSocket		= idSocket;

	m_tDelete.SetInterval(DELETECONNECTUSER_SECS);
	m_tDelete.Update();
	m_LoginState = state;
	m_Youke = nYouke;
	m_nIP = nIP;
	return true;
}
//-------------------------------------------------------------------------------------------------------------
//  [5/26/2007]
//	王玉波
//	在人物选择界面时，要发送心跳包至帐号服务器
//	返回true，表示满足条件
bool CConnectUser::CheckHeartBeat( OBJID& idAccount )
{
	if(m_tDelete.IsActive())
	{
		if(m_tDelete.IsTimeOut())
		{
			idAccount = m_idAccount;
			return true;
		}
	}
	idAccount = ID_NONE;
	return false;

// 	if( m_LoginState >= LOGINOK && m_idSocket >= 0 )	//保证在帐号服务器与游戏服务器都发了信息时
// 	{	
// 		if ( m_tTick.ToNextTime( POINTFEE_SECS ) )//3分一次
// 		{
// 			idAccount = m_idAccount;
// 			return true;
// 		}	
// 	}
// 	idAccount = ID_NONE;
// 	return false;
}

