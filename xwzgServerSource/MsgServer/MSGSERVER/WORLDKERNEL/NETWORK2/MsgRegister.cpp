// MsgRegister.cpp: implementation of the CMsgRegister class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "basefunc.h"
using namespace world_kernel;
#include "worldkernel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgRegister::CMsgRegister()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgRegister::~CMsgRegister()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgRegister::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_REGISTER != this->GetType())
		return false;

	return true;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgRegister::Create(const char* szName, const char* szPassword, OBJID idAccount, USHORT unLook, USHORT unData)
{
	// init
	this->Init();
	
	// fill structure
	m_unMsgType	=_MSG_REGISTER;
	m_unMsgSize	=sizeof(MSG_Info);
	
	m_pInfo->idAccount	= idAccount;
	m_pInfo->unLook		= unLook;
	m_pInfo->unData		= unData;
	if(szName && strlen(szName) < _MAX_NAMESIZE)
		strcpy(m_pInfo->szName, szName);
	if(szPassword && strlen(szPassword) < _MAX_NAMESIZE)
		strcpy(m_pInfo->szPassword, szPassword);
	return true;
}
//////////////////////////////////////////////////////////////////////
void CMsgRegister::Process(void *pInfo)
{
	if(m_pInfo->unLook != 0)	//李京哲0327 此时是注册
	{
// 		if(!StringCheck(m_pInfo->szAccount))
// 		{
// 			CMsgTalk	msg;
// 			if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, STR_ERROR_ILLEGAL_ACCOUNT, NULL, _COLOR_WHITE, _TXTATR_REGISTER))
// 				SendMsg(&msg);
// 			return;
// 		}
// 		
		if(!StringCheck(m_pInfo->szPassword))
		{
			CMsgTalk	msg;
			if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, STR_ERROR_ILLEGAL_PASSWORD, NULL, _COLOR_WHITE, _TXTATR_REGISTER))
				SendMsg(&msg);
			return;
		}
		
		if(!NameStrCheck(m_pInfo->szName))
		{
			CMsgTalk	msg;
			if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, STR_ERROR_ILLEGAL_NAME, NULL, _COLOR_WHITE, _TXTATR_REGISTER))
				SendMsg(&msg);
			return;
		}
		g_UserList.CreateNewPlayer(GetSocketID()/*, m_pInfo->szAccount*/, m_pInfo->szName, m_pInfo->szPassword, 
			m_pInfo->unLook, m_pInfo->unData, m_pInfo->idAccount);
	}
	else//是删除
	{
		int synId = this->GetTransData();
		if(!GameWorld()->DeleteRole(m_pInfo->idAccount,m_pInfo->unData,m_pInfo->szPassword,m_pInfo->szName))
		{
			CMsgTalk	msg;
			if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, "角色删除失败", NULL, _COLOR_WHITE, _TXTATR_REGISTER))
				SendMsg(&msg);
		}
		else
		{
			CMsgTalk	msg;
			if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, "角色删除成功", NULL, _COLOR_WHITE, _TXTATR_REGISTER))
				SendMsg(&msg);
		}
	}

}
