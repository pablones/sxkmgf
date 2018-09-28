#include "MsgVasLv.h"

CMsgVasLv::CMsgVasLv()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;
}

CMsgVasLv::~CMsgVasLv()
{
	
}

BOOL CMsgVasLv::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return false;
	Init();
	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return true;
}

BOOL CMsgVasLv::Create(unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem)
{
	if(usAction == ACTION_NONE)
		return false;
	
	this->Init();
	
	m_pInfo->usAction	=	usAction;
	m_pInfo->id			=	id;
	m_pInfo->vastotal	=	vastotal;
	m_pInfo->lvtotal	=	lvtotal;
	m_pInfo->dwcount	=	dwcount;
	m_pInfo->vaslv		=	vaslv;
	m_pInfo->awarditem	=	awarditem;
	
	m_unMsgType	= m_pInfo->unMsgType = _MSG_VASLV;
	m_unMsgSize	= m_pInfo->unMsgSize = sizeof(MSG_Info);
	
	return true;
}

BOOL CMsgVasLv::Create(unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid,DWORD questionan)
{
	if(usAction == ACTION_NONE)
		return false;
	
	this->Init();
	
	m_pInfo->usAction	=	usAction;
	m_pInfo->id			=	id;
	m_pInfo->vastotal	=	vastotal;
	m_pInfo->lvtotal	=	lvtotal;
	m_pInfo->dwcount	=	dwcount;
	m_pInfo->vaslv		=	vaslv;
	m_pInfo->awarditem	=	awarditem;
	
	m_pInfo->viscou		=	viscou;
	m_pInfo->maxvis		=	maxvis;
	m_pInfo->vascount	=	vascount;
	m_pInfo->questionid	=	questionid;
	m_pInfo->questionan	=	questionan;
	
	m_unMsgType	= m_pInfo->unMsgType = _MSG_VASLV;
	m_unMsgSize	= m_pInfo->unMsgSize = sizeof(MSG_Info);
	
	return true;
}