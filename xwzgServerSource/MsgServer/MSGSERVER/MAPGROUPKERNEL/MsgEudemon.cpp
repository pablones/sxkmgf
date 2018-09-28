// MsgEudemon.cpp: implementation of the CMsgEudemon class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "User.h"
#include "AiNpc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgEudemon::CMsgEudemon()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgEudemon::~CMsgEudemon()
{

}

BOOL CMsgEudemon::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_EUDEMON != this->GetType())
		return false;

	return true;
}

BOOL CMsgEudemon::Append		(EudemonData data,int nMoney)
{
	m_pInfo->data[m_pInfo->usAmount].id = data.id;		
	m_pInfo->data[m_pInfo->usAmount].gettype = data.gettype;
	m_pInfo->data[m_pInfo->usAmount].playTime = data.playtimes;
	m_pInfo->data[m_pInfo->usAmount].lookface = data.dwLookface;
	m_pInfo->data[m_pInfo->usAmount].needlev = data.dwNeedlev;
	m_pInfo->data[m_pInfo->usAmount].sex = data.sex;
	m_pInfo->data[m_pInfo->usAmount].habit = data.habit;
	m_pInfo->data[m_pInfo->usAmount].lev = data.level;
	m_pInfo->data[m_pInfo->usAmount].gift = data.gift;
	m_pInfo->data[m_pInfo->usAmount].bronTime = data.borntime;
	m_pInfo->data[m_pInfo->usAmount].understand = data.understand;
	m_pInfo->data[m_pInfo->usAmount].failtime = data.failtime;
	m_pInfo->data[m_pInfo->usAmount].money = nMoney;
	m_pInfo->data[m_pInfo->usAmount].nGrow = data.grow;
	m_pInfo->data[m_pInfo->usAmount].usStatus = data.dwState;
	m_pInfo->data[m_pInfo->usAmount].fusion = data.fusion;
	m_pInfo->data[m_pInfo->usAmount].intell = data.intelligence;
	strcpy(m_pInfo->data[m_pInfo->usAmount].szName,data.name);
	m_pInfo->usAmount++;

	m_unMsgSize	= sizeof(MSG_Info) + (m_pInfo->usAmount - 1) * sizeof(eudemonShow);
	if(m_unMsgSize >= MAX_PACKETSIZE)
		return false;
	return true;
}
BOOL CMsgEudemon::Create(USHORT usAction,EudemonData* data ,OBJID idOwner)
{
	// init
	this->Init();
	m_pInfo->usAction = usAction;
	m_pInfo->usAmount = 0;
	if (idOwner)
		m_pInfo->idOwner = idOwner;
	else if(data)
		m_pInfo->idOwner = data->ownerId;
	if(data != NULL)
	{
		m_pInfo->usAmount = 1;
		m_pInfo->data[0].id = data->id;		
		m_pInfo->data[0].gettype = data->gettype;
		m_pInfo->data[0].playTime = data->playtimes;
		m_pInfo->data[0].lookface = data->dwLookface;
		m_pInfo->data[0].needlev = data->dwNeedlev;
		m_pInfo->data[0].sex = data->sex;
		m_pInfo->data[0].habit = data->habit;
		m_pInfo->data[0].lev = data->level;
		m_pInfo->data[0].gift = data->gift;
		m_pInfo->data[0].bronTime = data->borntime;
		m_pInfo->data[0].understand = data->understand;
		m_pInfo->data[0].failtime = data->failtime;
		m_pInfo->data[0].nGrow = data->grow;
		m_pInfo->data[0].usStatus = data->dwState;
		m_pInfo->data[0].fusion = data->fusion;
		m_pInfo->data[0].intell = data->intelligence;
		strcpy(m_pInfo->data[0].szName,data->name);
	}
	m_unMsgType	=_MSG_EUDEMON;
	m_unMsgSize	= sizeof(MSG_Info);
	return true;

}

//////////////////////////////////////////////////////////////////////
void CMsgEudemon::Process(void *pInfo)
{
	return;
}
