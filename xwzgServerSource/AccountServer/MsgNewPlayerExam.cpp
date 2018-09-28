// MsgNewPlayerExam.cpp: implementation of the CMsgNewPlayerExam class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MsgNewPlayerExam.h"


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
////////////////
// BOOL CMsgNewPlayerExam::Create(int nAccountID,int size, char* strState )///生成增值服务请求消息
// {
// 	// init
// 	this->Init();
// 
// 	// fill info now
// 	m_pInfo->unMsgSize	=sizeof(MSG_Info)-1+size;
// 	m_pInfo->unMsgType	=_MSG_EXAM;
// 	m_pInfo->szBuf		= strState;
// 	return true;
// }


//----------------------------------
BOOL CMsgNewPlayerExam::Create(USHORT usAction, char* pSzTemp,USHORT usSize)
{

	// fill info now
	m_unMsgSize	=sizeof( MSG_Info )-1+usSize;
	m_unMsgType	=_MSG_EXAM;
	m_pInfo->usAction = usAction;
	memcpy(m_pInfo->szBuf,pSzTemp,usSize);


	return true;
}


//////////////////////////////////////////////////////////////////////
void CMsgNewPlayerExam::Process(void *pInfo)
{
	
}