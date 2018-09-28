// MsgTeamMember.cpp: implementation of the CMsgTeamMember class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "team.h"
#include "mapgroup.h"
#include <memory.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTeamMember::CMsgTeamMember()
{
	this->Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgTeamMember::~CMsgTeamMember()
{
	
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTeamMember::Create(CUser* pMember)
{
	if (!pMember)
		return false;

	CTeam* pTeam	= pMember->GetTeam();
	if (!pTeam)
		return false;

	TeamMemberInfo info;
	memset(&info, 0L, sizeof(TeamMemberInfo));
	info.id			= pMember->GetID();
	info.dwLookFace	= pMember->GetLookFace();
	info.dwLev      = pMember->GetLev();
// 	info.usHp		= pMember->GetLife();
// 	info.usMaxHp	= pMember->GetMaxLife();
	info.usProfession = pMember->GetProfession();
	info.usProLev     = pMember->GetJobLev();
	strcpy(info.szName, pMember->GetName());
 //	info.usPosX		=  pMember->GetPosX() ;//-------彭峰---2007/05/31-----
 //	info.usPosY		=  pMember->GetPosY() ;//-------彭峰---2007/05/31-----
	// init
	this->Init();

	// fill info now
	m_unMsgType	= _MSG_TEAMMEMBER;

	m_pInfo->ucAction	= _MSG_TEAMMEMBER_ADDMEMBER;
	m_pInfo->ucAmount	= 1;
	// By Minsky,2010-05-28
	//memcpy(m_pInfo->setMember, &info, sizeof(TeamMemberInfo));
	m_pInfo->usSizeIn = 1;
	m_pInfo->usSizeOut = 0;
	memcpy(&m_pInfo->memInfo[0],&info, sizeof(TeamMemberInfo));

	m_unMsgSize	= sizeof(MSG_Info) - 1 + sizeof(TeamMemberInfo);

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTeamMember::Create(int nAction, TeamMemberInfo setInfo)
{
	// init
	this->Init();

	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_TEAMMEMBER;

	m_pInfo->ucAction	= nAction;
	m_pInfo->ucAmount	= 1;
	
	// By Minsky,2010-05-28
	//memcpy(m_pInfo->setMember, &setInfo, sizeof(TeamMemberInfo));
	if(setInfo.id == ID_NONE)
	{
		m_pInfo->usSizeIn = 0;
		m_pInfo->usSizeOut = 0;
		m_unMsgSize -= sizeof(TeamMemberInfo);
		return true;
	}
	m_pInfo->usSizeIn = 1;
	m_pInfo->usSizeOut = 0;
	memcpy(&m_pInfo->memInfo[0],&setInfo,sizeof(TeamMemberInfo));
	return true;
}

BOOL CMsgTeamMember::Append			(int nAction,TeamMemberInfo info)
{
	m_unMsgSize += sizeof(TeamMemberInfo);
	if(nAction == 1)
	{
		memcpy(&m_pInfo->memInfo[m_pInfo->usSizeIn],&info,sizeof(TeamMemberInfo));
		m_pInfo->usSizeIn ++;
	}
	else
	{
		memcpy(&m_pInfo->memInfo[m_pInfo->usSizeIn + m_pInfo->usSizeOut],&info,sizeof(TeamMemberInfo));
		m_pInfo->usSizeOut ++;
	}
	return true;
}

BOOL CMsgTeamMember::Create(int nAction, UINT nMapID, UINT id, int nData)
{
	// init
	this->Init();

	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_TEAMMEMBER;

	m_pInfo->ucAction	= nAction;
	m_pInfo->ucAmount   = nMapID;
	m_pInfo->id = id;
	m_pInfo->nData = nData;

	// By Minsky,2010-05-28，添加标志位，标志该信息没最后2个包
	m_pInfo->usSizeIn = 0;
	m_pInfo->usSizeOut = 0;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTeamMember::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return false;

	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgTeamMember::Process(void* pInfo)
{
}