// MsgNewPlayerExam.cpp: implementation of the CMsgNewPlayerExam class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "WorldKernel.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgNewPlayerExam::CMsgNewPlayerExam()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;
}

CMsgNewPlayerExam::~CMsgNewPlayerExam()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgNewPlayerExam::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_EXAM != this->GetType())
		return false;

	return true;
}

BOOL CMsgNewPlayerExam::Create(USHORT usAction, void* pSzTemp,USHORT usSize,int AccountID,int UserID)
{
	Init();
	// fill info now
	m_unMsgSize	=sizeof( MSG_Info )-1+usSize;
	m_unMsgType	=_MSG_EXAM;
	m_pInfo->usAction = usAction;
	m_pInfo->nAccountID = AccountID;
	m_pInfo->nUserID	= UserID;
	m_pInfo->usSzSize = usSize;
	memcpy(m_pInfo->szBuf,pSzTemp,usSize);


	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgNewPlayerExam::Process(void *pInfo)
{
	GameWorld()->SendNewPlayerExam( m_pInfo->nUserID, m_pInfo->nAccountID, m_unMsgType, m_unMsgSize );
}