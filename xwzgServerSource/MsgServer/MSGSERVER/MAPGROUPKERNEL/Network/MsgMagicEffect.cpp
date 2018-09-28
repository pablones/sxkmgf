////////////////////////////////////////////////////////////////
//
//		MsgMagicEffect.cpp
//

#include "MsgMagicEffect.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgMagicEffect::CMsgMagicEffect()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgMagicEffect::~CMsgMagicEffect()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMagicEffect::Create(OBJID idUser, int nType, int nLevel, OBJID idTarget, UCHAR ucDir)
{
	CHECKF(idUser != ID_NONE/* && idTarget != ID_NONE*/);

	// init
	this->Init();

	// fill info now
	m_unMsgSize		= sizeof(MSG_Info);
	m_unMsgType		= _MSG_MAGICEFFECT;

	m_pInfo->idUser			= idUser;
	m_pInfo->usType			= nType;
	m_pInfo->usLevel		= nLevel;
	m_pInfo->idTarget		= idTarget;
	m_pInfo->ucDir			= ucDir;
	m_pInfo->ucEffectNum	= 0;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMagicEffect::Create( OBJID idUser, int nType, int nLevel, OBJID idTarget, int nData, UCHAR ucDir,DWORD dwBattleAddInfo )
{
	CHECKF(idUser != ID_NONE && idTarget != ID_NONE);

	// init
	this->Init();

	// fill info now
	m_unMsgSize		= sizeof(MSG_Info);
	m_unMsgType		= _MSG_MAGICEFFECT;

	m_pInfo->idUser			= idUser;
	m_pInfo->usType			= nType;
	m_pInfo->usLevel		= nLevel;
	m_pInfo->idTarget		= idTarget;
	m_pInfo->ucDir			= ucDir;
	m_pInfo->ucEffectNum	= 0;

//	if(idTarget)
	{
		m_unMsgSize		+= sizeof(EffectRoleStruct);
		m_pInfo->setEffectRole[m_pInfo->ucEffectNum].idRole	= idTarget;
		m_pInfo->setEffectRole[m_pInfo->ucEffectNum].nData	= nData;
		m_pInfo->setEffectRole[m_pInfo->ucEffectNum].dwBattleAddInfo = dwBattleAddInfo;
		m_pInfo->ucEffectNum++;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMagicEffect::CreateByPos(OBJID idUser, int nType, int nLevel, int x, int y, UCHAR ucDir, OBJID idTarget/*=ID_NONE*/, int nData/*=0*/)
{
	CHECKF(idUser != ID_NONE);

	// init
	this->Init();

	// fill info now
	m_unMsgSize		= sizeof(MSG_Info);
	m_unMsgType		= _MSG_MAGICEFFECT;

	m_pInfo->idUser			= idUser;
	m_pInfo->usType			= nType;
	m_pInfo->usLevel		= nLevel;
	m_pInfo->usPosX			= x;
	m_pInfo->usPosY			= y;
	m_pInfo->ucDir			= ucDir;
	m_pInfo->ucEffectNum	= 0;

	if(idTarget != ID_NONE)
	{
		m_unMsgSize		+= sizeof(EffectRoleStruct);
		m_pInfo->setEffectRole[m_pInfo->ucEffectNum].idRole	= idTarget;
		m_pInfo->setEffectRole[m_pInfo->ucEffectNum].nData	= nData;
		m_pInfo->ucEffectNum++;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMagicEffect::CreateCollide(OBJID idUser, int nType, int nLevel, OBJID idTarget, int nData, int nCollideDir, UCHAR ucDir)
{
	CHECKF(idUser != ID_NONE && idTarget != ID_NONE);

	// init
	this->Init();

	// fill info now
	m_unMsgSize		= sizeof(MSG_Info);
	m_unMsgType		= _MSG_MAGICEFFECT;

	m_pInfo->idUser			= idUser;
	m_pInfo->usType			= nType;
	m_pInfo->usLevel		= nLevel;
	m_pInfo->ucCollideDir	= nCollideDir;
	m_pInfo->ucDir			= ucDir;
	m_pInfo->ucEffectNum	= 0;

//	if(dwData)
	{
		//m_unMsgSize		+= sizeof(EffectRoleStruct);
		m_pInfo->setEffectRole[m_pInfo->ucEffectNum].idRole	= idTarget;
		m_pInfo->setEffectRole[m_pInfo->ucEffectNum].nData	= nData;
		m_pInfo->ucEffectNum++;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMagicEffect::AppendRole( OBJID idRole, int nData,DWORD dwBattleAddInfo )
{
	CHECKF(idRole != ID_NONE);
	CHECKF(m_unMsgSize + sizeof(EffectRoleStruct) < MAX_PACKETSIZE);

	// fill info now
	m_unMsgSize		+= sizeof(EffectRoleStruct);
	m_pInfo->setEffectRole[m_pInfo->ucEffectNum].idRole	= idRole;
	m_pInfo->setEffectRole[m_pInfo->ucEffectNum].nData	= nData;
	m_pInfo->setEffectRole[m_pInfo->ucEffectNum].dwBattleAddInfo = dwBattleAddInfo;

	m_pInfo->ucEffectNum++;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMagicEffect::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_MAGICEFFECT != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgMagicEffect::Process(CGameSocket* pSocket)
{
	::LogSave("invalid msg CMsgMagicEffect.");
}