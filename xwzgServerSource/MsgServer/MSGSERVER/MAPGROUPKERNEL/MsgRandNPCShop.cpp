// MsgRandNPCShop.cpp: implementation of the CMsgRandNPCShop class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "mapgroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgRandNPCShop::CMsgRandNPCShop()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgRandNPCShop::~CMsgRandNPCShop()
{

}

BOOL CMsgRandNPCShop::Create(OBJID id, int nType,const char* pszName)
{
	CHECKF(pszName);
	
	this->Init();
	m_pInfo->id = id;
	m_pInfo->nType = nType;
	m_pInfo->nItemAmount = 0;

	m_StrPacker.AddString(pszName);
	m_unMsgType	=_MSG_RANDNPCSHOP;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();	
	return true;
}

BOOL CMsgRandNPCShop::AddItem(OBJID itemType,DWORD sellType, DWORD leaveAmount, DWORD price)
{
	if(m_pInfo->nItemAmount >= 30)
		return false;
	m_pInfo->stItemInfo[m_pInfo->nItemAmount].nItemType = itemType;
	m_pInfo->stItemInfo[m_pInfo->nItemAmount].nSellType = sellType;
	m_pInfo->stItemInfo[m_pInfo->nItemAmount].nLeaveAmount = leaveAmount;
	m_pInfo->stItemInfo[m_pInfo->nItemAmount].nPrice = price;
	m_pInfo->nItemAmount++;
	return true;
}

BOOL CMsgRandNPCShop::Create(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;
	
	if(_MSG_RANDNPCSHOP != this->GetType())
		return false;

	return true;
}

void CMsgRandNPCShop::Process(CGameSocket* pSocket)
{
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;
}
