// PetEgg.cpp: implementation of the CPetEgg class.
//
//////////////////////////////////////////////////////////////////////
#include <afxdtctl.h>
#include "PetEgg.h"
#include "MapGroup.h"
#include "MsgPetegg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPetEgg::CPetEgg(PROCESS_ID idProcess, CUser* pUser)
{
	ASSERT (pUser);
	
	m_idProcess = idProcess;
	m_pUser		= pUser;
	
	m_setData	= CPetEggSet::CreateNew(true);
	m_tLastOnTimer = clock();
}

CPetEgg::~CPetEgg()
{
	if(m_setData)
		SAFE_RELEASE(m_setData);
}

CPetEgg* CPetEgg::CreateNew(PROCESS_ID idProcess, CUser* pUser)
{
	CHECKF (pUser);
	
	CPetEgg* pTreasure = new CPetEgg(idProcess, pUser);
	CHECKF(pTreasure);
	
	return pTreasure;
}

bool CPetEgg::CreateAll()
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(m_idProcess));
	CHECKF(m_setData);
	CHECKF(m_pUser);
	
	char szSQL[256]="";
	sprintf(szSQL,"SELECT * FROM %s WHERE owner_id=%u LIMIT 500", _TBL_PETEGG, m_pUser->GetID());
	//CHECKF(m_setData->Create(szSQL, Database()));用底层函数没数据时会报错

	IRecordset* pRes = Database()->CreateNewRecordset(szSQL, true);
	if(pRes)
	{
		for(int i = 0; i < pRes->RecordCount(); i++)
		{
			CPetEggData* pData = CPetEggData::CreateNew();
			if(!pData)
			{
				pRes->Release();
				return false;
			}
			
			if(!pData->Create(pRes/*,pDb*/))
			{
				pData->ReleaseByOwner();
				pRes->Release();
				return false;
			}
			
			m_setData->AddObj(pData);
			
			pRes->MoveNext();
		}
		
		pRes->Release();
	}
	
	return true;
}

bool CPetEgg::SaveInfo()
{
	for(int i = 0; i < m_setData->GetAmount(); i++)
	{
		CPetEggData* pData = m_setData->GetObjByIndex(i);
		IF_OK(pData)
		{
			pData->Update();
		}
	}
	return true;
}

OBJID CPetEgg::AddNewPetData(CItem* pItem, int nGrow, int nNextFlush)
{
	CHECKF(pItem);
	CHECKF(pItem->IsPetEgg());
	if(nNextFlush==0)
	{
		::LogSave("CPetEgg::AddNewPetData nNextFlush=0");
		nNextFlush = 60;
	}
	OBJID idEggData = ID_NONE;
	DEBUG_TRY
	CPetEggData* pData = pData = CPetEggData::CreateNew();
	CHECKF(pData);
	IF_NOT(pData->Create(GameDataDefault()->GetPeteggData(), idEggData))
		return ID_NONE;

	CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(pItem->GetInt(ITEMDATA_BASE1));
	if(!pEudemonType)
		return false;

	int nNeedLev = pEudemonType->GetInt(EUDEMONTYPEDATA_needLev);
	int nGetType = 2;

	if(nNeedLev > 0 && nNeedLev <= 20)
	{
		nGetType += RandGet(4);
	}
	else if(nNeedLev > 20 && nNeedLev <= 40)
	{
		nGetType += RandGet(5);
	}
	else if(nNeedLev > 40 && nNeedLev <= 60)
	{
		nGetType += RandGet(6);
	}
	else if(nNeedLev > 60 && nNeedLev <= 80)
	{
		nGetType += RandGet(7);
	}
	else
	{
		nGetType += RandGet(8);
	}

	pData->SetInt(PETEGGDATA_OWNERID, m_pUser->GetID());
	pData->SetInt(PETEGGDATA_ITEMID, pItem->GetID());
	pData->SetInt(PETEGGDATA_GETTYPE, nGetType);
	pData->SetInt(PETEGGDATA_GROW, nGrow);
	pData->SetInt(PETEGGDATA_STRCHG, pEudemonType->GetInt(EUDEMONTYPEDATA_initstrChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randstrChg)));
	pData->SetInt(PETEGGDATA_INTCHG, pEudemonType->GetInt(EUDEMONTYPEDATA_initintChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randintChg)));
	pData->SetInt(PETEGGDATA_STACHG, pEudemonType->GetInt(EUDEMONTYPEDATA_initstaChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randstaChg)));
	pData->SetInt(PETEGGDATA_SPICHG, pEudemonType->GetInt(EUDEMONTYPEDATA_initspiChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randspiChg)));
	pData->SetInt(PETEGGDATA_AGICHG, pEudemonType->GetInt(EUDEMONTYPEDATA_initagiChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randagiChg)));

	//计算下次刷新的时间，没有年，判断时要考虑跨年的情况
	CTime tCurr = CTime::GetCurrentTime();
	CTime tNextFlush = tCurr + CTimeSpan(nNextFlush/86400, nNextFlush/3600%24, nNextFlush/60%60, nNextFlush%60);
	DWORD dwNextTime = tNextFlush.GetMonth() * 100000000 + tNextFlush.GetDay() * 1000000 + tNextFlush.GetHour() * 10000 + tNextFlush.GetMinute() * 100 + tNextFlush.GetSecond();
	pData->SetInt(PETEGGDATA_NEXTFLUSH, dwNextTime);
	
	idEggData = pData->InsertRecord();
	m_setData->AddObj(pData);
	DEBUG_CATCH("CPetEgg::AddNewPetData")
	return idEggData;
}

bool CPetEgg::FlushPetData(OBJID idData, CItem* pItem, int nGrow, int nNextFlush)
{
	CHECKF(pItem);
	CHECKF(idData);

	if(nNextFlush==0)
	{
		::LogSave("CPetEgg::FlushPetData nNextFlush=0");
		nNextFlush = 60;
	}
	DEBUG_TRY
	CPetEggData* pData = m_setData->GetObj(idData);
	CHECKF(pData);

	CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(pItem->GetInt(ITEMDATA_BASE1));
	if(!pEudemonType)
		return false;

	int nNeedLev = pEudemonType->GetInt(EUDEMONTYPEDATA_needLev);
	int nGetType = 2;
	
	if(nNeedLev > 0 && nNeedLev <= 20)
	{
		nGetType += RandGet(4);
	}
	else if(nNeedLev > 20 && nNeedLev <= 40)
	{
		nGetType += RandGet(5);
	}
	else if(nNeedLev > 40 && nNeedLev <= 60)
	{
		nGetType += RandGet(6);
	}
	else if(nNeedLev > 60 && nNeedLev <= 80)
	{
		nGetType += RandGet(7);
	}
	else
	{
		nGetType += RandGet(8);
	}

	pData->SetInt(PETEGGDATA_GETTYPE, nGetType);
	pData->SetInt(PETEGGDATA_GROW, nGrow);
	pData->SetInt(PETEGGDATA_STRCHG, pEudemonType->GetInt(EUDEMONTYPEDATA_initstrChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randstrChg)));
	pData->SetInt(PETEGGDATA_INTCHG, pEudemonType->GetInt(EUDEMONTYPEDATA_initintChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randintChg)));
	pData->SetInt(PETEGGDATA_STACHG, pEudemonType->GetInt(EUDEMONTYPEDATA_initstaChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randstaChg)));
	pData->SetInt(PETEGGDATA_SPICHG, pEudemonType->GetInt(EUDEMONTYPEDATA_initspiChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randspiChg)));
	pData->SetInt(PETEGGDATA_AGICHG, pEudemonType->GetInt(EUDEMONTYPEDATA_initagiChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randagiChg)));
	
	//计算下次刷新的时间，没有年，判断时要考虑跨年的情况
	CTime tCurr = CTime::GetCurrentTime();
	CTime tNextFlush = tCurr + CTimeSpan(nNextFlush/86400, nNextFlush/3600%24, nNextFlush/60%60, nNextFlush%60);
	DWORD dwNextTime = tNextFlush.GetMonth() * 100000000 + tNextFlush.GetDay() * 1000000 + tNextFlush.GetHour() * 10000 + tNextFlush.GetMinute() * 100 + tNextFlush.GetSecond();
	pData->SetInt(PETEGGDATA_NEXTFLUSH, dwNextTime);
	DEBUG_CATCH("CPetEgg::FlushPetData")
	return true;
}

bool CPetEgg::FlushItemAllPetData(CItem* pItem, bool bUseProp/*=false*/)
{
	CHECKF(pItem);
	DEBUG_TRY
	OBJID idAction = 0;
	if(bUseProp)
		idAction = 4001;
	else
		idAction = 4000;

	OBJID idItem = pItem->GetID();
	int	  nMax = pItem->GetInt(ITEMDATA_BASE2);
	int	  nFlushed = 0;

	CPetEggData* pData = NULL;
	for(int i=m_setData->GetAmount()-1; i>=0; i--)
	{
		pData = m_setData->GetObjByIndex(i);
		if(!pData)
			continue;

		if(pData->GetInt(PETEGGDATA_ITEMID)==idItem)
		{
			if(nFlushed > nMax)//竟然多了
			{
				pData->DeleteRecord();
				m_setData->DelObj(pData->GetID());
				::LogSave("CPetEgg::FlushItemAllPetData nFlushed > nMax, nFlushed=%d,nMax=%d", nFlushed, nMax);
			}
			else
			{
				LuaScriptMachine()->InitVar(pData->GetID());
				m_pUser->ProcessActionIncludeItem(idAction, pItem);
			}
			nFlushed++;
		}
	}

	if(nFlushed < nMax)//竟然少了
	{
		for(i=nFlushed; i<nMax; i++)
		{
			LuaScriptMachine()->InitVar(0);
			m_pUser->ProcessActionIncludeItem(idAction, pItem);
		}
		//::LogSave("CPetEgg::FlushItemAllPetData nFlushed < nMax, nFlushed=%d,nMax=%d", nFlushed, nMax);
	}
	
	if(!bUseProp)
		pItem->SetInt(ITEMDATA_BASIC_ADDITION1, pItem->GetInt(ITEMDATA_BASIC_ADDITION1)+1);
	DEBUG_CATCH("CPetEgg::FlushItemAllPetData")
	return true;
}

CItem* CPetEgg::GetItem(CPetEggData* pData)
{
	CHECKF(pData);

	OBJID idItem = pData->GetInt(PETEGGDATA_ITEMID);
	CHECKF(idItem);
	
	return m_pUser->GetItem(idItem, false, true);
}

void CPetEgg::OnTimer(time_t tCurr)
{
	if(clock() - m_tLastOnTimer < PETEGG_ONTIMER_INTEVAL)
		return;

	int nStep = 0;
	m_tLastOnTimer = clock();
	DEBUG_TRY
	CPetEggData* pData = NULL;
	CItem* pItem = NULL;
	nStep = 1;
	for(int i=m_setData->GetAmount()-1; i>=0; i--)
	{
		nStep = 2;
		pData = m_setData->GetObjByIndex(i);
		nStep = 3;
		if(pData && pData->NeedFlush())
		{
			nStep = 4;
			pItem = GetItem(pData);
			if(pItem)
			{
				nStep = 5;
				if(FlushItemAllPetData(pItem))
					SendToClient(pItem, PETEGG_FLUSHED);
			}
			else//物品不存在了，删掉龙蛋数据
				DelPetEgg(pData->GetInt(PETEGGDATA_ITEMID));
		}
	}
	DEBUG_CATCH2("void CPetEgg::OnTimer(time_t tCurr) nStep=%d", nStep)
}

bool CPetEgg::CreatePetByEgg(OBJID idData, CItem* pEgg, int nGetType, int nGrow)
{
	CHECKF(idData);
	CHECKF(pEgg);
	CHECKF(pEgg->IsPetEgg());

	CPetEggData* pEggData = m_setData->GetObj(idData);
	if(!pEggData)
		return false;

//modify code by 林德渊 begin 2011.5.23
	CPetEggData* pData = NULL;
	for(int i=m_setData->GetAmount()-1; i>=0; i--)
	{
		pData = m_setData->GetObjByIndex(i);
		if(!pData)
			continue;
		
		if( NULL != pData )
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) one pet (typeid:%u)(grow:%d)(gettype:%d)(str:%d)(int:%d)(sta:%d)(spi:%d)(agi:%d) from pet egg.", m_pUser->GetID(), m_pUser->GetName(), pData->GetInt(PETEGGDATA_ITEMID), pData->GetInt(PETEGGDATA_GROW), pData->GetInt(PETEGGDATA_GETTYPE), pData->GetInt(PETEGGDATA_STRCHG), pData->GetInt(PETEGGDATA_INTCHG), pData->GetInt(PETEGGDATA_STACHG), pData->GetInt(PETEGGDATA_SPICHG), pData->GetInt(PETEGGDATA_AGICHG));
	}
//modify code by 林德渊 end

	OBJID idItem = pEgg->GetID();
	if(pEggData->GetInt(PETEGGDATA_ITEMID) != idItem)
		return false;

	EudemonData newPetData;
	newPetData.gettype = pEggData->GetInt(PETEGGDATA_GETTYPE);
	newPetData.grow    = pEggData->GetInt(PETEGGDATA_GROW);
	newPetData.strchg  = pEggData->GetInt(PETEGGDATA_STRCHG);
	newPetData.intchg  = pEggData->GetInt(PETEGGDATA_INTCHG);
	newPetData.stachg  = pEggData->GetInt(PETEGGDATA_STACHG);
	newPetData.spichg  = pEggData->GetInt(PETEGGDATA_SPICHG);
	newPetData.agichg  = pEggData->GetInt(PETEGGDATA_AGICHG);

	if(newPetData.gettype!=nGetType || newPetData.grow!=nGrow)
	{
		m_pUser->SendSysMsg(_TXTATR_TOPSHOW, "宠物数据已刷新，请重新打开界面");
		return false;
	}
	CUserPetData* pTemp = m_pUser->AwardEudemonByEgg(pEgg->GetInt(ITEMDATA_BASE1), newPetData);
 	if(!pTemp)
		return false;

	char szItemEgg[128] = "";
	sprintf(szItemEgg, "%d_[%s]_0_%d_0_0_%d", SNAPSHOTOBJ, pEgg->GetStr(ITEMDATA_NAME), pEgg->GetTypeID(), pEgg->GetColour());

	if(m_pUser->SpendItem(pEgg))
		DelPetEgg(idItem);
	
	m_pUser->SendSysMsg(_TXTATR_TOPSHOW, "领养成功");

	DEBUG_TRY
	//系统公告
	if(newPetData.grow > 60 || newPetData.gettype > 7)
	{
		char UserName[128] = "";
		char szPetName[128]  = "";
		sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, m_pUser->GetName());
		sprintf(szPetName,  "%d_[%s]_%d_%d_%d_%d_%d",
			SNAPSHOTUSEREDU, pTemp->GetStr(EUDEMONDATA_name), pTemp->GetID(), pTemp->GetInt(EUDEMONDATA_typeid), m_pUser->GetID(), 0, COLORPURPLE);
		char szGrow[64] = "";
		if(newPetData.grow < 21)
			strcpy(szGrow,"3_普通_6");
		else if(newPetData.grow > 20 && newPetData.grow < 41)
			strcpy(szGrow,"3_优秀_6");
		else if(newPetData.grow > 40 && newPetData.grow < 61)
			strcpy(szGrow,"3_杰出_6");
		else if(newPetData.grow > 60 && newPetData.grow < 81)
			strcpy(szGrow,"3_卓越_6");
		else if(newPetData.grow > 80)
			strcpy(szGrow,"3_完美_6");
		
		char szMsg[255] = "";
		sprintf(szMsg, "<%s>非常有爱心的在<%s>中领养到了一只<%s>的<3_%d_6>档<%s>，令旁人无比羡慕。",UserName, szItemEgg, szGrow, newPetData.gettype-1, szPetName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,m_pUser, pTemp->GetID(), pTemp->GetInt(EUDEMONDATA_typeid), NULL, ALLUSERS_NAME);
	}

	::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get pet %u(type:%u) from pet egg. grow(%d) gettype(%d)", m_pUser->GetID(), m_pUser->GetName(), pTemp->GetID(), pTemp->GetInt(EUDEMONDATA_typeid), newPetData.grow, newPetData.gettype);
	DEBUG_CATCH("CPetEgg::CreatePetByEgg broadcast error!")

	return true;
}

void CPetEgg::DelPetEgg(OBJID idItem)
{
	CHECK(idItem);

	DEBUG_TRY
	CPetEggData* pData = NULL;
	for(int i=m_setData->GetAmount()-1; i>=0; i--)
	{
		pData = m_setData->GetObjByIndex(i);
		if(pData && pData->GetInt(PETEGGDATA_ITEMID)==idItem)
		{
			pData->DeleteRecord();
			m_setData->DelObj(pData->GetID());
		}
	}
	DEBUG_CATCH("CPetEgg::DelPetEgg")
}

void CPetEgg::SendToClient(CItem* pItem, int nType)
{
	CHECK(pItem);
	
	DEBUG_TRY
	CEudemonTypeData* pEudemonType = EudemonType()->GetObj(pItem->GetInt(ITEMDATA_BASE1));
	if(!pEudemonType)
		return;

	CMsgPetegg msg;
	if(!msg.Create(m_pUser->GetID(), pItem->GetID(), nType, pItem->GetInt(ITEMDATA_BASE1), 0, pEudemonType->GetStr(EUDEMONTYPEDATA_name)))
		return;

	CPetEggData* pData = NULL;
	OBJID idItem = pItem->GetID();
	LONG  nextSec = 0;
	ST_PETEGG eggInfo ={0};
	for(int i=m_setData->GetAmount()-1; i>=0; i--)
	{
		pData = m_setData->GetObjByIndex(i);
		if(pData && pData->GetInt(PETEGGDATA_ITEMID)==idItem)
		{
			if(nextSec==0)
				nextSec = pData->GetNextFlushSeconds();

			eggInfo.idData    = pData->GetID();
			eggInfo.ucGetType = pData->GetInt(PETEGGDATA_GETTYPE);
			eggInfo.ucLookFace= pEudemonType->GetInt(EUDEMONTYPEDATA_lookface);
			eggInfo.usGrow    = pData->GetInt(PETEGGDATA_GROW);
			eggInfo.usStr	  = pData->GetInt(PETEGGDATA_STRCHG);
			eggInfo.usInt	  = pData->GetInt(PETEGGDATA_INTCHG);
			eggInfo.usSta     = pData->GetInt(PETEGGDATA_STACHG);
			eggInfo.usSpi     = pData->GetInt(PETEGGDATA_SPICHG);
			eggInfo.usAgi     = pData->GetInt(PETEGGDATA_AGICHG);
			eggInfo.usMaxStr  = pEudemonType->GetInt(EUDEMONTYPEDATA_initstrChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randstrChg);
			eggInfo.usMaxInt  = pEudemonType->GetInt(EUDEMONTYPEDATA_initintChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randintChg);
			eggInfo.usMaxSta  = pEudemonType->GetInt(EUDEMONTYPEDATA_initstaChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randstaChg);
			eggInfo.usMaxSpi  = pEudemonType->GetInt(EUDEMONTYPEDATA_initspiChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randspiChg);
			eggInfo.usMaxAgi  = pEudemonType->GetInt(EUDEMONTYPEDATA_initagiChg) + pEudemonType->GetInt(EUDEMONTYPEDATA_randagiChg);
			msg.Append(eggInfo);
		}
	}
	msg.m_pInfo->nNeedSec = nextSec;
	m_pUser->SendMsg(&msg);
	DEBUG_CATCH("CPetEgg::DelPetEgg")
}

bool CPetEgg::GetInfoByIndex(EggInfoStruct* pInfo, int idx)
{
	CHECKF(pInfo);
	CHECKF(idx >= 0 && idx < m_setData->GetAmount());
	
	CPetEggData* pData = m_setData->GetObjByIndex(idx);
	IF_OK(pData)
	{
		pInfo->id = pData->GetInt(PETEGGDATA_ID);
		pInfo->idUser = pData->GetInt(PETEGGDATA_OWNERID);
		pInfo->idItem  = pData->GetInt(PETEGGDATA_ITEMID);
		pInfo->usGetType = pData->GetInt(PETEGGDATA_GETTYPE);
		pInfo->usGrow = pData->GetInt(PETEGGDATA_GROW);
		pInfo->usStrChg = pData->GetInt(PETEGGDATA_STRCHG);
		pInfo->usIntChg = pData->GetInt(PETEGGDATA_INTCHG);
		pInfo->usStaChg = pData->GetInt(PETEGGDATA_STACHG);
		pInfo->usSpiChg = pData->GetInt(PETEGGDATA_SPICHG);
		pInfo->usAgiChg = pData->GetInt(PETEGGDATA_AGICHG);
		pInfo->dwNextFlush = pData->GetInt(PETEGGDATA_NEXTFLUSH);
		return true;
	}
	return false;
}
bool CPetEgg::AppendPetData(const EggInfoStruct* pInfo)
{
	CHECKF(pInfo);
	
	CPetEggData* pData = CPetEggData::CreateNew();
	CHECKF(pData);
	IF_NOT(pData->Create(GameDataDefault()->GetPeteggData(), pInfo->id))
		return false;
	
	pData->SetInt(PETEGGDATA_OWNERID, pInfo->idUser);
	pData->SetInt(PETEGGDATA_ITEMID, pInfo->idItem);
	pData->SetInt(PETEGGDATA_GETTYPE, pInfo->usGetType);
	pData->SetInt(PETEGGDATA_GROW, pInfo->usGrow);
	pData->SetInt(PETEGGDATA_STRCHG, pInfo->usStrChg);
	pData->SetInt(PETEGGDATA_INTCHG, pInfo->usIntChg);
	pData->SetInt(PETEGGDATA_STACHG, pInfo->usStaChg);
	pData->SetInt(PETEGGDATA_SPICHG, pInfo->usSpiChg);
	pData->SetInt(PETEGGDATA_AGICHG, pInfo->usAgiChg);
	pData->SetInt(PETEGGDATA_NEXTFLUSH, pInfo->dwNextFlush);
	pData->ClearUpdateFlags();
	m_setData->AddObj(pData);
	return true;
}