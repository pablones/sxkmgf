// MsgPlayDetil.cpp: implementation of the CMsgPlayDetil class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "mapgroup.h"
#include "User.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgPlayDetil::CMsgPlayDetil()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	
	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgPlayDetil::~CMsgPlayDetil()
{

}

BOOL CMsgPlayDetil::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;
	
	if(_MSG_PLAYERDETIL != this->GetType())
		return false;
	
	return true;
}

BOOL CMsgPlayDetil::Create(CUser* pUser)
{
	// init
	this->Init();
	m_pInfo->id				= pUser->GetID();
	for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i ++)
	{
 		CItem* pItem = pUser->GetEquipItemByPos(i);
		if(!pItem)
			continue;
		m_pInfo->nEquip[(i - ITEMPOSITION_EQUIPBEGIN) * 2]		= pItem->GetTypeID()*10+pItem->GetColour();
		m_pInfo->nEquip[(i - ITEMPOSITION_EQUIPBEGIN) * 2 + 1]	= pItem->GetID();
	}

	m_pInfo->usWarSroce		= pUser->GetWarSroce();
	m_pInfo->usUserlev		= pUser->GetLev();
	m_pInfo->usFirPro		= pUser->GetProfession(1);
	m_pInfo->usFirProLev	= pUser->GetJobLev(1);
	m_pInfo->usSecPro		= pUser->GetProfession(2);
	m_pInfo->usSecProLev	= pUser->GetJobLev(2);
	m_pInfo->usSex			= pUser->GetSex();
	BOOL bSucMake			=true;

	if(pUser->GetSyndicate())
		bSucMake	&=m_StrPacker.AddString(pUser->GetSyndicate()->GetStr(SYNDATA_NAME));
	else
		bSucMake	&=m_StrPacker.AddString("未加入帮会");
	//bSucMake	&=m_StrPacker.AddString("未使用称号");
	char szTitle[10] = "";
	sprintf(szTitle, "%d", pUser->GetUseTitle());
	bSucMake	&=m_StrPacker.AddString(szTitle);
	bSucMake	&=m_StrPacker.AddString(pUser->GetFeel());
	bSucMake	&=m_StrPacker.AddString("无");
	m_unMsgType	=_MSG_PLAYERDETIL;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
	return bSucMake;
}

//////////////////////////////////////////////////////////////////////
void CMsgPlayDetil::Process(void *pInfo)
{
	return;
}
