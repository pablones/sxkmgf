// MsgSchoolMember.cpp: implementation of the CMsgSchoolMember class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgSchoolMember.h"
using namespace world_kernel;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSchoolMember::CMsgSchoolMember()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgSchoolMember::~CMsgSchoolMember()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSchoolMember::Create	(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;
	
	if(_MSG_SCHOOLMEMBER != this->GetType())
		return false;
	
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSchoolMember::Create(OBJID idUser, UCHAR ucAction, ST_MEMBERINFO* pMember, UCHAR ucAmount, UCHAR ucPage)
{
	if (sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + ucAmount*sizeof(ST_MEMBERINFO) >= MAX_PACKETSIZE)
		return false;
	
	if (idUser==ID_NONE)
		return false;
	// init
	this->Init();
	
	// TODO: init data
	m_pInfo->idUser		= idUser;
	m_pInfo->ucAction	= ucAction;
	m_pInfo->ucPage		= ucPage;
	if (pMember)
	{
		m_pInfo->ucAmount	= ucAmount;
		memcpy(m_pInfo->setMember, pMember, sizeof(ST_MEMBERINFO)*ucAmount);
	}
	else
		m_pInfo->ucAmount	= 0;
	
	m_unMsgType	=_MSG_SCHOOLMEMBER;
	m_unMsgSize	=sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + m_pInfo->ucAmount*sizeof(ST_MEMBERINFO);
	
	return true;
}
BOOL CMsgSchoolMember::Append(ST_MEMBERINFO stMember)
{
	if (sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + (m_pInfo->ucAmount+1)*sizeof(ST_MEMBERINFO) >= MAX_PACKETSIZE)
		return FALSE;
	
	memcpy(&m_pInfo->setMember[m_pInfo->ucAmount], &stMember, sizeof(ST_MEMBERINFO));
	++m_pInfo->ucAmount;
	
	m_unMsgSize	=sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + m_pInfo->ucAmount*sizeof(ST_MEMBERINFO);
	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
void CMsgSchoolMember::Process(void* pSocket)
{
}