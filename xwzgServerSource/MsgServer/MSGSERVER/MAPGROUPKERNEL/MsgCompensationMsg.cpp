// MsgCompensationMsg.cpp: implementation of the CMsgCompensationMsg class.
//
//////////////////////////////////////////////////////////////////////
#include "MapGroup.h"
#include "AllMsg.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgCompensationMsg::CMsgCompensationMsg()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;
	
	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgCompensationMsg::~CMsgCompensationMsg()
{

}


//////////////////////////////////////////////////////////////////////
BOOL CMsgCompensationMsg::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;
	
	if(_MSG_COMPSITIONMSG != this->GetType())
		return false;
	
	return true;
}


//////////////////////////////////////////////////////////////////////
BOOL CMsgCompensationMsg::Create(USHORT nAction,char* szWord)
{
	CHECKF(nAction);	
	
	// init
	this->Init();
	// fill info now
	
	m_pInfo->usAction			= nAction;
	m_StrPacker.AddString(szWord);
	
	m_unMsgType	=_MSG_COMPSITIONMSG;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
	
	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgCompensationMsg::Process(void *pInfo)
{
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;
}

