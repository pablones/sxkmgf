// MsgAutoFight.cpp: implementation of the CMsgAutoFight class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgAutoFight.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAutoFight::CMsgAutoFight()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_unMsgSize = sizeof(MSG_Info);
}

CMsgAutoFight::~CMsgAutoFight()
{

}


BOOL CMsgAutoFight::Create(char* pMsgBuf, DWORD dwSize)
{
// 	if (!CNetMsg::Create(pMsgBuf, dwSize))
// 		return false;
// 
// 	if(_MSG_AUTOFIGHT != this->GetType())
// 		return false;

	return true;
}

BOOL CMsgAutoFight::Create(_AutoFight nAction, int nData /* = 0 */)
{
	return TRUE;
// 	m_unMsgType                 = _MSG_AUTOFIGHT;
// 	m_pInfo->nAction            = nAction;
// 
// 	m_pInfo->way = nData;
// 	
}

BOOL CMsgAutoFight::Create(int nAction, AUTOF &pInfo)
{
	m_unMsgType                 = _MSG_AUTOFIGHT;
	m_pInfo->nAction            = nAction;

//	m_pInfo->id                 = pInfo->id;
	m_pInfo->idUser             = pInfo.idUser;    
	m_pInfo->AutoSwitch			= pInfo.AutoSwitch;
	m_pInfo->PercentHp			= pInfo.PercentHp;
	m_pInfo->PercentMp			= pInfo.PercentMp;
	m_pInfo->SkillHp			= pInfo.SkillHp;
	m_pInfo->SkillMp			= pInfo.SkillMp;
	m_pInfo->SkillTeamhp		= pInfo.SkillTeamhp;
	m_pInfo->EudHp				= pInfo.EudHp;
	m_pInfo->EudHappy			= pInfo.EudHappy;
	m_pInfo->HitRange			= pInfo.HitRange;
	m_pInfo->DoubleExp			= pInfo.DoubleExp;
	m_pInfo->DoubleEudExp		= pInfo.DoubleEudExp;
	m_pInfo->LIYUCHUN			= pInfo.LIYUCHUN;
	m_pInfo->temp				= pInfo.temp;
	m_pInfo->Hpmed1				= pInfo.Hpmed1;
	m_pInfo->Hpmed2				= pInfo.Hpmed2;
	m_pInfo->Hpmed3				= pInfo.Hpmed3;
	m_pInfo->Mpmed1				= pInfo.Mpmed1;
	m_pInfo->Mpmed2				= pInfo.Mpmed2;
	m_pInfo->Mpmed3				= pInfo.Mpmed3;
	m_pInfo->EudHpmed1			= pInfo.EudHpmed1;
	m_pInfo->EudHpmed2			= pInfo.EudHpmed2;
	m_pInfo->EudHpmed3			= pInfo.EudHpmed3;
	m_pInfo->EudToy1			= pInfo.EudToy1;
	m_pInfo->EudToy2			= pInfo.EudToy2;

	m_unMsgSize = sizeof(MSG_Info);

	return TRUE;
}

void CMsgAutoFight::Process(void *pInfo)
{
	CUserPtr pUser	= UserManager()->GetUser(this);
	if (!pUser)
		return;
	
	switch(m_pInfo->nAction)
	{
	case _AutoFight_WRITE:
		{
			struct AutoFightInfo pInfo;

			pInfo.idUser            = m_pInfo->idUser;
			pInfo.AutoSwitch		= m_pInfo->AutoSwitch;
			pInfo.PercentHp			= m_pInfo->PercentHp;
			pInfo.PercentMp			= m_pInfo->PercentMp;
			pInfo.SkillHp			= m_pInfo->SkillHp;
			pInfo.SkillMp			= m_pInfo->SkillMp;
			pInfo.SkillTeamhp		= m_pInfo->SkillTeamhp;
			pInfo.EudHp				= m_pInfo->EudHp;
			pInfo.EudHappy			= m_pInfo->EudHappy;
			pInfo.HitRange			= m_pInfo->HitRange;
			pInfo.DoubleExp			= m_pInfo->DoubleExp;
			pInfo.DoubleEudExp		= m_pInfo->DoubleEudExp;
			pInfo.LIYUCHUN			= m_pInfo->LIYUCHUN;
			pInfo.temp				= m_pInfo->temp;
			pInfo.Hpmed1			= m_pInfo->Hpmed1;
			pInfo.Hpmed2			= m_pInfo->Hpmed2;
			pInfo.Hpmed3			= m_pInfo->Hpmed3;
			pInfo.Mpmed1			= m_pInfo->Mpmed1;
			pInfo.Mpmed2			= m_pInfo->Mpmed2;
			pInfo.Mpmed3			= m_pInfo->Mpmed3;
			pInfo.EudHpmed1			= m_pInfo->EudHpmed1;
			pInfo.EudHpmed2			= m_pInfo->EudHpmed2;
			pInfo.EudHpmed3			= m_pInfo->EudHpmed3;
			pInfo.EudToy1			= m_pInfo->EudToy1;
			pInfo.EudToy2			= m_pInfo->EudToy2;


			pUser->WriteAutoFightData(pInfo);
		}
		break;

	case _AutoFight_READ:
		{
			pUser->ReplyAutoFightData();
		}
		break;
// 
// 	case _AutoFight_PICKUP:
// 		{
// 			pUser->SetAutoFightPickUpMode(m_pInfo->Pickup);
// 		}
// 		break;

	default:
		break;
	}
}
