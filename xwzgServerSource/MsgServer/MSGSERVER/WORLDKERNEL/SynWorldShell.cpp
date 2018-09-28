// SynWorldShell.cpp: implementation of the CSyndicateWorld class.
//
//////////////////////////////////////////////////////////////////////
// #undef _WINDOWS_
// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0510 // ��������Ϊ�ʺ� Windows Me ����߰汾����Ӧֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0500	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��ֵ��
#endif
#define	_WINSOCKAPI_		// ��ֹ����winsock.h
// #include "afx.h"
#include "afxdisp.h"

#include "AllMsg.h"

#include "I_MessagePort.h"

#include "../share/define.h"

#include "MessageBoard.h"

#include "UserList.h"

#include "WorldKernel.h"

#include "SynWorldShell.h"


//////////////////////////////////////////////////////////////////////
const int	SYNMONEY_LIMIT		= 2000000000;		// ��������ƶ�
const int	MEMBERLIST_SIZE		= 10;				// �����б��ÿ������
const int	MEMBERLIST_LIMIT	= 1234567890;		// �����б�����ƶ�
//20070416����:��Ҫ�Դ˴����remoteCall���.
//ע�Ᵽ����user.h��ͬ��
enum REMOTE_CALL{
		REMOTE_CALL_DROP_LEADER,
		REMOTE_CALL_LEAVE_SYN,
		REMOTE_CALL_TEAM_CLOSE_MONEY,
		REMOTE_CALL_TEAM_CLOSE_ITEM,
		REMOTE_CALL_TEAM_CLOSE_GEM_ACCESS,
		REMOTE_CALL_TEAM_HUNTKILL,
		REMOTE_CALL_SYNPAYOFF,//20070414����:���ŷ����ʵ�����
		REMOTE_CALL_GOMATE,///˲�Ƶ���ż����
		REMOTE_CALL_REPLYGOMATE,///��Ӧ˲�Ƶ���ż����	
		REMOTE_CALL_KICKOUTPLAYER,//�ߵ����
		REMOTE_CALL_WARGAME,		//	ս���ʹ��
		REMOTE_CALL_SYNADDMENBER,
		REMOTE_CALL_SYNADDHAVE,
		REMOTE_CALL_SYNADDOK,
};

//���ų�Ա���׵ȼ�����
const int _SYN_USER_PROFFERLEV_MAX = 5;

//���ų�Ա���׵ȼ��빤�ʵĹ���
//��Ա���׶ȵȼ�	��ù�����	�㷨(ϵ��m)
//1�����׶���Ա	A	A
//2�����׶���Ա	B	A(1+m%)
//3�����׶���Ա	C	B(1+m%)
//4�����׶���Ա	D	C(1+m%)
//5�����׶���Ա	E	D(1+m%)
const int _SYN_USER_PAY_FACTOR = 10;//����
const int _SYN_USER_PAY_PROFFERLEV_1 = 100;//1�����׶�
const int _SYN_USER_PAY_PROFFERLEV_2 = _SYN_USER_PAY_PROFFERLEV_1*(100+_SYN_USER_PAY_FACTOR)/100;//2�����׶�
const int _SYN_USER_PAY_PROFFERLEV_3 = _SYN_USER_PAY_PROFFERLEV_2*(100+_SYN_USER_PAY_FACTOR)/100;//3�����׶�
const int _SYN_USER_PAY_PROFFERLEV_4 = _SYN_USER_PAY_PROFFERLEV_3*(100+_SYN_USER_PAY_FACTOR)/100;//4�����׶�
const int _SYN_USER_PAY_PROFFERLEV_5 = _SYN_USER_PAY_PROFFERLEV_4*(100+_SYN_USER_PAY_FACTOR)/100;//5�����׶�

//���Ź��׵ȼ��빱��ֵ�Ĺ���
//���׼���	����������˹��׶�	���Ź��ʶ��
//���׼���	����������˹��׶�	���׶�Ӧ����ֵ
//1�����׶���Ա	141	0
//2�����׶���Ա	200	141
//3�����׶���Ա	325	341
//4�����׶���Ա	516	666
//5�����׶���Ա	0	1182
//const int _SYN_USER_PROFFERNEED[_SYN_USER_PROFFERLEV_MAX] = {0,141,341,666,1182};

//  [6/15/2007]---------���-------
//���Ų߻����޸�
//1�����׶���Ա	7	
//2�����׶���Ա	14	
//3�����׶���Ա	28	
//4�����׶���Ա	56	
//5�����׶���Ա	0	
//const int _SYN_USER_PROFFERNEED[_SYN_USER_PROFFERLEV_MAX] = {0,7,14,28,56};

//20070807����:�߻���
//��Ա���ף�
//��Ա����	��Ա����������˹��׶�
//1��2	5
//2��3	42
//3��4	56
//4��5	70
const int _SYN_USER_PROFFERNEED[_SYN_USER_PROFFERLEV_MAX] = {0,5,42,56,70};



const int _SYN_PAYOFF_TEX = 30;//������˰.�ٷֱ�.100Ϊ100%



MYHEAP_IMPLEMENTATION(CSyndicateWorld,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSyndicateWorld::CSyndicateWorld()
{
	m_pData	= NULL;
	m_ptrMsgBd	= CMessageBoard::CreateNew(_TXTATR_MSG_SYN);
}

//////////////////////////////////////////////////////////////////////
CSyndicateWorld::~CSyndicateWorld()
{
	if(m_pData)
	{
		m_pData->Update();
		m_pData->Release();
	}

} 

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::Create(IRecordset* pRes)
{
	ASSERT(!m_pData);

	m_pData = CSynWorldBase::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pRes))
		return false;

	if(m_pData->GetInt(SYNDATA_STATE) == 3)		// ��ɾ���İ���
		return false;

	return true;

	// ͳ�ư�������
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT COUNT(*) FROM %s as s LEFT JOIN %s AS u ON u.id=s.id WHERE s.syn_id=%u && u.id IS NOT NULL", _TBL_SYNATTR, _TBL_USER, m_pData->GetID());
	IRecordset* pStatRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);
	if(pStatRes)
	{
		m_pData->SetInt(SYNDATA_AMOUNT, pStatRes->GetInt(0));	// 0: count
		m_pData->Update();
		pStatRes->Release();
	}

	// ��������б�
	//[��;���� 2008/9/24]���Ӳ�ѯ���෵���Ա��ְҵ����
	sprintf(szSQL, "SELECT u.id, u.name, s.rank, u.level, s.proffer, u.sex, u.profession,u.last_exit FROM %s as s LEFT JOIN %s AS u ON u.id=s.id WHERE s.syn_id=%u && u.id IS NOT NULL ORDER BY s.rank DESC, u.name LIMIT %d",
			_TBL_SYNATTR, _TBL_USER, m_pData->GetID(), MEMBERLIST_LIMIT);
	 pStatRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);
	if(pStatRes)
	{
		for(int i = 0; i < pStatRes->RecordCount(); i++,pStatRes->MoveNext())
		{
			//[��;���� 2008/9/24]�����Ա��ְҵ���ݵ�����
			//[��;���� 2008/9/25]���Ӷ�������Ա������
			m_setMember.push_back(MemberInfo(pStatRes->GetInt(0),//��ұ�ʶ
											pStatRes->GetStr(1),//�����
											pStatRes->GetInt(2),//���ְλ
											pStatRes->GetInt(3),//��ҵȼ�
											pStatRes->GetInt(4),//�������
											pStatRes->GetInt(5),//����Ա�
											pStatRes->GetInt(6),
											false,
											0));//���ְҵ
		}
		pStatRes->Release();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::Create(IRecordset* pDefault, const CreateSyndicateInfo* pInfo, bool bSave)
{
	ASSERT(!m_pData);

	OBJID idSyn = bSave ? ID_NONE : pInfo->idSyn;
	m_pData = CSynWorldBase::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pDefault, idSyn))
		return false;

	m_pData->SetInt(SYNDATA_ID, idSyn);
	m_pData->SetStr(SYNDATA_NAME, pInfo->szName, _MAX_NAMESIZE);
//	m_pData->SetStr(SYNDATA_ANNOUNCE, pInfo->szLeaderTitle, MAX_TENETSIZE);
	m_pData->SetInt(SYNDATA_LEADERID, pInfo->idLeader);
	m_pData->SetStr(SYNDATA_LEADERNAME, pInfo->szLeader, _MAX_NAMESIZE);
	m_pData->SetInt(SYNDATA_MONEY, pInfo->nMoney);

	if(bSave)
	{
		return m_pData->InsertRecord() != ID_NONE;
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
//[��;���� 2008/9/24]Demise���¼������������AddMember����
bool CSyndicateWorld::Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave, int nOldSex, int nOldProfession)	
{
	//[��;���� 2008/9/28]������ߵ�������
	if (idOldLeader == 0 && idNewLeader != 0)
	{
		m_pData->SetInt(SYNDATA_LEADERID, idNewLeader);
		m_pData->SetStr(SYNDATA_LEADERNAME, szNewLeader, _MAX_NAMESIZE);
		m_pData->Update();
		return true;
	}
	if(!( idOldLeader==(OBJID)m_pData->GetInt(SYNDATA_LEADERID) || (OBJID)m_pData->GetInt(SYNDATA_LEADERID)==ID_NONE ))	//? �����ھ�����
		return false;
	NAMESTR	szOldLeader;
	SafeCopy(szOldLeader, m_pData->GetStr(SYNDATA_LEADERNAME), _MAX_NAMESIZE);
	m_pData->SetInt(SYNDATA_LEADERID, idNewLeader);
	m_pData->SetStr(SYNDATA_LEADERNAME, szNewLeader, _MAX_NAMESIZE);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();

	int	RANK_LEV2		= 10;
	int RANK_LEADER		=100;
	SQLBUF	szSQL;
	sprintf(szSQL, "UPDATE %s SET syn_id=%u,rank=%d WHERE id=%u LIMIT 1",
			_TBL_SYNATTR, idBackSyn, RANK_LEV2, idOldLeader);
	GameWorld()->GetDatabase()->ExecuteSQL(szSQL);

	// synchro member list
//	if(GetID() != idBackSyn)
	{
		CSyndicateWorld * pBackSyn = SynWorldManager()->QuerySyndicate(idBackSyn);
		IF_OK(pBackSyn)
		{
			int proffer = GetMemberProffer(idOldLeader);
			CPlayer *player = UserList()->GetPlayer(szOldLeader);
			DelMember(szOldLeader);
			//[��;���� 2008/9/24]����AddMember������������ô���.
			//[��;���� 2008/9/25]���Ӷ��Ƿ����ߵĵǼ�.
			pBackSyn->AddMember(idOldLeader,
				szOldLeader, 
				RANK_LEV2,
				player==NULL?0:player->m_nLevel,
				proffer,
				nOldSex,
				nOldProfession,
				UserList()->IsLoginMapGroup(player->idSocket));

			proffer = GetMemberProffer(idNewLeader);
			player = UserList()->GetPlayer(szNewLeader);
			pBackSyn->DelMember(szNewLeader);
			//[��;���� 2008/9/24]����AddMember������������ô���.
			//[��;���� 2008/9/25]���Ӷ��Ƿ����ߵĵǼ�.
			AddMember(idNewLeader,
				szNewLeader,
				RANK_LEADER,
				player==NULL?0:player->m_nLevel,
				proffer,
				nOldSex,
				nOldProfession,
				UserList()->IsLoginMapGroup(player->idSocket));
		}
	}

	return true;
}

/////////////////////////////////////////////////////d/////////////////
bool CSyndicateWorld::SetTenet(LPCTSTR szTenet, bool bSave)		
{
// 	CHECKF(strlen(szTenet)<MAX_TENETSIZE);
// 	
// 	m_pData->SetStr(SYNDATA_TENET, szTenet, MAX_TENETSIZE);
// 	if(bSave)
// 		m_pData->Update();
// 	else
// 		m_pData->ClearUpdateFlags();
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave)		
{
	m_pData->SetStr(SYNDATA_ANNOUNCE, szLeaderTitle, MAX_TENETSIZE);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::SetMemberTitle(LPCTSTR szMemberTitle, bool bSave)		
{
	return true; 
}
//////////////////////////////////////////////////////////////////////
//06.10.25�������,ĳ�����ų�Ա�ȼ��ı���
bool CSyndicateWorld::SetMemberLevel( const char* pTargetName,int nLevel )
{
	CHECKF ( pTargetName );
	for( int i = 0;i < m_setMember.size();i++ )
	{
		if( 0 == strcmp( m_setMember[i].szName.c_str(),pTargetName ) )
		{
			m_setMember[ i ].nLevel = nLevel;
			//20070405����:�ƶ����ϲ�CSyndicateWorld::ChangeSyndicateͳһ����
			//m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//����ʱ��
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::AddData(SYNDATA idx, int nData, bool bSave)
{
	int nAllData = ::AddToTop(m_pData->GetInt(idx), nData, SYNMONEY_LIMIT);		// ���ڽ�Ǯ�Ͱ�������
	if(nAllData < 0)
		nAllData = 0;
	m_pData->SetInt(idx, nAllData);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::SetData(SYNDATA idx, int nData, bool bSave)
{
	m_pData->SetInt(idx, nData);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::PresentMoney(OBJID idSyn, int nMoney, bool bSave)
{
	if(nMoney <= 0 || m_pData->GetInt(SYNDATA_MONEY) < nMoney)
		return false;
	CSyndicateWorld* pTarget = GameWorld()->QuerySynManager()->QuerySyndicate(idSyn);
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
	::GmLogSave("EVENT: ����[%s]�����[%s]��������[%d]�����ӡ�", GetStr(SYNDATA_NAME), pTarget->GetStr(SYNDATA_NAME), nMoney);
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::SetSynName(LPCTSTR szSynName, bool bSave)		
{
	m_pData->SetStr(SYNDATA_NAME, szSynName, _MAX_NAMESIZE);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::ChangeSyndicate(/*const*/ SynFuncInfo0* pFuncInfo)
{
	bool ret = false;
	switch(pFuncInfo->ucFuncType)
	{
	case	SYNFUNC_DEMISE:
		{
			//[��;���� 2008/9/24]����Demise�¼Ӳ�����ĵ��ô���.
			int nOldID = m_pData->GetInt(SYNDATA_LEADERID);
			ret = Demise(pFuncInfo->IntParam[0],
				pFuncInfo->IntParam[3],
				pFuncInfo->IntParam[1],
				(LPCTSTR)&pFuncInfo->IntParam[5],
				pFuncInfo->IntParam[2],
				pFuncInfo->IntParam[4],
				pFuncInfo->ucSaveFlag!=0,
				pFuncInfo->nSex,
				pFuncInfo->nProfession);
			pFuncInfo->IntParam[0] = nOldID;
			//pFuncInfo->IntParam[3] = m_pData->GetInt(SYNATTRDATA_MEMBERLEVEL);
			
			m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405����:����ʱ��
		}		
		break;
// 	case	SYNFUNC_SETTENET:
// 		ret = (SetTenet(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag!=0)
// 				&& SetData(SYNDATA_PUBLISHTIME, pFuncInfo->dwData[0], pFuncInfo->ucSaveFlag!=0));
// 		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405����:����ʱ��
// 		break;
	case	SYNFUNC_SETLEADERTITLE:
		ret = SetLeaderTitle(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag!=0);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405����:����ʱ��
		break;
	case	SYNFUNC_SETMEMBERTITLE:
		ret = SetMemberTitle(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag!=0);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405����:����ʱ��
		break;
	case	SYNFUNC_ADDDATA:
		ret = AddData((SYNDATA)pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag!=0);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405����:����ʱ��
		break;
	case	SYNFUNC_SETDATA:
		ret = SetData((SYNDATA)pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag!=0);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405����:����ʱ��
		break;
	case	SYNFUNC_PRESENT:
		ret = PresentMoney(pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag!=0);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405����:����ʱ��
		break;
	case	SYNFUNC_SETSYNNAME:
		ret = SetSynName(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag!=0);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405����:����ʱ��
		break;
	case	SYNFUNC_ADD_MEMBER:
		{
			//[��;���� 2008/9/24]��������Ҵ������ָ�Ϊ����ұ�ʶ������,����������ֵĴ���
			//CPlayer *player = UserList()->GetPlayer((char*)&pFuncInfo->IntParam[1]);
			CPlayer *player = UserList()->GetPlayer(pFuncInfo->idUser);
			if(player != NULL)
			{
				//[��;���� 2008/9/24]��AddMember�������¼���������
				//[��;���� 2008/9/25]���Ӷ��Ƿ����ߵĵǼ�
				ret = AddMember(player->GetID(),
						(char*)&pFuncInfo->IntParam[1], 
						pFuncInfo->nRank,
						player==NULL?0:player->m_nLevel,
						pFuncInfo->IntParam[0],
						pFuncInfo->nSex,
						pFuncInfo->nProfession,
						UserList()->IsLoginMapGroup(player->idSocket));
				m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405����:����ʱ��
			}
			return ret;		// not broadcast
		}
		break;
	case	SYNFUNC_DEL_MEMBER:
		ret = DelMember(pFuncInfo->StrParam);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405����:����ʱ��
		return ret;		// not broadcast
		break;
	case	SYNFUNC_SET_RANK:
		//return SetMemberRank(pFuncInfo->StrParam, pFuncInfo->nRank);
// 		CPlayer *player = UserList()->GetPlayer(pFuncInfo->StrParam);
// 		if(player)
// 		{
// 
// 		}
		ret = SetMemberRank(pFuncInfo->StrParam, pFuncInfo->nRank);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405����:����ʱ��
		//return ret;
		break;
//---------06.10.25�������-------begin----------------------------------------
		//ĳ���������,������Ҫ����������Ϣ
	case SYNFUNC_UP_LEVEL:
		//return SetMemberLevel( ( char* )&pFuncInfo->IntParam[ 1 ],pFuncInfo->IntParam[ 0 ] );
		ret = SetMemberLevel( ( char* )&pFuncInfo->IntParam[ 1 ],pFuncInfo->IntParam[ 0 ] );
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405����:����ʱ��
		return ret;
		//ֱ��return ���´�
		break;
//----------------------------------end------------------------------------------
		//20070414����:����������
	case SYNFUNC_PAYOFF:
		{
			//Ǯ�������溯���۳�.������ַ�.�����ߵ����ֱ���޸����ݿ�
			ret = PayOff(pFuncInfo->IntParam[0], pFuncInfo->ucSaveFlag!=0);
			m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//����ʱ��
			//�����������ͼ��㲥.�ص�����ͼ��ʱ�������ڴ�
		}
		break;
		//20070417����:��Ա���׶Ȼ㱨
	case SYNFUNC_SET_MEMBERPROFFER:
		{
			ret = SetMemberProffer(pFuncInfo->IntParam[0],pFuncInfo->IntParam[1],false);
			m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//����ʱ��
			return ret;//���ٻص�.����㱨
		}
		break;
	//[��;���� 2008/9/25]������߼�¼
	case	SYNFUNC_LOGIN:
	//[��;���� 2008/9/25]������߼�¼
	case	SYNFUNC_LOGOUT:
		{			
			for (int i = 0; i < m_setMember.size(); i++)
			{
				if (m_setMember[i].nId == pFuncInfo->idUser)
				{
					m_setMember[i].bOnline = pFuncInfo->bOnline;
					m_setMember[i].nOfflineTime = 0;
					if(!pFuncInfo->bOnline)
					{
						m_setMember[i].nOfflineTime = GetTickCount() /1000 + 100000000;
						WriteOfflineTime(pFuncInfo->idUser);						
					}
					break;
				}
			}
			if (m_setMember.size() == i)
			{
				break;
			}
			char szMsg[64] = { 0 };
			sprintf(szMsg, "%s %d", 
				m_setMember[i].szName.c_str(), 
				m_setMember[i].bOnline);
			/*char szTemp[128];
			if (m_setMember[i].bOnline)
			{
				sprintf(szTemp, "�����Ա[%s]������.", m_setMember[i].szName.c_str());
			}
			else
			{
				sprintf(szTemp, "�����Ա[%s]������.", m_setMember[i].szName.c_str());
			}*/
			for (int n = 0; n < m_setMember.size(); n++)
			{
				if (i != n && m_setMember[n].bOnline)
				{
					CPlayer* pUser = UserList()->GetPlayer(m_setMember[n].nId);
					if (pUser)
					{
						CMsgName	msg;
						if (msg.Create(NAMEACT_SETLINE, szMsg))
						{
							pUser->SendMsg(&msg);
						}
						

						/*CMsgTalk msg;
						if (msg.Create(_TXTATR_SYSTEM, szTemp))
						{
							pUser->SendMsg(&msg);
						}*/
					}
				}
			}
		}
		break;
	case SYNFUNC_LEAVERANK:
		{
			ret = true;
		}
		break;

	default:
	//	ASSERT(!"switch CSynShell::ChangeSyndicate()");
		::LogSave( "CSyndicateWorld::ChangeSyndicate case Error,pFuncInfo->ucFuncType:%d",pFuncInfo->ucFuncType );
		return false;
		break;
	}

	if(ret)
	{
		// broadcast
		MESSAGESTR	buf;
		SynFuncInfo0*	pInfo = (SynFuncInfo0*)buf;
		memcpy(buf, pFuncInfo, MAX_MESSAGESIZE);
		pInfo->ucSaveFlag	= (unsigned char)false;
		GameWorld()->QuerySynMessage()->ChangeSyndicate(pInfo);
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////
//[��;���� 2008/9/25]����bOnline����,��¼������Ա
bool CSyndicateWorld::AddMember(int id,LPCTSTR szName, int nUserRank,int level,int proffer, int nSex, int nProfession, bool bOnline)
{
	CHECKF(szName);
	ASSERT(!DelMember(szName));

	//[��;���� 2008/9/24]�������ֱ��д�뷽ʽ
	//[��;���� 2008/9/25]���Ӷ��Ƿ����ߵļ�¼
	m_setMember.push_back(MemberInfo(id,szName, nUserRank,level,proffer, nSex, nProfession, bOnline));
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::DelMember(LPCTSTR szName)
{
	CHECKF(szName);

	for(int i = m_setMember.size()-1; i >= 0; i--)		// delete, reverse traverse
	{
		if(strcmp(m_setMember[i].szName.c_str(), szName) == 0)
		{
			m_setMember.erase(m_setMember.begin() + i);
			return true;
		}
	}

	return false;
}
bool CSyndicateWorld::DelMember(int id)
{
	if(id == 0)
		return false;

	for(int i = m_setMember.size()-1; i >= 0; i--)		// delete, reverse traverse
	{
		if(m_setMember[i].nId == id)
		{
			m_setMember.erase(m_setMember.begin() + i);
			return true;
		}
	}

	return false;
}
/////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::SetMemberRank(const char* pszName, int nRank)
{
	CHECKF (pszName);
	for (int i=0; i<m_setMember.size(); i++)
	{
		if (strcmp(m_setMember[i].szName.c_str(), pszName) == 0)
		{
			m_setMember[i].nRank = nRank;
			//20070405����:�ƶ����ϲ�CSyndicateWorld::ChangeSyndicateͳһ����
			//m_dwTimeLastUpdateMemberInfo = ::GetTickCount();
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////
//06.4.21�����޸�
enum {
		RANK_LEADER			= 100,			//�����ų�		
		RANK_SUBLEADER		= 90,			//���ų�
		RANK_TEAMLEADER		= 60,			//�ӳ�
		RANK_LEV5			= 50,			//�弶��Ա
		RANK_LEV4			= 40,			//�ļ���Ա
		RANK_LEV3			= 30,			//������Ա
		RANK_LEV2			= 20,			//������Ա
		RANK_LEV1			= 10,			//һ����Ա		
		RANK_NONE			= 0,
	
	RANK_SUB		= RANK_LEADER - RANK_SUBLEADER,
};

int CSyndicateWorld::GetSynRankShow(int nRank)
{
	int nRankShow = nRank;
	if (nRankShow > RANK_LEV2)
	{
	}
	return nRankShow;
}

//////////////////////////////////////////////////////////////////////
/*
int CSyndicateWorld::AppendOnlineMember(class CMsgName& msg, int idx)
{
	int	nAmount = m_setMember.size();
	int nCount	= 0;
	for(int i = idx; i < nAmount && nCount < MEMBERLIST_SIZE; i++)
	{
		bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);
		if(bOnline)
		{
			char	szMsg[256];
			sprintf(szMsg, "%s %d %d", m_setMember[i].szName.c_str(), GetSynRankShow(m_setMember[i].nRank), bOnline);

			// msg append
			CHECKF(msg.AppendString(szMsg, i));
			nCount++;
		}
	}
	if (nCount < MEMBERLIST_SIZE)
	{
	}

	return nCount;
}
//////////////////////////////////////////////////////////////////////
int CSyndicateWorld::AppendOnlineMemberForAllMember(class CMsgName& msg, int idx)
{
	int	nAmount = m_setMemberTmp.size();
	int nCount	= 0;
	for(int i = idx; i < nAmount && nCount < MEMBERLIST_SIZE; i++)
	{
		bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);
		if(bOnline)
		{
			char	szMsg[256];
			sprintf(szMsg, "%s %d %d", m_setMemberTmp[i].szName.c_str(), GetSynRankShow(m_setMemberTmp[i].nRank), bOnline);

			// msg append
			CHECKF(msg.AppendString(szMsg, i));
			nCount++;
		}
	}

	return nCount;
}
//////////////////////////////////////////////////////////////////////
int CSyndicateWorld::AppendOfflineMember(class CMsgName& msg, int idx, int nCount)
{
	int	nAmount = m_setMember.size();
	for(int i = idx; i < nAmount && nCount < MEMBERLIST_SIZE; i++)
	{
		bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);
		if(!bOnline)
		{
			char	szMsg[256];
			sprintf(szMsg, "%s %d %d", m_setMember[i].szName.c_str(), GetSynRankShow(m_setMember[i].nRank), bOnline);
			// msg append
			CHECKF(msg.AppendString(szMsg, i));
			nCount++;
		}
	}
	return nCount;
}
//////////////////////////////////////////////////////////////////////
int CSyndicateWorld::AppendOfflineMemberForAllMember(class CMsgName& msg, int idx, int nCount)
{
	int	nAmount = m_setMemberTmp.size();
	for(int i = idx; i < nAmount && nCount < MEMBERLIST_SIZE; i++)
	{
		bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);
		if(!bOnline)
		{
			char	szMsg[256];
			sprintf(szMsg, "%s %d %d", m_setMemberTmp[i].szName.c_str(), GetSynRankShow(m_setMemberTmp[i].nRank), bOnline);
			// msg append
			CHECKF(msg.AppendString(szMsg, i));
			nCount++;
		}
	}
	return nCount;
}
*/

//////////////////////////////////////////////////////////////////////
//06.10.26�����޸�
//����˵��:idxLast = 0,Ϊ��һ�β�ѯ
void CSyndicateWorld::SendMemberList(CPlayer* pUser,DWORD dwTime )
{
	/*
	try
	{
		if( dwTime < 0 )		
			return ;
		if( 0 == dwTime )//��һ�η���,��ϵͳʱ��
		{
			m_dwTimeLastUpdateMemberInfo = ::GetTickCount();
		}
		else //���������û�и��¹�����
		{
			if( m_dwTimeLastUpdateMemberInfo <= dwTime )
				return;
		}
		int nTotalAmount = GetMemberAmount();//�õ�����������������
		int nSendTime = nTotalAmount / MEMBERLIST_SIZE;
		if( nTotalAmount % MEMBERLIST_SIZE )
			nSendTime += 1;								//���ʹ�������
		for( int i = 0;i < nSendTime;i++ )
		{
			CMsgName	msg;
			CHECK( msg.CreateHead( NAMEACT_MEMBERLIST,i,m_dwTimeLastUpdateMemberInfo ) );
			
			int nCount = 0;
			AppendMember( msg,i * MEMBERLIST_SIZE,i );
			pUser->SendMsg( &msg );
		}
		
	}catch(...)
	{
		::LogSave( "catch CSyndicateWorld::SendMemberLis(),UserID=%d,dwTime=%d\n",pUser->GetID(),dwTime );
	}
	*/
	//-------���---2007/05/23-----
	DEBUG_TRY
		if( dwTime < 0 )		
			return ;
		if( 0 == dwTime )//��һ�η���,��ϵͳʱ��
		{
			m_dwTimeLastUpdateMemberInfo = ::GetTickCount();
		}
		else //���������û�и��¹�����
		{
			if( m_dwTimeLastUpdateMemberInfo <= dwTime )
				return;
		}
		int nTotalAmount = GetMemberAmount();//�õ�����������������
		int nSendTime = nTotalAmount / MEMBERLIST_SIZE;
		if( nTotalAmount % MEMBERLIST_SIZE )
			nSendTime += 1;								//���ʹ�������
		for( int i = 0;i < nSendTime;i++ )
		{
			CMsgName	msg;
			CHECK( msg.CreateHead( NAMEACT_MEMBERLIST,i,m_dwTimeLastUpdateMemberInfo ) );
			
			int nCount = 0;
			AppendMember( msg,i * MEMBERLIST_SIZE,i );
			pUser->SendMsg( &msg );
		}
	DEBUG_CATCH3("catch CSyndicateWorld::SendMemberLis(),UserID=%d,dwTime=%d\n",pUser->GetID(),dwTime)

}


//////////////////////////////////////////////////////////////////////
void CSyndicateWorld::CombineMemberList(const MEMBER_SET& setMember)
{
	if(setMember.size())
		m_setMember.insert(m_setMember.end(), setMember.begin(), setMember.end());
}

//////////////////////////////////////////////////////////////////////
CSyndicateWorld* CSyndicateWorld::GetMasterSyn()
{
	return this;//pSyn->GetMasterSyn();
}

//////////////////////////////////////////////////////////////////////
/*
void CSyndicateWorld::SendMemberListSpecifySyn(CPlayer *pUser, int idxLast)
{
	if(idxLast < -1 || idxLast >= static_cast<int>(m_setMember.size()))
		return ;

	// msg head
	CMsgName	msg;
	CHECK(msg.CreateHead(NAMEACT_MEMBERLIST, idxLast));

	bool	bOnline = true;		//? -1 for first query
	int nCount = 0;

	if(idxLast != -1)
		bOnline = (UserList()->GetPlayer(m_setMember[idxLast].szName.c_str()) != NULL);

	int idx	= idxLast + 1;
	if(bOnline)
	{
		nCount	= AppendOnlineMember(msg, idx);
		if(nCount < MEMBERLIST_SIZE)
			nCount = AppendOfflineMember(msg, 0, nCount);
	}
	else
	{
		nCount = AppendOfflineMember(msg, idx);
	}

	pUser->SendMsg(&msg);
}
*/
//06.10.26�����޸Ĵ˺���
//����˵��:	int idxFirst,�˴η������ݿ�ʼ��λ��
//			int idxTime,��ʶ���ʹ���,��һ��Ϊ0
void CSyndicateWorld::AppendMember(class CMsgName& msg,int idxFirst,int idxTime )
{
	int nSizeLimt = m_setMember.size();
	int nNewIndex;

	DWORD nPassTime = 0;
	int nOfflineTimeTemp = 0;

	for ( int i = 0;i < MEMBERLIST_SIZE;i++ )
	{
		nNewIndex = idxFirst + i;
		if( nNewIndex >= nSizeLimt )
			break;

		//����ʱ���ʼ���� satan 2009.7.30
		//m_dwTimeFirstUpdateMemberOffline��¼��ʼ����ʱ��
		//nOfflineTimeTemp ��¼��ʼʱ��������ʼ��ʱ�����ã� 1000000���ϱ�ʾ���������������н�ɫ�˳�ʱ��ʱ����� 
		//������ʾСʱ��
		//nPassData��¼���������һ���˳������ڵ�Сʱ��
		if(m_setMember[nNewIndex].bOnline)
		{
			nOfflineTimeTemp = 0;
		}
		else
		{
			nOfflineTimeTemp = m_setMember[nNewIndex].nOfflineTime;
			if(m_setMember[nNewIndex].nOfflineTime >= 100000000)
			{
				nOfflineTimeTemp = 0;
				nPassTime = GetTickCount()/1000 + 100000000 - m_setMember[nNewIndex].nOfflineTime;
			}
			else
			{
				nPassTime = GetTickCount()/1000 - m_dwTimeFirstUpdateMemberOffline;
			}
			nOfflineTimeTemp += nPassTime / (60 * 60 );
		}			
		
		

	//	bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);//����Ƿ����ߣ���ʱ����
		//20070417����:������˹���ֵ
//		char	szMsg[ 64 ] = { 0 };//enough
//		sprintf(szMsg, "%s %d %d", m_setMember[ nNewIndex ].szName.c_str(), GetSynRankShow(m_setMember[ nNewIndex ].nRank), m_setMember[ nNewIndex ].nLevel);
				
		char	szMsg[ 128 ] = { 0 };//enough
		//[��;���� 2008/9/24]�����Ա��ְҵ����ͻ���
		//[��;���� 2008/9/25]�����Ƿ�������Ϣ����ͻ���
		sprintf(szMsg, "%s %d %d %d %d %d %d %d", 
			m_setMember[ nNewIndex ].szName.c_str(), 
			GetSynRankShow(m_setMember[ nNewIndex ].nRank),//ְλ 
			m_setMember[ nNewIndex ].nLevel,
			m_setMember[nNewIndex].nProffer,
			m_setMember[nNewIndex].nSex,//�Ա�
			m_setMember[nNewIndex].nProfession,//ְҵ
			m_setMember[nNewIndex].bOnline,//�Ƿ�����
			nOfflineTimeTemp);
		
		msg.AppendString( szMsg,idxTime );
	}

}

int CSyndicateWorld::GetMemberProffer(OBJID id)
{
	for(int i=0;i<m_setMember.size();i++)
	{
		if(m_setMember[i].nId == id)
			return m_setMember[i].nProffer;
	
	}
	return NULL;
}

const char* CSyndicateWorld::GetMemberName(OBJID id)
{
	for(int i=0;i<m_setMember.size();i++)
	{
		if(m_setMember[i].nId == id)
			return m_setMember[i].szName.c_str();
	
	}
	return NULL;
}

//-------------------------------------
BOOL CSyndicateWorld::PayOff(DWORD dwMoney,BOOL bSave)//20070414����:������.�����޸����ݿ�.�Ҵ����Ա�ַ�
{
//	CHECKF(dwMoney>0                     //  [5/18/2007]�½������,������Ҫ�ж�˰��
//		&& m_pData->GetInt(SYNDATA_MONEY) > dwMoney*(100+_SYN_PAYOFF_TEX)/100);//�ڵ�ͼ��ʱ�Ѿ������˴�.�˴�ֱ����Ϊ�Ǵ���
	
// 	m_pData->SetInt(SYNDATA_MONEY, 
// 		m_pData->GetInt(SYNDATA_MONEY)-dwMoney*(100+_SYN_PAYOFF_TEX)/100);//dec money

	//  [3/19/2008 ���]	0002937: ���ŷ��������⡣���˵���� 
	CHECKF(dwMoney > 0);
	__int64 i64OldMoney = m_pData->GetInt(SYNDATA_MONEY);
	__int64 i64NewMoney = (__int64)dwMoney * (100 + _SYN_PAYOFF_TEX) / (__int64)100;
	i64NewMoney = i64OldMoney - i64NewMoney;

	CHECKF(i64NewMoney > 0);

	m_pData->SetInt(SYNDATA_MONEY, i64NewMoney);

	if(bSave)
	{
		m_pData->Update();
	}
	else
	{
		m_pData->ClearUpdateFlags();
	}

	//Ԥ����.���1�����׶���Ա����-----------------------
	//�������г�Ա,ͨ�����׵ȼ�ͳ�Ƶõ���ֵ
	int memberProfferLevAmount[_SYN_USER_PROFFERLEV_MAX] = {0};//��Ա�ȼ�ͳ��
	for(int i=0;i<m_setMember.size();i++)
	{
		int nProffer = m_setMember[i].nProffer;//����ֵ
		for(int j=_SYN_USER_PROFFERLEV_MAX-1;j>=0;j--)
		{
			if(nProffer>=_SYN_USER_PROFFERNEED[j])
			{
				break;
			}
		}
		//j,���ǹ��׵ȼ�-1
		CHECKF(j>=0 && j<_SYN_USER_PROFFERLEV_MAX);
		
		memberProfferLevAmount[j]++;
	}
	//ͨ��ͳ��ֵ�����ֵ
	int baseValuePercent = 0;//��ֵ����
	for(i=0;i<_SYN_USER_PROFFERLEV_MAX;i++)
	{
		switch(i)
		{
		case 0://1�����׵ȼ�������
			baseValuePercent += _SYN_USER_PAY_PROFFERLEV_1*memberProfferLevAmount[i];
			break;
		case 1://2�����׵ȼ�������
			baseValuePercent += _SYN_USER_PAY_PROFFERLEV_2*memberProfferLevAmount[i];
			break;
		case 2://3�����׵ȼ�������
			baseValuePercent += _SYN_USER_PAY_PROFFERLEV_3*memberProfferLevAmount[i];
			break;
		case 3://4�����׵ȼ�������
			baseValuePercent += _SYN_USER_PAY_PROFFERLEV_4*memberProfferLevAmount[i];
			break;
		case 4://5�����׵ȼ�������
			baseValuePercent += _SYN_USER_PAY_PROFFERLEV_5*memberProfferLevAmount[i];
			break;
		default://error!
			break;
		}
	}
	//��ֵ
	int baseValue = (__int64)dwMoney*100/baseValuePercent;
	//Ԥ����end----------------------------------------
	


	//�����������Ա�ַ�
	//������Ա,����ʱת����ͼ���Ǯ,������ʱֱ���ڴ��޸����ݿ�
	int realPayAmount = 0;//�Ѿ������ֵĳ�Ա�ĸ���.��ʵ�ʹ���.��ͳ��
	for(i=0;i<m_setMember.size();i++)
	{
		OBJID playerID = m_setMember[i].nId;//��Աid

		int nProffer = m_setMember[i].nProffer;
		for(int j=_SYN_USER_PROFFERLEV_MAX-1;j>=0;j--)
		{
			if(nProffer>=_SYN_USER_PROFFERNEED[j])
			{
				break;
			}
		}
		//j,���ǹ��׵ȼ�-1
		CHECKF(j>=0 && j<_SYN_USER_PROFFERLEV_MAX);
		
		int profferLev = j;//���׵ȼ�
		int userPay = 0;//��������
		switch(profferLev)
		{
		case 0:userPay=(__int64)baseValue*_SYN_USER_PAY_PROFFERLEV_1/100;break;
		case 1:userPay=(__int64)baseValue*_SYN_USER_PAY_PROFFERLEV_2/100;break;
		case 2:userPay=(__int64)baseValue*_SYN_USER_PAY_PROFFERLEV_3/100;break;
		case 3:userPay=(__int64)baseValue*_SYN_USER_PAY_PROFFERLEV_4/100;break;
		case 4:userPay=(__int64)baseValue*_SYN_USER_PAY_PROFFERLEV_5/100;break;
		default:break;
		}
		
		CPlayer* pPlayer = g_UserList.GetPlayer(playerID);
		if(pPlayer && g_UserList.IsLoginMapGroup(pPlayer->idSocket))//�������
		{
			//����remoteCall�Ļ���
			REMOTE_CALL0 remoteCallInfo;
			remoteCallInfo.idUser=playerID;
			remoteCallInfo.ucFuncType=REMOTE_CALL_SYNPAYOFF;
			remoteCallInfo.IntParam[0]=userPay;//�������õ�Ǯ

			IMessagePort* pMsgPort = GameWorld()->GetMsgPort();
			pMsgPort->Send(pPlayer->idProcess, 
				MAPGROUP_REMOTECALL, 
				BUFFER_TYPE(sizeof(remoteCallInfo)), 
				&remoteCallInfo);

			realPayAmount++;
		}
		else//������.ֱ���޸����ݿ�
		{
			char szSQL[512];
			sprintf(szSQL, "select money from %s where id=%d",
				_TBL_USER, playerID);
			IRecordset* pRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);
			CHECKF(pRes);			
			
			DWORD dwCurMoney = pRes->GetInt(0);
			DWORD dwNewMoney = dwCurMoney+userPay;//����֮���Ǯ
			const int nMaxMoney = 2000000000;
			if(dwNewMoney >= nMaxMoney)
				dwNewMoney = nMaxMoney;
	
			sprintf(szSQL, "update %s set money=%d where id=%d",
				_TBL_USER,dwNewMoney,playerID);
			CHECKF(GameWorld()->GetDatabase()->ExecuteSQL(szSQL));
			pRes->Release();
			pRes->Release();
			realPayAmount++;
		}
	}


	::GmLogSave("EVENT: ����[%s]������.����[%d]��", GetStr(SYNDATA_NAME), dwMoney);
	return true; 
}
//-------------------------------------------
BOOL CSyndicateWorld::SetMemberProffer(int dwProffer,OBJID idMember,BOOL bSave)//20070417����:��Ա����ֵ
{
	for(int i=0;i<m_setMember.size();i++)
	{
		if(m_setMember[i].nId == idMember)
		{
			m_setMember[i].nProffer = dwProffer;
			return true;
		}
	}
	return false;
}

void CSyndicateWorld::WriteOfflineTime(OBJID userID)
{
	//����ʱ��д�����ݿ� satan 2009.7.30
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT last_exit FROM %s WHERE id=%u", _TBL_USER, userID);
	IRecordset* pStatRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);
	if(pStatRes)
	{
		pStatRes->Release();
		char szTemp[64];
		SYSTEMTIME time;
		::GetLocalTime(&time);
		sprintf(szTemp,"%d-%d-%d-%d",time.wYear,time.wMonth,time.wDay,time.wHour);
		sprintf(szSQL, "UPDATE %s SET last_exit='%s' WHERE id=%u LIMIT 1", _TBL_USER,szTemp,userID);
		GameWorld()->GetDatabase()->ExecuteSQL(szSQL);
	}
}

// void CSyndicateWorld::SetSynAttackID(OBJID idAttack, bool bUpdate)
// {
// 	m_pData->SetInt(SYNDATA_ATTACKID, idAttack);
// 	if(bUpdate)
// 		m_pData->Update();
// }
// 
// OBJID CSyndicateWorld::GetSynAttackID()
// {
// 	return m_pData->GetInt(SYNDATA_ATTACKID);
// }
// 
// OBJID CSyndicateWorld::GetSynDefenceID()
// {
// 	return m_pData->GetInt(SYNDATA_DEFENCEID);
// }
// 
// void CSyndicateWorld::SetSynDefenceID(OBJID idDefence, bool bUpdate)
// {
// 	m_pData->SetInt(SYNDATA_DEFENCEID, idDefence);
// 	if(bUpdate)
// 		m_pData->Update();
// }

bool CSyndicateWorld::Update()
{
	return m_pData->Update();
}




