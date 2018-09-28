// MsgCompensation.cpp: implementation of the CMsgCompensation class.
//
//////////////////////////////////////////////////////////////////////
#include "MapGroup.h"
#include "AllMsg.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgCompensation::CMsgCompensation()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgCompensation::~CMsgCompensation()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgCompensation::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_COMPSITION != this->GetType())
		return false;

	return true;
}

BOOL CMsgCompensation::Add(OBJID id,OBJID typeId,DWORD	nAmount)
{
	if(id == 0)
		return false;
	if(m_pInfo->usAmount != 0)
	{
		if(m_unMsgSize + sizeof(CompensationMiniInfo) > MAX_PACKETSIZE)
			return false;
		if(m_pInfo->usAmount > 50)
			return false;
		m_unMsgSize		+= sizeof(CompensationMiniInfo);
	}
	

	// fill info now
	m_pInfo->m_CompensationInfo[m_pInfo->usAmount].id				= id;
	m_pInfo->m_CompensationInfo[m_pInfo->usAmount].typeId			= typeId;
	m_pInfo->m_CompensationInfo[m_pInfo->usAmount].nAmount			= nAmount;
	m_pInfo->usAmount++;
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgCompensation::Create(USHORT nAction,OBJID id, OBJID typeId, DWORD nAmount)
{
	CHECKF(nAction);	

	// init
	this->Init();
	// fill info now

	m_pInfo->usAction			= nAction;

	if(id == ID_NONE)
		m_pInfo->usAmount			= 0;
	else
	{
		m_pInfo->usAmount			= 1;
		m_pInfo->m_CompensationInfo[0].id			= id;
		m_pInfo->m_CompensationInfo[0].typeId		= typeId;
		m_pInfo->m_CompensationInfo[0].nAmount		= nAmount;
	}

	
	m_unMsgType	=_MSG_COMPSITION;
	m_unMsgSize	=sizeof(MSG_Info);

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgCompensation::Process(void *pInfo)
{
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;

	switch(m_pInfo->usAction)
	{
	case COMPOSITONACTION_QUERY:
		pUser->SendCompentationInfo();
		pUser->SendMsg(this);
		break;
	case COMPOSITONACTION_QUERYMSG:
		pUser->SendCompentationInfoMsg();
		pUser->SendMsg(this);
		break;
	case COMPOSITONACTION_PICK:
		bool bOther = false;
		if(pUser->PickCompentation(m_pInfo->m_CompensationInfo->id,bOther))
		{
			CMsgCompensation msg;
			msg.Create(COMPOSITONACTION_PICK_SUC,m_pInfo->m_CompensationInfo->id,m_pInfo->m_CompensationInfo->typeId,m_pInfo->m_CompensationInfo->nAmount);
			pUser->SendMsg(&msg);
		}
		else
		{
			CMsgCompensation msg;
			msg.Create(COMPOSITONACTION_PICK_FAIL,m_pInfo->m_CompensationInfo->id,m_pInfo->m_CompensationInfo->typeId,m_pInfo->m_CompensationInfo->nAmount);
			pUser->SendMsg(&msg);
		}
		break;
	}
}                                                                        
