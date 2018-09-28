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
//	����
//	������ѡ�����ʱ��Ҫ�������������ʺŷ�����
//	����true����ʾ��������
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

// 	if( m_LoginState >= LOGINOK && m_idSocket >= 0 )	//��֤���ʺŷ���������Ϸ��������������Ϣʱ
// 	{	
// 		if ( m_tTick.ToNextTime( POINTFEE_SECS ) )//3��һ��
// 		{
// 			idAccount = m_idAccount;
// 			return true;
// 		}	
// 	}
// 	idAccount = ID_NONE;
// 	return false;
}

