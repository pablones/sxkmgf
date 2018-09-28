// MsgJinmei.cpp: implementation of the CMsgJinmei class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgJinmei.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgJinmei::CMsgJinmei()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgJinmei::~CMsgJinmei()
{

}

BOOL CMsgJinmei::Create( char* pMsgBuf, DWORD dwSize )
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;
	
	if(_MSG_JINMEISYS != this->GetType())
		return false;
	
	return true;
}

BOOL CMsgJinmei::Create(OBJID idUser, int nType, int nLev, int nValue, int nLeaveTime, int nStatu, int nAddQueueTime, int nStrengthLev, int nAddRate, int nAllLevGrade, int nAllStrengthLevAdd )
{
	this->Init();
	m_unMsgSize = sizeof(MSG_Info);
	m_unMsgType = _MSG_JINMEISYS;
	m_pInfo->idUser = idUser;
	m_pInfo->nAllLevGrade = nAllLevGrade;
	m_pInfo->nAllStrengthLevAdd = nAllStrengthLevAdd;
	m_pInfo->nAmount = 0;	
 	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nType = nType;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nLev = nLev;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nValue = nValue;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nLeaveTime = nLeaveTime;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nStrengthLev = nStrengthLev;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nAddRate = nAddRate;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nStatu = nStatu;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nAddQueueTime = nAddQueueTime;
	m_pInfo->nAmount++;

	return TRUE;
}

BOOL CMsgJinmei::Append( int nType, int nLev, int nValue, int nLeaveTime, int nStrengthLev, int nAddRate, int nStatu, int nAddQueueTime )
{
	if (m_unMsgSize + sizeof(UserJinmeiInfoStruct) > _MAX_MSGSIZE)
	{
		return FALSE;
	}

	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nType = nType;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nLev = nLev;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nValue = nValue;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nLeaveTime = nLeaveTime;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nStrengthLev = nStrengthLev;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nAddRate = nAddRate;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nStatu = nStatu;
	m_pInfo->setJinmeiAttr[m_pInfo->nAmount].nAddQueueTime = nAddQueueTime;
	m_unMsgSize += sizeof(UserJinmeiInfoStruct);
	m_pInfo->nAmount++;    

	return TRUE;
}

void CMsgJinmei::Process( CGameSocket* pSocket )
{
}