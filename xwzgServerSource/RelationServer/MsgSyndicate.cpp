// MsgBetOutside.cpp: implementation of the CMsgAtk class.
// �ɽ��ޣ�2002.1.8
//////////////////////////////////////////////////////////////////////
//#include "AllMsg.h"
//#include "basefunc.h"
//#include "MapGroup.h"
//#include "SynManager.h"
#include "MsgSyndicate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSyndicate::CMsgSyndicate()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgSyndicate::~CMsgSyndicate()
{
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgSyndicate::Create(unsigned short usAction, OBJID idTarget, OBJID idFealty)
{
	// check param
	if(usAction == ACTION_NONE) //  || idTarget == ID_NONE
	{
//		ASSERT(!"CMsgSyndicate::Create()");
		return false;
	}

	// init
	this->Init();

	m_pInfo->usAction	=usAction;
	m_pInfo->idTarget	=idTarget;
//	m_pInfo->idFealty	=idFealty;
	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_SYNDICATE;


	return true;
}
BOOL CMsgSyndicate::Create(unsigned short usAction, char* pMsgBuf)
{
	this->Init();
	m_StrPacker.AddString(pMsgBuf);
	m_pInfo->usAction = usAction;
	
	m_unMsgType = _MSG_SYNDICATE;
	m_unMsgSize = sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return true;
}
//////////////////////////////////////////////////////////////////////
BOOL	CMsgSyndicate::Create		(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_SYNDICATE != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
// void	CMsgSyndicate::Process		(CGameSocket* pSocket)
// {
// 	try {
// 	// �����Ϣ��ʽ
// 	//����������ͼ���û�в���
// 	if(SYN_WAR_SET == m_pInfo->usAction)//��һ��ͼ��㲥
// 	{
// 		if(PID == MSGPORT_MAPGROUP_FIRST)
// 			return;
// 		OBJID thisID = 0,targetID = 0;
// 		int state = 0;
// 
// 		targetID = m_pInfo->idTarget;
// 		char szName[_MAX_NAMESIZE] = "";
// 		if (m_StrPacker.GetStrAmount() > 0)
// 		m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
// 		if(sscanf(szName,"%d %d",&thisID,&state) == 2)
// 		{
// 			CSynPtr pSyn = SynManager()->QuerySyndicate(thisID);
// 			if(!pSyn)
// 				return ;
// 			CSynPtr pSynTarget = SynManager()->QuerySyndicate(targetID);
// 			if(!pSynTarget)
// 				return ;
// 
// 			pSyn->SetSynWar(targetID,false);
// 			pSynTarget->SetSynWar(thisID,false);
// 
// 			pSyn->SetState(state);
// 			pSynTarget->SetState(state);
// 
// 			pSyn->SynchroInfo();
// 			pSynTarget->SynchroInfo();
// 			
// 		}		
// 
// 		if(PID < MSGPORT_MAPGROUP_INSTANCE - 1)
// 			CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(PID + 1,this);
// 		return;
// 	}
// 	else if(SYN_WAR_SEND == m_pInfo->usAction)//��һ��ͼ�����
// 	{
// 		if(PID != MSGPORT_MAPGROUP_FIRST)
// 			return;
// 
// 		OBJID thisID = 0,targetID = 0;
// 		int state = 0;
// 		
// 		targetID = m_pInfo->idTarget;
// 		char szName[_MAX_NAMESIZE] = "";
// 		if (m_StrPacker.GetStrAmount() > 0)
// 			m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
// 		if(sscanf(szName,"%d %d",&thisID,&state) == 2)
// 		{
// 			CSynPtr pSyn = SynManager()->QuerySyndicate(thisID);
// 			if(!pSyn)
// 				return ;
// 			CSynPtr pSynTarget = SynManager()->QuerySyndicate(targetID);
// 			if(!pSynTarget)
// 					return ;
// 
// 			if(0 == state)//��ʼ
// 			{
// 				pSyn->SetSynWar(targetID);
// 				pSynTarget->SetSynWar(thisID);
// 				MSGBUF szMsg;
// 				sprintf(szMsg, "%s����%s��ῪʼԼս!", pSyn->GetStr(SYNDATA_NAME), pSynTarget->GetStr(SYNDATA_NAME));
// 				UserManager()->BroadcastMsg(szMsg, NULL, NULL, 0xffffff, _TXTATR_SYSTEM);
// 			}
// 			else if(1 == state)//ɱ��
// 			{
// 				int synKill = pSynTarget->GetKill();
// 				pSynTarget->SetKill(synKill + 1);
// 
// 			}
// 			
// 			
// 		}
// 		return;
// 	}
	// �������
// 	ASSERT(pSocket);
// 	if(!pSocket)
// 		return;
// 
// 	// ״̬���
// 	CUserPtr pUser = UserManager()->GetUser(this);
// 	IF_NOT(pUser)
// 		return;
// 
// 	OBJID idUser = pUser->GetID();
// 	IF_NOT(idUser != ID_NONE)
// 		return;
// 
// 	if(!pUser->IsAlive())
// 	{
// 		pUser->SendSysMsg("���Ѿ�����");//STR_DIE
// 		return ;
// 	}
// 
// 	char szName[_MAX_NAMESIZE] = "";
// 	char szWord[256] = "";
// 	if (m_StrPacker.GetStrAmount() > 0)
// 		m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
// 	if (m_StrPacker.GetStrAmount() > 1)
// 		m_StrPacker.GetString(1, szWord, 256);
// 
// 	switch(m_pInfo->usAction)
// 	{
// 	case SYN_CREATE:
// 		{
// 			CItem* pItem = pUser->GetItem(m_pInfo->idTarget,false);
// 			if(!pItem || pItem->GetTypeID() != 630007)
// 				return;
// 
// 			if(pUser->GetMoney() < 1000000)
// 				return;
// 		
// 			CSyndicate* pSynOld = pUser->GetSyndicate();
// 			if(pSynOld )
// 			{
// 				if(pSynOld->GetInt(SYNDATA_STATE) == 1)
// 				{
// 					if(pUser->QuerySynAttr()->GetSynRank() == 100)
// 					{
// 						pUser->SendSysMsg("���İ����ڴ����У����ܽ�����İ���");
// 						return;
// 					}
// 					else
// 					{
// 						if(!pUser->QuerySynAttr()->LeaveSyn())
// 						{
// 							pUser->SendSysMsg("���İ����ڴ����У����ܽ�����İ���");
// 							return;
// 						}
// 					}
// 				}
// 				else
// 				{
// 					pUser->SendSysMsg("���Ѿ��ڰ����У��˳�����ܼ�����������");
// 					return;
// 				}
// 			}
// 
// 			OBJID idSyn =8888;/// pUser->QuerySynAttr()->CreateSyndicate(szName, szWord);
// 			if(idSyn != ID_NONE)
// 			{
// 				pUser->SpendItem(pItem);
// 				pUser->SpendMoney(1000000,true);
// 				
// 				this->Create(SYN_CREATE_OK,idSyn);
// 				pUser->SendMsg(this);
// 
// 				///���ɴ����ɹ�����ͨ����ϵ�������㲥��������ȥ��������Ҳ�����������,�Ա������ߵ���ҿ�����Ӧ�������
// 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this, ALLUSERS_NAME);
// 
// 			}
// 			else
// 			{
// 				this->Create(SYN_CREATE_ERROR,pUser->GetID());
// 				pUser->SendMsg(this);
// 			}
// 		}
// 		break;
// 	case SYN_QUERY_CREATE:
// 		{
// 			SynManager()->SendCreateSynToUser(pUser,m_pInfo->dwData);
// 		}
// 		break;
// 	case SYN_JOIN:
// 		{
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			if(!pSyn)
// 				return;
// 			if(pSyn->GetInt(SYNDATA_STATE) == 1) //��Ӧ������Ҫ�������� �Զ����ϴ���Ӧ
// 			{
// 				CSyndicate* pSynOld = pUser->GetSyndicate();
// 				if(pSynOld && (pSynOld->GetInt(SYNDATA_STATE) == 1))
// 				{
// 					if(pUser->QuerySynAttr()->GetSynRank() == 100)
// 					{
// 						pUser->SendSysMsg("���İ����ڴ����У�������Ӧ���˵İ���");
// 					}
// 					else
// 					{
// 						if(!pUser->QuerySynAttr()->LeaveSyn())
// 							pUser->SendSysMsg("���İ����ڴ����У�������Ӧ���˵İ���");
// 					}
// 
// 				}
// 				pUser->QuerySynAttr()->JoinSyn(m_pInfo->idTarget);
// 				pUser->SendSysMsg("��Ӧ�ɹ�");
// 			}
// 			else
// 			{
// 				pUser->SendSysMsg("��Ӧ�����Ѿ������ɹ�");
// 			}
// 
// 			CMsgSyndicateList msg;
// 			if (msg.Create(SYNLISTACTION_UPDATA,pSyn->GetID(),pSyn->GetStr(SYNDATA_NAME),pSyn->GetStr(SYNDATA_LEADERNAME),pSyn->GetInt(SYNDATA_LEVEL),pSyn->GetInt(SYNDATA_AMOUNT)))
// 				pUser->SendMsg(&msg);
// 		}
// 		break;
// 	case SYN_QUERY_INFO:
// 		{
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			if(m_pInfo->idTarget == ID_NONE)
// 				pSyn = pUser->GetSyndicate();
// 			else
// 				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 
// 			if(!pSyn)
// 				return;
// 			pSyn->SendInfoToClient(pUser);
// 		}
// 		break;
// 
// 	case SYN_QUERY_MEMBER:
// 		{
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			if(m_pInfo->idTarget == ID_NONE)
// 				pSyn = pUser->GetSyndicate();
// 			else
// 				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			
// 			if(!pSyn)
// 				return;
// 
// 			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
// 				return;
// 			
// 			pSyn->SendMenberToClient(pUser,m_pInfo->dwData);
// 		}
// 		break;
// 	case SYN_QUERY_LIST:
// 		{
// 			SynManager()->SendSynList(pUser,m_pInfo->dwData);
// 		}
// 		break;
// 	case SYN_JOIN_QUERY:
// 		{
// 
// 			CSyndicate* pSynOld = pUser->GetSyndicate();
// 			if(pSynOld )
// 			{
// 				if(pSynOld->GetInt(SYNDATA_STATE) == 1)
// 				{
// 					if(pUser->QuerySynAttr()->GetSynRank() == 100)
// 					{
// 						pUser->SendSysMsg("���İ����ڴ����У����ܼ����İ���");
// 						return;
// 					}
// 					else
// 					{
// 						if(!pUser->QuerySynAttr()->LeaveSyn())
// 						{
// 							pUser->SendSysMsg("���İ����ڴ����У����ܼ����İ���");
// 							return;
// 						}
// 					}
// 				}
// 				else
// 				{
// 					pUser->SendSysMsg("���Ѿ��ڰ����У��˳�����ܼ�����������");
// 					return;
// 				}
// 			}
// 
// 
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			if(m_pInfo->idTarget == ID_NONE)
// 				pSyn = pUser->GetSyndicate();
// 			else
// 				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			
// 			if(!pSyn)
// 				return;
// 			
// 			if(pSyn->GetInt(SYNDATA_STATE) != 2) 
// 				return;
// 
// 			int levMemberAmount[] = {50,50,100,150,250};
// 			if(pSyn->GetMemberAmount() >= levMemberAmount[pSyn->GetInt(SYNDATA_LEVEL)])
// 			{
// 				pUser->SendSysMsg("�����������");
// 				return;
// 			}
// 			
// 			CSyndicate::MemberInfo info;
// 			info.nId = pUser->GetID();
// 			info.nLevel = pUser->GetLev();
// 			info.firJob = pUser->GetProfession(1);
// 			info.firLevel = pUser->GetJobLev(1);
// 			info.nRank = 10;
// 			info.nProffer = 0;
// 			SafeCopy(info.szName,pUser->GetName());
// 
// 			if(pSyn->AddMemberList(info,2) == RANK_NORMAL)
// 			{
// 				pUser->SendSysMsg("���뷢�ͳɹ�");
// 			}
// 			else
// 			{
// 				pUser->SendSysMsg("���Ѿ��ڶԷ��������б��У��벻Ҫ�ظ�����");
// 			}
// 		}
// 		break;
// 	case SYN_QUERY_QUERYMEMBER:
// 		{
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			if(m_pInfo->idTarget == ID_NONE)
// 				pSyn = pUser->GetSyndicate();
// 			else
// 				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			
// 			if(!pSyn)
// 				return;
// 			
// 			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
// 				return;
// 			
// 			pSyn->SendQueryMenberToClient(pUser,m_pInfo->dwData);
// 		}
// 		break;
// 	case SYN_ANSERR_OK:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
// 				return;
// 
// 			int nRank = pUser->QuerySynRank();
// 			if(nRank < RANK_FEELLEADER1)
// 			{
// 				pUser->SendSysMsg("���Ȩ�޲���");
// 				return;
// 			}
// 
// 			int levMemberAmount[] = {50,50,100,150,250};
// 			if(pSyn->GetMemberAmount() >= levMemberAmount[pSyn->GetInt(SYNDATA_LEVEL)])
// 			{
// 				pUser->SendSysMsg("�����������");
// 				return;
// 			}
// 
// 			if(!pSyn->AllowMenber(m_pInfo->idTarget,pUser))
// 			{
// 				pUser->SendSysMsg("�Է��Ѿ������˰���");
// 				return;
// 			}
// 
// 			CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
// 			if(pThisUser)
// 			{
// 				pThisUser->SendSysMsg("����%sͨ��",pSyn->GetStr(SYNDATA_NAME));
// 
// 				this->Create(SYN_UPDATE,pThisUser->GetID());
// 				pThisUser->SendMsg(this);
// 				pUser->SendMsg(this);
// 			}
// 			else
// 			{
// 				this->Create(SYN_UPDATE,pUser->GetID());
// 				pUser->SendMsg(this);
// 			}
// 		}
// 		break;
// 	case SYN_ANSERR_NO:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
// 				return;
// 
// 			int nRank = pUser->QuerySynRank();
// 			if(nRank < RANK_FEELLEADER1)
// 			{
// 				pUser->SendSysMsg("���Ȩ�޲���");
// 				return;
// 			}
// 
// 			pSyn->DelMemberList(m_pInfo->idTarget,2);
// 			CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
// 			if(pThisUser)
// 			{
// 				pThisUser->SendSysMsg("��%s�ܾ�",pSyn->GetStr(SYNDATA_NAME));
// 			}
// 
// 			this->Create(SYN_UPDATE,pUser->GetID());
// 			pUser->SendMsg(this);
// 		}
// 		break;
// 	case SYN_KICKOUT:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 
// 			int nMyRank = pUser->QuerySynRank();
// 
// 			if(nMyRank < RANK_FEELLEADER1)
// 			{
// 				pUser->SendSysMsg("���Ȩ�޲���");
// 				return;
// 			}
// 
// 			CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
// 			if(pThisUser)
// 			{
// 				if((pThisUser->GetSyndicate()->GetID() != pSyn->GetID()) || (nMyRank / 10  <= pThisUser->QuerySynRank() / 10))
// 				{
// 					pUser->SendSysMsg("���Ȩ�޲���");
// 					return;
// 				}
// 
// 				if(nMyRank / 10 == 8)
// 				{
// 					if( pThisUser->QuerySynRank() % 10 != nMyRank % 10 )
// 					{
// 						pUser->SendSysMsg("�㲻���߳������õĳ�Ա");
// 						return;
// 					}
// 				}
// 
// 				pThisUser->QuerySynAttr()->LeaveSyn(true);
// 				pThisUser->SendSysMsg("�㱻�߳�����");
// 
// 				this->Create(SYN_UPDATE,pThisUser->GetID());
// 				pThisUser->SendMsg(this);
// 				pUser->SendMsg(this);
// 			}
// 			else
// 			{
// 				int nThisRank = pUser->QuerySynAttr()->GetRankByID(m_pInfo->idTarget,pSyn->GetID());
// 				if(nThisRank == RANK_NONE)
// 				{
// 					pUser->SendSysMsg("�Է��Ѿ��뿪�˰���");
// 					return;
// 				}
// 
// 				if(nMyRank / 10  <= nThisRank / 10)
// 				{
// 					pUser->SendSysMsg("���Ȩ�޲���");
// 					return;
// 				}
// 
// 				if(nMyRank / 10 == 8)
// 				{
// 					if(nThisRank % 10 != nMyRank % 10 )
// 					{
// 						pUser->SendSysMsg("�㲻���߳������õĳ�Ա");
// 						return;
// 					}
// 				}
// 
// 				pUser->QuerySynAttr()->KickoutMember(m_pInfo->idTarget);
// 
// 				this->Create(SYN_UPDATE,pUser->GetID());
// 				pUser->SendMsg(this);
// 			}
// 			pUser->SendSysMsg("�߳��ɹ�");
// 		}
// 		break;
// 	case SYN_LEAVER:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			int nMyRank = pUser->QuerySynRank();
// 			if(nMyRank == RANK_LEADER)
// 				return;
// 
// 			pUser->QuerySynAttr()->LeaveSyn();
// 			pUser->SendSysMsg("�뿪�ɹ�");
// 
// 			this->Create(SYN_UPDATE,pUser->GetID());
// 			pUser->SendMsg(this);
// 		}
// 		break;
// 	case SYN_SETRANK:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			int nMyRank = pUser->QuerySynRank();
// 			if(nMyRank < RANK_FEELLEADER1)
// 			{
// 				pUser->SendSysMsg("���Ȩ�޲���");
// 				return;
// 			}
// 
// 			int nThisRank = pUser->QuerySynAttr()->GetRankByID(m_pInfo->idTarget,pSyn->GetID());
// 			if(nThisRank == RANK_NONE)
// 			{
// 				pUser->SendSysMsg("�Է��Ѿ��뿪�˰���");
// 				return;
// 			}
// 
// 			if(nMyRank / 10  <= nThisRank / 10)
// 			{
// 				pUser->SendSysMsg("���Ȩ�޲���");
// 				return;
// 			}
// 
// 			if(nMyRank / 10 == 8)
// 			{
// 				if(m_pInfo->dwData % 10 != nMyRank % 10 )
// 				{
// 					pUser->SendSysMsg("�㲻�����������õ�ְλ");
// 					return;
// 				}
// 			}
// 
// 			if(m_pInfo->dwData < RANK_NORMAL || m_pInfo->dwData > RANK_SUBLEADER)
// 			{
// 				pUser->SendSysMsg("ְλ����");
// 				return;
// 			}
// 
// 			if(pUser->QuerySynAttr()->SetIdRank(m_pInfo->idTarget,pSyn->GetID(),m_pInfo->dwData))
// 			{
// 				this->Create(SYN_UPDATE,m_pInfo->idTarget);
// 				pUser->SendMsg(this);
// 				pUser->SendSysMsg("�����ɹ�");
// 			}
// 		}
// 		break;
// 
// 	case SYN_DEMISE:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			int nMyRank = pUser->QuerySynRank();
// 			if(nMyRank != RANK_LEADER)
// 			{
// 				pUser->SendSysMsg("���Ȩ�޲���");
// 				return;
// 			}
// 
// 			int nThisRank = pUser->QuerySynAttr()->GetRankByID(m_pInfo->idTarget,pSyn->GetID());
// 			if(nThisRank == RANK_NONE)
// 			{
// 				pUser->SendSysMsg("�Է��Ѿ��뿪�˰���");
// 				return;
// 			}
// 
// 			if(pUser->QuerySynAttr()->SetIdRank(m_pInfo->idTarget,pSyn->GetID(),RANK_LEADER))
// 				pUser->SendSysMsg("��λ�ɹ�");
// 
// 		}
// 		break;
// 	case SYN_ANNONCE:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			int nMyRank = pUser->QuerySynRank();
// 			if(nMyRank != RANK_LEADER && nMyRank != RANK_SUBLEADER)
// 			{
// 				pUser->SendSysMsg("���Ȩ�޲���");
// 				return;
// 			}
// 			
// 			pSyn->SetStr(SYNDATA_ANNOUNCE,szWord,256,true);
// 
// 
// 			CMsgTalk	msg;
// 			IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szWord, NULL, 0xFFFFFF, _TXTATR_SYNANNOUNCE))
// 				pSyn->BroadcastSynMsg(&msg);
// 
// 			pUser->SendSysMsg("�޸ĳɹ�");
// 		}
// 		break;
// 		
// // 	case SYN_DEMISE:
// // 		{
// // 			//  [1/15/2008 ���]	�������¿���
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			// ������
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			
// // 
// // 			//[��;���� 2008/9/27]�Է������Ǹ�����
// // 			// ���Ȩ��
// // 			
// // 			if(pUser->GetSynRank() != RANK_LEADER
// // 				|| pUser->QuerySynAttr()->GetPlayRankByName(szName) != 90 
// // 				|| pUser->QuerySynAttr()->GetSynIDByOffLinePlay(szName) != idSyn)
// // 			{
// // 				return;
// // 			}
// // 
// // 			// target syn
// // 			CUser* pTarget = UserManager()->GetUser(szName);
// // 
// // 			//[��;���� 2008/9/27]���ήְ���Ƿ�����
// // 			bool bOnline = true;
// // 			bool bWifeOnline = true;
// // 			bool bWife = true;
// // 			bool bSubWifeOnline = false;
// // 			bool bSubWife = false;
// // 			CUser* pwife = NULL;
// // 			CUser* pSubwife = NULL;
// // 
// // 			OBJID subLeadID = pUser->QuerySynAttr()->GetUserIDByName(szName); //������ID
// // 			OBJID LeadWifeID = pSyn->GetLeaderWifeID();                       //��������ID
// // 			OBJID subLeadWifeID = ID_NONE;
// // 			
// // 
// // 			if(!pTarget)
// // 			{
// // 				//[��;���� 2008/9/27]�޸Ĳ����ߵ�ֱ�Ӽ�¼��״̬����ֱ��д���ݿ�
// // 				bOnline = false;
// // 				//pUser->SendSysMsg(g_objStrRes.GetStr(12011));//STR_NOT_HERE
// // 				//return ;
// // 			}
// // 
// // 			//[��;���� 2008/9/27]���εȼ��жϺ�һЩ���õ��ж�
// // 			//int nLevel = 40;
// // 			//int nProffer = 50000;		
// // 			//if(pTarget->GetLev() < nLevel || pTarget->QuerySynProffer() < nProffer)
// // 			//{
// // 			//	char szBuff[128];
// // 			//	sprintf(szBuff,g_objStrRes.GetStr(12014),pTarget->GetName());//STR_LEVEL_NOT_ENOUGH
// // 			//	pUser->SendSysMsg(szBuff);
// // 			//	return ;
// // 			//}
// // 
// // 			//[��;���� 2008/9/27]�����ߺ����ߵķֱ�Դ�
// // 			OBJID	idTargetSyn;
// // 			if (bOnline)
// // 			{
// // 				idTargetSyn = pTarget->GetSynID();
// // 			}
// // 			else
// // 			{
// // 				idTargetSyn = pUser->QuerySynAttr()->GetSynIDByOffLinePlay(szName);
// // 			}
// // 
// // 			//[��;���� 2008/9/27]�����ߺ����ߵķֱ�Դ�
// // 			if (bOnline)
// // 			{
// // 				if((pTarget->GetID() == pUser->GetID()) || (pTarget->GetSynID() != pUser->GetSynID()))
// // 				{
// // 					return;
// // 				}
// // 				if( KICKOUTDATA_OUTDO_FALSE != pUser->QuerySynAttr()->GetLeaveSyn(pTarget->GetID()) )
// // 				{
// // 					pUser->SendSysMsg("����ʧ�ܣ��Է��Ѿ������˰");
// // 					return;
// // 				}
// // 			}
// // 			else
// // 			{
// // 				OBJID targetID = pUser->QuerySynAttr()->GetUserIDByName(szName);
// // 				if((targetID == pUser->GetID()) || (idTargetSyn != pUser->GetSynID()))
// // 				{
// // 					return;
// // 				}
// // 				if( KICKOUTDATA_OUTDO_FALSE != pUser->QuerySynAttr()->GetLeaveSyn(targetID) )
// // 				{
// // 					pUser->SendSysMsg("����ʧ�ܣ��Է��Ѿ������˰");
// // 					return;
// // 				}
// // 			}
// // 
// // 			if(idTargetSyn == ID_NONE || idTargetSyn != idSyn)
// // 				return ;
// // 
// // 			if (bOnline)
// // 			{
// // 				pSyn->QueryModify()->Demise(pUser->GetID(),pUser->QuerySynAttr()->GetSynMemberLevel(), pTarget->GetID(), pTarget->GetName(), pTarget->QuerySynAttr()->GetSynMemberLevel(),pSyn->GetID());
// // 				pTarget->QuerySynAttr()->SetRank(100, true);
// // 				pUser->QuerySynAttr()->SetRank(10, true);
// // 				pTarget->QuerySynAttr()->SynchroInfo();
// // 				pUser->QuerySynAttr()->SynchroInfo();
// // 			}
// // 			else
// // 			{
// // 				pUser->QuerySynAttr()->Demise(pUser->GetID(),(char *)pUser->GetName(), pUser->QuerySynAttr()->GetUserIDByName(szName), (char *)szName);
// // 				pSyn->QueryModify()->Demise(0,0, pUser->QuerySynAttr()->GetUserIDByName(szName), (char *)szName, 0, pSyn->GetID());
// // 			}
// // 
// // // 			pSyn->SetSubLeaderID(0);
// // 			pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, 0, true);
// // //---------------------------------------------------------
// // 			if(LeadWifeID == ID_NONE)
// // 				bWife = false;
// // 			if(bWife)
// // 			{
// // 				pSyn->QueryModify()->SetData(SYNDATA_LEADERWIFE, 0, true);
// // 				pwife = UserManager()->GetUser(LeadWifeID);
// // 				if(!pwife)
// // 				{
// // 					bWifeOnline = false;
// // 				}
// // 			}
// // 			char temp[16] = {0};
// // 			
// // 			pUser->QuerySynAttr()->GetMateIDByID(subLeadWifeID, subLeadID);
// // //			pUser->QuerySynAttr()->GetMateNameByID(temp , subLeadID);
// // //			if(strcmp("��",temp) != 0)
// // //				subLeadWifeID = pUser->QuerySynAttr()->GetUserIDByName(temp); //����������ID
// // 			if(subLeadWifeID != ID_NONE && pUser->QuerySynAttr()->GetSynIDByUserID(subLeadWifeID) == pSyn->GetID())
// // 				bSubWife = true;
// // 			if(bSubWife)
// // 			{
// // 				pSubwife = UserManager()->GetUser(subLeadWifeID);
// // 				if(pSubwife)
// // 				{
// // 					bSubWifeOnline = true;
// // 				}
// // 			}
// // 			//----------------------------------------
// // 			if(bSubWife && bSubWifeOnline)
// // 			{			
// // 				pSubwife->QuerySynAttr()->SetRank(80, true);				
// // 				pSubwife->QuerySynAttr()->SynchroInfo();
// // 			}
// // 			else if(bSubWife)
// // 			{				
// // 			// 	pUser->QuerySynAttr()->SetRankByName(80,temp);
// // 				pUser->QuerySynAttr()->SetRankByID(80,subLeadWifeID);
// // 			}
// // 			if(bSubWife)
// // 			{
// // // 				pSyn->SetLeaderWifeID(subLeadWifeID);
// // 				pSyn->QueryModify()->SetData(SYNDATA_LEADERWIFE, subLeadWifeID, true);
// // 				pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), "�°�����ż����");
// // 			}
// // 
// // 			if(bWife && bWifeOnline)
// // 			{
// // 				pwife->QuerySynAttr()->SetRank(10, true);				
// // 				pwife->QuerySynAttr()->SynchroInfo();
// // 			}
// // 			else if(bWife)
// // 			{
// // //				pUser->QuerySynAttr()->GetMateNameByID(temp , pUser->GetID());
// // //				pUser->QuerySynAttr()->SetRankByName(10,temp);
// // 
// // 				pUser->QuerySynAttr()->SetRankByID(10, LeadWifeID);
// // 			}
// // 			
// // 
// // 			
// // 			char szBuff[128];
// // 			sprintf(szBuff,g_objStrRes.GetStr(10043),szName);//STR_SYN_DEMISE_SUCC
// // 			pUser->SendSysMsg(szBuff);
// // 			if (bOnline)
// // 			{
// // 				sprintf(szBuff,g_objStrRes.GetStr(10044),pSyn->GetStr(SYNDATA_NAME));//STR_SYN_DEMISE_CON
// // 				pTarget->SendSysMsg(szBuff);
// // 				sprintf(szBuff,g_objStrRes.GetStr(10045),pTarget->GetName(),pSyn->GetStr(SYNDATA_NAME));//STR_SYN_DEMISE_PROMPT
// // 				pTarget->SendSysMsg(_TXTATR_SYNDICATE,szBuff);
// // 			}	
// // 		}
// // 		break;
// // 	case SYN_SET_ASSISTANT://[��;���� 2008/9/28]�԰��ڽ�����ְ
// // 		{
// // 						// ������
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			// ���Ȩ��
// // 			int nRank = pUser->GetSynRank();
// // 
// // 			//[��;���� 2008/9/27]���ΰ��ж�
// // 			//if(nRank != RANK_LEADER)
// // 			//	return ;
// // 
// // 			//[��;���� 2008/9/27]���δ˴��ж�,����ͳһ�ж�.
// // 			// check amount, temporary code.
// // 			//if(pUser->QuerySynAttr()->GetAssistantCount() >= MAX_ASSISTANTSIZE)
// // 			//{
// // 			//	pUser->SendSysMsg(g_objStrRes.GetStr(11099));//STR_SYN_PLACE_FULL
// // 			//	return ;
// // 			//}
// // 
// // 			// target syn
// // 			CUser* pTarget = UserManager()->GetUser(szName);
// // 			//[��;���� 2008/9/27]����ְ���Ƿ�����
// // 			bool bOnline = true;
// // 			if(!pTarget )/*|| (!pTarget->GetSyndicate()->IsSubSyn())*/
// // 			{
// // 				//[��;���� 2008/9/27]�޸Ĳ����ߵ�ֱ�Ӽ�¼��״̬����ֱ��д���ݿ�
// // 				bOnline = false;
// // 				//pUser->SendSysMsg(g_objStrRes.GetStr(12011));//STR_NOT_HERE
// // 				//return ;
// // 			}
// // 			//20070428����:�߻�����,ȥ���ж�
// // //			int nLevel = 35;
// // //			int nProffer = 30000;		
// // //			int rand = 50;
// // //			if(pTarget->GetLev() < nLevel || pTarget->QuerySynProffer() < nProffer || pTarget->GetSynRank()<50)
// // //			{
// // //				char szBuff[128];
// // //				sprintf(szBuff,STR_LEVEL_NOT_ENOUGH,pTarget->GetName());
// // //				pUser->SendSysMsg(szBuff);
// // //				pTarget->SendSysMsg(STR_SYN_SETSUBLEADER_ERR);
// // //				return ;
// // //			}
// // 
// // 			//[��;���� 2008/9/27]�����ߺ����ߵķֱ�Դ�
// // 			OBJID	idTargetSyn;
// // 			if (bOnline)
// // 			{
// // 				idTargetSyn = pTarget->GetSynID();
// // 			}
// // 			else
// // 			{
// // 				idTargetSyn = pUser->QuerySynAttr()->GetSynIDByOffLinePlay(szName);
// // 			}
// // 			if(idTargetSyn == ID_NONE || idTargetSyn != idSyn)
// // 				return ;
// // 			CSynPtr pSubSyn = SynManager()->QuerySyndicate(idTargetSyn);
// // 			CHECK(pSubSyn);
// // 			
// // 			int nTargetRank = pUser->QuerySynAttr()->GetPlayRankByName(szName);
// // 			OBJID targetID = pUser->QuerySynAttr()->GetUserIDByName(szName);
// // 
// // 			if( KICKOUTDATA_OUTDO_FALSE != pUser->QuerySynAttr()->GetLeaveSyn(targetID) )
// // 			{
// // 				pUser->SendSysMsg("����ʧ�ܣ��Է��Ѿ������˰");
// // 				return;
// // 			}
// // 	
// // 
// // 			int nNewRank = 10;
// // 
// // 			switch (nTargetRank)
// // 			{
// // 			case 100:
// // 				return;
// // 			case 90:
// // 				return;
// // 			case 80:
// // 				return;
// // 			case 60:
// // 				if(/*pUser->QuerySynAttr()->GetRankCount(90)*/pSyn->GetSubLeaderID() != ID_NONE || (100 != nRank))
// // 				{
// // 					return;
// // 				}
// // 				nNewRank = 90;
// // 				break;
// // 			//[��;���� 2008/9/27]��ʱ����Ҫ����ȼ��Ļ�Ա
// // 			//case 30:
// // 			default:
// // 				if (((pSyn->GetInt(SYNDATA_RANK) * 2 + 2) <= pUser->QuerySynAttr()->GetRankCount(60))
// // 					&& (90 > nRank))
// // 				{
// // 					return;
// // 				}
// // 				nNewRank = 60;
// // 				break;
// // 			/*default:
// // 				if (80 > nRank)
// // 				{
// // 					return;
// // 				}
// // 				nNewRank = nTargetRank + 10;
// // 				break;*/
// // 			}
// // 
// // 			//[��;���� 2008/9/27]�����ߺ����ߵķֱ�Դ�
// // 			if(nNewRank == 90)
// // 			{
// // 			// 	pSyn->SetSubLeaderID(targetID);
// // 				pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, targetID, true);
// // 			}
// // 			if (bOnline)
// // 			{
// // 				pTarget->QuerySynAttr()->SetRank(nNewRank, true);				
// // 				pUser->QuerySynAttr()->SynchroInfo();
// // 			}
// // 			else
// // 			{
// // 				pUser->QuerySynAttr()->SetRankByName(nNewRank, szName);
// // 			}
// // 	/*		CMsgSyndicate msg;
// // 			if(msg.Create(SYN_SET_BUS,(char*)pTarget->GetName()))
// // 			{
// // 				msg.m_pInfo->dwData	=	90;
// // 				pSubSyn->BroadcastSynMsg(&msg, pUser);
// // 			}*/
// // 
// // 			//[��;���� 2008/9/27]���β���Ҫ����,���¸�������������
// // 			/*SQLBUF szSQL = "";
// // 			sprintf(szSQL,"SELECT s.id,u.name from %s AS s LEFT JOIN sk_user AS u ON u.id = s.id  where s.syn_id= %d and s.rank = 90",_TBL_SYNATTR,idSyn);
// // 
// // 			IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// // 			if(pRes)
// // 			{
// // 				CUser *pUser = UserManager()->GetUser(pRes->GetInt(0));
// // 				if(pUser)
// // 				{
// // 					pUser->QuerySynAttr()->SetRank(50);
// // 				}
// // 				else
// // 				{
// // 					MESSAGESTR	buf;
// // 					SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// // 					pInfo->idSyn		= idSyn;
// // 					pInfo->nRank		= 50;
// // 					pInfo->ucFuncType	= SYNFUNC_SET_RANK;
// // 					SafeCopy(pInfo->StrParam, pRes->GetStr(1), _MAX_NAMESIZE);
// // 					pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
// // 					MapGroup(PID)->QuerySynMessage()->ChangeSyndicate(pInfo);
// // 					sprintf(szSQL, "UPDATE %s SET rank = 50  WHERE id = %u LIMIT 1",
// // 								_TBL_SYNATTR,pRes->GetInt(0));
// // 					Database()->ExecuteSQL(szSQL);
// // 				}
// // 			}*/
// // 
// // 			char szBuff[128];
// // 			if(nNewRank == 90)
// // 			{
// // 				sprintf(szBuff,g_objStrRes.GetStr(10047),szName,pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_SUCC
// // 				pUser->SendSysMsg(szBuff);
// // 				//[��;���� 2008/9/27]�����ߺ����ߵķֱ�Դ�
// // 				if (bOnline)
// // 				{
// // 					sprintf(szBuff,g_objStrRes.GetStr(10048),pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_CON
// // 					pTarget->SendSysMsg(szBuff);
// // 				}
// // 				sprintf(szBuff,g_objStrRes.GetStr(10049),szName,pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_PROMPT
// // 					pUser->SendSysMsg(_TXTATR_SYNDICATE,szBuff);
// // 			}
// // 			else
// // 			{
// // 				sprintf(szBuff,"�������%sΪ%s����һְ",szName,pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_SUCC
// // 				pUser->SendSysMsg(szBuff);
// // 				//[��;���� 2008/9/27]�����ߺ����ߵķֱ�Դ�
// // 				if (bOnline)
// // 				{
// // 					sprintf(szBuff,"��ϲ����Ϊ��%s����",pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_CON
// // 					pTarget->SendSysMsg(szBuff);
// // 				}
// // 				sprintf(szBuff,"%s����%s����һְ",szName,pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_PROMPT
// // 					pUser->SendSysMsg(_TXTATR_SYNDICATE,szBuff);
// // 			}
// // 			
// // 			
// // 		}
// // 		break;
// // 	case SYN_SET_BUS://[��;���� 2008/9/28]�԰��ڽ��н�ְ
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			// ���Ȩ��
// // 			int nRank = pUser->GetSynRank();
// // 
// // 			//[��;���� 2008/9/27]���ΰ��ж�
// // 			//if(nRank != RANK_LEADER)
// // 			//	return ;
// // 
// // 			//[��;���� 2008/9/27]���δ˴��ж�,����ͳһ�ж�.
// // 			// check amount, temporary code.
// // 			//if(pUser->QuerySynAttr()->GetAssistantCount() >= MAX_ASSISTANTSIZE)
// // 			//{
// // 			//	pUser->SendSysMsg(g_objStrRes.GetStr(11099));//STR_SYN_PLACE_FULL
// // 			//	return ;
// // 			//}
// // 
// // 			// target syn
// // 			CUser* pTarget = UserManager()->GetUser(szName);
// // 			//[��;���� 2008/9/27]����ְ���Ƿ�����
// // 			bool bOnline = true;
// // 			if(!pTarget /*|| (!pTarget->GetSyndicate()->IsSubSyn())*/)
// // 			{
// // 				//[��;���� 2008/9/27]�޸Ĳ����ߵ�ֱ�Ӽ�¼��״̬����ֱ��д���ݿ�
// // 				bOnline = false;
// // 				//pUser->SendSysMsg(g_objStrRes.GetStr(12011));//STR_NOT_HERE
// // 				//return ;
// // 			}
// // 			//20070428����:�߻�����,ȥ���ж�
// // //			int nLevel = 35;
// // //			int nProffer = 30000;		
// // //			int rand = 50;
// // //			if(pTarget->GetLev() < nLevel || pTarget->QuerySynProffer() < nProffer || pTarget->GetSynRank()<50)
// // //			{
// // //				char szBuff[128];
// // //				sprintf(szBuff,STR_LEVEL_NOT_ENOUGH,pTarget->GetName());
// // //				pUser->SendSysMsg(szBuff);
// // //				pTarget->SendSysMsg(STR_SYN_SETSUBLEADER_ERR);
// // //				return ;
// // //			}
// // 
// // 			OBJID targetID = pUser->QuerySynAttr()->GetUserIDByName(szName);			
// // 			if( KICKOUTDATA_OUTDO_FALSE != pUser->QuerySynAttr()->GetLeaveSyn(targetID) )
// // 			{
// // 				pUser->SendSysMsg("����ʧ�ܣ��Է��Ѿ������˰");
// // 				return;
// // 			}
// // 
// // 			//[��;���� 2008/9/27]�����ߺ����ߵķֱ�Դ�
// // 			OBJID	idTargetSyn;
// // 			if (bOnline)
// // 			{
// // 				idTargetSyn = pTarget->GetSynID();
// // 			}
// // 			else
// // 			{
// // 				idTargetSyn = pUser->QuerySynAttr()->GetSynIDByOffLinePlay(szName);
// // 			}
// // 			if(idTargetSyn == ID_NONE || idTargetSyn != idSyn)
// // 				return ;
// // 			CSynPtr pSubSyn = SynManager()->QuerySyndicate(idTargetSyn);
// // 			CHECK(pSubSyn);
// // 			
// // 			int nTargetRank = pUser->QuerySynAttr()->GetPlayRankByName(szName);
// // 
// // 			int nNewRank = 10;
// // 
// // 			switch (nTargetRank)
// // 			{
// // 			case 100:
// // 				return;
// // 			case 80:
// // 				return;
// // 			case 90:
// // 				if (((pSyn->GetInt(SYNDATA_RANK) * 2 + 2) <= pUser->QuerySynAttr()->GetRankCount(80))
// // 					&& (90 > nRank))
// // 				{
// // 					return;
// // 				}
// // 				nNewRank = 60;
// // 				break;
// // 			case 60:
// // 				nNewRank = 10;
// // 				break;
// // 			//[��;���� 2008/9/27]��ʱ����Ҫ����ȼ��Ļ�Ա
// // 			//case 30:
// // 			default:
// // 				return;
// // 			}
// // 
// // 			//[��;���� 2008/9/27]�����ߺ����ߵķֱ�Դ�
// // 			if(nTargetRank == 90)
// // 			{
// // 			// 	pSyn->SetSubLeaderID(0);
// // 				pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, 0, true);
// // 			}
// // 			if (bOnline)
// // 			{
// // 				pTarget->QuerySynAttr()->SetRank(nNewRank, true);
// // 				pUser->QuerySynAttr()->SynchroInfo();
// // 			}
// // 			else
// // 			{
// // 				pUser->QuerySynAttr()->SetRankByName(nNewRank, szName);
// // 			}
// // 	/*		CMsgSyndicate msg;
// // 			if(msg.Create(SYN_SET_BUS,(char*)pTarget->GetName()))
// // 			{
// // 				msg.m_pInfo->dwData	=	90;
// // 				pSubSyn->BroadcastSynMsg(&msg, pUser);
// // 			}*/
// // 
// // 			//[��;���� 2008/9/27]���β���Ҫ����,���¸�������������
// // 			/*SQLBUF szSQL = "";
// // 			sprintf(szSQL,"SELECT s.id,u.name from %s AS s LEFT JOIN sk_user AS u ON u.id = s.id  where s.syn_id= %d and s.rank = 90",_TBL_SYNATTR,idSyn);
// // 
// // 			IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// // 			if(pRes)
// // 			{
// // 				CUser *pUser = UserManager()->GetUser(pRes->GetInt(0));
// // 				if(pUser)
// // 				{
// // 					pUser->QuerySynAttr()->SetRank(50);
// // 				}
// // 				else
// // 				{
// // 					MESSAGESTR	buf;
// // 					SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// // 					pInfo->idSyn		= idSyn;
// // 					pInfo->nRank		= 50;
// // 					pInfo->ucFuncType	= SYNFUNC_SET_RANK;
// // 					SafeCopy(pInfo->StrParam, pRes->GetStr(1), _MAX_NAMESIZE);
// // 					pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
// // 					MapGroup(PID)->QuerySynMessage()->ChangeSyndicate(pInfo);
// // 					sprintf(szSQL, "UPDATE %s SET rank = 50  WHERE id = %u LIMIT 1",
// // 								_TBL_SYNATTR,pRes->GetInt(0));
// // 					Database()->ExecuteSQL(szSQL);
// // 				}
// // 			}*/
// // 
// // 			char szBuff[128];
// // 			if(nNewRank == 60)
// // 			{
// // 				sprintf(szBuff,"�������%sΪ%s����һְ",szName,pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_SUCC
// // 				pUser->SendSysMsg(szBuff);
// // 				//[��;���� 2008/9/27]�����ߺ����ߵķֱ�Դ�
// // 				if (bOnline)
// // 				{
// // 					sprintf(szBuff,"����Ϊ��%s����",pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_CON
// // 					pTarget->SendSysMsg(szBuff);
// // 					sprintf(szBuff,"%s����%s����һְ",pTarget->GetName(),pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_PROMPT
// // 					pTarget->SendSysMsg(_TXTATR_SYNDICATE,szBuff);
// // 				}
// // 
// // 			}
// // 			else
// // 			{
// // 				sprintf(szBuff,"�������%s��ְ��",szName);//STR_SYN_SETSUBLEADER_SUCC
// // 				pUser->SendSysMsg(szBuff);
// // 				//[��;���� 2008/9/27]�����ߺ����ߵķֱ�Դ�
// // 				if (bOnline)
// // 				{
// // 					strcpy(szBuff,"����ְ�񱻽����");//STR_SYN_SETSUBLEADER_CON
// // 					pTarget->SendSysMsg(szBuff);
// // 					sprintf(szBuff,"%s��ְ�񱻽��",pTarget->GetName());//STR_SYN_SETSUBLEADER_PROMPT
// // 					pTarget->SendSysMsg(_TXTATR_SYNDICATE,szBuff);
// // 				}
// // 
// // 			}
// // 			
// // 			
// // 		}
// // 		break;
// // 	case INVITE_RETIRE://[��;���� 2008/10/7]���ڴ�ְ
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			if( KICKOUTDATA_OUTDO_FALSE != pUser->QuerySynAttr()->GetLeaveSyn(pUser->GetID()) )
// // 			{
// // 				pUser->SendSysMsg("����ʧ�ܣ����Ѿ������˰");
// // 				return;
// // 			}
// // 
// // 			int nTargetRank = pUser->QuerySynAttr()->GetPlayRankByName(szName);
// // 			if(nTargetRank == 90)
// // 			{
// // 		// 		pSyn->SetSubLeaderID(0);
// // 				pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, 0, true);
// // 			}
// // 
// // 			char szBuff[128];
// // 			sprintf(szBuff,"%s %s ��ȥ ��%s�� ְλ��",pUser->QuerySynAttr()->GetRankTitle(),pUser->GetName(),pUser->QuerySynAttr()->GetRankTitle());
// // 			pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szBuff);
// // 		
// // 			pUser->QuerySynAttr()->SetRank(10, true);
// // 			pUser->QuerySynAttr()->SynchroInfo();			
// // 
// // 			sprintf(szBuff,"���Ѿ���ְ�ɹ�.");//STR_SYN_SETSUBLEADER_SUCC
// // 			pUser->SendSysMsg(szBuff);	
// // 		}
// // 		break;
// // 	case SYN_BUY_UP:
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			int level = pSyn->GetBuyLev();
// // 			if(level < 3 )
// // 			{
// // 				int nAmount = 0;
// // 				int nMoney = 0;
// // 				for(int i = 0; i < SynHomeLevSet()->GetAmount(); i++)
// // 				{
// // 					CSynHomeLevData* pType = SynHomeLevSet()->GetObjByIndex(i);
// // 					if(11 + level == pType->GetInt(SYNHOMELEVDATA_TYPE))
// // 					{
// // 						nAmount = pType->GetInt(SYNHOMELEVDATA_RANK);
// // 						nMoney = pType->GetInt(SYNHOMELEVDATA_MONEY);
// // 						break;
// // 					}
// // 				}
// // 					
// // 				int enReput = pSyn->GetInt(SYNDATA_REPUTE);
// // 				if(nAmount > enReput)
// // 				{
// // 					pUser->SendSysMsg("���׶Ȳ���!");
// // 					return;
// // 				}
// // 				int data = max(0,enReput - nAmount);
// // 			// 	pSyn->SetRepute(data);	
// // 				pSyn->QueryModify()->SetData(SYNDATA_REPUTE, data, true);
// // 				
// // 				int enMoney = pUser->GetMoney();
// // 				if(nMoney > enMoney)
// // 				{
// // 					pUser->SendSysMsg("�ʽ���!");
// // 					return;
// // 				}
// // 				pUser->SpendMoney(nMoney,true);
// // 
// // //				pSyn->SetBuyLev(level + 1);
// // //				pSyn->SynchroInfo();
// // 				pSyn->QueryModify()->SetData(SYNDATA_BUYLEV, level + 1, true);
// // 			}
// // 		}
// // 		break;
// // 	case SYN_MAIN_UP:		
// // 		break;
// // 	case SYN_TASK_UP:
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			int level = pSyn->GetTaskLev();
// // 			if(level < 3 )
// // 			{
// // 				int nAmount = 0;
// // 				int nMoney = 0;
// // 				for(int i = 0; i < SynHomeLevSet()->GetAmount(); i++)
// // 				{
// // 					CSynHomeLevData* pType = SynHomeLevSet()->GetObjByIndex(i);
// // 					if(21 + level == pType->GetInt(SYNHOMELEVDATA_TYPE))
// // 					{
// // 						nAmount = pType->GetInt(SYNHOMELEVDATA_RANK);
// // 						nMoney = pType->GetInt(SYNHOMELEVDATA_MONEY);
// // 						break;
// // 					}
// // 				}
// // 					
// // 				int enReput = pSyn->GetInt(SYNDATA_REPUTE);
// // 				if(nAmount > enReput)
// // 				{
// // 					pUser->SendSysMsg("���׶Ȳ���!");
// // 					return;
// // 				}
// // 				int data = max(0,enReput - nAmount);
// // 			// 	pSyn->SetRepute(data);	
// // 				pSyn->QueryModify()->SetData(SYNDATA_REPUTE, data, true);
// // 				
// // 				int enMoney = pUser->GetMoney();
// // 				if(nMoney > enMoney)
// // 				{
// // 					pUser->SendSysMsg("�ʽ���!");
// // 					return;
// // 				}
// // 				pUser->SpendMoney(nMoney,true);
// // 
// // //				pSyn->SetTaskLev(level + 1);
// // //				pSyn->SynchroInfo();
// // 				pSyn->QueryModify()->SetData(SYNDATA_TASKLEV, level + 1, true);
// // 			}
// // 		}
// // 		break;
// // 	case SYN_CHG_UP:
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			int level = pSyn->GetChgLev();
// // 			if(level < 3 )
// // 			{
// // 				int nAmount = 0;
// // 				int nMoney = 0;
// // 				for(int i = 0; i < SynHomeLevSet()->GetAmount(); i++)
// // 				{
// // 					CSynHomeLevData* pType = SynHomeLevSet()->GetObjByIndex(i);
// // 					if(31 + level == pType->GetInt(SYNHOMELEVDATA_TYPE))
// // 					{
// // 						nAmount = pType->GetInt(SYNHOMELEVDATA_RANK);
// // 						nMoney = pType->GetInt(SYNHOMELEVDATA_MONEY);
// // 						break;
// // 					}
// // 				}
// // 					
// // 				int enReput = pSyn->GetInt(SYNDATA_REPUTE);
// // 				if(nAmount > enReput)
// // 				{
// // 					pUser->SendSysMsg("���׶Ȳ���!");
// // 					return;
// // 				}
// // 				int data = max(0,enReput - nAmount);
// // 			// 	pSyn->SetRepute(data);	
// // 				pSyn->QueryModify()->SetData(SYNDATA_REPUTE, data, true);
// // 				
// // 				int enMoney = pUser->GetMoney();
// // 				if(nMoney > enMoney)
// // 				{
// // 					pUser->SendSysMsg("�ʽ���!");
// // 					return;
// // 				}
// // 				pUser->SpendMoney(nMoney,true);
// // 
// // //				pSyn->SetChgLev(level + 1);
// // //				pSyn->SynchroInfo();
// // 				pSyn->QueryModify()->SetData(SYNDATA_CHGLEV, level + 1, true);
// // 			}
// // 		}
// // 		break;
// // 	case SYN_SET_TEAMLEADER:
// // 		{
// // 			//  [1/15/2008 ���]	�������¿���
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			// ���Ȩ��
// // 			int nRank = pUser->GetSynRank();
// // 			if(nRank != RANK_LEADER || nRank != RANK_SUBLEADER)
// // 				return ;
// // 
// // 			// check amount, temporary code.
// // 			if(pUser->QuerySynAttr()->GetAssistantCount() >= MAX_ASSISTANTSIZE)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11099));//STR_SYN_PLACE_FULL
// // 				return ;
// // 			}
// // 
// // 			// target syn
// // 			CUser* pTarget = UserManager()->GetUser(szName);
// // 			if(!pTarget || (!pTarget->GetSyndicate()->IsSubSubSyn()))
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(12011));//STR_NOT_HERE
// // 				return ;
// // 			}
// // 			int nLevel = 30;
// // 			int nProffer = 1000;			
// // 
// // 			if(pTarget->GetLev() < nLevel && pTarget->QuerySynProffer() < nProffer )
// // 			{				
// // 				char szBuff[128];
// // 				sprintf(szBuff,g_objStrRes.GetStr(12014),pTarget->GetName());//STR_LEVEL_NOT_ENOUGH
// // 				pTarget->SendSysMsg(szBuff);
// // 				return ;
// // 			}
// // 
// // 			OBJID	idTargetSyn = pTarget->GetSynID();
// // 			if(idTargetSyn == ID_NONE || idTargetSyn == idSyn)
// // 				return ;
// // 			CSynPtr pSubSubSyn = SynManager()->QuerySyndicate(idTargetSyn);
// // 			CHECK(pSubSubSyn);
// // 
// // 			if(nRank == RANK_SUBLEADER)
// // 			{
// // 			}
// // 
// // 			pSubSubSyn->QueryModify()->Demise(pSubSubSyn->GetInt(SYNDATA_LEADERID),0, pTarget->GetID(), pTarget->GetName(), pTarget->QuerySynAttr()->GetSynMemberLevel(),pSubSubSyn->GetID());
// // 				
// // 			char szBuff[128];
// // 			sprintf(szBuff,g_objStrRes.GetStr(10050),szName,pTarget->GetSyndicate()->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBSUBLEADER_SUCC
// // 			pUser->SendSysMsg(szBuff);
// // 			sprintf(szBuff,g_objStrRes.GetStr(10051),pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBSUBLEADER_CON
// // 			pTarget->SendSysMsg(szBuff);
// // 			sprintf(szBuff,g_objStrRes.GetStr(10052),pTarget->GetName(),pTarget->GetSyndicate()->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBSUBLEADER_PROMPT
// // 			pTarget->SendSysMsg(_TXTATR_SYNDICATE,szBuff);		
// // 		}
// // 		break;
// // /*
// // 	case SYN_CHANGE_SUBSUBNAME://�Ķ���
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			int	  nRank = pUser->GetSynRankShow();
// // 			if(idSyn == ID_NONE || nRank != RANK_TEAMLEADER)
// // 				return ;
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 
// // 			if(!pSyn || strlen(pSyn->GetStr(SYNDATA_NAME)) <= 6 || !pSyn->IsSubSubSyn())
// // 				return ;
// // 
// // 			pSyn->SetNewSubName(szName);
// // 
// // 			CMsgSyndicate	msg;
// // 			if(msg.Create(SYN_CHANGE_SUBSUBNAME, pSyn->GetID(), szName))
// // 				pSyn->BroadcastSynMsg(&msg);
// // 			
// // 		}
// // 		break;
// // 	case SYN_CHANGE_SUBNAME://�ķ�����
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			int	  nRank = pUser->GetSynRankShow();
// // 			if(idSyn == ID_NONE || nRank != RANK_SUBLEADER)
// // 				return ;
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 
// // 			if(!pSyn || strlen(pSyn->GetStr(SYNDATA_NAME)) <= 6 || !pSyn->IsSubSyn())
// // 				return ;
// // 
// // 			pSyn->SetNewSubName(szName);
// // 
// // 			CMsgSyndicate	msg;
// // 			if(msg.Create(SYN_CHANGE_SUBNAME,  pSyn->GetID(), szName))
// // 				pSyn->BroadcastSynMsg(&msg);
// // 		}
// // 		break;
// // */
// // 
// // 	case SYNMEMBER_ASSIGN:			//���ɳ�Ա����---
// // 		{
// // 			//  [1/15/2008 ���]	�������¿���
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			int nRank = pUser->GetSynRank();
// // 			if(nRank!=RANK_LEADER || nRank!=RANK_SUBLEADER)
// // 			{
// // 				return;
// // 			}
// // 			CSyndicate * pSyn = pUser->GetSyndicate();
// // 			CHECK(pSyn);
// // 
// // 			OBJID idSynAssigned = m_pInfo->idTarget;
// // 			CSyndicate * pSynAssigned = SynManager()->QuerySyndicate(idSynAssigned);
// // 			CHECK(pSynAssigned);
// // 			
// // 			OBJID idUser = /*m_pInfo->idUser*/0;
// // 			CUser * pAssignUser = UserManager()->GetUser(idUser);
// // 			CHECK(pAssignUser);
// // 			
// // 			CSyndicate * pCurSyn = pAssignUser->GetSyndicate();
// // 			if(pCurSyn->GetID()==pSynAssigned->GetID())//����Ϊͬһ������
// // 				return;
// // 			//---��ʼ����---begin			
// // 			if(pAssignUser->QuerySynAttr()->ChangeSyn(pCurSyn, pSynAssigned))
// // 			{
// // 				//�ɹ��󣬻�Ӧ��Ϣ
// // 				pCurSyn->BroadcastSynMsg(this);				
// // 				pAssignUser->QuerySynAttr()->SynchroInfo(true);				
// // 			}					
// // 			//---��ʼ����---end			
// // 			
// // 		}
// // 		break;
// //	case	APPLY_JOIN:					// �����������
// //		{
// //			//  [1/15/2008 ���]	�������¿���
// //			if(!g_bEnableSynModificate)
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// //				return;
// //			}
// //
// //			// �����
// //			if(m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == idUser)
// //				return;
// //
// //			CUserPtr pTarget = g_UserManager.GetUser(m_pInfo->idTarget);
// //			if(!pTarget)
// //				return ;
// ////			if(pTarget->GetPhyle() != pUser->GetPhyle())
// ////				return ;
// //			// ������
// //			OBJID	idSyn	= pUser->GetSynID();
// //			int		nRank	= pUser->GetSynRank();
// //			OBJID	idTargetSyn = pTarget->GetSynID();
// //			int		nTargetRank	= pTarget->GetSynRank();
// //
// //			//��� ����
// //			if(idSyn != ID_NONE || idTargetSyn == ID_NONE ||  nTargetRank < RANK_TEAMLEADER || nTargetRank == RANK_LEADERWIFE )
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(10033));//STR_SYN_APPLY_JOIN_FAILED
// //				return;
// //			}
// //
// //			
// //			// no money ?
// //			CSyndicate* pTargetSyn = SynManager()->QuerySyndicate(idTargetSyn);
// //			if (!pTargetSyn)
// //				return;
// //
// //			if (pTargetSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(11011));//STR_TOO_LOWER_SYN_MONEY
// //				return;
// //			}
// //
// //			// is full?
// //			//int MemberAmount[7]={0,10,15,20,25,30,35};
// ////���ż���	��������
// ////S��	120
// ////A��	100
// ////B��	80
// ////C��	60
// ////D��	40
// ////E��	20
// //
// //			//[��;���� 2008/9/27]���ɳ�Ա�������һ��
// //			int MemberAmount[7]={0,40,80,120,160,200,240};//20070807����:�߻���
// //			if(pTargetSyn->GetInt(SYNDATA_AMOUNT) >= MemberAmount[pTargetSyn->GetInt(SYNDATA_RANK)])
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(11036));//STR_SYN_FULL
// //				return;
// //			}
// //			
// //
// //			if(!pTarget->FetchApply(CUser::APPLY_INVITE_JOIN_SYN, idUser))
// //			{
// //				// ����������
// //				pUser->SetApply(CUser::APPLY_JOIN_SYN, m_pInfo->idTarget);
// //				//20070403����:��������
// //				//IF_OK_(Create(APPLY_JOIN, idUser))
// //				IF_OK_(Create(APPLY_JOIN, idUser,pUser->GetName()))
// //					pTarget->SendMsg(this);
// //				return;
// //			}
// //
// //			nRank = RANK_NORMAL;
// ////			if( pUser->GetLev() < SYN_CONDITION_NEWBIE)
// ////				nRank = RANK_LEV1;
// //			if(pUser->Get_MateID() == pTargetSyn->GetLeaderID())
// //			{
// //				nRank = RANK_LEADERWIFE;
// //				pTargetSyn->SetLeaderWifeID(m_pInfo->idTarget);
// //			}
// //			
// //			pUser->QuerySynAttr()->JoinSyn(idTargetSyn, nRank);
// //			pUser->SendSysMsg(g_objStrRes.GetStr(11039), pTargetSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));//STR_JOIN_SYN_s
// //			MSGBUF szMsg;
// //			sprintf(szMsg,g_objStrRes.GetStr(11040) , pUser->GetName());//STR_SYN_NOTIFY_JOIN_s
// //			if(pUser->Get_MateID() == pTargetSyn->GetLeaderID())
// //			{
// //				sprintf(szMsg,"%s���������Ϊ����İ�������!", pUser->GetName());
// //			}
// //			pTargetSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// //		}
// //		break;
// // 	case INVITE_JOIN_NAME://[��;���� 2008/10/6]������Ҽ�����
// // 		{
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			// ������
// // 			OBJID	idSyn	= pUser->GetSynID();
// // 			int		nRank	= pUser->GetSynRank();
// // 			if(idSyn == ID_NONE || (nRank < RANK_TEAMLEADER || nRank == RANK_LEADERWIFE))
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11037));
// // 				return;
// // 			}
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if (!pSyn)
// // 				return;
// // 			
// // 			if(pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11011));
// // 				return;
// // 			}
// 			
// 			//int MemberAmount[7]={0,10,15,20,25,30,35};
// 			//���ż���	��������
// //S��	120
// //A��	100
// //B��	80
// //C��	60
// //D��	40
// //E��	20
// // 
// // 			int MemberAmount[7]={0,40,80,120,160,200,240};//20070807����:�߻���
// // 			if(pSyn->GetInt(SYNDATA_AMOUNT) >= MemberAmount[pSyn->GetInt(SYNDATA_RANK)])
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11036));//STR_SYN_FULL
// // 				return;
// // 			}
// 
// // 			CUser *pUserTarget = UserManager()->GetUser(szName);
// // 			if (!pUserTarget)
// // 			{
// // 				pUser->SendSysMsg("��������㲻��ͬһ��ͼ�����Ժ����ԡ�");
// // 				return;
// // 			}
// // 			else
// // 			{
// // 			m_pInfo->idTarget = idSyn;
// // 			m_pInfo->idFealty = pUser->GetID();
// // 			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
// // 			}
// // 			m_pInfo->idTarget = pUserTarget->GetID();
// // 		}
// // 		break;
// 		//[��;���� 2008/10/6]���¼���ִ��INVITE_JOIN���ϲ���
// 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	INVITE_JOIN_RE:
// // 		{
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 			if(m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == idUser)
// // 				return;
// // 
// // 			CUserPtr pTarget = g_UserManager.GetUser(m_pInfo->idTarget);
// // 			if(!pTarget)
// // 				return ;
// // 			
// // 			// ������
// // 			OBJID	idSyn	= pUser->GetSynID();
// // 			int		nRank	= pUser->GetSynRank();
// // 			OBJID	idTargetSyn = pTarget->GetSynID();
// // 			int		nTargetRank	= pTarget->GetSynRank();
// // 			if(idSyn == ID_NONE || (nRank < RANK_TEAMLEADER || nRank == RANK_LEADERWIFE) || idTargetSyn != ID_NONE)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11037));//STR_JOIN_SYN_FAILED
// // 				return;
// // 			}
// // 
// // 			// no money ?
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if (!pSyn)
// // 				return;
// // 			if(pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11011));//STR_TOO_LOWER_SYN_MONEY
// // 				return;
// // 			}
// // 			//int MemberAmount[7]={0,10,15,20,25,30,35};
// // 			//���ż���	��������
// // //S��	120
// // //A��	100
// // //B��	80
// // //C��	60
// // //D��	40
// // //E��	20
// // 
// // 			int MemberAmount[7]={0,40,80,120,160,200,240};//20070807����:�߻���
// // 			if(pSyn->GetInt(SYNDATA_AMOUNT) >= MemberAmount[pSyn->GetInt(SYNDATA_RANK)])
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11036));//STR_SYN_FULL
// // 				return;
// // 			}
// // 
// // 			if(pUser->FetchApply(CUser::APPLY_JOIN_SYN, m_pInfo->idTarget))
// // 			{
// // 				pUser->SetApply(0, 0);
// // 			}
// // 			else
// // 				return;
// // 			
// // 			nRank = RANK_NORMAL;
// // 			OBJID idMate = 0;
// // 			pTarget->QuerySynAttr()->GetMateIDByID(idMate, pTarget->GetID());
// // 			if(/*pTarget->Get_MateID()*/idMate == pSyn->GetLeaderID())
// // 			{
// // 				nRank = RANK_LEADERWIFE;
// // // 				pSyn->SetLeaderWifeID(m_pInfo->idTarget);
// // 				pSyn->QueryModify()->SetData(SYNDATA_LEADERWIFE, m_pInfo->idTarget, true);
// // 			}
// // 			
// // 			pTarget->QuerySynAttr()->JoinSyn(idSyn, nRank);
// // 			pTarget->SendSysMsg(g_objStrRes.GetStr(11039), pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));//STR_JOIN_SYN_s
// // 			MSGBUF szMsg;
// // 			sprintf(szMsg,g_objStrRes.GetStr(11040), pTarget->GetName());//STR_SYN_NOTIFY_JOIN_s
// // 			if(idMate == pSyn->GetLeaderID())
// // 			{
// // 				sprintf(szMsg,"%s���������Ϊ����İ�����ż!", pTarget->GetName());
// // 			}
// // 			pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 		}
// // 		break;
// // 	case	INVITE_JOIN:				// �����������
// // 		{
// // 		//  [1/15/2008 ���]	�������¿���
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			// �����
// // 			if(m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == idUser)
// // 				return;
// // 
// // 			CUserPtr pTarget = g_UserManager.GetUser(m_pInfo->idTarget);
// // 			if(!pTarget)
// // 				return ;
// // //			if(pTarget->GetPhyle() != pUser->GetPhyle())
// // //				return ;
// // 
// // 			// ������
// // 			OBJID	idSyn	= pUser->GetSynID();
// // 			int		nRank	= pUser->GetSynRank();
// // 			OBJID	idTargetSyn = pTarget->GetSynID();
// // 			int		nTargetRank	= pTarget->GetSynRank();
// // 			if(idTargetSyn == ID_NONE || (nTargetRank < RANK_TEAMLEADER || nTargetRank == RANK_LEADERWIFE) || idSyn != ID_NONE)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11037));//STR_JOIN_SYN_FAILED
// // 				return;
// // 			}
// // 
// // 			// no money ?
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idTargetSyn);
// // 			if (!pSyn)
// // 				return;
// // //			if(pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// // //			{
// // //				pUser->SendSysMsg(g_objStrRes.GetStr(11011));//STR_TOO_LOWER_SYN_MONEY
// // //				return;
// // //			}
// // 			//int MemberAmount[7]={0,10,15,20,25,30,35};
// // 			//���ż���	��������
// // //S��	120
// // //A��	100
// // //B��	80
// // //C��	60
// // //D��	40
// // //E��	20
// // 
// // 			int MemberAmount[7]={0,40,80,120,160,200,240};//20070807����:�߻���
// // 			if(pSyn->GetInt(SYNDATA_AMOUNT) >= MemberAmount[pSyn->GetInt(SYNDATA_RANK)])
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11036));//STR_SYN_FULL
// // 				return;
// // 			}
// // 
// // 			if(!pTarget->FetchApply(CUser::APPLY_JOIN_SYN, idUser))
// // 			{
// // 				// ����������
// // 				pTarget->SetApply(CUser::APPLY_JOIN_SYN, idUser);
// // 				//20070403����:��������
// // 				//IF_OK_(Create(INVITE_JOIN, idUser))
// // 				IF_OK_(Create(/*INVITE_JOIN*/APPLY_JOIN, idUser,pUser->GetName()))
// // 					pTarget->SendMsg(this);
// // 				return;
// // 			}
// // 		}
// // 		break;
// // 	case	APPLY_JOIN:	
// // 		{
// // 			if(pUser->GetSynID() == 0)
// // 			{
// // 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
// // 			}
// 			//  [1/15/2008 ���]	�������¿���
// //			if(!g_bEnableSynModificate)
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// //				return;
// //			}
// //
// //			// �����
// //			if(m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == idUser)
// //				return;
// //
// //			CUserPtr pTarget = g_UserManager.GetUser(m_pInfo->idTarget);
// //			if(!pTarget)
// //				return ;
// ////			if(pTarget->GetPhyle() != pUser->GetPhyle())
// ////				return ;
// //
// //			// ������
// //			OBJID	idSyn	= pUser->GetSynID();
// //			int		nRank	= pUser->GetSynRank();
// //			OBJID	idTargetSyn = pTarget->GetSynID();
// //			int		nTargetRank	= pTarget->GetSynRank();
// //			if(idSyn == ID_NONE || (nRank < RANK_TEAMLEADER && nRank != RANK_LEADERWIFE) || idTargetSyn != ID_NONE)
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(11037));//STR_JOIN_SYN_FAILED
// //				return;
// //			}
// //
// //			// no money ?
// //			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// //			if (!pSyn)
// //				return;
// //			if(pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(11011));//STR_TOO_LOWER_SYN_MONEY
// //				return;
// //			}
// //			//int MemberAmount[7]={0,10,15,20,25,30,35};
// //			//���ż���	��������
// ////S��	120
// ////A��	100
// ////B��	80
// ////C��	60
// ////D��	40
// ////E��	20
// //
// //			int MemberAmount[7]={0,40,80,120,160,200,240};//20070807����:�߻���
// //			if(pSyn->GetInt(SYNDATA_AMOUNT) >= MemberAmount[pSyn->GetInt(SYNDATA_RANK)])
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(11036));//STR_SYN_FULL
// //				return;
// //			}
// //
// //			if(!pTarget->FetchApply(CUser::APPLY_JOIN_SYN, idUser))
// //			{
// //				// ����������
// //				pUser->SetApply(CUser::APPLY_INVITE_JOIN_SYN, m_pInfo->idTarget);
// //				//20070403����:��������
// //				//IF_OK_(Create(INVITE_JOIN, idUser))
// //				IF_OK_(Create(INVITE_JOIN, idUser,pUser->GetName()))
// //					pTarget->SendMsg(this);
// //				return;
// //			}
// //
// //			//20070406����:�߻�����.������Ա���������
// //			//�������20�� ��Ԥ����Ա
// ////			nRank = RANK_LEV2;
// ////			if( pTarget->GetLev() < SYN_CONDITION_NEWBIE)
// ////				nRank = RANK_LEV1;
// //			nRank = RANK_NORMAL;
// //			if(pTarget->Get_MateID() == pSyn->GetLeaderID())
// //			{
// //				nRank = RANK_LEADERWIFE;
// //				pSyn->SetLeaderWifeID(m_pInfo->idTarget);
// //			}
// //			
// //			pTarget->QuerySynAttr()->JoinSyn(idSyn, nRank);
// //			pTarget->SendSysMsg(g_objStrRes.GetStr(11039), pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));//STR_JOIN_SYN_s
// //			MSGBUF szMsg;
// //			sprintf(szMsg,g_objStrRes.GetStr(11040), pTarget->GetName());//STR_SYN_NOTIFY_JOIN_s
// //			if(pTarget->Get_MateID() == pSyn->GetLeaderID())
// //			{
// //				sprintf(szMsg,"%s���������Ϊ����İ�������!", pTarget->GetName());
// //			}
// //			pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 		}
// // 		break;
// 
// 		// the operation of members
// 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	LEAVE_SYN:				// ��������
// // 		{
// // 			//  [1/15/2008 ���]	�������¿���
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			// ������
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return;
// // 			
// // 			// ������
// // 			int	nRank = pUser->GetSynRank();
// // 			if(nRank == RANK_LEADER)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(12008));//STR_NO_DISBAND
// // 				return;
// // 			}
// // 
// // 			int reData = pUser->QuerySynAttr()->SetLeaveSyn(1,pUser->GetID());
// // 			if(KICKOUTDATA_OUTDO_TRUE == reData)
// // 			{
// // 				pUser->SendSysMsg("����ɹ���1����¼ʱ����˳�������");
// // 			}
// // 			else if(KICKOUTDATA_OUTCHECK_BYKICK == reData)
// // 			{
// // 				pUser->SendSysMsg("���Ѿ����߳���ᣬ�����ĵȴ���������ɹ�1��󣬵�¼ʱ����˳�������");
// // 			}
// // 			else if(KICKOUTDATA_OUTCHECK_BYUSER == reData)
// // 			{
// // 				pUser->SendSysMsg("���Ѿ������˳�����ˣ������ĵȴ���������ɹ�1��󣬵�¼ʱ����˳�������");
// // 			}
// // 			
// // 			return;
// // 			//pUser->QuerySynAttr()->GetLeaveSyn(pUser->GetID());
// // 		
// // 			
// // 			
// // 			pUser->QuerySynAttr()->LeaveSyn();
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if (pSyn)
// // 			{
// // 				if(RANK_SUBLEADER == nRank)
// // 				{
// // 		// 			pSyn->SetSubLeaderID(0);
// // 					pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, 0, true);
// // 				}
// // 				else if(RANK_LEADERWIFE == nRank)	
// // 				{
// // 	// 				pSyn->SetLeaderWifeID(0);
// // 					pSyn->QueryModify()->SetData(SYNDATA_LEADERWIFE, 0, true);
// // 				}
// // 				
// // 				char szMsg[256];
// // 				sprintf(szMsg,g_objStrRes.GetStr(10035) , pUser->GetName());//STR_SYN_LEAVE_s
// // 			
// // 				CMsgTalk	msg;
// // 				IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, 0xFFFFFF, _TXTATR_SYNDICATE))
// // 					pSyn->BroadcastSynMsg(&msg);
// // 				m_StrPacker.AddString(pUser->GetName());
// // 				m_unMsgSize = sizeof(MSG_Info)-1+m_StrPacker.GetSize();
// // 				pSyn->BroadcastSynMsg(this);				
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(14095));//"���Ѿ��˳��˾���"
// // 
// // 			}
// // 			
// // 		}
// // 		break;
// // 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case KICKOUT_MEMBER_NOAGREE:
// // 		{
// // 			//  [1/15/2008 ���]	�������¿���
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			char	szSQL[1024];
// // 			sprintf(szSQL, "SELECT id FROM sk_synattr WHERE syn_id = %d and rank = 90",idSyn);	
// // 			IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// // 			if(pRes)
// // 			{
// // 				CUser* pTargetUser = UserManager()->GetUser(pRes->GetInt(0));
// // 				if(pTargetUser)
// // 				{
// // 					pTargetUser->SendSysMsg(g_objStrRes.GetStr(15087));//"�ų��ܾ�����������"
// // 				}
// // 			}
// // 
// // 			//////////////////////////////////////////////////////////////////////////
// // 		/*	char	szSQL[1024];
// // 			sprintf(szSQL, "SELECT * FROM %s WHERE words LIKE 'ApplyDismiss%'", _TBL_LEAVEWORD);	
// // 			IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// // 			if(pRes)
// // 			{
// // 				for(int i = 0; i < pRes->RecordCount(); i++)
// // 				{
// // 					char	szRecvUser[_MAX_NAMESIZE];
// // 					SafeCopy(szRecvUser, pRes->GetStr(LEAVEWORDDATA_USER), _MAX_NAMESIZE);
// // 					
// // 					CUserPtr pUser = g_UserManager.GetUser(szRecvUser);
// // 					if(pUser && !pUser->IsMoreLeaveWord())
// // 					{
// // 						char	szTime[_MAX_NAMESIZE];
// // 						char	szWords[_MAX_WORDSSIZE];					
// // 						SafeCopy(szTime, pRes->GetStr(LEAVEWORDDATA_TIME), _MAX_NAMESIZE);
// // 						SafeCopy(szWords, pRes->GetStr(LEAVEWORDDATA_WORDS), _MAX_WORDSSIZE);						
// // 						//---jinggy---2004-12-20---�ж��Ƿ��� ���뿪����Ա������---begin
// // 						char szBuffer[255]=""; //������Ա�ı�־
// // 						OBJID idSyn; //Ҫ�����ĳ�Ա�İ���ID
// // 						char  szNameDismissed[16]; //�����ĳ�ԱID		
// // 						char  szLeaderName[16];
// // 						
// // 						if(4!=sscanf(szWords,"%s %d %s %s",szBuffer,&idSyn,szLeaderName,szNameDismissed))
// // 							continue;
// // 						if(strcmp(szBuffer,"ApplyDismiss")==0 && idSyn>0 ) //�����뿪������
// // 						{
// // 							CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 							CHECK(pSyn);
// // 							DWORD dwMainSynID = pSyn->GetMasterSynID();
// // 							
// // 							char szCurLeaderName[16];
// // 							sprintf(szCurLeaderName,"%s",pSyn->GetStr(SYNDATA_LEADERNAME));
// // 							
// // 							if(strcmp(szNameDismissed,szName)==0) //��Ч���ڴ��˻����쵼��������
// // 							{
// // 								//ɾ����¼
// // 								OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
// // 								sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
// // 								g_db.ExecuteSQL(szSQL);								
// // 								break;								
// // 							}						
// // 						}						
// // 					}
// // 				}
// // 				pRes->MoveNext();
// // 			}
// // 			pRes->Release();*/
// // 		
// // 		}
// // 		break;
// // 
// // 	case KICKOUT_MEMBER_INFO_QUERY: //�������б�
// // 		{
// // 			//  [1/15/2008 ���]	�������¿���
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			if(pUser->GetSynRank() != RANK_LEADER)  
// // 				return;
// // 
// // 			char	szSQL[1024];
// // 			sprintf(szSQL, "SELECT * FROM %s WHERE words LIKE 'ApplyDismiss%'", _TBL_LEAVEWORD);	
// // 			IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// // 			if(pRes)
// // 			{
// // 				for(int i = 0; i < pRes->RecordCount(); i++)
// // 				{
// // 					char	szRecvUser[_MAX_NAMESIZE];
// // 					SafeCopy(szRecvUser, pRes->GetStr(LEAVEWORDDATA_USER), _MAX_NAMESIZE);
// // 					
// // 					CUserPtr pUser = g_UserManager.GetUser(szRecvUser);
// // 					if(pUser && !pUser->IsMoreLeaveWord())
// // 					{
// // 					//	char	szSender[_MAX_NAMESIZE];
// // 						char	szTime[_MAX_NAMESIZE];
// // 						char	szWords[_MAX_WORDSSIZE];
// // 					//	SafeCopy(szSender, pRes->GetStr(LEAVEWORDDATA_SENDER), _MAX_NAMESIZE);
// // 						SafeCopy(szTime, pRes->GetStr(LEAVEWORDDATA_TIME), _MAX_NAMESIZE);
// // 						SafeCopy(szWords, pRes->GetStr(LEAVEWORDDATA_WORDS), _MAX_WORDSSIZE);
// // 						
// // 						
// // 						//---jinggy---2004-12-20---�ж��Ƿ��� ���뿪����Ա������---begin
// // 						char szBuffer[255]=""; //������Ա�ı�־
// // 						OBJID idSyn; //Ҫ�����ĳ�Ա�İ���ID
// // 						char  szNameDismissed[16]; //�����ĳ�ԱID		
// // 						char  szLeaderName[16];
// // 						
// // 						if(4!=sscanf(szWords,"%s %d %s %s",szBuffer,&idSyn,szLeaderName,szNameDismissed))
// // 							continue;
// // 						
// // 						if(strcmp(szBuffer,"ApplyDismiss")==0 && idSyn>0 ) //�����뿪������
// // 						{
// // 							time_t	tDelta = time(NULL);
// // 							tDelta -= 7 * 86400;  //����7�죬����û����Ӧ�ͱ�ʾͬ�⿪��
// // 							char	szDelta[_MAX_NAMESIZE];
// // 							
// // 							tm*	pTm		= localtime(&tDelta);
// // 							if(pTm)
// // 							{
// // 								sprintf(szDelta, "%04d%02d%02d%02d%02d%02d", 
// // 									pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);								
// // 							}
// // 							
// // 							if(strcmp(szTime,szDelta) < 0)
// // 							{
// // 								//��Ӧ����											
// // 								CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // //								if(pSyn && pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// // //								{
// // //									pUser->SendSysMsg(g_objStrRes.GetStr(11011));//STR_TOO_LOWER_SYN_MONEY
// // //									return;
// // //								}
// // 								pUser->QuerySynAttr()->KickoutMember(szNameDismissed);						
// // 								
// // 								MSGBUF	szMsg;//STR_KICKOUT_MEMBER_ss
// // 								sprintf(szMsg,g_objStrRes.GetStr(11041), szNameDismissed, pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));
// // 								if(pSyn)
// // 									pSyn->BroadcastSubSynMsg(pUser->GetSocketID(), 0, szMsg);
// // 								
// // 								
// // 								//ɾ����¼
// // 								OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
// // 								sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
// // 								g_db.ExecuteSQL(szSQL);
// // 							}
// // 							else  
// // 							{
// // 									
// // 								CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 								DWORD dwMainSynID = pSyn->GetMasterSynID();
// // 								
// // 								char szCurLeaderName[16];
// // 								sprintf(szCurLeaderName,"%s",pSyn->GetStr(SYNDATA_LEADERNAME));
// // 
// // 								if(strcmp(szCurLeaderName,szLeaderName)==0) //��Ч���ڴ��˻����쵼��������
// // 								{
// // 									//������������뿪���ĳ�Ա����Ϣ									
// // //									if(Create(KICKOUT_MEMBER_INFO_QUERY, szNameDismissed))
// // //										pUser->SendMsg(this);								
// // 								}
// // 								else //����Ч�������ǻ��˰�������ȡ������
// // 								{
// // 									//ɾ����¼
// // 									OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
// // 									sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
// // 									g_db.ExecuteSQL(szSQL);
// // 								}
// // 							}							
// // 						}
// // 					}
// // 					pRes->MoveNext();
// // 				}
// // 				pRes->Release();
// // 			}
// // 		}
// // 		break;
// // 	case KICKOUT_MEMBER_AGREE:  //����ͬ������Ŀ�����Ա
// // 
// // 		{
// // 		/*	// ������
// // 			OBJID idSyn = pUser->GetSynID();
// // 			int nRank = pUser->GetSynRank();
// // 			
// // 			if(idSyn == ID_NONE || nRank != RANK_LEADER )
// // 			{
// // 				pUser->SendSysMsg(STR_NOT_AUTHORIZED);
// // 				return;
// // 			}
// // 
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 
// // 			// no money ?
// // 			if(pSyn && pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// // 			{
// // 				pUser->SendSysMsg(STR_TOO_LOWER_SYN_MONEY);
// // 				return;
// // 			}
// // 
// // 			//pTarget->QuerySynAttr()->LeaveSyn();		// �ĳ������߳�
// // 			pUser->QuerySynAttr()->KickoutMember(szName);
// // 
// // 			MSGBUF	szMsg;
// // 			sprintf(szMsg, STR_KICKOUT_MEMBER_ss, LPCTSTR(szName), pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));
// // 			IF_OK(pSyn)
// // 				pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);		
// // 			
// // 			//////////////////////////////////////////////////////////////////////////
// // 			char	szSQL[1024];
// // 			sprintf(szSQL, "SELECT * FROM %s WHERE words LIKE 'ApplyDismiss%'", _TBL_LEAVEWORD);	
// // 			IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// // 			if(pRes)
// // 			{
// // 				for(int i = 0; i < pRes->RecordCount(); i++)
// // 				{
// // 					char	szRecvUser[_MAX_NAMESIZE];
// // 					SafeCopy(szRecvUser, pRes->GetStr(LEAVEWORDDATA_USER), _MAX_NAMESIZE);
// // 					
// // 					CUserPtr pUser = g_UserManager.GetUser(szRecvUser);
// // 					if(pUser && !pUser->IsMoreLeaveWord())
// // 					{
// // 						char	szTime[_MAX_NAMESIZE];
// // 						char	szWords[_MAX_WORDSSIZE];					
// // 						SafeCopy(szTime, pRes->GetStr(LEAVEWORDDATA_TIME), _MAX_NAMESIZE);
// // 						SafeCopy(szWords, pRes->GetStr(LEAVEWORDDATA_WORDS), _MAX_WORDSSIZE);						
// // 						//---jinggy---2004-12-20---�ж��Ƿ��� ���뿪����Ա������---begin
// // 						char szBuffer[255]=""; //������Ա�ı�־
// // 						OBJID idSyn; //Ҫ�����ĳ�Ա�İ���ID
// // 						char  szNameDismissed[16]; //�����ĳ�ԱID		
// // 						char  szLeaderName[16];
// // 						
// // 						if(4!=sscanf(szWords,"%s %d %s %s",szBuffer,&idSyn,szLeaderName,szNameDismissed))
// // 							continue;
// // 						if(strcmp(szBuffer,"ApplyDismiss")==0 && idSyn>0 ) //�����뿪������
// // 						{
// // 							CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 							CHECK(pSyn);
// // 							DWORD dwMainSynID = pSyn->GetMasterSynID();
// // 							
// // 							char szCurLeaderName[16];
// // 							sprintf(szCurLeaderName,"%s",pSyn->GetStr(SYNDATA_LEADERNAME));
// // 							
// // 							if(strcmp(szNameDismissed,szName)==0) //��Ч���ڴ��˻����쵼��������
// // 							{
// // 								//ɾ����¼
// // 								OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
// // 								sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
// // 								g_db.ExecuteSQL(szSQL);								
// // 								break;								
// // 							}						
// // 						}						
// // 					}
// // 				}
// // 				pRes->MoveNext();
// // 			}
// // 			pRes->Release();*/
// // 		}
// // 		break;
// // 		
// // 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	KICKOUT_MEMBER:				// ����������Ա
// // 		{
// // 			//  [1/15/2008 ���]	�������¿���
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			// ������
// // 			OBJID idSyn = pUser->GetSynID();
// // 			int nRank = pUser->GetSynRank();
// // 			if(idSyn == ID_NONE || nRank < 90)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11097));//STR_NOT_AUTHORIZED
// // 				return;
// // 			}
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			CHECK(pSyn);
// // 			
// // 			// no money ?
// // //			if(pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// // //			{
// // //				pUser->SendSysMsg(g_objStrRes.GetStr(11011));//STR_TOO_LOWER_SYN_MONEY
// // //				return;
// // //			}
// // 			if( nRank == 100 || nRank == 90)//�ų��͸��ų�ֱ�ӿ���
// // 			{				
// // 				// ��
// // 				CUser* pTargetUser = UserManager()->GetUser(szName);
// // 				if (pTargetUser)
// // 				{
// // 					if(pTargetUser->GetSynRank() == 80)
// // 					{
// // 						pUser->SendSysMsg("���ܶ԰�����ż���в���!");//STR_TOO_LOWER_SYN_MONEY
// // 						return;
// // 					}
// // 					if(pTargetUser->GetSynRank() == 90)
// // 					{
// // 				// 		pSyn->SetSubLeaderID(0);
// // 						pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, 0, true);
// // 					}
// // 					//pTargetUser->SendSysMsg(g_objStrRes.GetStr(11041), pTargetUser->GetName(), pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));//STR_KICKOUT_MEMBER_ss
// // 					if (pTargetUser->GetSynID() == idSyn)
// // 					{
// // 						OBJID idData = pUser->QuerySynAttr()->GetUserIDByName(szName);
// // 						int reData = pUser->QuerySynAttr()->SetLeaveSyn(2,idData);
// // 						if(KICKOUTDATA_OUTDO_TRUE == reData)
// // 						{
// // 							pUser->SendSysMsg("����ɹ���1����¼ʱ����߳�������");
// // 							pTargetUser->SendSysMsg("����쵼�����������ᣬ��1����¼ʱ����߳�������");
// // 						}
// // 						else if(KICKOUTDATA_OUTCHECK_BYKICK == reData)
// // 						{
// // 							pUser->SendSysMsg("���Ѿ����߳���ᣬ�����ĵȴ���������ɹ�1��󣬵�¼ʱ����˳�������");
// // 						}
// // 						else if(KICKOUTDATA_OUTCHECK_BYUSER == reData)
// // 						{
// // 							pUser->SendSysMsg("���Ѿ������˳�����ˣ������ĵȴ���������ɹ�1��󣬵�¼ʱ����˳�������");
// // 						}
// // 					}					
// // 					//	pTargetUser->QuerySynAttr()->LeaveSyn();
// // 				}
// // 				else
// // 				{
// // 					if(pUser->QuerySynAttr()->GetPlayRankByName(szName) == 80)
// // 					{
// // 						pUser->SendSysMsg("���ܶ԰�����ż���в���!");//STR_TOO_LOWER_SYN_MONEY
// // 					}
// // 					if(pUser->QuerySynAttr()->GetPlayRankByName(szName) == 90)
// // 					{
// // // 						pSyn->SetSubLeaderID(0);
// // 						pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, 0, true);
// // 					}
// // 
// // 					OBJID idData = pUser->QuerySynAttr()->GetUserIDByName(szName);
// // 					int reData = pUser->QuerySynAttr()->SetLeaveSyn(2,idData);
// // 					if(KICKOUTDATA_OUTDO_TRUE == reData)
// // 					{
// // 						pUser->SendSysMsg("����ɹ���1����¼ʱ����߳�������");
// // 					}
// // 					else if(KICKOUTDATA_OUTCHECK_BYKICK == reData)
// // 					{
// // 						pUser->SendSysMsg("���Ѿ����߳���ᣬ�����ĵȴ���������ɹ�1��󣬵�¼ʱ����˳�������");
// // 					}
// // 					else if(KICKOUTDATA_OUTCHECK_BYUSER == reData)
// // 					{
// // 						pUser->SendSysMsg("���Ѿ������˳�����ˣ������ĵȴ���������ɹ�1��󣬵�¼ʱ����˳�������");
// // 					}
// // 					pUser->QuerySynAttr()->KickoutMember(szName);
// // 				}
// // 				
// // // 				MSGBUF	szMsg;
// // // 				//			// ֪ͨ�Է�
// // // 				//			sprintf(szMsg, STR_KICKOUT_ME_s, pUser->GetName());
// // // 				//			CMsgTalk msg;
// // // 				//			if (msg.Create(SYSTEM_NAME, szName, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
// // // 				//				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, szName);
// // // 				
// // // 				sprintf(szMsg, g_objStrRes.GetStr(11041), szName, pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));//STR_KICKOUT_MEMBER_ss
// // // 				IF_OK(pSyn)
// // // 					pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // // 				//m_StrPacker.AddString(pTargetUser->GetName());
// // // 				m_StrPacker.AddString(szName);//20061027����,��Ա������ʱ���ܷǷ�����
// // // 				m_unMsgSize = sizeof(MSG_Info)-1+m_StrPacker.GetSize();
// // // 				pSyn->BroadcastSynMsg(this);
// // 			}
// // //			else	//���ų����ų�����
// // //			{
// // //				CUser *pLeader = UserManager()->GetUser(pSyn->GetLeaderID());
// // //				if(pLeader == NULL)
// // //				{
// // //					pUser->SendSysMsg(g_objStrRes.GetStr(15088));//"�Բ����ų������ߣ��˴β����޷�ִ�С�"
// // //				}
// // //				else
// // //				{
// // //					if(Create(KICKOUT_MEMBER_INFO_QUERY,szName))
// // //						pLeader->SendMsg(this);
// // //				}
// // //			}
// // 		}
// // 		break;
// // 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	QUERY_SYN_NAME:
// // 		{
// // 			OBJID idSyn	= m_pInfo->idTarget;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 			{
// // //				LOGWARNING("���[%s]δ�ҵ�������[%d]!", pUser->GetName(), idSyn);
// // 				return;
// // 			}
// // 
// // 			CSynPtr pMasterSyn = pSyn->GetMasterSyn();
// // 			CHECK(pMasterSyn);
// // 
// // 			CMsgName	msg;
// // 			IF_OK(msg.Create(NAMEACT_SYNDICATE, pMasterSyn->GetStr(SYNDATA_NAME), idSyn))
// // 			{
// // 				if(pSyn->GetID() != pMasterSyn->GetID())
// // 				{
// // 					ASSERT(msg.AppendStr(pSyn->GetStr(SYNDATA_NAME)));
// // 				}
// // 				pUser->SendMsg(&msg);
// // 			}
// // 		}
// // 		break;
// // 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	DONATE_MONEY:
// // 		{
// // 			if(pUser->GetSynID() == ID_NONE)
// // 				return ;
// // 
// // 			int nMoney = m_pInfo->idTarget;
// // 
// // 		//	pUser->GiveSynVAS(nMoney);
// // 			//pUser->QuerySynAttr()->DonateMoney(nMoney);
// // 		}
// // 		break;
// // 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	QUERY_SYNATTR:
// // 		{
// // 			pUser->QuerySynAttr()->SendInfoToClient();
// // 		}
// // 		break;
// // 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	SYN_WAR:
// // 		{
// // 			if(m_pInfo->szBuf == NULL )
// // 				return;
// // 
// // 			CSynPtr pTargetSyn = SynManager()->QuerySynByName(szName);			
// // 			if( !pTargetSyn )			
// // 			{
// // 				pUser->SendSysMsg("������ƴ���!");
// // 				return;
// // 			}
// // 
// // 			OBJID	idSyn	= pUser->GetSynID();
// // 			OBJID	idTargetSyn = pTargetSyn->GetID();
// // 		
// // 			CSynPtr pThisSyn = SynManager()->QuerySyndicate(idSyn);
// // 
// // 			if(pThisSyn->GetState() != 0)
// // 			{
// // 				pUser->SendSysMsg("�Ұ�ỹ��Լս��!");
// // 				return;
// // 			}
// // 			if(pTargetSyn->GetState() != 0)
// // 			{
// // 				pUser->SendSysMsg("�Է�����Լս��!");
// // 				return;
// // 			}
// // 			if(pTargetSyn->GetInt(SYNDATA_RANK) + 2 <= pThisSyn->GetInt(SYNDATA_RANK))
// // 			{
// // 				pUser->SendSysMsg("�����۸���С���!");
// // 				return;
// // 			}
// // 
// 
// // 			
// // 			bool isOK = false;
// // 			OBJID	idTemp =	pTargetSyn->GetLeaderID();
// // 			CUserPtr pTargetUser = UserManager()->GetUser(idTemp);
// // 			if(pTargetUser)
// // 			{
// // 				isOK = true;
// // 				m_pInfo->idTarget = idTemp;
// // 			}
// // 			if(!isOK)
// // 			{
// // 				idTemp	=	pTargetSyn->GetSubLeaderID();
// // 				pTargetUser = UserManager()->GetUser(idTemp);
// // 				if(pTargetUser)
// // 				{
// // 					isOK = true;
// // 					m_pInfo->idTarget = idTemp;
// // 				}
// // 			}
// // 			if(!isOK)
// // 			{
// // 				idTemp	=	pTargetSyn->GetLeaderWifeID();
// // 				pTargetUser = UserManager()->GetUser(idTemp);
// // 				if(pTargetUser)
// // 				{
// // 					isOK = true;
// // 					m_pInfo->idTarget = idTemp;
// // 				}
// // 			}
// // 			
// // 			if(!pTargetUser || !isOK)
// // 			{			
// // 				pUser->SendSysMsg(/*g_objStrRes.GetStr(15089)*/"�Է�����쵼�߲����߻��ڱ���ͼ!");//"�Է����ų�������"
// // 				return;
// // 			}
// // 
// // 			// ������
// // 			int		nRank	= pUser->GetSynRankShow();
// // 			int		nTargetRank	= pTargetUser->GetSynRankShow();
// // 
// // 			if(idSyn == ID_NONE || (nRank != RANK_LEADER && nRank != RANK_SUBLEADER && nRank != RANK_LEADERWIFE)
// // 				|| idTargetSyn == ID_NONE || (nTargetRank != RANK_LEADER && nTargetRank != RANK_SUBLEADER && nRank != RANK_LEADERWIFE))
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11014));//STR_APPLY_ALLY_FAILED
// // 				return;
// // 			}
// // 
// // 			if(!pTargetUser->FetchApply(CUser::APPLY_SYNWAR, idUser))
// // 			{
// // 				// ����������
// // 				pUser->SetApply(CUser::APPLY_SYNWAR, m_pInfo->idTarget);
// // 				char tempName[_MAX_NAMESIZE] = "";
// // 				
// // 				strcpy(tempName,pThisSyn->GetStr(SYNDATA_NAME));
// // 				IF_OK_(Create(SYN_WAR,/*szName*/tempName))
// // 					pTargetUser->SendMsg(this);
// // 				return;
// // 			}
// // 
// // 			
// //   			CSyndicate* pSyn = pUser->GetSyndicate();
// // 			CHECK(pSyn);	
// // 			CHECK(pTargetSyn);			
// // 
// // 
// // 			char sendTemp[128];
// // 			sprintf(sendTemp,"%d %d",pSyn->GetID(),0);
// // 			CMsgSyndicate msg;
// // 			msg.Create(SYN_WAR_SEND,pTargetSyn->GetID(),sendTemp);
// // 			CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_FIRST,&msg);
// // 
// // // 			pSyn->SetSynWar(idTargetSyn);
// // // 			pTargetSyn->SetSynWar(idSyn);
// // // 			
// // // 			MSGBUF szMsg;
// // // 			sprintf(szMsg, "%s����%s��ῪʼԼս!", pSyn->GetStr(SYNDATA_NAME), pTargetSyn->GetStr(SYNDATA_NAME));
// // // 			UserManager()->BroadcastMsg(szMsg, NULL, NULL, 0xffffff, _TXTATR_SYSTEM);		
// // 						
// // 		}
// // 		break;
// // 	case	_APPLY_ALLY:	// ������ӽ��˰���
// // 		{
// // 			// �����
// // 		//	if(m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == idUser)
// // //				return;
// // 			if(m_pInfo->szBuf == NULL )
// // 				return;
// // 
// // 			CSynPtr pTargetSyn = SynManager()->QuerySynByName(szName);			
// // 			if( !pTargetSyn )
// // 			{
// // 				pUser->SendSysMsg("������ƴ���!");
// // 				return;
// // 			}
// // 			
// // 			OBJID	idSyn	= pUser->GetSynID();
// // 			OBJID	idTargetSyn = pTargetSyn->GetID();
// // 			if(idSyn == idTargetSyn)
// // 				return;
// // 			
// // 			bool isOK = false;
// // 			OBJID	idTemp =	pTargetSyn->GetLeaderID();
// // 			CUserPtr pTargetUser = UserManager()->GetUser(idTemp);
// // 			if(pTargetUser)
// // 			{
// // 				isOK = true;
// // 				m_pInfo->idTarget = idTemp;
// // 			}
// // 			if(!isOK)
// // 			{
// // 				idTemp	=	pTargetSyn->GetSubLeaderID();
// // 				pTargetUser = UserManager()->GetUser(idTemp);
// // 				if(pTargetUser)
// // 				{
// // 					isOK = true;
// // 					m_pInfo->idTarget = idTemp;
// // 				}
// // 			}
// // 			if(!isOK)
// // 			{
// // 				idTemp	=	pTargetSyn->GetLeaderWifeID();
// // 				pTargetUser = UserManager()->GetUser(idTemp);
// // 				if(pTargetUser)
// // 				{
// // 					isOK = true;
// // 					m_pInfo->idTarget = idTemp;
// // 				}
// // 			}
// // 			
// // 			if(!pTargetUser || !isOK)
// // 			{			
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(15089));//"�Է����ų�������"
// // 				return;
// // 			}
// // 
// // 			// ������
// // 			int		nRank	= pUser->GetSynRankShow();
// // 			int		nTargetRank	= pTargetUser->GetSynRankShow();
// // 
// // 			if(idSyn == ID_NONE || (nRank != RANK_LEADER && nRank != RANK_SUBLEADER && nRank != RANK_LEADERWIFE)
// // 				|| idTargetSyn == ID_NONE || (nTargetRank != RANK_LEADER && nTargetRank != RANK_SUBLEADER && nRank != RANK_LEADERWIFE))
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11014));//STR_APPLY_ALLY_FAILED
// // 				return;
// // 			}
// // 
// // 			if(!pTargetUser->FetchApply(CUser::APPLY_ALLY_SYN, idUser))
// // 			{
// // 				// ����������
// // 				pUser->SetApply(CUser::APPLY_ALLY_SYN, m_pInfo->idTarget);
// // 				char tempName[_MAX_NAMESIZE] = "";
// // 				CSynPtr pThisSyn = SynManager()->QuerySyndicate(idSyn);
// // 				strcpy(tempName,pThisSyn->GetStr(SYNDATA_NAME));
// // 				IF_OK_(Create(_APPLY_ALLY,/*szName*/tempName))
// // 					pTargetUser->SendMsg(this);
// // 				return;
// // 			}
// // 
// // 			// set ally
// //   			CSyndicate* pSyn = pUser->GetSyndicate();
// // 			CHECK(pSyn);
// // //			CSyndicate* pTargetSyn = pTargetUser->GetSyndicate();
// // 			CHECK(pTargetSyn);
// // 
// // 			// �Ƚ���жԹ�ϵ
// // 			pSyn->ClearEnemySyn(pTargetSyn);
// // 			pTargetSyn->ClearEnemySyn(pSyn);
// // 
// // 			int nIdx = -1;
// 
// // 			if(nIdx == -1)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11098));//STR_ALLY_FULL
// // 				return ;
// // 			}
// // 			int nIdx2 = -1;
// //
// // 			if(nIdx2 == -1)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(12010));//STR_HIS_ALLY_FULL
// // 				return ;
// // 			}
// 
// // 			MSGBUF szMsg;//STR_SYN_ALLY_s
// // 			sprintf(szMsg, g_objStrRes.GetStr(11043), pSyn->GetStr(SYNDATA_NAME), pTargetSyn->GetStr(SYNDATA_NAME));
// // 			pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 			pTargetSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 
// // 			
// // 			// synchro npc server
// // 			CMsgSyndicate	msg;
// // 			IF_OK(msg.Create(SET_ALLY, pTargetSyn->GetID()))
// // 			MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(pSyn->GetID(), &msg);			
// // 		}
// // 		break;
// // 	case _CLEAR_ALLY: //�ͻ���������� �������
// // 		{
// // 			// ������
// // 			OBJID idSyn = pUser->QuerySynAttr()->GetSynID();
// // 			int	nRankShow	= pUser->QuerySynAttr()->GetSynRankShow();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			if(nRankShow != RANK_LEADER && nRankShow != RANK_LEADERWIFE && nRankShow != RANK_SUBLEADER)
// // 			{
// // 				pUser->SendSysMsg("��Ȩ������!!");
// // 			}
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			CHECK(pSyn);
// // 
// // 			CSyndicate* pTargetSyn = SynManager()->QuerySynByName(szName);
// // 			if (!pTargetSyn)
// // 				return ;
// // 
// // 			pSyn->ClearAllySyn(pTargetSyn);
// // 
// // 			MSGBUF szMsg;
// // 			sprintf(szMsg,g_objStrRes.GetStr(11044) , szName);//STR_SYN_CLEAR_ALLY1_s
// // 			pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 
// // 			sprintf(szMsg, g_objStrRes.GetStr(11045), pSyn->GetStr(SYNDATA_NAME));//STR_SYN_CLEAR_ALLY2_s
// // 			pTargetSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 		}	
// // 		break;		
// // 	case _SET_ANTAGONIZE:	// ��ӵжԾ���
// // 		{
// // 			// ������
// // 			OBJID idSyn = pUser->QuerySynAttr()->GetSynID();
// // 			int nRankShow = pUser->QuerySynAttr()->GetSynRankShow();
// // 			if (idSyn == ID_NONE || (nRankShow != RANK_LEADER && nRankShow != RANK_SUBLEADER && nRankShow != RANK_LEADERWIFE))
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			CHECK(pSyn);
// // 
// // 			// ������
// // 			CSyndicate* pTargetSyn = SynManager()->QuerySynByName(szName);
// // 			if (!pTargetSyn)
// // 			{
// // 				pUser->SendSysMsg("������ִ���!");
// // 				return;
// // 			}
// // 
// // 			// �Ƚ��ͬ�˹�ϵ
// // 			pSyn->ClearAllySyn(pTargetSyn);
// // 			pTargetSyn->ClearAllySyn(pSyn);
// // 
// // 			if((!pTargetSyn->IsMasterSyn()) || pTargetSyn->GetID() == idSyn)
// // 				return ;
// // 			
// 	
// // 			CMsgSyndicate	msg;
// // 			IF_OK(msg.Create(SET_ANTAGONIZE, pTargetSyn->GetID()))
// // 				MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(pSyn->GetID(), &msg);
// // 		}
// // 		break;
// // 	case _CLEAR_ANTAGONIZE:	// ɾ���жԾ���
// // 		{
// // 			// ������
// // 			OBJID idSyn = pUser->QuerySynAttr()->GetSynID();
// // 			int nRankShow = pUser->QuerySynAttr()->GetSynRankShow();
// // 			if (idSyn == ID_NONE || (nRankShow != RANK_LEADER && nRankShow != RANK_SUBLEADER && nRankShow != RANK_LEADERWIFE ))
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			CHECK(pSyn);
// // 
// // 			CSyndicate* pTargetSyn = SynManager()->QuerySynByName(szName);
// // 			if (!pTargetSyn)
// // 				return;
// // 
// // 			pSyn->ClearEnemySyn(pTargetSyn);
// // 
// // 			MSGBUF szMsg;
// // 			sprintf(szMsg, g_objStrRes.GetStr(11047), pSyn->GetStr(SYNDATA_NAME));//STR_SYN_CLEAR_ENEMY_s
// // 			pTargetSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 			sprintf(szMsg, "���������%s���ͽ�!", pTargetSyn->GetStr(SYNDATA_NAME));//STR_SYN_CLEAR_ENEMY_s
// // 			pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 		}	
// // 		break;
// // 	case SYN_PAYOFF://20070413����:������
// // 		{
// // 			// ������
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			// ���Ȩ��
// // 			if(pUser->GetSynRank() != RANK_LEADER)
// // 				return ;
// // 
// // 
// // 			if(!pSyn->ReqPayOff(m_pInfo->idTarget))
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(15090));//"����:������ʧ��"
// // 			}
// // 		}
// // 		break;
// // 	default:
// // 		ASSERT(!"switch MsgSyndicate");
// // 		return;
// // 	}
// // 
// // 	}
// // 	catch(...)
// // 	{	
// // 		::LogSave("MsgSyndicate error: %d,%d,%d",m_pInfo->usAction,m_pInfo->idTarget,m_pInfo->dwData);//20061025����.������.�Լ��ֶ����⿪����
// // 	}
// //}

//--------------------------------------------------
//20070403����:��������Ŀ������
BOOL CMsgSyndicate::Create(unsigned short usAction, OBJID idTarget, const char *szTargetName)
{
	// check param
	if(usAction == ACTION_NONE) //  || idTarget == ID_NONE
	{
//		ASSERT(!"CMsgSyndicate::Create()");
		return false;
	}

	// init
	this->Init();

	m_pInfo->usAction	=usAction;
	m_pInfo->idTarget	=idTarget;

	m_StrPacker.AddString( szTargetName );
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
	m_unMsgType	=_MSG_SYNDICATE;


	return true;
}
