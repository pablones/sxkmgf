// MsgNPCShop.cpp: implementation of the CMsgNPCShop class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "mapgroup.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgNPCShop::CMsgNPCShop()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);

}

CMsgNPCShop::~CMsgNPCShop()
{

}

BOOL CMsgNPCShop::Create(OBJID id, int nType,const char* pszName)
{
	CHECKF(pszName);

	this->Init();
	m_pInfo->id = id;
	m_pInfo->nType = nType;
	m_pInfo->nItemAmount = 0;

	m_StrPacker.AddString(pszName);
	m_unMsgType	=_MSG_NPCSHOP;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
	
	return true;
}

BOOL CMsgNPCShop::AddItem(OBJID itemType)
{
	if(m_pInfo->nItemAmount >= 60)
		return false;
	m_pInfo->nItemType[m_pInfo->nItemAmount++] = itemType;
	return true;
}
	
BOOL CMsgNPCShop::Create(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;
	
	if(_MSG_NPCSHOP != this->GetType())
		return false;
	return true;
}

void CMsgNPCShop::Process(CGameSocket* pSocket)
{
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;
}
