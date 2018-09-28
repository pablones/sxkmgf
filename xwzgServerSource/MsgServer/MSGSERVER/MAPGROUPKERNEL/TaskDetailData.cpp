// TaskDetailData.cpp: implementation of the CTaskDetailData class.
//
//////////////////////////////////////////////////////////////////////

#include "TaskDetailData.h"
#include "GiftTempleteManager.h"
#include "NpcMagicManager.h"
#include "GiftManager.cpp"
#include "UserPet.h"
//#include "Magic.h"

char	szTaskDetailTable[]=_TBL_TASKDETAIL;

MYHEAP_IMPLEMENTATION(CTaskDetailData, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskDetailData::CTaskDetailData()
{
	m_pData = NULL;
	m_bNeedUpdate = false;
}

CTaskDetailData::~CTaskDetailData()
{
	SAFE_RELEASE (m_pData);
}

bool CTaskDetailData::Create(IRecordset *pRes, IDatabase *pDb)
{
	CHECKF (!m_pData);
	CHECKF (pRes);
	
	m_pData	= CGameTaskDetailData::CreateNew();
	IF_NOT (m_pData)
		return false;
	IF_NOT (m_pData->Create(pRes))
		return false;

	return true;
}

bool CTaskDetailData::CreateRecord(ST_TASKDETAILDATA *pInfo, IDatabase *pDb)
{
	CHECKF (!m_pData);
	CHECKF (pInfo);
	CHECKF (pDb);

	m_pData = CGameTaskDetailData::CreateNew();
	IF_NOT (m_pData)
		return false;

	IF_NOT (m_pData->Create(pDb, ID_NONE))
		return false;
	
// 	m_pData->SetInt(TASKDETAILDATA_USERID, pInfo->idUser);
// 	m_pData->SetInt(TASKDETAILDATA_TASKID, pInfo->idTask);
// 	m_pData->SetInt(TASKDETAILDATA_TASKPHASE, pInfo->nTaskPhase);
// 	m_pData->SetInt(TASKDETAILDATA_TASKCOMPLETENUM, pInfo->nCompleteNum);
// 	m_pData->SetInt(TASKDETAILDATA_TASKBEGINTIME, TimeGet(TIME_SECOND));
// 
// 	m_pData->SetInt(TASKDETAILDATA_TASKTYPE, pInfo->nType);
// 	m_pData->SetInt(TASKDETAILDATA_TASKCHAIN, pInfo->nTaskChain);
// 	m_pData->SetInt(TASKDETAILDATA_REQUIREMONSTER, pInfo->Require_Monster);
// 	m_pData->SetInt(TASKDETAILDATA_REQUIREITEM, pInfo->Require_Item);
// 	m_pData->SetInt(TASKDETAILDATA_REQUIRENUM, pInfo->Require_Num);
// 	m_pData->SetInt(TASKDETAILDATA_RECEIVENPC, pInfo->Receive_NPC);
// 	m_pData->SetInt(TASKDETAILDATA_COMPLETENPC, pInfo->Complete_NPC);
// 	m_pData->SetInt(TASKDETAILDATA_LIFETIME, pInfo->leftTime);
// 	m_pData->SetInt(TASKDETAILDATA_INDEXTASK,pInfo->indexTask);
	if (ID_NONE == m_pData->InsertRecord())
		return false;	
	return true;
}
//06.11.21
//王玉波
//增加新函数,调用CreateRecord(ST_TASKDETAILDATA *pInfo, IDatabase *pDb)关闭服务器时会出错
bool CTaskDetailData::CreateRecord( IRecord* pDefault, const ST_TASKDETAILDATA* pInfo ,bool bInset/* = true*/)
{
	CHECKF( !m_pData );
	CHECKF( pInfo );
	CHECKF( pDefault );

	m_pData = CGameTaskDetailData::CreateNew();
	IF_NOT ( m_pData )
		return false;

	if(bInset)
	{
		IF_NOT ( m_pData->Create( pDefault, ID_NONE ) )
			return false;
	}
	else
	{
		IF_NOT ( m_pData->Create( pDefault, pInfo->id ) )
			return false;
	}
	
	m_pData->SetInt( TASKDETAILDATA_USERID, pInfo->idUser );
	m_pData->SetInt( TASKDETAILDATA_TASKID, pInfo->idTask );
	m_pData->SetInt( TASKDETAILDATA_TASKPHASE, pInfo->nTaskPhase );
	m_pData->SetInt( TASKDETAILDATA_TASKBEGINTIME, pInfo->dwBegainTime );
	m_pData->SetInt( TASKDETAILDATA_DATA1, pInfo->data1 );
	m_pData->SetInt( TASKDETAILDATA_DATA2, pInfo->data2 );
	m_pData->SetInt( TASKDETAILDATA_DATA3, pInfo->data3 );
	m_pData->SetInt( TASKDETAILDATA_DATA4, pInfo->data4 );
	m_pData->SetInt( TASKDETAILDATA_DATA5, pInfo->data5 );
	m_pData->SetInt( TASKDETAILDATA_DATA6, pInfo->data6 );
	m_pData->SetInt( TASKDETAILDATA_ISREC, pInfo->isRec );
	m_pData->SetInt( TASKDETAILDATA_RANDSTEP, pInfo->randStep );
	m_pData->SetInt( TASKDETAILDATA_SPENDENETYPE, pInfo->spendEneType );

	if(bInset)
	{
		if ( ID_NONE == m_pData->InsertRecord() )
			return false;
	}
	else
	{
		m_pData->ClearUpdateFlags();
	}
	m_bNeedUpdate = false;
	return true;
}

void CTaskDetailData::SetInt(TASKDETAILDATA idx, int nData, bool bUpdate/* = false*/)	
{ 
	if(nData == m_pData->GetInt(idx))
		return;

	m_pData->SetInt(idx, nData); 
	if (bUpdate) 
		UpdateData();
	else
		m_bNeedUpdate = true;
}

void CTaskDetailData::SetStr(TASKDETAILDATA idx, LPCTSTR szData, int nSize, bool bUpdate/* = false*/)	
{ 
	if(strcmp(m_pData->GetStr(idx),szData) == 0)
		return;

	m_pData->SetStr(idx, szData, nSize); 
	if (bUpdate) 
		UpdateData(); 
	else
		m_bNeedUpdate = true;
}

void  CTaskDetailData::UpdateData()			
{ 
	if(m_bNeedUpdate)
	{
		m_pData->Update();
		m_bNeedUpdate = false;
	}
}

bool  CTaskDetailData::GetInfo(ST_TASKDETAILDATA& info)
{
	CHECKF (m_pData);
	info.id = m_pData->GetInt(TASKDETAILDATA_ID );
	info.idUser = m_pData->GetInt(TASKDETAILDATA_USERID );
	info.idTask = m_pData->GetInt(TASKDETAILDATA_TASKID );
	info.nTaskPhase = m_pData->GetInt(TASKDETAILDATA_TASKPHASE );
	info.dwBegainTime = m_pData->GetInt(TASKDETAILDATA_TASKBEGINTIME );
	info.data1 = m_pData->GetInt(TASKDETAILDATA_DATA1 );
	info.data2 = m_pData->GetInt(TASKDETAILDATA_DATA2 );
	info.data3 = m_pData->GetInt(TASKDETAILDATA_DATA3 );
	info.data4 = m_pData->GetInt(TASKDETAILDATA_DATA4 );
	info.data5 = m_pData->GetInt(TASKDETAILDATA_DATA5 );
	info.data6 = m_pData->GetInt(TASKDETAILDATA_DATA6 );
	info.isRec = m_pData->GetInt(TASKDETAILDATA_ISREC );
	info.randStep = m_pData->GetInt(TASKDETAILDATA_RANDSTEP );
	info.spendEneType = m_pData->GetInt(TASKDETAILDATA_SPENDENETYPE );
	return true;
}


MYHEAP_IMPLEMENTATION(CUserPetData, s_heap)
CUserPetData::CUserPetData()
{
	m_pData = NULL;
	m_bLock = false;
}

CUserPetData::~CUserPetData()
{
	SAFE_RELEASE (m_pData);
}

bool CUserPetData::Create(IRecord* pDefaultRes, OBJID id)  
{ 
	CHECKF (!m_pData);
	m_pData	= CEudemonData::CreateNew();
	IF_NOT (m_pData)
		return false;
	return m_pData->Create(pDefaultRes,id); 
}	

bool CUserPetData::Create(IRecordset *pRes, IDatabase *pDb)
{
	CHECKF (!m_pData);
	CHECKF (pRes);
	
	m_pData	= CEudemonData::CreateNew();
	IF_NOT (m_pData)
		return false;
	IF_NOT (m_pData->Create(pRes))
		return false;
	
	return true;
}

// bool CUserPetData::CreateRecord(ST_TASKDETAILDATA *pInfo, IDatabase *pDb)
// {
// 	CHECKF (!m_pData);
// 	CHECKF (pInfo);
// 	CHECKF (pDb);
// 	
// 	m_pData = CGameTaskDetailData::CreateNew();
// 	IF_NOT (m_pData)
// 		return false;
// 	
// 	IF_NOT (m_pData->Create(pDb, ID_NONE))
// 		return false;
// 	if (ID_NONE == m_pData->InsertRecord())
// 		return false;	
// 	return true;
// }
// bool CTaskDetailData::CreateRecord( IRecord* pDefault, const ST_TASKDETAILDATA* pInfo )
// {
// 	CHECKF( !m_pData );
// 	CHECKF( pInfo );
// 	CHECKF( pDefault );
// 	
// 	m_pData = CGameTaskDetailData::CreateNew();
// 	IF_NOT ( m_pData )
// 		return false;
// 	
// 	IF_NOT ( m_pData->Create( pDefault, ID_NONE ) )
// 		return false;
// 	
// 	m_pData->SetInt( TASKDETAILDATA_USERID, pInfo->idUser );
// 	m_pData->SetInt( TASKDETAILDATA_TASKID, pInfo->idTask );
// 	m_pData->SetInt( TASKDETAILDATA_TASKPHASE, pInfo->nTaskPhase );
// 	m_pData->SetInt( TASKDETAILDATA_TASKBEGINTIME, pInfo->dwBegainTime );
// 	m_pData->SetInt( TASKDETAILDATA_DATA1, pInfo->data1 );
// 	m_pData->SetInt( TASKDETAILDATA_DATA2, pInfo->data2 );
// 	m_pData->SetInt( TASKDETAILDATA_DATA3, pInfo->data3 );
// 	m_pData->SetInt( TASKDETAILDATA_DATA4, pInfo->data4 );
// 	m_pData->SetInt( TASKDETAILDATA_DATA5, pInfo->data5 );
// 	m_pData->SetInt( TASKDETAILDATA_DATA6, pInfo->data6 );
// 	m_pData->SetInt( TASKDETAILDATA_ISREC, pInfo->isRec );
// 	
// 	if ( ID_NONE == m_pData->InsertRecord() )
// 		return false;	
// 	return true;
// }
void CUserPetData::SetInt(EUDEMONDATA idx, int nData, bool bUpdate)
{
	if(GetInt(idx) == nData)
		return;

	m_pData->SetInt(idx, nData);
	if(bUpdate)
		m_pData->Update();
}

bool CUserPetData::GetData(EudemonData& data)
{
	CHECKF(m_pData);
	data.dwState  = m_pData->GetInt( EUDEMONDATA_state);
// 	if(!bForce && data.dwState == 3)
// 		return false;

	data.id = m_pData->GetInt( EUDEMONDATA_id);
	data.ownerId = m_pData->GetInt( EUDEMONDATA_owner_id);
	data.typeId = m_pData->GetInt( EUDEMONDATA_typeid);
	data.playtimes = m_pData->GetInt( EUDEMONDATA_playtime);
	data.life = m_pData->GetInt( EUDEMONDATA_life);
	data.level = m_pData->GetInt( EUDEMONDATA_level);
	data.exp = m_pData->GetInt( EUDEMONDATA_exp);
	data.sex = m_pData->GetInt( EUDEMONDATA_sex);
	data.gettype = m_pData->GetInt( EUDEMONDATA_gettype);
	data.happy = m_pData->GetInt( EUDEMONDATA_happy);
	data.understand = m_pData->GetInt( EUDEMONDATA_understand);
	data.habit = m_pData->GetInt( EUDEMONDATA_habit);
	data.lifespan = m_pData->GetInt( EUDEMONDATA_lifespan);
	data.gift = m_pData->GetInt( EUDEMONDATA_bone);
	data.borntime = m_pData->GetInt( EUDEMONDATA_borntime);
	data.grow = m_pData->GetInt( EUDEMONDATA_grow);
	data.addpoint = m_pData->GetInt( EUDEMONDATA_addpoint);
	data.firstr = m_pData->GetInt( EUDEMONDATA_firstr);
	data.stradd = m_pData->GetInt( EUDEMONDATA_stradd);
	data.strchg = m_pData->GetInt( EUDEMONDATA_strchg);
	data.firint = m_pData->GetInt( EUDEMONDATA_firint);
	data.intadd = m_pData->GetInt( EUDEMONDATA_intadd);
	data.intchg = m_pData->GetInt( EUDEMONDATA_intchg);
	data.firsta = m_pData->GetInt( EUDEMONDATA_firsta);
	data.staadd = m_pData->GetInt( EUDEMONDATA_staadd);
	data.stachg = m_pData->GetInt( EUDEMONDATA_stachg);
	data.firspi = m_pData->GetInt( EUDEMONDATA_firspi);
	data.spiadd = m_pData->GetInt( EUDEMONDATA_spiadd);
	data.spichg = m_pData->GetInt( EUDEMONDATA_spichg);
	data.firagi = m_pData->GetInt( EUDEMONDATA_firagi);
	data.agiadd = m_pData->GetInt( EUDEMONDATA_agiadd);
	data.agichg = m_pData->GetInt( EUDEMONDATA_agichg);
	data.skill[0] = m_pData->GetInt( EUDEMONDATA_skill_1);
	data.skill[1] = m_pData->GetInt( EUDEMONDATA_skill_2);
	data.skill[2] = m_pData->GetInt( EUDEMONDATA_skill_3);
	data.skill[3] = m_pData->GetInt( EUDEMONDATA_skill_4);
	data.skill[4] = m_pData->GetInt( EUDEMONDATA_skill_5);
	data.skill[5] = m_pData->GetInt( EUDEMONDATA_skill_6);
	data.skill[6] = m_pData->GetInt( EUDEMONDATA_skill_7);
	data.skill[7] = m_pData->GetInt( EUDEMONDATA_skill_8);
	data.skill[8] = m_pData->GetInt( EUDEMONDATA_skill_9);
	data.skill[9] = m_pData->GetInt( EUDEMONDATA_skill_10);
	data.failtime = m_pData->GetInt( EUDEMONDATA_failtime);
	strcpy(data.name,m_pData->GetStr(EUDEMONDATA_name));
	data.fusion = m_pData->GetInt(EUDEMONDATA_fusion);
	data.intelligence = m_pData->GetInt(EUDEMONDATA_intelligence);

	return true;
}

bool CUserPetData::SetData(EudemonData& data,bool bInset)
{
	CHECKF(m_pData);
	m_pData->SetInt( EUDEMONDATA_state,data.dwState);
	m_pData->SetInt( EUDEMONDATA_id,data.id);
	m_pData->SetInt( EUDEMONDATA_owner_id,data.ownerId);
	m_pData->SetInt( EUDEMONDATA_typeid,data.typeId);
	m_pData->SetInt( EUDEMONDATA_playtime,data.playtimes);
	m_pData->SetInt( EUDEMONDATA_life,data.life);
	m_pData->SetInt( EUDEMONDATA_level,data.level);
	m_pData->SetInt( EUDEMONDATA_exp,data.exp);
	m_pData->SetInt( EUDEMONDATA_sex,data.sex);
	m_pData->SetInt( EUDEMONDATA_gettype,data.gettype);
	m_pData->SetInt( EUDEMONDATA_happy,data.happy);
	m_pData->SetInt( EUDEMONDATA_understand,data.understand);
	m_pData->SetInt( EUDEMONDATA_habit,data.habit);
	m_pData->SetInt( EUDEMONDATA_lifespan,data.lifespan);
	m_pData->SetInt( EUDEMONDATA_bone,data.gift);
	m_pData->SetInt( EUDEMONDATA_borntime,data.borntime);
	m_pData->SetInt( EUDEMONDATA_grow,data.grow);
	m_pData->SetInt( EUDEMONDATA_addpoint,data.addpoint);
	m_pData->SetInt( EUDEMONDATA_firstr,data.firstr);
	m_pData->SetInt( EUDEMONDATA_stradd,data.stradd);
	m_pData->SetInt( EUDEMONDATA_strchg,data.strchg);
	m_pData->SetInt( EUDEMONDATA_firint,data.firint);
	m_pData->SetInt( EUDEMONDATA_intadd,data.intadd);
	m_pData->SetInt( EUDEMONDATA_intchg,data.intchg);
	m_pData->SetInt( EUDEMONDATA_firsta,data.firsta);
	m_pData->SetInt( EUDEMONDATA_staadd,data.staadd);
	m_pData->SetInt( EUDEMONDATA_stachg,data.stachg);
	m_pData->SetInt( EUDEMONDATA_firspi,data.firspi);
	m_pData->SetInt( EUDEMONDATA_spiadd,data.spiadd);
	m_pData->SetInt( EUDEMONDATA_spichg,data.spichg);
	m_pData->SetInt( EUDEMONDATA_firagi,data.firagi);
	m_pData->SetInt( EUDEMONDATA_agiadd,data.agiadd);
	m_pData->SetInt( EUDEMONDATA_agichg,data.agichg);
	m_pData->SetInt( EUDEMONDATA_skill_1,data.skill[0]);
	m_pData->SetInt( EUDEMONDATA_skill_2,data.skill[1]);
	m_pData->SetInt( EUDEMONDATA_skill_3,data.skill[2]);
	m_pData->SetInt( EUDEMONDATA_skill_4,data.skill[3]);
	m_pData->SetInt( EUDEMONDATA_skill_5,data.skill[4]);
	m_pData->SetInt( EUDEMONDATA_skill_6,data.skill[5]);
	m_pData->SetInt( EUDEMONDATA_skill_7,data.skill[6]);
	m_pData->SetInt( EUDEMONDATA_skill_8,data.skill[7]);
	m_pData->SetInt( EUDEMONDATA_skill_9,data.skill[8]);
	m_pData->SetInt( EUDEMONDATA_skill_10,data.skill[9]);
	m_pData->SetInt( EUDEMONDATA_failtime,data.failtime);
	m_pData->SetStr(EUDEMONDATA_name,data.name,64);
	m_pData->SetInt(EUDEMONDATA_intelligence,data.intelligence);
	m_pData->SetInt(EUDEMONDATA_fusion,data.fusion);
	if(!bInset)
		m_pData->ClearUpdateFlags();
	return true;
}

// bool CUserPetData::CreateRecord(EudemonData * pInfo,IDatabase * pDb)
// {
// 	CHECKF (!m_pData);
// 	CHECKF (pInfo);
// 	CHECKF (pDb);
// 	
// 	m_pData = CEudemonData::CreateNew();
// 	IF_NOT (m_pData)
// 		return false;
// 	
// 	IF_NOT (m_pData->Create(pDb, ID_NONE))
// 		return false;
// 
// 	if (ID_NONE == m_pData->InsertRecord())
// 		return false;	
// 	return true;
// 
// }
int	 CUserPetData::ProcGift1001(int nType, int oldValue,int valueMask /*= valueMask_ALL*/)
{
	int  nAdd = 0;
	CHECKF(m_pData);
	if(nType > DOTYPE_petAddMaxLife || nType < DOTYPE_addhp)
		return 0;
	
	int skill[9];
	skill[0] = m_pData->GetInt( EUDEMONDATA_skill_2);
	skill[1] = m_pData->GetInt( EUDEMONDATA_skill_3);
	skill[2] = m_pData->GetInt( EUDEMONDATA_skill_4);
	skill[3] = m_pData->GetInt( EUDEMONDATA_skill_5);
	skill[4] = m_pData->GetInt( EUDEMONDATA_skill_6);
	skill[5] = m_pData->GetInt( EUDEMONDATA_skill_7);
	skill[6] = m_pData->GetInt( EUDEMONDATA_skill_8);
	skill[7] = m_pData->GetInt( EUDEMONDATA_skill_9);
	skill[8] = m_pData->GetInt( EUDEMONDATA_skill_10);

	for(int i = 0 ; i < 9 ; i ++)
	{
		if(skill[i] / 1000 == 0)
			continue;

		CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonData(skill[i] / 1000);
		if(!pMagicData)
			continue;

		int skillType = pMagicData->GetInt(EM_EUDEMONMAGIC_dataID);
		int skilllev = skill[i] % 1000;
		if(skillType == 0)
			continue;
		
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
		
		//触发条件判断
		int doValue = pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1001_dodata) + pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1001_dodatainc) *  (skilllev - 1) / 100;
		if(doValue >= 31000)
			doValue = 31000;
		
		if(doValue >= 30000)
		{
			nAdd += oldValue * ((doValue - 30000) / 100.0f);
		}
		else
		{
			nAdd += doValue;
		}
	}
	
	return nAdd;
}

bool CUserPetData::CreateRecord( IRecord* pDefault, const EudemonData* pInfo )
{
	CHECKF( !m_pData );
	CHECKF( pInfo );
	CHECKF( pDefault );

// 	CEudemonData* pDataTemp = CEudemonData::CreateNew();
// 	IF_OK(pDataTemp)
// 	{
// 		IF_OK(pDataTemp->Create(GameDataDefault()->GetEudemonData(), ID_NONE))
// 			{

	
	m_pData = CEudemonData::CreateNew();
	IF_NOT ( m_pData )
		return false;
	
	IF_NOT ( m_pData->Create( pDefault, ID_NONE ) )
		return false;

	
	m_pData->SetInt(EUDEMONDATA_owner_id, pInfo->ownerId);
	//m_pData->SetInt(EUDEMONDATA_itemid, pInfo->itemid);
	if ( ID_NONE == m_pData->InsertRecord() )
		return false;	
	return true;
}

MYHEAP_IMPLEMENTATION(CUserPeifangData, s_heap)
CUserPeifangData::CUserPeifangData()
{
	m_pData = NULL;
}

CUserPeifangData::~CUserPeifangData()
{
	SAFE_RELEASE (m_pData);
}

bool CUserPeifangData::CreateRecord( IRecord* pDefault, OBJID idUser , OBJID idPeifang )
{
	CHECKF( !m_pData );
	CHECKF( pDefault );
	
	m_pData = CPeifangData::CreateNew();
	IF_NOT ( m_pData )
		return false;
	
	IF_NOT ( m_pData->Create( pDefault, ID_NONE ) )
		return false;
	
	
	m_pData->SetInt(PEIFANGDATA_owner_id, idUser);
	m_pData->SetInt(PEIFANGDATA_peifangid, idPeifang);
	if ( ID_NONE == m_pData->InsertRecord() )
		return false;	
	return true;
}

bool CUserPeifangData::Create(IRecordset *pRes, IDatabase *pDb)
{
	CHECKF (!m_pData);
	CHECKF (pRes);
	
	m_pData	= CPeifangData::CreateNew();
	IF_NOT (m_pData)
		return false;
	IF_NOT (m_pData->Create(pRes))
		return false;
	
	return true;
}

bool CUserPeifangData::Create(IRecord* pDefaultRes, OBJID id)  
{ 
	CHECKF (!m_pData);
	CHECKF (pDefaultRes);
	
	m_pData	= CPeifangData::CreateNew();
	IF_NOT (m_pData)
		return false;

	return m_pData->Create(pDefaultRes,id); 
	return true;
}

bool  CUserPeifangData::GetInfo(ST_PEIFANGDATA& info)
{
	CHECKF (m_pData);
	info.id = GetInt(PEIFANGDATA_id);
	info.owner_id = GetInt(PEIFANGDATA_owner_id);
	info.idPeifang = GetInt(PEIFANGDATA_peifangid);
	return true;
}
bool CUserPetData::IsSoulState()
{
	return GetInt(EUDEMONDATA_state) == PETSTATE_ADDIN;
}

bool CUserPetData::SetSkill(int nIndex, int nData, bool bUpdate)
{
	CHECKF(nIndex>=0 && nIndex<10);

	switch(nIndex)
	{
	case 0:
		SetInt(EUDEMONDATA_skill_1,nData,bUpdate);
		break;
	case 1:
		SetInt(EUDEMONDATA_skill_2,nData,bUpdate);
		break;
	case 2:
		SetInt(EUDEMONDATA_skill_3,nData,bUpdate);
		break;
	case 3:
		SetInt(EUDEMONDATA_skill_4,nData,bUpdate);
		break;
	case 4:
		SetInt(EUDEMONDATA_skill_5,nData,bUpdate);
		break;
	case 5:
		SetInt(EUDEMONDATA_skill_6,nData,bUpdate);
		break;
	case 6:
		SetInt(EUDEMONDATA_skill_7,nData,bUpdate);
		break;
	case 7:
		SetInt(EUDEMONDATA_skill_8,nData,bUpdate);
		break;
	case 8:
		SetInt(EUDEMONDATA_skill_9,nData,bUpdate);
		break;
	case 9:
		SetInt(EUDEMONDATA_skill_10,nData,bUpdate);
		break;
	default:
		return false;
	}
	return true;
}