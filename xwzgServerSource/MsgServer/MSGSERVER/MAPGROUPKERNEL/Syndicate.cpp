// Syndicate.cpp: implementation of the CSyndicate class.
//
//////////////////////////////////////////////////////////////////////
#include "afx.h"
#include "AllMsg.h"
#include "Syndicate.h"
#include "MapGroup.h"
#include "UserTitle.h"
extern int g_SpecilLine;
//////////////////////////////////////////////////////////////////////
char	szSynTable[] = _TBL_SYNDICATE;

MYHEAP_IMPLEMENTATION(CSyndicate,s_heap)
const int _SYN_PAYOFF_TEX = 30;//������˰.�ٷֱ�.100Ϊ100% // �½������ [5/18/2007]
#define  MONEY_BASIC 10000//�ѵ�λת�ɽ�

int Grows[]={0,10000,20000,50000,120000,250000,300000,350000,400000,450000,500000};//��������--jay
int Moneys[]={0,2000,5000,10000,20000,50000,60000,70000,80000,90000,100000};//�ʽ�����,��λ:�� ��ʹ��ʱ��Ҫ��*money_basic

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSyndicate::CSyndicate()
{
	SetObjType(OBJ_SYN);
	m_pData	= NULL;
//	m_obj.m_pOwner	= this;
	m_link.Init(this);
	warState = 0;
	m_warKill = 0;
	warTurn = 0;
	dwLastUpdate = clock();
	m_bLock=false;
	AmontStatue = -1;
	MaintainSta = -1;
	m_nMapID=ID_NONE;
	m_bCallTask=false;
}

//////////////////////////////////////////////////////////////////////
CSyndicate::~CSyndicate()
{
	if(m_pData)
	{
		m_pData->Update();
		m_pData->Release();
	}
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::Create(IRecordset* pRes)
{
	ASSERT(!m_pData);

	m_pData = CSynBase::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pRes))
		return false;
	if(m_pData->GetInt(SYNDATA_STATE) == 3)		// ��ɾ���İ���
		return false;

	idLeader = ID_NONE;
	idSubLeader = ID_NONE;
	idFeelLeader =ID_NONE;
	idSubLeader2 =ID_NONE;
// 
// 	idFeelLeader1 = ID_NONE;
// 	idFeelLeader2 = ID_NONE;
// 	idFeelLeader3 = ID_NONE;
// 	idFeelLeader4 = ID_NONE;
// 	idSubFeelLeader1 = ID_NONE;
// 	idSubFeelLeader2 = ID_NONE;
// 	idSubFeelLeader3 = ID_NONE;
// 	idSubFeelLeader4 = ID_NONE;

	// ͳ�ư�������
	char	szSQL[1024];
	sprintf(szSQL, "SELECT u.id, u.name, u.level, u.Fir_Job, u.sex,u.last_login,u.vipstate,u.viplev ,s.rank ,s.history_con,s.history_money,s.history_grow FROM %s as s LEFT JOIN %s AS u ON u.id=s.id WHERE s.syn_id=%u && u.id IS NOT NULL ORDER BY s.rank DESC, u.name LIMIT %d",
		_TBL_SYNATTR, _TBL_USER, m_pData->GetID(), 250);

	IRecordset* pStatRes = Database()->CreateNewRecordset(szSQL);
	if(pStatRes)
	{
		m_setMember.clear();
		for(int i = 0; i < pStatRes->RecordCount(); i++,pStatRes->MoveNext())
		{
			OBJID id = pStatRes->GetInt(0);
			int nRank = pStatRes->GetInt(8);
			int newRank = nRank;
			switch(nRank)
			{
			case 10:
			case 20:
			case 51:
			case 52:
			case 53:
			case 54:
			case 61:
			case 62:
			case 63:
			case 64:
			case 71:
			case 72:
			case 73:
			case 74:
			case 81:
			case 82:
			case 83:
			case 84:
				newRank=RANK_NORMAL;
				break;
			case RANK_FEELLEADER:
				newRank=RANK_FEELLEADER;
					break;
			case RANK_SUBLEADER:
				if(idSubLeader != ID_NONE)
				{
					if (idSubLeader2 != ID_NONE)
					{
						newRank = 10;
					}
					else
						idSubLeader2 = id;
				}
				else
					idSubLeader = id;
				break;
			case RANK_LEADER:
				if(idLeader != ID_NONE)
					newRank = 10;
				else
					idLeader = id;
				break;
			default:
				newRank = 10;
			}
			if(newRank != nRank)
			{
				char	szSQL[1024];
				sprintf(szSQL, "UPDATE %s SET rank=%u WHERE id=%u LIMIT 1",
					_TBL_SYNATTR,newRank, id );
				IF_NOT(Database()->ExecuteSQL(szSQL))
				{
/*					::LogSave("���ɳ�Ա�б����ʧ��,rank = %u, id = %u", newRank, id);*/
					return false;
				}
			}
// 			nAmount ++;

			MemberInfo info={0};
			info.id = pStatRes->GetInt(0);
			SafeCopy(info.szName,pStatRes->GetStr(1));
			info.level=pStatRes->GetInt(2);
			info.firJob=pStatRes->GetInt(3);
			info.sex=pStatRes->GetInt(4);
			info.lastlogin=pStatRes->GetInt(5);
			if (pStatRes->GetInt(6) == 1)
				info.vip=pStatRes->GetInt(7);
			else
				info.vip=0;
			info.rank=pStatRes->GetInt(8);
			info.hiscon =pStatRes->GetInt(9);
			info.hismoney=pStatRes->GetInt(10);
			info.hisgrow =pStatRes->GetInt(11);
			info.line=0;
			MemberInfo *newinfo=GetMemberFromList(info.id,1);
			if(newinfo==NULL)
				m_setMember.push_back(info);
		}		
		pStatRes->Release();
	}
	return true;
}
void CSyndicate::DelSynMonster()
{
	for (int i=0;i<m_SynMonsterList.size();i++)
	{
		if (m_SynMonsterList[i]!=ID_NONE)
			NpcManager()->DelMonsterByID(m_SynMonsterList[i]);
	}
	m_SynMonsterList.clear();
}
void CSyndicate::AddSynMonster(CMonsterPtr pMonster)
{
	CHECK(pMonster);
	m_SynMonsterList.push_back(pMonster->GetID());
}
//////////////////////////////////////////////////////////////////////
bool CSyndicate::ReloadData()
{
	ASSERT(m_pData);

/*		//??? ��������������ҪRELOAD
	CSynBase* pData = CSynBase::CreateNew();
	CHECKF(pData);
	IF_NOT(pData->Create(GetID(), Database()))
	{
		pData->Release();
		return false;
	}

	if(pData->GetInt(SYNDATA_DEL_FLAG) != 0)		// ��ɾ���İ���
	{
		pData->Release();
		return false;
	}

	m_pData->Release();
	m_pData	= pData;
*/
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::Create(CreateSyndicateInfo* pInfo, bool bSave)
{
	ASSERT(!m_pData);

	OBJID idSyn = bSave ? ID_NONE : pInfo->idSyn;
	m_pData = CSynBase::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(GameDataDefault()->GetSynData(), idSyn))
		return false;
	//m_pData->SetInt(SYNDATA_ID, idSyn);
	m_pData->SetStr(SYNDATA_NAME, pInfo->szName, _MAX_NAMESIZE);
	m_pData->SetStr(SYNDATA_ANNOUNCE, pInfo->szWord, MAX_TENETSIZE);
	m_pData->SetInt(SYNDATA_LEADERID, pInfo->idLeader);
	m_pData->SetStr(SYNDATA_LEADERNAME, pInfo->szLeader, _MAX_NAMESIZE);
	m_pData->SetStr(SYNDATA_CREATELEADERNAME, pInfo->szLeader, _MAX_NAMESIZE);
	m_pData->SetInt(SYNDATA_STATE, 1);
	m_pData->SetInt(SYNDATA_MONEY, pInfo->nMoney);

	SYSTEMTIME localTime;
	::GetLocalTime( &localTime );
	int nCurTime = localTime.wYear * 1000000 + localTime.wMonth * 10000 + localTime.wDay* 100+ localTime.wHour;
	m_pData->SetInt( SYNDATA_CREATETIME,nCurTime );

	idLeader = ID_NONE;
	idSubLeader = ID_NONE;
	idSubLeader = ID_NONE;
	idFeelLeader =ID_NONE;
	idSubLeader2 =ID_NONE;

// 	idFeelLeader1 = ID_NONE;
// 	idFeelLeader2 = ID_NONE;
// 	idFeelLeader3 = ID_NONE;
// 	idFeelLeader4 = ID_NONE;
// 	idSubFeelLeader1 = ID_NONE;
// 	idSubFeelLeader2 = ID_NONE;
// 	idSubFeelLeader3 = ID_NONE;
// 	idSubFeelLeader4 = ID_NONE;
// 	nAmount = 0;
//	nShpAmount = 0;
// 	nOnlineAmount = 0;


	if(bSave)
	{
		m_pData->InsertRecord();
		pInfo->idSyn = m_pData->GetInt(SYNDATA_ID);
		return  pInfo->idSyn != ID_NONE;
	}
	else
	{
		m_pData->ClearUpdateFlags();
		return true;
	}
}

//////////////////////////////////////////////////////////////////////
// modify
//////////////////////////////////////////////////////////////////////
bool CSyndicate::Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave)	
{
	if(!( idOldLeader==(OBJID)m_pData->GetInt(SYNDATA_LEADERID) || (OBJID)m_pData->GetInt(SYNDATA_LEADERID)==ID_NONE ))	//? �����ھ�����
		return false;

	NAMESTR	szNewLeader2;	
	SafeCopy(szNewLeader2, szNewLeader, _MAX_NAMESIZE);

	m_pData->SetInt(SYNDATA_LEADERID, idNewLeader);
	m_pData->SetStr(SYNDATA_LEADERNAME, szNewLeader2, _MAX_NAMESIZE);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();

	CUser* pOldUser = UserManager()->GetUser(idOldLeader);
	if(pOldUser)
	{
		pOldUser->QuerySynAttr()->SetIdRank(0,GetID(), RANK_NORMAL);
		pOldUser->QuerySynAttr()->SaveInfo();
		pOldUser->QuerySynAttr()->SynchroInfo();
	}

	CUser* pNewUser = UserManager()->GetUser(idNewLeader);
	if(pNewUser)
	{
		pNewUser->QuerySynAttr()->SetIdRank(0,this->GetID(), RANK_LEADER);
		pNewUser->QuerySynAttr()->SaveInfo();
		pNewUser->QuerySynAttr()->SynchroInfo();
	}

	CMsgSynInfo	msg;
	if(msg.Create(this))
		this->BroadcastSynMsg(&msg );
	
	return true; 
}

bool CSyndicate::SetLastBonusTime( LPCTSTR szTime )
{
//	m_pData->SetStr( SYNDATA_LASTBONUSTIME, szTime, 10 );
	m_pData->Update();
	return true; 
}
//////////////////////////////////////////////////////////////////////
bool CSyndicate::SetTenet(LPCTSTR szTenet, bool bSave)
{
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave)
{
	m_pData->SetStr(SYNDATA_ANNOUNCE, szLeaderTitle, MAX_TENETSIZE);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::SetMemberTitle(LPCTSTR szMemberTitle, bool bSave)
{

	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::AddData(SYNDATA idx, int nData, bool bSave)
{
	int nOldMoney	= m_pData->GetInt(SYNDATA_MONEY);
// 	int nOldRepute  = m_pData->GetInt(SYNDATA_REPUTE);
// 	if(idx == SYNDATA_REPUTE && nOldRepute + nData <0 )
// 		nData	=	nOldRepute * -1;
// 	int nAllData = ::AddToTop(m_pData->GetInt(idx), nData, SYNMONEY_LIMIT);		// use for money or amount
// 	if(nAllData<0)
// 		nAllData = 0;
	m_pData->SetInt(idx, nData);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();

	//ȥ�����ɻ��� satan 2009.03.06
//	if(nOldMoney > 0 && m_pData->GetInt(SYNDATA_MONEY) <= 0)
//	{
//		CMsgTalk	msg;
//		IF_OK(msg.Create(_TXTATR_IMPORTINFO, STR_DESTROY_SYNDICATE_SOON))
//			BroadcastSynMsg(&msg);
//	}

	{
		this->SynchroInfo();
	}

	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::SetData(SYNDATA idx, int nData, bool bSave)
{
//	int	nOldData = m_pData->GetInt(idx);
	if(m_pData->GetInt(idx) != nData)
	{
		m_pData->SetInt(idx, nData);
		if(bSave)
			m_pData->Update();
		else
			m_pData->ClearUpdateFlags();
	}
	// synchro
//	SynchroInfo();
	

	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::PresentMoney(OBJID idSyn, int nMoney, bool bSave)
{
	if(nMoney <= 0 || m_pData->GetInt(SYNDATA_MONEY) < nMoney)
		return false;
	CSyndicate* pTarget = SynManager()->QuerySyndicate(idSyn);
	if(!pTarget)
		return false;

	m_pData->SetInt(SYNDATA_MONEY, m_pData->GetInt(SYNDATA_MONEY)-nMoney);
	pTarget->m_pData->SetInt(SYNDATA_MONEY, pTarget->m_pData->GetInt(SYNDATA_MONEY)+nMoney);
	if(bSave)
	{
		m_pData->Update();
		pTarget->m_pData->Update();
	}
	else
	{
		m_pData->ClearUpdateFlags();
		pTarget->m_pData->ClearUpdateFlags();
	}
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::SetSynName(LPCTSTR szSynName, bool bSave)
{
	m_pData->SetStr(SYNDATA_NAME, szSynName, _MAX_NAMESIZE);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();


	//ͬ��client.broadcast
	CSyndicate* pMaster = this->GetMasterSyn();
	CHECKF(pMaster);	

	CUserManager::Iterator pUser = UserManager()->NewEnum();
	while(pUser.Next())
	{
		if(pUser && pUser->GetSynID() == pMaster->GetID())				
		{
			CMsgName msg;
			msg.Create( 3, szSynName, pUser->GetSynID());
			pUser->SendMsg(&msg);
		}
	}
	return true; 
}

void CSyndicate::SetSynWar(OBJID warID, bool bSave)
{
	SYSTEMTIME localTime;
	char szSynWarTime[20];	
	::GetLocalTime( &localTime );
	sprintf(szSynWarTime,"%d-%d-%d-%d-%d",localTime.wYear,localTime.wMonth,localTime.wDay,localTime.wHour,localTime.wMinute);	
	
// 	m_pData->SetInt(SYNDATA_WARID, warID);
// 	if(bSave)
// 	{
// 		m_pData->SetStr(SYNDATA_WARSTARTTIME, szSynWarTime, 20);
// 		m_pData->Update();
// 	}
// 	else
// 		m_pData->ClearUpdateFlags();
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::ChangeSyndicate(const SynFuncInfo0* pFuncInfo)
{
	CHECKF(pFuncInfo->idSyn == GetID());

	switch(pFuncInfo->ucFuncType)
	{
	case	SYNFUNC_DEMISE:
		return Demise(pFuncInfo->IntParam[0], pFuncInfo->IntParam[3],pFuncInfo->IntParam[1], (LPCTSTR)&pFuncInfo->IntParam[5], pFuncInfo->IntParam[2],pFuncInfo->IntParam[4], pFuncInfo->ucSaveFlag!=0 /*true*/);//[��;���� 2008/10/7]����ͬ������
		break;
// 	case	SYNFUNC_SETTENET:
// 		{
// 			bool bRet = (SetTenet(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag!=0)
// 							&& SetData(SYNDATA_PUBLISHTIME, pFuncInfo->dwData[0], pFuncInfo->ucSaveFlag!=0));
// 			if (bRet)
// 			{
// 				CMsgSyndicate msg;
// 				if (msg.Create(SYN_SET_PUBLISHTIME, GetID(), GetInt(SYNDATA_PUBLISHTIME)))
// 					BroadcastSynMsg(&msg);
// 			}
// 			return bRet;
// 		}
// 		break;
	case	SYNFUNC_SETLEADERTITLE:
		return SetLeaderTitle(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag!=0);
		break;
	case	SYNFUNC_SETMEMBERTITLE:
		return SetMemberTitle(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag!=0);
		break;
	case	SYNFUNC_ADDDATA:
		return AddData((SYNDATA)pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag!=0);
		break;
	case	SYNFUNC_SETDATA:
		return SetData((SYNDATA)pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag!=0);
		break;
	case	SYNFUNC_PRESENT:
		return PresentMoney(pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag!=0);
		break;
	case	SYNFUNC_SETSYNNAME:
		return SetSynName(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag!=0);
		break;
		//20070414����:������.
	case SYNFUNC_PAYOFF:
		{
			return PayOff(pFuncInfo->IntParam[0],pFuncInfo->ucSaveFlag!=0);
		}
		break;
	case SYNFUNC_SET_RANK:
		{
			CUser* pTarget = UserManager()->GetUser(pFuncInfo->StrParam);
			if(pTarget)
			{
				pTarget->QuerySynAttr()->SetIdRank(0,pTarget->GetSynID(),pFuncInfo->nRank);
			}
			//SetMemberRank(pFuncInfo->StrParam, pFuncInfo->nRank);
		}
		break;
	case SYNFUNC_LEAVERANK:
		{
			CSyndicate* pSyn = SynManager()->QuerySyndicate(pFuncInfo->idSyn);
			if (pSyn)
			{
				if(RANK_SUBLEADER == pFuncInfo->nRank)
				{
				// 	pSyn->SetSubLeaderID(0);
//					pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, 0, true);
				}
//				else if(RANK_LEADERWIFE == pFuncInfo->nRank)
//				{
// 					pSyn->SetLeaderWifeID(0);
//					pSyn->QueryModify()->SetData(SYNDATA_LEADERWIFE, 0, true);
//				}

				return true;
			}else
			{
				return false;
			}
		}
		break;
	default:
		ASSERT(!"switch CSyndicate::ChangeSyndicate()");
		break;
	}
	return false;
}
void CSyndicate::SetInSynMapID()
{
	OBJID idmap=0;
	if (GetSynMapID()==ID_NONE)
	{
		SQLBUF szSQL;
		sprintf(szSQL, "SELECT * FROM %s WHERE idsyn = %d LIMIT 1", _TBL_SYNMAP,GetID());
		IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
		if (pRes && pRes->RecordCount()>0)
		{
			idmap=pRes->GetInt(0);
			pRes->Release();
		}
		else
			LOGERROR("������ȡ���ɵ�ͼ���� idsyn=%u",GetID());
		SetSynMapID(idmap);
	} 
}
void CSyndicate::CreateSynMap()
{
	NewMapInfo info={0};
	OBJID idmap=0;
	CMapPtr pMap=CGameMap::CreateNew();
	CHECK(pMap);
	SQLBUF szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE id = 1048 LIMIT 1", _TBL_MAP);
	IRecordset* pRes2 = Database()->CreateNewRecordset(szSQL, false);
	CHECK(pRes2);
	sprintf(szSQL, "SELECT * FROM %s WHERE idsyn = %d LIMIT 1", _TBL_SYNMAP,GetID());
	IRecordset* pRes3 = Database()->CreateNewRecordset(szSQL);
	if (pRes3 && pRes3->RecordCount()>0)
	{
		idmap=pMap->CreateSpecilMap(m_idProcess,pRes3);
		pRes3->Release();
	}
	else
	{
		SafeCopy(info.szName,pRes2->GetStr(1),_MAX_NAMESIZE);
		info.nMapDoc=pRes2->GetInt(2);
		info.nType=pRes2->GetInt(3);
		info.nMapGroup=GetID();
		info.nOwnerType=pRes2->GetInt(5);
		info.idOwner=pRes2->GetInt(6);
		info.nDisable=pRes2->GetInt(7);
		idmap=pMap->CreateSpecilMap(m_idProcess,&info);
	}
	if (idmap!=ID_NONE)
	{
		SetSynMapID(idmap);
		MapManager()->AddSynMap(pMap);
		LuaScriptMachine()->InitVar(GetID());
		LuaScriptMachine()->RunScriptFile("action/bp_02.lua");
	}
	else
	{
		pMap->ReleaseByOwner();
		LogSave("ERROR: ���ɵ�ͼ����ʧ��! idSyn=%u",GetID());
	}
	pRes2->Release();
	CMsgSyndicate msgSyndicate;
	msgSyndicate.Create(SYN_SETSYNMAPID,GetID(),"",GetSynMapID(),"");
	MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSyndicate,0);
}
//////////////////////////////////////////////////////////////////////
// bool CSyndicate::XSynModify::Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave)	
// {
// 	MESSAGESTR	buf;
// 	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// 	pInfo->idSyn		= This()->GetID();
// 	pInfo->ucFuncType	= SYNFUNC_DEMISE;
// 	pInfo->IntParam[0]	= idOldLeader;
// 	pInfo->IntParam[1]	= idNewLeader;
// 	pInfo->IntParam[2]	= idBackSyn;
// 	pInfo->IntParam[3]	= nOldSynMemberLevel;
// 	pInfo->IntParam[4]	= nNewSynMemberLevel;
// 	SafeCopy((char*)(&(pInfo->IntParam[5])), szNewLeader, _MAX_NAMESIZE);
// 	pInfo->ucSaveFlag		= (unsigned char)bSave;
// 	pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int)*3 + _MAX_NAMESIZE;
// 
// 	MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
// 	return true;
// }
// 
// //////////////////////////////////////////////////////////////////////
// bool CSyndicate::XSynModify::SetTenet(LPCTSTR szTenet, bool bSave)
// {
// 	MESSAGESTR	buf;
// 	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// 	pInfo->idSyn		= This()->GetID();
// 	pInfo->dwData[0]	= DateStamp();
// 	pInfo->ucFuncType	= SYNFUNC_SETTENET;
// 	SafeCopy(pInfo->StrParam, szTenet, MAX_TENETSIZE);
// 	pInfo->ucSaveFlag		= (unsigned char)bSave;
// 	pInfo->nSize		= sizeof(SynFuncInfo0) + MAX_TENETSIZE;
// 
// 	MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
// 	return true;
// }
// 
// //////////////////////////////////////////////////////////////////////
// bool CSyndicate::XSynModify::SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave)
// {
// 	MESSAGESTR	buf;
// 	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// 	pInfo->idSyn		= This()->GetID();
// 	pInfo->ucFuncType	= SYNFUNC_SETLEADERTITLE;
// 	SafeCopy(pInfo->StrParam, szLeaderTitle, _MAX_NAMESIZE);
// 	pInfo->ucSaveFlag		= (unsigned char)bSave;
// 	pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
// 
// 	MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
// 	return true;
// }
// 
// //////////////////////////////////////////////////////////////////////
// bool CSyndicate::XSynModify::SetMemberTitle(LPCTSTR szMemberTitle, bool bSave)
// {
// 	MESSAGESTR	buf;
// 	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// 	pInfo->idSyn		= This()->GetID();
// 	pInfo->ucFuncType	= SYNFUNC_SETMEMBERTITLE;
// 	SafeCopy(pInfo->StrParam, szMemberTitle, _MAX_NAMESIZE);
// 	pInfo->ucSaveFlag		= (unsigned char)bSave;
// 	pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
// 
// 	MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
// 	return true;
// }
// 
// //////////////////////////////////////////////////////////////////////
// bool CSyndicate::XSynModify::AddData(SYNDATA idx, int nData, bool bSave)
// {
// 	MESSAGESTR	buf;
// 	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// 	pInfo->idSyn		= This()->GetID();
// 	pInfo->ucFuncType	= SYNFUNC_ADDDATA;
// 	pInfo->IntParam[0]	= idx;
// 	pInfo->IntParam[1]	= nData;
// 	pInfo->ucSaveFlag	= (unsigned char)bSave;
// 	pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int)*2;
// 
// 	if (idx == SYNDATA_MONEY || idx == SYNDATA_REPUTE )
// 	{
// 		CSyndicate* pSyn = This()->GetMasterSyn();
// 		IF_OK (pSyn)
// 			pInfo->idSyn = pSyn->GetID();					// ����ϲ�������Ҳ�ϲ�
// 	}
// 	MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
// 	return true;
// }
// 
// //////////////////////////////////////////////////////////////////////
// bool CSyndicate::XSynModify::SetData(SYNDATA idx, int nData, bool bSave)
// {
// 	MESSAGESTR	buf;
// 	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// 	pInfo->idSyn		= This()->GetID();
// 	pInfo->ucFuncType	= SYNFUNC_SETDATA;
// 	pInfo->IntParam[0]	= idx;
// 	pInfo->IntParam[1]	= nData;
// 	pInfo->ucSaveFlag	= (unsigned char)bSave;
// 	pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int)*2;
// 
// 	ASSERT(idx != SYNDATA_MONEY);
// 	ASSERT(idx != SYNDATA_REPUTE);
// 	MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
// 	return true;
// }
// 
// //////////////////////////////////////////////////////////////////////
// bool CSyndicate::XSynModify::PresentMoney(OBJID idSyn, int nMoney, bool bSave)
// {
// 	MESSAGESTR	buf;
// 	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// 	pInfo->idSyn		= This()->GetID();
// 	pInfo->ucFuncType	= SYNFUNC_PRESENT;
// 	pInfo->IntParam[0]	= idSyn;
// 	pInfo->IntParam[1]	= nMoney;
// 	pInfo->ucSaveFlag	= (unsigned char)bSave;
// 	pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int)*2;
// 
// 	MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
// 	return true;
// }
// 
// //////////////////////////////////////////////////////////////////////
// bool CSyndicate::XSynModify::SetSynName(LPCTSTR szSynName, bool bSave)
// {
// 	MESSAGESTR	buf;
// 	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// 	pInfo->idSyn		= This()->GetID();
// 	pInfo->ucFuncType	= SYNFUNC_SETSYNNAME;
// 	SafeCopy(pInfo->StrParam, szSynName, _MAX_NAMESIZE);
// 	pInfo->ucSaveFlag		= (unsigned char)bSave;
// 	pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
// 	
// 	MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
// 	return true;
// }
// //---------------------------------------------
// //20070414����:������
// bool CSyndicate::XSynModify::PayOff(DWORD dwMoney,BOOL bSave)//bSave=true
// {
// 	MESSAGESTR	buf;
// 	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// 	pInfo->idSyn		= This()->GetID();
// 	pInfo->ucFuncType	= SYNFUNC_PAYOFF;
// 	pInfo->IntParam[0]	= dwMoney;//����
// 	pInfo->ucSaveFlag	= (unsigned char)bSave;
// 	pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int);
// 
// 	MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
// 	return true;
// }
// //----------------------------------------------
// //20070417����:ͬ�������߳��ڳ�Ա����ֵ
// bool CSyndicate::XSynModify::SetMemberProffer(int dwProffer,OBJID idMember,BOOL bSave/* =true */)
// {
// 	MESSAGESTR	buf;
// 	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// 	pInfo->idSyn		= This()->GetID();
// 	pInfo->ucFuncType	= SYNFUNC_SET_MEMBERPROFFER;
// 	pInfo->IntParam[0]	= dwProffer;//����ֵ
// 	pInfo->IntParam[1]	= idMember;//��Աid
// 	pInfo->ucSaveFlag	= (unsigned char)bSave;
// 	pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int)*2;
// 
// 	MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
// 	return true;
// }

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
int	CSyndicate::GetInt(SYNDATA idx)
{
	return m_pData->GetInt(idx);		// no loop
}


//////////////////////////////////////////////////////////////////////
// �ڱ����̹߳㲥�Ӱ�����Ϣ
// void CSyndicate::BroadcastLocalSynMsg(CNetMsg* pMsg, CUser* pExcludeSender)
// {
// 	UINT		usAction=0;
// 	UINT		nMsgType=0;
// 	try
// 	{
// 		CHECK(pMsg);
// 		nMsgType=pMsg->GetType();
// 		memcpy(&usAction,pMsg->GetBuf(),sizeof(UINT));
// 		CUserManager::Iterator pUser = UserManager()->NewEnum();
// 		while(pUser.Next())
// 		{
// 			if(pUser && pUser->CanBeSee()==false)
// 				continue;
// 
// 		//	pUser->GetID();
// 			if(pUser && pUser->GetSynID() == GetID() 
// 					/*&& !(pExcludeSender && pExcludeSender->GetID() == pUser->GetID())*/ )
// 			{
// 				pUser->SendMsg(pMsg);
// 			}
// 		}
// 	}
// 	catch(...)
// 	{
// 		LogSave("CSyndicate::BroadcastLocalSynMsg  crash()! msgtype:%d,action:%d",pMsg->GetType(),usAction);
// 		if(nMsgType==_MSG_TALK)
// 		{
// 			///debug
// 			CMsgTalk::Dump((char*)pMsg->GetBuf(),pMsg->GetSize());
// 		}
// 	}
// }

void CSyndicate::BroadcastLocalSynMsg(CNetMsg* pMsg, CUser* pExcludeSender)
{
	UINT		usAction=0;
	UINT		nMsgType=0;
	OBJID		idUser=0;
	int			nCatchValue=0;
	CUserPtr	pUser=NULL;
	try
	{
		CHECK(pMsg);
		nMsgType=pMsg->GetType();
		memcpy(&usAction,pMsg->GetBuf(),sizeof(UINT));
		nCatchValue=1;
		for(int i = 0 ; i < m_setMember.size() ; i ++)
		{
			idUser=m_setMember[i].id;
			nCatchValue=2;
			pUser=UserManager()->GetUser(idUser);
			if(pUser)
			{
				///���Ե��ų����û�
				if(pExcludeSender && pExcludeSender->GetID()==pUser->GetID())
					continue;

				nCatchValue=3;
				if(pUser->CanBeSee()==false)
					continue;

				if(pUser->GetSynID()!=GetID())
				{
					LogSave("CSyndicate::BroadcastLocalSynMsg error!���Ը�һ�����ڱ������ҷ��ͳ�Ա��������Ϣ,��m_setMember��������!");
					continue;
				}
				nCatchValue=4;
 				pUser->SendMsg(pMsg);
			}
		}
		nCatchValue=5;
// 		USERNAME_SET& usernameset=UserManager()->GetUserNameSet();
// 		for(USERNAME_SET::iterator it=usernameset.begin();it!=usernameset.end();++it)
// 		{
// 			idUser=it->second;
// 			pUser=UserManager()->GetUser(idUser);
// 			if(pUser)
// 			{
// 				if(pUser->CanBeSee()==false)
// 					continue;
// 				if(pUser->GetSynID() != GetID())
// 					continue;
// 
// 				pUser->SendMsg(pMsg);
// 			}
// 		}
	}
	catch(...)
	{
		LogSave("CSyndicate::BroadcastLocalSynMsg  crash()! msgtype:%d,action:%d,nCatchValue:%d",pMsg->GetType(),usAction,nCatchValue);
		if(nMsgType==_MSG_TALK)
		{
			///debug
			CMsgTalk::Dump((char*)pMsg->GetBuf(),pMsg->GetSize());
		}
	}
}

//////////////////////////////////////////////////////////////////////
// �ڱ��߳��й㲥�ܰ�����Ϣ(���Ӱ���)
void CSyndicate::BroadcastSynMsg(CNetMsg* pMsg, CUser* pExcludeSender)
{
	DEBUG_TRY
	BroadcastLocalSynMsg(pMsg, pExcludeSender);
	DEBUG_CATCH("CSyndicate::BroadcastSynMsg");

//	MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pMsg, ALLUSERS_NAME);

// 	CSyndicate* pMaster = this->GetMasterSyn();
// 	CHECK(pMaster);
// 	pMaster->BroadcastLocalSynMsg(pMsg, pExcludeSender);
// 
// 	{
// 		OBJID idMaster = pMaster->GetID();
// 		CHECK(idMaster != ID_NONE);
// 		for(int i = 0; i < SynManager()->QuerySynSet()->GetAmount(); i++)
// 		{
// 			CSynPtr pSyn = SynManager()->QuerySynSet()->GetObjByIndex(i);
// 			if(pSyn && pSyn->GetInt(SYNDATA_ID) == idMaster)
// 			{
// 				pSyn->BroadcastLocalSynMsg(pMsg, pExcludeSender);
// 			}
// 		}
// 	}
}

//////////////////////////////////////////////////////////////////////
// �������й㲥�ܰ�����Ϣ(���Ӱ���)
void CSyndicate::BroadcastSubSynMsg(SOCKET_ID idSocket, OBJID idNpc, LPCTSTR szMsg, CUser* pExcludeSender, LPCTSTR pszEmotion, DWORD dwColor, int nAttribute)
{
	CHECK(szMsg);
	CMsgTalk	msg;
	IF_OK(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, pszEmotion, dwColor, nAttribute))
		BroadcastSynMsg(&msg, pExcludeSender);

	MapGroup(PID)->QueryIntraMsg()->TransmitMsg(&msg, idSocket, idNpc);
}
void CSyndicate::TransmitWorldMsg(CNetMsg *msg)//ȫ��ת��
{
	MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(msg,0);
}
//////////////////////////////////////////////////////////////////////
LPCTSTR CSyndicate::GetFealtyName()
{
	return SYNNAME_NONE;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::IsNearSyn(OBJID idTarget)
{
	if(idTarget == ID_NONE)
		return false;

	if(GetID() == idTarget)
		return true;

	CSyndicate* pTarget = SynManager()->QuerySyndicate(idTarget);
	if(!pTarget)		// �������ӵİ���ID������Ч
		return false;


	return false;
}

//////////////////////////////////////////////////////////////////////
CSyndicate* CSyndicate::GetMasterSyn()
{
	return this;
}

//////////////////////////////////////////////////////////////////////
void CSyndicate::SendInfoToClient(CUser* pUser)
{
	CHECK(pUser);

	CMsgSynInfo msg;
	if(msg.Create(this))
		pUser->SendMsg(&msg);
}
void CSyndicate::SendSynInfoToClient(CUser* pUser)
{
	CHECK(pUser);
	CMsgSynInfo msg;
	if(msg.CreateInfo(this))
		pUser->SendMsg(&msg);
}
void CSyndicate::SendAllInfoToClient(CUser* pUser)
{
	CHECK(pUser);
	CMsgSynInfo msg;
	if(msg.CreateNew(this,pUser))
		pUser->SendMsg(&msg);
}
// void CSyndicate::SendQueryMenberToClient(CUser* pUser,int nPage)
// {
// 	CHECK(pUser);
// 	
// 	CMsgSynMemberInfo msg;
// 	if (!msg.Create(2,nPage,GetQueryMemberAmount()))
// 		return;
// 	
// 	bool bUpdate = false;
// 	if(dwLastUpdate - clock() > 300000)
// 	{
// 		dwLastUpdate = clock();
// 		bUpdate = true;
// 	}
// 	if(bUpdate)
// 	{
// 		for (int i = 0 ; i < GetQueryMemberAmount() ; i ++)
// 		{
// 			CUserPtr pUser = UserManager()->GetUser(m_setQueryMember[i].id);
// 			if(pUser && pUser->QuerySynAttr())
// 			{
// 				m_setQueryMember[i].level = pUser->GetLev();
// 				m_setQueryMember[i].firJob = pUser->GetProfession(1);
// 			//	m_setQueryMember[i].firLevel = pUser->GetJobLev(1);
// 				m_setQueryMember[i].rank = pUser->QuerySynAttr()->GetSynRank();
// 			//	m_setQueryMember[i].nProffer = pUser->QuerySynAttr()->GetProffer();
// 			//	m_setQueryMember[i].nOnline = 1;
// 			}
// 			else
// 			{
// //				m_setQueryMember[i].nOnline = 0;
// 			}
// 		}
// 	}
// 
// 	for(int i = (nPage - 1) * 14 ; i < nPage * 14 ; i ++)
// 	{
// 		if(i < 0 || i >= GetQueryMemberAmount())
// 			break;
// 		msg.Append(m_setQueryMember[i].nId,m_setQueryMember[i].nLevel,m_setQueryMember[i].firJob,m_setQueryMember[i].firLevel,m_setQueryMember[i].nRank,m_setQueryMember[i].nProffer,m_setQueryMember[i].szName,m_setQueryMember[i].nOnline);
// 	}
// 	
// 	pUser->SendMsg(&msg);
// }


// void CSyndicate::SendMenberToClient(CUser* pUser,int nPage)
// {
// 	CHECK(pUser);
// 	
// 	CMsgSynMemberInfo msg;
// 	if (!msg.Create(1,nPage,GetMemberAmount()))
// 		return;
// 	
// 	bool bUpdate = false;
// 	if(dwLastUpdate - clock() > 300000)
// 	{
// 		dwLastUpdate = clock();
// 		bUpdate = true;
// 	}
// 	if(bUpdate)
// 	{
// 		for (int i = 0 ; i < GetMemberAmount() ; i ++)
// 		{
// 			CUserPtr pUser = UserManager()->GetUser(m_setMember[i].nId);
// 			if(pUser && pUser->QuerySynAttr())
// 			{
// 				m_setMember[i].nLevel = pUser->GetLev();
// 				m_setMember[i].firJob = pUser->GetProfession(1);
// 				m_setMember[i].firLevel = pUser->GetJobLev(1);
// 				m_setMember[i].nRank = pUser->QuerySynAttr()->GetSynRank();
// 				m_setMember[i].nProffer = pUser->QuerySynAttr()->GetProffer();
// 				m_setMember[i].nOnline = 1;
// 			}
// 			else
// 			{
// 	//			m_setMember[i].nOnline = 0;
// 			}
// 		}
// 	}
// 	
// 	for(int i = (nPage - 1) * 10 ; i < nPage * 10 ; i ++)
// 	{
// 		if(i < 0 || i >= GetMemberAmount())
// 			break;
// 		msg.Append(m_setMember[i].nId,m_setMember[i].nLevel,m_setMember[i].firJob,m_setMember[i].firLevel,m_setMember[i].nRank,m_setMember[i].nProffer,m_setMember[i].szName,m_setMember[i].nOnline);
// 	}
// 	
// 	pUser->SendMsg(&msg);
// }
void CSyndicate::GetMemberList(vector<CUserPtr> &User)
{
	User.clear();
	for (int i = 0 ; i < GetMemberAmount() ; i ++)
	{
		CUserPtr pUser = UserManager()->GetUser(m_setMember[i].id);
		if(pUser && pUser->QuerySynAttr() && pUser->GetSynID() == GetID())
		{	
			User.push_back(pUser);	
			int temp=User.size();
		}
	}

}
void CSyndicate::ReSetSynName(const char* szName)
{
	SetStr(SYNDATA_NAME,szName,_MAX_NAMESIZE,true);
}
void CSyndicate::ReSetSynMemberName(OBJID nUserid,const char* szName)
{
	for (int i=0;i<m_setMember.size();i++)
	{
		if (m_setMember[i].id==nUserid)
		{
			SafeCopy(m_setMember[i].szName,szName,_MAX_NAMESIZE);
			if (m_setMember[i].id==idLeader)
			{
				if (strcmp(GetStr(SYNDATA_CREATELEADERNAME),GetStr(SYNDATA_LEADERNAME))==0)
				{
					SetStr(SYNDATA_CREATELEADERNAME,szName,_MAX_NAMESIZE,true);
				}
				SetStr(SYNDATA_LEADERNAME,szName,_MAX_NAMESIZE,true);
			}
			return;
		}
	}
}
void CSyndicate::SendMenberListToClient(CUser* pUser)//���������õ�
{
	CHECK(pUser);
	
	CMsgSynMemberInfo msg;
	if (!msg.Create(MemberList,1,GetMemberAmount()))
		return;
	
	bool bUpdate = false;
	if(dwLastUpdate - clock() > 300000)
	{
		dwLastUpdate = clock();
		bUpdate = true;
	}
	int nNowSend=0;
	if(bUpdate)
	{
		for (int i = 0 ; i < GetMemberAmount() ; i ++)
		{
			CUserPtr pUser = UserManager()->GetUser(m_setMember[i].id);
			if(pUser && pUser->QuerySynAttr() && pUser->GetSynID() == GetID())
			{	
				m_setMember[i].level = pUser->GetLev();
				m_setMember[i].firJob = pUser->GetProfession(1);
				m_setMember[i].rank = pUser->GetMySynRank();
				m_setMember[i].hiscon = pUser->GetHistorySynCon();
				m_setMember[i].hismoney =pUser->GetHistorySynMoney();
				m_setMember[i].hisgrow = pUser->GetHistorySynGrow();
				m_setMember[i].lastlogin=pUser->GetLastLogin();
				if (pUser->IsVipActive())
					m_setMember[i].vip = pUser->GetVipLev();
				else
					m_setMember[i].vip = 0;
			}
			else
			{
				//m_setMember[i].line = 0;
			}
		}
	}
	
	for(int i =0 ; i < GetMemberAmount() ; i ++)
	{
		msg.Append(m_setMember[i].id,m_setMember[i].szName,m_setMember[i].level,m_setMember[i].firJob,m_setMember[i].sex,m_setMember[i].rank,
					m_setMember[i].line,m_setMember[i].vip,m_setMember[i].lastlogin,m_setMember[i].hiscon,m_setMember[i].hismoney,m_setMember[i].hisgrow,m_setMember[i].maxpower);
		nNowSend ++;
		if(nNowSend > 10)
		{
			pUser->SendMsg(&msg);
			if (!msg.Create(MemberList,1,GetMemberAmount()))
				return;
			nNowSend = 0;
		}
	}
	if(nNowSend != 0)
		pUser->SendMsg(&msg);
}


void CSyndicate::SendQueryListToClient(CUser* pUser,int nPage)//���������õ�
{
	CHECK(pUser);
	CMsgSynMemberInfo msg;
	if (!msg.Create(QueryList,nPage,GetQueryMemberAmount()))
		return;
	int nNowSend=0;
	bool bUpdate = false;
	if(dwLastUpdate - clock() > 300000)
	{
		dwLastUpdate = clock();
		bUpdate = true;
	}
	if(bUpdate)
	{
		for (int i = 0 ; i < GetQueryMemberAmount() ; i ++)
		{
			CUserPtr pUser = UserManager()->GetUser(m_setQueryMember[i].id);
			if(pUser && pUser->QuerySynAttr() && pUser->GetSynID() == GetID())
			{
				m_setQueryMember[i].level = pUser->GetLev();
				m_setQueryMember[i].firJob = pUser->GetProfession(1);
				m_setQueryMember[i].maxpower=pUser->GetMaxScore();
				if (pUser->IsVipActive())
					m_setMember[i].vip = pUser->GetVipLev();
				else
					m_setMember[i].vip = 0;
			}
			else
			{
				//m_setQueryMember[i].nOnline = 0;
			}
		}
	}
	
	for(int i = 0; i < GetQueryMemberAmount() ; i ++)
	{
		msg.Append(m_setQueryMember[i].id,m_setQueryMember[i].szName,m_setQueryMember[i].level,m_setQueryMember[i].firJob,m_setQueryMember[i].sex,
					m_setQueryMember[i].rank,m_setQueryMember[i].line,m_setQueryMember[i].vip,m_setQueryMember[i].lastlogin,0,0,0,m_setQueryMember[i].maxpower);
		nNowSend++;
		if (nNowSend>10)
		{
			pUser->SendMsg(&msg);
			if (!msg.Create(QueryList,nPage,GetQueryMemberAmount()))
				return;
			nNowSend=0;
		}
	}
	if(nNowSend != 0)
		pUser->SendMsg(&msg);
}

void CSyndicate::BroadcastSynMemberStatusChangeMsg(OBJID idMember)
{
	if (this->GetInt(SYNDATA_STATE) == 3)
		return;
	char szWord[1024]="";
	::MemberInfo* info =GetMemberFromList(idMember,1);
	if(info)
	{
		CUser* pExcludeSender=UserManager()->GetUser(idMember);
		if(info->line != 0)
			sprintf(szWord, "���ɳ�Ա%s������!", info->szName);
		else
			sprintf(szWord, "���ɳ�Ա%s������!", info->szName);
		CMsgTalk msg;
		if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_TOPSHOW))
			BroadcastSynMsg(&msg,pExcludeSender );///��Ҫ���������û�
	}
}

void CSyndicate::BroadcastSynMemberInfoMsg(OBJID idMember)
{
	DEBUG_TRY
	CMsgSynMemberInfo msg;
	if(msg.Create(4,0,1))
	{
		for (int i = 0 ; i < GetMemberAmount() ; i ++)
		{
			MemberInfo &info=m_setMember[i];
			if(info.id==idMember)
			{
				CUserPtr pUser = UserManager()->GetUser(info.id);
				if (pUser)
				{
					if(pUser->QuerySynAttr() && pUser->QuerySynAttr()->GetSynID() == GetID())
					{
						info.level = pUser->GetLev();
						info.firJob = pUser->GetProfession(1);
						info.rank = pUser->QuerySynAttr()->GetSynRank();
						info.hiscon=pUser->QuerySynAttr()->GetHistoryCon();
						info.hismoney=pUser->QuerySynAttr()->GetHistoryMoney();
						info.hisgrow=pUser->QuerySynAttr()->GetHistoryGrow();
						if (pUser->IsVipActive())
							info.vip = pUser->GetVipLev();
						else
							info.vip = 0;
						info.lastlogin = pUser->GetLastLogin();
					}
				}
					msg.Append(info.id,info.szName,info.level,info.firJob,info.sex,info.rank,
					info.line,info.vip,info.lastlogin,info.hiscon,info.hismoney,info.hisgrow,info.maxpower);
				BroadcastSynMsg(&msg );
				break;
			}
		}

	}
	BroadcastSynMemberStatusChangeMsg(idMember);
	DEBUG_CATCH2("BroadcastSynMemberInfoMsg idMember:%u",idMember);
}

// void CSyndicate::SendMenberToClient(CUser* pUser,int nPage)
// {
// 	CHECK(pUser);
// 	
// 	CMsgSynMemberInfo msg;
// 	if (!msg.Create(1,nPage,GetMemberAmount()))
// 		return;
// 
// 	bool bUpdate = false;
// 	if(dwLastUpdate - clock() > 300000)
// 	{
// 		dwLastUpdate = clock();
// 		bUpdate = true;
// 	}
// 	if(bUpdate)
// 	{
// 		for (int i = 0 ; i < GetMemberAmount() ; i ++)
// 		{
// 			CUserPtr pUser = UserManager()->GetUser(m_setMember[i].nId);
// 			if(pUser && pUser->QuerySynAttr())
// 			{
// 				m_setMember[i].nLevel = pUser->GetLev();
// 				m_setMember[i].firJob = pUser->GetProfession(1);
// 				m_setMember[i].firLevel = pUser->GetJobLev(1);
// 				m_setMember[i].nRank = pUser->QuerySynAttr()->GetSynRank();
// 				m_setMember[i].nProffer = pUser->QuerySynAttr()->GetProffer();
// 				m_setMember[i].nOnline = 1;
// 			}
// 			else
// 			{
// //				m_setMember[i].nOnline = 0;
// 			}
// 		}
// 	}
// 
// 	for(int i = (nPage - 1) * 10 ; i < nPage * 10 ; i ++)
// 	{
// 		if(i < 0 || i >= GetMemberAmount())
// 			break;
// 		msg.Append(m_setMember[i].nId,m_setMember[i].nLevel,m_setMember[i].firJob,m_setMember[i].firLevel,m_setMember[i].nRank,m_setMember[i].nProffer,m_setMember[i].szName,m_setMember[i].nOnline);
// 	}
// 
// 	pUser->SendMsg(&msg);
// }

//////////////////////////////////////////////////////////////////////
// syn war -------------------------------------
//////////////////////////////////////////////////////////////////////
void CSyndicate::AddSynWarScore(OBJID idNpc, int nScore)
{
	for(int i = 0; i < m_setSynWarScore.size(); i++)
	{
		if(m_setSynWarScore[i].idNpc == idNpc)
		{
			m_setSynWarScore[i].nScore	+= nScore;
			return;
		}
	}

	SynWarInfo	info;
	info.idNpc		= idNpc;
	info.nScore		= nScore;
	m_setSynWarScore.push_back(info);
}

//////////////////////////////////////////////////////////////////////
int CSyndicate::GetSynWarScore(OBJID idNpc)
{
	for(int i = 0; i < m_setSynWarScore.size(); i++)
	{
		if(m_setSynWarScore[i].idNpc == idNpc)
			return m_setSynWarScore[i].nScore;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
void CSyndicate::ClearSynWarScore(OBJID idNpc)
{
	for(int i = 0; i < m_setSynWarScore.size(); i++)
	{
		if(m_setSynWarScore[i].idNpc == idNpc)
		{
			m_setSynWarScore.erase(m_setSynWarScore.begin() + i);
			return ;
		}
	}
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::IsFriendly(OBJID idTarget, BOOL bNest)
{
	if(idTarget == ID_NONE)
		return false;
	CSyndicate* pTarget = SynManager()->QuerySyndicate(idTarget);
	CHECKF(pTarget);
	idTarget = pTarget->GetMasterSynID();



	return false;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::IsHostile(OBJID idTarget, BOOL bNest)
{
	CSyndicate* pTarget = SynManager()->QuerySyndicate(idTarget);
	idTarget = pTarget->GetMasterSynID();



	
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::IsNeutral(OBJID idSyn)
{
	if(!IsFriendly(idSyn) && !IsHostile(idSyn))
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////
//06.4.24�����޸�
//��Ϊʱ��֮�������Ӱ���ϵͳ,���޸ĳ�����
int CSyndicate::GetMemberAmount(bool bIncludeSub)
{
	return m_setMember.size();
}

int CSyndicate::GetOnlineAmount()
{
	int nOnlineAmount=0;
	for(int i = 0; i < m_setMember.size() ; i ++)
	{
		MemberInfo& info=m_setMember[i];
		if(info.line != 0)
			nOnlineAmount++;
	}
	return nOnlineAmount;
}

int CSyndicate::GetShopMemberAmount(bool bIncludeSub)
{
	int nShpAmount=0;
	for(int i = 0; i < m_setMember.size() ; i ++)
	{
		MemberInfo& info=m_setMember[i];
		if(info.rank == RANK_SHOP)
			nShpAmount++;
	}
	return nShpAmount;
}

int CSyndicate::GetQueryMemberAmount(bool bIncludeSub)
{
	return m_setQueryMember.size();
}

//////////////////////////////////////////////////////////////////////
//06.4.24�����޸�
//��Ϊʱ��֮�������Ӱ���ϵͳ,���޸ĳ�����
//�˺���Ҳ�û�þ�����Ա����
int CSyndicate::GetSynAmount()
{

	return GetInt(SYNDATA_AMOUNT);
/*	if(!IsMasterSyn())
	{
		CSyndicate * pMainSyn = this->GetMasterSyn();
		CHECKF(pMainSyn);
		return pMainSyn->GetMemberAmount(true);
	}	

	return GetMemberAmount(true);*/
}

//////////////////////////////////////////////////////////////////////
void CSyndicate::SetNewSubName(LPCTSTR szName)		// ��ʱ����
{
	m_pData->SetStr(SYNDATA_NAME, szName, _MAX_NAMESIZE);
	m_pData->Update();
}

//////////////////////////////////////////////////////////////////////@
bool CSyndicate::IsWhiteSynMapOwner()
{
	return false;/*GetID() == SynManager()->GetMapSynID(SET_WHITE_SYN)*/;
}

//////////////////////////////////////////////////////////////////////@
bool CSyndicate::IsBlackSynMapOwner()
{
	return false/*GetID() == SynManager()->GetMapSynID(SET_BLACK_SYN)*/;
}

//////////////////////////////////////////////////////////////////////@
bool CSyndicate::ClearAllySyn(CSyndicate* pTargetSyn)
{
	// ������
	CHECKF (pTargetSyn);
	

// 	CMsgSyndicate	msg;
// 	IF_OK(msg.Create(CLEAR_ALLY, pTargetSyn->GetID()))
// 		MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(GetID(), &msg);

	return true;
}

//////////////////////////////////////////////////////////////////////@
bool CSyndicate::ClearEnemySyn(CSyndicate* pTargetSyn)
{
	// ������
	CHECKF (pTargetSyn);


// 	CMsgSyndicate	msg;
// 	IF_OK(msg.Create(CLEAR_ANTAGONIZE, pTargetSyn->GetID()))
// 		MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(GetID(), &msg);

	return true;
}
// void CSyndicate::GetMemberList(vector<CUserPtr> &User)
// {
// 	User.clear();
// 	for (int i = 0 ; i < GetMemberAmount() ; i ++)
// 	{
// 		CUserPtr pUser = UserManager()->GetUser(m_setMember[i].id);
// 		if(pUser && pUser->QuerySynAttr())
// 		{	
// 			User.push_back(pUser);	
// 			int temp=User.size();
// 		}
// 	}
// 	
// }
// void CSyndicate::ReSetSynName(const char* szName)
// {
// 	SetStr(SYNDATA_NAME,szName,_MAX_NAMESIZE,true);
// }
// void CSyndicate::ReSetSynMemberName(OBJID nUserid,const char* szName)
// {
// 	for (int i=0;i<m_setMember.size();i++)
// 	{
// 		if (m_setMember[i].id==nUserid)
// 		{
// 			SafeCopy(m_setMember[i].szName,szName,_MAX_NAMESIZE);
// 			if (m_setMember[i].id==idLeader)
// 			{
// 				if (strcmp(GetStr(SYNDATA_CREATELEADERNAME),GetStr(SYNDATA_LEADERNAME))==0)
// 				{
// 					SetStr(SYNDATA_CREATELEADERNAME,szName,_MAX_NAMESIZE,true);
// 				}
// 				SetStr(SYNDATA_LEADERNAME,szName,_MAX_NAMESIZE,true);
// 			}
// 			return;
// 		}
// 	}
// }
//////////////////////////////////////////////////////////////////////@
// add huang 2004.1.7
bool CSyndicate::CheckDismissSyn()
{
	return false;
/*	if(this->IsFallLevel())
		return true;
	else
		return false;*/

	/*-----------����ע������*/
/*	bool bCheck = false;
	CMsgTalk msg;
	if(GetInt(SYNDATA_MONEY) < DISMISSSYN_MONEY_LIMIT)
	{
		IF_OK(msg.Create(_TXTATR_SYNDICATE, STR_SYN_MONEY_LIMIT))
			BroadcastSynMsg(&msg);
		bCheck = true;
	}
	if(GetInt(SYNDATA_AMOUNT) < DISMISSSYN_AMOUNT_LIMIT)
	{
		IF_OK(msg.Create(_TXTATR_SYNDICATE, STR_SYN_AMOUNT_LIMIT))
			BroadcastSynMsg(&msg);
		bCheck = true;
	}
	if(!bCheck)
		return false;
	return true;
	*/
}

bool CSyndicate::CheckDismissTime()
{
	time_t tTime = time(NULL);
// 	if(GetInt(SYNDATA_DISMISS_TIME) < tTime - DISMISSSYN_TIME_LIMIT)
// 		return true;
	return false;
}

void CSyndicate::ProcAutoSynWar()
{
// 	if(!m_pData)
// 		return;
// 	char startTime[56];
// 	int nYear=0,nMonth=0,nDay=0,nHour=0,nMinute=0;
// 	strcpy(startTime,GetStr(SYNDATA_WARSTARTTIME));	
// 	int id = GetInt(SYNDATA_WARID);
// 	CSyndicate* enSyn = SynManager()->QuerySyndicate(id);
// 
// 	if(enSyn && strlen(startTime)>0 )
// 	{
// 		char szBuf[300]	= "";
// 		char myName[16],enName[16];
// 		int myKill = GetKill();
// 		int enKill = enSyn->GetKill();
// 		strcpy(myName,GetStr(SYNDATA_NAME));
// 		strcpy(enName,enSyn->GetStr(SYNDATA_NAME));
// 
// 		sscanf(startTime,"%d-%d-%d-%d-%d",&nYear,&nMonth,&nDay,&nHour,&nMinute);
// 		SYSTEMTIME localTime;
// 		::GetLocalTime( &localTime );
// //		if(localTime.wYear != nYear  || localTime.wMonth != nMonth || localTime.wDay != nDay)
// //			return;
// 		int allday = 0;			
// 		if(localTime.wMonth == nMonth + 1  || (nMonth == 12 && localTime.wMonth == 1) 
// 			||localTime.wDay == nDay + 1 || (localTime.wDay == 1 && nDay > 27))
// 		{		
// 			switch(nMonth)
// 			{
// 			case 0:
// 				break;
// 			case 1:
// 			case 3:
// 			case 5:
// 			case 7:
// 			case 8:
// 			case 10:
// 			case 12:
// 				allday = 31;
// 				break;
// 			case 2:
// 				if(nYear % 4 == 0)
// 				{
// 					allday = 29;				
// 				}
// 				else
// 					allday = 28;
// 				break;
// 			case 4:
// 			case 6:
// 			case 9:
// 			case 11:
// 				allday = 30;
// 				break;
// 			}			
// 			allday -= nDay - localTime.wDay;
// 			if(allday > 1 && allday < 0)
// 				return;
// 		}
// 		else if(localTime.wMonth != nMonth || localTime.wYear != nYear || localTime.wDay != nDay)
// 		{
// 			return;
// 		}
// 		
// 		DWORD offtime = 0;		
// 		offtime = allday*24*60 +
// 			(localTime.wHour-nHour)*60 + 
// 			localTime.wMinute - nMinute;
// 		if(offtime >= /*60*/4 && warState != 0)
// 		{				
// 			warState = 0;
// 
// 			char sendTemp[128];
// 			sprintf(sendTemp,"%d %d",this->GetID(),warState);
// 			CMsgSyndicate msg;
// 			msg.Create(SYN_WAR_SET,id,sendTemp);
// 			CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_FIRST + 1,&msg);
// 
// 			if(GetKill() >= enSyn->GetKill())
// 			{				
// 				sprintf(szBuf, "%s���Լս%s����ս����������!",myName,enName);
// 				UserManager()->BroadcastMsg(szBuf, NULL, NULL, 0xffffff, _TXTATR_SYSTEM);
// 				sprintf(szBuf, "%s��Ṳɱ��%d�ˣ�%s��Ṳɱ��%d��!",myName,myKill,enName,enKill);
// 				UserManager()->BroadcastMsg(szBuf, NULL, NULL, 0xffffff, _TXTATR_SYSTEM);
// 				if(GetKill() > enSyn->GetKill())
// 				{
// 					sprintf(szBuf, "%s���ʤ��!",myName);
// 					UserManager()->BroadcastMsg(szBuf, NULL, NULL, 0xffffff, _TXTATR_SYSTEM);
// 
// 					int enReput = enSyn->GetInt(SYNDATA_REPUTE);
// 					int lev = enSyn->GetInt(SYNDATA_RANK);
// 					int synHonor = lev * 1000;
// 					if(synHonor > enReput)
// 					{
// 						synHonor = enReput;
// 					}
// 
// 					if(synHonor > 0)
// 					{
// 						enSyn->SetData(SYNDATA_REPUTE,enReput - synHonor,true);
// 					}
// 
// 					int synGetHonor = synHonor * 9 / 10;
// 					if(synGetHonor < 1)
// 						synGetHonor = 1;
// 					{
// 						SetData(SYNDATA_REPUTE,GetInt(SYNDATA_REPUTE) + synGetHonor,true);
// 					}
// 				}				
// 			}			
// 		}
// 		else if(offtime >= 2 )
// 		{
// 			if(warState == 1)
// 			{
// 				warState = 2;
// 
// 				char sendTemp[128];
// 				sprintf(sendTemp,"%d %d",this->GetID(),warState);
// 				CMsgSyndicate msg;
// 				msg.Create(SYN_WAR_SET,id,sendTemp);
// 				CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_FIRST + 1,&msg);
// 
// 				SetKill(0);
// 				enSyn->SetKill(0);
// 				sprintf(szBuf, "%s����%s����Լս��ʼ!",myName,enName);
// 				UserManager()->BroadcastMsg(szBuf, NULL, NULL, 0xffffff, _TXTATR_SYSTEM);				
// 			}
// 			else if(warState == 2 )
// 			{
// 				warTurn ++ ;
// 				if(warTurn == 2)
// 				{
// 					warState = 2;	
// 					
// 					char sendTemp[128];
// 					sprintf(sendTemp,"%d %d",this->GetID(),warState);
// 					CMsgSyndicate msg;
// 					msg.Create(SYN_WAR_SET,id,sendTemp);
// 					CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_FIRST + 1,&msg);
// 
// 					sprintf(szBuf, "%s����%s����Լս���ڽ���!",myName,enName);
// 					UserManager()->BroadcastMsg(szBuf, NULL, NULL, 0xffffff, _TXTATR_SYSTEM);
// 					sprintf(szBuf, "%s��Ṳɱ��%d�ˣ�%s��Ṳɱ��%d��!",myName,myKill,enName,enKill);
// 					UserManager()->BroadcastMsg(szBuf, NULL, NULL, 0xffffff, _TXTATR_SYSTEM);
// 					warTurn = 0;
// 				}				
// 			}
// 			
// 		}
// 		else if(offtime < 2 )
// 		{
// 			warState = 1;
// 			
// 			char sendTemp[128];
// 			sprintf(sendTemp,"%d %d",this->GetID(),warState);
// 			CMsgSyndicate msg;
// 			msg.Create(SYN_WAR_SET,id,sendTemp);
// 			CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_FIRST + 1,&msg);
// 
// 			sprintf(szBuf, "%s����%s����Լս���Ͽ�ʼ��%s����Ա׼��!",myName,enName,enName,myName,enName);
// 			UserManager()->BroadcastMsg(szBuf, NULL, NULL, 0xffffff, _TXTATR_SYSTEM);
// 		}
// 		else
// 		{
// 			char sendTemp[128];
// 			sprintf(sendTemp,"%d %d",this->GetID(),warState);
// 			CMsgSyndicate msg;
// 			msg.Create(SYN_WAR_SET,id,sendTemp);
// 			CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_FIRST + 1,&msg);
// 
// 			warState = 0;
// 		}
// 		this->SynchroInfo();
// 	}
//	SYSTEMTIME localTime;
//	::GetLocalTime( &localTime );
//	int nCurTime = localTime.wYear * 10000 + localTime.wMonth * 100 + localTime.wDay;
//	if( nCurTime - GetInt( SYNDATA_WARSTARTTIME) >= AUTOCHECK_TIME )//30��(����һ���¼��ʱ��)
//
//	CSyndicate* SynManager()->QuerySyndicate(warID);

}
//�Զ��������ɢ����
bool CSyndicate::ProcAutoDismissOrDestory()
{

	SYSTEMTIME localTime;
	::GetLocalTime( &localTime );
// 	int nCurTime = localTime.wYear * 10000 + localTime.wMonth * 100 + localTime.wDay;
// 	if( nCurTime - GetInt( SYNDATA_DISMISS_TIME ) >= AUTOCHECK_TIME )//30��(����һ���¼��ʱ��)
// 	{
// 		if( IsCanFallLevel() )
// 		{
// // 			if( GetInt(SYNDATA_RANK)==SYNRANK_ONE )	//һ������
// // 			{
// // 				//SynManager()->QueryModify()->DestroySyndicate(GetID());
// // 				return true;
// // 			}
// //			QueryModify()->AddData( SYNDATA_RANK,-SYNRANK_ONE,true );//����
// //			QueryModify()->SetData( SYNDATA_DISMISS_TIME,nCurTime,true );//������һ�μ��ʱ��
// 			return true;
// 		}
// 	}
	return false;
}

int CSyndicate::ChgMemberRank(OBJID idUser,int nRank/* = 10*/,bool bSend /*= true*/,CUser* pUser/* = NULL*/)
{
	if(idUser == ID_NONE)
		return 0;

	int OldIndex = -1;
	int thisIndex = -1;

	bool needCheck = false;

	switch(nRank)
	{
//	case RANK_SUBLEADER:
	case RANK_LEADER:
		needCheck = true;
		break;
	}

	for(int i = 0; i < m_setMember.size() ; i ++)
	{
		MemberInfo& info=m_setMember[i];
		if(m_setMember[i].id == idUser)
		{
			if(m_setMember[i].rank == nRank)
				return nRank;

			thisIndex = i;

			if(!needCheck)
				break;
		}

		if(needCheck && (m_setMember[i].rank == nRank))
		{
			OldIndex = i;
		}
	}

	
	if(OldIndex != -1)
	{
		ChgMemberRank(m_setMember[OldIndex].id,RANK_NORMAL,false,pUser);
	}

	bool bUp = false;
	if(m_setMember[thisIndex].rank < nRank)
		bUp = true;
	int nOldRank = m_setMember[thisIndex].rank;
	if( nRank != RANK_NORMAL)
		ChgMemberRank(idUser,RANK_NORMAL,false,pUser);

	
	if(thisIndex != -1)
	{
		switch(m_setMember[thisIndex].rank)
		{
		case RANK_NORMAL:
		case RANK_NORMAL1:			//����1����
		case RANK_NORMAL2:			//����2����
		case RANK_NORMAL3:			//����3����
		case RANK_NORMAL4:			//����4����
		case RANK_FEELMENBER1:
		case RANK_FEELMENBER2:
		case RANK_FEELMENBER3:
		case RANK_FEELMENBER4:
		case RANK_SHOP:
		case RANK_SUBFEELLEADER1:
		case RANK_SUBFEELLEADER2:
		case RANK_SUBFEELLEADER3:
		case RANK_SUBFEELLEADER4:
		case RANK_FEELLEADER1:
		case RANK_FEELLEADER2:
		case RANK_FEELLEADER3:
		case RANK_FEELLEADER4:
		case RANK_FEELLEADER://��Ӣ
			break;
		case RANK_SUBLEADER:
			{
				if (idSubLeader==m_setMember[thisIndex].id)
					idSubLeader = 0;
				if (idSubLeader2==m_setMember[thisIndex].id)
					idSubLeader2=0;
			}	
			break;
		case RANK_LEADER:
			idLeader = 0;
			break;
		}

		switch(nRank)
		{
		case RANK_SHOP:
		case RANK_SUBFEELLEADER1:
		case RANK_SUBFEELLEADER2:
		case RANK_SUBFEELLEADER3:
		case RANK_SUBFEELLEADER4:
		case RANK_FEELLEADER1:
		case RANK_FEELLEADER2:
		case RANK_FEELLEADER3:
		case RANK_FEELLEADER4:
		case RANK_NORMAL1:			//����1����
		case RANK_NORMAL2:			//����2����
		case RANK_NORMAL3:			//����3����
		case RANK_NORMAL4:			//����4����
		case RANK_FEELMENBER1:
		case RANK_FEELMENBER2:
		case RANK_FEELMENBER3:
		case RANK_FEELMENBER4:
		case RANK_FEELLEADER:
		break;

		case RANK_SUBLEADER:
			if (idSubLeader != idUser && idSubLeader2 != idUser)
			{
				if(idSubLeader != ID_NONE)
				{
					if (idSubLeader2 != ID_NONE)
						nRank = RANK_NORMAL;
					else
						SetSubLeaderID2(idUser);
				}
				else
					SetSubLeaderID(idUser);
			}
			break;
		case RANK_LEADER:
			if(idLeader != ID_NONE)
				nRank = RANK_NORMAL;
			else
				idLeader = idUser;
			break;
		default:
			nRank = RANK_NORMAL;
		}

		m_setMember[thisIndex].rank = nRank;
		CUser* pTarget = UserManager()->GetUser(idUser);

		if(pTarget&&pTarget->QuerySynAttr())
		{
			pTarget->QuerySynAttr()->SetIdRank(m_setMember[thisIndex].id,this->GetID(),nRank,false);
		}
		else if(pUser&&pUser->QuerySynAttr())
		{
			pUser->QuerySynAttr()->SetIdRank(m_setMember[thisIndex].id,this->GetID(),nRank,false);
		}

		if(nRank ==RANK_LEADER)
		{
			SetData(SYNDATA_LEADERID,m_setMember[thisIndex].id,true);
			SetStr(SYNDATA_LEADERNAME,m_setMember[thisIndex].szName,16,true);
		}
		
		if(bSend)
		{
			char szWord[1024];
			
			if(pTarget&&pTarget->QuerySynAttr())
			{
				if(bUp)
					sprintf(szWord, "�㱻��ְΪ%s",pTarget->QuerySynAttr()->GetRankTitle(nRank));
				else
					sprintf(szWord, "�㱻��ְΪ%s",pTarget->QuerySynAttr()->GetRankTitle(nRank));
				pTarget->SendSysMsg(szWord);
				pTarget->QuerySynAttr()->SynchroInfo();

				//goto ���ɳƺ�
				int nNewLev = 0;
				switch(nRank)
				{
				case RANK_LEADER:nNewLev = 14;break;//����
				case RANK_SUBLEADER:nNewLev = 13;break;//������
				case RANK_FEELLEADER:nNewLev = 15;break;//////////////////////////��Ӣ����ע��

				default://����ְ������С��,Ҫɾ��ԭ�гƺ�
					if (pTarget->FindTitleByType(_TITLE_SYNDICATE))
						pTarget->DelHoldTitle(_TITLE_SYNDICATE);
					break;
				}
				if (nNewLev != 0)
				{
					if (pTarget->FindTitleByType(_TITLE_SYNDICATE))
					{
						if (pTarget->GetTitleLev(_TITLE_SYNDICATE) != nNewLev)
							pTarget->SetTitleLevel(_TITLE_SYNDICATE, nNewLev);
					}
					else
						pTarget->AddHoldTitle(_TITLE_SYNDICATE, nNewLev);
				}
			}
				if(bUp)
					sprintf(szWord, "%s����ְΪ%s", m_setMember[thisIndex].szName,GetRankTitle(nRank));
				else
					sprintf(szWord, "%s����ְΪ%s", m_setMember[thisIndex].szName,GetRankTitle(nRank));
				CMsgTalk msg;
				if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
					BroadcastSynMsg(&msg );
		}
		
		return nRank;
	}
	return 0;
}



LPCTSTR CSyndicate::GetRankTitle(int Rank)
{
// 	if(Rank == RANK_NONE)
// 		Rank = this->GetSynRank();
	switch( Rank )
	{
	case	RANK_LEADER:
		return "����";
		break;
	case	RANK_SUBLEADER:
		return "������";
		break;
	case RANK_FEELLEADER:
		return "��Ӣ";
		break;
	case RANK_FEELLEADER1:
	case RANK_FEELLEADER2:
	case RANK_FEELLEADER3:
	case RANK_FEELLEADER4:
	case RANK_SUBFEELLEADER1:
	case RANK_SUBFEELLEADER2:
	case RANK_SUBFEELLEADER3:
	case RANK_SUBFEELLEADER4:
	case RANK_FEELMENBER1:
	case RANK_FEELMENBER2:
	case RANK_FEELMENBER3:
	case RANK_FEELMENBER4:
	case RANK_NORMAL1:			//����1����
	case RANK_NORMAL2:
	case RANK_NORMAL3:			//����3����
	case RANK_NORMAL4:			//����4����
	case RANK_SHOP:
	case RANK_NORMAL:
		return "����";
		break;
	}
	return STR_ERROR;
}

int CSyndicate::AddMemberList(MEMBERINFO info,int nType)
{
	if(nType == 2)
	{
		if (HasQuerymember(info.id))
			return RANK_NONE;
		char szTemp[1024];
		sprintf(szTemp,"%s����������",info.szName);
		CMsgTalk	msg;
		if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szTemp, NULL, 0xff0000, _TXTATR_SYNDICATE))
			BroadcastSynMsg(&msg );
		m_setQueryMember.push_back(info);
		CMsgSynMemberInfo msgtoc;
		if (msgtoc.Create(AddQuery,1,1))
		{
			msgtoc.Append(info.id,info.szName,info.level,info.firJob,
		info.sex,info.rank,info.line,info.vip,info.lastlogin,info.hiscon,
		info.hismoney,info.hisgrow,info.maxpower);
			BroadcastSynMsg(&msgtoc);
		}
		return RANK_NORMAL;
	}
	int nNewRank = info.rank;
	switch(info.rank)
	{
	case RANK_NORMAL:
	case RANK_NORMAL1:			//����1����
	case RANK_NORMAL2:			//����2����
	case RANK_NORMAL3:			//����3����
	case RANK_NORMAL4:			//����4����
	case RANK_FEELMENBER1:
	case RANK_FEELMENBER2:
	case RANK_FEELMENBER3:
	case RANK_FEELMENBER4:
	case RANK_SHOP:
	case RANK_SUBFEELLEADER1:
	case RANK_SUBFEELLEADER2:
	case RANK_SUBFEELLEADER3:
	case RANK_SUBFEELLEADER4:
	case RANK_FEELLEADER1:
	case RANK_FEELLEADER2:
	case RANK_FEELLEADER3:
	case RANK_FEELLEADER4:
	case RANK_FEELLEADER:
			break;
	case RANK_SUBLEADER:
		if(idSubLeader != ID_NONE)
		{
			if (idSubLeader2 != ID_NONE)
			{
				nNewRank = RANK_NORMAL;
			}
			else
				idSubLeader2 = info.id;
		}
		else
			idSubLeader = info.id;
		break;
	case RANK_LEADER:
		if(idLeader != ID_NONE)
			nNewRank = RANK_NORMAL;
		else
			idLeader = info.id;
		break;
	default:
		nNewRank = RANK_NORMAL;
	}
	if (!Hasmember(info.id))
		m_setMember.push_back(info);
	m_pData->SetInt(SYNDATA_AMOUNT,m_setMember.size());

	if(m_pData->GetInt(SYNDATA_STATE) == 1)
	{
		if(m_setMember.size() > 0)
		{
			m_pData->SetInt(SYNDATA_STATE,2);	
			char Msgtemp[256]= "";
			char UserName[128] = "";
			char SYNName[128] = "";
			sprintf(UserName, "%d_[%s]_7", SNAPSHOTUSER, GetStr(SYNDATA_LEADERNAME));
			sprintf(SYNName, "%d_%s_%d", SNAPSHOTNOTHING, GetStr(SYNDATA_NAME), COLORPINK);
			sprintf(Msgtemp, "��������ͻ��<%s>��һ������<%s>�Ĵ����£����������漣������!", SYNName, UserName);
			m_pData->SetInt(SYNDATA_LEVEL, 1);
			m_pData->SetInt(SYNDATA_GROW, 0);
			CMsgTalk msgTalk;
			if(msgTalk.Create(SYSTEM_NAME,ALLUSERS_NAME,Msgtemp,NULL,0x0099ff,_TXTATR_ANNOUNCE))
				UserManager()->BroadcastMapGroupMsg(&msgTalk,NULL);
			CMsgSyndicate msgAction;
			IF_OK(msgAction.Create(SYN_CREATECOMPLETE, this->GetID()))
				BroadcastSynMsg(&msgAction);
		}
	}
	CMsgSynMemberInfo msgtoc;
	if (msgtoc.Create(AddMember,1,1))
	{
		msgtoc.Append(info.id,info.szName,info.level,info.firJob,
			info.sex,info.rank,info.line,info.vip,info.lastlogin,info.hiscon,
			info.hismoney,info.hisgrow,info.maxpower);
			BroadcastSynMsg(&msgtoc);
	}
	if (MainServerFlag()==1)
		m_pData->Update();
	CUserPtr pUser=UserManager()->GetUser(info.id);
	if (pUser)
	{
		if (!pUser->QueryMagic()->FindMagic(MAGIC_BACKSYN))//�ذ＼��
		pUser->QueryMagic()->LearnMagic(MAGIC_BACKSYN,1,true,0,0,0);
	}
	return nNewRank;
}

bool CSyndicate::Hasmember(OBJID idUser)
{
	DEBUG_TRY
	for(int i = 0 ; i < m_setMember.size() ; i ++)
	{
		if(m_setMember[i].id == idUser)
			return true;		
	}
	DEBUG_CATCH2("CSyndicate::Hasmember idUser:%d",idUser);
	return false;
}
bool CSyndicate::HasQuerymember(OBJID idUser)
{
	DEBUG_TRY
		for(int i = 0 ; i < m_setQueryMember.size() ; i ++)
		{
			if(m_setQueryMember[i].id == idUser)
				return true;		
		}
		DEBUG_CATCH2("CSyndicate::HasQuerymember idUser:%d",idUser);
		return false;
}
void CSyndicate::SetOnline(OBJID idUser,UCHAR cLine)
{
	DEBUG_TRY
	for(int i = 0 ; i < m_setMember.size() ; i ++)
	{
		if(m_setMember[i].id == idUser)
		{
			m_setMember[i].line=cLine;
			return;
		}
	}
	DEBUG_CATCH3("CSyndicate::SetOnline idUser:%d-bOnline:%d",idUser,cLine);
}

bool CSyndicate::AllowMenber(OBJID idUser,CUser* pUser)
{
	if(DelMemberList(idUser, 3))
	{
		CUserPtr pThisUser = UserManager()->GetUser(idUser);
		if(pThisUser&&pThisUser->QuerySynAttr()!=NULL)
		{
			if(pThisUser->GetSyndicate())
				return false;
			if (!Hasmember(idUser))
				pThisUser->QuerySynAttr()->JoinSyn(this->GetID(),false,idUser);		
			else
				return false;
		}
		else
		{
			if (!Hasmember(idUser)&&pUser->QuerySynAttr()!=NULL)
			{
				if(!pUser->QuerySynAttr()->JoinSyn(this->GetID(),true ,idUser))
					return false;
			}
			else
				return false;
		}

// 		::MemberInfo* info =GetMemberFromList(idUser,1);
// 		if(info)
// 		{
// 			char szWord[1024]="";
// 			sprintf(szWord,"%s�������",info->szName);
// 
// 			CMsgTalk	msg;
// 			if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
// 				BroadcastSynMsg(&msg );
// 		}
		return true;
	}
	return false;
}

MemberInfo* CSyndicate::GetMemberFromList(OBJID idUser,int nType)
{
	MemberInfo* memberptr=NULL;
	if(nType==2 || nType==3)
	{
		for(int i = 0; i < m_setQueryMember.size() ; i ++)
		{
			if(m_setQueryMember[i].id == idUser)
			return memberptr=&m_setQueryMember[i];
		}
		return NULL;
	}
	for(int i = 0; i < m_setMember.size() ; i ++)
	{
		if(m_setMember[i].id == idUser)
			return memberptr=&m_setMember[i];
	}
	return NULL;
}

bool CSyndicate::DelMemberList(OBJID idUser,int nType)
{
	if(idUser == ID_NONE)
		return false;

	if(nType == 2 || nType == 3)
	{
		for(int i = 0; i < m_setQueryMember.size() ; i ++)
		{
			if(m_setQueryMember[i].id != idUser)
				continue;

			if(nType == 2)
			{
 				char szTemp[1024];
 				sprintf(szTemp,"%s��������󱻾ܾ�",m_setQueryMember[i].szName);
 				CMsgTalk	msg;
 				if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szTemp, NULL, 0xff0000, _TXTATR_SYNDICATE))
					BroadcastSynMsg(&msg );
				
			}
			m_setQueryMember.erase(m_setQueryMember.begin() + i);
			CMsgSynMemberInfo msgtoc;
			if (msgtoc.Create(DelQuery,1,1))
			{
				msgtoc.Append(idUser,"",0,0,0,0,0,0,0,0,0,0,0);
				CUserPtr pUser=UserManager()->GetUser(idUser);
				if (pUser)
					BroadcastSynMsg(&msgtoc,pUser);
				else
					BroadcastSynMsg(&msgtoc);
			}
			return true;
		}
		return false;
	}
	
	for(int i = 0; i < m_setMember.size() ; i ++)
	{
		
		if(m_setMember[i].id != idUser)
			continue;

		switch(m_setMember[i].rank)
		{
		case RANK_NORMAL:
		case RANK_FEELMENBER1:
		case RANK_FEELMENBER2:
		case RANK_FEELMENBER3:
		case RANK_FEELMENBER4:
		case RANK_SHOP:
		case RANK_SUBFEELLEADER1:
		case RANK_SUBFEELLEADER2:
		case RANK_SUBFEELLEADER3:
		case RANK_SUBFEELLEADER4:
		case RANK_FEELLEADER1:
		case RANK_FEELLEADER2:
		case RANK_FEELLEADER3:
		case RANK_FEELLEADER4:
		case RANK_FEELLEADER:
			break;
		case RANK_SUBLEADER:
			if (idSubLeader==m_setMember[i].id)
					idSubLeader = 0;
			else if (idSubLeader2==m_setMember[i].id)
					idSubLeader2 = 0;
			break;
		case RANK_LEADER:
			return false;
		}

		///��������Ա������״̬������������1
		m_setMember.erase(m_setMember.begin() + i);
		m_pData->SetInt(SYNDATA_AMOUNT,m_setMember.size());
		if (MainServerFlag()==1)
			m_pData->Update();
		CMsgSynMemberInfo msgtoc;
		if (msgtoc.Create(DelMember,1,1))
		{
			msgtoc.Append(idUser,"",0,0,0,0,0,0,0,0,0,0,0);
			CUserPtr pUser=UserManager()->GetUser(idUser);
			if (pUser)
				BroadcastSynMsg(&msgtoc,pUser);
			else
				BroadcastSynMsg(&msgtoc);
		}
		return true;
	}
	return false;
}

int CSyndicate::GetMoney()
{
	return GetInt(SYNDATA_MONEY);
}

int CSyndicate::GetSubSynNum()
{
	int nNum = 0;

	for (int i=0; i<SynManager()->QuerySynSet()->GetAmount(); i++)
	{
		CSyndicate* pSyn = SynManager()->QuerySynSet()->GetObjByIndex(i);
		
	}
	return nNum;


}
//���������뿪����Ա
bool CSyndicate::ApplyKickoutMember(CUserPtr pLeader, CUserPtr pTarget)
{
	DEBUG_TRY

	OBJID idSynLeader=pLeader->GetSynID(),idTarget=pLeader->GetSynID();
	if(idSynLeader!=idTarget)
		return false;
	
	int nRank = pLeader->GetSynRank();

	if(nRank!=RANK_SUBLEADER)
		return false;
	
	CSyndicate * pSyn = pLeader->GetSyndicate();
	if(!pSyn)
		return false;

	char szWords[256];
	memset(szWords,0,sizeof(szWords));
	
	//�ض���ʽ�����ԣ�������Ա�����룩
	sprintf(szWords,"ApplyDismiss %d %s %s",pSyn->GetID(),pLeader->GetName(),pTarget->GetName() );
	//��������
	LeaveWord()->InsertWords(pLeader->GetName(),pSyn->GetStr(SYNDATA_LEADERNAME),szWords);
	
	DEBUG_CATCH("bool CSyndicate::ApplyKickoutMember(CUserPtr pLeader, CUserPtr pTarget)")
	return true;

}
//����ÿ�ܻ���ͳһ�ĵط����ű��ܾ��������һ������Ϊ��Ա����
/*
CUserPtr pUser ����
DWORD dwBonus ����
*/
bool CSyndicate::AllotBonusForMember(CUserPtr pUser, DWORD dwBonus)
{

	OBJID idMap = pUser->GetMapID();
	OBJID idSyn = pUser->GetSynID();

	DWORD dwProffer			= 0;
	DWORD dwMoney			= 0;
	DWORD dwTotalProffer	= 0;
	std::vector<CUser*>	setUser;

	//�õ��ڳ������еİ��ɳ�Ա����������ֵ
	CUserManager::Iterator pCurUser = UserManager()->NewEnum();
	while(pCurUser.Next())
	{
		if(pCurUser && pCurUser->GetMapID()==idMap && pCurUser->IsMemberOfSyn(idSyn))
		{
			setUser.push_back(pCurUser);

			DWORD dwInc = /*pCurUser->QuerySynAttr()->GetProfferInc()*/0;//�õ����׶�����
			if( dwInc > SYNPROFFER_LIMIT )
				dwInc = SYNPROFFER_LIMIT;

			dwTotalProffer += dwInc;			
		}
	}
	if( !dwTotalProffer )
		return false;	
	// �ȿ�Ǯ
	//ȥ�����ɻ��� satan 2009.03.06
	//this->QueryModify()->AddData(SYNDATA_MONEY, -1*dwBonus);

	//��ÿλ�ڳ��İ��ɳ�Ա�������ǵĹ���ֵ���佱��
	float fAvg = ( float )dwBonus / ( float )dwTotalProffer;

	for (int i=0; i<setUser.size(); i++)
	{
		CUser* pUser = setUser[i];
		IF_NOT (pUser)
			continue;

		{
			dwProffer	= /*pUser->QuerySynAttr()->GetProfferInc()*/0;
			dwMoney	= dwProffer * fAvg;
			pUser->AddMoney(dwMoney);				
			pUser->QuerySynAttr()->ResetSynProfferInc();
		}
	}
	return true;
}

//�Ƿ���Դ����ֶ�
bool CSyndicate::IfCanCreateSubSubSyn()
{
	int nNum = this->GetSubSubSynNum();
	if(nNum<0)
		return false;
	
// 	int nSynRank = GetInt(SYNDATA_RANK);
// 	
// 	switch(nSynRank)
// 	{
// 	case SYNRANK_ONE:
// 		{		
// 			return false;
// 		}
// 		break;
// 	case SYNRANK_TWO:
// 		{
// 			if(nNum>=2)
// 				return false;			
// 		}
// 		break;
// 	case SYNRANK_THREE:
// 		{
// 			if(nNum>=2)
// 				return false;			
// 		}
// 		break;
// 	case SYNRANK_FOUR:
// 		{
// 			if(nNum>=3)
// 				return false;			
// 		}
// 		break;
// 		
// 	default :
// 		return false;
// 		break;
// 	}
	
	return true;
}
//�õ����ŵķֶӸ���
int CSyndicate::GetSubSubSynNum()
{
	int nNum = 0;
	return nNum;	
}
//�Ƿ��� ����
bool CSyndicate::IsSubSyn()
{
	return false;

}
//�Ƿ��Ƿֶ�
bool CSyndicate::IsSubSubSyn()
{
	return false;
}
//�Ƿ�԰��ɽ��н���
//����������Ҫ������������޶�,�������̶�ʱ��μ����������ή��
const int	AmountCheck[] ={ 5,13,18,23,28,33 };
bool CSyndicate::IsCanFallLevel()
{
//��Ϊʱ��֮�����޷���˵��,ע�������ж�

	int nSynRank	= /*GetInt( SYNDATA_RANK )*/0; //���ŵȼ�
	int nMemSum		= GetMemberAmount();
	assert( nSynRank >= 1 && nSynRank <= 6 );
	if( nMemSum < AmountCheck[ nSynRank - 1 ] )
		return true;
	return false;
//����ע���޸�
/*	switch(nSynRank)
	{
	case SYNRANK_TWO:				
		{
			nLimit = MulDiv(SYNMEMSUM_LOWERLIMIT_TWO, 90, 100);
			if(nMemSum <= nLimit)
				return true;			
		}
		break;
	case SYNRANK_THREE:
		{
			nLimit = MulDiv(SYNMEMSUM_LOWERLIMIT_THREE, 90, 100);
			if(nMemSum <= nLimit)
				return true;			
		}
		break;
	case SYNRANK_FOUR:
		{
			nLimit = MulDiv(SYNMEMSUM_LOWERLIMIT_FOUR, 90, 100);
			if(nMemSum <= nLimit)
				return true;			
		}
		break;
	default :	
		break;
	}
	return false;*/
}
//�԰��ɽ��н����������������ǰ�ȼ���Ƿ�������

bool CSyndicate::FallLevel()
{

// 	if(GetInt(SYNDATA_RANK)==SYNRANK_ONE)	//һ������
// 	{
// //		SynManager()->QueryModify()->DestroySyndicate(GetID());
// #ifdef _DEBUG
// 		::MyLogSave( "syslog/wyb_syndicate","Syndicate has Destory,Syn name = %s",GetStr( SYNDATA_NAME ) );
// #endif
// 		return true;
// 	}
// //	this->QueryModify()->AddData(SYNDATA_RANK,-SYNRANK_ONE,true);
// #ifdef _DEBUG
// 	::MyLogSave( "syslog/wyb_syndicate","Syndicate has fall level,Syn name = %s",GetStr( SYNDATA_NAME ) );
// #endif
	return true;

//����ע�Ͳ��޸�,��ʱ��֮�������Ӱ���
/*	if(!this->IsMasterSyn())
		return false;
	if(GetInt(SYNDATA_RANK)==SYNRANK_ONE)
	{
		SynManager()->QueryModify()->DestroySyndicate(GetID());
		return true;
	}
	

	//����
	this->QueryModify()->AddData(SYNDATA_RANK,-SYNRANK_ONE,true);

	return true;*/

}

OBJID CSyndicate::GetMasterSynID()
{
	return GetID();
}

void CSyndicate::SynchroInfo()
{
	CSyndicate* pMaster = this->GetMasterSyn();
	CHECK(pMaster);	

	CUserManager::Iterator pUser = UserManager()->NewEnum();
	while(pUser.Next())
	{
		if(pUser && pUser->GetSynID() == pMaster->GetID())				
		{
			pUser->QuerySynAttr()->SendInfoToClient();
		}
	}

	{
		OBJID idMaster = pMaster->GetID();
		CHECK(idMaster != ID_NONE);
		for(int i = 0; i < SynManager()->QuerySynSet()->GetAmount(); i++)
		{
			CSynPtr pSyn = SynManager()->QuerySynSet()->GetObjByIndex(i);
		}
	}
}

int CSyndicate::GetLeaderID()
{
	return idLeader;
}
int CSyndicate::GetSubLeaderID()
{
	return idSubLeader;
}
int CSyndicate::GetSubLeader2ID()
{
	return idSubLeader2;
}
int CSyndicate::GetLeaderWifeID()
{
	return 0;
}
void CSyndicate::SetSubLeaderID(OBJID id)
{
	if (idSubLeader!=id && idSubLeader2!=id)
		idSubLeader = id;
}
void CSyndicate::SetSubLeaderID2(OBJID id)
{
	if (idSubLeader!=id && idSubLeader2!=id)
		idSubLeader2 = id;
}
void CSyndicate::SetLeaderWifeID(OBJID id)
{
}

//--------------------------------------------
//BOOL CSyndicate::SetTaskPublishTime(DWORD curTime,BOOL bUpdate/*=UPDATE_NEVER*/)//�����̻߳ص�,���þ������񷢲�ʱ��
//{
//	CHECKF(m_pData);
//
//	m_pData->SetInt( SYNDATA_TASKPUBLISHTIME, curTime);
//	if(bUpdate == UPDATE_TRUE)
//		m_pData->Update();
//	else if(bUpdate == UPDATE_NEVER)//���޸��ڴ�,�ڱ��Ѿ�update
//		m_pData->ClearUpdateFlags();
//	else//���ܵȴ�����Set֮��,ͳһһ��update
//	{
//	}
//	return true; 
//}

//---------------------------------------------
//BOOL CSyndicate::SetActionID(DWORD actionID,BOOL bUpdate/*=UPDATE_NEVER*/)//�����̻߳ص�,���þ�������action
//{
//	CHECKF(m_pData);
//
//	m_pData->SetInt( SYNDATA_ACTIONID, actionID);
//	if(bUpdate == UPDATE_TRUE)
//		m_pData->Update();
//	else if(bUpdate == UPDATE_NEVER)//���޸��ڴ�,�ڱ��Ѿ�update
//		m_pData->ClearUpdateFlags();
//	else//���ܵȴ�����Set֮��,ͳһһ��update
//	{
//	}
//	return true; 
//}
//-----------------------------------------------
BOOL CSyndicate::ReqSetTaskPublishTime(DWORD curTime)//���������߳�,�ص�ʱ�����޸��ҽ��޸ı���ͼ���ڴ�����
{
	//ע��!!���µ��Ĳ���CSyndicate::AddData()
	//����CSyndicate.XSynModify.AddData
//	return this->QueryModify()->SetData(SYNDATA_TASKPUBLISHTIME, curTime);//bSave==true
	return true;
}
//------------------------------------------------
BOOL CSyndicate::ReqSetActionID(DWORD actionID)//���������߳�,�ص�ʱ�����޸��ҽ��޸ı���ͼ���ڴ�����
{
	//ע��!!���µ��Ĳ���CSyndicate::AddData()
	//����CSyndicate.XSynModify.AddData
//	return this->QueryModify()->SetData(SYNDATA_ACTIONID, actionID);//bSave==true
	return true;
}
//------------------------------------------------
BOOL CSyndicate::ReqSetSynProffer(int synProffer)//���������߳����þ��Ź���ֵ
{
//	return this->QueryModify()->SetData(SYNDATA_SYNPROFFER, synProffer);//bSave==true
	return true;
}
//------------------------------------------------
bool CSyndicate::PayOff(DWORD dwMoney,BOOL bSave/* =true */)//20070414����:������.�������ڴ�.
{
//	CHECKF(dwMoney>0
//		&& m_pData->GetInt(SYNDATA_MONEY) > dwMoney*(100+_SYN_PAYOFF_TEX)/100);//��Ӧ�ڵ�ͼ������ʱ���Ѿ����.�����߳�ʱҲ����

//	m_pData->SetInt(SYNDATA_MONEY, m_pData->GetInt(SYNDATA_MONEY)-dwMoney);//dec money
//	m_pData->SetInt(SYNDATA_MONEY, 
//		m_pData->GetInt(SYNDATA_MONEY)-dwMoney*(100+_SYN_PAYOFF_TEX)/100); // �½������,������Ҫ��˰�� [5/18/2007]

	//  [3/19/2008 ���]	0002937: ���ŷ��������⡣���˵���� 
	CHECKF(dwMoney > 0);
	__int64 i64OldMoney = m_pData->GetInt(SYNDATA_MONEY);
	__int64 i64NewMoney = (__int64)dwMoney * (100 + _SYN_PAYOFF_TEX) / (__int64)100;
	i64NewMoney = i64OldMoney - i64NewMoney;

	CHECKF(i64NewMoney > 0);

	m_pData->SetInt(SYNDATA_MONEY, i64NewMoney);


	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();

	SynchroInfo();//ͬ���ͻ���

	return true;
}
//-----------------------------------------------
BOOL CSyndicate::ReqPayOff(DWORD dwMoney)//���������̷߳�����
{
	//�Ƿ�
	if(dwMoney==0)
		return false;
	
	//����
	if(GetMoney()<dwMoney)
		return false;

	//��С���
	if(dwMoney<10000)
		return false;
	
//	return this->QueryModify()->PayOff(dwMoney);
	return true;
}
//----------------------------------------------
BOOL CSyndicate::ReportMemberProfferToWolrd(int dwProffer,OBJID idMember)//�㱨��Ա����ֵ�������߳�.
{
//	return this->QueryModify()->SetMemberProffer(dwProffer, idMember);
	return true;
}

//----------------------------------------------
BOOL CSyndicate::ReqSetSynName(LPCTSTR szNewSynName)//20070825����:���������߳̾��Ÿ���
{
//	return this->QueryModify()->SetSynName(szNewSynName);
	return true;
}

void CSyndicate::SendSubToC(CUser* pUser)
{
	CHECK(pUser);
	
	CMsgSynInfo msg;
	if(msg.CreateSub(this))
	pUser->SendMsg(&msg);
}
void CSyndicate::SendSciToC(CUserPtr pUser)
{
	CHECK(pUser);
	
	CMsgSynInfo msg;
	if(msg.CreateSci(this))
		pUser->SendMsg(&msg);
}
// UINT CSyndicate::GetBuilder(OBJID SubId)
// {
// 
// 	if (1 == SubId)
// 	{
// 		return GetInt(SYNDATA_SUB1_BUILDER);
// 	}
// 	else if (2 == SubId)
// 	{
// 		return GetInt(SYNDATA_SUB2_BUILDER);
// 	}
// 	else if (3 == SubId)
// 	{
// 		return GetInt(SYNDATA_SUB3_BUILDER);
// 	}
// 	else if (4 == SubId)
// 	{
// 		return GetInt(SYNDATA_SUB4_BUILDER);
// 	}
// 	return 0;
// }

// void CSyndicate::SetBuider(OBJID SubId, int Data)
// {
//         //���жϹ�Ӷ�������Ƿ��������
// 	if ((GetInt(SYNDATA_LEVEL)-1)*300 + 100 < Data)
// 	{
// 		return;
// 	}
// 	if (1 == SubId)
// 	{
// 		if ((GetInt(SYNDATA_SUBLEV1)-1)*300 + 100 < Data)
// 		{
// 			return;
// 		}
// 		m_pData->SetInt(SYNDATA_SUB1_BUILDER, Data);
// 	}
// 	else if (2 == SubId)
// 	{
// 		if ((GetInt(SYNDATA_SUBLEV2)-1)*300 + 100 < Data)
// 		{
// 			return;
// 		}
// 		m_pData->SetInt(SYNDATA_SUB2_BUILDER, Data);
// 	}
// 	else if (3 == SubId)
// 	{
// 		if ((GetInt(SYNDATA_SUBLEV3)-1)*300 + 100 < Data)
// 		{
// 			return;
// 		}
// 		m_pData->SetInt(SYNDATA_SUB3_BUILDER, Data);
// 	}
// 	else if (4 == SubId)
// 	{
// 		if ((GetInt(SYNDATA_SUBLEV4)-1)*300 + 100 < Data)
// 		{
// 			return;
// 		}
// 		m_pData->SetInt(SYNDATA_SUB4_BUILDER, Data);
// 	}
// 
// 	char szWords[32] = "";
// 	sprintf(szWords, "����������������Ϊ��%d", Data);
// 	CMsgTalk msg;
// 	if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWords, NULL, 0xff0000, _TXTATR_SYNDICATE))
// 		BroadcastSynMsg(&msg,NULL);
// }

// UINT CSyndicate::GetKONGFU(OBJID SubId)
// {
// 
// 	if (1 == SubId)
// 	{
// 		return GetInt(SYNDATA_SUB1_KONGFU);
// 	}
// 	else if (2 == SubId)
// 	{
// 		return GetInt(SYNDATA_SUB2_KONGFU);
// 	}
// 	else if (3 == SubId)
// 	{
// 		return GetInt(SYNDATA_SUB3_KONGFU);
// 	}
// 	else if (4 == SubId)
// 	{
// 		return GetInt(SYNDATA_SUB4_KONGFU);
// 	}
// 	return NULL;
// }
// 
// void CSyndicate::SetKONGFU(OBJID SubId, int Data)
// {
// 	if ((GetInt(SYNDATA_LEVEL)-1)*300 + 100 < Data)
// 	{
// 		return;
// 	}
// 	if (1 == SubId)
// 	{
// 		if ((GetInt(SYNDATA_SUBLEV1)-1)*300 + 100 < Data)
// 		{
// 			return;
// 		}
// 		m_pData->SetInt(SYNDATA_SUB1_KONGFU, Data);
// 	}
// 	else if (2 == SubId)
// 	{
// 		if ((GetInt(SYNDATA_SUBLEV2)-1)*300 + 100 < Data)
// 		{
// 			return;
// 		}
// 		m_pData->SetInt(SYNDATA_SUB2_KONGFU, Data);
// 	}
// 	else if (3 == SubId)
// 	{
// 		if ((GetInt(SYNDATA_SUBLEV3)-1)*300 + 100 < Data)
// 		{
// 			return;
// 		}
// 		m_pData->SetInt(SYNDATA_SUB3_KONGFU, Data);
// 	}
// 	else if (4 == SubId)
// 	{
// 		if ((GetInt(SYNDATA_SUBLEV4)-1)*300 + 100 < Data)
// 		{
// 			return;
// 		}
// 		m_pData->SetInt(SYNDATA_SUB4_KONGFU, Data);
// 	}
// 	m_pData->Update();
// 
// 	char szWords[32] = "";
// 	sprintf(szWords, "��ʦ��������Ϊ��%d", Data);
// 	CMsgTalk msg;
// 	if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWords, NULL, 0xff0000, _TXTATR_SYNDICATE))
// 		BroadcastSynMsg(&msg,NULL);
// }
// 
// UINT CSyndicate::GetBusMan(OBJID SubId)
// {
// 	if (1 == SubId)
// 	{
// 		return GetInt(SYNDATA_SUB1_BUSMAN);
// 	}
// 	else if (2 == SubId)
// 	{
// 		return GetInt(SYNDATA_SUB2_BUSMAN);
// 	}
// 	else if (3 == SubId)
// 	{
// 		return GetInt(SYNDATA_SUB3_BUSMAN);
// 	}
// 	else if (4 == SubId)
// 	{
// 		return GetInt(SYNDATA_SUB4_BUSMAN);
// 	}
// 	return NULL;
// 
// }
// 
// bool CSyndicate::SetBusMan(OBJID SubId, int Data1, int Data2, int Data3, const char* szName, int Money)
// {
// 	if ((GetInt(SYNDATA_LEVEL)-1)*300 + 100 < Data1 || (GetInt(SYNDATA_LEVEL)-1)*300 + 100 < Data2 || (GetInt(SYNDATA_LEVEL)-1)*300 + 100 < Data3)
// 	{
// 		return false;
// 	}
// 	if (1 == SubId)
// 	{
// 		if (GetInt(SYNDATA_SUBLEV1) == 0 && Data1 !=0 && Data2 != 0 && Data3 != 0)
// 		{
// 			if ((GetInt(SYNDATA_SUBLEV1)-1)*300 + 100 < Data1 || (GetInt(SYNDATA_SUBLEV1)-1)*300 + 100 < Data2 || (GetInt(SYNDATA_SUBLEV1)-1)*300 + 100 < Data3)
// 			{
// 				return false;
// 			}
// 		}
// 		m_pData->SetInt(SYNDATA_SUB1_BUSMAN, Data3);
// 		m_pData->SetInt(SYNDATA_SUB1_KONGFU, Data2);
// 		m_pData->SetInt(SYNDATA_SUB1_BUILDER, Data1);
// 	}
// 	else if (2 == SubId)
// 	{
// 		if (GetInt(SYNDATA_SUBLEV2) == 0 && Data1 !=0 && Data2 != 0 && Data3 != 0)
// 		{
// 			if ((GetInt(SYNDATA_SUBLEV2)-1)*300 + 100 < Data1 || (GetInt(SYNDATA_SUBLEV2)-1)*300 + 100 < Data2 || (GetInt(SYNDATA_SUBLEV2)-1)*300 + 100 < Data3)
// 			{
// 				return false;
// 			}
// 		}
// 		m_pData->SetInt(SYNDATA_SUB2_BUSMAN, Data3);
// 		m_pData->SetInt(SYNDATA_SUB2_KONGFU, Data2);
// 		m_pData->SetInt(SYNDATA_SUB2_BUILDER, Data1);
// 	}
// 	else if (3 == SubId)
// 	{
// 		if (GetInt(SYNDATA_SUBLEV3) == 0 && Data1 !=0 && Data2 != 0 && Data3 != 0)
// 		{
// 			if ((GetInt(SYNDATA_SUBLEV3)-1)*300 + 100 < Data1 || (GetInt(SYNDATA_SUBLEV3)-1)*300 + 100 < Data2 || (GetInt(SYNDATA_SUBLEV3)-1)*300 + 100 < Data3)
// 			{
// 				return false;
// 			}
// 		}
// 		m_pData->SetInt(SYNDATA_SUB3_BUSMAN, Data3);
// 		m_pData->SetInt(SYNDATA_SUB3_KONGFU, Data2);
// 		m_pData->SetInt(SYNDATA_SUB3_BUILDER, Data1);
// 	}
// 	else if (4 == SubId)
// 	{
// 		if (GetInt(SYNDATA_SUBLEV4) == 0 && Data1 !=0 && Data2 != 0 && Data3 != 0)
// 		{
// 			if ((GetInt(SYNDATA_SUBLEV4)-1)*300 + 100 < Data1 || (GetInt(SYNDATA_SUBLEV4)-1)*300 + 100 < Data2 || (GetInt(SYNDATA_SUBLEV4)-1)*300 + 100 < Data3)
// 			{
// 				return false;
// 			}
// 		}
// 		m_pData->SetInt(SYNDATA_SUB4_BUSMAN, Data3);
// 		m_pData->SetInt(SYNDATA_SUB4_KONGFU, Data2);
// 		m_pData->SetInt(SYNDATA_SUB4_BUILDER, Data1);
// 	}
// 
// 	if (!szName)
// 	{
// 		return true;
// 	}
// 	char szSub[32] = "";
// 	char szWords[256] = "";
// 	sprintf(szSub, GetSubNameByID(SubId));
// 	if (Money > 0)
// 	{
// 		sprintf(szWords, "<0_[%s]_7>�޸�<3_%s_7>�Ĺ�Ӷ����Ϊ����������%d�ˣ���ʦ%d�ˣ�����%d�ˣ��˴��޸ĺķ��ʽ�%d��",szName, szSub, Data1, Data2, Data3, Money/10000);
// 	}
// 	else
// 		sprintf(szWords, "<0_[%s]_7>�޸�<3_%s_7>�Ĺ�Ӷ����Ϊ����������%d�ˣ���ʦ%d�ˣ�����%d�ˡ�",szName, szSub, Data1, Data2, Data3);
// 	CMsgTalk msg;
// 	if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWords, NULL, 0xff0000, _TXTATR_SYNDICATE))
// 	BroadcastSynMsg(&msg, NULL);
// 	return true;
// }

LPCTSTR CSyndicate::GetSubNameByID(OBJID sub)
{
	if (sub == 0)
	{
		return "����";
	}
	if (sub == 1)
	{
		return "������";
	}
	if (sub == 2)
	{
		return "�׻���";
	}
	if (sub == 3)
	{
		return "������";
	}
	if (sub == 4)
	{
		return "��ȸ��";
	}
	if (sub == 5)
	{
		return (m_pData->GetStr(SYNDATA_NAME));
	}
	return STR_ERROR;
}

UINT CSyndicate::GetGrow()
{
	UINT Grow = GetInt(SYNDATA_GROW);
	return Grow;
}

bool CSyndicate::MaintainSynData()
{
// 	UINT OldGrow		= GetGrow();
// 	UINT OldActiv	= GetInt(SYNDATA_ACTIVEAMOUNT);
// 	int UpMin = 5;
// 	UINT uData = 0;
// 	/////////////////////////////////////////////////////////////////////////////
// 	if (1 == GetInt(SYNDATA_SUBGROW1))
// 	{
// 		UINT	OldSkill = GetInt(SYNDATA_SUB1_SKILL1);
// 		uData = GetInt(SYNDATA_SUB1_KONGFU)*UpMin +OldSkill;
// 		SetSkill(1, uData);
// 	}
// 	else if (2 == GetInt(SYNDATA_SUBGROW1))
// 	{
// 		UINT	OldSkill = GetInt(SYNDATA_SUB1_SKILL2);
// 		uData = GetInt(SYNDATA_SUB1_KONGFU)*UpMin +OldSkill;
// 		SetSkill(1, uData);
// 	}
// 	else if (3 == GetInt(SYNDATA_SUBGROW1))
// 	{
// 		UINT	OldSkill = GetInt(SYNDATA_SUB1_SKILL3);
// 		uData = GetInt(SYNDATA_SUB1_KONGFU)*UpMin +OldSkill;
// 		SetSkill(1, uData);
// 	}
// 	
// 	/////////////////////////////////////////////////////////////////////////////
// 	if (1 == GetInt(SYNDATA_SUBGROW2))
// 	{
// 		UINT	OldSkill = GetInt(SYNDATA_SUB2_SKILL1);
// 		uData = GetInt(SYNDATA_SUB2_KONGFU)*UpMin +OldSkill;
// 		SetSkill(2, uData);
// 	}
// 	else if (2 == GetInt(SYNDATA_SUBGROW2))
// 	{
// 		UINT	OldSkill = GetInt(SYNDATA_SUB2_SKILL2);
// 		uData = GetInt(SYNDATA_SUB2_KONGFU)*UpMin +OldSkill;
// 		SetSkill(2, uData);
// 	}
// 	else if (3 == GetInt(SYNDATA_SUBGROW2))
// 	{
// 		UINT	OldSkill = GetInt(SYNDATA_SUB2_SKILL3);
// 		uData = GetInt(SYNDATA_SUB2_KONGFU)*UpMin +OldSkill;
// 		SetSkill(2, uData);
// 	}
// 	
// 	/////////////////////////////////////////////////////////////////////////////
// 	if (1 == GetInt(SYNDATA_SUBGROW3))
// 	{
// 		UINT	OldSkill = GetInt(SYNDATA_SUB3_SKILL1);
// 		uData = GetInt(SYNDATA_SUB3_KONGFU)*UpMin +OldSkill;
// 		SetSkill(3, uData);
// 	}
// 	else if (2 == GetInt(SYNDATA_SUBGROW3))
// 	{
// 		UINT	OldSkill = GetInt(SYNDATA_SUB3_SKILL2);
// 		uData = GetInt(SYNDATA_SUB3_KONGFU)*UpMin +OldSkill;
// 		SetSkill(3, uData);
// 	}
// 	else if (3 == GetInt(SYNDATA_SUBGROW3))
// 	{
// 		UINT	OldSkill = GetInt(SYNDATA_SUB3_SKILL3);
// 		uData = GetInt(SYNDATA_SUB3_KONGFU)*UpMin +OldSkill;
// 		SetSkill(3, uData);
// 	}
// 	
// 	/////////////////////////////////////////////////////////////////////////////
// 	if (1 == GetInt(SYNDATA_SUBGROW4))
// 	{
// 		UINT	OldSkill = GetInt(SYNDATA_SUB4_SKILL1);
// 		uData = GetInt(SYNDATA_SUB4_KONGFU)*UpMin +OldSkill;
// 		SetSkill(4, uData);
// 	}
// 	else if (2 == GetInt(SYNDATA_SUBGROW4))
// 	{
// 		UINT	OldSkill = GetInt(SYNDATA_SUB4_SKILL2);
// 		uData = GetInt(SYNDATA_SUB4_KONGFU)*UpMin +OldSkill;
// 		SetSkill(4, uData);
// 	}
// 	else if (3 == GetInt(SYNDATA_SUBGROW4))
// 	{
// 		UINT	OldSkill = GetInt(SYNDATA_SUB4_SKILL3);
// 		uData = GetInt(SYNDATA_SUB4_KONGFU)*UpMin +OldSkill;
// 		SetSkill(4, uData);
// 	}
	
	SetDeadline();//ÿ��ά����ʱ��Ҫ����Ƿ񳬹�������
	return true;
}


bool CSyndicate::NoPay()
{
	if (GetInt(SYNDATA_PAY) != 0)
	{
		return true;
	}
	else 
		return false;
}

bool CSyndicate::PayBasic()
{
	CMsgTalk	msg;
	UINT		NeedMoney	=	0;
	int nUpline=GetDeadline(5,0);
	int nCurMoney=m_pData->GetInt(SYNDATA_MONEY);
	double Base=(double)nCurMoney/(double)nUpline;
	if (Base > 0.7)
		Base=2;
	if (Base <= 0.7 && Base >= 0.2)
		Base=1.5;
	if (Base < 0.2)
		Base=1;
	char szWords[128]	=	"";
	NeedMoney=(m_pData->GetInt(SYNDATA_LEVEL)+4)*10000*Base;
	if ( NeedMoney > GetInt(SYNDATA_MONEY))
	{
		if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , "���棺�����ʽ��㣬����ֹͣά�������ְ��ɹ����ѹرգ�", NULL, 0xff0000, _TXTATR_SYNDICATE))
			BroadcastSynMsg(&msg );
		SetPay(1);
		///////////////////////////////////�߻�Ҫ��Ǯ��������Ҫ�۵�
		CMsgTalk SynMsg;
		if (GetInt(SYNDATA_MONEY) <= NeedMoney)
		{
			NeedMoney = GetInt(SYNDATA_MONEY);
			SetGrow(SPENDMONEY,0,false);
		}
		else
		SetGrow(SPENDMONEY, GetInt(SYNDATA_MONEY) - NeedMoney,false);

		sprintf(szWords, "����ά�������˰����ʽ�%d��%d��%dͭ��", NeedMoney/10000, (NeedMoney-((NeedMoney/10000)*10000))/100,NeedMoney % 100);
		if (SynMsg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWords, NULL, 0xff0000, _TXTATR_SYNDICATE))
			BroadcastSynMsg(&SynMsg );
	}
	else
	{
		SetGrow(SPENDMONEY,GetInt(SYNDATA_MONEY) - NeedMoney,false);
		sprintf(szWords, "����ά�������˰����ʽ�%d��%d��%dͭ��", NeedMoney/10000,(NeedMoney-((NeedMoney/10000)*10000))/100,NeedMoney % 100);
		if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWords, NULL, 0xff0000, _TXTATR_SYNDICATE))
			BroadcastSynMsg(&msg );
		SetPay(0);
		return true;
	}
	return false;
}

void CSyndicate::OnTimer(time_t tCur)
{

	DEBUG_TRY
	if(t_Maintain.ToNextTick(60 * 5))	//5���Ӹ���һ��
	{
		if (MainServerFlag() == 1)			//ֻ�������������ܸ������ݿ�
		{	
			m_pData->Update();
			MaintainSynData();
		}
	}
	DEBUG_CATCH("CSyndicate::OnTimer Crash!");

	DEBUG_TRY
	SYSTEMTIME ct = {0};
	UINT date = 0;
	GetLocalTime(&ct);
	int	nhour = ct.wHour;
	int nmin  = ct.wMinute;
//	nhour = nmin;
	if (AmontStatue == -1)
	{
		AmontStatue = nhour + 1;
	}
	else
	{
		if (AmontStatue == -1 || AmontStatue == nhour)
		{
			this->PayBasic();
			if (nhour == 5)
			{
				if (g_SpecilLine==1)
				{
					DelSynMonster();
					SetCallTask(false);
				}
			}
			if (nhour == 23)
				AmontStatue = 0;
			else
				AmontStatue = nhour+1;
		}
	}
	DEBUG_CATCH("CSyndicate::OnTimer Crash!");
}

void CSyndicate::ReduceLevel()/////////////////////////////////////���ɽ���
{
	int nSub = 0;
	int nSubLev =  5;

	if (1 == GetLev())
	{
		//Ŀǰ����ɢ
		return;
// 		SetLev(0, 0);
// 		SetPay(25);
// 		m_pData->SetInt(SYNDATA_STATE, 3);
	}
	else
	{
		SetLev(0, GetLev() - 1);
		///////////////////////�Ǹ��������Ǹ��þ�ȥ��
		if (GetInt(SYNDATA_SUBLEV1) !=0 && GetInt(SYNDATA_SUBLEV1) <  nSubLev && ((GetInt(SYNDATA_SUB1_START) & 1) == 0))
		{
			nSub = 1;
			nSubLev = GetLev(nSub);
		}
		if (GetInt(SYNDATA_SUBLEV2) !=0 && GetInt(SYNDATA_SUBLEV2) <  nSubLev && ((GetInt(SYNDATA_SUB1_START) & 2) == 0))
		{
			nSub = 2;
			nSubLev = GetLev(nSub);
		}
		if (GetInt(SYNDATA_SUBLEV3) !=0 && GetInt(SYNDATA_SUBLEV3) <  nSubLev && ((GetInt(SYNDATA_SUB1_START) & 4) == 0))
		{
			nSub = 3;
			nSubLev = GetLev(nSub);
		}
		if (GetInt(SYNDATA_SUBLEV4) !=0 && GetInt(SYNDATA_SUBLEV4) <  nSubLev && ((GetInt(SYNDATA_SUB1_START) & 8) == 0))
		{
			nSub = 4;
			nSubLev = GetLev(nSub);
		}
		SetDeadline();		//////////////////////////�����ж����������޵�ֱ�Ӹɵ�
		if (GetSubAmount() > GetLev())
		{ 
			if(0 < nSub && 5 > nSub && nSubLev > 0)
			{
				SynManager()->DestroySynSub(GetID(), nSub);
			}
		}
		else
		{
			char szWords[128] = "";
			sprintf(szWords, "�������ڹ����ƣ�����Ϊ%d������з��ü������޽��͡�", GetLev());
			//	sprintf(szWords, "%s", szTempWords)
			CMsgTalk msg;
			if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWords, NULL, 0xff0000, _TXTATR_SYNDICATE))
				BroadcastSynMsg(&msg );
		}
	}
}

UINT CSyndicate::GetLev(int nSub /*=0*/)
{
	if (nSub < 0 || nSub > 4)
	{
		return 0;
	}
	return GetInt(SYNDATA(SYNDATA_LEVEL+nSub));

}

void CSyndicate::SetLev(int nSub, int nLev)
{
	if ((nSub < 0 || nSub > 4 )&& ( nLev<0 || nLev > 5) )
		return;
	if (m_pData->GetInt(SYNDATA(SYNDATA_LEVEL+nSub))!=nLev)
		m_pData->SetInt(SYNDATA(SYNDATA_LEVEL+nSub),nLev);//���õȼ���SYNDATA_LEVEL��Ȼ������Ϊ������
}

void CSyndicate::SetStopsub(int nSub)
{
	int nStop = GetInt(SYNDATA_SUB1_START) + pow((double)2, (int)(nSub-1));
	m_pData->SetInt(SYNDATA_SUB1_START, nStop);
	char szWords[256] = "";
	char szTempWords[128] = "";
	int Lev = GetLev();
	if (GetSubNameByID(nSub) != NULL)
	{
		sprintf(szTempWords, "%s������ʱ�رգ�", GetSubNameByID(nSub));
	}
	sprintf(szWords, "�������ڹ����ƣ�����Ϊ%d���%s���з��ü������޽��͡�", Lev, szTempWords);
	CMsgTalk msg;
	if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWords, NULL, 0xff0000, _TXTATR_SYNDICATE))
		BroadcastSynMsg(&msg );
	if(MainServerFlag() == 1)
	{
		m_pData->Update();
	}

}

void CSyndicate::ReSetStopsub(int nSub)
{
	if (nSub == 0)
		return;
	int nStop = GetInt(SYNDATA_SUB1_START) - pow((double)2, (int)(nSub - 1));
	if (nStop >= 0)
	{
		m_pData->SetInt(SYNDATA_SUB1_START, nStop);
	}
	char szWords[128];
	sprintf(szWords, "%s���¿���", GetSubNameByID(nSub));
	CMsgTalk msg;
	if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWords, NULL, 0xff0000, _TXTATR_SYNDICATE))
		BroadcastSynMsg(&msg );
	if(MainServerFlag() == 1)
	{
		m_pData->Update();
	}
}

const __int64 CSyndicate::GetDeadline(int nThing, int Sub)//1�Ƿ��ٽ��� 2�Ǽ��� 3�ǰ��ɵȼ� 4�Ǹ��ֹ������� 5���ʽ�
{
	if (nThing == 1)
	{
		if (GetLev()>10)
			return 0;
		return Grows[GetLev()];
	}

	else	if (nThing == 2)
	{
		if (GetLev(Sub)==0 || GetLev(Sub) > 10)
			return 0;
		return GetLev(Sub)*10;
	}

	else	if (nThing == 3)
	{
		return GetLev();
	}

	else	if (nThing == 4)
	{
		return 0;//GetLev() * 30 - 20;//����һ���㷨���ɶ���û��ô�ߣ������һ��
	}
	else	if (nThing == 5)
	{
		if (GetLev()>10)
			return 0;
		return Moneys[GetLev()]*MONEY_BASIC;
	}
	else
		return 0;
}

void CSyndicate::SetDeadline()////////////////////////////////////////////////���õ���
{
	//////////////////////////////////////////////////////////////////////////�����,���ٶ�,Ǯ
	if (GetInt(SYNDATA_GROW) > GetDeadline(1))
	{
		m_pData->SetInt(SYNDATA_GROW, GetDeadline(1));
	}
// 	if (GetInt(SYNDATA_ACTIVEAMOUNT) > GetDeadline(1))
// 	{
// 		m_pData->SetInt(SYNDATA_ACTIVEAMOUNT, GetDeadline(1));
// 	}
	if (GetInt(SYNDATA_MONEY) > GetDeadline(5))
	{
		m_pData->SetInt(SYNDATA_MONEY, GetDeadline(5));
	}

	//////////////////////////////////////////////////////////////////////////����
// 	if (GetInt(SYNDATA_SUB1_SKILL1) > GetDeadline(2, 1))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB1_SKILL1, GetDeadline(2, 1));
// 	}
// 	if (GetInt(SYNDATA_SUB1_SKILL2) > GetDeadline(2, 1))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB1_SKILL2, GetDeadline(2, 1));
// 	}
// 	if (GetInt(SYNDATA_SUB1_SKILL3) > GetDeadline(2, 1))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB1_SKILL3, GetDeadline(2, 1));
// 	}
// 
// 	if (GetInt(SYNDATA_SUB2_SKILL1) > GetDeadline(2, 2))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB2_SKILL1, GetDeadline(2, 2));
// 	}
// 	if (GetInt(SYNDATA_SUB2_SKILL2) > GetDeadline(2, 2))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB2_SKILL2, GetDeadline(2, 2));
// 	}
// 	if (GetInt(SYNDATA_SUB2_SKILL3) > GetDeadline(2, 2))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB2_SKILL3, GetDeadline(2, 2));
// 	}
// 
// 	if (GetInt(SYNDATA_SUB3_SKILL1) > GetDeadline(2, 3))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB3_SKILL1, GetDeadline(2, 3));
// 	}
// 	if (GetInt(SYNDATA_SUB3_SKILL2) > GetDeadline(2, 3))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB3_SKILL2, GetDeadline(2, 3));
// 	}
// 	if (GetInt(SYNDATA_SUB3_SKILL3) > GetDeadline(2, 3))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB3_SKILL3, GetDeadline(2, 3));
// 	}
// 
// 	if (GetInt(SYNDATA_SUB4_SKILL1) > GetDeadline(2, 4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB4_SKILL1, GetDeadline(2, 4));
// 	}
// 	if (GetInt(SYNDATA_SUB4_SKILL2) > GetDeadline(2, 4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB4_SKILL2, GetDeadline(2, 4));
// 	}
// 	if (GetInt(SYNDATA_SUB4_SKILL3) > GetDeadline(2, 4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB4_SKILL3, GetDeadline(2, 4));
// 	}

	//////////////////////////////////////////////////////////////////////////���õȼ�
	if (GetInt(SYNDATA_SUBLEV1) > GetDeadline(3))
	{
		m_pData->SetInt(SYNDATA_SUBLEV1, GetDeadline(3));
	}
	if (GetInt(SYNDATA_SUBLEV2) > GetDeadline(3))
	{
		m_pData->SetInt(SYNDATA_SUBLEV2, GetDeadline(3));
	}
	if (GetInt(SYNDATA_SUBLEV3) > GetDeadline(3))
	{
		m_pData->SetInt(SYNDATA_SUBLEV3, GetDeadline(3));
	}
	if (GetInt(SYNDATA_SUBLEV4) > GetDeadline(3))
	{
		m_pData->SetInt(SYNDATA_SUBLEV4, GetDeadline(3));
	}

	//////////////////////////////////////////////////////////////////////////��������
// 	if (GetInt(SYNDATA_SUB1_BUILDER) > GetDeadline(4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB1_BUILDER, GetDeadline(4));
// 	}
// 	if (GetInt(SYNDATA_SUB2_BUILDER) > GetDeadline(4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB2_BUILDER, GetDeadline(4));
// 	}
// 	if (GetInt(SYNDATA_SUB3_BUILDER) > GetDeadline(4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB3_BUILDER, GetDeadline(4));
// 	}
// 	if (GetInt(SYNDATA_SUB4_BUILDER) > GetDeadline(4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB4_BUILDER, GetDeadline(4));
// 	}
// 
// 	if (GetInt(SYNDATA_SUB1_KONGFU) > GetDeadline(4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB1_KONGFU, GetDeadline(4));
// 	}
// 	if (GetInt(SYNDATA_SUB2_KONGFU) > GetDeadline(4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB2_KONGFU, GetDeadline(4));
// 	}
// 	if (GetInt(SYNDATA_SUB3_KONGFU) > GetDeadline(4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB3_KONGFU, GetDeadline(4));
// 	}
// 	if (GetInt(SYNDATA_SUB4_KONGFU) > GetDeadline(4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB4_KONGFU, GetDeadline(4));
// 	}
// 
// 	if (GetInt(SYNDATA_SUB1_BUSMAN) > GetDeadline(4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB1_BUSMAN, GetDeadline(4));
// 	}
// 	if (GetInt(SYNDATA_SUB2_BUSMAN) > GetDeadline(4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB2_BUSMAN, GetDeadline(4));
// 	}
// 	if (GetInt(SYNDATA_SUB3_BUSMAN) > GetDeadline(4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB3_BUSMAN, GetDeadline(4));
// 	}
// 	if (GetInt(SYNDATA_SUB4_BUSMAN) > GetDeadline(4))
// 	{
// 		m_pData->SetInt(SYNDATA_SUB4_BUSMAN, GetDeadline(4));
// 	}
}

void CSyndicate::SetPay(int nPay)
{
	m_pData->SetInt(SYNDATA_PAY, nPay);
	return;
}
void CSyndicate::UpdateInfo(int nThing,OBJID nData,USHORT nSub)//0�ǽ��裬1���ʽ�2�ǵȼ�
{
	switch(nThing)
	{
	case 0:
		SetGrow(SPENDBUILD,nData,false);
		break;
	case 1:
		SetGrow(SPENDMONEY,nData,false);
		break;
	case 2:
		SetLev(nSub,nData);
		break;
	}
}
void CSyndicate::UpdateLevel(int sub)
{
	SetLev(sub,GetLev(sub)+1);
}
bool CSyndicate::UpLevel(CUserPtr pUser, int nSub)
{
	if (CanUpLevel(nSub,pUser))
	{
		return true;
	}
	return false;
}
bool CSyndicate::CanUpLevel(int nSub,CUserPtr pUser)
{
	if (GetInt(SYNDATA_STATE) == 3 || GetLev() < 1)
	{
		return false;
	}
	switch(nSub)
	{
		case 0:
			{
				if(pUser->ProcessAction(1014))
				{
					SetLev(0, (GetLev(0)+1));
					return true;
				}
				return false;
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			{
				LuaScriptMachine()->InitVar(nSub);
				if(pUser->ProcessAction(1015))
				{
					SetLev(nSub, (GetLev(nSub)+1));
					return true;
				}
				return false;
			}
			break;
	}
	return false;
}

bool CSyndicate::CheckCondition(/*CUserPtr pUser, */int nSub)
{
	UINT NeedMoney = 0;
	UINT NeedGrow = 0;

	if (GetInt(SYNDATA_STATE) < 2 || GetLev() < 1)
	{
		return false;
	}
	if (nSub == 0)
	{
		if (GetLev(1) < GetLev() && GetLev(2) < GetLev() && GetLev(3) < GetLev() && GetLev(4) < GetLev())
		{
			return false;
		}
		switch (GetLev())
		{
			case	1:
				{
					NeedMoney = 3000000;
					NeedGrow =  29000;
				}
			break;
			case	2:
				{
					NeedMoney = 50000000;
					NeedGrow =  460000;
				}
			break;
			case	3:
				{
					NeedMoney = 250000000;
					NeedGrow =  2400000;
				}
			break;
			case	4:
				{
					NeedMoney = 900000000;
					NeedGrow = 9200000;
				}
			break;
		}
		if(SpendMoney(SPENDMONEY, NeedMoney) && SpendMoney(SPENDBUILD, NeedGrow))
		{
			char szWords[128] = "";
			sprintf(szWords, "%s����������%d����ȣ�%d����", GetSubNameByID(nSub), NeedGrow,  NeedMoney/100);
			CMsgTalk msg;
			CMsgTalk Msg;
			if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWords, NULL, 0xff0000, _TXTATR_SYNDICATE))
				BroadcastSynMsg(&msg );
			sprintf(szWords, "%s�ȼ�������%d��", GetSubNameByID(nSub), (GetLev(nSub)+1));
			if (Msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWords, NULL, 0xff0000, _TXTATR_SYNDICATE))
				BroadcastSynMsg(&Msg );

			SetLev(nSub, (GetLev(nSub)+1));

			if (GetInt(SYNDATA_SUB1_START))
			{
				DEBUG_TRY
					if(GetLev() > GetSubAmount())
					{
						////���ҵ����б���з����////Ȼ���ҵ���Щ���еȼ���ߵ�
						int	nResetSub[4] = {1, 2, 4, 8};
						int nResetLev[4] = {0, 0, 0, 0};
						int nMaxSubLev = 0;
						int nMax = 0;
						for (int i=0; i<4; i++)
						{
							if ((GetInt(SYNDATA_SUB1_START) & nResetSub[i]) == 0)
							{
								nResetSub[i] = 0;
							}
							else
							{
								nResetLev[i] = GetLev(i+1);
								if (nMaxSubLev < nResetLev[i])
								{
									nMax = i+1;
									nMaxSubLev = nResetLev[i];
								}
							}
						}
						if(nMaxSubLev != 0)
							ReSetStopsub(nMax);
					}
				DEBUG_CATCH("ReSetStopsub in UpLevel ERROR!")
			}
		
			return true;
		}
	}
	else	if(nSub > 0 && nSub < 5)
	{
		switch(GetLev(nSub))
		{
		case	0:
			{
				NeedMoney = 60000;
				NeedGrow =  5000;
			}
			break;
		case	1:
			{
				NeedMoney = 1500000;
				NeedGrow =  14500;
			}
			break;
		case	2:
			{
				NeedMoney = 12500000;
				NeedGrow =  115000;
			}
			break;
		case	3:
			{
				NeedMoney = 42000000;
				NeedGrow =  400000;
			}
			break;
		case	4:
			{
				NeedMoney = 112500000;
				NeedGrow =  1150000;
			}
			break;
		}
		if (GetInt(SYNDATA_MONEY) < NeedMoney || GetInt(SYNDATA_GROW) < NeedGrow || GetSubAmount() >= GetLev())
			return false;
		if(SpendMoney(SPENDMONEY, NeedMoney) && SpendMoney(SPENDBUILD, NeedGrow)  && CheckSkillDeadline(nSub))
		{
			char szWords[128] = "";
			sprintf(szWords, "%s����������%d����ȣ�%d����", GetSubNameByID(nSub), NeedGrow, NeedMoney/100, (GetLev(nSub)+1));
			CMsgTalk msg;
			CMsgTalk Msg;
			if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWords, NULL, 0xff0000, _TXTATR_SYNDICATE))
				BroadcastSynMsg(&msg );
			sprintf(szWords, "%s�ȼ�������%d��", GetSubNameByID(nSub), (GetLev(nSub)+1));
			if (Msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWords, NULL, 0xff0000, _TXTATR_SYNDICATE))
				BroadcastSynMsg(&Msg );

			SetLev(nSub, (GetLev(nSub)+1));
			return true;
		}
	}
	else
		return false;
	return false;
}

bool CSyndicate::SpendMoney(int Syndata, UINT ndata)
{
	if (Syndata == SPENDMONEY && GetInt(SYNDATA_MONEY) >= ndata)
	{
		int nData = GetInt(SYNDATA_MONEY) - ndata;
		if(ndata >= 0)
		{
			SetGrow(SPENDMONEY, nData);
			return true;
		}
		else
			return false;
	}
	else if (Syndata == SPENDBUILD && (GetInt(SYNDATA_GROW) >= ndata))
	{
		int nData = GetInt(SYNDATA_GROW) - ndata; 
		if (ndata >= 0)
		{
			SetGrow(SPENDBUILD, nData);
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

UINT CSyndicate::GetSkillExp(int nSub, int nSkill)
{
	UINT uRealExp = 0;
	switch(nSub)
	{
		case	1:
			{
				if (nSkill == 0)
				{
					nSkill = GetInt(SYNDATA_SUBGROW1);
					if (nSkill != 0)
					{
						uRealExp = GetSkillExp(nSub, nSkill);
					}
					else
						return 0;
				}
				else if (nSkill == 1)
					return GetInt(SYNDATA_SUB1_SKILL1);
				else if (nSkill == 2)
					return GetInt(SYNDATA_SUB1_SKILL2);
				else if (nSkill == 3)
					return GetInt(SYNDATA_SUB1_SKILL3);
			}
			break;
		case	2:
			{
				if (nSkill == 0)
				{
					nSkill = GetInt(SYNDATA_SUBGROW2);
					if (nSkill != 0)
					{
						uRealExp = GetSkillExp(nSub, nSkill);
					}
					else
						return 0;
				}
				else if (nSkill == 1)
					return GetInt(SYNDATA_SUB2_SKILL1);
				else if (nSkill == 2)
					return GetInt(SYNDATA_SUB2_SKILL2);
				else if (nSkill == 3)
					return GetInt(SYNDATA_SUB2_SKILL3);
			}
			break;
		case	3:
			{
				if (nSkill == 0)
				{
					nSkill = GetInt(SYNDATA_SUBGROW3);
					if (nSkill != 0)
					{
						uRealExp = GetSkillExp(nSub, nSkill);
					}
					else
						return 0;
				}
				else if (nSkill == 1)
					return GetInt(SYNDATA_SUB3_SKILL1);
				else if (nSkill == 2)
					return GetInt(SYNDATA_SUB3_SKILL2);
				else if (nSkill == 3)
					return GetInt(SYNDATA_SUB3_SKILL3);
			}
			break;
		case	4:
			{
				if (nSkill == 0)
				{
					nSkill = GetInt(SYNDATA_SUBGROW4);
					if (nSkill != 0)
					{
						uRealExp = GetSkillExp(nSub, nSkill);
					}
					else
						return 0;
				}
				else if (nSkill == 1)
					return GetInt(SYNDATA_SUB4_SKILL1);
				else if (nSkill == 2)
					return GetInt(SYNDATA_SUB4_SKILL2);
				else if (nSkill == 3)
					return GetInt(SYNDATA_SUB4_SKILL3);
			}
			break;
	}
	return uRealExp;
}

int CSyndicate::GetSubAmount()
{
	int nAmount = 0;
	if (GetLev(1) && ((GetInt(SYNDATA_SUB1_START) & 1) == 0))
	{
		nAmount++;
	}
	if (GetLev(2) && ((GetInt(SYNDATA_SUB1_START) & 2) == 0))
	{
		nAmount++;
	}
	if (GetLev(3) && ((GetInt(SYNDATA_SUB1_START) & 4) == 0))
	{
		nAmount++;
	}
	if (GetLev(4) && ((GetInt(SYNDATA_SUB1_START) & 8) == 0))
	{
		nAmount++;
	}
	return nAmount;
}

bool CSyndicate::CheckSkillDeadline(int nSub)
{
	bool bResult = false;
	if (nSub < 0 || nSub > 4)
	{
		return false;
	}
	if (nSub == 0)
	{
		for (int i = 1; i < 5; i++)
		{
			for (int j = 1; j < 4; j++)
			{
				if (GetLev(nSub) == 0)
				{
					bResult = true;
				}
				if (GetSkillExp(i, j) >= GetDeadline(2))
				{
					bResult = true;
				}
			}
		}
	}
	else
	{
		for (int j = 1; j < 4; j++)
		{
			if (GetLev(nSub) == 0)
			{
				bResult = true;
			}
			if (GetSkillExp(nSub, j) >= GetDeadline(2,nSub))
			{
				bResult = true;
			}
		}
	}
		return bResult;
}

bool CSyndicate::UpSubSkill(int nSub, int nSkill, char *szUserName)
{
	if (nSub <= 0 || nSub > 4)//0�������ã�����û����
	{
		return false;
	}

	if (nSkill < 0 || nSkill > 3)
	{
		return false;
	}

	m_pData->SetInt(SYNDATA(SYNDATA_GROW+nSub),nSkill); /*SYNDATA_GROW��12��SYNDATA_SUBGROW1��13��nSub��1��ʼ*/   
		return true;
}

UINT CSyndicate::SetGrow(int nType, UINT nData, bool bSendtoOtherline)
{
	UINT Result;
	if (nType == SPENDBUILD)
	{
		if (nData < GetDeadline(1))
		{
			m_pData->SetInt(SYNDATA_GROW, nData);
			Result =  nData;
		}
		else 		
		{
			m_pData->SetInt(SYNDATA_GROW, GetDeadline(1));
			Result =  GetDeadline(1);
		}
		CMsgSynInfo msgupdate;//���Ϳͻ��˸���
		IF_OK(msgupdate.CreateUpdate(102,Result))
		BroadcastSynMsg(&msgupdate );	
	}

	else	if (nType == SPENDMONEY)
	{
		if (nData < GetDeadline(5))
		{
			m_pData->SetInt(SYNDATA_MONEY, (nData));
			Result =  nData;
		}
		else 		
		{
			m_pData->SetInt(SYNDATA_MONEY, GetDeadline(5));
			Result =  GetDeadline(5);
		}
		if ((double)m_pData->GetInt(SYNDATA_MONEY)/(double)GetDeadline(5,0)>=0.05)
			SetPay(0);
		else
			SetPay(1);
		CMsgSynInfo msgupdate;//���Ϳͻ��˸���
		IF_OK(msgupdate.CreateUpdate(101,Result))
		BroadcastSynMsg(&msgupdate );	
	}

	if (bSendtoOtherline)
	{
		int idSyn = this->GetID() * 10 + nType;
		CMsgSyndicate msgSyndicate;
		msgSyndicate.Create(SYN_SETBUILDOTHERLINE,Result , NULL,idSyn, "");
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSyndicate,(OBJID)0);
	}
	
	return Result;
	
}

void CSyndicate::SetSkill(int nSub, int nData, bool bSendtoOtherline)
{
// 	if (nData > GetDeadline(2, nSub))
// 	{
// 		nData = GetDeadline(2, nSub);
// 	}
// 	switch(nSub)
// 	{
// 	case 1:
// 		{
// 			if (GetInt(SYNDATA_SUBGROW1) == 1)
// 			{
// 				m_pData->SetInt(SYNDATA_SUB1_SKILL1, nData);
// 			} 
// 			else	if (GetInt(SYNDATA_SUBGROW1) == 2)
// 			{
// 				m_pData->SetInt(SYNDATA_SUB1_SKILL2, nData);
// 			} 
// 			else	if (GetInt(SYNDATA_SUBGROW1) == 3)
// 			{
// 				m_pData->SetInt(SYNDATA_SUB1_SKILL3, nData);
// 			} 
// 		}
// 		break;
// 	case 2:
// 		{
// 			if (GetInt(SYNDATA_SUBGROW2) == 1)
// 			{
// 				m_pData->SetInt(SYNDATA_SUB2_SKILL1, nData);
// 			} 
// 			else	if (GetInt(SYNDATA_SUBGROW2) == 2)
// 			{
// 				m_pData->SetInt(SYNDATA_SUB2_SKILL2, nData);
// 			} 
// 			else	if (GetInt(SYNDATA_SUBGROW2) == 3)
// 			{
// 				m_pData->SetInt(SYNDATA_SUB2_SKILL3, nData);
// 			} 
// 		}
// 		break;
// 	case 3:
// 		{
// 			if (GetInt(SYNDATA_SUBGROW3) == 1)
// 			{
// 				m_pData->SetInt(SYNDATA_SUB3_SKILL1, nData);
// 			} 
// 			else	if (GetInt(SYNDATA_SUBGROW3) == 2)
// 			{
// 				m_pData->SetInt(SYNDATA_SUB3_SKILL2, nData);
// 			} 
// 			else	if (GetInt(SYNDATA_SUBGROW3) == 3)
// 			{
// 				m_pData->SetInt(SYNDATA_SUB3_SKILL3, nData);
// 			} 
// 		}
// 		break;
// 	case 4:
// 		{
// 			if (GetInt(SYNDATA_SUBGROW4) == 1)
// 			{
// 				m_pData->SetInt(SYNDATA_SUB4_SKILL1, nData);
// 			} 
// 			else	if (GetInt(SYNDATA_SUBGROW4) == 2)
// 			{
// 				m_pData->SetInt(SYNDATA_SUB4_SKILL2, nData);
// 			} 
// 			else	if (GetInt(SYNDATA_SUBGROW4) == 3)
// 			{
// 				m_pData->SetInt(SYNDATA_SUB4_SKILL3, nData);
// 			} 
// 		}
// 		break;
// 	}
// 	if (bSendtoOtherline)
// 	{
// 		int idSyn = this->GetID() * 10 + 9;
// 		char szWord[4] = "";
// 		sprintf(szWord, "%d", nSub);
// 		m_pData->Update();
// 		CMsgSyndicate msgSyndicate;
// 		msgSyndicate.Create(SYN_SETBUILDOTHERLINE, nData, NULL, idSyn, szWord);
// 		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSyndicate,0);
// 	}
}

int CSyndicate::GetSkillLev(int nSub, int nSkill)
{
	UINT	ExpNow = GetSkillExp(nSub, nSkill);
	UINT	EXPEND = 0;
	int		nSkillLev = 0; 
	for(int i=1; i<=150; i++)
	{
		CLevupexpData* pLevupexp = LevupexpSet()->GetObj(LEVUPEXPDATA_SYNSKILL_EXP * _EXP_TYPE + i);
		if (!pLevupexp)
			return 0;
		UINT LevupExp	= pLevupexp->GetInt64(LEVUPEXPDATA_EXP);
		EXPEND += LevupExp;
		if (ExpNow < EXPEND)
		{
			nSkillLev = i - 1;
			break;
		}
	}
	return nSkillLev;
}

bool CSyndicate::IsStopSub(int nSub)
{
	if ((GetInt(SYNDATA_SUB1_START) & (int)pow((double)2, (int)nSub)) == 0)
		return false;
	return true;
}

DWORD CSyndicate::GetSkillExpDeadline(int nSub)
{
	int nLev = GetLev(nSub);
	int nDeadLev = 30 * nLev;
	UINT nDeadExp = 0;
	DEBUG_TRY
	for(int i=1; i<=nDeadLev; i++)
	{
		CLevupexpData* pLevupexp = LevupexpSet()->GetObj(LEVUPEXPDATA_SYNSKILL_EXP * _EXP_TYPE + i);
		if (!pLevupexp)
			return 0;
		nDeadExp += pLevupexp->GetInt64(LEVUPEXPDATA_EXP);
	}
	DEBUG_CATCH("GetSkillExpDeadline(int nSub)");
	return nDeadExp;
}
