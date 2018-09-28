// Team.cpp: implementation of the CTeam class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "Team.h"
#include "mapgroup.h"
#include "SKHonorMurderList.h"//20070213����
#include "SKPostOffice.h"

#define GUIDING_TIME  60*60		//һ��Сʱ


MYHEAP_IMPLEMENTATION(CTeam,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTeam::CTeam(PROCESS_ID idProcess)
{
	m_idProcess	= idProcess;
	m_bEudemonBron = false;
	this->m_tGuidingTime.Startup(GUIDING_TIME);
	memset(&m_TeamIn,0,sizeof(TeamMemberInfo) * 6);
	m_idTeamMap=0;
}

CTeam::~CTeam()
{

}

//////////////////////////////////////////////////////////////////////
CTeam* CTeam::CreateNew(PROCESS_ID idProcess, OBJID m_idTeam, OBJID idLeader,bool bSend)
{
	CTeam* pTeam	= new CTeam(idProcess);
	if (!pTeam)
		return NULL;

	if (!pTeam->Create(idLeader,m_idTeam,bSend))
	{
		delete pTeam;
		return NULL;
	}

	pTeam->m_id = m_idTeam;
	return pTeam;
}

//////////////////////////////////////////////////////////////////////
CTeam* CTeam::CreateNew(PROCESS_ID idProcess, TeamInfoStruct* pInfo)
{
	CTeam* pTeam	= new CTeam(idProcess);
	IF_NOT(pTeam)
		return NULL;

//	IF_NOT(pTeam->Create(pInfo->idLeader))
//	{
//		delete pTeam;
//		return NULL;
//	}

	pTeam->m_id = pInfo->id;

// 	for(int i = 0; i < _MAX_TEAMAMOUNT; i++)
// 	{
// 		if(pInfo->setMember[i] != ID_NONE/* && pInfo->setMember[i] != pInfo->idLeader*/)
// 			pTeam->AddMemberOnly(pInfo->setMember[i]);
// 	}
	pTeam->m_idLeader = pInfo->idLeader;
// 	pTeam->m_bCloseMoney	= pInfo->bCloseMoney;
// 	pTeam->m_bCloseItem		= pInfo->bCloseItem;
	//20070206����:�¼Ӽ���ֵ
	//------------------------------
//	int							nTeamMode;
//	int							nExpMode;
//	int							nItemMode;
//	char						sTeamName[24];
// 	pTeam->m_nTeamMode = pInfo->nTeamMode;
// 	pTeam->m_nExpMode = pInfo->nExpMode;
// 	pTeam->m_nItemMode = pInfo->nItemMode;
	strcpy(pTeam->m_sTeamName,pInfo->sTeamName);
	//------------------------------

//	pTeam->m_nLev = pInfo->m_nLev;
//	pTeam->m_nTarget = pInfo->m_nTarget;
	strcpy(pTeam->m_sLeadName, pInfo->m_sLeadName);
	strcpy(pTeam->m_sTeamText, pInfo->m_sTeamText);

	return pTeam;
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::Create(OBJID idLeader,OBJID m_idTeam,bool bSend)
{
	CUser* pPlayer	= UserManager()->GetUser(idLeader);
	if (pPlayer)			// may be not current mapgroup
	{
		this->Init();
		m_idLeader	= idLeader;
	
		TeamMemberInfo info;
		info.id = pPlayer->GetID();
		info.dwLookFace = pPlayer->GetLookFace();
		info.dwLev = pPlayer->GetLev();
		info.usProfession = pPlayer->GetProfession(0);
		info.usProLev = pPlayer->GetJobLev();
		SafeCopy(info.szName,pPlayer->GetName(),_MAX_NAMESIZE);
		
		this->AddMemberOnly(info);
	    strcpy(m_sLeadName, pPlayer->GetName());

		if(bSend)
		{
			MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_CREATE, m_idTeam, idLeader);
			MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_ADD, m_idTeam, info.id,info.dwLookFace,info.dwLev,info.usProfession,info.usProLev,info.szName);
		}
	}
	else
	{
		this->Init();
		m_idLeader	= idLeader;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::AddMember(OBJID idMember, bool bOwnerMap)
{
	CUser* pPlayer	= UserManager()->GetUser(idMember);
	if (pPlayer)			// may be not current mapgroup
	{
		if(m_bEudemonBron)
			EudemonCancel();

		int nOldAmount = this->GetMemberAmount();
		if (nOldAmount >= _MAX_TEAMAMOUNT)
			return false;

		if(this->IsTeamMember(idMember))
		{
			return false;
		}
	
		TeamMemberInfo info;
		info.id = pPlayer->GetID();
		info.dwLookFace = pPlayer->GetLookFace();
		info.dwLev = pPlayer->GetLev();
		info.usProfession = pPlayer->GetProfession(0);
		info.usProLev = pPlayer->GetJobLev();
		SafeCopy(info.szName,pPlayer->GetName(),_MAX_NAMESIZE);
		this->AddMemberOnly(info);

		MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_ADD, GetID(), info.id,info.dwLookFace,info.dwLev,info.usProfession,info.usProLev,info.szName);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::DelMember(OBJID idMember)
{
	if(m_bEudemonBron)
		EudemonCancel();

	DelMemberOnly(idMember);	
	MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_DEL, GetID(), idMember);
	return true;
}


void CTeam::DelBuffByTutorOnDisband()
{
	for(int i = 0; i < GetMemberAmount(); i++)
	{

		CUser *pPlayer = UserManager()->GetUser(GetMemberByIndex(i));
		if(pPlayer)
		{
			CRole::DetachStatus( pPlayer->QueryRole(),STATUS_PHYATTACKUP_TUTOR_PERCENT );
			CRole::DetachStatus( pPlayer->QueryRole(),STATUS_MAGATTACKUP_TUTOR_PERCENT );
		}
	}
}

void CTeam::AddBuffByTutor( OBJID UserID )
{
	CUser* pPlayer = UserManager()->GetUser(UserID);
	if(!pPlayer)
		return;

	// �Ƿ��С�ʦ������ա�pengfeng [10/26/2007]
	if( pPlayer->GetTutor() )
	{
		int idTutor = pPlayer->GetTutor()->GetTutorID();
		if( IsTeamMember(idTutor) )
		{
			CUser *pTutor = UserManager()->GetUser(idTutor);
			if(pTutor)
			{
				CRole::AttachStatus( (IRole*)pTutor,STATUS_PHYATTACKUP_TUTOR_PERCENT, 10, -1 );
				CRole::AttachStatus( (IRole*)pPlayer,STATUS_PHYATTACKUP_TUTOR_PERCENT, 10, -1 );

				CRole::AttachStatus( (IRole*)pTutor,STATUS_MAGATTACKUP_TUTOR_PERCENT, 10, -1 );
				CRole::AttachStatus( (IRole*)pPlayer,STATUS_MAGATTACKUP_TUTOR_PERCENT, 10, -1 );
			}
		}
	}
	else 
	{
		for(int i = 0; i < GetMemberAmount(); i++)
		{
			if(pPlayer->GetStudent(GetMemberByIndex(i)))
			{
				CUser *pStudent = UserManager()->GetUser(GetMemberByIndex(i));
				if(pStudent)
				{
					CRole::AttachStatus( (IRole*)pStudent,STATUS_PHYATTACKUP_TUTOR_PERCENT, 10, -1 );
					CRole::AttachStatus( (IRole*)pPlayer,STATUS_PHYATTACKUP_TUTOR_PERCENT, 10, -1 );

					CRole::AttachStatus( (IRole*)pStudent,STATUS_MAGATTACKUP_TUTOR_PERCENT, 10, -1 );
					CRole::AttachStatus( (IRole*)pPlayer,STATUS_MAGATTACKUP_TUTOR_PERCENT, 10, -1 );
				}
			}
		}
	}
}

void CTeam::DelBuffByTutor( OBJID UserID )
{
	CUser* pPlayer = UserManager()->GetUser(UserID);
	if(pPlayer)
	{	
/*		CRole::DetachStatus( pPlayer->QueryRole(),STATUS_PHYATTACKUP_TUTOR_PERCENT );
		CRole::DetachStatus( pPlayer->QueryRole(),STATUS_MAGATTACKUP_TUTOR_PERCENT );*/
		// �Ƿ��С�ʦ������ա�pengfeng [10/26/2007]
		if( pPlayer->GetTutor() )
		{
			int idTutor = pPlayer->GetTutor()->GetTutorID();
			if( IsTeamMember(idTutor) )
			{
				CUser *pTutor = UserManager()->GetUser(idTutor);
				if(pTutor)
				{
					CRole::DetachStatus( pTutor->QueryRole(),STATUS_PHYATTACKUP_TUTOR_PERCENT );
					CRole::DetachStatus( pTutor->QueryRole(),STATUS_MAGATTACKUP_TUTOR_PERCENT );

					CRole::DetachStatus( pPlayer->QueryRole(),STATUS_PHYATTACKUP_TUTOR_PERCENT );
					CRole::DetachStatus( pPlayer->QueryRole(),STATUS_MAGATTACKUP_TUTOR_PERCENT );
				}
			}
		}
		else 
		{
			for(int i = 0; i < GetMemberAmount(); i++)
			{
				if(pPlayer->GetStudent(GetMemberByIndex(i)))
				{
					CUser *pStudent = UserManager()->GetUser(GetMemberByIndex(i));
					if(pStudent)
					{
						CRole::DetachStatus( pStudent->QueryRole(),STATUS_PHYATTACKUP_TUTOR_PERCENT );
						CRole::DetachStatus( pStudent->QueryRole(),STATUS_MAGATTACKUP_TUTOR_PERCENT );

						CRole::DetachStatus( pPlayer->QueryRole(),STATUS_PHYATTACKUP_TUTOR_PERCENT );
						CRole::DetachStatus( pPlayer->QueryRole(),STATUS_MAGATTACKUP_TUTOR_PERCENT );
					}
				}
			}
		}
		//-------------------end--------------------------------------------
	}
	else
	{
		//����С�ӳ�Ա
		for(int i = 0; i < GetMemberAmount(); i++)
		{
			CUser* pPlayer = UserManager()->GetUser(GetMemberByIndex(i));
			if(pPlayer)
			{
				if(pPlayer->GetTutor())
				{
					if( pPlayer->GetTutor()->GetTutorID() == UserID )
					{
						CRole::DetachStatus( pPlayer->QueryRole(),STATUS_PHYATTACKUP_TUTOR_PERCENT );
						CRole::DetachStatus( pPlayer->QueryRole(),STATUS_MAGATTACKUP_TUTOR_PERCENT );
					}
				}
				else
				{
					BOOL bDelBuff = false;
					for(int i = 0; i < GetMemberAmount(); i++)
					{
						CTutorData* pStudent =  pPlayer->GetStudent(GetMemberByIndex(i));
						if(pStudent)
						{
							if( pStudent->GetStuID() == UserID )
							{
								bDelBuff = true;
							}
							else
							{
								bDelBuff = false;
								break;
							}
						}
					}
					if( bDelBuff )
					{
						CRole::DetachStatus( pPlayer->QueryRole(),STATUS_PHYATTACKUP_TUTOR_PERCENT );
						CRole::DetachStatus( pPlayer->QueryRole(),STATUS_MAGATTACKUP_TUTOR_PERCENT );
					}
				}
			}
		}
	}
}

// void CTeam::SetTeamMap(OBJID val,BOOL bTransmitMsg) 
// {
// 	m_idTeamMap = val;
// 	if(bTransmitMsg)
// 		MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_SETTEAMMAP,GetID(),0,val);//nData
// }

// void CTeam::ResetOfflineLocation(BOOL bTransmitMsg)///���������߶�Ա�ı���ĸ�����ͼλ������Ϊ0
// {
// 	DEBUG_TRY
// 	for(int i = 0 ; i < 6 ; i ++)
// 	{
// 		if(m_TeamIn[i].id!=ID_NONE && m_TeamIn[i].bOnlineStatus==false)
// 		{
// 			m_TeamIn[i].idMap=0;
// 			m_TeamIn[i].usPosX=0;
// 			m_TeamIn[i].usPosY=0;
// 			///�㲥��������ͼ��
// 			if(bTransmitMsg)
// 				MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_SETLOCATION,GetID(),m_TeamIn[i].id,0,0,0);
// 		}
// 	}
// 	DEBUG_CATCH("CTeam::ResetOfflineLocation");
// }
// 
// void CTeam::SetLocation(OBJID idMember,OBJID idMap,int x,int y,BOOL bTransmitMsg)///����ʱ����һ�¶�Ա��λ��
// {
// 	for(int i = 0 ; i < 6 ; i ++)
// 	{
// 		if(m_TeamIn[i].id == idMember)
// 		{
// 			m_TeamIn[i].idMap=idMap;
// 			m_TeamIn[i].usPosX=x;
// 			m_TeamIn[i].usPosY=y;
// 			///�㲥״̬���ͻ���
// 			///�㲥��������ͼ��
// 			if(bTransmitMsg)
// 				MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_SETLOCATION,GetID(),idMember,idMap,x,y);//nData
// 			break;
// 		}
// 	}
// }
// 
// void CTeam::SetOnlineStatus(OBJID idMember,BOOL bStatus,BOOL bTransmitMsg)
// {
// 	for(int i = 0 ; i < 6 ; i ++)
// 	{
// 		if(m_TeamIn[i].id == idMember)
// 		{
// 			m_TeamIn[i].bOnlineStatus=bStatus;
// 
// 			///�㲥��������ͼ��
// 			if(bTransmitMsg)
// 			{
// 				MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_ONLINESTATUSCHANGE,GetID(),idMember,bStatus);//nData
// 
// 				///�㲥״̬���ͻ���
// 				CMsgTeam msg;
// 				if(msg.Create(idMember,_MSGTEAM_UPDATA_ONLINEINFO,bStatus))
// 					BroadcastTeamMsg(&msg,NULL);
// 			}
// 			break;
// 		}
// 	}
// }

//////////////////////////////////////////////////////////////////////
void CTeam::Dismiss(OBJID idLeader)
{
	CUser* pLeader = UserManager()->GetUser(idLeader);
			
	int nSize = this->GetMemberAmount();
	for (int i=nSize-1; i>=0; i--)
	{// ����ɾ��
		CUser* pPlayer	= UserManager()->GetUser(m_setIdUser[i]);
		if (pPlayer)
		{
			IF_OK(pPlayer->GetTeam())
			{
				if(pPlayer->GetID() != idLeader)	// owner������ɾ��
					pPlayer->ClrTeam();					//? ������Զ�̣�CUser::DelTeamMember()�л�ɾ��
			}
		}
		else
		{
			MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_DISMISS, GetID(), idLeader);
		}
	}
}

//////////////////////////////////////////////////////////////////////
OBJID CTeam::GetMemberByIndex(int nIndex)
{
	if (nIndex >= m_setIdUser.size() || nIndex < 0)
		return ID_NONE;

	return m_setIdUser[nIndex];
}

//////////////////////////////////////////////////////////////////////
void CTeam::Destroy()
{
	this->Init();
}

//////////////////////////////////////////////////////////////////////
void CTeam::Init()
{
	m_setIdUser.clear();
	m_fClosed			= false;
	m_bCloseMoney		= false;
	m_bCloseItem		= true;
	m_bCloseGemAccess	= true;
	m_idLeader			= ID_NONE;
	m_nItemMode = 1;// ����ʼ����ӵ���Ʒȡ��ģʽΪ������ȡ�á� [7/5/2007]
}

//////////////////////////////////////////////////////////////////////
void CTeam::SetLeader(OBJID idLeader)
{
	if (m_idLeader != ID_NONE)	// �Ѿ����ù��ӳ��ˡ�
		return;

	CUser* pPlayer	= UserManager()->GetUser(idLeader);
	if (!pPlayer)
		return;

	m_idLeader	= idLeader;
}

//////////////////////////////////////////////////////////////////////
OBJID CTeam::GetLeader()
{
	return m_idLeader;
}

///�õ����������������
// int CTeam::GetOnlineMemberAmount()
// {
// 	int nOnlineMemberAmount=0;
// 	for(int i = 0 ; i < 6 ; i ++)
// 	{
// 		if(m_TeamIn[i].id == ID_NONE)
// 			continue;
// 		if(m_TeamIn[i].bOnlineStatus)
// 			nOnlineMemberAmount++;
// 	}
// 	return nOnlineMemberAmount;
// }

//////////////////////////////////////////////////////////////////////
int CTeam::GetMemberAmount()
{
	return __min(_MAX_TEAMAMOUNT, m_setIdUser.size());
}

bool CTeam::UpdateMemberInfo()
{
	for(int i = 0 ; i < 6 ; i ++)
	{
		if(m_TeamIn[i].id == ID_NONE)
			continue;
		CUser* pMember	= NULL;
		pMember			= UserManager()->GetUser(m_TeamIn[i].id );
		if(pMember)
		{
			m_TeamIn[i].id			= pMember->GetID();
			m_TeamIn[i].dwLookFace	= pMember->GetLookFace();
			m_TeamIn[i].dwLev      = pMember->GetLev();
			m_TeamIn[i].usProfession = pMember->GetProfession();
			m_TeamIn[i].usProLev = pMember->GetJobLev();
			strcpy(m_TeamIn[i].szName, pMember->GetName());
		}
	}
	return true;
}

///�ж϶�Ա�Ƿ�����
// bool CTeam::IsMemberOnline(OBJID idUser)
// {
// 	for(int i = 0 ; i < 6 ; i ++)
// 	{
// 		if(m_TeamIn[i].id == idUser && m_TeamIn[i].bOnlineStatus)
// 		{
// 			return true;
// 		}
// 	}
// 	return false;
// }

bool CTeam::GetMemberInfo(TeamMemberInfo& menberInfo,OBJID idUser)
{
	for(int i = 0 ; i < 6 ; i ++)
	{
		if(m_TeamIn[i].id == idUser)
		{
			memcpy(&menberInfo,&m_TeamIn[i],sizeof(TeamMemberInfo));
			return true;
		}
	}
	return false;
}

int CTeam::GetMemberLev()
{
	int amount = 0;
	int membercount=0;
	int nMemberLev=0;
	DEBUG_TRY
	if(this->GetMemberAmount() < 1)
		return false;
	for (int i=0; i<this->GetMemberAmount(); i++)
	{
		CUser* pMember	= NULL;
		OBJID idUser	= this->GetMemberByIndex(i);
		pMember			= UserManager()->GetUser(idUser);
		if(pMember==NULL)
			continue;
// 		if (!pMember)
// 			return -1;
		amount += pMember->GetLev();
		membercount++;
	}
	if(membercount==0)
		return 1;///��ֹ��0�쳣
	nMemberLev=max(amount/(membercount) - 2,1);
	DEBUG_CATCH("CTeam::GetMemberLev()")
	return nMemberLev;
}

bool CTeam::CheckActive(CUser* pUser)
{
	DEBUG_TRY
	OBJID idLeader=GetLeader();
	CUser *pLeader=UserManager()->GetUser(idLeader);
	CHECKF(pUser);
	CHECKF(pLeader);
	CUser* pMember	= NULL;
	for(int i = 0 ; i <_MAX_TEAMAMOUNT ; i ++)
	{
		if (m_TeamIn[i].id==ID_NONE)
			continue;
		pMember = UserManager()->GetUser(m_TeamIn[i].id);
		if (!pMember)
		{
			pLeader->SendSysMsg(_TXTATR_TOPSHOW,"��Ա%s��������߻��߲�����!",m_TeamIn[i].szName);//STR_INTEAM_NOJOIN
			return false;
		}
		
		if(!pMember->CanBeSee())
			return false;
		
		if(pMember->GetMapID() != pUser->GetMapID())
		{
			pLeader->SendSysMsg(_TXTATR_TOPSHOW,"��Ա%s���㲻��ͬһ��ͼ!",pMember->GetName());//STR_INTEAM_NOJOIN
			return false;
		}
		
		if(pMember->GetDistance(pUser) > 20)
		{
			pLeader->SendSysMsg(_TXTATR_TOPSHOW,"��Ա%s�������̫Զ!",pMember->GetName());//STR_INTEAM_NOJOIN
			return false;
		}
	}


// 	for (int i=0; i<this->GetMemberAmount(); i++)
// 	{
// 		CUser* pMember	= NULL;
// 		OBJID idUser	= this->GetMemberByIndex(i);
// 		pMember			= UserManager()->GetUser(idUser);
// 		if (!pMember)
// 			continue;//return false;
// 
// 		if(!pMember->CanBeSee())
// 			return false;
// 
// 		if(pMember->GetMapID() != pUser->GetMapID())
// 		{
// 			pLeader->SendSysMsg(_TXTATR_TOPSHOW,"��Ա%s���㲻��ͬһ��ͼ!",pMember->GetName());//STR_INTEAM_NOJOIN
// 			return false;
// 		}
// 
// // 		if(!pMember->CanBeSee())
// // 			return false;
// 
// 		if(pMember->GetDistance(pUser) > 20)
// 		{
// 			pLeader->SendSysMsg(_TXTATR_TOPSHOW,"��Ա%s�������̫Զ!",pMember->GetName());//STR_INTEAM_NOJOIN
// 			return false;
// 		}
// 	}
	DEBUG_CATCH("CTeam::CheckActive crash!")
	return true;
}


//////////////////////////////////////////////////////////////////////
void CTeam::BroadcastTeamMsg(CNetMsg* pMsg, CUser* pSender/*=NULL*/,bool bSendOtherLine/* = true*/)
{
	UINT nMsgType=0;
	DEBUG_TRY
	if (!pMsg)
		return;
	UINT nMsgType=pMsg->GetType();
	CUser* pUser	= NULL;
	for(int i = 0 ; i <_MAX_TEAMAMOUNT ; i ++)
	{
		if (m_TeamIn[i].id==ID_NONE)
			continue;
		pUser = UserManager()->GetUser(m_TeamIn[i].id);
		if (pUser)
			pUser->SendMsg(pMsg);
		else if(bSendOtherLine)// �������
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pMsg, m_TeamIn[i].id);
	}
// 	for (int i=0; i<this->GetMemberAmount(); i++)
// 	{
// 		CUser* pUser	= NULL;
// 		OBJID idUser	= this->GetMemberByIndex(i);
// 		pUser			= UserManager()->GetUser(idUser);
// 		if (pUser)
// 		{
// 			//if (pUser != pSender)
// 			pUser->SendMsg(pMsg);
// 		}
// 		else // �������
// 		{
// 			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pMsg, idUser);
// 		}
// 	}
	DEBUG_CATCH2("CTeam::BroadcastTeamMsg crash!nMsgType:%d",nMsgType)
}

OBJID  CTeam::GetNextTeamLeader()///���õ����߷Ƕӳ���Ա,������ͬ��ͼ�����
{
	CUserPtr pLeader=NULL;
	CUserPtr pGoodNextLeader=NULL;
	OBJID idGoodNextLeader=ID_NONE;///�����ص���һ�ζӳ�id
	OBJID idFirstMember=ID_NONE;
	DEBUG_TRY
	///�õ��ӳ�ָ��
// 	pLeader=UserManager()->GetUser(GetLeader());
// 	CHECKF(pLeader);///�ӳ�������Ч
// 	OBJID idLeaderMap=pLeader->GetMapID();///�õ��ӳ����ڵ�ͼID
	for(int i = 0 ; i < 6 ; i ++)
	{
		///��Ч��Ա�����߶�Ա������
		if(m_TeamIn[i].id==ID_NONE || m_TeamIn[i].bOnlineStatus==false || m_TeamIn[i].id==GetLeader())
			continue;
		
 		///���汯�ۺ�ѡ��
// 		if(idFirstMember==ID_NONE)
// 			idFirstMember=m_TeamIn[i].id;

		pGoodNextLeader=UserManager()->GetUser(m_TeamIn[i].id);
		if(pGoodNextLeader)
		{
			idGoodNextLeader=m_TeamIn[i].id;
			break;
		}
// 		///���Եõ������Ա��ʵ��
// 		pGoodNextLeader=UserManager()->GetUser(m_TeamIn[i].id);
// 		if(pGoodNextLeader && pGoodNextLeader->GetMapID()>DYNAMIC_MAP_ID)
// 		{
// 			idGoodNextLeader=m_TeamIn[i].id;
// 			break;
// 		}
// 		else if(!pGoodNextLeader && idLeaderMap<DYNAMIC_MAP_ID)
// 		{
// 			idGoodNextLeader=m_TeamIn[i].id;
// 			break;
// 		}
	}
//	if(idGoodNextLeader==ID_NONE)
//		LogSave("CTeam::GetNextTeamLeader Ѱ�ҵ�������һ�ζӳ�ʱû���ҵ�,���õĲ�����,���ؿ�!");
// 		idGoodNextLeader=idFirstMember;
	DEBUG_CATCH("CTeam::GetNextTeamLeader crash!")
	return idGoodNextLeader;
}

OBJID  CTeam::GetOnlineTeamMemberOnly()///���õ����߳�Ա�Ƕӳ�
{
	DEBUG_TRY
	for(int i = 0; i < GetMemberAmount(); i++)
	{
		if(GetMemberByIndex(i) != GetLeader() && IsMemberOnline(GetMemberByIndex(i)))
			return GetMemberByIndex(i);
	}
	DEBUG_CATCH("CTeam::GetOnlineTeamMemberOnly crash!")
	return ID_NONE;
}

/////////////////////////////////////////////////////////////////////
//void BordcastTeamMsg(int nChannel,const char* pMsg)
//{
//	for (int i=0; i<this->GetMemberAmount(); i++)
//	{
//		CUser* pUser	= NULL;
//		OBJID idUser	= this->GetMemberByIndex(i);
//		pUser			= UserManager()->GetUser(idUser);
//		if (pUser)
//		{
//			if (pUser)
//				pUser->SendSysMsg(nChannel,pMsg);
//		}
//		else // �������
//		{
//			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pMsg, idUser);
//		}
//	}
//}

//////////////////////////////////////////////////////////////////////
void CTeam::Open()
{
	m_fClosed	= false;
}

//////////////////////////////////////////////////////////////////////
void CTeam::Close()
{
	m_fClosed	= true;
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::IsClosed()
{
	return m_fClosed;
}

//////////////////////////////////////////////////////////////////////
void CTeam::OpenMoney()
{
	m_bCloseMoney	= false;
	char szBuf[1024];
	sprintf(szBuf,g_objStrRes.GetStr(11002) , g_objStrRes.GetStr(11005));//STR_TEAM_MONEY_sSTR_OPEN

	for (int i=0; i<this->GetMemberAmount(); i++)
	{
		OBJID idTarget = this->GetMemberByIndex(i);
		CUser* pTarget = UserManager()->GetUser(idTarget);		
		if (pTarget)			//? may be another mapgroup
		{
			CTeam* pTeam = pTarget->GetTeam();
			IF_OK(pTeam)
			{
				pTeam->SetCloseMoney(false);
				pTarget->SendSysMsg(_TXTATR_TEAM, szBuf);
			}
		}
		else
		{
			// remote call
			char	buf[1024];
			REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
			pInfo->idUser		= idTarget;
			pInfo->ucFuncType	= REMOTE_CALL_TEAM_CLOSE_MONEY;
			pInfo->IntParam[0]	= false;
			pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int);
			MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
		}
	}
}

//////////////////////////////////////////////////////////////////////
void CTeam::CloseMoney()
{
	m_bCloseMoney	= true;
	char szBuf[1024];
	sprintf(szBuf,g_objStrRes.GetStr(11002) , g_objStrRes.GetStr(11006));//STR_TEAM_MONEY_sSTR_CLOSE

	for (int i=0; i<this->GetMemberAmount(); i++)
	{
		OBJID idTarget = this->GetMemberByIndex(i);
		CUser* pTarget = UserManager()->GetUser(idTarget);		
		if (pTarget)			//? may be another mapgroup
		{
			CTeam* pTeam = pTarget->GetTeam();
			IF_OK(pTeam)
			{
				pTeam->SetCloseMoney(true);
				pTarget->SendSysMsg(_TXTATR_TEAM, szBuf);
			}
		}
		else
		{
			// remote call
			char	buf[1024];
			REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
			pInfo->idUser		= idTarget;
			pInfo->ucFuncType	= REMOTE_CALL_TEAM_CLOSE_MONEY;
			pInfo->IntParam[0]	= true;
			pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int);
			MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
		}
	}
}

//////////////////////////////////////////////////////////////////////
void CTeam::OpenItem()
{
	m_bCloseItem	= false;
	char szBuf[1024];
	sprintf(szBuf, g_objStrRes.GetStr(11003), g_objStrRes.GetStr(11005));//STR_TEAM_ITEM_sSTR_OPEN

	for (int i=0; i<this->GetMemberAmount(); i++)
	{
		OBJID idTarget = this->GetMemberByIndex(i);
		CUser* pTarget = UserManager()->GetUser(idTarget);		
		if (pTarget)			//? may be another mapgroup
		{
			CTeam* pTeam = pTarget->GetTeam();
			IF_OK(pTeam)
			{
				pTeam->SetCloseItem(false);
				pTarget->SendSysMsg(_TXTATR_TEAM, szBuf);
			}
		}
		else
		{
			// remote call
			char	buf[1024];
			REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
			pInfo->idUser		= idTarget;
			pInfo->ucFuncType	= REMOTE_CALL_TEAM_CLOSE_ITEM;
			pInfo->IntParam[0]	= false;
			pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int);
			MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
		}
	}
}

//////////////////////////////////////////////////////////////////////
void CTeam::CloseItem()
{
	m_bCloseItem	= true;
	char szBuf[1024];
	sprintf(szBuf, g_objStrRes.GetStr(11003), g_objStrRes.GetStr(11006));//STR_TEAM_ITEM_sSTR_CLOSE

	for (int i=0; i<this->GetMemberAmount(); i++)
	{
		OBJID idTarget = this->GetMemberByIndex(i);
		CUser* pTarget = UserManager()->GetUser(idTarget);		
		if (pTarget)			//? may be another mapgroup
		{
			CTeam* pTeam = pTarget->GetTeam();
			IF_OK(pTeam)
			{
				pTeam->SetCloseItem(true);
				pTarget->SendSysMsg(_TXTATR_TEAM, szBuf);
			}
		}
		else
		{
			// remote call
			char	buf[1024];
			REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
			pInfo->idUser		= idTarget;
			pInfo->ucFuncType	= REMOTE_CALL_TEAM_CLOSE_ITEM;
			pInfo->IntParam[0]	= true;
			pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int);
			MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
		}
	}
}

//////////////////////////////////////////////////////////////////////
void CTeam::OpenGemAccess()
{
	m_bCloseGemAccess	= false;
	char szBuf[1024];
	sprintf(szBuf, g_objStrRes.GetStr(11004), g_objStrRes.GetStr(11005));//STR_TEAM_GEM_sSTR_OPEN

	for (int i=0; i<this->GetMemberAmount(); i++)
	{
		OBJID idTarget = this->GetMemberByIndex(i);
		CUser* pTarget = UserManager()->GetUser(idTarget);		
		if (pTarget)			//? may be another mapgroup
		{
			CTeam* pTeam = pTarget->GetTeam();
			IF_OK(pTeam)
			{
				pTeam->SetCloseGemAccess(false);
				pTarget->SendSysMsg(_TXTATR_TEAM, szBuf);
			}
		}
		else
		{
			// remote call
			char	buf[1024];
			REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
			pInfo->idUser		= idTarget;
			pInfo->ucFuncType	= REMOTE_CALL_TEAM_CLOSE_GEM_ACCESS;
			pInfo->IntParam[0]	= false;
			pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int);
			MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
		}
	}
}

//////////////////////////////////////////////////////////////////////
void CTeam::CloseGemAccess()
{
	m_bCloseGemAccess	= true;
	char szBuf[1024];
	sprintf(szBuf,g_objStrRes.GetStr(11004) , g_objStrRes.GetStr(11006));//STR_TEAM_GEM_sSTR_CLOSE

	for (int i=0; i<this->GetMemberAmount(); i++)
	{
		OBJID idTarget = this->GetMemberByIndex(i);
		CUser* pTarget = UserManager()->GetUser(idTarget);		
		if (pTarget)			//? may be another mapgroup
		{
			CTeam* pTeam = pTarget->GetTeam();
			IF_OK(pTeam)
			{
				pTeam->SetCloseGemAccess(true);
				pTarget->SendSysMsg(_TXTATR_TEAM, szBuf);
			}
		}
		else
		{
			// remote call
			char	buf[1024];
			REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
			pInfo->idUser		= idTarget;
			pInfo->ucFuncType	= REMOTE_CALL_TEAM_CLOSE_GEM_ACCESS;
			pInfo->IntParam[0]	= true;
			pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int);
			MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
		}
	}
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::IsTeamWithNewbie(OBJID idKiller, IRole* pTarget)
{
	CHECKF (pTarget);

	BOOL bNewbieBonus = false;
	int nMonsterLev = pTarget->GetLev();
	for (int i=0; i<this->GetMemberAmount(); i++)
	{
		CUser* pUser = UserManager()->GetUser(this->GetMemberByIndex(i));		
		if (pUser)			//? may be another mapgroup
		{
			if (!pUser->IsAlive())
				continue;

			if (pUser->GetMapID() != pTarget->GetMap()->GetID())
				continue;

			if (abs(pUser->GetPosX()-pTarget->GetPosX()) > _RANGE_EXPSHARE ||
					abs(pUser->GetPosY()-pTarget->GetPosY()) > _RANGE_EXPSHARE)
				continue; // out of share range
			
			if (pUser->GetLev()+20 < nMonsterLev)
			{
				bNewbieBonus = true;					
				break;
			}
		}
	}

	return bNewbieBonus;
}

BOOL CTeam::IsTeamWithTutor(OBJID idKiller, IRole* pTarget)
{
	CHECKF (pTarget);
	
	bool bWithTutor = false;
	for (int i=0; i<this->GetMemberAmount(); i++)
	{
		CUser* pUser = UserManager()->GetUser(this->GetMemberByIndex(i));
		if (pUser)			//? may be another mapgroup
		{
			if (!pUser->IsAlive())
				continue;

			if (pUser->GetMapID() != pTarget->GetMap()->GetID())
				continue;

			if (abs(pUser->GetPosX()-pTarget->GetPosX()) > _RANGE_EXPSHARE ||
					abs(pUser->GetPosY()-pTarget->GetPosY()) > _RANGE_EXPSHARE)
				continue; // out of share range
			
			if (pUser->GetStudent(idKiller))
			{
				bWithTutor = true;
				break;
			}
		}
	}

	return bWithTutor;
}

//////////////////////////////////////////////////////////////////////
void CTeam::BroadcastMemberLife(CUser* pMember, bool bMaxLife)
{
	if (!pMember)
		return;

	CMsgUserAttrib msg;
	if (msg.Create(pMember->GetID(), _USERATTRIB_LIFE, pMember->GetLife()))
	{
		if(bMaxLife)
			msg.Append(_USERATTRIB_MAXLIFE, pMember->GetMaxLife());
		
// 		pUser->SendMsg(&msg);
		BroadcastTeamMsg(&msg,pMember,false);
	}

//	for (int i=0; i<this->GetMemberAmount(); i++)
//	{
//		CUser* pUser	= NULL;
//		OBJID idUser	= this->GetMemberByIndex(i);
//		DEBUG_TRY
//		pUser	= UserManager()->GetUser(idUser);
//		DEBUG_CATCH("Team broadcast member life get user by id crash.")
//		if (pUser && pUser->GetID() != pMember->GetID())
//		{
//			CMsgUserAttrib msg;
//			if (msg.Create(pMember->GetID(), _USERATTRIB_LIFE, pMember->GetLife()))
//			{
//				if(bMaxLife)
//					msg.Append(_USERATTRIB_MAXLIFE, pMember->GetMaxLife());
//
//				pUser->SendMsg(&msg);
//			}
//		}
//	}
}
void CTeam::BroadcastMemberMana(CUser* pMember, bool bMaxMana)
{
	if (!pMember)
		return;

	CMsgUserAttrib msg;
	if (msg.Create(pMember->GetID(), _USERATTRIB_MANA, pMember->GetMana()))
	{
		if(bMaxMana)
			msg.Append(_USERATTRIB_MAXMANA, pMember->GetMaxMana());
		
	// 	pUser->SendMsg(&msg);
		BroadcastTeamMsg(&msg,pMember,false);
	}

//	for (int i=0; i<this->GetMemberAmount(); i++)
//	{
//		CUser* pUser	= NULL;
//		OBJID idUser	= this->GetMemberByIndex(i);
//		DEBUG_TRY
//		pUser	= UserManager()->GetUser(idUser);
//		DEBUG_CATCH("Team broadcast member mana get user by id crash.")
//		if (pUser && pUser->GetID() != pMember->GetID())
//		{
//			CMsgUserAttrib msg;
//			if (msg.Create(pMember->GetID(), _USERATTRIB_MANA, pMember->GetMana()))
//			{
//				if(bMaxMana)
//					msg.Append(_USERATTRIB_MAXMANA, pMember->GetMaxMana());
//
//				pUser->SendMsg(&msg);
//			}
//		}
//	}
}

void CTeam::BroadcastMemberLev(CUser* pMember, bool bLev)
{
	if (!pMember)
		return;

	CMsgUserAttrib msg;
	if (msg.Create(pMember->GetID(), _USERATTRIB_LEV, pMember->GetLev()))
	{
		BroadcastTeamMsg(&msg,pMember,false);
	}

//	for (int i=0; i<this->GetMemberAmount(); i++)
//	{
//		CUser* pUser	= NULL;
//		OBJID idUser	= this->GetMemberByIndex(i);
//		DEBUG_TRY
//		pUser	= UserManager()->GetUser(idUser);
//		DEBUG_CATCH("Team broadcast member mana get user by id crash.")
//		if (pUser /*&& pUser->GetID() != pMember->GetID()*/)
//		{
//			CMsgUserAttrib msg;
//			if (msg.Create(pMember->GetID(), _USERATTRIB_LEV, pMember->GetLev()))
//			{
////				if(bMaxMana)
////					msg.Append(_USERATTRIB_MAXMANA, pMember->GetMaxMana());
//
//				pUser->SendMsg(&msg);
//			}
//		}
//	}
}
//////////////////////////////////////////////////////////////////////
BOOL CTeam::IsTeamMember(OBJID idMember)
{
	for(int i = 0; i < GetMemberAmount(); i++)
	{
		if(GetMemberByIndex(i) == idMember)
			return TRUE;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::IsAccountTeamMember(OBJID idAccount)
{
	for(int i = 0; i < m_setAccountIdUser.size(); i++)
	{
		if(m_setAccountIdUser[i] == idAccount)
			return TRUE;
	}

	return FALSE;
}

// OBJID  CTeam::GetNextTeamLeader()///���õ����߷Ƕӳ���Ա,�����ڸ�����
// {
// 	///�õ��ӳ�ָ��
// 	CUserPtr pLeader=UserManager()->GetUser(GetLeader());
// 	CUserPtr pGoodNextLeader=NULL;
// 	CHECKF(pLeader);///�ӳ�������Ч
// 	OBJID idLeaderMap=pLeader->GetMapID();///�õ��ӳ����ڵ�ͼID
// 	OBJID idGoodNextLeader=ID_NONE;///�����ص���һ�ζӳ�id
// 	OBJID idFirstMember=ID_NONE;
// 	for(int i = 0 ; i < 6 ; i ++)
// 	{
// 		///��Ч��Ա�����߶�Ա������
// 		if(m_TeamIn[i].id==ID_NONE || m_TeamIn[i].bOnlineStatus==false || m_TeamIn[i].id==GetLeader())
// 			continue;
// 
// 		///���汯�ۺ�ѡ��
// 		if(idFirstMember==ID_NONE)
// 			idFirstMember=m_TeamIn[i].id;
// 		///���Եõ������Ա��ʵ��
// 		pGoodNextLeader=UserManager()->GetUser(m_TeamIn[i].id);
// 		if(pGoodNextLeader && pGoodNextLeader->GetMapID()>DYNAMIC_MAP_ID)
// 		{
// 			idGoodNextLeader=m_TeamIn[i].id;
// 			break;
// 		}
// 		else if(!pGoodNextLeader && idLeaderMap<DYNAMIC_MAP_ID)
// 		{
// 			idGoodNextLeader=m_TeamIn[i].id;
// 			break;
// 		}
// 	}
// 	if(idGoodNextLeader==ID_NONE)
// 		idGoodNextLeader=idFirstMember;
// 	return idGoodNextLeader;
// }

// OBJID  CTeam::GetOnlineTeamMemberOnly()///���õ����߳�Ա�Ƕӳ�
// {
// 	for(int i = 0; i < GetMemberAmount(); i++)
// 	{
// 		if(GetMemberByIndex(i) != GetLeader() && IsMemberOnline(GetMemberByIndex(i)))
// 			return GetMemberByIndex(i);
// 	}
// 	return ID_NONE;
// }

/////////////////////////////////////////////////////////////////////
OBJID CTeam::GetTeamMemberOnly()///���õ���Ա�Ƕӳ�
{
	for(int i = 0; i < GetMemberAmount(); i++)
	{
		if(GetMemberByIndex(i) != GetLeader())
			return GetMemberByIndex(i);
	}

	return ID_NONE;
}

///�ж϶�Ա�Ƿ�����
bool CTeam::IsMemberOnline(OBJID idUser)
{
	for(int i = 0 ; i < 6 ; i ++)
	{
		if(m_TeamIn[i].id == idUser && m_TeamIn[i].bOnlineStatus)
		{
			return true;
		}
	}
	return false;
}

///�õ����������������
int CTeam::GetOnlineMemberAmount(int nSex)
{
	if (nSex < 0 || nSex > 2)
	{
		LogSave("�ű�����һ����ֵ��Ա������ṩ�ӿڴ�");
		return 0;
	}
	int nOnlineMemberAmount=0;
	for(int i = 0 ; i < 6 ; i ++)
	{
		if(m_TeamIn[i].id == ID_NONE)
			continue;
		if(nSex == 2)
		{
			if(m_TeamIn[i].bOnlineStatus)
				nOnlineMemberAmount++;
		}
		else
		{
			CUserPtr pUser = UserManager()->GetUser(m_TeamIn[i].id);//���Ի�������ҵ�ָ��
			if(pUser && (pUser->GetSex() == nSex))
				nOnlineMemberAmount++;
		}
	}
	return nOnlineMemberAmount;
}

void CTeam::ResetLocation(BOOL bTransmitMsg)
{
	DEBUG_TRY
	for(int i = 0 ; i < 6 ; i ++)
	{
		if(m_TeamIn[i].id!=ID_NONE)
		{
			m_TeamIn[i].idMap=0;
			m_TeamIn[i].usPosX=0;
			m_TeamIn[i].usPosY=0;
		}			
	}
	///�㲥��������ͼ��
	if(bTransmitMsg)
			MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_CLRLOCATION,GetID(),0,0,0,0);
	DEBUG_CATCH("CTeam::ResetLocation");
}

void CTeam::ResetOfflineLocation(BOOL bTransmitMsg)///���������߶�Ա�ı���ĸ�����ͼλ������Ϊ0
{
	DEBUG_TRY
		for(int i = 0 ; i < 6 ; i ++)
		{
			if(m_TeamIn[i].id!=ID_NONE && m_TeamIn[i].bOnlineStatus==false)
			{
				m_TeamIn[i].idMap=0;
				m_TeamIn[i].usPosX=0;
				m_TeamIn[i].usPosY=0;
				///�㲥��������ͼ��
				if(bTransmitMsg)
					MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_SETLOCATION,GetID(),m_TeamIn[i].id,0,0,0);
			}
		}
		DEBUG_CATCH("CTeam::ResetOfflineLocation");
}

void CTeam::SetLocation(OBJID idMember,OBJID idMap,int x,int y,BOOL bTransmitMsg)///����ʱ����һ�¶�Ա��λ��
{
	for(int i = 0 ; i < 6 ; i ++)
	{
		if(m_TeamIn[i].id == idMember)
		{
			m_TeamIn[i].idMap=idMap;
			m_TeamIn[i].usPosX=x;
			m_TeamIn[i].usPosY=y;
			///�㲥״̬���ͻ���
			///�㲥��������ͼ��
			if(bTransmitMsg)
				MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_SETLOCATION,GetID(),idMember,idMap,x,y);//nData
			break;
		}
	}
}

void CTeam::SetOnlineStatus(OBJID idMember,BOOL bStatus,BOOL bTransmitMsg)
{
	for(int i = 0 ; i < 6 ; i ++)
	{
		if(m_TeamIn[i].id == idMember)
		{
			m_TeamIn[i].bOnlineStatus=bStatus;
			
			///�㲥��������ͼ��
			if(bTransmitMsg)
			{
				MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_ONLINESTATUSCHANGE,GetID(),idMember,bStatus);//nData
				
				///�㲥״̬���ͻ���
				CMsgTeam msg;
				if(msg.Create(idMember,_MSGTEAM_UPDATA_ONLINEINFO,bStatus))
					BroadcastTeamMsg(&msg,NULL);
			}
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////
CTeam* CTeam::CloneNew()
{
	CTeam* pTeam	= new CTeam(m_idProcess);
	CHECKF(pTeam);

	*pTeam = *this;

	return pTeam;
}

//////////////////////////////////////////////////////////////////////
void CTeam::GetInfo(TeamInfoStruct* pInfo)
{
	memset(pInfo, 0, sizeof(TeamInfoStruct));
	pInfo->id		= m_id;
	pInfo->idLeader	= m_idLeader;
	for(int i = 0; i < m_setIdUser.size() && i < _MAX_TEAMAMOUNT; i++)
	{
		pInfo->setMember[i]	= m_setIdUser[i];
	}
	//pInfo->bCloseMoney	= m_bCloseMoney;
	//pInfo->bCloseItem	= m_bCloseItem;
	
	//nfo->nTeamMode = m_nTeamMode;
	//pInfo->nExpMode = m_nExpMode;
	//pInfo->nItemMode = m_nItemMode;
	strcpy(pInfo->sTeamName,m_sTeamName);
	//------------------------------

	//nfo->m_nLev = m_nLev;
	//nfo->m_nTarget = m_nTarget;
	//rcpy(pInfo->m_sLeadName, m_sLeadName);
	//rcpy(pInfo->m_sTeamText, m_sTeamText);
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CTeam::AddMemberOnly		(TeamMemberInfo menberInfo)
{
	USERIDSET::iterator it = find(m_setIdUser.begin(), m_setIdUser.end(), menberInfo.id);
	if(it == m_setIdUser.end())   //û�ҵ�������һ����Ա
	{
		m_setIdUser.push_back(menberInfo.id);
// 		UINT accountID = PostOffice()->GetReceiverAccountID(idMember);
// 		m_setAccountIdUser.push_back(accountID);
	}
	int inIndex = -1;
	for(int i = 0 ; i < 6 ; i ++)
	{
		if(m_TeamIn[i].id == menberInfo.id)
			break;
		if(inIndex == -1 && m_TeamIn[i].id == 0)
		{
			inIndex = i;
			break;
		}
	}

	if(inIndex < 6 && inIndex != -1)
	{
		::memcpy(&m_TeamIn[inIndex],&menberInfo,sizeof(TeamMemberInfo));
	}

	if(menberInfo.id == m_idLeader)
	{
		sprintf(m_sTeamName,"%s��С��" ,menberInfo.szName);
		SafeCopy(m_sLeadName,menberInfo.szName,_MAX_NAMESIZE);
	}
}

//////////////////////////////////////////////////////////////////////
void CTeam::DelMemberOnly		(OBJID idMember)
{
	USERIDSET::iterator iter = find(m_setIdUser.begin(), m_setIdUser.end(), idMember);
	if(iter != m_setIdUser.end())
		m_setIdUser.erase(iter);

	for(int i = 0 ; i < 6 ; i ++)
	{
		if(m_TeamIn[i].id == idMember)
		{
			m_TeamIn[i].id = 0;
		}
	}

// 	UINT accountID = PostOffice()->GetReceiverAccountID(idMember);
// 	USERIDSET::iterator iter1 = find(m_setAccountIdUser.begin(), m_setAccountIdUser.end(), accountID);
// 	if(iter1 != m_setAccountIdUser.end())
// 		m_setAccountIdUser.erase(iter1);
}


//////////////////////////////////////////////////////////////////////
void CTeam::AttachMemberStatus(int nStatus, int nPower, int nSecs, int nTimes, OBJID idExclude)
{
//-----------07.4.9����ע��
//	CUser* pLeader = UserManager()->GetUser(this->GetLeader());
//	if (!pLeader || !pLeader->IsAlive())
//		return ;		// �ӳ������ڻ�������
//
//	for (int i=0; i<this->GetMemberAmount(); i++)
//	{
//		CUser*	pUser = UserManager()->GetUser(this->GetMemberByIndex(i));
//		if (pUser)				//? may be another mapgroup
//		{
//			// ����Ҽ�״̬
//			if (pUser->GetID() != idExclude)
//			{
//				if (pUser->IsAlive()
//					&& pUser->GetMapID() == pLeader->GetMapID()
//					&& (pUser->GetDistance(pLeader->QueryMapThing()) <= _RANGE_TEAM_STATUS || STATUS_ADD_EXP == nStatus))
//				{
//					if (!pUser->QueryStatus(nStatus))
//						CRole::AttachStatus(pUser->QueryRole(), nStatus, nPower, nSecs, nTimes);
//				}
//			}
//		}
//	}
}
//////////////////////////////////////////////////////////////////////
void CTeam::AdjustMemberStatus(int nOldAmount, int nNewAmount)
{
	//����ע��
/*	USERIDSET::iterator it=m_setIdUser.begin();
	for (; it!=m_setIdUser.end(); it++)
	{
		CUser* pUser = UserManager()->GetUser(*it);
		if (pUser)
		{
			// ֻ���ٶ���Ҫ֪ͨ�ͻ��ˣ��������״̬��Ӱ��Ч�����Ǽ�ʱ���������֪ͨ�ͻ���
			if (pUser->QueryStatus(STATUS_TEAMSPEED))
			{
				CMsgUserAttrib msg;
				IF_OK (msg.Create(pUser->GetID(), _USERATTRIB_SPEED, pUser->AdjustSpeed(pUser->GetSpeed())))
					pUser->BroadcastRoomMsg(&msg, INCLUDE_SELF);
			}
		}
	}

	CUser* pLeader = UserManager()->GetUser(this->GetLeader());
	if (pLeader)
	{
		for (int i=STATUS_TEAM_BEGIN; i<=STATUS_TEAM_END; i++)
		{
			if (pLeader->QueryStatus(i))
			{
				char szMsg[256] = "";
				if (nNewAmount < TEAM_STATUS_REQ_ROLES)
					::SafeCopy(szMsg, STR_TEAM_STATUS_DISAPPEAR, 256);
				else 
				{
					if (nOldAmount < nNewAmount)
						sprintf(szMsg, STR_TEAM_STATUS_POWER_INC, 10*(nNewAmount-nOldAmount));
					else
						sprintf(szMsg, STR_TEAM_STATUS_POWER_DEC, 10*(nOldAmount-nNewAmount));
				}
				CMsgTalk msg;
				if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
					this->BroadcastTeamMsg(&msg);

				break;
			}
		}
	}*/
}
//////////////////////////////////////////////////////////////////////
// ƽ���˺��������޸ĺ��ʵ���˺�
int CTeam::ShareDamage(int nDamage, OBJID idExclude)
{

	return nDamage;
}

//���������������ÿ�������������һ��Сʱ��������������100�㡣
//void CTeam::DealTeamLeaderInfo()
//{
//	DEBUG_TRY
//
//	if(this->m_tGuidingTime.ToNextTime())
//	{
//		CUser * pLeader = UserManager()->GetUser(this->GetLeader());
//		if(pLeader)
//		{
//			CSyndicate * pSyn = pLeader->GetSyndicate();
//			if(pSyn)
//			{
//				pSyn->QueryModify()->AddData(SYNDATA_REPUTE, TEAMLEADER_WITHNEWBIE_REPUTEINC, true);	
//				//�����������
//			//	pLeader->QuerySynAttr()->AddReputeInc(TEAMLEADER_WITHNEWBIE_REPUTEINC);
//			}
//		}
//	}
//
//	DEBUG_CATCH("void CTeam::DealTeamLeaderInfo()")
//}

bool CTeam::DevolveOnLeader(int newLeaID)
{
	return false;
}

void CTeam::ChangeTeamText(char *pText)
{
	CHECK(pText);

	SetTeamText(pText);

	//����С�ӳ�Ա
	for(int i = 0; i < m_setIdUser.size(); i++)
	{
		CUser* pPlayer = UserManager()->GetUser(m_setIdUser[i]);
		if(pPlayer)//�ڱ���ͼ��
		{
			CTeam* chgTeam = pPlayer->GetTeam();
			CHECK(chgTeam);
			chgTeam->SetTeamText(pText);
		}
		else//�ڱ�ĵ�ͼ��
		{
// 			MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_TEAMTEXT, 
// 				GetID(), 
// 				m_setIdUser[i],//idUser
// 				0,//nData
// 				pText);
		}
	}
}

//20070202����:С�Ӹ���
void CTeam::ChangeTeamName(char *teamName)
{
	//����������С�Ӹ���.�����ǿͻ���ͬ��.
	//�ͻ���ͬ����������һ����ﴦ��
	CHECK(teamName);

	//�Լ�������
	this->SetTeamName(teamName);

	//����С�ӳ�Ա
	int nSize = m_setIdUser.size();
	for(int i = 0; i < nSize; i++)
	{
		CUser* pPlayer = UserManager()->GetUser(m_setIdUser[i]);
		if(pPlayer)//�ڱ���ͼ��
		{
			pPlayer->ChangeTeamNameOnly(teamName);
		}
		else//�ڱ�ĵ�ͼ��
		{
// 			MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_CHANGENAME, 
// 				GetID(), 
// 				m_setIdUser[i],//idUser
// 				0,//nData
// 				teamName);
		}
	}

}
//--------------------------------------------
//20070203����:�ӳ����
void CTeam::ChangeTeamLeader(OBJID leaderID)
{
	if(m_bEudemonBron)
		EudemonCancel();

	CHECK(leaderID);

	this->SetTeamLeaderOnly(leaderID);

	//����С�ӳ�Ա
// 	int nSize = m_setIdUser.size();
// 	for(int i = 0; i < nSize; i++)
// 	{
// 		CUser* pPlayer = UserManager()->GetUser(m_setIdUser[i]);
// 		if(pPlayer)//�ڱ���ͼ��
// 		{
// 			CTeam *pPlayerTeam=NULL;
// 			pPlayerTeam = pPlayer->GetTeam();
// 			CHECK(pPlayerTeam);
// 			pPlayerTeam->SetTeamLeaderOnly(leaderID);
// 			pPlayerTeam->SetTeamLeaderName(pPlayer->GetName());
// 
// 			if(pPlayer->GetID() == leaderID)
// 			{
// 				pPlayer->AddTeamLeader(leaderID);
// 				char pText[36];
// 				sprintf(pText, "%s��СС��", pPlayer->GetName());
// 				pPlayerTeam->ChangeTeamName(pText);
// 			}
// 
// 			if(pPlayer->GetID() == teamLeaderTemp)
// 				pOldLeader = pPlayer;
// 		}
//		MapGroup(PID)->SendBroadcastMsg()
// 		else//�ڱ�ĵ�ͼ��
// 		{
// 			MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_CHANGLEADER, 
// 				GetID(), 
// 				m_setIdUser[i],//idUser
// 				leaderID);//nData
// 		}
//	}

	//��Ч----------------
	//��ɾ��ԭ�ӳ���Ч
// 	CUser* pPlayer = UserManager()->GetUser(teamLeaderTemp);
// 	if(pPlayer)//�ڱ���ͼ��
// 	{
// 		pPlayer->ClsStatus(STATUS_TEAMLEADER);
// 	}
// 	else//�ڱ�ĵ�ͼ��
// 	{
// 		MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_CHANGLEADER_CLSEFFECT, 
// 			GetID(), 
// 			teamLeaderTemp);//idUser
// 	}
	
	
	//����¶ӳ���Ч
// 	pPlayer = UserManager()->GetUser(leaderID);
// 	if(pPlayer)//�ڱ���ͼ��
// 	{
// 		pPlayer->SetStatus(STATUS_TEAMLEADER);
// 	}
// 	else//�ڱ�ĵ�ͼ��
// 	{
// 		MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_CHANGLEADER_SETEFFECT, 
// 			GetID(), 
// 			leaderID);//idUser
// 	}

	

	MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_CHANGLEADER, 
		GetID(), 
		leaderID);//nData
}
//--------------------------------------
//20070205����:�ӳ�����.�����öӳ�.��������
void CTeam::SetTeamLeaderOnly(OBJID leaderID)
{
	//�����ǲ�ͬ��ͼ��.����㺯����֤.
	CHECK(leaderID);

	OBJID teamLeaderTemp = GetLeader();
	CUser* pLeader = UserManager()->GetUser(leaderID);
	if(pLeader)
		this->SetTeamLeaderName(pLeader->GetName());
	m_idLeader	= leaderID;
	
	CUser* pOldLeader = UserManager()->GetUser(teamLeaderTemp);
	if(pLeader)
	{
// 		CMsgTeam msg;
// 		msg.Create(pLeader->GetID(), pLeader->GetName(),_MSGTEAM_ASKINFO);
// 		pLeader->SendMsg(&msg);

		CMsgPlayer msgPlayer;
		if (msgPlayer.Create(pLeader->QueryRole()))
			pLeader->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
	}
	if(pOldLeader)
	{
// 		CMsgTeam msg;
// 		msg.Create(pOldLeader->GetID(), pOldLeader->GetName(),_MSGTEAM_ASKINFO);
// 		pOldLeader->SendMsg(&msg);

		CMsgPlayer msgPlayer;
		if (msgPlayer.Create(pOldLeader->QueryRole()))
			pOldLeader->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
	}
}

void CTeam::SetTeamLeaderName(const char *leaderName)
{
	strcpy(m_sLeadName, leaderName);
}
//--------------------------------------
//20070207����:�޸�����
void CTeam::ChangeTeamInfo(int teamInfo)
{
	//���з���������.��������ͻ���ͬ��
	//teamInfo���þ��ݵ�...д��

	//�Լ�������---------
	ChangeTeamInfoOnly(teamInfo);
	//-------------------

	//����С�ӳ�Ա
	int nSize = m_setIdUser.size();
	for(int i = 0; i < nSize; i++)
	{
		CUser* pPlayer = UserManager()->GetUser(m_setIdUser[i]);
		if(pPlayer)//�ڱ���ͼ��
		{
			CTeam *pPlayerTeam=NULL;
			pPlayerTeam = pPlayer->GetTeam();
			CHECK(pPlayerTeam);
			//-----------------
			pPlayerTeam->ChangeTeamInfoOnly(teamInfo);
			//-----------------
		}
	}
}
//-------------------------------------
void CTeam::ChangeTeamInfoOnly(int teamInfo)//�����޸�
{
	SetTeamMode(teamInfo & 0x3);//ȡ1��2λ
	SetExpMode((teamInfo & 0x0c)>>2);//ȡ3��4λ
	SetItemMode((teamInfo & 0x70)>>4);//ȡ5��6��7λ
	if(teamInfo & 0x2)
		Close();
	else
		Open();
}


int CTeam::GetTeamAddExp(int nExp, int nMemberCount)
{
	return (int)(nExp * (0.8f + nMemberCount * 0.2f));
}

// ʦͽ��Ӽ�BUF [�½���10/26/2007]
//void CTeam::AwardBufForTeacher()
//{
//	int nSize = m_setIdUser.size();
//	for(int i = 0; i < nSize; i++)
//	{
//		CUser* pTutor = UserManager()->GetUser(m_setIdUser[i]);
//		if(pPlayer)//�ڱ���ͼ��
//		{
//
//		}
//		else//�ڱ�ĵ�ͼ��
//		{
//
//		}
//	}
//}


// By Minsky,2010-05-28
void CTeam::AddMemberApply(OBJID idMember)
{
// 	USERIDSET::iterator it = find(m_setIdApply.begin(), m_setIdApply.end(), idMember);
// 	if(it == m_setIdUser.end() || it == NULL || *it == ID_NONE)   //û�ҵ�������һ�������Ա
// 	{
// 		m_setIdApply.push_back(idMember);
// 	}
	int nAmount = m_setIdApply.size();
	if (nAmount == 8)// By Minsky,2010-05-28 ����������ܷ���
	{
		return;
	}
	for(int i=0; i<nAmount; i++)
	{
		if (m_setIdApply[i] == idMember)// �ҵ��ˣ�����Ҫ������
		{
			return;
		}
	}
	m_setIdApply.push_back(idMember);
}

bool CTeam::DelMemberApply(OBJID idMember)
{
	USERIDSET::iterator iter = find(m_setIdApply.begin(), m_setIdApply.end(), idMember);
	if(iter != m_setIdApply.end())
	{
		m_setIdApply.erase(iter);
		return true;
	}
	return false;
}

// By Minsky,2010-05-28
OBJID CTeam::GetMemberApplyByIndex(int nIndex)
{
	if (nIndex >= m_setIdApply.size() || nIndex < 0)
		return ID_NONE;
	
	return m_setIdApply[nIndex];
}

//  By Minsky,2010-05-28
int CTeam::GetMemberApplyAmount()
{
	return __min(_MAX_TEAMAMOUNTOUT, m_setIdApply.size());
}


bool CTeam::initEudemonBron()
{
	if(m_setIdUser.size() != 2)
	{
		return false;
	}

	CUser* pPlayer = UserManager()->GetUser(m_setIdUser[0]);
	if(pPlayer)
	{
		if(!pPlayer->CanEudemonBorn())
		{
			return false;
		}
		m_bronUser[0] = m_setIdUser[0];
		m_bronEudemon[0] = 0;
		m_bEudemonBronLock[0] = false;
	}
	else
		return false;

	pPlayer = UserManager()->GetUser(m_setIdUser[0]);
	if(pPlayer)
	{
		if(!pPlayer->CanEudemonBorn())
		{
			return false;
		}
		m_bronUser[1] = m_setIdUser[1];
		m_bronEudemon[1] = 0;	
		m_bEudemonBronLock[1] = false;
	}
	else
		return false;

	m_bEudemonBron = true;
//	SendEudemonBronInfo(m_setIdUser[0],m_setIdUser[1]);
//	SendEudemonBronInfo(m_setIdUser[1],m_setIdUser[0]);
	return false;
}

bool CTeam::initEudemonPlay()
{
	if(m_setIdUser.size() != 2)
	{
		return false;
	}
	
	CUser* pPlayer = UserManager()->GetUser(m_setIdUser[0]);
	if(pPlayer)
	{
		if(!pPlayer->CanEudemonBorn())
		{
			return false;
		}
		m_playUser[0] = m_setIdUser[0];
		m_playEudemon[0] = 0;
		m_bEudemonPlayLock[0] = false;
	}
	else
		return false;
	
	pPlayer = UserManager()->GetUser(m_setIdUser[0]);
	if(pPlayer)
	{
		if(!pPlayer->CanEudemonBorn())
		{
			return false;
		}
		m_playUser[1] = m_setIdUser[1];
		m_playEudemon[1] = 0;	
		m_bEudemonPlayLock[1] = false;
	}
	else
		return false;
	
	m_bEudemonPlay = true;

	return false;
}

bool CTeam::EudemonBronChange(OBJID idUser,OBJID idEudmon)
{
	if(!m_bEudemonBron)
		return false;

	if(m_setIdUser.size() != 2)
	{
		return false;
	}

	if(m_bronUser[0] == idUser)
	{
		if(m_bEudemonBronLock[0])
		{
			return false;
		}
		else
		{
			CUser *pUser1 = UserManager()->GetUser(m_bronUser[0]);
			CUser *pUser2 = UserManager()->GetUser(m_bronUser[1]);
			if(!pUser1 || !pUser2)
				return false;
			OBJID idOldEudemon = m_bronEudemon[0];
			if(idEudmon == ID_NONE)
			{
				m_bronEudemon[0] = ID_NONE;
			}
			else if(pUser1->CanEudemonBorn(idEudmon))
			{
				m_bronEudemon[0] = idEudmon;
			}
			else
			{
				return false;
			}

			//����ԭ��Ʒ
			if(idOldEudemon != ID_NONE)
				pUser1->UnLockEudemon(idOldEudemon);
			if(m_bronEudemon[0] != ID_NONE)
				pUser1->LockEudemon(m_bronEudemon[0]);

			if(m_bEudemonBronLock[1])
			{
				m_bEudemonBronLock[1] = false;
				CUser *pUser1 = UserManager()->GetUser(m_bronUser[0]);
				CUser *pUser2 = UserManager()->GetUser(m_bronUser[1]);
				if(pUser1 && pUser2)
				{
					CMsgEudemon msg;
					if(msg.Create(PETACTION_UnLockBornPet,NULL,m_bronUser[1]))
					{
						pUser1->SendMsg(&msg);
						pUser2->SendMsg(&msg);
					}
				}
			}
			SendEudemonBronInfo(m_bronUser[0]);
		}
	}
	else if(m_bronUser[1] == idUser)
	{
		if(m_bEudemonBronLock[1])
		{
			return false;
		}
		else
		{
			CUser *pUser1 = UserManager()->GetUser(m_bronUser[0]);
			CUser *pUser2 = UserManager()->GetUser(m_bronUser[1]);
			if(!pUser1 || !pUser2)
				return false;
			OBJID idOldEudemon = m_bronEudemon[1];
			if(idEudmon == ID_NONE)
			{
				m_bronEudemon[1] = ID_NONE;
			}
			else if(pUser2->CanEudemonBorn(idEudmon))
			{
				m_bronEudemon[1] = idEudmon;
			}
			else
			{
				return false;
			}
			
			//����ԭ��Ʒ
			//����ԭ��Ʒ
			if(idOldEudemon != ID_NONE)
				pUser1->UnLockEudemon(idOldEudemon);
			if(m_bronEudemon[1] != ID_NONE)
				pUser1->LockEudemon(m_bronEudemon[1]);


			if(m_bEudemonBronLock[0])
			{
				m_bEudemonBronLock[0] = false;
				CUser *pUser1 = UserManager()->GetUser(m_bronUser[0]);
				CUser *pUser2 = UserManager()->GetUser(m_bronUser[1]);
				if(pUser1 && pUser2)
				{
					CMsgEudemon msg;
					if(msg.Create(PETACTION_UnLockBornPet,NULL,m_bronUser[0]))
					{
						pUser1->SendMsg(&msg);
						pUser2->SendMsg(&msg);
					}
				}
			}
			SendEudemonBronInfo(m_bronUser[1]);
		}
	}
	return false;
}
bool CTeam::EudemonBronChange(OBJID idUser,bool idLock)
{
	if(!m_bEudemonBron)
		return false;

	if(!idLock)
		return false;
	
	if(m_setIdUser.size() != 2)
	{
		return false;
	}
	
	if(m_bronUser[0] == idUser)
	{
		if(m_bEudemonBronLock[0])
		{
			return false;
		}
		else
		{
			m_bEudemonBronLock[0] = true;
			CUser *pUser1 = UserManager()->GetUser(m_bronUser[0]);
			CUser *pUser2 = UserManager()->GetUser(m_bronUser[1]);
			if(pUser1 && pUser2)
			{
				CMsgEudemon msg;
				if(msg.Create(PETACTION_LockBornPet,NULL,idUser))
				{
					pUser1->SendMsg(&msg);
					pUser2->SendMsg(&msg);
				}
			}
		}
	}
	else if(m_bronUser[1] == idUser)
	{
		if(m_bEudemonBronLock[1])
		{
			return false;
		}
		else
		{
			m_bEudemonBronLock[1] = true;
			CUser *pUser1 = UserManager()->GetUser(m_bronUser[0]);
			CUser *pUser2 = UserManager()->GetUser(m_bronUser[1]);
			if(pUser1 && pUser2)
			{
				CMsgEudemon msg;
				if(msg.Create(PETACTION_LockBornPet,NULL,idUser))
				{
					pUser1->SendMsg(&msg);
					pUser2->SendMsg(&msg);
				}
			}
		}
	}
	return false;
}

bool CTeam::EudemonBron()
{
	if(!m_bEudemonBron)
		return false;

	CUser *pUser1 = UserManager()->GetUser(m_bronUser[0]);
	CUser *pUser2 = UserManager()->GetUser(m_bronUser[1]);
	if(!pUser1 || !pUser2)
		return false;

	if(!m_bEudemonBronLock[0])
		return false;

	if(!m_bEudemonBronLock[1])
		return false;

	pUser1->UnLockEudemon(m_bronEudemon[0]);
	pUser2->UnLockEudemon(m_bronEudemon[1]);
	
	if(pUser1->EudemonBron(2,m_bronEudemon[0],m_bronEudemon[1],pUser2))
	{
		CUser *pUser1 = UserManager()->GetUser(m_bronUser[0]);
		CUser *pUser2 = UserManager()->GetUser(m_bronUser[1]);
		if(pUser1 && pUser2)
		{
			CMsgEudemon msg;
			if(msg.Create(PETACTION_BornBegin,NULL,0))
			{
				pUser1->SendMsg(&msg);
				pUser2->SendMsg(&msg);
			}
		}
	}
	else
	{
		EudemonCancel();
	}
	m_bEudemonBron = false;
	return true;
}

bool CTeam::EudemonCancel()
{
	if(!m_bEudemonBron)
		return false;

	
	CMsgEudemon msg;
	if(!msg.Create(PETACTION_BornStop,NULL))
		return false;

	CUser *pUser1 = UserManager()->GetUser(m_bronUser[0]);
	if(pUser1)
	{
		if(m_bronEudemon[0] != ID_NONE)
			pUser1->UnLockEudemon(m_bronEudemon[0]);
		pUser1->SendMsg(&msg);
	}
	CUser *pUser2 = UserManager()->GetUser(m_bronUser[1]);
	{
		if(m_bronEudemon[1] != ID_NONE)
			pUser2->UnLockEudemon(m_bronEudemon[1]);
		pUser2->SendMsg(&msg);
	}

	m_bEudemonBron= false;
	return true;
}

bool CTeam::SendEudemonBronInfo(OBJID idUser,OBJID idUserTarget)
{
	if(!m_bEudemonBron)
		return false;

	int index = -1;
	if(m_bronUser[0] == idUser)
		index = 0;
		
	if(m_bronUser[1] == idUser)
		index = 1;

	if(index > 1 || index < 0)
		return false;

	CUser* pUser = UserManager()->GetUser(idUser);
	if(!pUser)
		return false;

	CUser* pUserTarget1 = NULL;
	CUser* pUserTarget2 = NULL;
	if(idUserTarget == ID_NONE)
	{
		pUserTarget1 = UserManager()->GetUser(m_bronUser[0]);
		pUserTarget2 = UserManager()->GetUser(m_bronUser[1]);

		if(!pUserTarget1 || !pUserTarget2)
			return false;
	}
	else if((idUserTarget == m_bronUser[0])  || (idUserTarget == m_bronUser[1]))
	{
		pUserTarget1 = UserManager()->GetUser(idUserTarget);
		if(!pUserTarget1)
			return false;
	}

	if(m_bronEudemon[index]/* && pUser->CanEudemonBorn(m_bronEudemon[index])*/)
	{
		EudemonData data;
		pUser->LoadEudemon(m_bronEudemon[index],data,2);
		CMsgEudemon msg;
		if(msg.Create(PETACTION_PetBorn,&data,idUser))
		{
			if(pUserTarget1)
				pUserTarget1->SendMsg(&msg);
			if(pUserTarget2)
				pUserTarget2->SendMsg(&msg);
		}
	}
	else
	{
		CMsgEudemon msg;
		if(msg.Create(PETACTION_BornStop,NULL,idUser))
		{
			if(pUserTarget1)
				pUserTarget1->SendMsg(&msg);
			if(pUserTarget2)
				pUserTarget2->SendMsg(&msg);
		}
	}
	return true;
}

bool CTeam::CheckTutorActive(CUser* pLeader)
{
	DEBUG_TRY
	OBJID idLeader=GetLeader();
	if (pLeader->GetID() != idLeader)
		return false;

	CUser* pMember	= NULL;
	CTutorData* pData = NULL;
	for(int i = 0 ; i <_MAX_TEAMAMOUNT ; i ++)
	{
		if (m_TeamIn[i].id==ID_NONE)
			continue;
		pData = pLeader->GetStudent(m_TeamIn[i].id);
		if (!pData)//����ͽ�ܲ����ж�
			continue;
		if(pData->GetState()!=TUTORSTATE_NORMAL && pData->GetState()!=TUTORSTATE_AGREED)
			continue;
		pMember = UserManager()->GetUser(m_TeamIn[i].id);
		if (!pMember)
		{
			pLeader->SendSysMsg(_TXTATR_TOPSHOW,"ͽ��%s��������߻��߲�����!",m_TeamIn[i].szName);//STR_INTEAM_NOJOIN
			return false;
		}
		
		if(!pMember->CanBeSee())
			return false;
		
		if(pMember->GetMapID() != pLeader->GetMapID())
		{
			//pLeader->SendSysMsg(_TXTATR_TOPSHOW,"ͽ��%s���㲻��ͬһ��ͼ!",pMember->GetName());//STR_INTEAM_NOJOIN
			return false;
		}
		
		if(pMember->GetDistance(pLeader) > 20)
		{
			pLeader->SendSysMsg(_TXTATR_TOPSHOW,"ͽ��%s�������̫Զ!",pMember->GetName());//STR_INTEAM_NOJOIN
			return false;
		}
	}
	
	DEBUG_CATCH("CTeam::CheckTutorActive crash!")
	return true;
}

bool CTeam::EudemonPlayChange(OBJID idUser,OBJID idEudmon)
{
	if(!m_bEudemonPlay)
		return false;

	if(m_setIdUser.size() != 2)
	{
		return false;
	}

	if(m_playUser[0] == idUser)
	{
		if(m_bEudemonPlayLock[0])
		{
			return false;
		}
		else
		{
			CUser *pUser1 = UserManager()->GetUser(m_playUser[0]);
			CUser *pUser2 = UserManager()->GetUser(m_playUser[1]);
			if(!pUser1 || !pUser2)
				return false;
			OBJID idOldEudemon = m_playEudemon[0];
			if(idEudmon == ID_NONE)
			{
				m_playEudemon[0] = ID_NONE;
			}
			else if(pUser1->CanEudemonPlay(idEudmon))
			{
				m_playEudemon[0] = idEudmon;
			}
			else
			{
				return false;
			}

			//����ԭ��Ʒ
			if(idOldEudemon != ID_NONE)
				pUser1->UnLockEudemon(idOldEudemon);
			if(m_playEudemon[0] != ID_NONE)
				pUser1->LockEudemon(m_playEudemon[0]);

			if(m_bEudemonPlayLock[1])
			{
				m_bEudemonPlayLock[1] = false;
				CUser *pUser1 = UserManager()->GetUser(m_playUser[0]);
				CUser *pUser2 = UserManager()->GetUser(m_playUser[1]);
				if(pUser1 && pUser2)
				{
					CMsgEudemon msg;
					if(msg.Create(PETACTION_UnLockPlayPet,NULL,m_playUser[1]))
					{
						pUser1->SendMsg(&msg);
						pUser2->SendMsg(&msg);
					}
				}
			}
			SendEudemonPlayInfo(m_playUser[0]);
		}
	}
	else if(m_playUser[1] == idUser)
	{
		if(m_bEudemonPlayLock[1])
		{
			return false;
		}
		else
		{
			CUser *pUser1 = UserManager()->GetUser(m_playUser[0]);
			CUser *pUser2 = UserManager()->GetUser(m_playUser[1]);
			if(!pUser1 || !pUser2)
				return false;
			OBJID idOldEudemon = m_playEudemon[1];
			if(idEudmon == ID_NONE)
			{
				m_playEudemon[1] = ID_NONE;
			}
			else if(pUser2->CanEudemonPlay(idEudmon))
			{
				m_playEudemon[1] = idEudmon;
			}
			else
			{
				return false;
			}
			
			//����ԭ��Ʒ
			//����ԭ��Ʒ
			if(idOldEudemon != ID_NONE)
				pUser1->UnLockEudemon(idOldEudemon);
			if(m_playEudemon[1] != ID_NONE)
				pUser1->LockEudemon(m_playEudemon[1]);


			if(m_bEudemonPlayLock[0])
			{
				m_bEudemonPlayLock[0] = false;
				CUser *pUser1 = UserManager()->GetUser(m_playUser[0]);
				CUser *pUser2 = UserManager()->GetUser(m_playUser[1]);
				if(pUser1 && pUser2)
				{
					CMsgEudemon msg;
					if(msg.Create(PETACTION_UnLockPlayPet,NULL,m_playUser[0]))
					{
						pUser1->SendMsg(&msg);
						pUser2->SendMsg(&msg);
					}
				}
			}
			SendEudemonPlayInfo(m_playUser[1]);
		}
	}
	return false;
}

bool CTeam::SendEudemonPlayInfo(OBJID idUser,OBJID idUserTarget)
{
	if(!m_bEudemonPlay)
		return false;
	
	int index = -1;
	if(m_playUser[0] == idUser)
		index = 0;
	
	if(m_playUser[1] == idUser)
		index = 1;
	
	if(index > 1 || index < 0)
		return false;
	
	CUser* pUser = UserManager()->GetUser(idUser);
	if(!pUser)
		return false;
	
	CUser* pUserTarget1 = NULL;
	CUser* pUserTarget2 = NULL;
	if(idUserTarget == ID_NONE)
	{
		pUserTarget1 = UserManager()->GetUser(m_playUser[0]);
		pUserTarget2 = UserManager()->GetUser(m_playUser[1]);
		
		if(!pUserTarget1 || !pUserTarget2)
			return false;
	}
	else if((idUserTarget == m_playUser[0])  || (idUserTarget == m_playUser[1]))
	{
		pUserTarget1 = UserManager()->GetUser(idUserTarget);
		if(!pUserTarget1)
			return false;
	}
	
	if(m_playEudemon[index])
	{
		EudemonData data;
		pUser->LoadEudemon(m_playEudemon[index],data,2);
		CMsgEudemon msg;
		if(msg.Create(PETACTION_PetPlay,&data,idUser))
		{
			if(pUserTarget1)
				pUserTarget1->SendMsg(&msg);
			if(pUserTarget2)
				pUserTarget2->SendMsg(&msg);
		}
	}
	else
	{
		CMsgEudemon msg;
		if(msg.Create(PETACTION_PlayStop,NULL,idUser))
		{
			if(pUserTarget1)
				pUserTarget1->SendMsg(&msg);
			if(pUserTarget2)
				pUserTarget2->SendMsg(&msg);
		}
	}
	return true;
}

bool CTeam::EudemonPlayChange(OBJID idUser,bool idLock)
{
	if(!m_bEudemonPlay)
		return false;
	
	if(!idLock)
		return false;
	
	if(m_setIdUser.size() != 2)
	{
		return false;
	}
	
	if(m_playUser[0] == idUser)
	{
		if(m_bEudemonPlayLock[0])
		{
			return false;
		}
		else
		{
			m_bEudemonPlayLock[0] = true;
			CUser *pUser1 = UserManager()->GetUser(m_playUser[0]);
			CUser *pUser2 = UserManager()->GetUser(m_playUser[1]);
			if(pUser1 && pUser2)
			{
				CMsgEudemon msg;
				if(msg.Create(PETACTION_LockPlayPet,NULL,idUser))
				{
					pUser1->SendMsg(&msg);
					pUser2->SendMsg(&msg);
				}
			}
		}
	}
	else if(m_playUser[1] == idUser)
	{
		if(m_bEudemonPlayLock[1])
		{
			return false;
		}
		else
		{
			m_bEudemonPlayLock[1] = true;
			CUser *pUser1 = UserManager()->GetUser(m_playUser[0]);
			CUser *pUser2 = UserManager()->GetUser(m_playUser[1]);
			if(pUser1 && pUser2)
			{
				CMsgEudemon msg;
				if(msg.Create(PETACTION_LockPlayPet,NULL,idUser))
				{
					pUser1->SendMsg(&msg);
					pUser2->SendMsg(&msg);
				}
			}
		}
	}
	return false;
}

bool CTeam::EudemonPlayCancel()
{
	if(!m_bEudemonPlay)
		return false;
	
	
	CMsgEudemon msg;
	if(!msg.Create(PETACTION_PlayStop,NULL))
		return false;
	
	CUser *pUser1 = UserManager()->GetUser(m_playUser[0]);
	if(pUser1)
	{
		if(m_playEudemon[0] != ID_NONE)
			pUser1->UnLockEudemon(m_playEudemon[0]);
		pUser1->SendMsg(&msg);
	}
	CUser *pUser2 = UserManager()->GetUser(m_playUser[1]);
	{
		if(m_playEudemon[1] != ID_NONE)
			pUser2->UnLockEudemon(m_playEudemon[1]);
		pUser2->SendMsg(&msg);
	}
	
	m_bEudemonPlay= false;
	return true;
}

bool CTeam::EudemonPlay()
{
	if(!m_bEudemonPlay)
		return false;
	
	CUser *pUser1 = UserManager()->GetUser(m_playUser[0]);
	CUser *pUser2 = UserManager()->GetUser(m_playUser[1]);
	if(!pUser1 || !pUser2)
		return false;
	
	if(!m_bEudemonPlayLock[0])
		return false;
	
	if(!m_bEudemonPlayLock[1])
		return false;
	
	pUser1->UnLockEudemon(m_playEudemon[0]);
	pUser2->UnLockEudemon(m_playEudemon[1]);
	
	if(!pUser1->QueryUserPetSet())
		return false;

	if(pUser1->QueryUserPetSet()->EudemonPlay(m_playEudemon[0],m_playEudemon[1],pUser2))
	{
		CUser *pUser1 = UserManager()->GetUser(m_playUser[0]);
		CUser *pUser2 = UserManager()->GetUser(m_playUser[1]);
		if(pUser1 && pUser2)
		{
			CMsgEudemon msg;
			if(msg.Create(PETACTION_PlayBegin,NULL,0))
			{
				pUser1->SendMsg(&msg);
				pUser2->SendMsg(&msg);
			}
		}
	}
	else
	{
		EudemonPlayCancel();
	}
	m_bEudemonPlay = false;
	return true;
}

bool CTeam::CheckIsSameSyn(OBJID idsyn)
{
	DEBUG_TRY
	
	CUser* pMember	= NULL;
	for(int i = 0 ; i <_MAX_TEAMAMOUNT ; i ++)
	{
		if (m_TeamIn[i].id==ID_NONE)
			continue;
		pMember = UserManager()->GetUser(m_TeamIn[i].id);
		if (!pMember)
			return false;
		if(!pMember->CanBeSee())
			return false;
		if (pMember->GetSynID()!=idsyn)
			return false;

	}
	
	DEBUG_CATCH("CTeam::CheckIsSameSyn crash!")
		return true;
}
