// Magic.cpp: implementation of the CMagic class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4800)
#include "AllMsg.h"
#include "GameObj.h"
#include "Magic.h"
#include "MapGroup.h"
#include "StatusOnce.h"
#include "StatusMore.h"
#include "RoleManager.h"
#include "AiNpc.h"
#include "MsgMagicIntone.h"
#include "Array.h"
#include "ResourceWar.h"
#include "GiftTempleteManager.h"

char			szMagicTable[] = _TBL_MAGIC;
const OBJID		MIN_TEMPORARY_MAGICID	= (DWORD)40*10000*10000 + 1;
long			CMagic::m_lockNextID	= (long)MIN_TEMPORARY_MAGICID;		// temporary magic id, use for "next magic" id.

const int		PUBLICDELAYTIME	= 0;//����CD ms	// ������� [5/30/2007]
//////////////////////////////////////////////////////////////////////
unsigned char	Prof2MaskTable[] = {
	10,			// ħ��ʦ
	11,			// ħ��ʦתְ
	20,			// սʿ
	21,			// սʿתְ
	30,			// ������
	31,			// ������תְ
};

const int	MAX_USER_PROFS = sizeof(Prof2MaskTable)/sizeof(Prof2MaskTable[0]);


const DWORD MASK_ALLPROF	= 0;
DWORD	Prof2Mask(int nProf)		// return ERROR_MASK : error
{
	for(int i = 0; i < MAX_USER_PROFS; i++)
		if(Prof2MaskTable[i] == nProf)
			return 1 << i;
	return 0;				// ����ְҵ
}

MYHEAP_IMPLEMENTATION(CMagicData,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMagicData::~CMagicData()
{
	SAFE_RELEASE (m_pData);	
}
int CMagicData::NeedSend()
{	
	return needsend;
}

//////////////////////////////////////////////////////////////////////
IMagicData* IMagicData::CreateNew()
{
	// ȱʡ�Ǵ���CMagicData
	return CMagicData::CreateNew();
}

//////////////////////////////////////////////////////////////////////
bool CMagicData::Create(IRecordset* pDefaultRecord, OBJID idNewKey)
{
	m_pData	= CMagicDataBase::CreateNew();
	if (m_pData && m_pData->Create(pDefaultRecord, idNewKey))
	{
		return true;
	}

	SAFE_RELEASE (m_pData);
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagicData::Create(IRecordset* pRecordset/*,IDatabase* pDb*/)
{
	m_pData	= CMagicDataBase::CreateNew();
	if (m_pData && m_pData->Create(pRecordset))
	{
		SetDelay();

		needsend = 0;
		SYSTEMTIME thisTime;
		char thedata[24];
		pRecordset->LoadString(thedata, "lastuse", 24);	
		int dataY,dataM,dataD,dataH,dataMo,dataS;
		if(6 == sscanf(thedata,"%d-%d-%d-%d-%d-%d",&dataY,&dataM,&dataD,&dataH,&dataMo,&dataS))
		{
			::GetLocalTime(&thisTime);
			//DWORD dwCDTime	= 0;
			int iType	= m_pData->GetInt( MAGICDATA_TYPE );
			int iLevel	= m_pData->GetInt( MAGICDATA_LEVEL );	
		
			
			int time = 0,temp1 =0,temp2 =0,temp3 =0;
			if(dataY > 0)
			{
				if(dataM == thisTime.wMonth  &&  dataD == thisTime.wDay  &&  dataH <= thisTime.wHour)
				{
					temp1 = thisTime.wHour   - dataH;
					temp2 = thisTime.wMinute - dataMo;
					temp3 = thisTime.wSecond - dataS;
					//�������
					time = temp1 * 60 * 60 + temp2 * 60 + temp3 ;
					if(time > 0 && dataY - time > 1)
					{
						needsend = dataY - time;
					}					
				}
			}
					
		}		
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagicData::Create(ST_MAGIC_DATA* pMagicData)
{
	ASSERT(!"CMagicData::Create(ST_MAGIC_DATA* pMagicData)");
	return false;
}

//////////////////////////////////////////////////////////////////////
int CMagicData::GetInt(MAGICDATA nDataName)
{
	//	CHECKF(m_pType);
	CHECKF (m_pData);
	

	if(nDataName < MAGICTYPEDATA_OFFSET)
		return m_pData->GetInt(nDataName);

	if(m_pType == NULL)
	{
		IF_NOT(FindType())
		{
			LOGERROR("ħ���Ҳ�����͡�type[%d],level[%d]", m_pData->GetInt(MAGICDATA_TYPE), m_pData->GetInt(MAGICDATA_LEVEL));
			return false;
		}
	}
	
	return m_pType->GetInt((MAGICTYPEDATA)(nDataName - MAGICTYPEDATA_OFFSET));
}

//////////////////////////////////////////////////////////////////////
void CMagicData::SetInt(MAGICDATA idx, int nData)
{
	ASSERT(idx < MAGICTYPEDATA_OFFSET);

	if(nData == GetInt(idx))
		return;

	m_pData->SetInt(idx, nData);
	m_bNeedUpdate = true;
}

void CMagicData::SetStr(MAGICDATA idx, char* szData,int nSize)
{
	ASSERT(idx < MAGICTYPEDATA_OFFSET);

	m_pData->SetStr(idx, szData , nSize);
	m_bNeedUpdate = true;
}

int CMagicData::GetMagicLevel()
{
	//[2010-08-02 goto]���ܱ��޸ģ�û���˸��ӵȼ�
	if (m_pData)
	{
		return m_pData->GetInt(MAGICDATA_LEVEL);
	}
	return 0;
}

int	CMagicTempData::GetMagicLevel()
{
	return 150;//[2010-08-02 goto]���м��ܶ���150��

}

bool CMagicData::Update()
{
	CHECKF(m_pData);
	if(m_bNeedUpdate)
	{
		m_pData->Update();
		m_bNeedUpdate = false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagicData::FindType()
{
	m_pType = CMagic::FindMagicType(m_pData->GetInt(MAGICDATA_TYPE)/*, m_pData->GetInt(MAGICDATA_LEVEL)*/);
	if(m_pType != NULL)
		return true;
	
	return false;
}

//////////////////////////////////////////////////////////////////////
// ���������MagicData.cpp��ʵ�ֵ�
// ���ھ��д����ͷ�ļ������Ƚϸ��ӣ���ʱ�򵥴���д������
bool CMagicTempData::FindType()
{
	m_pType = CMagic::FindMagicType(m_data.usType/*, m_data.usLevel + m_data.dwAddLevel*/);
	return (m_pType != NULL);
}

MYHEAP_IMPLEMENTATION(CMagic,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMagic::CMagic(PROCESS_ID idProcess, IRole* pRole)
	: m_nMagicState(MAGICSTATE_NONE), m_nApplyTimes(0), m_bTargetLocked(false)
{
	ASSERT (pRole);

	m_idProcess = idProcess;
	m_pOwner		= pRole;

	m_setData	= CMagicSet::CreateNew(true);
	m_pMagic	= NULL;

	//m_nRawDelay = MAGIC_DELAY - MAGIC_DECDELAY_PER_LEVEL*m_pOwner->GetProfessionLevel();
	//ASSERT(m_nRawDelay >= 500);

	//m_nRawDelay = 1000;

	m_tDelay.SetInterval( PUBLICDELAYTIME );
	//m_nDelay	= m_nRawDelay;//MulDiv(m_nRawDelay, AUTO_MAGIC_DELAY_PERCENT, 100);

	m_tIntone.Clear();
	m_tApply.Clear();
	m_nAttackSameAmount	= 0;

	m_nPublicCDTime = 0;
    m_nPublicCDInterval = 1000;
}

//////////////////////////////////////////////////////////////////////
CMagic::~CMagic()
{
	if(m_setData)
		m_setData->Release();
}

//////////////////////////////////////////////////////////////////////
int CMagic::GetDieMode()
{
	return HitByMagic()?DIE_MAGIC:(m_pOwner->IsBowman()?DIE_ARROWSHOOT:DIE_NORMAL);
}

//////////////////////////////////////////////////////////////////////
CMagic* CMagic::CreateNew(PROCESS_ID idProcess, IRole* pRole)
{
	CHECKF (pRole);
	
	CMagic* pMagic = new CMagic(idProcess, pRole);
	CHECKF(pMagic);

	pMagic->m_idProcess = idProcess;
	pMagic->m_pOwner		= pRole;

	return pMagic;
}

//////////////////////////////////////////////////////////////////////

int CMagic::CreateAllCd()
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(m_idProcess));
	CHECKF(m_setData);
	CHECKF(m_pOwner);
	
	int nAmount = 0;

	for(int i = m_setData->GetAmount()-1; i >= 0; i--)
	{
		IMagicData* pData = m_setData->GetObjByIndex(i);
		int nType		= pData->GetInt( MAGICDATA_TYPE );
		int nTime		= pData->NeedSend();
		if(nTime > 0)
		{			
			DWORD dwCDTime	= pData->GetInt( MAGICDATA_DELAY );
			CMsgCoolDown msg;
			if( msg.Create( m_pOwner->GetID(),
				MAGICCOOLDOWN_BEGIN,
				MAGIC_INFO,
				nType,
				nTime * 1000,
				PUBLICDELAYTIME ) ) //˽��CD�͹���CD
			{
				nAmount ++;
				m_pOwner->SendMsg( &msg );
				pData->QueryDelayObj().SetInterval(nTime*1000);
				pData->StartDelay();
			}
		}
	}

	return nAmount;
}
// login
//////////////////////////////////////////////////////////////////////
bool CMagic::CreateAll()
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(m_idProcess));
	CHECKF(m_setData);
	CHECKF(m_pOwner);

	SQLBUF	szSQL;

	int nFirJob=m_pOwner->GetProfession(),nSecJob = 0;
	CUser* pUser = NULL;
	if( m_pOwner->QueryObj( OBJ_USER,( void** )&pUser) )
	{
		nFirJob = pUser->GetProfession(1);
		nSecJob = pUser->GetProfession(2);
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE ownerid=%u", _TBL_MAGIC, m_pOwner->GetID());

	int nStep = 0;
	DEBUG_TRY
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL, false);
	CHECKF(pRes);
	nStep = 1;
	IMagicData* pDataTemp = NULL;
	set<OBJID> setType;
	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		nStep = 11;
		CMagicTypeData* pMagicType = CMagic::FindMagicType(pRes->GetInt(MAGICDATA_TYPE));
		if (!pMagicType)
		{
			pRes->MoveNext();
			continue;
		}

		DWORD need_prof = pMagicType->GetInt(MAGICTYPEDATA_NEED_PROFMASK);
		if(need_prof == 9999 
			||((need_prof & nFirJob) != 0) 
			||((need_prof & nSecJob) != 0))
		{
			nStep = 2;
			IMagicData* pData = IMagicData::CreateNew();
			if(!pData)
			{
				pRes->Release();
				return false;
			}
			
			
			nStep = 3;
			if(!pData->Create(pRes/*,pDb*/))
			{
				pData->ReleaseByOwner();
				pRes->Release();
				return false;
			}
			nStep = 4;
			//����Ƿ����ظ��ģ�����У�ȡ�ȼ��ߵ�
			pDataTemp = FindMagic(pData->GetInt(MAGICDATA_TYPE));
			if(pDataTemp)
			{
				if(setType.count(pData->GetInt(MAGICDATA_TYPE))==0)
					setType.insert(pData->GetInt(MAGICDATA_TYPE));
				nStep = 5;
				if(pData->GetMagicLevel() > pDataTemp->GetMagicLevel())
				{
					m_setData->DelObj(pDataTemp->GetID());
				}
				else
				{
					nStep = 6;
					pRes->MoveNext();
					continue;
				}
			}
			nStep = 7;
			m_setData->AddObj(pData);
		}
		pRes->MoveNext();
	}
	nStep = 8;
	pRes->Release();
	nStep = 9;
// 	if(setType.size()>0)
// 	{
// 		char szType[512]="";
// 		char szTemp[10]="";
// 		for(set<OBJID>::iterator it=setType.begin(); it!=setType.end(); it++)
// 		{
// 			sprintf(szTemp, "%d,", *it);
// 			strcat(szType, szTemp);
// 		}
// 		::LogSave("���%u���ظ�ѧϰ�ļ��ܣ�����type(%s)", m_pOwner->GetID(), szType);
// 	}
	if (!FindMagic(MAGIC_BACK))//�سǼ���
		LearnMagic(MAGIC_BACK,1,true,0,0,0);
	if (m_pOwner->GetSynID()!=ID_NONE)
	{
		if (!FindMagic(MAGIC_BACKSYN))//�ذ＼��
		LearnMagic(MAGIC_BACKSYN,1,true,0,0,0);
	}
	
	DEBUG_CATCH2("CMagic::CreateAll() load data error!nStep=%d", nStep)

	return true;
}

//////////////////////////////////////////////////////////////////////
int CMagic::SendInfoToClient()
{
	int nAmount = 0;
	for(int i = m_setData->GetAmount()-1; i >= 0; i--)
	{
		IMagicData* pData = m_setData->GetObjByIndex(i);
		if(pData && pData->GetInt(MAGICDATA_TYPE))
		{
			if(CMagic::FindMagicType(pData->GetInt(MAGICDATA_TYPE)/*,pData->GetMagicLevel()*/))
			{
				CMsgMagicInfo msg;
				IF_OK(msg.Create(m_pOwner->GetID(),
								pData->GetInt(MAGICDATA_TYPE),
								pData->GetMagicLevel(),
								pData->GetInt(MAGICDATA_EXP)))
				{
					nAmount ++;
					m_pOwner->SendMsg(&msg);
				}
			}
			else
			{
				LOGWARNING("��ɫѧϰ�˷Ƿ�ħ��: [%s][%d].", m_pOwner->GetName(), m_pOwner->GetID());
				m_setData->DelObj(pData->GetID());
			}
		}
		else
		{
			LOGWARNING("��ɫѧϰ�˷Ƿ�ħ��: [%s][%d]", m_pOwner->GetName(), m_pOwner->GetID());
		}
	}

	return nAmount;
}

//////////////////////////////////////////////////////////////////////
// MapGroup
//////////////////////////////////////////////////////////////////////
bool CMagic::SaveInfo()
{
	CHECKF(m_pOwner->IsUser());
	for(int i = 0; i < m_setData->GetAmount(); i++)
	{
		IMagicData* pData = m_setData->GetObjByIndex(i);
		IF_OK(pData)
		{
			pData->Update();
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::GetInfoByIndex(MagicInfoStruct* pInfo, int idx)
{
	CHECKF(pInfo);
	CHECKF(idx >= 0 && idx < m_setData->GetAmount());

	IMagicData* pData = m_setData->GetObjByIndex(idx);
	IF_OK(pData)
	{
		pInfo->id			= pData->GetID();
		pInfo->idOwner		= m_pOwner->GetID();
		pInfo->nType		= pData->GetInt(MAGICDATA_TYPE);
		pInfo->nLevel		= pData->GetInt(MAGICDATA_LEVEL);
		pInfo->dwExp		= pData->GetInt(MAGICDATA_EXP);
		pInfo->updateClock	= pData->GetDelayUpdateTime();// ������� [5/30/2007]
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::AppendMagic(const MagicInfoStruct* pInfo)
{
	IMagicData* pData = NULL;
// 	if (pInfo->id >= MIN_TEMPORARY_MAGICID)
// 	{
// 		pData = CMagicTempData::CreateNew();
// 		CHECKF (pData);
// 		ST_MAGIC_DATA	data;
// 		memset(&data, 0L, sizeof(ST_MAGIC_DATA));
// 		data.idMagic	= pInfo->id;
// 		IF_NOT (pData->Create(&data))
// 			return false;
// 	}
// 	else
	{
		pData = CMagicData::CreateNew();
		CHECKF(pData);
		IF_NOT(pData->Create(GameDataDefault()->GetMagicData(), pInfo->id))
			return false;
	}
	pData->SetInt(MAGICDATA_OWNERID, pInfo->idOwner);
	pData->SetInt(MAGICDATA_TYPE, pInfo->nType);
	pData->SetInt(MAGICDATA_LEVEL, pInfo->nLevel);
	pData->SetInt(MAGICDATA_EXP, pInfo->dwExp);
	pData->SetIndex(pInfo->usCDIndex);
	pData->ClearUpdateFlags();
	pData->FindType();
	pData->SetDelay();				//06.9.4�������,�����л���ͼ���CD����..,�趨���ʱ��
	//----------------begin----------------------------------------------------
	//  [5/30/2007]�������
	if( pInfo->updateClock > 0 )	//CD��
	{
		pData->QueryDelayObj().SetUpdateTime( pInfo->updateClock );
	}
	//----------------end------------------------------------------------------
	m_setData->AddObj(pData);
	return true;
}

int CMagic::GetAtkRange()
{
	//zengû��Ҫ��ͣ��д��־
	//CHECKF(m_pMagic);
	if(m_pMagic)
		return m_pMagic->GetInt(MAGICDATA_DISTANCE);
	else
		return 0;
}

//////////////////////////////////////////////////////////////////////
// const
//////////////////////////////////////////////////////////////////////
bool CMagic::IsImmunity(IRole* pRole, bool bSendMsg)
{
	CHECKF(m_pMagic);
	CHECKF(pRole);

	if(!m_pOwner->IsAtkable(pRole, bSendMsg))
		return true;
//07.5.18�������ע�ͣ���ע��---------begin--------------
//-------Ŀ���Ƿ��еģ��Լ����Ƿ��еģ��˼�������������ʹ�õģ����Ҳ��ǹ���Ŀ������
//	if(pRole->IsWing() && !m_pOwner->IsWing() && m_pMagic->GetInt(MAGICDATA_WEAPON_HIT) && m_pMagic->GetInt(MAGICDATA_WEAPON_SUBTYPE) != BOW_SUBTYPE)
//		return true;
//--------------------------------------end----------------
	//20070316����
	//return m_pOwner->IsImmunity(pRole);
	return false;
}

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
IMagicData* CMagic::FindMagic(int nType)
{
	for(int i = 0; i < m_setData->GetAmount(); i++)
	{
		IMagicData* pData = m_setData->GetObjByIndex(i);
		if(pData && pData->GetInt(MAGICDATA_TYPE) == nType)
			return pData;
	}
	return NULL;
}

int CMagic::GetMagicScore()
{
	int nAllScore = 0;
	for(int i = 0; i < m_setData->GetAmount(); i++)
	{
		IMagicData* pData = m_setData->GetObjByIndex(i);
		if(pData)
		{
			CMagicTypeData* pType = FindMagicType(pData->GetInt(MAGICDATA_TYPE));
			if(pType)
				nAllScore += pType->GetInt(MAGICTYPEDATA_NEED_COST_LEV) * pData->GetInt(MAGICDATA_LEVEL);
		}
	}
	return nAllScore;
}

//////////////////////////////////////////////////////////////////////
CMagicTypeData* CMagic::FindMagicType(int nType/*, int nLevel*/)
{
	for(int i = 0; i < MagicTypeSet()->GetAmount(); i++)
	{
		CMagicTypeData* pType = MagicTypeSet()->GetObjByIndex(i);
		if(pType && pType->GetInt(MAGICTYPEDATA_TYPE) == nType/* && pType->GetInt(MAGICTYPEDATA_LEVEL) == nLevel*/)
			return pType;
	}
	return NULL;
}

//goto ���Ӽ���������
bool CMagic::AwardExp(int nType, int nExp)
{
	if (nExp==0)
		return true;
	IMagicData* pData = FindMagic(nType);
	if (!pData)
		return false;
	int nInitExp = pData->GetInt(MAGICDATA_EXP);
	int nNewExp = nInitExp+nExp;
	if (nInitExp>=1000000)
	{
		m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "�����������Ѵ�����");
		return false;
	}
	if (nType>=6001 && nType<=6003)//�ɼ�
	{
		if (nInitExp>=200*pow((double)2,(int)pData->GetInt(MAGICDATA_LEVEL)-1))
			return false;

		if (nNewExp>=200*pow((double)2,(int)pData->GetInt(MAGICDATA_LEVEL)-1))
			nNewExp = 200*pow((double)2,(int)pData->GetInt(MAGICDATA_LEVEL)-1);
	}
	else if (nType>=6004 && nType<=6006)//����
	{
		int nNeedExp = 0;
		switch(pData->GetInt(MAGICDATA_LEVEL))
		{
		case 1:nNeedExp = 120; break;
		case 2:nNeedExp = 300; break;
		case 3:nNeedExp = 600; break;
		case 4:nNeedExp = 1250; break;
		case 5:nNeedExp = 2500; break;
		case 6:nNeedExp = 5000; break;
		case 7:nNeedExp = 10000; break;
		case 8:nNeedExp = 20000; break;
		default:return false;
		}
		
		if (nInitExp >= nNeedExp)
		{
			m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "�����������Ѵﵽ��ǰ�ȼ�����,������");
			return false;
		}
		if (nNewExp >= nNeedExp)
		{
			nNewExp = nNeedExp;
			nExp = nNeedExp - nInitExp;
		}
		char szTemp[64] = "";
		if (nType==6004)
			sprintf(szTemp, "���켼������������%d", nExp);
		else if (nType==6005)
			sprintf(szTemp, "��Ƥ��������������%d", nExp);
		else if (nType==6006)
			sprintf(szTemp, "������������������%d", nExp);
		if (szTemp && strlen(szTemp)>0)
			m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
	}
	
	pData->SetInt(MAGICDATA_EXP, nNewExp);

	CMsgMagicInfo msg;
	IF_OK(msg.Create(m_pOwner->GetID(),
		nType,
		pData->GetMagicLevel(),
		pData->GetInt(MAGICDATA_EXP),
		_MAGICINFO_AWARDEXP))
	m_pOwner->SendMsg(&msg);

	return true;
}
//////////////////////////////////////////////////////////////////////
bool CMagic::AwardExp(int nType, int nBattleExp, int nExp)
{
	IMagicData* pData = FindMagic(nType);
	if (!pData)
		return false;

	IMagicData* pMagic = m_pMagic;
	m_pMagic	= pData;

	bool bRet = false;
	DEBUG_TRY
	bRet = AwardExp(nBattleExp, nExp, true);
	DEBUG_CATCH("CMagic::AwardExp(nBattleExp, nExp)")
	m_pMagic	= pMagic;
	return bRet;
}

//////////////////////////////////////////////////////////////////////
// bool CMagic::AwardExp(int nBattleExp, int nExp, bool bIgnoreFlag)
// {
// 	CHECKF(m_pMagic);
// 
// 	if(nExp <= 0)
// 		return false;
// 	
// 	if(m_pOwner->GetMap()->IsTrainMap())
// 	{
// 		if(nBattleExp)
// 		{
// 			if(m_pOwner->IsBowman())
// 				nBattleExp	/= 2;
// 			nBattleExp = ::CutTrail(1, MulDiv(nBattleExp, GOAL_EXP_PERCENT, 100));
// 		}
// 	}

	// ս������ֵ
// 	if(nBattleExp)
// 		m_pOwner->AwardBattleExp(nBattleExp);

// 	if(m_pMagic->GetInt(MAGICDATA_NEED_EXP) > 0									// >0 : ��������
// 		&& ((m_pMagic->GetInt(MAGICDATA_AUTO_ACTIVE)&AUTOACTIVE_EXPDISABLE) == 0)	// ���Եõ�����
// 			|| bIgnoreFlag)
// 	{
// 		// ....
// 		int	nAllExp = m_pMagic->GetInt(MAGICDATA_EXP) + nExp;
// 		m_pMagic->SetInt(MAGICDATA_EXP, nAllExp);
// 
// 		// inform client
// 		if ((m_pMagic->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_HIDDEN) == 0)
// 		{
// 			CMsgFlushExp	msg;
// 			IF_OK(msg.Create(MSGFLUSHEXP_MAGIC, m_pMagic->GetInt(MAGICDATA_TYPE), nAllExp))
// 				m_pOwner->SendMsg(&msg);
// 		}
// 		UpLevel(SYNCHRO_TRUE);
// 		return true;
// 	}
// 	return false;
// }

static DWORD CalcOtherSkillExp(DWORD lev)
{
	switch(lev)
	{
	case 1:
		return 120; 
		break;
	case 2:
		return 300; 
		break;
	case 3:
		return 600; 
		break;
	case 4:
		return 1250; 
		break;
	case 5:
		return 2500; 
		break;
	case 6:
		return 5000; 
		break;
	case 7:
		return 10000; 
		break;
	case 8:
		return 20000; 
		break;
	case 9:
		return 40000; 
		break;
	default:
		return 0;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::UpLevel(IMagicData* pMagic,int nLev, bool bSynchro)
{
	CHECKF(pMagic);

	if (pMagic->GetInt(MAGICDATA_TYPE)==MAGIC_BACK||pMagic->GetInt(MAGICDATA_TYPE)==MAGIC_BACKSYN)
		return false;

	CUser* pUser = NULL;
	if (!m_pOwner->QueryObj( OBJ_USER,( void** )&pUser))
		return false;

	CHECKF(pUser);

	//����
	DWORD dwCurMagic = pMagic->GetInt(MAGICDATA_TYPE);
	CMagicTypeData* pMagicType = CMagic::FindMagicType(dwCurMagic);
	if (!pMagicType)
		return ID_NONE;


	DWORD dwCurLev = pMagic->GetInt(MAGICDATA_LEVEL);
	CHECKF(dwCurLev > 0);
	if (dwCurLev  >= 150)
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����Ѵﵽ��ߵȼ�");
		return false;
	}

	if (nLev  > 150)
	{
		nLev = 150;
	}

	CHECKF(nLev > dwCurLev);

	DWORD dwExp = 0;
	DWORD dwMoney = 0;
	DWORD nCostSyn = 0;//�۰ﹱ
	if (dwCurMagic >= 6000 && dwCurMagic < 7000)//�����
	{
		if (pUser->GetLev()<20)
		{
			pUser->SendSysMsg(_TXTATR_TOPSHOW, "����ȼ�����");
			return false;
		}

		if (dwCurMagic >= 6001 && dwCurMagic <= 6003)//�ɼ�
		{
			if (dwCurLev >= 10)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����Ѿ��ﵽ��ߵȼ�");
				return false;
			}

			//�ɼ�����������ȼ��ﵽ20����ʼ����ѧϰ������ȼ�ÿ��5�����Զ�ѧһ��
			int nMaxLev = min(10,(pUser->GetLev()-20)/5+1);
			if (nMaxLev <= dwCurLev)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "����ȼ�����");
				return false;
			}
			else
				nLev = min(nLev ,nMaxLev);

			if (pMagic->GetInt(MAGICDATA_EXP) < 200 * pow((double)2,(int)nLev-2))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���������Ȳ���");
				return false;
			}
		}
		else if (pMagic->GetInt(MAGICDATA_TYPE)>=6004 && pMagic->GetInt(MAGICDATA_TYPE)<=6006)//����
		{
			if (dwCurLev >= 10)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����Ѿ��ﵽ��ߵȼ�");
				return false;
			}
			
			//�ɼ�����������ȼ��ﵽ20����ʼ����ѧϰ������ȼ�ÿ��5�����Զ�ѧһ��
			int nMaxLev = min(10,(pUser->GetLev()-20)/5+1);
			if (nMaxLev <= dwCurLev)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "����ȼ�����");
				return false;
			}
			else
				nLev = min(nLev ,nMaxLev);

			if (pMagic->GetInt(MAGICDATA_EXP) < CalcOtherSkillExp(nLev-1))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���������Ȳ���");
				return false;
			}
		}

		for (int i = dwCurLev + 1 ; i < nLev + 1 ; i ++ )
		{
			dwExp += i * 100000;
			dwMoney += i * 100000;
		}
	}
	else if (dwCurMagic > 2100 && dwCurMagic < 2500)//���ɼ���
	{
		if (!pUser->GetSyndicate())//û����
		{
			pUser->SendSysMsg("�㻹û�н���");
			return false;
		}
		if (pUser->GetSyndicate()->NoPay())//����ֹͣά��
		{
			pUser->SendSysMsg("������ͣά����");
			return false;
		}

		if ((pUser->GetSyndicate()->GetLev((pMagic->GetInt(MAGICDATA_TYPE)/100)%10)*20) < nLev)
		{
			pUser->SendSysMsg("�õȼ�����");
			return false;
		}


		if (nLev > (pUser->GetLev()+10))
		{
			pUser->SendSysMsg(_TXTATR_TOPSHOW, "���ܵȼ����ܳ�������ȼ�10������");
			return false;
		}
		
		for (int i = dwCurLev + 1 ; i < nLev + 1 ; i ++ )
		{
			CLevupexpData* pLevSyn = LevupexpSet()->GetObj(EXP_TYPE_SYNSKILLCON * _EXP_TYPE + i);
			if (!pLevSyn)
				return false;
			CLevupexpData* pLevMoney = LevupexpSet()->GetObj(EXP_TYPE_SYNMONEY * _EXP_TYPE +i);
			if (!pLevMoney)
				return ID_NONE;
			nCostSyn += (pMagicType->GetInt(MAGICTYPEDATA_NEED_COST_LEV)*pLevSyn->GetInt(LEVUPEXPDATA_EXP))/10 ;
			dwMoney += (pLevMoney->GetInt(LEVUPEXPDATA_EXP) * pMagicType->GetInt(MAGICTYPEDATA_NEED_COST_LEV))/10;
		}
		int nVipLev = 0;
		if (pUser->IsVipActive())
			nVipLev = pUser->GetVipLev();
		if (nVipLev==1 || nVipLev==4)//VIP�ۿ�
		{
			dwMoney = dwMoney * (MAGIC_VIP_LEARN1 / 100.0);
		}
		else if (nVipLev==2)
			dwMoney = dwMoney * (MAGIC_VIP_LEARN2 / 100.0);
		else if (nVipLev==3)
			dwMoney = dwMoney * (MAGIC_VIP_LEARN3 / 100.0);
// 
// 		if (pUser->GetSynCon()<nCostSyn)
// 		{
// 			CLevupexpData* pLevSyn = LevupexpSet()->GetObj(7000+nLev);
// 			if (!pLevSyn)
// 				return ID_NONE;
// 			nCostSyn += pLevSyn->GetInt(LEVUPEXPDATA_EXP) * pMagicType->GetInt(MAGICTYPEDATA_NEED_COST_LEV);
// 		}

		
	}
	else
	{
		for (int i = dwCurLev + 1 ; i < nLev + 1 ; i ++ )
		{
			CLevupexpData* pLevExp = LevupexpSet()->GetObj(EXP_TYPE_SKILLEXP * _EXP_TYPE+i);
			if (!pLevExp)
				return ID_NONE;
			CLevupexpData* pLevMoney = LevupexpSet()->GetObj(EXP_TYPE_SKILLMONEY * _EXP_TYPE+i);
			if (!pLevMoney)
				return ID_NONE;
			
			dwExp += pLevExp->GetInt(LEVUPEXPDATA_EXP) * pMagicType->GetInt(MAGICTYPEDATA_NEED_COST_LEV);
			dwMoney += pLevMoney->GetInt(LEVUPEXPDATA_EXP) * pMagicType->GetInt(MAGICTYPEDATA_NEED_COST_LEV);
		}
		
		int nVipLev = 0;
		if (pUser->IsVipActive())
			nVipLev = pUser->GetVipLev();
		if (nVipLev==1 || nVipLev==4)//VIP�ۿ�
		{
			dwMoney = dwMoney * (MAGIC_VIP_LEARN1 / 100.0);
		}
		else if (nVipLev==2)
			dwMoney = dwMoney * (MAGIC_VIP_LEARN2 / 100.0);
		else if (nVipLev==3)
			dwMoney = dwMoney * (MAGIC_VIP_LEARN3 / 100.0);

		//��ɫ����ѧϰ����߼��ܵȼ�=��ɫ�ȼ�+10
		if (pMagic->GetInt(MAGICDATA_NEED_PROFMASK)!=9999)
		{
			if (nLev > (pUser->GetJobLev()+10))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���ܵȼ����ܳ���ְҵ�ȼ�10������");
				return false;
			}
		}
		else
		{
			if (nLev > (pUser->GetLev()+10))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���ܵȼ����ܳ�������ȼ�10������");
				return false;
			}
		}
	}
	
	
	if (pUser->GetExp() < dwExp)
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "��ɫӵ�еľ��鲻�㣬����ѧϰ����");
		return ID_NONE;
	}
	if (pUser->GetMoney() < dwMoney)
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "�������㣬����ѧϰ����");
		return ID_NONE;
	}
	if (pUser->GetSynCon() < nCostSyn)
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "��ɫӵ�еİ��ɹ��׶Ȳ��㣬����ѧϰ����");
		return ID_NONE;
	}

	if(dwMoney > 0)
	{
		if(!pUser->SpendMoney(dwMoney, SYNCHRO_TRUE))//�۳���Ǯ
			return false;
	}

	if(dwExp > 0)
		pUser->SetAttrib(_USERATTRIB_EXP, pUser->GetExp()-dwExp, SYNCHRO_TRUE);//ͬ�����ﾭ��

	if (nCostSyn>0)
	{
		OBJID oldcon=pUser->GetSynCon();
		pUser->SetSynCon( oldcon - nCostSyn);
		if (nCostSyn >= 100)
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) oldcon=%u spend %u from upskill", pUser->GetID(), pUser->GetName(),oldcon, nCostSyn);
	}

	pMagic->SetInt(MAGICDATA_LEVEL, nLev);
	
	if(bSynchro)
	{
		CMsgMagicInfo msg;
		IF_OK(msg.Create(m_pOwner->GetID(), pMagic->GetInt(MAGICDATA_TYPE), nLev, pMagic->GetInt(MAGICDATA_EXP), _MAGICINFO_CHANGELEV))
			m_pOwner->SendMsg(&msg);
		char szTemp[128];
		sprintf(szTemp, "%s������%d��", pMagic->GetName(), nLev);
		m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
	}

	///�������ܸ���
	if (pMagic->GetInt(MAGICDATA_SORT)== 99)
		pUser->UpdateCalculatedProperty();

	DEBUG_TRY
	pUser->UpdateMissionMsgByMonsterType(200000 + pMagic->GetInt(MAGICDATA_TYPE));
	DEBUG_CATCH("pUser->UpdateMissionMsgByMonsterType(200000 + pMagic->GetInt(MAGICDATA_TYPE));")

	return true;
}
//////////////////////////////////////////////////////////////////////
// dwData=5106 [0][0][5][1][0][6] ����(+05, -06)
static POINT CalcOffsetPosFromDWORD(DWORD dwInfo)
{
	int nOffsetX, nOffsetY;
	int nSignX, nSignY;
	nSignX		= (dwInfo % 1000000)/100000;
	nSignY		= (dwInfo % 1000)/100;
	nOffsetX	= (dwInfo % 100000)/1000;
	nOffsetY	= (dwInfo % 100);
	if(nSignX != 0) nOffsetX = -nOffsetX;
	if(nSignY != 0) nOffsetY = -nOffsetY;
	
	POINT ptOffset;
	ptOffset.x		= nOffsetX;
	ptOffset.y		= nOffsetY;
	return ptOffset;
}

bool CMagic::TestMagicCastEnable(OBJID idMagicType,OBJID idTarget,int x,int y)
{
	//����CD����
	if(::TimeGet(TIME_MILLISECOND) - m_nPublicCDTime <= m_nPublicCDInterval)
	{
		return false;
	}

	IMagicData* pData = FindMagic(idMagicType);
	if(!pData)
	{
		return false;
	}

	char szError[512]={0};
	return CheckCondition(pData, idTarget, x, y,false,szError);
}
//	MAGICSTATE_NONE		= 0,		// ��״̬
//	MAGICSTATE_INTONE	= 1,		// ����״̬
//	MAGICSTATE_LAUNCH	= 2,		// ʩչ״̬
//	MAGICSTATE_DELAY	= 3,		// �ӳ�״̬
//////////////////////////////////////////////////////////////////////
//06.4.19�����޸�
bool CMagic::MagicAttack(int nType, OBJID idTarget, int x, int y, UCHAR ucAutoActive /*= 0*/,bool needLearn,int learnLev)
{
	int nStep = 0;
	DEBUG_TRY
	CUser* pCheckUser = NULL;
	
	if( m_pOwner->QueryObj( OBJ_USER,( void** )&pCheckUser) )//ĳUSERʩ�ż���
	{
	}
	else//����ż���
	{
		if (m_pOwner->GetID() >= EUDEMON_ID_FIRST && m_pOwner->GetID() <= EUDEMON_ID_LAST)
		{
			pCheckUser = m_pOwner->QueryOwnerUser();
			if (pCheckUser)
			{
				CMonster* pMonster = pCheckUser->QueryCallPet();//��ó�ս�ĳ���
				if (!pMonster)//û�г�ս�ĳ���
					return false;
				if (pMonster->GetID() != m_pOwner->GetID())//��ս�ĳ�����ܷż���
					return false;
			}
		}
	}
	nStep = 1;
	//����CD����
	if(::TimeGet(TIME_MILLISECOND) - m_nPublicCDTime <= m_nPublicCDInterval&& nType!=MAGIC_BACKSYN)
	{
		m_pOwner->SendSysMsg( _TXTATR_TOPSHOW, "���ܻ�����ȴ��" );
		return false;
	}

	if (PHYSICAL_ATT == m_nMagicState)
	{
		m_nMagicState = MAGICSTATE_NONE;
	}

	switch (m_nMagicState)
	{
	case MAGICSTATE_INTONE:
		{
#ifdef _DEBUG
			m_pOwner->SendSysMsg("Debug: ħ�������б���ϣ�׼��ʹ����ħ����");
#endif
			AbortMagic(true);		// true: send faild msg
		}
		break;
	case MAGICSTATE_LAUNCH:
		{
#ifdef _DEBUG
			m_pOwner->SendSysMsg("Debug: ħ��ʩչ�У�����ʩչ����ħ����");
#endif
			return false;
		}
		break;
	case MAGICSTATE_DELAY:
		{
#ifdef _DEBUG
			m_pOwner->SendSysMsg("Debug: ħ���ӳ�(����DELAY)��δ����������ʩչ����ħ����");
#endif
			return false;
		}
		break;
		case PHYSICAL_ATT:
		{
#ifdef _DEBUG
			m_pOwner->SendSysMsg("Debug: ��������δ����������ʩչ����ħ����");
#endif
			return false;
		}
		break;
	}

	m_bTargetLocked	= false;

	nStep = 2;
	IMagicData* pData = FindMagic(nType);
	if(!pData && needLearn)
	{
		LearnMagic(nType,learnLev,false);
		pData = FindMagic(nType);
	}
	if(!( pData))
	{
		///���ﲻ��������Ϊ
		if(m_pOwner->IsMonster()==false)
			LOGCHEAT("invalid magic type: %d, user[%s][%d]ucAutoActive[%d]", nType, m_pOwner->GetName(), m_pOwner->GetID(),ucAutoActive);
		AbortMagic(true);		// true: send faild msg
		return false;
	}
	if (m_pOwner->IsUser())//��Ϊ�и�ְҵ�����Խ���ְҵ���
	{
		if ((pData->GetInt(MAGICDATA_NEED_PROFMASK) != m_pOwner->GetProfession()) && pData->GetInt(MAGICDATA_NEED_PROFMASK)!=9999)//�������ܲ�����
			return false;
	}
	else if (m_pOwner->GetID() >= EUDEMON_ID_FIRST && m_pOwner->GetID() <= EUDEMON_ID_LAST)//����ż����ж�
	{
		if (pData && pData->GetInt(MAGICDATA_NEED_PROFMASK)!=2000)
			return false;
	}
	
	m_pMagic		= pData;
	m_idTarget		= idTarget;
	if(pData->GetInt(MAGICDATA_GROUND))
		m_idTarget	= ID_NONE;
	nStep = 3;
	m_nPublicCDInterval = m_pMagic->GetInt(MAGICDATA_PUBLICDELAY);
	m_nPublicCDInterval = AdjustMagicPower(m_nPublicCDInterval, 0, m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_PUBLICCD);
	m_nPublicCDTime = ::TimeGet(TIME_MILLISECOND);
	
	m_bAutoAttack	= true;
	
	if (MAGICSORT_COLLIDE == pData->GetInt(MAGICDATA_SORT))
		m_nData	= (int)idTarget;
	
	nStep = 4;
	char szError[512]={0};
	if(!CheckCondition(pData, idTarget, x, y,false,szError) )				//??? don't return false above this line.
	{

//#ifdef _DEBUG
		if(strlen(szError) > 0)
		{
			m_pOwner->SendSysMsg( _TXTATR_TOPSHOW, szError );
		}
//#endif
		AbortMagic(true);		// true: send faild msg
		return false;
	}

	m_pos.x			= x;
	m_pos.y			= y;


	nStep = 6;
	// spend resource
	IRole* pRole = RoleManager()->QueryRole(m_idTarget);
	
	if(true)
	{
		if(m_pMagic->GetInt(MAGICDATA_USE_MP) > 0)
		{
			int nSpendMp = m_pMagic->GetInt( MAGICDATA_USE_MP );
			IStatus* pStatus = m_pOwner->QueryStatus( STATUS_ERUPTPOT_PERCENT );//Ǳ�ܱ���
			if( pStatus )
			{
				nSpendMp = ::MulDiv( nSpendMp,pStatus->GetPower(),100 );
			}

			m_bIsSpendMp = false;
			this->nSpendMp = AdjustMagicPower(nSpendMp, 0, m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_MP);//����ӳ�
		}
		if(m_pMagic->GetInt(MAGICDATA_USE_ANG) > 0)
		{			
			m_bIsSpendAng = false;
			m_nSpendAng = m_pMagic->GetInt(MAGICDATA_USE_ANG);//ŭ��û�мӳ�
		}

	}

	nStep = 7;
	//[��;���� 2009.06.02]����Ǹ�������Ѫ�Լ��ǰ�����������
	try
	{
		CUser *pUser = static_cast<CUser *>(m_pOwner);
		CUser *pTarget = static_cast<CUser *>(m_pOwner->FindAroundRole(m_idTarget));
		if (pUser 
			&& pTarget  
			&& pTarget->IsGrayName()
			&& m_pMagic->GetInt(MAGICDATA_SORT) == MAGICSORT_RECRUIT
			&& CanRecurit(pTarget))
		{
			pUser->IsEvil() ? NULL : pUser->SetCrimeStatus();
		}
	}
	catch (...)
	{
		::LogSave("�������ô���!");
	}


	m_nMagicState	= MAGICSTATE_LAUNCH;
	m_nApplyTimes	= 1;

	nStep = 8;
	if (!Launch())	// ħ��ִ��ʧ�ܾͽ������������������ǰĿ���Ѿ�������Ŀ����ʧ�����
	{
		nStep = 9;
		LockTarget(false);
		m_tApply.Clear();
		//ResetDelay();//goto ����ʹ��ʧ�ܺ󲻿���������ȴ
	}
	else
	{
		nStep = 10;
		ResetDelay();
		//m_tApply.Startup(m_pMagic->GetApplyMS());
	}

	//modify code by �ֵ�Ԩ begin 2011.6.22
	if(m_pOwner && !m_pOwner->IsAlive())
	{
		IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
		if(pTarget)
			pTarget->Kill(m_pOwner, GetDieMode());
	}
	//modify code by �ֵ�Ԩ end

	return true;
	DEBUG_CATCH4("CMagic::MagicAttack,type:%d,target:%d,step=%d",nType, idTarget,nStep)
	return true;	// ����������Ѫ
}

//////////////////////////////////////////////////////////////////////
void CMagic::BreakAutoAttack()
{
	m_bAutoAttack	= false;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::AbortMagic(bool bSynchro)
{
	// ħ��ʩչ�����н�ֹ��ֹ ���� ��ʱ��ô�趨
	if (m_nMagicState == MAGICSTATE_LAUNCH)
	{
#ifdef ZLONG_DEBUG
		m_pOwner->SendSysMsg("Debug: ħ��ʩ�Ź����У�������ֹ��");
#endif
//		m_tApply.Clear();
		return false;
	}

	BreakAutoAttack();
	if (m_nMagicState == MAGICSTATE_DELAY)
	{
#ifdef ZLONG_DEBUG
		m_pOwner->SendSysMsg("Debug: ħ���ӳ��С�");
#endif
//		m_tDelay.Clear();
		return false;
	}
	
	m_pMagic = NULL;
	if (m_nMagicState == MAGICSTATE_INTONE)
	{
		m_tIntone.Clear();

#ifdef ZLONG_DEBUG
		m_pOwner->SendSysMsg("Debug: ħ����������ֹ��");
#endif
	}

	m_nMagicState	= MAGICSTATE_NONE;
	if(bSynchro)
	{
		CMsgAction msg;
		if (msg.Create(m_pOwner->GetID(), 0, 0, 0, actionAbordMagic))
			m_pOwner->SendMsg(&msg);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMagic::ShowMiss()
{
	CHECK(m_pMagic);

	CMsgMagicEffect	msg;
	if(m_pMagic->GetInt(MAGICDATA_GROUND))
	{
		if (!msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_pos.x, m_pos.y, m_pOwner->GetDir()))
			return;
	}
	else
	{
		if (!msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, 0, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
			return;
	}
	m_pOwner->BroadcastRoomMsg(&msg, true);
}

//////////////////////////////////////////////////////////////////////
//20070614����:���Ӵ�����Ϣ
//�ɲ���szError����.
bool CMagic::CheckCondition(IMagicData* pData, 
							OBJID idTarget, 
							int x, 
							int y, 
							bool bSpiritSkill,
							char *szError/*=NULL*/)
{
// delay
//	if(!m_tDelay.IsTimeOut(m_nDelay) && MAGICSORT_COLLIDE != pData->GetInt(MAGICDATA_SORT))
//����ע�����ϲ��޸�
	CHECKF(pData);
	CHECKF(m_pOwner);

	if( !m_tDelay.IsTimeOut() && pData->GetInt(MAGICDATA_TYPE)!=MAGIC_BACKSYN)
	{
		if(szError && m_pOwner && m_pOwner->IsUser())
			//sprintf(szError,g_objStrRes.GetStr(16089));//"����:ħ���ӳ�δ����"
			sprintf(szError,"����������ȴ��");//"����:ħ���ӳ�δ����"
		return false;
	}

	if (!pData->IsReady()&& pData->GetInt(MAGICDATA_TYPE)!=MAGIC_BACKSYN)	// ���ħ���ĵ����ӳ�ʱ��
	{
		if(szError && m_pOwner && m_pOwner->IsUser())
			//sprintf(szError,g_objStrRes.GetStr(16089));//"����:ħ���ӳ�δ����"
			sprintf(szError,"����������ȴ��");//"����:ħ���ӳ�δ����"
		return false;
	}

	//satan 2009.08.07 ����Ǿ���  ��ôȡ��һ�е�ͼ���ƣ��������Լ��ĵ�ͼ����
	IRole* pTarget = RoleManager()->QueryRole(idTarget);
	CUser* pUserPVP = NULL;
	bool isPVP = false;
	if(m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUserPVP)))
	{
		if(pTarget && pUserPVP)
		{
			if(pUserPVP->IsPVPStuts() && pUserPVP->GetPVPTargetID() == pTarget->GetID())
			{
				isPVP = true;
			}
		}

		//��Χħ�������޸�
		if(pUserPVP->IsPVPStuts() && pData->GetInt(MAGICDATA_CRIME))
		{
			isPVP = true;
		}
	}

	//20071010����:�г����򲻿���ħ������
	//------------------------

	//�г����� satan******************************
// 	if(!isPVP && m_pOwner->GetMap()->IsInRegionType(REGION_MARKET, m_pOwner->GetPosX(), m_pOwner->GetPosY()))
// 	{
// 		if(szError)
// 			sprintf(szError,g_objStrRes.GetStr(16090));//"���������:�г�����"
// 		return false;
// 	}
	//------------------------

	

	//PK�������� satan******************************
// 	if(!isPVP && m_pOwner->GetMap()->IsInRegionType(REGION_PK_PROTECTED, m_pOwner->GetPosX(), m_pOwner->GetPosY()))
// 	{
// 		if (!(pData->GetInt(MAGICDATA_MASKBIT) & MAGICMASKBIT_IGNOREPKDIABLE) )
// 		{
// 			if(pData->GetInt(MAGICDATA_CRIME))
// 			{
// 				if(szError)
// 					sprintf(szError,g_objStrRes.GetStr(16091));//"����:PK��������"
// 				return false;
// 			}
// 		}
// 	}

	// ֻ����Ҽ�� MP,EP,XP, weapon, ���޲������
	CUser* pUser = NULL;
	if(m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
	{
		if(true)
		{
			//06.12.25�����ӻ꼼�������ж�------------------------------
			if( bSpiritSkill )
			{
// 				if( pUser->GetSpiritAllValue() < pData->GetInt( MAGICDATA_USE_MP ) )
// 				{
// 					if(szError)
// 						sprintf(szError,g_objStrRes.GetStr(16092));//"����:�ǳ���������"
// 					return false;
// 				}
			}
			else
			{
				DWORD dwNeedMp = AdjustMagicPower(pData->GetInt( MAGICDATA_USE_MP ),0,pData->GetMagicLevel(),MAGICLEVADDTYPE_MP);
				IStatus* pStatus = m_pOwner->QueryStatus( STATUS_ERUPTPOT_PERCENT );//Ǳ�ܱ���
				if( pStatus )
				{
					dwNeedMp = ::MulDiv( dwNeedMp,pStatus->GetPower(),100 );
				}
				pStatus = m_pOwner->QueryStatus(STATUS_SPENDMP_UP);//��Ԫ���壬����������
				if (pStatus)
					dwNeedMp = dwNeedMp*1.2;
                // ����ϵͳ���Լ���������(cody)
                int nDecManaRate = UserJinmeiSysMgr()->GetJinmeiDecManaRate(pUser->GetID());
                dwNeedMp = dwNeedMp - dwNeedMp * nDecManaRate / 1000;

				if( m_pOwner->GetMana() < dwNeedMp )
				{
					if(szError)
						//sprintf(szError,g_objStrRes.GetStr(16093));//"����:ħ������"
						sprintf(szError,"�����㣬�޷�ʹ�ü���");//"����:ħ������"
					return false;
				}
				DWORD dwNeedAng = pData->GetInt( MAGICDATA_USE_ANG );
				if (pUser->GetSp() < dwNeedAng)
				{
					if(szError)
						sprintf(szError,"ŭ�����㣬�޷�ʹ�ü���");//"����:ŭ������"
					return false;
				}
			}
		}
	}
	// resource


	// check map
 	int nSort = pData->GetInt(MAGICDATA_SORT);
// 	if((nSort == MAGICSORT_CALLTEAMMEMBER || nSort == MAGICSORT_RECORDTRANSSPELL) && m_pOwner->GetMap()->IsFlyToDisable())
// 	{
// 		if(szError)
// 			sprintf(szError,g_objStrRes.GetStr(16094));//"����:���ɴ��͵ĵ�ͼ"
// 		return false;
// 	}
// 	if(m_pOwner->QueryTransformation() && nSort != MAGICSORT_TRANSFORM)
// 	{
// 		if(szError)
// 			sprintf(szError,g_objStrRes.GetStr(16096));//"����:�ǻû�����"
// 		return false;
// 	}
// 	if(m_pOwner->IsWing() && nSort == MAGICSORT_TRANSFORM)
// 	{
// 		if(szError)
// 			sprintf(szError,g_objStrRes.GetStr(16097));//"����:�û�����"
// 		return false;
// 	}
// 	if( m_pOwner->GetMap()->GetID() == g_ResourceWar.GetWarMapID() //��Դս��ͼ��ʹ�ü���
// 		|| m_pOwner->GetMap()->GetType() & MAPTYPE_NPCCHGMAP_ONLY)//20070719����:��npc����
// 	{
// 		if( MAGICSORT_SPACETIMEDOOR == nSort ) //������ʹ��ʱ��֮��
// 		{
// 			if(szError)
// 				sprintf(szError,g_objStrRes.GetStr(16098));//"����:����ʹ��ʱ��֮��"
// 			return false;
// 		}
// 	}

	// check distance
 	IRole* pRole = NULL;
	if (!pData->GetInt(MAGICDATA_GROUND))	//���ǵ�Чħ��
	{
		pRole = m_pOwner->FindAroundRole(idTarget);		// may be NULL
		if (!pRole)
		{
			//if(szError)
				//sprintf(szError,"�����Ŀ��");//"����:�����Ŀ��"
			return false;			// not find target
		}

		if (!pRole->IsAlive() && nSort != MAGICSORT_REBORN) 
		{
			//if(szError)
				//sprintf(szError,g_objStrRes.GetStr(17000));//"����:Ŀ��������"
				//sprintf(szError,"Ŀ��������");//"����:Ŀ��������"
			return false;
		}

// 		if( SPECIALSKILL_TYPE_PERISH1 == pData->GetInt( MAGICDATA_TYPE )
// 			|| SPECIALSKILL_TYPE_PERISH2 == pData->GetInt( MAGICDATA_TYPE )
// 			|| SPECIALSKILL_TYPE_PERISH3 == pData->GetInt( MAGICDATA_TYPE ) )//���⼼��,�������������Ŀ��Ѫ���Ƿ����20%
// 		{
// 			//20070720����:�������Ѫ���Ƿ����20%
// 			if(m_pOwner->GetLife()*100/m_pOwner->GetMaxLife() >= PERISH_TAGLIFE_PERCENT)
// 			{
// 				if(szError)
// 					sprintf(szError,g_objStrRes.GetStr(17001),PERISH_TAGLIFE_PERCENT);//"����:����Ѫ�������ڰٷ�֮%d"
// 				return false;
// 
// 			}
// //			if( pRole->GetLife() * 100 / pRole->GetMaxLife() >= PERISH_TAGLIFE_PERCENT )
// //			{
// //				if(szError)
// //					sprintf(szError,"����:Ѫ��������%d%%",PERISH_TAGLIFE_PERCENT);
// //				return false;
// //			}
// 			
// 		}
// 		// �������� [12/14/2007 �½���]------------------------------------------------------------
// 		if( SPECIALSKILL_TYPE_OPPOSE == pData->GetInt( MAGICDATA_TYPE ) )
// 		{
// 			if( m_pOwner->GetLife()*100/m_pOwner->GetMaxLife() >= OPPOSE_TAGLIFE_PERCENT )
// 			{
// 				if(szError)
// 					sprintf(szError,"����:Ѫ��������%d%%",OPPOSE_TAGLIFE_PERCENT);
// 				return false;
// 			}
// 		}
		//----------------------------------------------------------------------------------------
// 		if (nSort == MAGICSORT_ESCAPE)
// 		{
// 			if (pRole->GetLife()*100/pRole->GetMaxLife() >= MAGIC_ESCAPE_LIFE_PERCENT)
// 			{
// 				if(szError)
// 					sprintf(szError,g_objStrRes.GetStr(17002),MAGIC_ESCAPE_LIFE_PERCENT);//"����:Ѫ��������%d%%"
// 				return false;
// 			}
// 		}

// 		if (nSort == MAGICSORT_SORB_SOUL)
// 		{
// 			CMonster* pMonster = NULL;
// 			if (!pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
// 			{
// 				if(szError)
// 					sprintf(szError,g_objStrRes.GetStr(17003));//"����:Ŀ��ǹ���"
// 				return false;
// 			}
// 			if ((pMonster->GetMaskData() & MASK_DISABLE_SORB_SOUL) == MASK_DISABLE_SORB_SOUL)
// 			{
// 				if(szError)
// 					sprintf(szError,g_objStrRes.GetStr(17004));//"����:������ʹ������"
// 				return false;
// 			}
// 		}
// 		if (nSort == MAGICSORT_STEAL_MONEY)
// 		{
// 			CMonster* pMonster = NULL;
// 			if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
// 			{
// 				if ((pMonster->GetMaskData() & MASK_DISABLE_STEAL_MONEY) == MASK_DISABLE_STEAL_MONEY)
// 				{
// 					if(szError)
// 						sprintf(szError,g_objStrRes.GetStr(17005));//"����:������͵��"
// 					return false;
// 				}
// 			}
// 		}

		x = pRole->GetPosX();
		y = pRole->GetPosY();
	}
	if(m_pOwner->GetDistance(x, y) > pData->GetInt(MAGICDATA_DISTANCE) && pData->GetInt(MAGICDATA_TYPE)!=MAGIC_BACK)
	{
		if(szError)
			//sprintf(szError,g_objStrRes.GetStr(17006));//"����:������Χ"
			sprintf(szError, "Ŀ�����̫Զ");//"����:������Χ"

		CUser* pAutoFightUser = NULL;
		if( m_pOwner->QueryObj( OBJ_USER,( void** )&pAutoFightUser) )
		{
		 	if(pAutoFightUser->GetAutoFightFlag())
			{
				CMsgAutoFight msg;
				if(msg.Create(_AutoFight_OUTOFRANGE))
				{
					pAutoFightUser->SendMsg(&msg);
				}
			}
		}
		
		return false;
	}

	// check goal
	CNpc* pNpc = NULL;
	if(pRole)
		pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc));
	if(pNpc)
	{
		if(!pNpc->IsBeAtkable())
		{
			if(szError)
				sprintf(szError,g_objStrRes.GetStr(17007));//"����:Ŀ�겻�ɱ�����"
			return false;
		}
//		if (pNpc->IsGoal() || pNpc->GetType() != _MAGICGOAL_NPC) //���ܴ�����,ħ�����ӿ��Դ�
//		{
//			if(szError)
//				sprintf(szError,g_objStrRes.GetStr(17008));//"����:�����Թ�������"
//			return false;
//		}
	}

	// check recruit
// 	if(pRole && pData->GetInt(MAGICDATA_SORT) == MAGICSORT_RECRUIT)
// 	{
// 		CMonster* pMonster=NULL;
// 		pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster));
// 		if(pMonster && !pMonster->IsEudemon())			// && pMonster->IsSynPet()
// 		{
// 			//���岻��				
// // 	        if(m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
// // 			{
// // 				if(szError)
// // 				    sprintf(szError,"�����Ŀ��");//"����:�����Ŀ��"
// // 			    return false;
// // 			}			
// 		}
// 	}

//////////////////////////////////���¼��ܲ��ж��赲
/*
����		type	ְҵ
�ָ���		0102	3������Ĺ��֡�ǹ��
������		0302	����
����֮Ϣ	0303	����
��������	0307	����
����		0602	�취
Ⱥ����		0603	�취
�������	0604	�취
��������	0902	����
�����籩	0903	����
Ⱥ��������	0907	����*/
////////////////////////////////�ж��赲/////////////////////
// 	int nTypeID = pData->GetInt( MAGICDATA_TYPE );
// 	if( nTypeID != 102 && nTypeID != 302 && nTypeID != 303 &&
// 		nTypeID != 307 && nTypeID != 602 && nTypeID != 603 &&
// 		nTypeID != 604 && nTypeID != 902 && nTypeID != 903 && nTypeID != 907 &&
// 		m_pOwner != pRole )
// 	{
// 		if( !m_pOwner->IsArrowPass( x, y ) )
// 		{
// 			if(szError)
// 				sprintf(szError,g_objStrRes.GetStr(17010));//"����:������Ŀ��"
// 			return false;
// 		}
// 	}

	// ���ħ��ֻ�жӳ��ſ��Է���
//-----07.4.18����ע��-----begin---------------------
//	if (pData->GetInt(MAGICDATA_SORT) == MAGICSORT_TEAM_MAGIC)
//	{
//		CTeam* pTeam = m_pOwner->GetTeam();
//		if (!pTeam || pTeam->GetLeader() != m_pOwner->GetID() 
//			|| pTeam->GetMemberAmount() < TEAM_STATUS_REQ_ROLES)	// ������ᣬ�����б���������3��
//		{
//			return false;
//		}
//	}
//---------------------------end----------------------------

	//  [12/20/2007 ���]	�츳���ܲ�����״̬
// 	if(pData->GetInt(MAGICDATA_GIFTFLAG) == EM_GIFT_MAGIC_INVALID)
// 	{
// 		if(szError)
// 			sprintf(szError,"�츳���ܲ�����");
// 		return false;
// 	}


	//  [12/20/2007 ���]	Ŀ������
// 	if(!CRole::CheckDebuffLockTarget(m_pOwner, idTarget))
// 		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CheckCrime(IRole* pRole)
{
	CHECKF(m_pMagic);
	CHECKF(pRole);

	if(!m_pMagic->GetInt(MAGICDATA_CRIME))
		return false;

	return m_pOwner->CheckCrime(pRole);
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CheckCrime(const vector<IRole*>* pSet)
{
	CHECKF(m_pMagic);

	if(!m_pMagic->GetInt(MAGICDATA_CRIME))
		return false;

	for(int i = 0; i < pSet->size(); i++)
	{
		if(m_pOwner->CheckCrime(pSet->at(i)))
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::UserKillTarget(IRole* pTarget)
{
	for(int i = 0; i < m_setData->GetAmount(); i++)
	{
		IMagicData* pData = m_setData->GetObjByIndex(i);
// 		if( pData && (pData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_KILL)
// 				&& !m_pOwner->QueryTransformation()
// 				&& (!pData->GetInt(MAGICDATA_WEAPON_SUBTYPE) || m_pOwner->CheckWeaponSubType(pData->GetInt(MAGICDATA_WEAPON_SUBTYPE)))
// 				&& ::RandGet(100) < pData->GetInt(MAGICDATA_PERCENT) )
// 		{
// 			// backup
// 			IMagicData*	pMagic		= m_pMagic;
// 			OBJID		idTarget	= m_idTarget;
// 			POINT		pos;
// 			pos.x			= m_pos.x;
// 			pos.y			= m_pos.y;
// 
// 			switch(pData->GetInt(MAGICDATA_SORT))
// 			{
// 			case	MAGICSORT_RECRUIT:
// 				{
// 					m_pMagic		= pData;
// 					m_idTarget		= m_pOwner->GetID();
// 					m_pos.x			= m_pOwner->GetPosX();
// 					m_pos.y			= m_pOwner->GetPosY();
// 					ProcessRecruit();
// 				}
// 				break;
// 			case	MAGICSORT_SERIALCUT:
// 				{
// 					m_pMagic		= pData;
// 					m_idTarget		= pTarget->GetID();
// 					m_pos.x			= pTarget->GetPosX();
// 					m_pos.y			= pTarget->GetPosY();
// 					ProcessSerialCut();
// 				}
// 				break;
// 			}
// 
// 			// restore
// 			m_pMagic		= pMagic;
// 			m_idTarget		= idTarget;
// 			m_pos.x			= pos.x;
// 			m_pos.y			= pos.y;
// 
// 			return true;
// 		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::AutoAttackTarget(IRole* pTarget)
{
	int nPercentAdjust = 100;
	// ���޷������ܵĸ��ʵ���
	CMonster* pMonster = NULL;
	if (m_pOwner->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && pMonster->IsEudemon())
	{
		if (pMonster->GetFidelity() < 500)
			nPercentAdjust = 0;
		else
			nPercentAdjust = 150 + 5*((pMonster->GetFidelity()-500)/50);
	}
//modify code by �ֵ�Ԩ begin 2011.6.22
	if( pMonster )
	{
		int objid = pMonster->GetType();
		if( objid > 6300 && objid <= 6330 )
			ProcessBuffBomb(pMonster,2);//pMonster->MagicAttack(6003, pMonster->GetID(), pMonster->GetPosX(), pMonster->GetPosY());
	}
//modify code by �ֵ�Ԩ end
	//�������......
// 	for(int i = 0; i < m_setData->GetAmount(); i++)		//??? ���Ż�
// 	{
// 		IMagicData* pData = m_setData->GetObjByIndex(i);
// 
// 		if( pData && (pData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_RANDOM)
// 			&& ::RandGet(100) < (nPercentAdjust*pData->GetInt(MAGICDATA_PERCENT)/100))
// 		{
// 			return MagicAttack(pData->GetInt(MAGICDATA_TYPE), pTarget->GetID(), pTarget->GetPosX(), pTarget->GetPosY(), AUTOACTIVE_RANDOM);
// 		}
// 	}

	return false;
}

//////////////////////////////////////////////////////////////////////
// timer
//	MAGICSTATE_NONE		= 0,		// ��״̬
//	MAGICSTATE_INTONE	= 1,		// ����״̬
//	MAGICSTATE_LAUNCH	= 2,		// ʩչ״̬
//	MAGICSTATE_DELAY	= 3,		// �ӳ�״̬
//////////////////////////////////////////////////////////////////////
void CMagic::OnTimer(DWORD tCurr)
{
	if (!m_pMagic)
		return;
	switch (m_nMagicState)
	{
	case MAGICSTATE_INTONE:
		{
			bool bWithBreak = true;
			if (!m_tIntone.IsActive() || m_tIntone.TimeOver())
			{
				m_nMagicState	= MAGICSTATE_LAUNCH;//06.10.27�������

				if (!Launch())	// ħ��ִ��ʧ�ܾͽ��������������Ŀ��������Ŀ����ʧ�����
				{
					LockTarget(false);
					m_tApply.Clear();
					ResetDelay();
					break;
				}
				else
				{
					ResetDelay();
				}

			//	m_nMagicState	= MAGICSTATE_LAUNCH;//06.10.27����ע��
				// ħ�����õĴ����ɹ켣��������
				// Ϊ������û�й켣��ħ��ִ�У�Ĭ�����ô���Ϊ1���޹켣
		//-------------------begin---06.10.27-------------------------------------			
//  				int nApplyMS = m_pMagic->GetApplyMS();		
//  				m_tApply.Startup( nApplyMS );				
// 				if( 0 == nApplyMS ) //û������ʱ���
// 					bWithBreak = false;//���û������ʱ���,ֱ�ӽ������´���,���ȴ���һ��Ƶ�ʵĵ���
			}
			if( bWithBreak )
				break;
		}
//		break;
		//---------------------end----------------------------------------------
	case MAGICSTATE_LAUNCH:
		{
			if (!m_tApply.IsActive() || m_tApply.IsTimeOut())
			{
#ifdef _DEBUG
				m_pOwner->SendSysMsg("Debug: ħ��ʩչ����");
#endif
				//20070428����ע�͵�:��launch�ڶ��д���
				// MAGICSORT_ATTACK���͵�ħ�����ڹ���������KillĿ��
//				if (m_pMagic->GetInt(MAGICDATA_SORT) == MAGICSORT_ATTACK
//					&& m_idTarget != ID_NONE)
//				{
//					IRole* pTarget = RoleManager()->QueryRole(m_idTarget);
//					if(pTarget && !pTarget->IsAlive())
//					{
//						m_pOwner->Kill(pTarget, GetDieMode());
//					}
//				}

				// ����������Ҫ��ѯ�Ƿ�������״̬��������������Kill����֮�����
				// ���򽫲�����ʬ����ʧ״̬
				LockTarget(false);

				// ResetDelay();
				// OnTimer�ĵ���Ƶ��Ϊ0.5�룬
				// Ϊ�˼�ʱִ��NextMagic������ֱ�ӽ���MAGICSTATE_DELAY
			}
			else
			{
				break;
			}
		}
//		break;
	case MAGICSTATE_DELAY:
		{
			if (!m_tDelay.IsActive())
			{
				m_nMagicState	= MAGICSTATE_NONE;
				break;
			}

			if ( m_tDelay.IsActive() && m_tDelay.IsTimeOut() )
			{
#ifdef _DEBUG
				m_pOwner->SendSysMsg("Debug: ħ���ӳٽ���");
#endif
/////////////////////������ʱע������,���ǹ�����ȴʱ��//////////////////////////////////
/*				//�������,COOLDOWN����----begin---------------------------------
				CUser* pUser = NULL;
				if( m_pOwner->QueryObj( OBJ_USER,( void** )&pUser ) ) //Ϊ���ʹ��ħ��
				{
					int nType = m_pMagic->GetInt( MAGICDATA_TYPE );
					CMsgCoolDown msg;
					if( msg.Create( pUser->GetID(),nType,MAGICCOOLDOWN_END,0 ) )
						pUser->SendMsg( &msg );
				}*/
				//---------------------------end-----------------------------------
				m_nMagicState	= MAGICSTATE_NONE;
			}
		}
		break;
	case MAGICSTATE_NONE:
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// task
//////////////////////////////////////////////////////////////////////
bool CMagic::CheckType(int nType)
{
	return FindMagic(nType) != NULL;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CheckLevel(int nType, int nLevel)
{
	IMagicData* pData = FindMagic(nType);
	if(pData && pData->GetInt(MAGICDATA_LEVEL) == nLevel)
		return true;
	return false;
}


OBJID CMagic::LearnMagic(int nType, int nLevel/*=0*/, bool bSaveMagic/*=true*/, int	nGiftFlag, DWORD dwAddLev, DWORD dwLearn)
{
	OBJID idMagic = ID_NONE;
	//����
	CMagicTypeData* pMagicType = CMagic::FindMagicType(nType/*, nLevel*/);
	if (!pMagicType)
		return ID_NONE;

	//ӵ����
	CUser* pUser = NULL;
	if (!m_pOwner->QueryObj( OBJ_USER,( void** )&pUser))
		return ID_NONE;
	CHECKF(pUser);

	//�Ѿ�ѧ��
	bool bUplevel = false;
	IMagicData* pMagicData = FindMagic(nType);
	if(pMagicData)
	{
		bUplevel = true;
		if(nLevel <= pMagicData->GetInt(MAGICDATA_LEVEL))
			return 0;
	}

	if(nLevel > 150)
		nLevel = 150;

	if(bUplevel)
	{
		if(nType == MAGIC_BACK||nType == MAGIC_BACKSYN)
			return ID_NONE;

		if (UpLevel(pMagicData,nLevel))
			return idMagic;
		return ID_NONE;
	}

	//��
	OBJID idBook = pMagicType->GetInt(MAGICTYPEDATA_BOOK);
	CItem* pBook = NULL;
	if (idBook != ID_NONE)
	{
		pBook = pUser->GetItemByType(idBook);
		if (!pBook || pBook->IsLocked())
		{
			pUser->SendSysMsg(_TXTATR_TOPSHOW, "������û����Ӧ�ļ�����");
			return ID_NONE;
		}
	}

	if (pMagicType->GetInt(MAGICTYPEDATA_NEED_PROFMASK) != pUser->GetProfession() && pMagicType->GetInt(MAGICTYPEDATA_NEED_PROFMASK)!=9999)
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "ְҵ����");
		return ID_NONE;
	}

	if (pMagicType->GetInt(MAGICTYPEDATA_NEED_PROFMASK) != 9999)
	{
		if (pMagicType->GetInt(MAGICTYPEDATA_NEED_LEVEL) > pUser->GetJobLev())
		{
			pUser->SendSysMsg(_TXTATR_TOPSHOW, "ְҵ�ȼ�����");
			return ID_NONE;
		}
	}
	else
	{
		if (pMagicType->GetInt(MAGICTYPEDATA_NEED_LEVEL) > pUser->GetLev())
		{
			pUser->SendSysMsg(_TXTATR_TOPSHOW, "����ȼ�����");
			return ID_NONE;
		}
	}
	
	DWORD dwExp = 0;
	DWORD dwMoney = 0;
	DWORD nCostSyn = 0;//�۰ﹱ
	if (nType>=6000 && nType<7000)//�����
	{
		nLevel = 1;
		if (pUser->GetLev()<20)
		{
			pUser->SendSysMsg(_TXTATR_TOPSHOW, "����ȼ�����");
			return false;
		}

		dwExp = 100000;
		dwMoney = 100000;
	}
	else if (nType>2100 && nType<2500)//���ɼ���
	{
		if (!pUser->GetSyndicate())//û����
		{
			pUser->SendSysMsg("�㻹û�н���");
			return false;
		}
		if (pUser->GetSyndicate()->NoPay())//����ֹͣά��
		{
			pUser->SendSysMsg("������ͣά����");
			return false;
		}
		if((pUser->GetSyndicate()->GetLev((nType/100)%10)*20) < nLevel)
		{
			pUser->SendSysMsg("�õȼ�����");
			return ID_NONE;
		}
		for (int i = 1 ; i < nLevel + 1 ; i ++ )
		{
			CLevupexpData* pLevCon = LevupexpSet()->GetObj(EXP_TYPE_SYNSKILLCON * _EXP_TYPE+i);
			if (!pLevCon)
				return ID_NONE;
			CLevupexpData* pLevMoney = LevupexpSet()->GetObj(EXP_TYPE_SYNMONEY * _EXP_TYPE+i);
			if (!pLevMoney)
				return ID_NONE;
			nCostSyn += (pMagicType->GetInt(MAGICTYPEDATA_NEED_COST_LEV) *pLevCon->GetInt(LEVUPEXPDATA_EXP))/10;
			dwMoney += (pLevMoney->GetInt(LEVUPEXPDATA_EXP) * pMagicType->GetInt(MAGICTYPEDATA_NEED_COST_LEV))/10;
		}
		int nVipLev = 0;
		if (pUser->IsVipActive())
			nVipLev = pUser->GetVipLev();
		if (nVipLev==1 || nVipLev==4)//VIP�ۿ�
			dwMoney = dwMoney * (MAGIC_VIP_LEARN1 / 100.0);
		else if (nVipLev==2)
			dwMoney = dwMoney * (MAGIC_VIP_LEARN2 / 100.0);
		else if (nVipLev==3)
			dwMoney = dwMoney * (MAGIC_VIP_LEARN3 / 100.0);
// 		if (pUser->GetSynCon()<nCostSyn)
// 		{
// 			CLevupexpData* pLevSyn = LevupexpSet()->GetObj(7000+nLevel);
// 			if (!pLevSyn)
// 				return ID_NONE;
// 			nCostSyn += pLevSyn->GetInt(LEVUPEXPDATA_EXP) * pMagicType->GetInt(MAGICTYPEDATA_NEED_COST_LEV);
// 		}
	}
	else
	{
		for (int i = 1 ; i < nLevel + 1 ; i ++ )
		{
			CLevupexpData* pLevExp = LevupexpSet()->GetObj(EXP_TYPE_SKILLEXP * _EXP_TYPE+i);
			if (!pLevExp)
				return ID_NONE;
			CLevupexpData* pLevMoney = LevupexpSet()->GetObj(EXP_TYPE_SKILLMONEY * _EXP_TYPE+i);
			if (!pLevMoney)
				return ID_NONE;

			dwExp += pLevExp->GetInt(LEVUPEXPDATA_EXP) * pMagicType->GetInt(MAGICTYPEDATA_NEED_COST_LEV);
			dwMoney += pLevMoney->GetInt(LEVUPEXPDATA_EXP) * pMagicType->GetInt(MAGICTYPEDATA_NEED_COST_LEV);
		}

		int nVipLev = 0;
		if (pUser->IsVipActive())
			nVipLev = pUser->GetVipLev();
		if (nVipLev==1 || nVipLev==4)//VIP�ۿ�
		{
			dwMoney = dwMoney * (MAGIC_VIP_LEARN1 / 100.0);
		}
		else if (nVipLev==2)
			dwMoney = dwMoney * (MAGIC_VIP_LEARN2 / 100.0);
		else if (nVipLev==3)
			dwMoney = dwMoney * (MAGIC_VIP_LEARN3 / 100.0);
	}

	
	if (pUser->GetExp() < dwExp)
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "��ɫӵ�еľ��鲻�㣬����ѧϰ����");
		return ID_NONE;
	}
	if (pUser->GetMoney() < dwMoney)
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "�������㣬����ѧϰ����");
		return ID_NONE;
	}
	if (pUser->GetSynCon() < nCostSyn)
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "��ɫӵ�еİ��ɹ��׶Ȳ��㣬����ѧϰ����");
		return ID_NONE;
	}
	
	IMagicData* pData = NULL;
	if (bSaveMagic)
	{
		pData = CMagicData::CreateNew();
		CHECKF(pData);
		IF_NOT(pData->Create(GameDataDefault()->GetMagicData(), idMagic))
			return ID_NONE;
	}
// 	else
// 	{
// 		// ��ʱѧ��ħ���������������ڴ���
// 		pData = CMagicTempData::CreateNew();
// 		CHECKF (pData);
// 		ST_MAGIC_DATA data;
// 		memset(&data, 0L, sizeof(ST_MAGIC_DATA));
// 		data.idMagic	= CMagic::SpawnMagicID();
// 		IF_NOT (pData->Create(&data))
// 			return ID_NONE;
// 		idMagic	= data.idMagic;
//	}

	if(dwMoney > 0)
	{
		if (!pUser->SpendMoney(dwMoney, SYNCHRO_TRUE))//�۳�����
		{
			if(pData)
				pData->ReleaseByOwner();
			CHECKF(!"pUser->SpendMoney(dwMoney, SYNCHRO_TRUE)");
		}
	}
	
	if(dwExp > 0)
		pUser->SetAttrib(_USERATTRIB_EXP, pUser->GetExp() - dwExp, SYNCHRO_TRUE);//ͬ�����ﾭ��

	if (nCostSyn>0)
	{
		OBJID oldcon=pUser->GetSynCon();
		pUser->SetSynCon(oldcon-nCostSyn);
		if (nCostSyn >= 100)
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) oldcon=%u spend %u from upskill", pUser->GetID(), pUser->GetName(), oldcon,nCostSyn);
	}

	if(pBook)
	{
		bool bIsCostly = pBook->IsCostlyItem();
		if (!pUser->SpendItem(pBook))//ɾ��������
		{
			if(pData)
				pData->ReleaseByOwner();
			CHECKF(!"pUser->SpendItem(pBook)");
		}

		if (bIsCostly)//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", pUser->GetID(), pUser->GetName(), idBook);
	}

	pData->SetInt(MAGICDATA_OWNERID, m_pOwner->GetID());
	pData->SetInt(MAGICDATA_TYPE, nType);
	pData->SetInt(MAGICDATA_LEVEL, nLevel);
	pData->SetInt(MAGICDATA_EXP, 0);

	if(bSaveMagic)
	{
		idMagic = pData->InsertRecord();
		if(idMagic == ID_NONE)
		{
			pData->ReleaseByOwner();
			return false;
		}
	}
	
	pData->FindType();
	pData->SetDelay();		// �����ӳ�

	if ( bSaveMagic && nType!=MAGIC_BACK&& nType!=MAGIC_BACKSYN)//���沢��ʾ����
	{
		CMsgMagicInfo msg;
		IF_OK(msg.Create(m_pOwner->GetID(),
			pData->GetInt(MAGICDATA_TYPE),
			pData->GetMagicLevel(),
			pData->GetInt(MAGICDATA_EXP),
			_MAGICINFO_LEARNNEW))
			m_pOwner->SendMsg(&msg);

		char temp[128];
		sprintf(temp, "��ѧ����%d��%s", nLevel, pMagicType->GetStr(MAGICTYPEDATA_NAME));
		pUser->SendSysMsg(_TXTATR_TOPSHOW, temp);
	}
	
	m_setData->AddObj(pData);

	//�������ܸ���
	if (pData->GetInt(MAGICDATA_SORT)== 99)
		pUser->UpdateCalculatedProperty();

	if (nType>=6000 && nType<7000)//�����
	{
		//���ýű��Ա��������ָ��
		int nActionID = 0;
		switch(nType)
		{
			case 6001:nActionID = 10035; break;
			case 6002:nActionID = 10036; break;
			case 6003:nActionID = 10037; break;
			case 6004:nActionID = 10038; break;
			case 6005:nActionID = 10039; break;
			case 6006:nActionID = 10040; break;
		}
		if (nActionID)
			pUser->ProcessAction(nActionID);
	}

	DEBUG_TRY
	pUser->UpdateMissionMsgByMonsterType(200000 + nType);
	DEBUG_CATCH("pUser->UpdateMissionMsgByMonsterType(200000 + nType);")

	return idMagic;
}

//////////////////////////////////////////////////////////////////////
//06.4.13�����޸�
//�˺�����������ħ���˺�
//////////////////////////////////////////////////////////////////////
int CMagic::CalcMagicPower(IRole* pAtker, IRole* pTarget, int* pAdjustAtk/*=0*/)
{
	return 0;
	//07.1.31����ע��
/*	int nAtk = 0;

	if( ::RandGet(100) < 50 )
		nAtk = pAtker->GetMgcMaxAtk() - ::RandGet((pAtker->GetMgcMaxAtk()-pAtker->GetMgcMinAtk()) / 2);
	else
		nAtk = pAtker->GetMgcMinAtk() + ::RandGet((pAtker->GetMgcMaxAtk()-pAtker->GetMgcMinAtk()) / 2);

	nAtk = pAtker->AdjustMagicAtk( nAtk );

	if( pAdjustAtk )
	{
		if(*pAdjustAtk)
			nAtk = nAtk + ::CutTrail( 0, CRole::AdjustDataEx( nAtk, *pAdjustAtk ) );

		*pAdjustAtk = nAtk;
	}

	int nDef = pTarget->GetMagicDef();				//�õ�ħ��������

	nDef	= pTarget->AdjustMagicDef(nDef);		//����ħ��������

	// damage
	int	nDamage	= nAtk - nDef;
	if (pAtker->QueryOwnerUser() && pTarget->IsMonster())
	{
		CMonster* pMonster;
		if(pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && !pMonster->IsEquality())
			nDamage	= CBattleSystem::CalcDamageUser2Monster(nAtk, nDef, pAtker->GetLev(), pTarget->GetLev());
		nDamage = pTarget->AdjustMagicDamage(nDamage);
		nDamage	= CBattleSystem::AdjustMinDamageUser2Monster(nDamage, pAtker, pTarget);
	}
	else if(pAtker->IsMonster() && pTarget->QueryOwnerUser())
	{
		CMonster* pMonster;
		if(pAtker->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && !pMonster->IsEquality())
			nDamage	= CBattleSystem::CalcDamageMonster2User(nAtk, nDef, pAtker->GetLev(), pTarget->GetLev());
		nDamage = pTarget->AdjustMagicDamage(nDamage);
		nDamage	= CBattleSystem::AdjustMinDamageMonster2User(nDamage, pAtker, pTarget);
	}
	else
	{
		// adjust magic damage
		nDamage = pTarget->AdjustMagicDamage(nDamage);
	}

	
	return ::CutTrail(1, nDamage);*/
}

//////////////////////////////////////////////////////////////////////
bool CMagic::HitByMagic()
{
	CHECKF(m_pMagic);
	return true;
}

//////////////////////////////////////////////////////////////////////
// �������ܹ�������ħ�����ܹ������� MAGICDATA_WEAPON_HIT ����
ATK_TYPE CMagic::GetAtkType() const
{
	ASSERT(m_pMagic);

	ATK_TYPE type = { ATKTYPE_MAGIC_SKILL/*ATKTYPE_PHYSICAL_SKILL*/ };//�����ܹ���
	
	return type;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::HitByWeapon()
{
	CHECKF(m_pMagic);
	return false;
}

void CMagic::ClearMagicCD()
{
	m_nMagicState = MAGICSTATE_NONE;
	m_tDelay.Clear();
}

//////////////////////////////////////////////////////////////////////
void CMagic::ResetDelay()
{
	CHECK(m_pMagic);
	int nStep = 0;
	int nType = m_pMagic->GetInt( MAGICDATA_TYPE );
	DEBUG_TRY
	int nMagicLev = m_pMagic->GetMagicLevel();
	int nPublicDelay = m_pMagic->GetInt(MAGICDATA_PUBLICDELAY);
	nPublicDelay = AdjustMagicPower(nPublicDelay, 0, nMagicLev,MAGICLEVADDTYPE_PUBLICCD);//���ܵȼ��ӳ�
	if(0 == nPublicDelay)
	{
		m_nMagicState	= MAGICSTATE_NONE;
	}
	else
	{
		m_nMagicState	= MAGICSTATE_DELAY;
	}
	m_tDelay.Update();	// ��ʼDELAY
	// ���� ÿ��ħ���������ӳټ�ʱ��


	nStep = 1;
	int nMagicDelay = m_pMagic->GetInt(MAGICDATA_DELAY);
	nStep = 10;
	nMagicDelay = AdjustMagicPower(nMagicDelay, 0, nMagicLev,MAGICLEVADDTYPE_PRIVATECD);//���ܵȼ��ӳ�
	nStep = 11;
	if(nMagicDelay > 0)
	{
		m_pMagic->QueryDelayObj().SetInterval(nMagicDelay);
	}
	nStep = 2;
//--------------------06.9.4�����޸Ĺ��ڼ���CD---begin-----------------------------
	m_pMagic->StartDelay(); //CD
	nStep = 3;
	//20080122����:����NPCҲ��ҪCD��Ϣ-------------------
	DWORD dwCDTime	= m_pMagic->GetInt( MAGICDATA_DELAY );
	CMsgCoolDown msg;
	if( msg.Create( m_pOwner->GetID(),
		MAGICCOOLDOWN_BEGIN,
		MAGIC_INFO,
		nType,
		nMagicDelay,
		nPublicDelay ) )
// 		PUBLICDELAYTIME ) ) //˽��CD�͹���CD
	{
		nStep = 4;
		if(!m_bIsSpendMp)
		{
			if (!(m_pOwner->GetID() >= EUDEMON_ID_FIRST && m_pOwner->GetID() <= EUDEMON_ID_LAST))//���ﲻ����
			{
				nStep = 5;
				m_bIsSpendMp = true;
				if (m_pOwner->QueryStatus(STATUS_SPENDMP_UP))//����������
					this->nSpendMp = this->nSpendMp*1.2;
				nStep = 6;
				int nDecManaRate = UserJinmeiSysMgr()->GetJinmeiDecManaRate(m_pOwner->GetID());
				nStep = 7;
				this->nSpendMp = this->nSpendMp * (1000 - nDecManaRate) / 1000;
				m_pOwner->AddAttrib( _USERATTRIB_MANA, -1 * (this->nSpendMp), SYNCHRO_TRUE );
				this->nSpendMp = 0;
			}
//			m_pOwner->SendMsg( &msg );
		}else
		{
			//m_pOwner->SendSysMsg("Debug: �������㣬�޷�ʩչ��");
		}
		nStep = 8;
		if(!m_bIsSpendAng)
		{
			if (!(m_pOwner->GetID() >= EUDEMON_ID_FIRST && m_pOwner->GetID() <= EUDEMON_ID_LAST))
			{
				m_bIsSpendAng = true;
				m_pOwner->AddAttrib( _USERATTRIB_SP, -1 * (this->m_nSpendAng), SYNCHRO_TRUE );
				this->m_nSpendAng = 0;
			}
//			m_pOwner->SendMsg( &msg );
		}
		nStep = 9;
		m_pOwner->SendMsg( &msg );
	}
	DEBUG_CATCH3("CMagic::ResetDelay(), nStep=%d, magictype=%d", nStep, nType)
//	CUser* pUser = NULL;
//	if( m_pOwner->QueryObj( OBJ_USER,( void** )&pUser ) ) //Ϊ���ʹ��ħ��
//	{
//		int nType		= m_pMagic->GetInt( MAGICDATA_TYPE );
//		DWORD dwCDTime	= m_pMagic->GetInt( MAGICDATA_DELAY );
//		CMsgCoolDown msg;
//		if( msg.Create( pUser->GetID(),MAGICCOOLDOWN_BEGIN,MAGIC_INFO,nType,dwCDTime,PUBLICDELAYTIME ) ) //˽��CD�͹���CD
//			pUser->SendMsg( &msg );
//	}
	//----------------------------------------------------
	
//-------------------------------------------------end------------------------------	
}

//////////////////////////////////////////////////////////////////////
bool CMagic::LockTarget(bool bLock)
{
	return false;

}

//////////////////////////////////////////////////////////////////////
OBJID CMagic::GetType()
{
	OBJID idType = ID_NONE;
	if(m_pMagic)
		idType = m_pMagic->GetInt( MAGICDATA_TYPE );
	return idType;
}
int CMagic::GetPower(int nStepIndex /* = -1*/)
{
	CHECKF(m_pMagic);
	return m_pMagic->GetInt(MAGICDATA_POWER);
}


//////////////////////////////////////////////////////////////////////////
//  [2007-9-29] ���	�ɷ��Ѫ
BOOL CMagic::CanRecurit(IRole *pRole)
{
	CHECKF(m_pMagic);
	CHECKF(pRole);
	CHECKF(m_pOwner);

	//if(HasSameWargameStatus(pRole))	//	��ͬһ�
		return IsImmunity(pRole);	//	ֱ�ӵ����Ƿ�����,���߾Ϳɼ�Ѫ

// 	else	//	����ͬһ�
// 		return FALSE;
}

//  [2007-9-29] ���	�Ƿ�ͬһ�
//	--��--�μ�ͬһ���û�μӻ����TRUE
//	--��--���ڻBUF�����ԼӼ�BUF,����MAGIC�ľͷ���TRUE
//	�μӲ�ͬ�����FALSE
BOOL CMagic::HasSameWargameStatus(IRole *pRole)
{
	CHECKF(m_pMagic);
	CHECKF(pRole);
	CHECKF(m_pOwner);

	if(m_pOwner != pRole)	//	�����Լ�
	{
		IStatus* pStatus1 = m_pOwner->QueryStatus(STATUS_WARGAME);
		IStatus* pStatus2 = pRole->QueryStatus(STATUS_WARGAME);

		if(m_pOwner->IsUser())	//  [���10/22/2007] BUG0001949
		{
			CMonster* pMonster = NULL;
			if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
			{
				if(pStatus2 == NULL)	//	��û��ʱ,��Ҷ����Դ�
					return TRUE;
			}
			
		}

		if(pStatus1 != NULL && pStatus2 != NULL)
		{
			if(pStatus1->GetPower() != pStatus2->GetPower())
				return FALSE;	//	��ͬ�
			else
				return TRUE;	//	��ͬ�
		}
		else if(pStatus1 != NULL && pStatus2 == NULL)	//	һ���л
			return FALSE;
		else if(pStatus1 == NULL && pStatus2 != NULL)	//	��һ���л
			return FALSE;
		
		//	if(pStatus1 == NULL && pStatus2 == NULL)
		return TRUE;	//	��û�л
	}

	return TRUE;	//	�Լ�
}



// �������CD [12/21/2007 �½���]
//------------------------------------------------------------------------------------
void CMagic::ClearDelay()
{
	m_tDelay.Clear();
	for(int i = 0; i < m_setData->GetAmount(); i++)
	{
		IMagicData* pData = m_setData->GetObjByIndex(i);
		if ( pData )
		{
			pData->QueryDelayObj().Clear();
		}
	}
}
//--------------------------------------------------------------------------------------------

// int CMagic::GetMagicLevel(IMagicData* pData)
// {
// 	if (pData)
// 	{
// // 		int nLev = pData->GetInt(MAGICDATA_LEVEL) + pData->GetInt(MAGICDATA_ADDLEVEL);
// // 		if (nLev > 10)
// // 		{
// // 			return 10;
// // 		}
// // 		return nLev;
// 		return pData->GetInt(MAGICDATA_LEVEL) + pData->GetInt(MAGICDATA_ADDLEVEL);
// 	}
// 	return NULL;
// }
//goto ���ﵽ�ȼ�nLev�ļ��ܸ����Ƿ���ڵ���nAmount
bool CMagic::CheckMagicLevAmount(int nLev, int nAmount, int nProf)
{
	if (nLev<=0 || nAmount<=0)
		return true;
	int nHasAmount = 0;
	for(int i = 0; i < m_setData->GetAmount(); i++)
	{
		IMagicData* pData = m_setData->GetObjByIndex(i);
		if(pData && pData->GetInt(MAGICDATA_LEVEL) >= nLev && pData->GetInt(MAGICDATA_NEED_PROFMASK)==nProf)
			nHasAmount++;
	}
	return nHasAmount >= nAmount;
}
