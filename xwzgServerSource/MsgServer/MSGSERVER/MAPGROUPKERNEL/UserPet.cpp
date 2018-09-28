// UserPet.cpp: implementation of the CUserPet class.
//
//////////////////////////////////////////////////////////////////////

#include "UserPet.h"
#include "I_Role.h"
#include "MapGroup.h"
#include "User.h"
#include "AllMsg.h"
#include "GiftTempleteManager.h"
MYHEAP_IMPLEMENTATION(CUserPet, s_heap)
MYHEAP_IMPLEMENTATION(CUserPeifang, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserPet::CUserPet()
{

}

CUserPet::~CUserPet()
{
	USERPETDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPetData* pData = *it;
		SAFE_RELEASE (pData);
	}

}

bool CUserPet::fillType(EudemonData& data,int nType)
{
	if(data.id == ID_NONE)
		return false;
	if(data.typeId == ID_NONE)
		return false;
	CEudemonTypeData* pEudemonType = EudemonType()->GetObj(data.typeId);
	if(!pEudemonType)
		return false;

	if(nType == 1)
	{
		data.dwLookface = pEudemonType->GetInt(EUDEMONTYPEDATA_lookface);
		data.dwNeedlev = pEudemonType->GetInt(EUDEMONTYPEDATA_needLev);
	}
	else if(nType == 2)
	{
		data.dwLookface = pEudemonType->GetInt(EUDEMONTYPEDATA_lookface);
		data.dwNeedlev = pEudemonType->GetInt(EUDEMONTYPEDATA_needLev);
		data.initgrow = /*pEudemonType->GetInt(EUDEMONTYPEDATA_initgrow)*/100;
		data.maxlifespan = pEudemonType->GetInt(EUDEMONTYPEDATA_maxlifespan);
		data.maxstrchg = pEudemonType->GetInt(EUDEMONTYPEDATA_initstrChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randstrChg);
		data.maxintchg = pEudemonType->GetInt(EUDEMONTYPEDATA_initintChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randintChg);
		data.maxstachg = pEudemonType->GetInt(EUDEMONTYPEDATA_initstaChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randstaChg);
		data.maxspichg = pEudemonType->GetInt(EUDEMONTYPEDATA_initspiChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randspiChg);
		data.maxagichg = pEudemonType->GetInt(EUDEMONTYPEDATA_initagiChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randagiChg);
	}
	else if(nType == 3)
	{
		data.dwLookface = pEudemonType->GetInt(EUDEMONTYPEDATA_lookface);
		data.dwNeedlev = pEudemonType->GetInt(EUDEMONTYPEDATA_needLev);
		data.initgrow = /*pEudemonType->GetInt(EUDEMONTYPEDATA_initgrow)*/100;
		data.maxlifespan = pEudemonType->GetInt(EUDEMONTYPEDATA_maxlifespan);
		data.maxstrchg = pEudemonType->GetInt(EUDEMONTYPEDATA_initstrChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randstrChg);
		data.maxintchg = pEudemonType->GetInt(EUDEMONTYPEDATA_initintChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randintChg);
		data.maxstachg = pEudemonType->GetInt(EUDEMONTYPEDATA_initstaChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randstaChg);
		data.maxspichg = pEudemonType->GetInt(EUDEMONTYPEDATA_initspiChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randspiChg);
		data.maxagichg = pEudemonType->GetInt(EUDEMONTYPEDATA_initagiChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randagiChg);
	}
	return true;
}

bool CUserPet::fillAtt(EudemonData& data,int nType)
{
	if(data.id == ID_NONE)
		return false;
	if(data.typeId == ID_NONE)
		return false;

	CUserPetData* pThisData= QueryData(data.id);

	data.dwScore = 0;
	
	PETGIFTDATA EudemonGifeData;
	if(nType == 2)
	{
		DWORD thisGrow = data.initgrow + data.grow;
		data.dwStr = data.firstr / 100 + thisGrow / 100.0f * data.level * 5  + data.stradd;
		data.dwInt = data.firint / 100 + thisGrow / 100.0f * data.level * 5  + data.intadd;
		data.dwSta = data.firsta / 100 + thisGrow / 100.0f * data.level * 5 + data.staadd;
		data.dwSpi = data.firspi / 100 + thisGrow / 100.0f * data.level * 5 + data.spiadd;
		data.dwAgi = data.firagi / 100 + thisGrow / 100.0f * data.level * 5 + data.agiadd;

		if(pThisData)
		{
			EudemonGifeData.m_dwGiftAddStr = pThisData->ProcGift1001(DOTYPE_addstr,data.dwStr,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddSta = pThisData->ProcGift1001(DOTYPE_addsta,data.dwSta,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddInt = pThisData->ProcGift1001(DOTYPE_addint,data.dwInt,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddSpi = pThisData->ProcGift1001(DOTYPE_addspi,data.dwSpi,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddAgi = pThisData->ProcGift1001(DOTYPE_addagi,data.dwAgi,valueMask_CONST);
	

			data.dwStr += EudemonGifeData.m_dwGiftAddStr;
			data.dwInt += EudemonGifeData.m_dwGiftAddInt;
			data.dwSta += EudemonGifeData.m_dwGiftAddSta;
			data.dwSpi += EudemonGifeData.m_dwGiftAddSpi;
			data.dwAgi += EudemonGifeData.m_dwGiftAddAgi;

			//goto �����buff״̬Ч��,ֻ�г�ս�ĳ���Ż���
			CMonster* pThisMonster = m_pOwner->QueryCallPet();
			if (pThisMonster && pThisMonster->GetID()==data.id)
			{
				//��"="����"+=",��Ϊ�������ص��Ǽӳ��Ժ��ֵ
				data.dwStr = pThisMonster->AdjustUserAttrib(data.dwStr, ADJUSTUSERATTRIB_STR);
				data.dwInt = pThisMonster->AdjustUserAttrib(data.dwInt, ADJUSTUSERATTRIB_INT);
				data.dwSta = pThisMonster->AdjustUserAttrib(data.dwSta, ADJUSTUSERATTRIB_STA);
				data.dwSpi = pThisMonster->AdjustUserAttrib(data.dwSpi, ADJUSTUSERATTRIB_SPI);
				data.dwAgi = pThisMonster->AdjustUserAttrib(data.dwAgi, ADJUSTUSERATTRIB_AGI);
			}
		}

		float gettypeadd = pow((float)1.06f, (int)data.gettype);
		float intelligenceadd=0.0f;
		
		switch(data.intelligence)
		{
		case 1:
			intelligenceadd = 0.01f;
			break;
		case 2:
			intelligenceadd = 0.03f;
			break;
		case 3:
			intelligenceadd = 0.05f;
			break;
		case 4:
			intelligenceadd = 0.07f;
			break;
		case 5:
			intelligenceadd = 0.09f;
			break;
		case 6:
			intelligenceadd = 0.11f;
			break;
		case 7:
			intelligenceadd = 0.13f;
			break;
		case 8:
			intelligenceadd = 0.15f;
			break;
		case 9:
			intelligenceadd = 0.17f;
			break;
		case 10:
			intelligenceadd = 0.2f;
			break;
		}

		float undestandadd = 1.0f;
		switch (data.understand)
		{
		case 1:
			undestandadd = 1.010f;
			break;
		case 2:
			undestandadd = 1.016f;
			break;
		case 3:
			undestandadd = 1.026f;
			break;
		case 4:
			undestandadd = 1.041f;
			break;
		case 5:
			undestandadd = 1.066f;
			break;
		case 6:
			undestandadd = 1.105f;
			break;
		case 7:
			undestandadd = 1.168f;
			break;
		case 8:
			undestandadd = 1.268f;
			break;
		case 9:
			undestandadd = 1.429f;
			break;
		case 10:
			undestandadd = 1.687f;
			break;
		}

		data.dwStrChg = data.strchg * gettypeadd * (undestandadd + intelligenceadd); //2011��6��10�գ������������ʼӳ�
		data.dwIntChg = data.intchg * gettypeadd * (undestandadd + intelligenceadd);
		data.dwStaChg = data.stachg * gettypeadd * (undestandadd + intelligenceadd);
		data.dwSpiChg = data.spichg * gettypeadd * (undestandadd + intelligenceadd);
		data.dwAgiChg = data.agichg * gettypeadd * (undestandadd + intelligenceadd);
		
		data.dwMaxLife = (DWORD)(data.dwSta *  data.dwStaChg * 0.026775f + data.level * 80.2f - data.level * data.level * 0.2825f + 120.5f);

		if(pThisData)
		{
			EudemonGifeData.m_dwGiftAddhp = pThisData->ProcGift1001(DOTYPE_addhp,data.dwMaxLife,valueMask_CONST);
			data.dwMaxLife += EudemonGifeData.m_dwGiftAddhp;

			//goto �����buff״̬Ч��,ֻ�г�ս�ĳ���Ż���
			CMonster* pThisMonster = m_pOwner->QueryCallPet();
			if (pThisMonster && pThisMonster->GetID()==data.id)
			{
				//��"="����"+=",��Ϊ�������ص��Ǽӳ��Ժ��ֵ
				data.dwMaxLife = pThisMonster->AdjustUserAttrib(data.dwMaxLife, ADJUSTUSERATTRIB_MAXLIFE);
			}
		}
		

		data.dwScore = 0;
// 		data.dwScore = data.strchg + data.intchg + data.stachg + data.spichg + data.agichg ;
// 		data.dwScore += (data.skill[0] % 10 )* 200 + (data.skill[1] % 10 )* 200;
// 		for(int i = 0 ; i < 5 ; i ++)
// 		{
// 			data.dwScore += (data.skill[i+2] % 10 )* 150;
// 		}
	}
	else if(nType == 3)
	{
		data.dwScore = 100;
		//data.dwState = 0;

		DWORD thisGrow = data.initgrow + data.grow;
		data.dwStr = data.firstr / 100 + thisGrow / 100.0f * data.level * 5  + data.stradd;
		data.dwInt = data.firint / 100 + thisGrow / 100.0f * data.level * 5  + data.intadd;
		data.dwSta = data.firsta / 100 + thisGrow / 100.0f * data.level * 5 + data.staadd;
		data.dwSpi = data.firspi / 100 + thisGrow / 100.0f * data.level * 5 + data.spiadd;
		data.dwAgi = data.firagi / 100 + thisGrow / 100.0f * data.level * 5 + data.agiadd;

		if(pThisData)
		{
			EudemonGifeData.m_dwGiftAddStr = pThisData->ProcGift1001(DOTYPE_addstr,data.dwStr,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddSta = pThisData->ProcGift1001(DOTYPE_addsta,data.dwSta,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddInt = pThisData->ProcGift1001(DOTYPE_addint,data.dwInt,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddSpi = pThisData->ProcGift1001(DOTYPE_addspi,data.dwSpi,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddAgi = pThisData->ProcGift1001(DOTYPE_addagi,data.dwAgi,valueMask_CONST);
			
			data.dwStr += EudemonGifeData.m_dwGiftAddStr;
			data.dwInt += EudemonGifeData.m_dwGiftAddInt;
			data.dwSta += EudemonGifeData.m_dwGiftAddSta;
			data.dwSpi += EudemonGifeData.m_dwGiftAddSpi;
			data.dwAgi += EudemonGifeData.m_dwGiftAddAgi;

			//goto �����buff״̬Ч��,ֻ�г�ս�ĳ���Ż���
			CMonster* pThisMonster = m_pOwner->QueryCallPet();
			if (pThisMonster && pThisMonster->GetID()==data.id)
			{
				//��"="����"+=",��Ϊ�������ص��Ǽӳ��Ժ��ֵ
				data.dwStr = pThisMonster->AdjustUserAttrib(data.dwStr, ADJUSTUSERATTRIB_STR);
				data.dwInt = pThisMonster->AdjustUserAttrib(data.dwInt, ADJUSTUSERATTRIB_INT);
				data.dwSta = pThisMonster->AdjustUserAttrib(data.dwSta, ADJUSTUSERATTRIB_STA);
				data.dwSpi = pThisMonster->AdjustUserAttrib(data.dwSpi, ADJUSTUSERATTRIB_SPI);
				data.dwAgi = pThisMonster->AdjustUserAttrib(data.dwAgi, ADJUSTUSERATTRIB_AGI);
			}
		}
		
		float gettypeadd = pow((float)1.06f, (int)data.gettype);

		float undestandadd = 1.0f;
		switch (data.understand)
		{
		case 1:
			undestandadd = 1.010f;
			break;
		case 2:
			undestandadd = 1.016f;
			break;
		case 3:
			undestandadd = 1.026f;
			break;
		case 4:
			undestandadd = 1.041f;
			break;
		case 5:
			undestandadd = 1.066f;
			break;
		case 6:
			undestandadd = 1.105f;
			break;
		case 7:
			undestandadd = 1.168f;
			break;
		case 8:
			undestandadd = 1.268f;
			break;
		case 9:
			undestandadd = 1.429f;
			break;
		case 10:
			undestandadd = 1.687f;
			break;
		}

		float intelligenceadd=0.0f;	
		switch(data.intelligence)
		{
		case 1:
			intelligenceadd = 0.01f;
			break;
		case 2:
			intelligenceadd = 0.03f;
			break;
		case 3:
			intelligenceadd = 0.05f;
			break;
		case 4:
			intelligenceadd = 0.07f;
			break;
		case 5:
			intelligenceadd = 0.09f;
			break;
		case 6:
			intelligenceadd = 0.11f;
			break;
		case 7:
			intelligenceadd = 0.13f;
			break;
		case 8:
			intelligenceadd = 0.15f;
			break;
		case 9:
			intelligenceadd = 0.17f;
			break;
		case 10:
			intelligenceadd = 0.2f;
			break;
		}

		data.dwfirStrChg = (data.strchg * gettypeadd);
		data.dwfirIntChg = (data.intchg * gettypeadd);
		data.dwfirStaChg = (data.stachg * gettypeadd);
		data.dwfirSpiChg = (data.spichg * gettypeadd);
		data.dwfirAgiChg = (data.agichg * gettypeadd);

		data.dwStrChg = data.strchg * gettypeadd * (undestandadd + intelligenceadd);
		data.dwIntChg = data.intchg * gettypeadd * (undestandadd + intelligenceadd);
		data.dwStaChg = data.stachg * gettypeadd * (undestandadd + intelligenceadd);
		data.dwSpiChg = data.spichg * gettypeadd * (undestandadd + intelligenceadd);
		data.dwAgiChg = data.agichg * gettypeadd * (undestandadd + intelligenceadd);
		
		data.dwMaxLife = (DWORD)(data.dwSta *  data.dwStaChg * 0.026775f + data.level * 80.2f - data.level * data.level * 0.2825f + 120.5f);
		data.dwPhyAtt = (DWORD)(data.dwStr *  data.dwStrChg * 0.008505f + 0.5f);
		data.dwMagAtt = (DWORD)(data.dwInt *  data.dwIntChg * 0.008505f + 0.5f);
		data.dwPhyDef = (DWORD)((data.dwStr *  data.dwStrChg + data.dwSpi *  data.dwSpiChg * 2) * 0.0038f + 0.5f);
		data.dwMagDef = (DWORD)((data.dwInt *  data.dwIntChg + data.dwSpi *  data.dwSpiChg * 2) * 0.0038f + 0.5f);
		data.dwhit = (DWORD)(data.dwAgi *  data.dwAgiChg * 0.001806f + 0.5f);
		data.dwDoge = (DWORD)((data.dwSta *  data.dwStaChg * 2 + data.dwAgi *  data.dwAgiChg * 6) * 0.000110571f + 0.5f);
		data.dwCrit = (DWORD)(data.dwAgi *  data.dwAgiChg * 0.000728571f + 0.5f);
		data.dwTough = data.level * 10;

		if(pThisData)
		{
			EudemonGifeData.m_dwGiftAddhp = pThisData->ProcGift1001(DOTYPE_addhp,data.dwMaxLife,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddphyatt = pThisData->ProcGift1001(DOTYPE_addphyatt,data.dwPhyAtt,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddmagatt = pThisData->ProcGift1001(DOTYPE_addmagatt,data.dwMagAtt,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddphydef = pThisData->ProcGift1001(DOTYPE_addphydef,data.dwPhyDef,valueMask_CONST) + pThisData->ProcGift1001(DOTYPE_addPANDMDEF,0,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddmagdef = pThisData->ProcGift1001(DOTYPE_addmagdef,data.dwMagDef,valueMask_CONST) + pThisData->ProcGift1001(DOTYPE_addPANDMDEF,0,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddhit = pThisData->ProcGift1001(DOTYPE_addhit,data.dwhit,valueMask_CONST) + pThisData->ProcGift1001(DOTYPE_addHANDD,0,valueMask_CONST);
			EudemonGifeData.m_dwGiftAdddodge = pThisData->ProcGift1001(DOTYPE_adddodge,data.dwDoge,valueMask_CONST) + pThisData->ProcGift1001(DOTYPE_addHANDD,0,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddcrihit = pThisData->ProcGift1001(DOTYPE_addcrihit,data.dwCrit,valueMask_CONST);
			EudemonGifeData.m_dwGiftAddTough = pThisData->ProcGift1001(DOTYPE_addTough,data.dwTough,valueMask_CONST);

			data.dwMaxLife += EudemonGifeData.m_dwGiftAddhp;
			data.dwPhyAtt += EudemonGifeData.m_dwGiftAddphyatt;
			data.dwMagAtt += EudemonGifeData.m_dwGiftAddmagatt;
			data.dwPhyDef += EudemonGifeData.m_dwGiftAddphydef;
			data.dwMagDef += EudemonGifeData.m_dwGiftAddmagdef;
			data.dwhit += EudemonGifeData.m_dwGiftAddhit;
			data.dwDoge += EudemonGifeData.m_dwGiftAdddodge;
			data.dwCrit += EudemonGifeData.m_dwGiftAddcrihit;
			data.dwTough += EudemonGifeData.m_dwGiftAddTough;

			//goto �����buff״̬Ч��,ֻ�г�ս�ĳ���Ż���
			CMonster* pThisMonster = m_pOwner->QueryCallPet();
			if (pThisMonster && pThisMonster->GetID()==data.id)
			{
				//��"="����"+=",��Ϊ�������ص��Ǽӳ��Ժ��ֵ
				data.dwMaxLife = pThisMonster->AdjustUserAttrib(data.dwMaxLife, ADJUSTUSERATTRIB_MAXLIFE);
				data.dwPhyAtt  = pThisMonster->AdjustUserAttrib(data.dwPhyAtt, ADJUSTUSERATTRIB_PHYACK);
				data.dwMagAtt  = pThisMonster->AdjustUserAttrib(data.dwMagAtt, ADJUSTUSERATTRIB_MAGACK);
				data.dwPhyDef  = pThisMonster->AdjustUserAttrib(data.dwPhyDef, ADJUSTUSERATTRIB_PHYDEF);
				data.dwMagDef  = pThisMonster->AdjustUserAttrib(data.dwMagDef, ADJUSTUSERATTRIB_MAGDEF);
				data.dwhit	   = pThisMonster->AdjustUserAttrib(data.dwhit, ADJUSTUSERATTRIB_HITRANGE);
				data.dwDoge	   = pThisMonster->AdjustUserAttrib(data.dwDoge, ADJUSTUSERATTRIB_DOG);
				data.dwCrit    = pThisMonster->AdjustUserAttrib(data.dwCrit, ADJUSTUSERATTRIB_Crit);
				data.dwTough   = pThisMonster->AdjustUserAttrib(data.dwTough, ADJUSTUSERATTRIB_TOUGH);
			}
			else if(m_pOwner)//goto �Գ�����Ч�ı�������,����ɼ��ܵ�
			{
				data.dwPhyAtt += m_pOwner->GetPetAtkAdd();
				data.dwMagAtt += m_pOwner->GetPetMagAtkAdd();
				data.dwCrit += m_pOwner->GetPetCrihitAdd();
				data.dwMaxLife += m_pOwner->GetPetMaxLifeAdd();
			}
		}
		
		data.dwScore    = (data.dwPhyAtt + data.dwMagAtt) * 10;
		data.dwScore   += (data.dwPhyDef + data.dwMagDef) * 3.33f;
		data.dwScore   += (data.dwhit + data.dwCrit + data.dwDoge) * 33.3f;
		data.dwScore   += data.dwTough * 30;
		data.dwScore   += data.dwMaxLife * 2;


		CEudemonMagicData* pMagicData = NULL;
		pMagicData = g_pNpcMagicManager->QueryEudemonData(data.skill[0]/1000);
		if(pMagicData)
		{
			if(pMagicData->GetID() == pMagicData->GetInt(EM_EUDEMONMAGIC_basicLev))
				data.dwScore += 1000 * (data.skill[0] % 1000);
			else
				data.dwScore += 2200* (data.skill[0] % 1000);
		}

// 		pMagicData = g_pNpcMagicManager->QueryEudemonData(data.skill[1]/1000);
// 		if(pMagicData)
// 		{
// 			if(pMagicData->GetID() == pMagicData->GetInt(EM_EUDEMONMAGIC_basicLev))
// 				data.dwScore += 1000* (data.skill[1] % 1000);
// 			else
// 				data.dwScore += 2200* (data.skill[1] % 1000);
// 		}

		for(int i = 0 ; i < 9 ; i ++)
		{
			pMagicData = g_pNpcMagicManager->QueryEudemonData(data.skill[i+1]/1000);
			if(pMagicData)
			{
				if(pMagicData->GetID() == pMagicData->GetInt(EM_EUDEMONMAGIC_basicLev))
					data.dwScore += 800 * (data.skill[i+1] % 1000);
				else
					data.dwScore += 1800 * (data.skill[i+1] % 1000);
			}
		}

		data.dwScore   /= 300;
	}
	return true;
}

void CUserPet::SaveData()
{
	USERPETDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPetData* pData = *it;
		if (pData)
		{
			pData->SaveData();
		}
	}
}

int CUserPet::sendInfo(CUser* pUser)
{
	CHECKF(m_pOwner);

	int nAmount = 0;
	CMsgEudemon msg;
	if(!msg.Create(PETACTION_PetInfo,NULL,m_pOwner->GetID()))
		return 0;

	USERPETDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPetData* pData = *it;
		if (pData)
		{
			if(pData->GetInt(EUDEMONDATA_state) != PETSTATE_REST && pData->GetInt(EUDEMONDATA_state) != PETSTATE_INWAR && pData->GetInt(EUDEMONDATA_state) != PETSTATE_ADDIN)
				continue;
			EudemonData data;
			pData->GetData(data);
			fillType(data,1);
			nAmount ++;
			msg.Append(data);
		}
	}
	if(pUser)
		pUser->SendMsg(&msg);
	else
		m_pOwner->SendMsg(&msg);
	return nAmount;
}

bool CUserPet::sendStorageInfo()//���Ͳֿ��еĳ���
{
	CHECKF(m_pOwner);
	
	CMsgEudemon msg;
	BOOL bSend = FALSE;
	USERPETDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPetData* pData = *it;
		if (pData)
		{
			if(pData->GetInt(EUDEMONDATA_state) != PETSTATE_INSTORAGE)
				continue;
			if(!bSend)
			{
				EudemonData data;
				pData->GetData(data);
				fillType(data,1);
				bSend = msg.Create(PETACTION_StoragePet,&data,m_pOwner->GetID());
			}
			else
			{
				EudemonData data;
				pData->GetData(data);
				fillType(data,1);
				msg.Append(data);
			}
		}
	}
	if(bSend)
		m_pOwner->SendMsg(&msg);
	
	return true;
}
bool CUserPet::Create(PROCESS_ID idProcess,CUser *pUser,bool bLogin)
{
	CHECKF(pUser);
	m_pOwner	= pUser;
	m_idProcess = idProcess;

	if(!bLogin)
		return true;

	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE owner_id=%d LIMIT 40", _TBL_EUDEMON, pUser->GetID());
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
	if (pRes)
	{
		for (int i=0; i<pRes->RecordCount(); i++)
		{
			CUserPetData* pData = CUserPetData::CreateNew();
			if (pData && pData->Create(pRes, Database()))
			{
				if(pData->GetInt(EUDEMONDATA_state) == PETSTATE_INWAR)
					pUser->m_idEudemonCur = pData->GetInt(EUDEMONDATA_id);
				else if(pData->GetInt(EUDEMONDATA_state) == PETSTATE_ADDIN)
					pUser->m_idSoulEudemon = pData->GetInt(EUDEMONDATA_id);
				m_setData.push_back(pData);
			}
			else
			{
				SAFE_RELEASE (pData);
				SAFE_RELEASE (pRes);
				return false;
			}
			
			pRes->MoveNext();
		}
		SAFE_RELEASE (pRes);
	}
	return true;
}

int CUserPet::GetShowAmountByType(OBJID idType)
{
	int amount = 0;
	USERPETDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPetData* pData = *it;
		if (pData && (pData->GetInt(EUDEMONDATA_state) != PETSTATE_PLAY ) && (pData->GetInt(EUDEMONDATA_state) != PETSTATE_BORN ) && (pData->GetInt(EUDEMONDATA_state) != PETSTATE_INSTORAGE ))//10 ��ʾ�ֿ��еĳ���
			if(pData->GetInt(EUDEMONDATA_typeid) == idType)
				amount ++;
	}
	return amount;
}

int CUserPet::GetShowAmount()
{
	int amount = 0;
	USERPETDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPetData* pData = *it;
		if (pData && (pData->GetInt(EUDEMONDATA_state) != PETSTATE_PLAY ) && (pData->GetInt(EUDEMONDATA_state) != PETSTATE_BORN ) && (pData->GetInt(EUDEMONDATA_state) != PETSTATE_INSTORAGE ))//10 ��ʾ�ֿ��еĳ���
			amount ++;
	}
	return amount;
}

int CUserPet::GetStorageAmount()
{
	int amount = 0;
	USERPETDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPetData* pData = *it;
		if (pData && (pData->GetInt(EUDEMONDATA_state) == PETSTATE_INSTORAGE ))//10 ��ʾ�ֿ��еĳ���
			amount ++;
	}
	return amount;
}
//////////////////////////////////////////////////////////////////////
// CUserPetData* CUserPet::PopEudemon(OBJID idEudemon)
// {
// 	if (ID_NONE == idEudemon)
// 		return NULL;
// 
// 	CUserPetData* pEudemon = NULL;
// 	USERPETDATA_SET::iterator it=m_setData.begin();
// 	for (; it<m_setData.end(); )
// 	{
// 		CUserPetData* pData = *it;
// 		if (pData && pData->GetID() == idEudemon)
// 		{
// 			pEudemon = pData;
// 			m_setData.erase(it);
// 			continue;
// 		}
// 		it++;
// 	}
// 
// 	if (pEudemon)
// 	{
// 		//		SetIndexState(pItem->GetPosition(),pItem->GetPackageIndex(),0 );//06.9.21�������
// 		return pEudemon;
// 	}
// 	
// 	return NULL;
// }
//////////////////////////////////////////////////////////////////////
bool CUserPet::AddEudemon(CUserPetData* pEudemon, bool bUpdate)
{
	CHECKF(pEudemon);
	CHECKF(m_pOwner);

	if(GetShowAmount() >= 3 + m_pOwner->GetPacklev(4))
	{
		m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "���������Ѿ�������");
		return false;
	}
	
	m_setData.push_back(pEudemon);
	return true;
}

CUserPetData* CUserPet::QueryData(OBJID idData,bool bDel)
{
	USERPETDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPetData* pData = *it;
		if (pData && pData->GetID()==idData)
		{
			if(bDel)
			{
				m_setData.erase(it);
				return NULL;
			}

			return pData;
		}
	}
	return NULL;
}

bool CUserPet::DeleteData(OBJID idPet)
{
	USERPETDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPetData* pData = *it;
		if (pData && pData->GetInt(EUDEMONDATA_id) == idPet)
		{
			pData->DeleteRecord();
			SAFE_RELEASE (pData);
			m_setData.erase(it);
			return true;
		}
	}
	return false;
}

bool CUserPet::AddData(CUserPetData* pData)
{
	if(!pData)
		return false;

	m_setData.push_back(pData);
	return true;
}

bool CUserPet::CreateNewPetData(OBJID idEudemonType,EudemonData *pData,bool bSave, bool born,int nBornGrow,bool bCompensation/* = false*/)
{
	CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(idEudemonType);
	if(pEudemonType)
	{
		CUserPetData* pDataTemp = CUserPetData::CreateNew();
		if (pDataTemp)
		{
			if( pDataTemp->CreateRecord( GameDataDefault()->GetEudemonData(),pData ) )
			{
				//?		
				if(bCompensation)
				{
					pDataTemp->SetInt(EUDEMONDATA_exp, pData->exp);
					pDataTemp->SetInt(EUDEMONDATA_understand, pData->understand);
					pDataTemp->SetInt(EUDEMONDATA_gift, 0);
					pData->gift = 0;
					pDataTemp->SetInt(EUDEMONDATA_borntime, pData->borntime);
					pDataTemp->SetInt(EUDEMONDATA_addpoint, (pData->level - 1) * 5);
					pData->addpoint = (pData->level - 1) * 5;
					pDataTemp->SetInt(EUDEMONDATA_failtime, 0);
					pData->failtime = 0;
					pDataTemp->SetInt(EUDEMONDATA_bone, pData->bone);
					pDataTemp->SetInt(EUDEMONDATA_sex, pData->sex);
					pDataTemp->SetInt(EUDEMONDATA_gettype, pData->gettype);
					pDataTemp->SetInt(EUDEMONDATA_happy, 100);
					pData->happy = 100;
					pDataTemp->SetInt(EUDEMONDATA_level, pData->level);

					switch(pDataTemp->GetInt(EUDEMONDATA_gettype))
					{
					case 0:
						sprintf(pData->name,"Ұ��%s",pEudemonType->GetStr(EUDEMONTYPEDATA_name));
						break;
					case 1:
						sprintf(pData->name,"%s����",pEudemonType->GetStr(EUDEMONTYPEDATA_name));
						break;
					default:
						sprintf(pData->name,"%s(����)",pEudemonType->GetStr(EUDEMONTYPEDATA_name));
						break;
					}

					pDataTemp->SetStr(EUDEMONDATA_name, pData->name, 64);
					pDataTemp->SetInt(EUDEMONDATA_typeid, pEudemonType->GetInt(EUDEMONTYPEDATA_Id));
					pData->typeId = pDataTemp->GetInt(EUDEMONDATA_typeid);
					pDataTemp->SetInt(EUDEMONDATA_lifespan, pEudemonType->GetInt(EUDEMONTYPEDATA_maxlifespan));
					pData->lifespan = pDataTemp->GetInt(EUDEMONDATA_lifespan);

					pData->initgrow = /*pEudemonType->GetInt(EUDEMONTYPEDATA_initgrow)*/100;
					pDataTemp->SetInt(EUDEMONDATA_grow, pData->grow);

					pDataTemp->SetInt(EUDEMONDATA_habit, pData->habit);

					DWORD thisGrow = pData->initgrow + pData->grow;
					DWORD thisstr = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initstr) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randstr))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
					DWORD thisint = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initint) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randint))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
					DWORD thissta = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initsta) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randsta))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
					DWORD thisspi = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initspi) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randspi))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
					DWORD thisagi = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initagi) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randagi))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
					
					pDataTemp->SetInt(EUDEMONDATA_firstr, thisstr);
					pDataTemp->SetInt(EUDEMONDATA_firint, thisint);
					pDataTemp->SetInt(EUDEMONDATA_firsta, thissta);
					pDataTemp->SetInt(EUDEMONDATA_firspi, thisspi);
					pDataTemp->SetInt(EUDEMONDATA_firagi, thisagi);
					pData->firstr = pDataTemp->GetInt(EUDEMONDATA_firstr);
					pData->firint = pDataTemp->GetInt(EUDEMONDATA_firint);
					pData->firsta = pDataTemp->GetInt(EUDEMONDATA_firsta);
					pData->firspi = pDataTemp->GetInt(EUDEMONDATA_firspi);
					pData->firagi = pDataTemp->GetInt(EUDEMONDATA_firagi);

					
					pDataTemp->SetInt(EUDEMONDATA_strchg, pData->strchg);
					pDataTemp->SetInt(EUDEMONDATA_intchg, pData->intchg);
					pDataTemp->SetInt(EUDEMONDATA_stachg, pData->stachg);
					pDataTemp->SetInt(EUDEMONDATA_spichg, pData->spichg);
					pDataTemp->SetInt(EUDEMONDATA_agichg, pData->agichg);
			
					
					if(pData->skill[0] != 0)
						pDataTemp->SetInt(EUDEMONDATA_skill_1, pData->skill[0] / 1000 * 1000 + pData->level );
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_1, 0 );

					if(pData->skill[1] != 0)
						pDataTemp->SetInt(EUDEMONDATA_skill_2, pData->skill[1] / 1000 * 1000 + pData->level );
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_2, 0 );

					if(pData->skill[2] != 0)
						pDataTemp->SetInt(EUDEMONDATA_skill_3, pData->skill[2] / 1000 * 1000 + pData->level );
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_3, 0 );

					if(pData->skill[3] != 0)
						pDataTemp->SetInt(EUDEMONDATA_skill_4, pData->skill[3] / 1000 * 1000 + pData->level );
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_4, 0 );

					if(pData->skill[4] != 0)
						pDataTemp->SetInt(EUDEMONDATA_skill_5, pData->skill[4] / 1000 * 1000 + pData->level );
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_5, 0 );

					if(pData->skill[5] != 0)
						pDataTemp->SetInt(EUDEMONDATA_skill_6, pData->skill[5] / 1000 * 1000 + pData->level );
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_6, 0 );

					if(pData->skill[6] != 0)
						pDataTemp->SetInt(EUDEMONDATA_skill_7, pData->skill[6] / 1000 * 1000 + pData->level );
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_7, 0 );

					if(pData->skill[7] != 0)
						pDataTemp->SetInt(EUDEMONDATA_skill_8, pData->skill[7] / 1000 * 1000 + pData->level );
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_8, 0 );

					if(pData->skill[8] != 0)
						pDataTemp->SetInt(EUDEMONDATA_skill_9, pData->skill[8] / 1000 * 1000 + pData->level );
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_9, 0 );

					if(pData->skill[9] != 0)
						pDataTemp->SetInt(EUDEMONDATA_skill_10, pData->skill[9] / 1000 * 1000 + pData->level );
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_10, 0 );
					
					pData->skill[0] = pDataTemp->GetInt(EUDEMONDATA_skill_1);
					pData->skill[1] = pDataTemp->GetInt(EUDEMONDATA_skill_2);
					pData->skill[2] = pDataTemp->GetInt(EUDEMONDATA_skill_3);
					pData->skill[3] = pDataTemp->GetInt(EUDEMONDATA_skill_4);
					pData->skill[4] = pDataTemp->GetInt(EUDEMONDATA_skill_5);
					pData->skill[5] = pDataTemp->GetInt(EUDEMONDATA_skill_6);
					pData->skill[6] = pDataTemp->GetInt(EUDEMONDATA_skill_7);
					pData->skill[7] = pDataTemp->GetInt(EUDEMONDATA_skill_8);
					pData->skill[8] = pDataTemp->GetInt(EUDEMONDATA_skill_9);
					pData->skill[9] = pDataTemp->GetInt(EUDEMONDATA_skill_10);

					pData->id = pDataTemp->GetID();
					fillType(*pData,2);
					fillAtt(*pData,2);				
					pDataTemp->SetInt(EUDEMONDATA_life, pData->dwMaxLife,true);
					pData->life = pData->dwMaxLife;
					m_setData.push_back(pDataTemp);
					return true;
				}
				pDataTemp->SetInt(EUDEMONDATA_exp, 0);
				pDataTemp->SetInt(EUDEMONDATA_understand, 0);
				pDataTemp->SetInt(EUDEMONDATA_gift, 0);
				pDataTemp->SetInt(EUDEMONDATA_borntime, 0);
				pDataTemp->SetInt(EUDEMONDATA_addpoint, 0);
				pDataTemp->SetInt(EUDEMONDATA_failtime, 0);
				pDataTemp->SetInt(EUDEMONDATA_bone, RandGet(8000)%7);
				pDataTemp->SetInt(EUDEMONDATA_sex, RandGet(1000)%2);

				if(born)
					pDataTemp->SetInt(EUDEMONDATA_gettype, pData->gettype);
				else
					pDataTemp->SetInt(EUDEMONDATA_gettype, RandGet(1000)/700);

				pDataTemp->SetInt(EUDEMONDATA_happy, RandGet(31) + 70);
				//??
				char szTemp[64];
				switch(pDataTemp->GetInt(EUDEMONDATA_gettype))
				{
				case 0:
					pDataTemp->SetInt(EUDEMONDATA_level, max(1,pEudemonType->GetInt(EUDEMONTYPEDATA_getLev)));
					pDataTemp->SetInt(EUDEMONDATA_stradd, pDataTemp->GetInt(EUDEMONDATA_level) - 1);
					pDataTemp->SetInt(EUDEMONDATA_intadd, pDataTemp->GetInt(EUDEMONDATA_level) - 1);
					pDataTemp->SetInt(EUDEMONDATA_staadd, pDataTemp->GetInt(EUDEMONDATA_level) - 1);
					pDataTemp->SetInt(EUDEMONDATA_spiadd, pDataTemp->GetInt(EUDEMONDATA_level) - 1);
					pDataTemp->SetInt(EUDEMONDATA_agiadd, pDataTemp->GetInt(EUDEMONDATA_level) - 1);
					sprintf(szTemp,"Ұ��%s",pEudemonType->GetStr(EUDEMONTYPEDATA_name));
					break;
				case 1:
					pDataTemp->SetInt(EUDEMONDATA_level, 1);
					sprintf(szTemp,"%s����",pEudemonType->GetStr(EUDEMONTYPEDATA_name));
					pDataTemp->SetInt(EUDEMONDATA_stradd, 0);
					pDataTemp->SetInt(EUDEMONDATA_intadd, 0);
					pDataTemp->SetInt(EUDEMONDATA_staadd, 0);
					pDataTemp->SetInt(EUDEMONDATA_spiadd, 0);
					pDataTemp->SetInt(EUDEMONDATA_agiadd, 0);
					break;
				default:
					pDataTemp->SetInt(EUDEMONDATA_level, 1);
					sprintf(szTemp,"%s(����)",pEudemonType->GetStr(EUDEMONTYPEDATA_name));
					pDataTemp->SetInt(EUDEMONDATA_stradd, 0);
					pDataTemp->SetInt(EUDEMONDATA_intadd, 0);
					pDataTemp->SetInt(EUDEMONDATA_staadd, 0);
					pDataTemp->SetInt(EUDEMONDATA_spiadd, 0);
					pDataTemp->SetInt(EUDEMONDATA_agiadd, 0);
					break;
				}
				pDataTemp->SetStr(EUDEMONDATA_name, szTemp, 64);
				pDataTemp->SetInt(EUDEMONDATA_typeid, pEudemonType->GetInt(EUDEMONTYPEDATA_Id));
				pDataTemp->SetInt(EUDEMONDATA_lifespan, pEudemonType->GetInt(EUDEMONTYPEDATA_maxlifespan));

				int nAddData = 0;
				if(born)
					nAddData = nBornGrow;
				else
				{
					int nRand = RandGet(10000);
					
					if(nRand < 8500)
						nAddData = RandGet(10) + 1;
					else if(nRand < 9800)
						nAddData = 10 + RandGet(10) + 1;
					else if(nRand < 9950)
						nAddData = 20 + RandGet(10) + 1;
					else if(nRand < 9990)
						nAddData = 30 + RandGet(10) + 1;
					else
						nAddData = 40 + RandGet(10) + 1;
				}
				
				pData->initgrow = /*pEudemonType->GetInt(EUDEMONTYPEDATA_initgrow)*/100;
				pDataTemp->SetInt(EUDEMONDATA_grow, nAddData/*,true*/);

				int nTotal = 0;
				int nGet = RandGet(100);
				pDataTemp->SetInt(EUDEMONDATA_habit, 0);
				bool bSet = false;
				if(nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit1))
				{
					pDataTemp->SetInt(EUDEMONDATA_habit, 0);
					bSet = true;
				}
				nTotal+= pEudemonType->GetInt(EUDEMONTYPEDATA_habit1);

				if(!bSet && (nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit2) + nTotal))
				{
					pDataTemp->SetInt(EUDEMONDATA_habit, 1);
					bSet = true;
				}
				nTotal+= pEudemonType->GetInt(EUDEMONTYPEDATA_habit2);
				if(!bSet && (nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit3) + nTotal))
				{
					pDataTemp->SetInt(EUDEMONDATA_habit, 2);
					bSet = true;
				}
				nTotal+= pEudemonType->GetInt(EUDEMONTYPEDATA_habit3);
				if(!bSet && (nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit4) + nTotal))
				{
					pDataTemp->SetInt(EUDEMONDATA_habit, 3);
					bSet = true;
				}
				nTotal+= pEudemonType->GetInt(EUDEMONTYPEDATA_habit4);
				if(!bSet && (nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit5) + nTotal))
				{
					pDataTemp->SetInt(EUDEMONDATA_habit, 4);
					bSet = true;
				}

				pData->level = pDataTemp->GetInt(EUDEMONDATA_level);
				pData->exp = 0;
				pData->stradd = pDataTemp->GetInt(EUDEMONDATA_stradd);
				pData->intadd = pDataTemp->GetInt(EUDEMONDATA_intadd);
				pData->staadd = pDataTemp->GetInt(EUDEMONDATA_staadd);
				pData->spiadd = pDataTemp->GetInt(EUDEMONDATA_spiadd);
				pData->agiadd = pDataTemp->GetInt(EUDEMONDATA_agiadd);
				pData->failtime = 0;

				pData->sex = pDataTemp->GetInt(EUDEMONDATA_sex);
				pData->life = pDataTemp->GetInt(EUDEMONDATA_life);
				pData->gettype = pDataTemp->GetInt(EUDEMONDATA_gettype);
				pData->happy = pDataTemp->GetInt(EUDEMONDATA_happy);
				pData->typeId = pEudemonType->GetInt(EUDEMONTYPEDATA_Id);
				pData->lifespan = pDataTemp->GetInt(EUDEMONDATA_lifespan);
				pData->grow = pDataTemp->GetInt(EUDEMONDATA_grow);
				pData->habit = pDataTemp->GetInt(EUDEMONDATA_habit);

				pData->understand = pDataTemp->GetInt(EUDEMONDATA_understand);
				pData->gift = pDataTemp->GetInt(EUDEMONDATA_gift);
				pData->borntime = pDataTemp->GetInt(EUDEMONDATA_borntime);
				pData->addpoint = pDataTemp->GetInt(EUDEMONDATA_addpoint);
				pData->gift = pDataTemp->GetInt(EUDEMONDATA_bone);
				strcpy(pData->name,pDataTemp->GetStr(EUDEMONDATA_name));
				
				if(bSave)
				{
					pDataTemp->SetInt(EUDEMONDATA_firstr, pData->firstr);
					pDataTemp->SetInt(EUDEMONDATA_firint, pData->firint);
					pDataTemp->SetInt(EUDEMONDATA_firsta, pData->firsta);
					pDataTemp->SetInt(EUDEMONDATA_firspi, pData->firspi);
					pDataTemp->SetInt(EUDEMONDATA_firagi, pData->firagi);
					pDataTemp->SetInt(EUDEMONDATA_strchg, pData->strchg);
					pDataTemp->SetInt(EUDEMONDATA_intchg, pData->intchg);
					pDataTemp->SetInt(EUDEMONDATA_stachg, pData->stachg);
					pDataTemp->SetInt(EUDEMONDATA_spichg, pData->spichg);
					pDataTemp->SetInt(EUDEMONDATA_agichg, pData->agichg/*,true*/);

					pDataTemp->SetInt(EUDEMONDATA_skill_1, pData->skill[0]);
					pDataTemp->SetInt(EUDEMONDATA_skill_2, pData->skill[1]);
					pDataTemp->SetInt(EUDEMONDATA_skill_3, pData->skill[2]);
					pDataTemp->SetInt(EUDEMONDATA_skill_4, pData->skill[3]);
					pDataTemp->SetInt(EUDEMONDATA_skill_5, pData->skill[4]);
					pDataTemp->SetInt(EUDEMONDATA_skill_6, pData->skill[5]);
					pDataTemp->SetInt(EUDEMONDATA_skill_7, pData->skill[6]/*,true*/);
					pDataTemp->SetInt(EUDEMONDATA_skill_8, pData->skill[7]);
					pDataTemp->SetInt(EUDEMONDATA_skill_9, pData->skill[8]);
					pDataTemp->SetInt(EUDEMONDATA_skill_10, pData->skill[9]);
				}
				else
				{
					DWORD thisGrow = pData->initgrow + pData->grow;
					DWORD thisstr = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initstr) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randstr))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
					DWORD thisint = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initint) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randint))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
					DWORD thissta = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initsta) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randsta))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
					DWORD thisspi = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initspi) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randspi))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
					DWORD thisagi = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initagi) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randagi))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;

					pDataTemp->SetInt(EUDEMONDATA_firstr, thisstr);
					pDataTemp->SetInt(EUDEMONDATA_firint, thisint);
					pDataTemp->SetInt(EUDEMONDATA_firsta, thissta);
					pDataTemp->SetInt(EUDEMONDATA_firspi, thisspi);
					pDataTemp->SetInt(EUDEMONDATA_firagi, thisagi);

					//float gettypeadd = (pData->gettype + 9) / 10.0f;
					DWORD thisstrchg = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initstrChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randstrChg))/*) * gettypeadd*/;
					DWORD thisintchg = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initintChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randintChg))/*) * gettypeadd*/;
					DWORD thisstachg = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initstaChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randstaChg))/*) * gettypeadd*/;
					DWORD thisspichg = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initspiChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randspiChg))/*) * gettypeadd*/;
					DWORD thisagichg = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initagiChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randagiChg))/*) * gettypeadd*/;

					pDataTemp->SetInt(EUDEMONDATA_strchg, thisstrchg);
					pDataTemp->SetInt(EUDEMONDATA_intchg, thisintchg);
					pDataTemp->SetInt(EUDEMONDATA_stachg, thisstachg);
					pDataTemp->SetInt(EUDEMONDATA_spichg, thisspichg);
					pDataTemp->SetInt(EUDEMONDATA_agichg, thisagichg/*,true*/);
					
					if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand1))
						pDataTemp->SetInt(EUDEMONDATA_skill_1, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_1) * 1000 + pData->level);
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_1, 0);
					if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand2))
						pDataTemp->SetInt(EUDEMONDATA_skill_2, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_2) * 1000 + pData->level);
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_2, 0);
					if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand3))
						pDataTemp->SetInt(EUDEMONDATA_skill_3, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_3) * 1000 + pData->level);
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_3, 0);
					if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand4))
						pDataTemp->SetInt(EUDEMONDATA_skill_4, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_4) * 1000 + pData->level);
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_4, 0);
					if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand5))
						pDataTemp->SetInt(EUDEMONDATA_skill_5, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_5) * 1000 + pData->level);
					else
						pDataTemp->SetInt(EUDEMONDATA_skill_5, 0);
					pDataTemp->SetInt(EUDEMONDATA_skill_6, 0);
					pDataTemp->SetInt(EUDEMONDATA_skill_7, 0/*,true*/);
					pDataTemp->SetInt(EUDEMONDATA_skill_8, 0);
					pDataTemp->SetInt(EUDEMONDATA_skill_9, 0);
					pDataTemp->SetInt(EUDEMONDATA_skill_10, 0);


					pData->firstr = pDataTemp->GetInt(EUDEMONDATA_firstr);
					pData->firint = pDataTemp->GetInt(EUDEMONDATA_firint);
					pData->firsta = pDataTemp->GetInt(EUDEMONDATA_firsta);
					pData->firspi = pDataTemp->GetInt(EUDEMONDATA_firspi);
					pData->firagi = pDataTemp->GetInt(EUDEMONDATA_firagi);
					pData->strchg = pDataTemp->GetInt(EUDEMONDATA_strchg);
					pData->intchg = pDataTemp->GetInt(EUDEMONDATA_intchg);
					pData->stachg = pDataTemp->GetInt(EUDEMONDATA_stachg);
					pData->spichg = pDataTemp->GetInt(EUDEMONDATA_spichg);
					pData->agichg = pDataTemp->GetInt(EUDEMONDATA_agichg);
					
					pData->skill[0] = pDataTemp->GetInt(EUDEMONDATA_skill_1);
					pData->skill[1] = pDataTemp->GetInt(EUDEMONDATA_skill_2);
					pData->skill[2] = pDataTemp->GetInt(EUDEMONDATA_skill_3);
					pData->skill[3] = pDataTemp->GetInt(EUDEMONDATA_skill_4);
					pData->skill[4] = pDataTemp->GetInt(EUDEMONDATA_skill_5);
					pData->skill[5] = pDataTemp->GetInt(EUDEMONDATA_skill_6);
					pData->skill[6] = pDataTemp->GetInt(EUDEMONDATA_skill_7);
					pData->skill[7] = pDataTemp->GetInt(EUDEMONDATA_skill_8);
					pData->skill[8] = pDataTemp->GetInt(EUDEMONDATA_skill_9);
					pData->skill[9] = pDataTemp->GetInt(EUDEMONDATA_skill_10);
				}
				
				pData->id = pDataTemp->GetID();
				fillType(*pData,2);
				fillAtt(*pData,2);				
				pDataTemp->SetInt(EUDEMONDATA_life, pData->dwMaxLife,true);
				pData->life = pData->dwMaxLife;
				m_setData.push_back(pDataTemp);
				return true;
			}
			else
			{
				SAFE_RELEASE (pDataTemp);
			}
		}
	}
	return false;
}


CUserPeifang::CUserPeifang()
{
	
}

CUserPeifang::~CUserPeifang()
{
	USERPEIFANGDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPeifangData* pData = *it;
		SAFE_RELEASE (pData);
	}
	
}

CUserPeifangData* CUserPeifang::QueryData(OBJID idData,bool bDel)
{
	USERPEIFANGDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPeifangData* pData = *it;
		if (pData && pData->GetInt(PEIFANGDATA_peifangid)==idData)
		{
			if(bDel)
			{
				m_setData.erase(it);
				return NULL;
			}
			
			return pData;
		}
	}
	return NULL;
}

bool CUserPeifang::AddData(CUserPeifangData* pData)
{
	if(!pData)
		return false;
	
	m_setData.push_back(pData);
	return true;
}

int CUserPeifang::sendInfo(CUser* pUser)
{
	CHECKF(m_pOwner);
	
	int nSendAmount = 0;
	CMsgCompound msg;
	if(!msg.Create(MSGCOMPOUND_SENDPEIFANG,ID_NONE))
		return 0;
	
	USERPEIFANGDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPeifangData* pData = *it;
		if (pData)
		{
			nSendAmount ++;
			msg.AppendPeifang(pData->GetInt(PEIFANGDATA_peifangid));
			if(nSendAmount > 30)
			{
				m_pOwner->SendMsg(&msg);
				if(!msg.Create(MSGCOMPOUND_SENDPEIFANG,ID_NONE))
					return 0;
				nSendAmount = 0;
			}
		}
	}
	
	m_pOwner->SendMsg(&msg);
	return 1;
}

bool CUserPeifang::DeleteData(OBJID idPeifang)
{
	USERPEIFANGDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPeifangData* pData = *it;
		if (pData &&  pData->GetInt(PEIFANGDATA_peifangid)==idPeifang)
		{
			pData->DeleteRecord();
			SAFE_RELEASE (pData);
			m_setData.erase(it);
			return true;
		}
	}
	return false;
}

bool CUserPeifang::CreateNewPeifangData(OBJID idUser,OBJID idPeifangType)
{
	CPeifangTypeData*	pPeifangType = PeifangType()->GetObj(idPeifangType);
	if(pPeifangType)
	{
		CUserPeifangData* pDataTemp = CUserPeifangData::CreateNew();
		if (pDataTemp)
		{
			if( pDataTemp->CreateRecord( GameDataDefault()->GetPeifangData(),idUser,idPeifangType ) )
			{
				m_setData.push_back(pDataTemp);
				return true;
			}
			else
			{
				SAFE_RELEASE (pDataTemp);
			}
		}
	}
	return false;
}

bool CUserPeifang::Create(PROCESS_ID idProcess,CUser *pUser,bool bLogin)
{
	CHECKF(pUser);
	m_pOwner	= pUser;
	m_idProcess = idProcess;

	if(!bLogin)
		return true;

	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE userid=%d", _TBL_PEIFANG, pUser->GetID());
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
	if (pRes)
	{
		for (int i=0; i<pRes->RecordCount(); i++)
		{
			CUserPeifangData* pData = CUserPeifangData::CreateNew();
			if (pData && pData->Create(pRes, Database()))
			{
				m_setData.push_back(pData);
			}
			else
			{
				SAFE_RELEASE (pData);
				SAFE_RELEASE (pRes);
				return false;
			}
			
			pRes->MoveNext();
		}
		SAFE_RELEASE (pRes);
	}
	return true;
}

void  CUserPeifang::SendAllObjInfo(PROCESS_ID idProcess)
{
	CHECK(m_pOwner);
	USERPEIFANGDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPeifangData* pData = *it;
		if (pData)
		{
			ST_PEIFANGDATA info;
			if(pData->GetInfo(info))
				m_pOwner->SendObjInfo(idProcess,m_pOwner->GetID(),INFO_PEIFANG,&info,sizeof(info));
		}
	}
}

bool CUserPeifang::AddPeifangBySend(ST_PEIFANGDATA info)
{
	if(QueryData(info.idPeifang))
		return false;

	CUserPeifangData* pDataTemp = CUserPeifangData::CreateNew();
	CHECKF (pDataTemp);

	IF_NOT(pDataTemp->Create( GameDataDefault()->GetPeifangData(), info.id))
		return false;

	pDataTemp->SetInt(PEIFANGDATA_id,info.id);
	pDataTemp->SetInt(PEIFANGDATA_owner_id,info.owner_id);
	pDataTemp->SetInt(PEIFANGDATA_peifangid,info.idPeifang);
	pDataTemp->ClearUpdateFlags();
	m_setData.push_back(pDataTemp);

	return true;
}


CUserPetData* CUserPet::CreateNewPetByEgg(OBJID idEudemonType,EudemonData *pData)
{
	CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(idEudemonType);
	CHECKF(pEudemonType);

	CUserPetData* pDataTemp = CUserPetData::CreateNew();
	CHECKF(pDataTemp);
	if( pDataTemp->CreateRecord( GameDataDefault()->GetEudemonData(),pData ) )
	{
		//?				
		pDataTemp->SetInt(EUDEMONDATA_exp, 0);
		pDataTemp->SetInt(EUDEMONDATA_understand, 0);
		pDataTemp->SetInt(EUDEMONDATA_gift, 0);
		pDataTemp->SetInt(EUDEMONDATA_borntime, 0);
		pDataTemp->SetInt(EUDEMONDATA_addpoint, 0);
		pDataTemp->SetInt(EUDEMONDATA_failtime, 0);
		pDataTemp->SetInt(EUDEMONDATA_bone, RandGet(8000)%7);
		pDataTemp->SetInt(EUDEMONDATA_sex, RandGet(1000)%2);

		pDataTemp->SetInt(EUDEMONDATA_gettype, pData->gettype);

		pDataTemp->SetInt(EUDEMONDATA_happy, RandGet(31) + 70);
		//??
		char szTemp[64];
		pDataTemp->SetInt(EUDEMONDATA_level, 1);
		sprintf(szTemp,"%s(����)",pEudemonType->GetStr(EUDEMONTYPEDATA_name));
		pDataTemp->SetInt(EUDEMONDATA_stradd, 0);
		pDataTemp->SetInt(EUDEMONDATA_intadd, 0);
		pDataTemp->SetInt(EUDEMONDATA_staadd, 0);
		pDataTemp->SetInt(EUDEMONDATA_spiadd, 0);
		pDataTemp->SetInt(EUDEMONDATA_agiadd, 0);


		pDataTemp->SetStr(EUDEMONDATA_name, szTemp, 64);
		pDataTemp->SetInt(EUDEMONDATA_typeid, pEudemonType->GetInt(EUDEMONTYPEDATA_Id));
		pDataTemp->SetInt(EUDEMONDATA_lifespan, pEudemonType->GetInt(EUDEMONTYPEDATA_maxlifespan));
		
		pData->initgrow = /*pEudemonType->GetInt(EUDEMONTYPEDATA_initgrow)*/100;
		pDataTemp->SetInt(EUDEMONDATA_grow, pData->grow);

		int nTotal = 0;
		int nGet = RandGet(100);
		pDataTemp->SetInt(EUDEMONDATA_habit, 0);
		bool bSet = false;
		if(nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit1))
		{
			pDataTemp->SetInt(EUDEMONDATA_habit, 0);
			bSet = true;
		}
		nTotal+= pEudemonType->GetInt(EUDEMONTYPEDATA_habit1);

		if(!bSet && (nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit2) + nTotal))
		{
			pDataTemp->SetInt(EUDEMONDATA_habit, 1);
			bSet = true;
		}
		nTotal+= pEudemonType->GetInt(EUDEMONTYPEDATA_habit2);
		if(!bSet && (nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit3) + nTotal))
		{
			pDataTemp->SetInt(EUDEMONDATA_habit, 2);
			bSet = true;
		}
		nTotal+= pEudemonType->GetInt(EUDEMONTYPEDATA_habit3);
		if(!bSet && (nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit4) + nTotal))
		{
			pDataTemp->SetInt(EUDEMONDATA_habit, 3);
			bSet = true;
		}
		nTotal+= pEudemonType->GetInt(EUDEMONTYPEDATA_habit4);
		if(!bSet && (nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit5) + nTotal))
		{
			pDataTemp->SetInt(EUDEMONDATA_habit, 4);
			bSet = true;
		}

		pData->level = pDataTemp->GetInt(EUDEMONDATA_level);
		pData->exp = 0;
		pData->stradd = pDataTemp->GetInt(EUDEMONDATA_stradd);
		pData->intadd = pDataTemp->GetInt(EUDEMONDATA_intadd);
		pData->staadd = pDataTemp->GetInt(EUDEMONDATA_staadd);
		pData->spiadd = pDataTemp->GetInt(EUDEMONDATA_spiadd);
		pData->agiadd = pDataTemp->GetInt(EUDEMONDATA_agiadd);
		pData->failtime = 0;

		pData->sex = pDataTemp->GetInt(EUDEMONDATA_sex);
		pData->life = pDataTemp->GetInt(EUDEMONDATA_life);
		pData->gettype = pDataTemp->GetInt(EUDEMONDATA_gettype);
		pData->happy = pDataTemp->GetInt(EUDEMONDATA_happy);
		pData->typeId = pEudemonType->GetInt(EUDEMONTYPEDATA_Id);
		pData->lifespan = pDataTemp->GetInt(EUDEMONDATA_lifespan);
		pData->grow = pDataTemp->GetInt(EUDEMONDATA_grow);
		pData->habit = pDataTemp->GetInt(EUDEMONDATA_habit);

		pData->understand = pDataTemp->GetInt(EUDEMONDATA_understand);
		pData->gift = pDataTemp->GetInt(EUDEMONDATA_gift);
		pData->borntime = pDataTemp->GetInt(EUDEMONDATA_borntime);
		pData->addpoint = pDataTemp->GetInt(EUDEMONDATA_addpoint);
		pData->gift = pDataTemp->GetInt(EUDEMONDATA_bone);
		strcpy(pData->name,pDataTemp->GetStr(EUDEMONDATA_name));
		
		//DWORD thisGrow = pData->initgrow + pData->grow;
		//��ʼ����ֵ
		DWORD thisstr = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initstr) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randstr))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
		DWORD thisint = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initint) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randint))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
		DWORD thissta = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initsta) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randsta))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
		DWORD thisspi = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initspi) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randspi))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
		DWORD thisagi = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initagi) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randagi))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;

		pDataTemp->SetInt(EUDEMONDATA_firstr, thisstr);
		pDataTemp->SetInt(EUDEMONDATA_firint, thisint);
		pDataTemp->SetInt(EUDEMONDATA_firsta, thissta);
		pDataTemp->SetInt(EUDEMONDATA_firspi, thisspi);
		pDataTemp->SetInt(EUDEMONDATA_firagi, thisagi);

		//float gettypeadd = (pData->gettype + 9) / 10.0f;
		//����
// 		DWORD thisstrchg = pEudemonType->GetInt(EUDEMONTYPEDATA_initstrChg) + pData->strchg;
// 		DWORD thisintchg = pEudemonType->GetInt(EUDEMONTYPEDATA_initintChg) + pData->intchg;
// 		DWORD thisstachg = pEudemonType->GetInt(EUDEMONTYPEDATA_initstaChg) + pData->stachg;
// 		DWORD thisspichg = pEudemonType->GetInt(EUDEMONTYPEDATA_initspiChg) + pData->spichg;
// 		DWORD thisagichg = pEudemonType->GetInt(EUDEMONTYPEDATA_initagiChg) + pData->agichg;

		pDataTemp->SetInt(EUDEMONDATA_strchg, pData->strchg);
		pDataTemp->SetInt(EUDEMONDATA_intchg, pData->intchg);
		pDataTemp->SetInt(EUDEMONDATA_stachg, pData->stachg);
		pDataTemp->SetInt(EUDEMONDATA_spichg, pData->spichg);
		pDataTemp->SetInt(EUDEMONDATA_agichg, pData->agichg/*,true*/);
		
		//����
		if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand1))
			pDataTemp->SetInt(EUDEMONDATA_skill_1, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_1) * 1000 + pData->level);
		else
			pDataTemp->SetInt(EUDEMONDATA_skill_1, 0);
		if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand2))
			pDataTemp->SetInt(EUDEMONDATA_skill_2, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_2) * 1000 + pData->level);
		else
			pDataTemp->SetInt(EUDEMONDATA_skill_2, 0);
		if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand3))
			pDataTemp->SetInt(EUDEMONDATA_skill_3, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_3) * 1000 + pData->level);
		else
			pDataTemp->SetInt(EUDEMONDATA_skill_3, 0);
		if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand4))
			pDataTemp->SetInt(EUDEMONDATA_skill_4, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_4) * 1000 + pData->level);
		else
			pDataTemp->SetInt(EUDEMONDATA_skill_4, 0);
		if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand5))
			pDataTemp->SetInt(EUDEMONDATA_skill_5, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_5) * 1000 + pData->level);
		else
			pDataTemp->SetInt(EUDEMONDATA_skill_5, 0);
		pDataTemp->SetInt(EUDEMONDATA_skill_6, 0);
		pDataTemp->SetInt(EUDEMONDATA_skill_7, 0/*,true*/);
		pDataTemp->SetInt(EUDEMONDATA_skill_8, 0);
		pDataTemp->SetInt(EUDEMONDATA_skill_9, 0);
		pDataTemp->SetInt(EUDEMONDATA_skill_10, 0);

		//�������
		pData->firstr = pDataTemp->GetInt(EUDEMONDATA_firstr);
		pData->firint = pDataTemp->GetInt(EUDEMONDATA_firint);
		pData->firsta = pDataTemp->GetInt(EUDEMONDATA_firsta);
		pData->firspi = pDataTemp->GetInt(EUDEMONDATA_firspi);
		pData->firagi = pDataTemp->GetInt(EUDEMONDATA_firagi);
		pData->strchg = pDataTemp->GetInt(EUDEMONDATA_strchg);
		pData->intchg = pDataTemp->GetInt(EUDEMONDATA_intchg);
		pData->stachg = pDataTemp->GetInt(EUDEMONDATA_stachg);
		pData->spichg = pDataTemp->GetInt(EUDEMONDATA_spichg);
		pData->agichg = pDataTemp->GetInt(EUDEMONDATA_agichg);
		
		pData->skill[0] = pDataTemp->GetInt(EUDEMONDATA_skill_1);
		pData->skill[1] = pDataTemp->GetInt(EUDEMONDATA_skill_2);
		pData->skill[2] = pDataTemp->GetInt(EUDEMONDATA_skill_3);
		pData->skill[3] = pDataTemp->GetInt(EUDEMONDATA_skill_4);
		pData->skill[4] = pDataTemp->GetInt(EUDEMONDATA_skill_5);
		pData->skill[5] = pDataTemp->GetInt(EUDEMONDATA_skill_6);
		pData->skill[6] = pDataTemp->GetInt(EUDEMONDATA_skill_7);
		pData->skill[7] = pDataTemp->GetInt(EUDEMONDATA_skill_8);
		pData->skill[8] = pDataTemp->GetInt(EUDEMONDATA_skill_9);
		pData->skill[9] = pDataTemp->GetInt(EUDEMONDATA_skill_10);
		
		pData->id = pDataTemp->GetID();
		fillType(*pData,2);
		fillAtt(*pData,2);
		pDataTemp->SetInt(EUDEMONDATA_life, pData->dwMaxLife,true);
		pData->life = pData->dwMaxLife;
		m_setData.push_back(pDataTemp);
		return pDataTemp;
	}
	else
	{
		SAFE_RELEASE(pDataTemp);
	}

	return NULL;
}

void  CUserPet::SendAllObjInfo(PROCESS_ID idProcess)
{
	CHECK(m_pOwner);
	USERPETDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CUserPetData* pData = *it;
		if (pData)
		{
			EudemonData info;
			if(pData->GetData(info))
				m_pOwner->SendObjInfo(idProcess,m_pOwner->GetID(),INFO_EUDEMON,&info,sizeof(info));
		}
	}
}

bool CUserPet::AddPeTBySend(EudemonData info)
{
	if(QueryData(info.id))
		return false;
	
	CUserPetData* pDataTemp = CUserPetData::CreateNew();
	CHECKF (pDataTemp);
	
	IF_NOT(pDataTemp->Create( GameDataDefault()->GetEudemonData(), info.id))
		return false;
	
	pDataTemp->SetData(info,false);
	if(pDataTemp->GetInt(EUDEMONDATA_state) == PETSTATE_INWAR)
		m_pOwner->m_idEudemonCur = pDataTemp->GetInt(EUDEMONDATA_id);
	else if(pDataTemp->GetInt(EUDEMONDATA_state) == PETSTATE_ADDIN)
		m_pOwner->m_idSoulEudemon = pDataTemp->GetInt(EUDEMONDATA_id);
	m_setData.push_back(pDataTemp);

	return true;
}

bool CUserPet::EudemonTransform(OBJID idEudemon)
{
	CHECKF(m_pOwner);
	CHECKF(idEudemon);

	CUserPetData* pData = QueryData(idEudemon);
	if(!pData)
		return false;
	
	if(pData->isLocked())
		return false;
	
	if(m_pOwner->GetCallPetID() == idEudemon || pData->IsSoulState())
	{
		m_pOwner->SendSysMsg("��ս����ĳ��ﲻ�ܽ��д˲���");
		return false;
	}

	CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(pData->GetInt(EUDEMONDATA_typeid));
	if(!pEudemonType)
		return false;

	OBJID idNeedItem = 630033;//�û�����
	DWORD nNeedMoney = 2000000;//200��

	if(pData->GetInt(EUDEMONDATA_sex) >= 2)
	{
		m_pOwner->SendSysMsg("���޲����ٴλû�");
		return false;
	}

	if(pData->GetInt(EUDEMONDATA_understand) < 5)
	{
		m_pOwner->SendSysMsg("���ĳ�������ֵ����5���޷��û�");
		return false;
	}

	if(pData->GetInt(EUDEMONDATA_lifespan) <= 0)
	{
		m_pOwner->SendSysMsg("����Ϊ0�ĳ��ﲻ�ܽ��лû�");
		return false;
	}
	//�ж�����
	if (m_pOwner->GetMoney()<nNeedMoney)
	{
		m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return false;
	}
	CItem* pItem = m_pOwner->GetItemByType(idNeedItem);
	if(!pItem)
	{
		ItemInfoStruct info;
		CHECKF(ItemType()->GetInfo(idNeedItem, &info));
		m_pOwner->SendSysMsg("%s���㣬�벹��",info.szName);
		return false;
	}
	
	if(!m_pOwner->SpendMoney(nNeedMoney,true))
	{
		return false;
	}
	if(!m_pOwner->SpendItem(pItem))
	{
		return false;
	}
	
	
	pData->SetInt(EUDEMONDATA_sex,2,false);//�Ա�дΪ�û�
	pData->SetInt(EUDEMONDATA_borntime,0,false);//��ֳ��дΪ0
	m_pOwner->SendSysMsg("�û��ɹ�");

	CMsgEudemonAttrib msg;
	if(msg.Create(idEudemon, EUDATTR_SEX, pData->GetInt(EUDEMONDATA_sex)))
	{
		msg.Append(EUDATTR_BORN, pData->GetInt(EUDEMONDATA_borntime));
		m_pOwner->SendMsg(&msg);
	}
// 	EudemonData Data;
// 	m_pOwner->LoadEudemon(idEudemon,Data,2);
// 	CMsgEudemon msg;
// 	if(msg.Create(PETACTION_PetInfo,&Data,m_pOwner->GetID()))
// 	{
// 		m_pOwner->SendMsg(&msg);
// 	}

	char szMsg[256] = "";
	char szTemp[256] = "";
	char UserName[32] = "";
	char EudName[64] = "";
	sprintf(UserName, "%d_[%s]_%d", SNAPSHOTUSER, m_pOwner->GetName(), COLORYELLOW);
	sprintf(EudName, "%d_[%s]_%d_%d_%d_%d_%d", 
		SNAPSHOTUSEREDU, pData->GetStr(EUDEMONDATA_name), idEudemon, pData->GetInt(EUDEMONDATA_typeid), m_pOwner->GetID(), 0, COLORPURPLE);
	sprintf(szMsg, "<%s>�������ڲ�и�ؾ�������������ʹ�Լ���<%s>�ɱ�Ϊ���ޣ������������ۣ�", UserName, EudName);
	sprintf(szTemp, "%d_%d_%s_%d_%d_%d", 0, idEudemon, pData->GetStr(EUDEMONDATA_name), m_pOwner->GetID(), 0, 3);
	UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, m_pOwner, 0, 0, szTemp , ALLUSERS_NAME);
	
	if ((ItemType()->GetMonoly(idNeedItem)  & COSTLY_ITEM) != 0)//goto ������Ʒ��¼
	{
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", m_pOwner->GetID(), m_pOwner->GetName(), idNeedItem);
	}

	return true;
}

bool CUserPet::EudemonIntelUp(OBJID idEudemon)
{
	CHECKF(m_pOwner);
	CHECKF(idEudemon);

	CUserPetData* pData = QueryData(idEudemon);
	if(!pData)
		return false;
	
	if(pData->isLocked())
		return false;
	
	if(pData->IsSoulState())
		return false;

	CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(pData->GetInt(EUDEMONDATA_typeid));
	if(!pEudemonType)
		return false;
	int nSex = pData->GetInt(EUDEMONDATA_sex);
	if(nSex != 2)
	{
		m_pOwner->SendSysMsg("�û�������������");
		return false;
	}
	
	int nUnder = pData->GetInt(EUDEMONDATA_understand);
	int nIntel = pData->GetInt(EUDEMONDATA_intelligence);
	if(nIntel>=nUnder)
	{
		m_pOwner->SendSysMsg("���Բ��ܸ�������");
		return false;
	}

	OBJID idNeedItem = 630034;//��Ϭ��
	DWORD nNeedMoney = 20000;//2��
	//�ж�����
	if (m_pOwner->GetMoney()<nNeedMoney)
	{
		m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return false;
	}
	CItem* pItem = m_pOwner->GetItemByType(idNeedItem);
	if(!pItem)
	{
		ItemInfoStruct info;
		CHECKF(ItemType()->GetInfo(idNeedItem, &info));
		m_pOwner->SendSysMsg("%s���㣬�벹��",info.szName);
		return false;
	}
	int chance = 0;
	
	switch(nIntel)
	{
		case 0:
			chance = 10000;
			break;
		case 1:
			chance = 6000;
			break;
		case 2:
			chance = 3600;
			break;
		case 3:
			chance = 2000;
			break;
		case 4:
			chance = 1200;
			break;
		case 5:
			chance = 800;
			break;
		case 6:
			chance = 500;
			break;
		case 7:
			chance = 300;
			break;
		case 8:
			chance = 200;
			break;
		case 9:
			chance = 100;
			break;
	}

	if(!m_pOwner->SpendMoney(nNeedMoney,true))
	{
		return false;
	}
	if(!m_pOwner->SpendItem(pItem))
	{
		return false;
	}
	
	if(RandGet(10000) < chance)
	{
		m_pOwner->SendSysMsg("�����ɹ�");
		nIntel++;
		pData->SetInt(EUDEMONDATA_intelligence,nIntel,false);
		EudemonData Data;
		m_pOwner->LoadEudemon(idEudemon,Data,2);
		CMsgEudemon msg;
		if(msg.Create(PETACTION_PetInfo,&Data,m_pOwner->GetID()))
		{
			m_pOwner->SendMsg(&msg);
		}
		if(Data.intelligence >= 5)
		{
			char szMsg[256] = "";
			char szTemp[256] = "";
			char UserName[32] = "";
			char EudName[64] = "";
			sprintf(UserName, "%d_[%s]_%d", SNAPSHOTUSER, m_pOwner->GetName(), COLORYELLOW);
			sprintf(EudName, "%d_[%s]_%d_%d_%d_%d_%d", 
				SNAPSHOTUSEREDU, Data.name, idEudemon, Data.typeId, m_pOwner->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "<%s>Ǳ�����У����ڽ�<%s>������������<3_%d_6>����������ͻ�Ƶ��µľ��磡", UserName, EudName, Data.intelligence);
			sprintf(szTemp, "%d_%d_%s_%d_%d_%d", 0, idEudemon, Data.name, m_pOwner->GetID(), 0, 3);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, m_pOwner, 0, 0, szTemp , ALLUSERS_NAME);
		}
	}		
	else
	{
		m_pOwner->SendSysMsg("����ʧ��");
	}
	if ((ItemType()->GetMonoly(idNeedItem)  & COSTLY_ITEM) != 0)//goto ������Ʒ��¼
	{
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", m_pOwner->GetID(), m_pOwner->GetName(), idNeedItem);
	}

	return true;
}

bool CUserPet::EudemonFusionUp(OBJID idEudemon)//����Ĭ��
{
	CHECKF(m_pOwner);
	CHECKF(idEudemon);

	CUserPetData* pData = QueryData(idEudemon);
	if(!pData)
		return false;
	
	if(pData->isLocked())
		return false;
	
	if(pData->IsSoulState())
		return false;

	CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(pData->GetInt(EUDEMONDATA_typeid));
	if(!pEudemonType)
		return false;		
	
	if(pData->GetInt(EUDEMONDATA_sex) != 2)
	{
		return false;
	}

	int nFusion = pData->GetInt(EUDEMONDATA_fusion);

	if(nFusion >= pData->GetInt(EUDEMONDATA_intelligence))
	{
		m_pOwner->SendSysMsg("Ĭ���Ȳ��ܸ�������ֵ���޷���������");
		return false;
	}
		
	OBJID idNeedItem = 630035;//����һ��	
	DWORD nNeedMoney = 0;
	
	CItem* pItem = m_pOwner->GetItemByType(idNeedItem);
	if(!pItem)
	{
		ItemInfoStruct info;
		CHECKF(ItemType()->GetInfo(idNeedItem, &info));
		m_pOwner->SendSysMsg("%s���㣬�벹��",info.szName);
		return false;
	}

	int failto=nFusion,chance=0;
	
	switch(nFusion)
	{
	case 0:
		chance = 9000;
		nNeedMoney=20000;
		break;
	case 1:
		chance = 7500;
		nNeedMoney=20000;
		break;
	case 2:
		chance = 6000;
		nNeedMoney=40000;
		failto = 1;
		break;
	case 3:
		chance = 4500;
		nNeedMoney=40000;
		failto = 1;
		break;
	case 4:
		chance = 3000;
		nNeedMoney=60000;
		break;
	case 5:
		chance = 3000;
		nNeedMoney=60000;
		failto = 4;
		break;
	case 6:
		chance = 3000;
		nNeedMoney=80000;
		failto = 4;
		break;
	case 7:
		chance = 1500;
		nNeedMoney=80000;
		break;
	case 8:
		chance = 1000;
		nNeedMoney=100000;
		failto = 7;
		break;
	case 9:
		chance = 2000;
		nNeedMoney=100000;
		break;
	default:
		::LogSave("CUser::EudemonFusionUp Invalid fusion");
		return false;
	}
	//�ж�����
	if (m_pOwner->GetMoney()<nNeedMoney)
	{
		m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return false;
	}
	if(!m_pOwner->SpendMoney(nNeedMoney,true))
	{
		return false;
	}	
	if(!m_pOwner->SpendItem(pItem))
		return false;
	
	if(RandGet(10000) < chance)
	{
		m_pOwner->SendSysMsg("�����ɹ�");
		nFusion ++;
		pData->SetInt(EUDEMONDATA_fusion,nFusion,false);
		EudemonData Data = {0};
		m_pOwner->LoadEudemon(idEudemon,Data,2);
		CMsgEudemon msg;
		if(msg.Create(PETACTION_PetInfo,&Data,m_pOwner->GetID()))
			m_pOwner->SendMsg(&msg);
		
		if(Data.fusion>= 5)
		{
			char szMsg[256] = "";
			char szTemp[256] = "";
			char UserName[32] = "";
			char EudName[64] = "";
			sprintf(UserName, "%d_[%s]_%d", SNAPSHOTUSER, m_pOwner->GetName(), COLORYELLOW);
			sprintf(EudName, "%d_[%s]_%d_%d_%d_%d_%d", 
				SNAPSHOTUSEREDU, Data.name, idEudemon, Data.typeId, m_pOwner->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "<%s>��<%s>�������Ĭ��ֵ���˵���������<3_%d_6>���պ��ս����Ͻ�������죡", UserName, EudName, Data.fusion);
			sprintf(szTemp, "%d_%d_%s_%d_%d_%d", 0, idEudemon, Data.name, m_pOwner->GetID(), 0, 3);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, m_pOwner, 0, 0, szTemp , ALLUSERS_NAME);
		}
	}
	else
	{
		m_pOwner->SendSysMsg("����ʧ��");
		if(nFusion != failto)
		{
			nFusion = failto;
			pData->SetInt(EUDEMONDATA_fusion,nFusion,false);
			
			EudemonData Data;
			m_pOwner->LoadEudemon(idEudemon,Data,2);
			CMsgEudemon msg;
			if(msg.Create(PETACTION_PetInfo,&Data,m_pOwner->GetID()))
				m_pOwner->SendMsg(&msg);
		}
	}
	if ((ItemType()->GetMonoly(idNeedItem)  & COSTLY_ITEM) != 0)//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", m_pOwner->GetID(), m_pOwner->GetName(), idNeedItem);
	return true;
}

bool CUserPet::EudemonChangeSex(OBJID idEudemon)//�������
{
	CHECKF(m_pOwner);
	CHECKF(idEudemon);
	
	CUserPetData* pData = QueryData(idEudemon);
	if(!pData)
		return false;
	
	if(pData->isLocked())
		return false;
	
	if(m_pOwner->GetCallPetID() == idEudemon)
	{
		m_pOwner->SendSysMsg("��ս�ĳ��ﲻ�ܽ��д˲���");
		return false;
	}
	
	int nNowSex = pData->GetInt(EUDEMONDATA_sex);
	if(nNowSex >= 2)
	{
		m_pOwner->SendSysMsg("���޲��ܽ��д˲���");
		return false;
	}

	if(m_pOwner->GetSoulEudemon() == idEudemon)
	{
		m_pOwner->SendSysMsg("����ĳ��ﲻ�ܽ��д˲���");
		return false;
	}

	CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(pData->GetInt(EUDEMONDATA_typeid));
	if(!pEudemonType)
		return false;
	
	OBJID idNeedItem = 630037;//ת�Ե�
	DWORD nNeedMoney = 100000;//10��
	
	//�ж�����
	if (m_pOwner->GetMoney()<nNeedMoney)
	{
		m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return false;
	}
	CItem* pItem = m_pOwner->GetItemByType(idNeedItem);
	if(!pItem)
	{
		ItemInfoStruct info;
		CHECKF(ItemType()->GetInfo(idNeedItem, &info));
		m_pOwner->SendSysMsg("%s���㣬�벹��",info.szName);
		return false;
	}
	
	if(!m_pOwner->SpendMoney(nNeedMoney,true))
	{
		return false;
	}
	if(!m_pOwner->SpendItem(pItem))
	{
		return false;
	}
	
	int nNewSex = (nNowSex == 0 ? 1 : 0);
	pData->SetInt(EUDEMONDATA_sex, nNewSex, false);//�Ա�дΪ�û�

	m_pOwner->SendSysMsg("���Գɹ�");
	
	CMsgEudemonAttrib msg;
	if(msg.Create(idEudemon, EUDATTR_SEX, pData->GetInt(EUDEMONDATA_sex)))
	{
		m_pOwner->SendMsg(&msg);
	}
	
	if ((ItemType()->GetMonoly(idNeedItem)  & COSTLY_ITEM) != 0)//goto ������Ʒ��¼
	{
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", m_pOwner->GetID(), m_pOwner->GetName(), idNeedItem);
	}
	
	return true;
}

bool CUserPet::LearnSkill(OBJID idEudemon, OBJID idItem, int nIgnoreSkill)
{
	CHECKF(m_pOwner);
	CHECKF(idEudemon);
	CHECKF(idItem);

	int nNeedMoney = 10000;

	if(m_pOwner->GetMoney() < nNeedMoney)
	{
		m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return false;
	}
	CItem* pSkillBook = m_pOwner->GetItem(idItem,false);
	CHECKF(pSkillBook);

	if(pSkillBook->GetTypeID() / 1000 != 640)
		return false;
	
	ItemInfoStruct typeInfo = {0};
	if(!ItemType()->GetInfo(pSkillBook->GetTypeID() ,&typeInfo))
		return false;
	
	int nBookSkill = typeInfo.BasicAddID[0];
	CHECKF(nBookSkill);

	CUserPetData* pPetData = QueryData(idEudemon);
	if(!pPetData)
		return false;

	EudemonData petData = {0};
	m_pOwner->LoadEudemon(idEudemon,petData,1);

	CEudemonMagicData* pLearnMagicData = g_pNpcMagicManager->QueryEudemonData(nBookSkill);
	if(!pLearnMagicData)
		return false;
	
	int nHasSkillAmount = 0;
	int nEmptyIndex = -1;
	int nIgnoreIndex = -1;
	int nSucRate = 0;//�ɹ���
	int nHasIgnoreIndex = -1;
	CEudemonMagicData* pThisMagicData = NULL;

	if(pLearnMagicData->GetInt(EM_EUDEMONMAGIC_type) / 100 == 1)//ѧ��������,�ɹ���100%
	{
		if(petData.skill[0] != 0)
		{
			if(petData.skill[0] / 1000 != nIgnoreSkill)
			{
				m_pOwner->SendSysMsg("�����Ѿ�ӵ�����Ƽ���");
				return false;
			}
			else if(nIgnoreSkill != 0 && pLearnMagicData->GetInt(EM_EUDEMONMAGIC_nextLev) == nIgnoreSkill)//�и߼��ľͲ�����ѧ�ͼ��ģ�����Ҳ����
			{
				m_pOwner->SendSysMsg("�����Ѿ�ӵ�д˸߼�����");
				return false;
			}
			else
			{
				nIgnoreIndex = 0;
				nSucRate = 10000;
			}
		}
		else
		{
			nEmptyIndex = 0;
			nSucRate = 10000;
		}
	}
	else//��������
	{
		for(int i = 1 ; i < 10 ; i ++)//������1����������
		{
			if(i>0 && petData.skill[i] == 0 && nEmptyIndex == -1)//��������������ĵ�һ����λ
			{
				nEmptyIndex = i;
				continue;
			}
			
			if(i>0 && petData.skill[i] != 0)//ӵ�еı�����������
				nHasSkillAmount++;
			
			if(nIgnoreIndex != -1)//�Ѿ��ҵ�Ҫ���ǵ�λ����,ֻ��õ�ӵ�м�������
				continue;
			
			pThisMagicData = g_pNpcMagicManager->QueryEudemonData(petData.skill[i] / 1000);
			if(!pThisMagicData)
				continue;
			
			if(pThisMagicData->GetInt(EM_EUDEMONMAGIC_basicLev) == nBookSkill)//�и߼��ľͲ�����ѧ�ͼ��ģ�����Ҳ����
			{
				m_pOwner->SendSysMsg("�����Ѿ�ӵ�д˸߼�����");
				return false;
			}
			else if(pThisMagicData->GetInt(EM_EUDEMONMAGIC_nextLev) == nBookSkill)//�еͼ�����ѧ�߼���ֻ�ܸ���
			{
				if(nIgnoreSkill == 0)
				{
					m_pOwner->SendSysMsg("�����Ѿ�ӵ�����Ƽ���");
					return false;
				}
				else if(nIgnoreSkill == petData.skill[i] / 1000)//ָ�����ǵͼ���
				{
					nIgnoreIndex = i;
					nSucRate = 10000;//�߼����ǵͼ����ɹ���100%
				}
				else
				{
					m_pOwner->SendSysMsg("�����Ѿ�ӵ�����Ƽ���");
					return false;
				}
			}
			else if(pThisMagicData->GetInt(EM_EUDEMONMAGIC_type) == pLearnMagicData->GetInt(EM_EUDEMONMAGIC_type))//ӵ���໥��ͻ��,�ɹ��ʸ��ݼ��ܸ�����
			{
				if(nIgnoreSkill == 0 || nIgnoreSkill != petData.skill[i]/1000)//û��ָ������
				{
					m_pOwner->SendSysMsg("�����Ѿ�ӵ�����Ƽ���");
					return false;
				}
				else
				{
					nIgnoreIndex = i;
				}
			}
			else if(pThisMagicData->GetInt(EM_EUDEMONMAGIC_id) == nIgnoreSkill)
				nHasIgnoreIndex = i;
		}
		if(nIgnoreSkill != 0 && nIgnoreIndex == -1 && nHasIgnoreIndex != -1)//û�г�ͻ�ļ��ܣ�������Ȼѡ���˸���
		{
			nIgnoreIndex = nHasIgnoreIndex;
		}
	}
	if(nHasSkillAmount==9 && nIgnoreSkill==0)//�������������븲��һ��
	{
		return false;
	}
	if(nIgnoreIndex==-1 && nEmptyIndex==-1)//û���ҵ�ѧϰ���ܵ�λ��
	{
		::LogSave("CUserPet::LearnSkill: can not found a place for skill! petid=%u, learnskill=%d, ignoreskill=%d", idEudemon, nBookSkill, nIgnoreSkill);
		return false;
	}
	//��ɹ���
	if(nSucRate == 0)
	{
		int nRateParam =nHasSkillAmount;
		//�߻�Ҫ��ĳɸ���100%�ɹ�
// 		if(nIgnoreIndex != -1 && nHasSkillAmount > 0)//������ѡ����Ǹ��Ǽ��ܣ���������ѧ���ܣ��ɹ���Ҫ�����м��ܼ�1��
// 			nRateParam--;
		switch(nRateParam)
		{
		case 0:
			nSucRate = 10000;
			break;
		case 1:
			nSucRate = 7000;
			break;
		case 2:
			nSucRate = 6000;
			break;
		case 3:
			nSucRate = 5000;
			break;
		case 4:
			nSucRate = 4000;
			break;
		case 5:
			nSucRate = 3000;
			break;
		case 6:
			nSucRate = 2000;
			break;
		case 7:
			nSucRate = 1000;
			break;
		case 8:
			nSucRate = 500;
			break;
		case 9://�Ѿ���9���������ܣ�һ���Ǹ��ǵ�
			break;
		default:
			return false;
		}
	}

	if(!m_pOwner->SpendMoney(nNeedMoney,true))
		return false;
	int nItemType = pSkillBook->GetTypeID();
	BOOL bIsCostItem = pSkillBook->IsCostlyItem();
	if(!m_pOwner->SpendItem(pSkillBook))
		return false;

	if(nIgnoreIndex != -1)//���ǣ�100%
	{
		pPetData->SetSkill(nIgnoreIndex, nBookSkill * 1000 + pPetData->GetInt(EUDEMONDATA_level), true);
		
		if(nIgnoreSkill != 0)
		{
			CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonData(nIgnoreSkill);
			if(!pMagicData)
				return false;
			
			CMsgAction msgUnlean;
			if(msgUnlean.Create(idEudemon,0,0,0,actionEudSkillUnlean,pMagicData->GetInt(EM_EUDEMONMAGIC_dataID)))
				m_pOwner->SendMsg(&msgUnlean);
		}
		
		if(nBookSkill != 0)
		{
			CEudemonMagicData *pMagicData = g_pNpcMagicManager->QueryEudemonData(nBookSkill);
			if(!pMagicData)
				return false;
			
			CMsgAction msgLean;
			if(msgLean.Create(idEudemon,0,0,0,actionEudemonLean,pMagicData->GetInt(EM_EUDEMONMAGIC_dataID)))
				m_pOwner->SendMsg(&msgLean);
		}
	}
	else if(nEmptyIndex != -1)//ѧϰ�¼��ܣ��л���ʧ��
	{
		if(RandGet(10000) < nSucRate)
		{
			pPetData->SetSkill(nEmptyIndex, nBookSkill * 1000 + pPetData->GetInt(EUDEMONDATA_level), true);
			
			if(nBookSkill != 0)
			{
				CEudemonMagicData *pMagicData = g_pNpcMagicManager->QueryEudemonData(nBookSkill);
				if(!pMagicData)
					return false;
				
				CMsgAction msgLean;
				if(msgLean.Create(idEudemon,0,0,0,actionEudemonLean,pMagicData->GetInt(EM_EUDEMONMAGIC_dataID)))
					m_pOwner->SendMsg(&msgLean);
			}
		}
		else
			m_pOwner->SendSysMsg("ѧϰ����ʧ��");
	}

	if (bIsCostItem)//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", m_pOwner->GetID(), m_pOwner->GetName(), nItemType);

	return true;
}

bool CUserPet::UplevSkill(OBJID idEudemon, int nUplevSkill)
{
	CHECKF(m_pOwner);
	CHECKF(idEudemon);
	CHECKF(nUplevSkill);

	int nNeedMoney = 10000;
	if(m_pOwner->GetMoney() < nNeedMoney)
	{
		m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return false;
	}

	CUserPetData* pPetData = QueryData(idEudemon);
	if(!pPetData)
		return false;
	
	EudemonData petData = {0};
	m_pOwner->LoadEudemon(idEudemon,petData,1);

	int nUpSkillIndex = -1;
	CEudemonMagicData* pThisMagicData = NULL;
	int nSkillNextLev = 0;
	OBJID idNeedItem = ID_NONE;
	for(int i = 0 ; i < 10 ; i ++)
	{
		if(petData.skill[i] / 1000 == nUplevSkill)
		{
			nUpSkillIndex = i;

			pThisMagicData = g_pNpcMagicManager->QueryEudemonData(petData.skill[i] / 1000);
			if(!pThisMagicData)
				return false;
			
			nSkillNextLev = pThisMagicData->GetInt(EM_EUDEMONMAGIC_nextLev);
			if(nSkillNextLev == 0)
			{
				m_pOwner->SendSysMsg("�ü����Ѿ��ﵽ��ߵȼ�");
				return false;
			}

			idNeedItem = pThisMagicData->GetInt(EM_EUDEMONMAGIC_uplevitem);
			break;
		}
	}

	if(nUpSkillIndex == -1)
	{
		m_pOwner->SendSysMsg("�ó��ﻹû��ѧ���������");
			return false;
	}

	if(nSkillNextLev == 0)
		return false;

	CHECKF(idNeedItem);
	CItem* pItem = m_pOwner->GetItemByType(idNeedItem);
	if(!pItem)
	{
		ItemInfoStruct info;
		CHECKF(ItemType()->GetInfo(idNeedItem, &info));
		m_pOwner->SendSysMsg("%s���㣬�벹��",info.szName);
		
		return false;
	}

	if(!m_pOwner->SpendMoney(nNeedMoney,true))
		return false;
	int nItemType = pItem->GetTypeID();
	BOOL bIsCostItem = pItem->IsCostlyItem();
	if(!m_pOwner->SpendItem(pItem))
		return false;
	
	pPetData->SetSkill(nUpSkillIndex, nSkillNextLev * 1000 + pPetData->GetInt(EUDEMONDATA_level), true);
	
	if(nSkillNextLev != 0)
	{
		CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonData(nSkillNextLev);
		if(!pMagicData)
			return false;
		
		CMsgAction msgUplev;
		if(msgUplev.Create(idEudemon,0,0,0,actionEudemonKillUp,pMagicData->GetInt(EM_EUDEMONMAGIC_dataID)))
			m_pOwner->SendMsg(&msgUplev);
	}

	if (bIsCostItem)//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", m_pOwner->GetID(), m_pOwner->GetName(), nItemType);

	return true;
}

//������ˣ
bool CUserPet::EudemonPlay(OBJID idEudemon1,OBJID idEudemon2,CUser* pOther)
{
	CHECKF(m_pOwner);
	CHECKF(pOther);
	CHECKF(idEudemon1);
	CHECKF(idEudemon2);

	if(!m_pOwner->CanEudemonPlay())
	{
		m_pOwner->SendSysMsg("���г���������ˣ�У�������ȡ��������ˣ");
		return false;
	}

	DWORD nMoney = 20000;

	if(m_pOwner->GetMoney() < nMoney)
	{
		m_pOwner->SendSysMsg("��û���㹻������");
		pOther->SendSysMsg("�Է�û���㹻������");
		return false;
	}

	if(pOther->GetMoney() < nMoney)
	{
		pOther->SendSysMsg("��û���㹻������");
		m_pOwner->SendSysMsg("�Է�û���㹻������");
		return false;
	}

	EudemonData data1;
	if(!m_pOwner->LoadEudemon(idEudemon1,data1,1))
		return false;
	EudemonData data2;
	if(!pOther->LoadEudemon(idEudemon2,data2,1))
		return false;

	if(idEudemon1 == m_pOwner->m_idSoulEudemon)
		return false;
	if(idEudemon2 == pOther->GetSoulEudemon())
		return false;

	if(data1.gettype != 1 || data2.gettype != 1)
	{
		m_pOwner->SendSysMsg("ֻ�б���������ˣ");
		pOther->SendSysMsg("ֻ�б���������ˣ");
		return false;
	}

	if(data1.level < (50 + 20 * data1.playtimes) )
		return false;
	if(data2.level < (50 + 20 * data2.playtimes) )
		return false;

	if(data1.playtimes >= 3 || data2.playtimes >= 3)
		return false;

	if(data1.dwState != PETSTATE_REST || data2.dwState != PETSTATE_REST)
		return false;
	
	if(!m_pOwner->SpendMoney(nMoney,true))
		return false;

	if(!pOther->SpendMoney(nMoney,true))
		return false;

 	m_pOwner->EudemonSetPlay(idEudemon1, data1.playtimes + 1);

	pOther->EudemonSetPlay(idEudemon2, data2.playtimes + 1);

	DEBUG_TRY
	m_pOwner->UpdateMissionMsgByItemType(data1.typeId);
	pOther->UpdateMissionMsgByItemType(data2.typeId);
	DEBUG_CATCH("pUser->UpdateMissionMsgByItemType eudemon")

	return true;
}
