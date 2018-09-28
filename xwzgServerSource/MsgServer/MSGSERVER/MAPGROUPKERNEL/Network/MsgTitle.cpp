// MsgTitle.cpp: implementation of the CMsgTitle class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgTitle.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTitle::CMsgTitle()
{
	this->Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgTitle::~CMsgTitle()
{

}
//////////////////////////////////////////////////////////////////////
BOOL CMsgTitle::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return FALSE;
	
	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgTitle::Create(int nAction, OBJID idUser)
{
	if (idUser==ID_NONE)
		return FALSE;
	// init
	this->Init();
	
	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_TITLE;
	
	m_pInfo->uAction	= nAction;
	m_pInfo->idUser		= idUser;
	m_pInfo->uAmount	= 0;
	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgTitle::Append(UINT uType, UINT uLevel)
{
	m_unMsgSize += sizeof(TitleInfo);
	
	TitleInfo info;
	memset(&info, 0L, sizeof(TitleInfo));
	info.uType = uType;
	info.uLevel = uLevel;
	memcpy(&m_pInfo->titleInfo[m_pInfo->uAmount],&info,sizeof(TitleInfo));
	m_pInfo->uAmount++;
	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
void CMsgTitle::Process(void* pInfo)
{
}