// MsgWeaponSoul.cpp: implementation of the CMsgWeaponSoul class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgWeaponSoul.h"
using namespace world_kernel;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgWeaponSoul::CMsgWeaponSoul()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgWeaponSoul::~CMsgWeaponSoul()
{

}

//------------------------------
BOOL CMsgWeaponSoul::Create(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_WEAPONSOUL != this->GetType())
		return false;

	return true;
}

BOOL CMsgWeaponSoul::Create(int nAction/* = EM_MSGWPSL_UPDATE*/,OBJID idUser/*=ID_NONE*/,OBJID idItem/*=ID_NONE*/,OBJID idNeedUser/* = ID_NONE*/)
{
	// init
	this->Init();
	
	// fill info now
	m_pInfo->usAction = nAction;
	m_pInfo->idItem = idItem;
	m_pInfo->idUser = idUser;
	m_pInfo->ID = idNeedUser;

	
	m_unMsgType	=_MSG_WEAPONSOUL;
	m_unMsgSize	=sizeof(MSG_Info);
	return true;
}

//--------------------------------
void CMsgWeaponSoul::Process(void *pInfo)
{
}
