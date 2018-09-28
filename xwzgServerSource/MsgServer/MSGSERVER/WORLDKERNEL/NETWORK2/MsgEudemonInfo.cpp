// MsgEudemonInfo.cpp: implementation of the CMsgEudemonInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "MsgEudemonInfo.h"
//#include "User.h"
//#include "AiNpc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgEudemonInfo::CMsgEudemonInfo()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;

	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgEudemonInfo::~CMsgEudemonInfo()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgEudemonInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_EUDEMONINFO != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
// BOOL CMsgEudemonInfo::Create(CMonster* pMonster)
// {
// 	// EBUG_CREATEMSG("CMsgEudemonInfo",pRole->GetID(),pRole->IsUser(),"",pRole->GetPosX(),pRole->GetPosY())
// 	// param check
// 	if (!pMonster)
// 		return false;
// 
// 	if(!(pMonster->IsEudemon()))
// 		return false;
// 
// 	// init
// 	this->Init();
// 
// 	// fill info now
// 	m_pInfo->id				= pMonster->GetID();
// // 	m_pInfo->dwStatus[0]	= pMonster->GetEffect() & 0xFFFFFFFF;
// // 	m_pInfo->dwStatus[1]	= (pMonster->GetEffect() >> 32) & 0xFFFFFFFF;
// 	m_pInfo->dwLookFace		= ((IRole*)pMonster)->GetLookFace();
// 	CUser*	pUser			= pMonster->QueryOwnerUser();
// // 	if(pUser)
// // 		m_pInfo->idOwner	= pUser->GetID();	
// // 	m_pInfo->idMonsterType	= pMonster->GetEudemonType();
// // 
// // 	m_pInfo->usPosX			= pMonster->GetPosX();
// // 	m_pInfo->usPosY			= pMonster->GetPosY();
// // 	m_pInfo->ucDir			= pMonster->GetDir();
// 
// // 	m_pInfo->usMaxLife		= pMonster->GetMaxLife();
// // 	m_pInfo->usCurLife		= pMonster->GetLife();
// // 	m_pInfo->nLife_grow		= pMonster->GetMaxLifeGrow();	
// // 	m_pInfo->usMaxMana		= pMonster->GetMaxMana();
// // 	m_pInfo->usCurMana		= pMonster->GetMana();		
// // 	m_pInfo->nManaGrow		= pMonster->GetMaxManaGrow();	
// // 	m_pInfo->nAttack		= pMonster->GetAttack();
// // 	m_pInfo->nAttackGrow		= pMonster->GetAttackGrow();	
// // 	m_pInfo->nAttackMagic		= pMonster->GetMgcAtk();
// // 	m_pInfo->nAttackMagicGrow	= pMonster->GetAttackMagicGrow();	
// // 	m_pInfo->nDefance			= pMonster->GetDef();
// // 	m_pInfo->nDefanceGrow		= pMonster->GetDefanceGrow();	
// // 	m_pInfo->nDefanceMagic		= pMonster->GetMagicDef();
// // 	m_pInfo->nDefanceMagicGrow	= pMonster->GetDefanceMagicGrow();
// // 	m_pInfo->nSkill[0]		= pMonster->GetSkill1(); 
// // 	m_pInfo->nSkill[1]		= pMonster->GetSkill2(); 
// // 	m_pInfo->nSkill[2]		= pMonster->GetSkill3(); 
// // 	m_pInfo->nSkill[3]		= pMonster->GetSkill4(); 
// // 	m_pInfo->nSkill[4]		= pMonster->GetSkill5(); 
// // 	m_pInfo->nSkill[5]		= pMonster->GetSkill6(); 
// // 	
// // 	m_pInfo->nIdentity [0]		= pMonster->GetIdentity1();
// // 	m_pInfo->nIdentity [1]		= pMonster->GetIdentity2();
// // 	m_pInfo->nIdentity [2]		= pMonster->GetIdentity3();
// // 	m_pInfo->nIdentity [3]		= pMonster->GetIdentity4();
// // 	m_pInfo->nIdentity [4]		= pMonster->GetIdentity5();
// // 	m_pInfo->nIdentity [5]		= pMonster->GetIdentity6();
// // 	m_pInfo->nHabit		= pMonster->GetHabit();
// // 	m_pInfo->nLevel		= pMonster->GetLev(); 
// // 	m_pInfo->nexp		= pMonster->GetExp();
// // 	m_pInfo->nSex		= pMonster->GetSex();
// // 	m_pInfo->nPosition	= pMonster->GetPosition();
// // 	m_pInfo->nAllTimes  = pMonster->GetHpTimes() + pMonster->GetMagTimes() + pMonster->GetPhyTimes() + pMonster->GetDefTimes();
// // 	m_pInfo->hpTimes = pMonster->GetHpTimes();
// // 	m_pInfo->phyTimes = pMonster->GetPhyTimes();
// // 	m_pInfo->magTimes = pMonster->GetMagTimes();
// // 	m_pInfo->defTimes = pMonster->GetDefTimes();
// // 	//add by xumiao 2010-1-7
// // 	m_pInfo->nMoveType = pMonster->GetMoveType();
// // 	m_pInfo->nAttackType = pMonster->GetAttackType();
// 	//end
// 
// 	BOOL bSucMake	=true;
// 	bSucMake	&=m_StrPacker.AddString((char*)pMonster->GetName());
// 	bSucMake	&=m_StrPacker.AddString((char*)pMonster->QueryOwnerUser()->GetName());
// 
// 	m_unMsgType	=_MSG_EUDEMONINFO;
// 	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
// 
// 	return true;
// }

// BOOL CMsgEudemonInfo::Create(EudemonData data)
// {
// 	if (data.id == 0)
// 		return false;
// 	
// 	// init
// 	this->Init();
// 	
// 	// fill info now
// 	m_pInfo->id				= data.id;
// 	m_pInfo->dwLookFace		= data.dwLookface;
// 	m_pInfo->needLev		= data.dwNeedlev;
// 	m_pInfo->ownerId		= data.ownerId;
// 	m_pInfo->typeId			= data.typeId;
// 	m_pInfo->life			= data.life;					
// 	m_pInfo->exp			= data.exp;
// 	m_pInfo->addpoint		= data.addpoint;
// 	m_pInfo->gift			= data.gift;
// 	m_pInfo->grow			= data.grow;
// 	m_pInfo->understand		= data.understand;
// 	m_pInfo->level			= data.level;		
// 	m_pInfo->gettype		= data.gettype;
// 	m_pInfo->happy			= data.happy;
// 	m_pInfo->habit			= data.habit;
// 	m_pInfo->bronTime		= data.borntime;
// 	m_pInfo->state			= data.dwState;
// 	m_pInfo->sex			= data.sex;
// 	m_pInfo->score			= data.dwScore;
// 	m_pInfo->lifespan		= data.lifespan;
// 	m_pInfo->maxlifespan	= data.maxlifespan;
// 	m_pInfo->firStrChgMax	= data.maxstrchg;
// 	m_pInfo->firIntChgMax	= data.maxintchg;
// 	m_pInfo->firStaChgMax	= data.maxstachg;
// 	m_pInfo->firSpiChgMax	= data.maxspichg;
// 	m_pInfo->firAgiChgMax	= data.maxagichg;
// 	m_pInfo->firStrChg		= data.dwfirStrChg;
// 	m_pInfo->firIntChg		= data.dwfirIntChg;
// 	m_pInfo->firStaChg		= data.dwfirStaChg;
// 	m_pInfo->firSpiChg		= data.dwfirSpiChg;
// 	m_pInfo->firAgiChg		= data.dwfirAgiChg;
// 	m_pInfo->StrChg			= data.dwStrChg;
// 	m_pInfo->IntChg			= data.dwIntChg;
// 	m_pInfo->StaChg			= data.dwStaChg;
// 	m_pInfo->SpiChg			= data.dwSpiChg;
// 	m_pInfo->AgiChg			= data.dwAgiChg;
// 	m_pInfo->dwStr			= data.dwStr;
// 	m_pInfo->dwInt			= data.dwInt;
// 	m_pInfo->dwSta			= data.dwSta;
// 	m_pInfo->dwSpi			= data.dwSpi;
// 	m_pInfo->dwAgi			= data.dwAgi;
// 	m_pInfo->PhyAttack      = data.dwPhyAtt;		
// 	m_pInfo->MagicAttack	= data.dwMagAtt;		
// 	m_pInfo->PhyDef			= data.dwPhyDef;			
// 	m_pInfo->MagicDef		= data.dwMagDef;		
// 	m_pInfo->Hit			= data.dwhit;				    
// 	m_pInfo->Hide			= data.dwDoge;			
// 	m_pInfo->Crit			= data.dwCrit;			
// 	m_pInfo->Toughness      = data.dwTough;		
// 	m_pInfo->HpMax			= data.dwMaxLife;
// 
// 	if(data.skill[0] != 0)
// 	{
// 		CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonData(data.skill[0] / 1000);
// 		if(pMagicData)
// 			m_pInfo->skill[0]		= pMagicData->GetInt(EM_EUDEMONMAGIC_dataID) * 1000 + data.skill[0] % 1000;
// 		else
// 			m_pInfo->skill[0]		= 0;
// 	}
// 
// 	if(data.skill[1] != 0)
// 	{
// 		CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonData(data.skill[1] / 1000);
// 		if(pMagicData)
// 			m_pInfo->skill[1]		= pMagicData->GetInt(EM_EUDEMONMAGIC_dataID) * 1000 + data.skill[1] % 1000;
// 		else
// 			m_pInfo->skill[1]		= 0;
// 	}
// 
// 	if(data.skill[2] != 0)
// 	{
// 		CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonData(data.skill[2] / 1000);
// 		if(pMagicData)
// 			m_pInfo->skill[2]		= pMagicData->GetInt(EM_EUDEMONMAGIC_dataID) * 1000 + data.skill[2] % 1000;
// 		else
// 			m_pInfo->skill[2]		= 0;
// 	}
// 
// 	if(data.skill[3] != 0)
// 	{
// 		CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonData(data.skill[3] / 1000);
// 		if(pMagicData)
// 			m_pInfo->skill[3]		= pMagicData->GetInt(EM_EUDEMONMAGIC_dataID) * 1000 + data.skill[3] % 1000;
// 		else
// 			m_pInfo->skill[3]		= 0;
// 	}
// 
// 	if(data.skill[4] != 0)
// 	{
// 		CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonData(data.skill[4] / 1000);
// 		if(pMagicData)
// 			m_pInfo->skill[4]		= pMagicData->GetInt(EM_EUDEMONMAGIC_dataID) * 1000 + data.skill[4] % 1000;
// 		else
// 			m_pInfo->skill[4]		= 0;
// 	}
// 
// 	if(data.skill[5] != 0)
// 	{
// 		CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonData(data.skill[5] / 1000);
// 		if(pMagicData)
// 			m_pInfo->skill[5]		= pMagicData->GetInt(EM_EUDEMONMAGIC_dataID) * 1000 + data.skill[5] % 1000;
// 		else
// 			m_pInfo->skill[5]		= 0;
// 	}
// 
// 	if(data.skill[6] != 0)
// 	{
// 		CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonData(data.skill[6] / 1000);
// 		if(pMagicData)
// 			m_pInfo->skill[6]		= pMagicData->GetInt(EM_EUDEMONMAGIC_dataID) * 1000 + data.skill[6] % 1000;
// 		else
// 			m_pInfo->skill[6]		= 0;
// 	}
// 
// 	BOOL bSucMake	=true;
// 	bSucMake	&=m_StrPacker.AddString((char*)data.name);
// 	
// 	m_unMsgType	=_MSG_EUDEMONINFO;
// 	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
// 	
// 	return true;
// 
// }

//////////////////////////////////////////////////////////////////////
void CMsgEudemonInfo::Process(void *pInfo)
{
	return;
}
