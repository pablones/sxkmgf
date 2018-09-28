// MsgTeamPos.cpp: implementation of the CMsgTeamPos class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgTeamPos.h"
#include "MapGroup.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTeamPos::CMsgTeamPos()
{
	this->Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgTeamPos::~CMsgTeamPos()
{

}
//////////////////////////////////////////////////////////////////////
BOOL CMsgTeamPos::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return FALSE;
	
	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgTeamPos::Append(CUser* pMember)
{
	if (!pMember)
		return FALSE;
	
	CTeam* pTeam	= pMember->GetTeam();
	if (!pTeam)
		return FALSE;
	
	m_unMsgSize += sizeof(TeamPosInfo);

	TeamPosInfo info;
	memset(&info, 0L, sizeof(TeamPosInfo));
	info.idPlayer = pMember->GetID();
	info.idMap	  = pMember->GetMap()->GetDocID();
	info.usPosX   = pMember->GetPosX();
	info.usPosY   = pMember->GetPosY();
	::SafeCopy(info.szPlayerName, pMember->GetName(), _MAX_NAMESIZE);
	memcpy(&m_pInfo->memInfo[m_pInfo->ucAmount],&info,sizeof(TeamPosInfo));
	m_pInfo->ucAmount++;
	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgTeamPos::Append(OBJID idMember, UINT idMap, USHORT ucPosX, USHORT ucPosY, const char* szPlayerName)
{
	m_unMsgSize += sizeof(TeamPosInfo);
	
	TeamPosInfo info;
	memset(&info, 0L, sizeof(TeamPosInfo));
	info.idPlayer = idMember;
	info.idMap	  = idMap;
	info.usPosX   = ucPosX;
	info.usPosY   = ucPosY;
	::SafeCopy(info.szPlayerName, szPlayerName, _MAX_NAMESIZE);
	memcpy(&m_pInfo->memInfo[m_pInfo->ucAmount],&info,sizeof(TeamPosInfo));
	m_pInfo->ucAmount++;
	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgTeamPos::Create(int nAction, OBJID idUser)
{
	// init
	this->Init();
	
	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_TEAMPOS;
	
	m_pInfo->ucAction	= nAction;
	m_pInfo->idUser		= idUser;
	m_pInfo->ucAmount	= 0;
	
	return TRUE;
}

BOOL CMsgTeamPos::Append(TeamPosInfo info)
{
	m_unMsgSize += sizeof(TeamPosInfo);

	memcpy(&m_pInfo->memInfo[m_pInfo->ucAmount],&info,sizeof(TeamPosInfo));
	m_pInfo->ucAmount++;

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgTeamPos::Process(void* pInfo)
{
	DEBUG_TRY
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;
	
	switch(m_pInfo->ucAction)
	{
	case _TEAMPOS_GETPOSGROUP:
		{
			m_pInfo->ucAction = _TEAMPOS_SENDSELFPOS;
			if (Append(pUser))
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this,m_pInfo->idUser);
		}
		break;
	default:
		break;
	}
	DEBUG_CATCH2("CMsgTeamPos::Process action=%d", m_pInfo->ucAction)
}