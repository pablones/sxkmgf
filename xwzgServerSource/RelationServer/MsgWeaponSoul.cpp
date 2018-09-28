// MsgWeaponSoul.cpp: implementation of the CMsgWeaponSoul class.
//
//////////////////////////////////////////////////////////////////////
#include "MsgWeaponSoul.h"
#include "GameBaseCode/BaseFunc.h"

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

//--------------------------------
void CMsgWeaponSoul::Process(void *pInfo)
{
}
