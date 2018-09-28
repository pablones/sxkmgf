////////////////////////////////////////////////////////////////
//
//		MsgUserAttrib.cpp
//

#include "AllMsg.h"
#include "VariantManager.h"
#include "AdjustRate.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgUserAttrib::CMsgUserAttrib()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgUserAttrib::~CMsgUserAttrib()
{

}


/*
//////////////////////////////////////////////////////////////////////
BOOL CMsgUserAttrib::Create(OBJID idUser, int nType, DWORD dwData)
{
	CHECKF(nType == _USERATTRIB_NONE || nType >= 0);

	// init
	this->Init();

	// fill info now
	m_unMsgSize		= sizeof(MSG_Info);
	m_unMsgType		= _MSG_USERATTRIB;

	m_pInfo->idUser	=idUser;
	m_pInfo->dwAttributeNum	= 0;

	if(nType >= 0)
	{
		m_unMsgSize		+= sizeof(UserAttribStruct);
		m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= nType;
		m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= dwData;
		m_pInfo->dwAttributeNum++;
	}

	return true;
}
*/

//////////////////////////////////////////////////////////////////////
BOOL CMsgUserAttrib::Create(OBJID idUser, int nType, __int64 i64Data)
{
	// init
	this->Init();

	// fill info now
	m_unMsgSize		= sizeof(MSG_Info) - sizeof(UserAttribStruct);
	m_unMsgType		= _MSG_USERATTRIB;

	m_pInfo->idUser	=idUser;
	m_pInfo->dwAttributeNum	= 0;

	if(nType >= 0)
	{
		m_unMsgSize		+= sizeof(UserAttribStruct);
		m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= nType;
		m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= i64Data & 0xFFFFFFFF;
		m_pInfo->dwAttributeNum++;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
/*
BOOL CMsgUserAttrib::Append(int nType, DWORD dwData)
{
	CHECKF(nType >= 0);
	CHECKF(m_unMsgSize + sizeof(UserAttribStruct) < MAX_PACKETSIZE);

	// fill info now
	m_unMsgSize		+= sizeof(UserAttribStruct);
	m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= nType;
	m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= dwData;
	m_pInfo->dwAttributeNum++;

	return true;
}
*/

//////////////////////////////////////////////////////////////////////
BOOL CMsgUserAttrib::Append(int nType, __int64 i64Data)
{
	CHECKF(nType >= 0);
	CHECKF(m_unMsgSize + sizeof(UserAttribStruct) < MAX_PACKETSIZE);

	m_unMsgSize		+= sizeof(UserAttribStruct);
	m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= nType;
	m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= i64Data & 0xFFFFFFFF;
	m_pInfo->dwAttributeNum++;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgUserAttrib::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_USERATTRIB != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgUserAttrib::Process(CGameSocket* pSocket)
{
	//::LogSave("invalid msg CMsgUserAttrib.");
	CUserPtr pUser	= UserManager()->GetUser(this);
	if (!pUser)
		return;

	int nAmount = (int)m_pInfo->dwAttributeNum;
	for(int i = 0; i < nAmount; i ++)
	{
		switch(m_pInfo->setUserAttrib[i].ucAttributeType)
		{
			case _USERATTRIB_MAXSOUL:
			//	m_pInfo->setUserAttrib[i].dwAttributeData = g_adjustRate.Get_AdjustRate(_ADJUSTRATE_SOUL_LIMIT,_ADJUSTRATE_ADJUSTRATE);
			//	m_pInfo->setUserAttrib[i].dwAttributeData = (UserManager()->GetUser(this))->GetMaxSoulAmount();
				SendMsg(this);
				break;
			case _USERATTRIB_CURRENTSOUL:
			//	m_pInfo->setUserAttrib[i].dwAttributeData = g_VariantManager.GetVariantInt(SPIRIT_LIMIT,m_pInfo->idUser);
				SendMsg(this);
				break;
			case _USERATTRIB_AUTOFIGHT:
				{
					pUser->SetAutoFightFlag(m_pInfo->setUserAttrib[i].dwAttributeData);
				}
				break;
			default:
				break;
		}
	}
}