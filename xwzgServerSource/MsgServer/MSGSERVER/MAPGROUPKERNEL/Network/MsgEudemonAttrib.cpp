// MsgEUDATTR.cpp: implementation of the CMsgEudemonAttrib class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgEudemonAttrib.h"
#include "AiNpc.h"
#include "User.h"
#include "UserManager.h"
#include "MapGroup.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgEudemonAttrib::CMsgEudemonAttrib()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgEudemonAttrib::~CMsgEudemonAttrib()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgEudemonAttrib::Create(OBJID idEudemon, int nType, DWORD dwData)
{
	IF_NOT (nType >= 0 && nType < EUDATTR_LAST)
		return false;

	// init
	this->Init();

	// fill info now
	m_unMsgSize		= sizeof(MSG_Info);
	m_unMsgType		= _MSG_EUDEMONATTRIB;

	m_pInfo->idEudemonItem	= idEudemon;
	m_pInfo->dwAttrNum	= 0;

	if (nType >= 0 && nType < EUDATTR_LAST)
	{
		m_unMsgSize	+= sizeof(ST_EUDEMON_ATTRIB);
		m_pInfo->setAttrib[m_pInfo->dwAttrNum].usType	= nType;
		m_pInfo->setAttrib[m_pInfo->dwAttrNum].usData	= dwData;
		m_pInfo->dwAttrNum ++;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgEudemonAttrib::Append(int nType, DWORD dwData)
{
	CHECKF(nType >= 0 && nType < EUDATTR_LAST);
	CHECKF(m_unMsgSize + sizeof(ST_EUDEMON_ATTRIB) < MAX_PACKETSIZE);

	// fill info now
	m_unMsgSize		+= sizeof(ST_EUDEMON_ATTRIB);
	m_pInfo->setAttrib[m_pInfo->dwAttrNum].usType	= nType;
	m_pInfo->setAttrib[m_pInfo->dwAttrNum].usData	= dwData;
	m_pInfo->dwAttrNum ++;

	return true;
}



//////////////////////////////////////////////////////////////////////
//BOOL CMsgEudemonAttrib::Create(USHORT usAction, OBJID idEudemon, int nAttribNum, ST_EUDEMON_ATTRIB setAttrib[])
//{
//	if (usAction == _EA_ACTION_NONE)
//		return false;
//
//	// init
//	this->Init();
//
//	// fill info now
//	m_unMsgSize		= sizeof(MSG_Info);
//	m_unMsgType		= _MSG_EUDATTR;
//
//	m_pInfo->usAction	= usAction;
//	m_pInfo->idEudemon	= idEudemon;
//
//	IF_NOT (m_unMsgSize + nAttribNum*sizeof(ST_EUDEMON_ATTRIB) < MAX_PACKETSIZE)
//		return false;
//	m_pInfo->dwAttributeNum		= nAttribNum;
//
//	for (int i=0; i<nAttribNum; i++)
//	{
//		IF_NOT (setAttrib[i].dwAttributeType >= 0 && setAttrib[i].dwAttributeType < _MAX_EUDEMON_ATTRIB)
//			return false;
//		m_unMsgSize	+= sizeof(ST_EUDEMON_ATTRIB);
//		m_pInfo->setAttrib[i].dwAttributeType	= setAttrib[i].dwAttributeType;
//		m_pInfo->setAttrib[i].dwAttributeData	= setAttrib[i].dwAttributeData;
//	}
//
//	return true;


//////////////////////////////////////////////////////////////////////
BOOL CMsgEudemonAttrib::Create(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_EUDEMONATTRIB != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgEudemonAttrib::Process(CGameSocket* pSocket)
{
#ifdef _DEBUGx
	::LogSave("Process msg: CMsgEudemonAttrib.");
#endif
	
	//modified by xumiao 2010-1-8 
	CUser* pUser = UserManager()->GetUser(this);
	if (!pUser)
		return ;
	if (!pUser->IsAlive())
	{
		pUser->SendSysMsg(STR_DIE);
		return ;
	}
}

//////////////////////////////////////////////////////////////////////
