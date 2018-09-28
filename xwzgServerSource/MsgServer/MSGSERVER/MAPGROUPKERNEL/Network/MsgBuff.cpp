// MsgBuff.cpp: implementation of the CMsgBuff class.
//	20060922	ÐÞÂÞ
//	BuffÏûÏ¢
//////////////////////////////////////////////////////////////////////

#include "MsgBuff.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgBuff::CMsgBuff()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;	
}

CMsgBuff::~CMsgBuff()
{

}

BOOL CMsgBuff::Create(char *pMsgBuf,DWORD dwSize)
{	
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;
	
	if(_MSG_BUFF != this->GetType())
		return false;
	
	return true;
}
BOOL CMsgBuff::Create( int nAction,OBJID idUser,DWORD dwTypeID,int nTime,int nCount,int nPower,int nParam1,int nParam2,int nParam3 )
{	
	if( idUser == ID_NONE )
	{
		return false;
	}

	m_unMsgSize	=	sizeof( MSG_Info );
	m_unMsgType	=	_MSG_BUFF;

	m_pInfo->nAction	=	nAction;
	m_pInfo->idUser		=	idUser;
	m_pInfo->dwTypeID	=	dwTypeID;
	m_pInfo->nTime		=	nTime;
	m_pInfo->nCount		=	nCount;
	m_pInfo->nPower		=	nPower;
	m_pInfo->nParam1	=	nParam1;
	m_pInfo->nParam2	=	nParam2;
	m_pInfo->nParam3	=	nParam3;

	return true;
}

//-------------------------------------
void CMsgBuff::Process(void *pInfo)
{
}
