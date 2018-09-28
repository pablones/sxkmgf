// WeaponSoulData.cpp: implementation of the CWeaponSoulData class.
//
//////////////////////////////////////////////////////////////////////
#include "..\stdafx.h"
#include "WeaponSoulData.h"
#include "GiftTempleteManager.h"
#include "GiftManager.h"
#include "NpcMagicManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char	szWeaponSoulTable[]=_TBL_WEAPONSOUL;
CWeaponSoulData::CWeaponSoulData()
{
	m_pData=NULL;
}

CWeaponSoulData::~CWeaponSoulData()
{
	if(m_pData)
		m_pData->Release();
}

//------------------------------
bool CWeaponSoulData::Create (OBJID idWeaponSoul, IDatabase* pDb)
{
	CHECKF(!m_pData);
	CHECKF(pDb);

	// data
	m_pData = CGameDataWeaponSoul::CreateNew();
	IF_NOT(m_pData)
		return false;
	IF_NOT(m_pData->Create(idWeaponSoul, pDb))
		return false;

	return true;
}
//------------------------------
bool CWeaponSoulData::Create(IDatabase* pDb, 
		IRecord* pDefault, 
		const ST_WEAPONSOULDATA* pInfo, 
		bool bInsert/*=true*/, 
		OBJID idNew/*=ID_NONE*/)
{
	CHECKF(pDb);
	CHECKF(pDefault);
	CHECKF(pInfo);

	//id
	OBJID idWpSl = pInfo->ID;
	if(idNew)
		idWpSl = idNew;
	
	// data
	m_pData = CGameDataWeaponSoul::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pDefault, idWpSl))
		return false;

	//内存copy到数据
	m_pData->SetInt(WEAPONSOULDATA_ITEMID,pInfo->idItem);
	m_pData->SetInt(WEAPONSOULDATA_TYPE,pInfo->nType);	
	m_pData->SetInt(WEAPONSOULDATA_LEVEL,pInfo->level );
	m_pData->SetInt(WEAPONSOULDATA_EXP,pInfo->exp );
	m_pData->SetInt(WEAPONSOULDATA_LIFESPAN,pInfo->lifespan);
	m_pData->SetInt(WEAPONSOULDATA_GROW,pInfo->grow);
	m_pData->SetInt(WEAPONSOULDATA_MIXLEV,pInfo->mixlev);
	m_pData->SetInt(WEAPONSOULDATA_PHYATTLEV,pInfo->phyattlev);
	m_pData->SetInt(WEAPONSOULDATA_MAGATTLEV,pInfo->magattlev);
	m_pData->SetInt(WEAPONSOULDATA_BASESTR,pInfo->basestr);
	m_pData->SetInt(WEAPONSOULDATA_BASEINT,pInfo->baseint);
	m_pData->SetInt(WEAPONSOULDATA_BASESTA,pInfo->basesta);
	m_pData->SetInt(WEAPONSOULDATA_BASESPI,pInfo->basespi);
	m_pData->SetInt(WEAPONSOULDATA_BASEAGI,pInfo->baseagi);
	m_pData->SetInt(WEAPONSOULDATA_ADDTION0,pInfo->addition[0]);
	m_pData->SetInt(WEAPONSOULDATA_ADDTION1,pInfo->addition[1]);
	m_pData->SetInt(WEAPONSOULDATA_ADDTION2,pInfo->addition[2]);
	m_pData->SetInt(WEAPONSOULDATA_ADDTION3,pInfo->addition[3]);
	m_pData->SetInt(WEAPONSOULDATA_ADDTION4,pInfo->addition[4]);
	m_pData->SetInt(WEAPONSOULDATA_ADDTION5,pInfo->addition[5]);
	m_pData->SetInt(WEAPONSOULDATA_ADDTION6,pInfo->addition[6]);
	m_pData->SetInt(WEAPONSOULDATA_ADDTION7,pInfo->addition[7]);
	m_pData->SetInt(WEAPONSOULDATA_ADDTION8,pInfo->addition[8]);
	m_pData->SetInt(WEAPONSOULDATA_ADDTION9,pInfo->addition[9]);
	m_pData->SetInt(WEAPONSOULDATA_SKILL1,pInfo->skill[0]);
	m_pData->SetInt(WEAPONSOULDATA_SKILL2,pInfo->skill[1]);
	m_pData->SetInt(WEAPONSOULDATA_SKILL3,pInfo->skill[2]);

	//insert
	if(bInsert)
	{
		IF_NOT(m_pData->InsertRecord())
			return false;
	}
	else
	{
		m_pData->ClearUpdateFlags();
	}
	
	return true;
}
//------------------------------
bool CWeaponSoulData::DeleteRecord(IDatabase* pDb)
{
	return m_pData->DeleteRecord();
}

//-------------------------------
bool CWeaponSoulData::GetInfo(ST_WEAPONSOULDATA& pInfo,bool bCount)
{
	CHECKF(GetID() != ID_NONE);

	pInfo.ID=	GetID();//ID
	pInfo.idItem=	GetInt(WEAPONSOULDATA_ITEMID);
	pInfo.nType=	GetInt(WEAPONSOULDATA_TYPE);
	pInfo.level=	GetInt(WEAPONSOULDATA_LEVEL);
	pInfo.exp=	GetInt(WEAPONSOULDATA_EXP);
	pInfo.lifespan=	GetInt(WEAPONSOULDATA_LIFESPAN);
	pInfo.grow=	GetInt(WEAPONSOULDATA_GROW);
	if(bCount)
	{
		float nGrowCheck = pInfo.grow / 1000.0f;
		pInfo.mixlev=	GetInt(WEAPONSOULDATA_MIXLEV);
		pInfo.phyattlev=	GetInt(WEAPONSOULDATA_PHYATTLEV) * nGrowCheck + 0.5f;
		pInfo.magattlev=	GetInt(WEAPONSOULDATA_MAGATTLEV) * nGrowCheck + 0.5f;
		pInfo.basestr=	GetInt(WEAPONSOULDATA_BASESTR) * nGrowCheck + 0.5f;
		pInfo.baseint=	GetInt(WEAPONSOULDATA_BASEINT) * nGrowCheck + 0.5f;
		pInfo.basesta=	GetInt(WEAPONSOULDATA_BASESTA) * nGrowCheck + 0.5f;
		pInfo.basespi=	GetInt(WEAPONSOULDATA_BASESPI) * nGrowCheck + 0.5f;
		pInfo.baseagi=	GetInt(WEAPONSOULDATA_BASEAGI) * nGrowCheck + 0.5f;
	}
	else
	{
		pInfo.mixlev=	GetInt(WEAPONSOULDATA_MIXLEV);
		pInfo.phyattlev=	GetInt(WEAPONSOULDATA_PHYATTLEV);
		pInfo.magattlev=	GetInt(WEAPONSOULDATA_MAGATTLEV);
		pInfo.basestr=	GetInt(WEAPONSOULDATA_BASESTR);
		pInfo.baseint=	GetInt(WEAPONSOULDATA_BASEINT);
		pInfo.basesta=	GetInt(WEAPONSOULDATA_BASESTA);
		pInfo.basespi=	GetInt(WEAPONSOULDATA_BASESPI);
		pInfo.baseagi=	GetInt(WEAPONSOULDATA_BASEAGI);
	}
	pInfo.addition[0]=	GetInt(WEAPONSOULDATA_ADDTION0);
	pInfo.addition[1]=	GetInt(WEAPONSOULDATA_ADDTION1);
	pInfo.addition[2]=	GetInt(WEAPONSOULDATA_ADDTION2);
	pInfo.addition[3]=	GetInt(WEAPONSOULDATA_ADDTION3);
	pInfo.addition[4]=	GetInt(WEAPONSOULDATA_ADDTION4);
	pInfo.addition[5]=	GetInt(WEAPONSOULDATA_ADDTION5);
	pInfo.addition[6]=	GetInt(WEAPONSOULDATA_ADDTION6);
	pInfo.addition[7]=	GetInt(WEAPONSOULDATA_ADDTION7);
	pInfo.addition[8]=	GetInt(WEAPONSOULDATA_ADDTION8);
	pInfo.addition[9]=	GetInt(WEAPONSOULDATA_ADDTION9);
	pInfo.skill[0]=	GetInt(WEAPONSOULDATA_SKILL1);
	pInfo.skill[1]=	GetInt(WEAPONSOULDATA_SKILL2);
	pInfo.skill[2]=	GetInt(WEAPONSOULDATA_SKILL3);

	return true;
}

bool CWeaponSoulData::GetInfoByDefault(ST_WEAPONSOULDATA& pInfo,int nType/* = 0*/)
{
	pInfo.ID=	ID_NONE;
	pInfo.idItem=	ID_NONE;

	if(nType == 10 || nType == 20)
		pInfo.nType = nType;
	else
		pInfo.nType=	(RandGet(10000) / 5000 + 1) * 10;

	pInfo.level=	1;
	pInfo.exp=	0;
	pInfo.lifespan=	300;
	pInfo.grow=	500;
	pInfo.mixlev=	1;

	
	if(pInfo.nType / 10 == 1)//力量
	{
		pInfo.phyattlev=	30 + pInfo.level * 16.64f + 0.5f;
		pInfo.magattlev=	20 + pInfo.level * 5.96f + 0.5f;
		pInfo.basestr=		2 + pInfo.level * 1.7f + 0.5f;
		pInfo.baseint=		2 + pInfo.level * 0.22f + 0.5f;
		pInfo.basesta=		2 + pInfo.level * 0.35f + 0.5f;
		pInfo.basespi=		2 + pInfo.level * 0.30f + 0.5f;
		pInfo.baseagi=		2 + pInfo.level * 0.27f + 0.5f;
	}
	else
	{
		pInfo.phyattlev=	20 + pInfo.level * 5.96f + 0.5f;
		pInfo.magattlev=	30 + pInfo.level * 16.64f + 0.5f;
		pInfo.basestr=		2 + pInfo.level * 0.22f + 0.5f;
		pInfo.baseint=		2 + pInfo.level * 1.7f + 0.5f;
		pInfo.basesta=		2 + pInfo.level * 0.35f + 0.5f;
		pInfo.basespi=		2 + pInfo.level * 0.30f + 0.5f;
		pInfo.baseagi=		2 + pInfo.level * 0.27f + 0.5f;
	}
	
	pInfo.addition[0]=	(RandGet(14) + 101) * 1000 + 1;
	pInfo.addition[1]=	0;
	pInfo.addition[2]=	0;
	pInfo.addition[3]=	0;
	pInfo.addition[4]=	0;
	pInfo.addition[5]=	0;
	pInfo.addition[6]=	0;
	pInfo.addition[7]=	0;
	pInfo.addition[8]=	0;
	pInfo.addition[9]=	0;
	pInfo.skill[0]= 0;
	pInfo.skill[1]= 0;
	pInfo.skill[2]= 0;
	return true;
}

int	 CWeaponSoulData::ProcGift1001(int nType, int oldValue)
{
	int  nAdd = 0;
	CHECKF(m_pData);
	if(nType > DOTYPE_petAddMaxLife || nType < DOTYPE_addhp)
		return 0;
	int nMixLev = GetInt(WEAPONSOULDATA_MIXLEV);
	int nLev = GetInt(WEAPONSOULDATA_LEVEL);
	
	int skill[11];
	skill[0] = m_pData->GetInt( WEAPONSOULDATA_SKILL1);
	skill[1] = m_pData->GetInt( WEAPONSOULDATA_ADDTION0);
	skill[2] = m_pData->GetInt( WEAPONSOULDATA_ADDTION1);
	skill[3] = m_pData->GetInt( WEAPONSOULDATA_ADDTION2);
	skill[4] = m_pData->GetInt( WEAPONSOULDATA_ADDTION3);
	skill[5] = m_pData->GetInt( WEAPONSOULDATA_ADDTION4);
	skill[6] = m_pData->GetInt( WEAPONSOULDATA_ADDTION5);
	skill[7] = m_pData->GetInt( WEAPONSOULDATA_ADDTION6);
	skill[8] = m_pData->GetInt( WEAPONSOULDATA_ADDTION7);
	skill[9] = m_pData->GetInt( WEAPONSOULDATA_ADDTION8);
	skill[10] = m_pData->GetInt( WEAPONSOULDATA_ADDTION9);
	
	for(int i = 0 ; i < 11 ; i ++)
	{
		if(skill[i] / 1000 == 0)
			continue;
		
		CSoulMagicData* pMagicData = g_pNpcMagicManager->QuerySoulData(skill[i] / 1000);
		if(!pMagicData)
			continue;
		
		int skillType = pMagicData->GetInt(EM_SOULMAGIC_dataID);
		int nCountLev = pMagicData->GetInt(EM_SOULMAGIC_countLev);
		int nLevParam = pMagicData->GetInt(EM_SOULMAGIC_Levparam);
		int nMixParam = pMagicData->GetInt(EM_SOULMAGIC_mixparam);
		int skilllev = skill[i] % 1000;
		if(skillType == 0)
			continue;
		if(skilllev > 10 )
			skilllev = 10;
		if(skilllev < 1)
			skilllev = 1;
		
		CGiftTreeData* pTreeData = g_pGiftManager->QueryGiftTreeSet()->GetObj(skillType);
		if(!pTreeData)
			continue;
		if(pTreeData->GetInt(GIFTTREEDATA_temType) != 1001)
			continue;
		
		IRecord* pGiftTmpRecord = g_pGiftManager->GetGiftTemplete(skillType);
		
		if(!pGiftTmpRecord)
			continue;
		
		//有些没影响
		if(pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1001_dotype) != nType)
			continue;
		
		//武魂特殊计算  不按表中来
		int doValue = EFFECTARRAY[skilllev-1] * ((float)nLevParam / 100.0f) + nMixLev * nMixLev * ((float)nMixParam / 100.0f) + 0.5f ;
		if(nCountLev)
			doValue *= nLev/100.0f;
	
		nAdd += doValue;
	}
	
	return nAdd;
}

int	CWeaponSoulData::ProcGift1002(int nType, int oldValue)
{
	int  nAdd = 0;
	CHECKF(m_pData);
	if(nType > DOTYPE1002_decDef4 || nType < DOTYPE1002_decphydef)
		return 0;
	int nMixLev = GetInt(WEAPONSOULDATA_MIXLEV);
	int nLev = GetInt(WEAPONSOULDATA_LEVEL);
	
	int skill[10];
	skill[0] = m_pData->GetInt( WEAPONSOULDATA_ADDTION0);
	skill[1] = m_pData->GetInt( WEAPONSOULDATA_ADDTION1);
	skill[2] = m_pData->GetInt( WEAPONSOULDATA_ADDTION2);
	skill[3] = m_pData->GetInt( WEAPONSOULDATA_ADDTION3);
	skill[4] = m_pData->GetInt( WEAPONSOULDATA_ADDTION4);
	skill[5] = m_pData->GetInt( WEAPONSOULDATA_ADDTION5);
	skill[6] = m_pData->GetInt( WEAPONSOULDATA_ADDTION6);
	skill[7] = m_pData->GetInt( WEAPONSOULDATA_ADDTION7);
	skill[8] = m_pData->GetInt( WEAPONSOULDATA_ADDTION8);
	skill[9] = m_pData->GetInt( WEAPONSOULDATA_ADDTION9);
	
	for(int i = 0 ; i < 10 ; i ++)
	{
		if(skill[i] / 1000 == 0)
			continue;
		
		CSoulMagicData* pMagicData = g_pNpcMagicManager->QuerySoulData(skill[i] / 1000);
		if(!pMagicData)
			continue;
		
		int skillType = pMagicData->GetInt(EM_SOULMAGIC_dataID);
		int nCountLev = pMagicData->GetInt(EM_SOULMAGIC_countLev);
		int nLevParam = pMagicData->GetInt(EM_SOULMAGIC_Levparam);
		int nMixParam = pMagicData->GetInt(EM_SOULMAGIC_mixparam);
		int skilllev = skill[i] % 1000;
		if(skillType == 0)
			continue;
		if(skilllev > 10 )
			skilllev = 10;
		if(skilllev < 1)
			skilllev = 1;
		
		CGiftTreeData* pTreeData = g_pGiftManager->QueryGiftTreeSet()->GetObj(skillType);
		if(!pTreeData)
			continue;
		if(pTreeData->GetInt(GIFTTREEDATA_temType) != 1002)
			continue;
		
		IRecord* pGiftTmpRecord = g_pGiftManager->GetGiftTemplete(skillType);
		
		if(!pGiftTmpRecord)
			continue;
		
		//有些没影响
		if(pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1002_dotype) != nType)
			continue;
		
		//武魂特殊计算  不按表中来
		int doValue = EFFECTARRAY[skilllev-1] * ((float)nLevParam / 100.0f) + nMixLev * nMixLev * ((float)nMixParam / 100.0f) + 0.5f ;
		if(nCountLev)
			doValue *= nLev/100.0f;
	}
	
	return nAdd;
}

int	CWeaponSoulData::ProcGift1003(stRandBuffData& stData)
{
	stData.nBuff = 0;
	stData.nPow = 0;
	stData.nSec = 0;
	stData.nTime = 0;
	CHECKF(m_pData);
	int nMixLev = GetInt(WEAPONSOULDATA_MIXLEV);
	int nLev = GetInt(WEAPONSOULDATA_LEVEL);

	int skill[2];
	skill[0] = m_pData->GetInt( WEAPONSOULDATA_SKILL2);
	skill[1] = m_pData->GetInt( WEAPONSOULDATA_SKILL3);
	
	for(int i = 0 ; i < 2 ; i ++)
	{
		if(skill[i] / 1000 == 0)
			continue;
		
		CSoulMagicData* pMagicData = g_pNpcMagicManager->QuerySoulData(skill[i] / 1000);
		if(!pMagicData)
			continue;
		
		int skillType = pMagicData->GetInt(EM_SOULMAGIC_dataID);
		int nCountLev = pMagicData->GetInt(EM_SOULMAGIC_countLev);
		int nLevParam = pMagicData->GetInt(EM_SOULMAGIC_Levparam);
		int nMixParam = pMagicData->GetInt(EM_SOULMAGIC_mixparam);
		int skilllev = skill[i] % 1000;
		if(skillType == 0)
			continue;
		if(skilllev > 10 )
			skilllev = 10;
		if(skilllev < 1)
			skilllev = 1;
		
		CGiftTreeData* pTreeData = g_pGiftManager->QueryGiftTreeSet()->GetObj(skillType);
		if(!pTreeData)
			continue;
		if(pTreeData->GetInt(GIFTTREEDATA_temType) != 1003)
			continue;
		
		IRecord* pGiftTmpRecord = g_pGiftManager->GetGiftTemplete(skillType);
		
		if(!pGiftTmpRecord)
			continue;
		
		//触发条件判断
		int doChange = pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1003_dochange) + pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1003_dochangeinc) *  (skilllev - 1) / 100;
		if(RandGet(10000) > doChange)
			continue;

		//武魂特殊计算  不按表中来
		int doValue = EFFECTARRAY[skilllev-1] * ((float)nLevParam / 100.0f) + nMixLev * nMixLev * ((float)nMixParam / 100.0f) + 0.5f ;
		if(nCountLev)
			doValue *= nLev/100.0f;

		stData.nBuff = pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1003_dobuff);
		stData.nPow = doValue;
		stData.nSec = pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1003_dosec);
		stData.nTime = pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1003_dotime);
	}
	
	return 0;
}

int	CWeaponSoulData::ProcGift1004(int nType, int oldValue,stRandData& stData)
{
	stData.nChange = 0;
	stData.nData = 0;
	CHECKF(m_pData);
	if(nType > DOTYPE1004_Att4 || nType < DOTYPE1004_phgdem)
		return 0;
	int nMixLev = GetInt(WEAPONSOULDATA_MIXLEV);
	int nLev = GetInt(WEAPONSOULDATA_LEVEL);
	
	int skill[1];
	skill[0] = m_pData->GetInt( WEAPONSOULDATA_SKILL3);
	
	for(int i = 0 ; i < 1 ; i ++)
	{
		if(skill[i] / 1000 == 0)
			continue;
		
		CSoulMagicData* pMagicData = g_pNpcMagicManager->QuerySoulData(skill[i] / 1000);
		if(!pMagicData)
			continue;
		
		int skillType = pMagicData->GetInt(EM_SOULMAGIC_dataID);
		int nCountLev = pMagicData->GetInt(EM_SOULMAGIC_countLev);
		int nLevParam = pMagicData->GetInt(EM_SOULMAGIC_Levparam);
		int nMixParam = pMagicData->GetInt(EM_SOULMAGIC_mixparam);
		int skilllev = skill[i] % 1000;
		if(skillType == 0)
			continue;
		if(skilllev > 10 )
			skilllev = 10;
		if(skilllev < 1)
			skilllev = 1;
		
		CGiftTreeData* pTreeData = g_pGiftManager->QueryGiftTreeSet()->GetObj(skillType);
		if(!pTreeData)
			continue;
		if(pTreeData->GetInt(GIFTTREEDATA_temType) != 1004)
			continue;
		
		IRecord* pGiftTmpRecord = g_pGiftManager->GetGiftTemplete(skillType);
		
		if(!pGiftTmpRecord)
			continue;
		
		//有些没影响
		if(pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1004_dotype) != nType)
			continue;

		int doChange = pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1004_dochange) + pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1004_dochangeinc) *  (skilllev - 1) / 100;
		stData.nChange += doChange;

		//武魂特殊计算  不按表中来
		int doValue = EFFECTARRAY[nCountLev] * ((float)nLevParam / 100.0f) + nMixLev * nMixLev * ((float)nLevParam / 100.0f)  + 0.5f;
		if(nCountLev)
			doValue *= nLev/100.0f;

		if(doValue > stData.nData)
			stData.nData = doValue;
	}
	
	return 0;
}

//--------------------------------
bool CWeaponSoulData::SaveInfo(void)
{
	CHECKF(m_pData);
	m_pData->Update();
	return true;
}

//------------------------------
int CWeaponSoulData::GetInt(WEAPONSOULDATA idx)
{
	return m_pData->GetInt(idx);	
}

//-----------------------------
LPCTSTR CWeaponSoulData::GetStr(WEAPONSOULDATA idx)
{
	return m_pData->GetStr(idx);
}

//-----------------------------
void CWeaponSoulData::SetInt(WEAPONSOULDATA idx, int nData, bool bUpdate/* = false*/)
{
	m_pData->SetInt(idx, nData);
	if(bUpdate)
		m_pData->Update();
}

//-----------------------------
void CWeaponSoulData::SetStr(WEAPONSOULDATA idx, LPCTSTR szData, int nSize, bool bUpdate/* = false*/)
{
	m_pData->SetStr(idx, szData, nSize);
	if(bUpdate)
		m_pData->Update();
}

bool CWeaponSoulData::UpdateAll(ST_WEAPONSOULDATA wpslInfo,WEAPONSOULUPDATE nType/* =WEAPONSOULUPDATE_MIXLEV*/,int nData/* = 0*/)
{
	CHECKF(m_pData);
	switch(nType)
	{
// 	case WEAPONSOULUPDATE_MIXLEV:
// 		m_pData->SetInt(WEAPONSOULDATA_MIXLEV,wpslInfo.mixlev);
// 		if(wpslInfo.mixlev == 5)
// 		{
// 			m_pData->SetInt(WEAPONSOULDATA_TYPE,wpslInfo.nType / 10 * 10 + RandGet(4) + 1);
// 		}
// 		break;
	case WEAPONSOULUPDATE_PROPOPEN:
		{
			for(int i = 0 ; i < wpslInfo.mixlev ; i ++)
			{
				if(wpslInfo.addition[i] == 0)
					break;
			}

			if(i >= wpslInfo.mixlev )
				return false;

			switch(i)
			{
			case 0:
				m_pData->SetInt(WEAPONSOULDATA_ADDTION0,88888);
				break;
			case 1:
				m_pData->SetInt(WEAPONSOULDATA_ADDTION1,88888);
				break;
			case 2:
				m_pData->SetInt(WEAPONSOULDATA_ADDTION2,88888);
				break;
			case 3:
				m_pData->SetInt(WEAPONSOULDATA_ADDTION3,88888);
				break;
			case 4:
				m_pData->SetInt(WEAPONSOULDATA_ADDTION4,88888);
				break;
			case 5:
				m_pData->SetInt(WEAPONSOULDATA_ADDTION5,88888);
				break;
			case 6:
				m_pData->SetInt(WEAPONSOULDATA_ADDTION6,88888);
				break;
			case 7:
				m_pData->SetInt(WEAPONSOULDATA_ADDTION7,88888);
				break;
			case 8:
				m_pData->SetInt(WEAPONSOULDATA_ADDTION8,88888);
				break;
			case 9:
				m_pData->SetInt(WEAPONSOULDATA_ADDTION9,88888);
				break;
			}
		}
		break;
	case WEAPONSOULUPDATE_PROPRAND:
		{
			WEAPONSOULDATA dataIndex = WEAPONSOULDATA_ADDTION0;
			switch(nData)
			{
			case 0:
				break;
			case 1:
				dataIndex = WEAPONSOULDATA_ADDTION1;
				break;
			case 2:
				dataIndex = WEAPONSOULDATA_ADDTION2;
				break;
			case 3:
				dataIndex = WEAPONSOULDATA_ADDTION3;
				break;
			case 4:
				dataIndex = WEAPONSOULDATA_ADDTION4;
				break;
			case 5:
				dataIndex = WEAPONSOULDATA_ADDTION5;
				break;
			case 6:
				dataIndex = WEAPONSOULDATA_ADDTION6;
				break;
			case 7:
				dataIndex = WEAPONSOULDATA_ADDTION7;
				break;
			case 8:
				dataIndex = WEAPONSOULDATA_ADDTION8;
				break;
			case 9:
				dataIndex = WEAPONSOULDATA_ADDTION9;
				break;
			default:
				return false;
			}
			

			int nLev = 1;
			vector<int> setSkill;
			setSkill.clear();
			for(int i = 101 ; i <= 114 ; i ++)
			{
				setSkill.push_back(i);
			}
			for(i = 0 ; i < 10 ; i ++)
			{
				if(wpslInfo.addition[i] == 0)
					break;
				if(wpslInfo.addition[i] == 88888)
					continue;
				int nType = wpslInfo.addition[i] / 1000;
				if(nType == 0)
					continue;
				
				for(vector<int>::iterator it = setSkill.begin();it != setSkill.end();it++)
				{
					if(*it == nType)
					{
						setSkill.erase(it);
						break;
					}
				}
			}
			int nType = setSkill[RandGet(setSkill.size())];

			if(m_pData->GetInt(dataIndex) != 88888)
				nLev = m_pData->GetInt(dataIndex) % 1000;

			CHECKF(nLev > 0 && nLev < 11);
			m_pData->SetInt(dataIndex,nType * 1000 + nLev);
		}
		break;
	case WEAPONSOULUPDATE_PROPLEVUP:
		{
			WEAPONSOULDATA dataIndex = WEAPONSOULDATA_ADDTION0;
			switch(nData)
			{
			case 0:
				break;
			case 1:
				dataIndex = WEAPONSOULDATA_ADDTION1;
				break;
			case 2:
				dataIndex = WEAPONSOULDATA_ADDTION2;
				break;
			case 3:
				dataIndex = WEAPONSOULDATA_ADDTION3;
				break;
			case 4:
				dataIndex = WEAPONSOULDATA_ADDTION4;
				break;
			case 5:
				dataIndex = WEAPONSOULDATA_ADDTION5;
				break;
			case 6:
				dataIndex = WEAPONSOULDATA_ADDTION6;
				break;
			case 7:
				dataIndex = WEAPONSOULDATA_ADDTION7;
				break;
			case 8:
				dataIndex = WEAPONSOULDATA_ADDTION8;
				break;
			case 9:
				dataIndex = WEAPONSOULDATA_ADDTION9;
				break;
			default:
				return false;
			}

			if(wpslInfo.addition[nData] == 0)
				return false;
			if(wpslInfo.addition[nData] == 88888)
				return false;

			int nType = wpslInfo.addition[nData] / 1000;
			int nLev = wpslInfo.addition[nData] % 1000 + 1;
			CHECKF(nLev > 0 && nLev < 11);
			m_pData->SetInt(dataIndex,nType * 1000 + nLev);
		}
		break;
	case WEAPONSOULUPDATE_EXP:
		{
			if(wpslInfo.level <= m_pData->GetInt(WEAPONSOULDATA_LEVEL))
			{
				m_pData->SetInt(WEAPONSOULDATA_EXP,wpslInfo.exp);
				return true;
			}
			
			//改属性
			if(wpslInfo.nType / 10 == 1)//力量
			{
				m_pData->SetInt(WEAPONSOULDATA_PHYATTLEV,30 + wpslInfo.level * 16.64f + 0.5f);
				m_pData->SetInt(WEAPONSOULDATA_MAGATTLEV,20 + wpslInfo.level * 5.96f + 0.5f);
				m_pData->SetInt(WEAPONSOULDATA_BASESTR,2 + wpslInfo.level * 1.7f + 0.5f);
				m_pData->SetInt(WEAPONSOULDATA_BASEINT,2 + wpslInfo.level * 0.22f + 0.5f);
				m_pData->SetInt(WEAPONSOULDATA_BASESTA,2 + wpslInfo.level * 0.35f + 0.5f);
				m_pData->SetInt(WEAPONSOULDATA_BASESPI,2 + wpslInfo.level * 0.30f + 0.5f);
				m_pData->SetInt(WEAPONSOULDATA_BASEAGI,2 + wpslInfo.level * 0.27f + 0.5f);
			}
			else
			{
				m_pData->SetInt(WEAPONSOULDATA_PHYATTLEV,20 + wpslInfo.level * 5.96f + 0.5f);
				m_pData->SetInt(WEAPONSOULDATA_MAGATTLEV,30 + wpslInfo.level * 16.64f + 0.5f);
				m_pData->SetInt(WEAPONSOULDATA_BASESTR,2 + wpslInfo.level * 0.22f + 0.5f);
				m_pData->SetInt(WEAPONSOULDATA_BASEINT,2 + wpslInfo.level * 1.7f + 0.5f);
				m_pData->SetInt(WEAPONSOULDATA_BASESTA,2 + wpslInfo.level * 0.35f + 0.5f);
				m_pData->SetInt(WEAPONSOULDATA_BASESPI,2 + wpslInfo.level * 0.30f + 0.5f);
				m_pData->SetInt(WEAPONSOULDATA_BASEAGI,2 + wpslInfo.level * 0.27f + 0.5f);
			}
			//一定等级开技能
			if(wpslInfo.skill[0] == 0 && wpslInfo.level >= 40)
				m_pData->SetInt(WEAPONSOULDATA_SKILL1,88888);

			if(wpslInfo.skill[1] == 0 && wpslInfo.level >= 70)
				m_pData->SetInt(WEAPONSOULDATA_SKILL2,88888);

			if(wpslInfo.skill[2] == 0 && wpslInfo.level >= 90)
				m_pData->SetInt(WEAPONSOULDATA_SKILL3,88888);

			//升级了
			m_pData->SetInt(WEAPONSOULDATA_EXP,wpslInfo.exp);
			m_pData->SetInt(WEAPONSOULDATA_LEVEL,wpslInfo.level);
		}
		break;
	case WEAPONSOULUPDATE_SKILLRAND:
		{
			bool bEnd = false;
			for(int i = 0 ; i < 3 ; i ++)
			{
				int nThisCheck = -1;
				WEAPONSOULDATA dataIndex = WEAPONSOULDATA_SKILL1;
				int nLev = 1;
				int nType = 0;
				int nOldType = 0;

				if(nData == -1)//重洗所有
				{
					if((wpslInfo.skill[i] == 0) || (wpslInfo.skill[i] == 88888 ))
						break;
					
					nLev = wpslInfo.skill[i] % 1000;
					nOldType = wpslInfo.skill[i] / 1000;
					nThisCheck = i;
				}
				else if(nData == i) //只洗对应的
				{
					if(wpslInfo.skill[i] != 88888 )
						return false;

					nLev = 1;
					nThisCheck = nData;
					bEnd = true;
				}
						

				if(nThisCheck >= 0)
				{
					
					switch(nThisCheck)
					{
					case 0:
						if(nData == -1)
						{
							nType = 1101 + RandGet(4);
							if(nOldType == nType)
								nType ++;
						}
						else
							nType = 1101 + RandGet(5);
						break;
					case 1:
						if(nData == -1)
						{
							nType = 1201 + RandGet(6);
							if(nOldType == nType)
								nType ++;
						}
						else
							nType = 1201 + RandGet(7);
						dataIndex = WEAPONSOULDATA_SKILL2;
						break;
					case 2:
						if(nData == -1)
						{
							nType = 1301 + RandGet(7);
							if(nOldType == nType)
								nType ++;
						}
						else
							nType = 1301 + RandGet(8);
						dataIndex = WEAPONSOULDATA_SKILL3;
						break;
					default:
						return false;
					}
					
					CHECKF(nLev > 0 && nLev < 11);
					m_pData->SetInt(dataIndex,nType * 1000 + nLev);
				}

				if(bEnd)
					break;
			}
		}
		break;
	case WEAPONSOULUPDATE_SKILLLEVUP:
		{
			WEAPONSOULDATA dataIndex = WEAPONSOULDATA_SKILL1;
			int nLev = 1;
			int nType = 0;
			switch(nData)
			{
			case 0:
				break;
			case 1:
				dataIndex = WEAPONSOULDATA_SKILL2;
				break;
			case 2:
				dataIndex = WEAPONSOULDATA_SKILL3;
				break;
			default:
				return false;
			}
			
			if(m_pData->GetInt(dataIndex) == 0)
				return false;
			if(m_pData->GetInt(dataIndex) == 88888)
				return false;

			nType = m_pData->GetInt(dataIndex) / 1000;
			nLev = m_pData->GetInt(dataIndex) % 1000 + 1;
			
			CHECKF(nLev > 0 && nLev < 11);
			m_pData->SetInt(dataIndex,nType * 1000 + nLev);
		}
		break;
	case WEAPONSOULUPDATE_TYPERAND:
		{
			if(m_pData->GetInt(WEAPONSOULDATA_TYPE) % 10 == 0)
				return false;
			
			int nType = m_pData->GetInt(WEAPONSOULDATA_TYPE) / 10 * 10 + nData;
			m_pData->SetInt(WEAPONSOULDATA_TYPE,nType);
		}
		break;
	case WEAPONSOULUPDATE_GROW:
		{
			CHECKF(nData > 500 && nData <= 1000);
			m_pData->SetInt(WEAPONSOULDATA_GROW,nData);
		}
		break;
	}

	return true;
}
